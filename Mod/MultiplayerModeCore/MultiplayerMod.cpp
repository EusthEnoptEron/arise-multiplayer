#include "MultiplayerMod.h"
#include <windows.h>
#include <time.h>
#include <utility>
#include "InputManager.h"
#include <iostream>
#include <fstream>
#include "libloaderapi.h"
#include <math.h>
#include "Utilities/Pattern.h"
#include <thread>
#include "Tracer.h"
#include "Utils.h"

#include "Modules/ConfigModule.h"
#include "Modules/VibrationModule.h"
#include "Modules/ReroutingModule.h"
#include "Modules/BlueprintProxyModule.h"
#include "Modules/BoostAttackModule.h"
#include "Modules/QualityOfLifeModule.h"
#include "Modules/StepGuardModule.h"
#include "Modules/ModActorModule.h"
#include "Modules/TestWidgetModule.h"
#include "Modules/ControlModeModule.h"
#include "Modules/InputModule.h"
#include "Modules/CameraModule.h"

#define hasFlag(x,m) ((x&m) > 0)

//typedef void(__thiscall* ActivateActionSet_t)(ISteamInput*, InputHandle_t, InputActionSetHandle_t);
//ActivateActionSet_t origActivateActionSet;


FNativeFuncPtr* MultiplayerMod::GNatives;
float* MultiplayerMod::GNearClippingPlane;
float MultiplayerMod::GNearOriginal;



// ###############################
// #           HOOKS             #
// ###############################

FEngineLoop__Tick_Fn FEngineLoop__Tick_Orig;
void FEngineLoop__Tick_Hook(void* thisptr)
{
#if ENABLE_TRACING
	Tracer::GetInstance()->OnBeginTick();
#endif
	((MultiplayerMod*)Mod::ModRef)->Tick();

	FEngineLoop__Tick_Orig(thisptr);
#if ENABLE_TRACING
	Tracer::GetInstance()->OnEndTick();
#endif
}


FNativeFuncPtr ProcessInternal;
void ProcessInternalHook(UE4::UObject* Context, UE4::FFrame& Stack, void* ret) {
	static auto thread_id = std::this_thread::get_id();
	auto current_thread_id = std::this_thread::get_id();

	if (current_thread_id != thread_id) {
		ProcessInternal(Context, Stack, ret);
		return;
	}

	auto mod = ((MultiplayerMod*)Mod::ModRef);
	if (mod->OnBeforeVirtualFunction(Context, Stack, ret)) {
		ProcessInternal(Context, Stack, ret);
	}
	mod->OnAfterVirtualFunction(Context, Stack, ret);
	//Log::Info("End");

};

// Only Called Once, if you need to hook shit, declare some global non changing values
void MultiplayerMod::InitializeMod()
{
	UE4::InitSDK();
	//SDK::InitSdk("Tales of Arise", GameProfile::SelectedGameProfile.GObject, GameProfile::SelectedGameProfile.GName, GameProfile::SelectedGameProfile.ProcessEvent);

	SDK::UObject::GObjects = (SDK::FUObjectArray*)GameProfile::SelectedGameProfile.GObject;
	SDK::FName::GNames = (SDK::TNameEntryArray*)GameProfile::SelectedGameProfile.GName;
	SDK::UObject::ProcessEventPtr = (SDK::ProcessEventFn)GameProfile::SelectedGameProfile.ProcessEvent;

	SetupHooks();

	// 0x42784DC, patch-2022-2: 0x3D57034
	auto nearClippingPlanePat = Pattern::Find("F3 ?? ?? ?? F3 ?? ?? ?? 83 FB 03 74 0A F3 0F 10 05 ?? ?? ?? ?? EB 05");
	uint32_t nearClippingPlanePatOffset = 17;
	auto nearClippingPlaneOff = *reinterpret_cast<uint32_t*>(nearClippingPlanePat + nearClippingPlanePatOffset);
	GNearClippingPlane = (float*)(nearClippingPlanePat + nearClippingPlanePatOffset + 4 + nearClippingPlaneOff);

	GNearOriginal = *GNearClippingPlane;
	Log::Info("Found near clipping plane at %p (%f)", GNearClippingPlane, *GNearClippingPlane);

	// 0x4BD9F90
	auto gnativePat = Pattern::Find("75 49 C6 05 ?? ?? ?? ?? ?? 48 8D 05 ?? ?? ?? ??");
	auto gnativeOff = *reinterpret_cast<uint32_t*>(gnativePat + 12);
	GNatives = (FNativeFuncPtr*)(gnativePat + 12 + 4 + gnativeOff);
	Log::Info("Found GNatives at %p", GNatives);

	//Log::Info("GNatives: %p", GNatives[1]);

	//Function Arise.BtlInputExtInputProcessBase.K2_IsBtlButtonJustPressed

	UseMenuButton = true; // Allows Mod Loader To Show Button

	MinHook::Init();

	auto tickFn = Pattern::Find("?? 48 8B C8 B2 01 E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B C8") - 0x2c;
	void* ProcessInterals = UE4::UObject::FindObject<UE4::UFunction>("Function InputExtPlugin.InputExtInputProcessBase.ReceiveBeginProcess")->GetFunction();

	MinHook::Add((DWORD64)tickFn, &FEngineLoop__Tick_Hook, &FEngineLoop__Tick_Orig, "FEngineLoop__Tick_Fn");
	MinHook::Add((DWORD_PTR)ProcessInterals,
		&ProcessInternalHook, &ProcessInternal, "ProcessInternal");

#if ENABLE_TRACING
	Tracer::GetInstance()->Hook();
#endif

	RegisterModules();

	Log::Info("Loading modules...");
	for (const auto& modl: Modules) {
		modl->Initialize(this);
	}

	Log::Info("Finished loading modules.");
}

