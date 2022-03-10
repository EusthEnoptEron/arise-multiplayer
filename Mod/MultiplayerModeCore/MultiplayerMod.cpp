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
#include "INI.h"
#include "Tracer.h"

#include "VibrationModule.h"
#include "ReroutingModule.h"
#include "BlueprintProxyModule.h"
#include "BoostAttackModule.h"
#include "QualityOfLifeModule.h"
#include "StepGuardModule.h"
#include "ModActorModule.h"
#include "TestWidgetModule.h"

#define hasFlag(x,m) ((x&m) > 0)

//typedef void(__thiscall* ActivateActionSet_t)(ISteamInput*, InputHandle_t, InputActionSetHandle_t);
//ActivateActionSet_t origActivateActionSet;


FNativeFuncPtr* MultiplayerMod::GNatives;
float* MultiplayerMod::GNearClippingPlane;
float MultiplayerMod::GNearOriginal;


UE4::UClass* FastGetClass(UE4::UObject* obj) {
	return (UE4::UClass*)((SDK::UObject*)(obj))->Class;
}

UE4::UStruct* FastGetSuperField(UE4::UStruct* obj) {
	return (UE4::UStruct*)((SDK::UStruct*)(obj))->SuperField;
}


bool FastIsA(UE4::UObject* obj, UE4::UClass* cmp)
{
	for (auto super = FastGetClass(obj); super; super = static_cast<UE4::UClass*>(FastGetSuperField(super)))
	{
		if (super == cmp)
		{
			return true;
		}
	}

	return false;
}

void PrintStackTrace(UE4::UObject * Context, UE4::FFrame &Stack) {
	Log::Info("%s [%s]", Context->GetName().c_str(), Context->GetClass()->GetName().c_str());

	UE4::FFrame* frame = &Stack;
	while (frame != nullptr) {
		Log::Info("%s::%s", frame->Object->GetName().c_str(), frame->Node->GetName().c_str());
		frame = frame->PreviousFrame;
	}
	Log::Info("");
}

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
// -----------------------

// #######################
//  Hooks to feed input
// #######################

APlayerController__PrePostProcessInputFn TickPlayerInput;
void TickPlayerInputHook(UE4::APlayerController* thisptr, const float DeltaTime, const bool bGamePaused) {
	auto instance = MultiplayerMod::GetInstance();

	if (thisptr != instance->Controllers[0] && instance->Controllers[0] != nullptr) {
		for (int i = 1; i < MAX_CONTROLLERS; i++) {
			if (instance->Controllers[i] == thisptr) {
				// This is a match!
				// Switch gamepad to the right one and process as usual.

				InputManager::GetInstance()->SetIndex(i);
				instance->CurrentPlayer = i;

				// Temporarily set a player because this will allow haptic feedback processing
				((SDK::APlayerController*)thisptr)->Player = ((SDK::APlayerController*)instance->Controllers[0])->Player;
				TickPlayerInput(thisptr, DeltaTime, bGamePaused);
				((SDK::APlayerController*)thisptr)->Player = nullptr;


				instance->CurrentPlayer = -1;
				InputManager::GetInstance()->SetIndex(0);
				return;
			}
		}
	}

	TickPlayerInput(thisptr, DeltaTime, bGamePaused);
}

//
// Alternatives to TickPlayerInput.
// It's generally better to hook TickPlayerInput because this will also give the possibility to deal with haptic feedback.
//
//APlayerController__PrePostProcessInputFn APlayerController__PreProcessInput;
//void APlayerController__PreProcessInputHook(UE4::APlayerController* thisptr, const float DeltaTime, const bool bGamePaused) {
//	auto instance = (MultiplayerMod*)(Mod::ModRef);
//
//	for (int i = 0; i < MAX_CONTROLLERS; i++) {
//		if (instance->Controllers[i] == thisptr) {
//			InputManager::GetInstance()->SetIndex(i);
//			instance->CurrentPlayer = i;
//			break;
//		}
//	}
//
//	APlayerController__PreProcessInput(thisptr, DeltaTime, bGamePaused);
//}
//
//APlayerController__PrePostProcessInputFn APlayerController__PostProcessInput;
//void APlayerController__PostProcessInputHook(UE4::APlayerController* thisptr, const float DeltaTime, const bool bGamePaused) {
//	APlayerController__PostProcessInput(thisptr, DeltaTime, bGamePaused);
//	InputManager::GetInstance()->SetIndex(0);
//
//	auto instance = (MultiplayerMod*)(Mod::ModRef);
//	instance->CurrentPlayer = -1;
//}

