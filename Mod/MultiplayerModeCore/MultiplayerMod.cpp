#include "MultiplayerMod.h"
#include <windows.h>
#include <time.h>
#include <utility>
#include "InputManager.h";
#include <iostream>
#include <fstream>
#include "libloaderapi.h"
#include <math.h>
#include "Utilities/Pattern.h"
#include <thread>
#include "Tracer.h"
#include "Utils.h"

#include "ConfigModule.h"
#include "VibrationModule.h"
#include "ReroutingModule.h"
#include "BlueprintProxyModule.h"
#include "BoostAttackModule.h"
#include "QualityOfLifeModule.h"
#include "StepGuardModule.h"
#include "ModActorModule.h"
#include "TestWidgetModule.h"
#include "InputModule.h"

#define hasFlag(x,m) ((x&m) > 0)

//typedef void(__thiscall* ActivateActionSet_t)(ISteamInput*, InputHandle_t, InputActionSetHandle_t);
//ActivateActionSet_t origActivateActionSet;


FNativeFuncPtr* MultiplayerMod::GNatives;
float* MultiplayerMod::GNearClippingPlane;
float MultiplayerMod::GNearOriginal;


// #######################
//  Main tick hook
// #######################
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


//Function Arise.BtlManager.BattlePause
FNativeFuncPtr BattlePause;
void BattlePauseHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	((MultiplayerMod*)(Mod::ModRef))->OnBeforePause();
	BattlePause(Context, Stack, result);
}

// Function Arise.BtlManager.BattleResume
FNativeFuncPtr BattleResume;
void BattleResumeHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	static auto onBeforePauseFn = UE4::UObject::FindObject<UE4::UFunction>("Function ModActor.ModActor_C.OnBeforePause");

	BattleResume(Context, Stack, result);

	((MultiplayerMod*)(Mod::ModRef))->OnBeforePause();
}



//Function Arise.BtlCharacterBase.GetPlayerOperation
FNativeFuncPtr GetPlayerOperation;
void GetPlayerOperationHook(UE4::UObject* Context, UE4::FFrame& Stack, bool* result) {
	auto ctx = (SDK::ABtlCharacterBase*)Context;
	auto controller = ctx->Controller;
	auto mod = ((MultiplayerMod*)(Mod::ModRef));

	GetPlayerOperation(Context, Stack, result);

	if (controller != nullptr) {
		int charaIndex = mod->GetPlayerIndex((UE4::APlayerController*)controller);
		if (mod->CurrentPlayer >= 0 && mod->CurrentPlayer != charaIndex) {
			Log::Info("Forcing %s (%d) to NOT player (current player = %d)", controller->GetName().c_str(), charaIndex, mod->CurrentPlayer);
			*result = false;
		} else if (charaIndex > 0) {
				*result = true;
			
		}
	}
}


//Function Arise.BtlCharacterBase.IsAutoOperation
FNativeFuncPtr IsAutoOperation;
void IsAutoOperationHook(UE4::UObject* Context, UE4::FFrame& Stack, bool* result) {
	auto ctx = (SDK::ABtlCharacterBase*)Context;
	auto controller = ctx->Controller;
	auto mod = ((MultiplayerMod*)(Mod::ModRef));

	IsAutoOperation(Context, Stack, result);

	if (controller != nullptr) {
		int charaIndex = mod->GetPlayerIndex((UE4::APlayerController*)controller);
		if (mod->CurrentPlayer >= 0 && mod->CurrentPlayer != charaIndex) {
			Log::Info("Forcing %s to AUTO", controller->GetName().c_str());

			*result = true;
		} else if (charaIndex > 0) {
				*result = false;
		}
	}
}