void MultiplayerMod::RegisterModules() {
	Modules.emplace_back(new ConfigModule());
	Modules.emplace_back(new VibrationModule());
	Modules.emplace_back(new ReroutingModule());
	Modules.emplace_back(new BlueprintProxyModule());
	Modules.emplace_back(new QualityOfLifeModule());
	Modules.emplace_back(new StepGuardModule());
	Modules.emplace_back(new BoostAttackModule());
	Modules.emplace_back(new ModActorModule());
	Modules.emplace_back(new TestWidgetModule());
	Modules.emplace_back(new ControlModeModule());
	Modules.emplace_back(new InputModule());
	Modules.emplace_back(new CameraModule());
}


UE4::UClass* functionLibClazz;
bool MultiplayerMod::IsBattleScene() {
	IsBattleSceneParams parms = { nullptr, false };
	functionLibClazz->ProcessEvent(IsBattleSceneFn, (void*)&parms);

	//Log::Info("%d", parms.bInBattle);
	return parms.bInBattle;
}

// TODO: Make lookup table for all those implementations
bool MultiplayerMod::OnBeforeVirtualFunction(UE4::UObject* Context, UE4::FFrame& Stack, void* ret) {
	auto currentFn = Stack.Node;

	auto handler = BlueprintHooks.find(currentFn);
	if (handler != BlueprintHooks.end()) {
		// Handler will take care of it
		handler->second(Context, Stack, ret, ProcessInternal);
		return false;
	}

	return true;
}

void MultiplayerMod::OnAfterVirtualFunction(UE4::UObject* Context, UE4::FFrame& Stack, void* ret) {

	static auto WaitEnd = UE4::UObject::FindObject<UE4::UFunction>("Function TO14_BP_MenuManagerBase.TO14_BP_MenuManagerBase_C.WaitEnd");
	static auto onBeforePauseFn = UE4::UObject::FindObject<UE4::UFunction>("Function ModActor.ModActor_C.OnBeforePause");


	if (Stack.Node == WaitEnd) {
		ModActor->ProcessEvent(onBeforePauseFn, nullptr);
	}
	
}

void MultiplayerMod::SetNearClippingPlane(float nearPlane) {
	//Log::Info("Near Plane: %f (%p)", nearPlane, GNearClippingPlane);
	*GNearClippingPlane = nearPlane;
}

void MultiplayerMod::ResetNearClippingPlane() {
	Log::Info("Set clipping plane to: %f", GNearOriginal);
	*GNearClippingPlane = GNearOriginal;
}


void MultiplayerMod::InitGameState()
{
	InputManager::Initialize();

	InputManager::GetInstance()->SkipFirstPlayer = ConfigModule::IsSkipFirstPlayer();
	InputManager::GetInstance()->Refresh(NewStates);
}

void MultiplayerMod::BeginPlay(UE4::AActor* Actor)
{
}