// ---------------------------

void MultiplayerMod::CompareDigitalStates(bool newValue, bool oldValue, bool* justPressed, bool* justReleased, const UE4::FString& name, int index) {
	if (newValue != oldValue) {
		if (newValue) {
			*justPressed = true;
			*justReleased = false;
			OnActionPressed(index, name);
		}
		else {
			*justReleased = true;
			*justPressed = false;
			OnActionReleased(index, name);
		}
	}
	else {
		*justReleased = false;
		*justPressed = false;
	}

	if (newValue) {
		OnAction(index, name);
	}
}

template<typename T> T GetParam(UE4::FFrame& Stack, bool progress = false) {
	T val;

	auto saveCode = Stack.Code;
	int32 comp = *(Stack.Code++);
	MultiplayerMod::GNatives[comp](Stack.Object, Stack, &val);

	if (!progress) {
		Stack.Code = saveCode;
	}

	return val;
}

std::string GetName(UE4::FFrame& Stack) {
	FScriptName name = { 0 };
	// Get axis name
	auto saveCode = Stack.Code;
	int32 comp = *(Stack.Code++);
	MultiplayerMod::GNatives[comp](Stack.Object, Stack, &name);
	Stack.Code = saveCode;

	return name.GetName();
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



FNativeFuncPtr GetBtlAxisValue;
void GetBtlAxisValueHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	std::string nameString = GetName(Stack);
	GetBtlAxisValue(Context, Stack, result);
	
	auto mod = ((MultiplayerMod*)Mod::ModRef);
	int id = mod->GetPlayerIndexFromInputProcessor((UE4::AActor *)Context);

	if (id > 0 && InputManager::GetInstance()->IsRerouteControllers()) {

		if ("BATTLE_MOVE_FORWARD_BACKWARD" == nameString) {
			*((float*)result) = mod->OldStates[id].Move.y;
		}
		else if ("BATTLE_MOVE_LEFT_RIGHT" == nameString) {
			*((float*)result) = mod->OldStates[id].Move.x;
		}
		else if ("BATTLE_TARGET_CHANGE_LEFT" == nameString) {
			*((float*)result) = max(0.0f, (-mod->OldStates[InputManager::GetInstance()->GetFirstPlayer()].Move.x));
		}
		else if ("BATTLE_TARGET_CHANGE_RIGHT" == nameString) {
			*((float*)result) = max(0.0f, (mod->OldStates[InputManager::GetInstance()->GetFirstPlayer()].Move.x));
		}
		else {
			Log::Info("Unknown direction: %s (%d)", nameString.c_str(), id);
		}
	}
	else {


	}
}