FNativeFuncPtr SetActiveCamera;
void SetActiveCameraHook(UE4::UObject* Context, UE4::FFrame& Stack, void* ret) {
	auto obj = GetParam<SDK::ABP_BtlCamera_C*>(Stack, false);
	auto actor = ((MultiplayerMod*)Mod::ModRef)->ModActor;

	actor->ProcessEvent(actor->GetFunction("OnBtlCamera"), &obj);

	SetActiveCamera(Context, Stack, ret);
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

	// 0x42784DC
	auto nearClippingPlanePat = Pattern::Find("66 0F 6E C7 0F 5B C0 0F 2E 05 ?? ?? ?? ??");
	auto nearClippingPlaneOff = *reinterpret_cast<uint32_t*>(nearClippingPlanePat + 10);
	GNearClippingPlane = (float*)(nearClippingPlanePat + 14 + nearClippingPlaneOff);

	GNearOriginal = *GNearClippingPlane;
	Log::Info("Found near clipping plane at %p (%f)", GNearClippingPlane, *GNearClippingPlane);

	// 0x4BD9F90
	auto gnativePat = Pattern::Find("CC 80 3D ?? ?? ?? ?? 00 48 8D 15 ?? ?? ?? ?? 75 49 C6 05 ?? ?? ?? ?? 01 48 8D 05 ?? ?? ?? ??");
	auto gnativeOff = *reinterpret_cast<uint32_t*>(gnativePat + 27);
	GNatives = (FNativeFuncPtr*)(gnativePat + 31 + gnativeOff);
	Log::Info("Found GNatives at %p", GNatives);

	//Log::Info("GNatives: %p", GNatives[1]);

	//Function Arise.BtlInputExtInputProcessBase.K2_IsBtlButtonJustPressed

	UseMenuButton = true; // Allows Mod Loader To Show Button

	MinHook::Init();



	// Get battle camera as soon as possible
	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlCameraLibrary.SetActiveCamera")->GetFunction()),
		&SetActiveCameraHook, &SetActiveCamera, "SetActiveCamera");


	// ----------------------------------------------------------------------------------
	// Hook into pause functions to react faster to camera changes (might be obsolete)
	// ----------------------------------------------------------------------------------
	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlManager.BattlePause")->GetFunction()),
		&BattlePauseHook, &BattlePause, "BattlePause");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlManager.BattleResume")->GetFunction()),
		&BattleResumeHook, &BattleResume, "BattleResume");

	// -------------------------------------------------------------------------------------------------
	// Instead of using SetPlayerOperation, simply override the accessors (has less bad side-effects)
	// -------------------------------------------------------------------------------------------------
	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlCharacterBase.GetPlayerOperation")->GetFunction()),
		&GetPlayerOperationHook, &GetPlayerOperation, "GetPlayerOperation");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlCharacterBase.IsAutoOperation")->GetFunction()),
		&IsAutoOperationHook, &IsAutoOperation, "IsAutoOperation");


	//
	auto tickFn = Pattern::Find("48 8B C4 48 89 48 08 55 53 56 57 41 54 41 55 41 56 41 57 48 8D A8 78 FD FF FF");
	void* ProcessInterals = UE4::UObject::FindObject<UE4::UFunction>("Function InputExtPlugin.InputExtInputProcessBase.ReceiveBeginProcess")->GetFunction();

	MinHook::Add((DWORD64)tickFn, &FEngineLoop__Tick_Hook, &FEngineLoop__Tick_Orig, "FEngineLoop__Tick_Fn");
	MinHook::Add((DWORD_PTR)ProcessInterals,
		&ProcessInternalHook, &ProcessInternal, "ProcessInternal");

#if ENABLE_TRACING
	Tracer::GetInstance()->Hook();
#endif

	RegisterModules();

	for (const auto& modl: Modules) {
		modl->Initialize(this);
	}
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
	Modules.emplace_back(new InputModule());
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

	auto handler = BlueprintHooks.find(Stack.Node);
	if (handler != BlueprintHooks.end()) {
		// Handler will take care of it
		handler->second(Context, Stack, ret, ProcessInternal);
		return false;
	}

	static auto DerivedInputStateComponent__OnOperationUnitChanged = UE4::UObject::FindObject<UE4::UFunction>("Function BP_DerivedInputStateComponent.BP_DerivedInputStateComponent_C.OnOperationUnitChanged");
	static auto DerivedInputStateComponent__GetOwnerFn = UE4::UObject::FindObject<UE4::UFunction>("Function Engine.ActorComponent.GetOwner");
	if (Stack.Node == DerivedInputStateComponent__OnOperationUnitChanged)
	{
		// Ignore if this unit is controlled by ourselves
		UE4::AActor* owner; // InputProcess
		Stack.Object->ProcessEvent(DerivedInputStateComponent__GetOwnerFn, &owner);

		int index = GetPlayerIndexFromInputProcessor(owner);
		if (index > 0) {
			// Ignore!
			Log::Info("Ignore operationunitchanged event");
			return false;
		}

	}

	static auto Btl_Camera__SetFocusUnitCamera = UE4::UObject::FindObject<UE4::UFunction>("Function BP_BtlCamera.BP_BtlCamera_C.SetFocusUnitCamera");
	if (currentFn == Btl_Camera__SetFocusUnitCamera) {
		if (CameraFrozen) {
			Log::Info("Ignore camera");
			return false;
		}
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

void MultiplayerMod::BP_OnCameraAngle(UE4::FVector2D angle)
{
	static auto fn = ModActor->GetFunction("BP_OnCameraAngle");
	ModActor->ProcessEvent(fn, &angle);
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

	// Calculate accumulate of all camera movements and listen to change chara button
	float x = 0.0f, y = 0.0f;
	for (int i = 0; i < 4; i++) {
		auto angle = OldStates[i].CameraAngle;
		if (angle.bActive) {
			if (abs(x) < abs(angle.x)) {
				x = angle.x;
			}
			if (abs(y) < abs(angle.y)) {
				y = angle.y;
			}
		}


		if (NewStates[i].IsChangeChara && !OldStates[i].IsChangeChara) {
			ChangePartyTop(i);
		}
	}

	// Swap arrays. OldStates will now carry the current state.
	std::swap(OldStates, NewStates);

	// Update camera angle
	BP_OnCameraAngle(UE4::FVector2D(x, y));



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

bool MultiplayerMod::IsControlledCharacter(UE4::AActor *actor, bool ignoreP1) {
	if (!FastIsA(actor, (UE4::UClass*)SDK::ABtlCharacterBase::StaticClass()))
		return false;

	auto pawn = (SDK::APawn*)actor;
	auto controller = (UE4::APlayerController *)pawn->Controller;

	for (int i = 0; i < MAX_CONTROLLERS; i++) {
		if (Controllers[i] == controller) {
			return i != 0 || !ignoreP1;
		}
	}

	return false;
}