void MultiplayerMod::PostBeginPlay(std::wstring ModActorName, UE4::AActor* Actor)
{
	// Filters Out All Mod Actors Not Related To Your Mod
	std::wstring TmpModName(ModName.begin(), ModName.end());
	if (ModActorName == TmpModName && !Initialized)
	{
		Initialized = true;

		//Sets ModActor Ref
		ModActor = Actor;
		//Log::Info("Initializing: %s", Actor->GetClass()->GetFullName().c_str());

		OnChangeFirstPlayerTemporarilyFn = Actor->GetFunction("OnChangeFirstPlayerTemporarily");
		OnRestoreFirstPlayerFn = Actor->GetFunction("OnRestoreFirstPlayer");
		GetControlledCharacterFn = Actor->GetFunction("GetControlledCharacterFn");
		OnBeforePauseFn = Actor->GetFunction("OnBeforePause");


		IsBattleSceneFn = UE4::UObject::FindObject<UE4::UFunction>("Function BP_GameFunctionLibrary.BP_GameFunctionLibrary_C.GameFunc_IsBattelScene");
		functionLibClazz = UE4::UObject::FindObject<UE4::UClass>("BlueprintGeneratedClass BP_GameFunctionLibrary.BP_GameFunctionLibrary_C");

		for (const auto& modl : Modules) {
			modl->PostBeginPlay(Actor);
		}

		//Actor->CallFunctionByNameWithArguments
	}
}

void MultiplayerMod::OnModMenuButtonPressed()
{
	Tracer::GetInstance()->Toggle();
	//logging = !logging;
	//LogEverything = !LogEverything;
}

void MultiplayerMod::DrawImGui()
{
}



bool MultiplayerMod::IsMultiplayerBattle() {
	static auto ModActor__IsMultiplayerBattle = UE4::UObject::FindObject<UE4::UFunction>("Function ModActor.ModActor_C.IsMultiplayerBattle");
	bool isMultiplayerBattle;
	ModActor->ProcessEvent(ModActor__IsMultiplayerBattle, &isMultiplayerBattle);

	return isMultiplayerBattle;
}



void MultiplayerMod::Tick()
{
	if (ModActor == nullptr) return; // not yet initialized

	// Update new states
	InputManager::GetInstance()->Refresh(NewStates);

	// Call modules with accurate values for OldStates and NewStates
	for (const auto& modl : Modules) {
		modl->Tick();
	}

	for (int i = 0; i < 4; i++) {
		if (NewStates[i].IsChangeChara && !OldStates[i].IsChangeChara) {
			ChangePartyTop(i);
		}
	}

	// Swap arrays. OldStates will now carry the current state.
	std::swap(OldStates, NewStates);

	static auto modTickFn = ModActor->GetFunction("ModTick");
	ModActor->ProcessEvent(modTickFn, nullptr);
}

void MultiplayerMod::ChangePartyTop(int index) {
	if (!AutoChangeCharas) return;

	Log::Info("Party top change requested.");

	auto gameMode = ((SDK::USystemFunctionLibrary*)ModActor)->STATIC_GetAriseGameMode();
	if (gameMode->GetDisplayScene() != SDK::EAriseGameScene::PlayerFree) {
		Log::Info("Only allowed in free mode, current state: %d", gameMode->GetDisplayScene());
		return;
	}

	if (!((SDK::UAriseGameDataLibrary*)ModActor)->STATIC_IsLockPartyTop()) {
		auto partyOrder = ((SDK::USystemFunctionLibrary*)ModActor)->STATIC_GetPartyOrder();

		if (partyOrder == nullptr) return;

		int charaIndex = index;

		auto controller = GetController(index);
		static auto MultiController = UE4::UObject::FindClass("BlueprintGeneratedClass MultiPlayerController.MultiPlayerController_C");


		if (controller != nullptr && FastIsA(controller, MultiController)) {
			UE4::GetVariable<int>(controller, "Index", charaIndex);
		}
		else {
			SDK::TArray<SDK::FMenuFormationData> formationData;
			((SDK::UMenuFormationSupport*)ModActor)->STATIC_MenuFormationGetData(&formationData);

			for (int i = 0; i < min(4,formationData.Num()); i++) {
				if (formationData[i].IsControl) {
					charaIndex = i;
					Log::Info("Found chara");
				}
			}
		}

		auto currentPartyTop = partyOrder->GetPartyTop();
		auto proposedPartyTop = partyOrder->GetPartyId((SDK::EPCOrder)charaIndex);
		if ((uint8_t)proposedPartyTop >= (uint8_t)SDK::EArisePartyID::MAX) {
			// Nothing to see here
			return;
		}

		Log::Info("Current top: %d, proposed top: %d", currentPartyTop, proposedPartyTop);

		if (proposedPartyTop != currentPartyTop) {
			partyOrder->SetPartyTop(proposedPartyTop);

			static auto buildPartyTop = UE4::UObject::FindObject<UE4::UFunction>("Function BP_MenuPFSupport.BP_MenuPFSupport_C.BuildPartyTop");
			ModActor->ProcessEvent(buildPartyTop, &ModActor);

			Log::Info("Set new party top: %d", proposedPartyTop);
		}
	}
}