FNativeFuncPtr SetActiveCamera;
void SetActiveCameraHook(UE4::UObject* Context, UE4::FFrame& Stack, void* ret) {
	auto obj = GetParam<SDK::ABP_BtlCamera_C*>(Stack);
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

FNativeFuncPtr K2_IsBtlButtonJustPressed;
void K2_IsBtlButtonJustPressedHook(UE4::UObject* Context, UE4::FFrame& Stack, void* ret) {
	int id = round(((UE4::AActor*)Context)->GetActorLocation().X);
	if (id > 0) {
		auto name = GetName(Stack);
		Log::Info("Just Pressed: %s", name.c_str());
	}
	K2_IsBtlButtonJustPressed(Context, Stack, ret);
};

FNativeFuncPtr K2_IsBtlButtonJustReleased;
void K2_IsBtlButtonJustReleasedHook(UE4::UObject* Context, UE4::FFrame& Stack, void* ret) {
	int id = round(((UE4::AActor*)Context)->GetActorLocation().X);
	if (id > 0) {
		auto name = GetName(Stack);
		Log::Info("Just Released: %s", name.c_str());
	}
	K2_IsBtlButtonJustReleased(Context, Stack, ret);
};

FNativeFuncPtr K2_IsBtlButtonPressed;
void K2_IsBtlButtonPressedHook(UE4::UObject* Context, UE4::FFrame& Stack, void* ret) {
	int id = round(((UE4::AActor*)Context)->GetActorLocation().X);
	if (id > 0) {
		auto name = GetName(Stack);
		Log::Info("Is Button Pressed: %s", name.c_str());
	}
	K2_IsBtlButtonPressed(Context, Stack, ret);
};

FNativeFuncPtr K2_IsBtlButtonRepeated;
void K2_IsBtlButtonRepeatedHook(UE4::UObject* Context, UE4::FFrame& Stack, void* ret) {
	int id = round(((UE4::AActor*)Context)->GetActorLocation().X);
	if (id > 0) {
		auto name = GetName(Stack);
		Log::Info("Just Released: %s", name.c_str());
	}
	K2_IsBtlButtonRepeated(Context, Stack, ret);
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

	// Feed our own axis input into the game
	auto fnPointer = UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlInputExtInputProcessBase.K2_GetBtlAxisValue")->GetFunction();
	//auto fnPointer = (PBYTE)GetModuleHandle(NULL) + 0x770680;
	// 770680
	// 76C780

	MinHook::Add((DWORD_PTR)
		(fnPointer),
		&GetBtlAxisValueHook, &GetBtlAxisValue, "K2_GetBtlAxisValue");

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


	//auto addrPlayerTick = Pattern::Find("40 53 57 41 56 48 81 EC D0 00 00 00 48 8B F9 0F 29 B4 24 C0 00 00 00 48 8B 89 58 04 00 00 45 0F B6 F0 0F 28 F1");
	auto addrPlayerTick = Pattern::Find("40 53 57 41 56 48 81 EC ?? ?? ?? ?? 48 8B F9 0F 29 B4 24 ?? ?? ?? ??");
	MinHook::Add((DWORD_PTR)
		addrPlayerTick,
		&TickPlayerInputHook, &TickPlayerInput, "TickPlayerInput");

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
	Modules.emplace_back(new VibrationModule());
	Modules.emplace_back(new ReroutingModule());
	Modules.emplace_back(new BlueprintProxyModule());
	Modules.emplace_back(new QualityOfLifeModule());
	Modules.emplace_back(new StepGuardModule());
	Modules.emplace_back(new BoostAttackModule());
	Modules.emplace_back(new ModActorModule());
	Modules.emplace_back(new TestWidgetModule());
}

void MultiplayerMod::OnAction(int index, const UE4::FString& name) {
	DigitalActionParam parm = { index, name };

	ModActor->ProcessEvent(OnActionFn, &parm);
}


void MultiplayerMod::OnActionPressed(int index, const UE4::FString& name) {
	DigitalActionParam parm = { index, name };

#if ENABLE_TRACING
	Tracer::GetInstance()->OnEvent("PLAYER PRESSED: " + name.ToString() + " (" + std::to_string(index) + ")");
#endif

	ModActor->ProcessEvent(OnActionPressedFn, &parm);
}

void MultiplayerMod::OnActionReleased(int index, const UE4::FString& name) {
	DigitalActionParam parm = { index, name };

#if ENABLE_TRACING
	Tracer::GetInstance()->OnEvent("PLAYER RELEASED: " + name.ToString() + " (" + std::to_string(index) + ")");
#endif

	ModActor->ProcessEvent(OnActionReleasedFn, &parm);
}

void MultiplayerMod::OnAnalogAction(int index, const UE4::FString& name, float x, float y) {
	AnalogActionParam parm = { index, name, x, y };

	ModActor->ProcessEvent(OnAnalogActionFn, &parm);
}
void MultiplayerMod::OnControllerConnected(int index) {
	Log::Info("Controller connected: %d", index);
	ModActor->ProcessEvent(OnControllerConnectedFn, &index);
	Controllers[index] = GetController(index);
}

void MultiplayerMod::OnControllerDisconnected(int index) {
	Log::Info("Controller disconnected: %d", index);
	ModActor->ProcessEvent(OnControllerDisconnectedFn, &index);
}

UE4::UClass* functionLibClazz;
bool MultiplayerMod::IsBattleScene() {
	IsBattleSceneParams parms = { nullptr, false };
	functionLibClazz->ProcessEvent(IsBattleSceneFn, (void*)&parms);

	//Log::Info("%d", parms.bInBattle);
	return parms.bInBattle;
}

void MultiplayerMod::ProcessFunction(UE4::UObject* obj, UE4::FFrame* Frame)
{
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
	// Read "FirstPlayerIsKeyboard" from config.
	INI::PARSE_FLAGS = INI::PARSE_COMMENTS_ALL | INI::PARSE_COMMENTS_SLASH | INI::PARSE_COMMENTS_HASH;
	INI config(INI_FILE_LOCATION, true);
	
	InputManager::Initialize();

	config.select("MISC");
	InputManager::GetInstance()->SkipFirstPlayer = std::stoi(config.get("FirstPlayerIsKeyboard", "0"));
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

		OnActionFn = Actor->GetFunction("OnAction");
		OnActionPressedFn = Actor->GetFunction("OnActionPressed");
		OnActionReleasedFn = Actor->GetFunction("OnActionReleased");
		OnAnalogActionFn = Actor->GetFunction("OnAnalogAction");
		OnControllerConnectedFn = Actor->GetFunction("OnControllerConnected");
		OnControllerDisconnectedFn = Actor->GetFunction("OnControllerDisconnected");
		OnChangeFirstPlayerTemporarilyFn = Actor->GetFunction("OnChangeFirstPlayerTemporarily");
		OnRestoreFirstPlayerFn = Actor->GetFunction("OnRestoreFirstPlayer");
		GetControlledCharacterFn = Actor->GetFunction("GetControlledCharacterFn");
		OnBeforePauseFn = Actor->GetFunction("OnBeforePause");


		IsBattleSceneFn = UE4::UObject::FindObject<UE4::UFunction>("Function BP_GameFunctionLibrary.BP_GameFunctionLibrary_C.GameFunc_IsBattelScene");
		functionLibClazz = UE4::UObject::FindObject<UE4::UClass>("BlueprintGeneratedClass BP_GameFunctionLibrary.BP_GameFunctionLibrary_C");

		//Actor->CallFunctionByNameWithArguments

		if (std::ifstream(INI_FILE_LOCATION).good()) {
			Watch = new filewatch::FileWatch<std::wstring>(
				INI_FILE_LOCATION_W.c_str(),
				[this](const std::wstring& path, const filewatch::Event change_type) {
					IniDirty = true;
				}
			);
		}
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

void MultiplayerMod::RefreshIni() {
	static auto applyConfigFn = ModActor->GetFunction("OnConfigChanged");
	ApplyConfigParams parms;

	INI::PARSE_FLAGS = INI::PARSE_COMMENTS_ALL | INI::PARSE_COMMENTS_SLASH | INI::PARSE_COMMENTS_HASH;
	INI config(INI_FILE_LOCATION, true);
	
	config.select("CAMERA");
	parms.MinDistance = std::stof(config.get("MinDistance", "1500"));
	parms.MaxDistance = std::stof(config.get("MaxDistance", "3500"));
	parms.MinClip = std::stof(config.get("MinClip", "10.0"));
	parms.MaxClip = std::stof(config.get("MaxClip", "100000.0"));
	parms.ClipRatio = std::stof(config.get("ClipRatio", "0.5"));
	parms.TargetOffset = std::stof(config.get("TargetOffset", "50"));
	parms.TargetEnemies = std::stoi(config.get("TargetEnemies", "1"));
	parms.TargetHeroes = std::stoi(config.get("TargetHeroes", "1"));
	parms.ZoomInSpeed = std::stof(config.get("ZoomInSpeed", "1000"));
	parms.ZoomOutSpeed = std::stof(config.get("ZoomOutSpeed", "1000"));
	parms.ZoomInPadding = std::stof(config.get("ZoomInPadding", "0.2"));
	parms.ZoomOutPadding = std::stof(config.get("ZoomOutPadding", "0.1"));
	parms.RotateSpeedX = std::stof(config.get("RotateSpeedX", "60"));
	parms.RotateSpeedY = std::stof(config.get("RotateSpeedY", "40"));
	parms.FieldOfView = std::stof(config.get("FieldOfView", "60"));
	parms.TargetSpeed = std::stof(config.get("TargetSpeed", "0.75"));
	parms.TargetRadius = std::stof(config.get("TargetRadius", "50000"));
	parms.MinPitch = std::stof(config.get("MinPitch", "-75"));
	parms.MaxPitch = std::stof(config.get("MaxPitch", "-1"));
	parms.IgnoreDeadPlayers = std::stoi(config.get("IgnoreDeadPlayers", "1"));
	parms.UseSinglePlayerCamera = std::stoi(config.get("UseSinglePlayerCamera", "0"));
	CameraShakeScale = std::stof(config.get("CameraShakeScale", "1.0"));

	config.select("MISC");

	parms.AllowSwitchingCharasDuringBattle = std::stoi(config.get("AllowSwitchingCharasDuringBattle", "1"));
	parms.ResetCharacterAssignmentsAfterBattle = std::stoi(config.get("ResetCharacterAssignmentsAfterBattle", "1"));
	parms.TargetCursorScale = std::stof(config.get("TargetCursorScale", "0.75"));
	parms.HideDefaultCursor = std::stoi(config.get("HideDefaultCursor", "1"));

	DisableHitStop = std::stoi(config.get("DisableHitStop", "0"));

	parms.DebugMenu = std::stoi(config.get("DebugMenu", "0"));
	
	AutoChangeCharas = std::stoi(config.get("AutoChangeCharas", "0"));
	RestrictBoostAttacksToCpuAndSelf = std::stoi(config.get("RestrictBoostAttacksToCpuAndSelf", "0"));
	RestrictBoostAttacksToP1 = std::stoi(config.get("RestrictBoostAttacksToP1", "0"));

	ModActor->ProcessEvent(applyConfigFn, &parms);
}


void MultiplayerMod::Tick()
{
	if (ModActor == nullptr) return; // not yet initialized

	if (IniDirty) {
		Log::Info("Update INI");
		RefreshIni();
		IniDirty = false;
	}

	//ModTickParams parms = *(Frame->GetParams<ModTickParams>());

	//Log::Info("Scene: %d Menu: %d", parms.Scene, parms.IsMenuOpen);

	//InputManager::GetInstance()->SetPreventBattleInput(parms.Scene != 3);
	InputManager::GetInstance()->Refresh(NewStates);

	if (Controllers[0] == Controllers[1] && Controllers[0] != nullptr) {
		Log::Info("STRANGE REFERENCES");
	}

	if (InputProcesses[0] == InputProcesses[1] && InputProcesses[0] != nullptr) {
		Log::Info("STRANGE REFERENCES 2");
	}

	// Compare
	for (int i = 1; i < 4; i++) {
		const GamepadState& newState = NewStates[i];
		const GamepadState& oldState = OldStates[i];
		GamepadState& justPressed = JustPressed[i];
		GamepadState& justReleased = JustReleased[i];


		if (!newState.IsConnected) {
			if (oldState.IsConnected) {
				OnControllerDisconnected(i);
			}

			continue;
		}

		if (!oldState.IsConnected) {
			OnControllerConnected(i);
		}

		/*CurrentPlayer =*/ i;

		// Only send through this for the time being
		CompareDigitalStates(newState.IsTarget, oldState.IsTarget, &(justPressed.IsTarget), &(justReleased.IsTarget), Actions::BATTLE_BASE_TARGET_QUICK, i);

#if ENABLE_TRACING
		CompareDigitalStates(newState.IsCameraReset, oldState.IsCameraReset, &(justPressed.IsCameraReset), &(justReleased.IsCameraReset), Actions::BATTLE_CAMERA_RESET, i);
		CompareDigitalStates(newState.IsMenu, oldState.IsMenu, &(justPressed.IsMenu), &(justReleased.IsMenu), Actions::BATTLE_MENU, i);
		CompareDigitalStates(newState.IsStrikeAttack0, oldState.IsStrikeAttack0, &(justPressed.IsStrikeAttack0), &(justReleased.IsStrikeAttack0), Actions::BATTLE_STRIKE_ATTACK_0, i);
		CompareDigitalStates(newState.IsStrikeAttack1, oldState.IsStrikeAttack1, &(justPressed.IsStrikeAttack1), &(justReleased.IsStrikeAttack1), Actions::BATTLE_STRIKE_ATTACK_1, i);
		CompareDigitalStates(newState.IsStrikeAttack2, oldState.IsStrikeAttack2, &(justPressed.IsStrikeAttack2), &(justReleased.IsStrikeAttack2), Actions::BATTLE_STRIKE_ATTACK_2, i);
		CompareDigitalStates(newState.IsStrikeAttack3, oldState.IsStrikeAttack3, &(justPressed.IsStrikeAttack3), &(justReleased.IsStrikeAttack3), Actions::BATTLE_STRIKE_ATTACK_3, i);
		CompareDigitalStates(newState.IsPause, oldState.IsPause, &(justPressed.IsPause), &(justReleased.IsPause), Actions::BATTLE_PAUSE, i);
		CompareDigitalStates(newState.IsArts0, oldState.IsArts0, &(justPressed.IsArts0), &(justReleased.IsArts0), Actions::BATTLE_BASE_ARTS_0, i);
		CompareDigitalStates(newState.IsArts1, oldState.IsArts1, &(justPressed.IsArts1), &(justReleased.IsArts1), Actions::BATTLE_BASE_ARTS_1, i);
		CompareDigitalStates(newState.IsArts2, oldState.IsArts2, &(justPressed.IsArts2), &(justReleased.IsArts2), Actions::BATTLE_BASE_ARTS_2, i);
		CompareDigitalStates(newState.IsJump, oldState.IsJump, &(justPressed.IsJump), &(justReleased.IsJump), Actions::BATTLE_BASE_JUMP, i);
		CompareDigitalStates(newState.IsTarget, oldState.IsTarget, &(justPressed.IsTarget), &(justReleased.IsTarget), Actions::BATTLE_BASE_TARGET, i);
		CompareDigitalStates(newState.IsSwap, oldState.IsSwap, &(justPressed.IsSwap), &(justReleased.IsSwap), Actions::BATTLE_BASE_SWAP, i);
		CompareDigitalStates(newState.IsAttack, oldState.IsAttack, &(justPressed.IsAttack), &(justReleased.IsAttack), Actions::BATTLE_BASE_ATTACK, i);
		CompareDigitalStates(newState.IsGuard, oldState.IsGuard, &(justPressed.IsGuard), &(justReleased.IsGuard), Actions::BATTLE_BASE_GUARD, i);
#endif

		//if (newState.Move.bActive)
		//{
		//	OnAnalogAction(i, Actions::BATTLE_MOVE, newState.Move.x, newState.Move.y);
		//}

		//if (newState.CameraAngle.bActive)
		//{
		//	OnAnalogAction(i, Actions::BATTLE_CAMERA_ANGLE, newState.CameraAngle.x, newState.CameraAngle.y);
		//}

		//CurrentPlayer = -1;
	}

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

	std::swap(OldStates, NewStates);

	BP_OnCameraAngle(UE4::FVector2D(x, y));

	// Code to calculate a diff
	//if (IsMultiplayerBattle()) {
	//	for (int i = 0; i < MAX_CONTROLLERS; i++) {
	//		if (Controllers[i] != nullptr) {
	//			auto chara = (SDK::ABP_BtlCharacterBase_C*)((SDK::APlayerController*)Controllers[i])->K2_GetPawn();
	//			if (chara != nullptr) {
	//				*GetPlayerOperationFlag(chara) = true;
	//				//void* firstAddress = &(chara->ComboNextArts);
	//				//void* lastAddress = &(chara->UberGraphFrame);
	//				//uint64 arraySize = (uint64)((uint64)lastAddress - (uint64)firstAddress);

	//				//unsigned char *before = (unsigned char*) malloc(arraySize);
	//				//unsigned char *after = (unsigned char *) malloc(arraySize);

	//				//memcpy(before, firstAddress, arraySize);

	//				//chara->SetPlayerOperation(true);

	//				//memcpy(after, firstAddress, arraySize);

	//				//// Compare
	//				//for (int i = 0; i < arraySize; i++) {
	//				//	if (before[i] != after[i]) {
	//				//		Log::Info("Difference at byte #%d", i);
	//				//		Log::Info("Before: #%d", before[i]);
	//				//		Log::Info("After: #%d", after[i]);
	//				//	}
	//				//}

	//				//free(before);
	//				//free(after);

	//			}
	//		}
	//	}
	//}

	for (const auto& modl : Modules) {
		modl->Tick();
	}

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