void MultiplayerMod::OnBeforePause() {
	ModActor->ProcessEvent(OnBeforePauseFn, nullptr);
}


void MultiplayerMod::ChangeFirstPlayerTemporarily(int playerIndex)
{
	ModActor->ProcessEvent(OnChangeFirstPlayerTemporarilyFn, &playerIndex);
}


void MultiplayerMod::RestoreFirstPlayer()
{
	ModActor->ProcessEvent(OnRestoreFirstPlayerFn, nullptr);
}

void MultiplayerMod::AddBlueprintHook(std::string fnName, FBlueprintHookHandler handler)
{
	auto fn = UE4::UObject::FindObject<UE4::UFunction>("Function " + fnName);
	if (fn == nullptr) {
		Log::Error("BP function not found: %s", fnName.c_str());
		return;
	}

	AddBlueprintHook(fn, handler);
}

void MultiplayerMod::AddBlueprintHook(UE4::UFunction* fn, FBlueprintHookHandler handler)
{
	if (fn == nullptr) {
		Log::Error("BP fn pointer was NULL!");
		return;
	}

	bool success = BlueprintHooks.try_emplace(fn, handler).second;
	if (success) {
		Log::Info("Added BP hook: %s", fn->GetFullName().c_str());
	}
	else {
		Log::Error("Function handled twice: %s", fn->GetFullName().c_str());
	}
}




// ###############################
// #      PLAYER ACCESSORS       #
// ###############################

UE4::APlayerController* MultiplayerMod::GetControllerFromInputProcessor(UE4::AActor* inputProcess) {
	if (inputProcess == nullptr) return nullptr;

	return Controllers[GetPlayerIndexFromInputProcessor(inputProcess)];
}

int MultiplayerMod::GetPlayerIndexFromInputProcessor(UE4::AActor* inputProcess) {
	if (inputProcess == nullptr) return 0;

	for (int i = 0; i < MAX_CONTROLLERS; i++) {
		if (InputProcesses[i] == inputProcess) {
			return i;
		}
	}

	return 0;
}

UE4::APawn* MultiplayerMod::GetControlledCharacter(int index) {
	if (GetControlledCharacterFn == nullptr) return nullptr;

	GetControlledCharacterParms parms = { index, nullptr };
	ModActor->ProcessEvent(GetControlledCharacterFn, &parms);

	return parms.Result;
}

UE4::APlayerController* MultiplayerMod::GetController(int index) {
	static auto GetControllerFn = ModActor->GetFunction("GetMultiControllerFn");

	GetControllerParms parms = { index, nullptr };
	ModActor->ProcessEvent(GetControllerFn, &parms);

	return parms.Result;
}

SDK::ABtlInputExtInputProcessBase* MultiplayerMod::GetInputProcess(int index)
{
	if (index < 0) return nullptr;
	return (SDK::ABtlInputExtInputProcessBase*)InputProcesses[index];
}

UE4::APlayerController* MultiplayerMod::GetControllerOfCharacter(UE4::APawn* pawn)
{
	if (pawn == nullptr) return nullptr;

	static auto pawnGetControllerFn = UE4::UObject::FindObject<UE4::UFunction>("Function Engine.Pawn.GetController");

	UE4::APlayerController* playerController = nullptr;
	pawn->ProcessEvent(pawnGetControllerFn, &playerController);

	return playerController;
}

int MultiplayerMod::GetPlayerIndex(UE4::APlayerController* playerController)
{
	if (playerController == nullptr) return 0;

	for (int i = 0; i < MAX_CONTROLLERS; i++) {
		if (Controllers[i] == playerController) {
			return i;
		}
	}
	return -1;
}

bool MultiplayerMod::IsControlledCharacter(UE4::AActor* actor, bool ignoreP1) {
	if (!FastIsA(actor, (UE4::UClass*)SDK::ABtlCharacterBase::StaticClass()))
		return false;

	auto pawn = (SDK::APawn*)actor;
	auto controller = (UE4::APlayerController*)pawn->Controller;

	for (int i = 0; i < MAX_CONTROLLERS; i++) {
		if (Controllers[i] == controller) {
			return i != 0 || !ignoreP1;
		}
	}

	return false;
}