#include "MultiplayerMod.h"
#include <windows.h>
#include <time.h>
#include <string>
#include <utility>
#include "InputManager.h";
#include "Utilities/MinHook.h"
#include <iostream>
#include <fstream>
#include "libloaderapi.h"
#include <math.h>
#include "SDK.h";
#include "SDK/BP_BtlCamera_classes.h";
#include "Utilities/Pattern.h";
#include <thread>

#define hasFlag(x,m) ((x&m) > 0)

//typedef void(__thiscall* ActivateActionSet_t)(ISteamInput*, InputHandle_t, InputActionSetHandle_t);
//ActivateActionSet_t origActivateActionSet;


FNativeFuncPtr *MultiplayerMod::GNatives;

FEngineLoop__Tick_Fn FEngineLoop__Tick_Orig;
void FEngineLoop__Tick_Hook(void* thisptr)
{
	FEngineLoop__Tick_Orig(thisptr);
	((MultiplayerMod*)Mod::ModRef)->Tick();
}

void MultiplayerMod::CompareDigitalStates(bool newValue, bool oldValue, bool* justPressed, bool* justReleased, const UE4::FString &name, int index) {
	if (newValue != oldValue) {
		if (newValue) {
			*justPressed = true;
			*justReleased = false;
			Log::Info("PRESSED %s", name.c_str());
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

template<typename T> T GetParam(UE4::FFrame& Stack) {
	T val = { 0 };

	auto saveCode = Stack.Code;
	int32 comp = *(Stack.Code++);
	MultiplayerMod::GNatives[comp](Stack.Object, Stack, &val);
	Stack.Code = saveCode;

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

FNativeFuncPtr GetBtlAxisValue;
void GetBtlAxisValueHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	int id = static_cast<int>(((UE4::AActor*)Context)->GetActorLocation().X);
	if (id > 0) {
		auto mod = ((MultiplayerMod *)Mod::ModRef);
		std::string nameString = GetName(Stack);

		GetBtlAxisValue(Context, Stack, result);

		if ("BATTLE_MOVE_FORWARD_BACKWARD" == nameString) {
			*((float*)result) = mod->OldStates[id].Move.y;
		}
		else if("BATTLE_MOVE_LEFT_RIGHT" == nameString) {
			*((float*)result) = mod->OldStates[id].Move.x;
		}
		else {
			Log::Info("Unknown direction: %s", nameString.c_str() );
			GetBtlAxisValue(Context, Stack, result);
		}
	}
	else {
		GetBtlAxisValue(Context, Stack, result);
	}
}

FNativeFuncPtr SetActiveCamera;
void SetActiveCameraHook(UE4::UObject* Context, UE4::FFrame& Stack, void* ret) {
	auto obj = GetParam<SDK::ABP_BtlCamera_C*>(Stack);
	auto actor = ((MultiplayerMod*)Mod::ModRef)->ModActor;

	actor->ProcessEvent(actor->GetFunction("OnBtlCamera"), &obj);

	SetActiveCamera(Context, Stack, ret);
}
//
//FNativeFuncPtr K2_ExecuteProcess;
//void K2_ExecuteProcessHook(UE4::UObject* Context, UE4::FFrame& Stack, void* ret) {
//	Log::Info("Start: %d", std::this_thread::get_id());
//
//	K2_ExecuteProcess(Context, Stack, ret);
//	Log::Info("End");
//
//};

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


FNativeFuncPtr K2_GetPlayerController;
void K2_GetPlayerControllerHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	//int id = round(((UE4::AActor*)Context)->GetActorLocation().X);
	/*if (id > 0.5) {
		Log::Info("%s [%s] (%d)", Context->GetName().c_str(), Context->GetClass()->GetName().c_str(), id);

		UE4::FFrame* frame = &Stack;
		while (frame != nullptr) {
			Log::Info("%s::%s", frame->Object->GetName().c_str(), frame->Node->GetName().c_str());
			frame = frame->PreviousFrame;
		}
		Log::Info("");
	}*/



	K2_GetPlayerController(Context, Stack, result);
}

FNativeFuncPtr ChangeAriseGameScene;
void ChangeAriseGameSceneHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	Log::Info("ChangeGameSceen %s", Stack.Node->GetName().c_str());
	uint8_t scene = GetParam<uint8_t>(Stack);
	Log::Info("Scene: %d", scene);
	ChangeAriseGameScene(Context, Stack, result);
}


// Only Called Once, if you need to hook shit, declare some global non changing values
void MultiplayerMod::InitializeMod()
{
	UE4::InitSDK();
	SetupHooks();

	auto offset = (DWORD64)GetModuleHandleW(0);
	GNatives = (FNativeFuncPtr*)((DWORD64)(offset + 0x4BC4D80));
	//Log::Info("GNatives: %p", GNatives[1]);

	//Function Arise.BtlInputExtInputProcessBase.K2_IsBtlButtonJustPressed

	UseMenuButton = true; // Allows Mod Loader To Show Button
	InputManager::Initialize();
	
	MinHook::Init();
	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlInputExtInputProcessBase.K2_GetBtlAxisValue")->GetFunction()), 
		&GetBtlAxisValueHook, &GetBtlAxisValue, "K2_GetBtlAxisValue");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlCameraLibrary.SetActiveCamera")->GetFunction()),
		&SetActiveCameraHook, &SetActiveCamera, "SetActiveCamera");
	
	//MinHook::Add((DWORD_PTR)
	//	(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlInputExtInputProcessBase.K2_IsBtlButtonJustPressed")->GetFunction()),
	//	&K2_IsBtlButtonJustPressedHook, &K2_IsBtlButtonJustPressed, "K2_IsBtlButtonJustPressed");
	//MinHook::Add((DWORD_PTR)
	//	(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlInputExtInputProcessBase.K2_IsBtlButtonJustReleased")->GetFunction()),
	//	&K2_IsBtlButtonJustReleasedHook, &K2_IsBtlButtonJustReleased, "K2_IsBtlButtonJustReleased");
	//MinHook::Add((DWORD_PTR)
	//	(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlInputExtInputProcessBase.K2_IsBtlButtonPressed")->GetFunction()),
	//	&K2_IsBtlButtonPressedHook, &K2_IsBtlButtonPressed, "K2_IsBtlButtonPressed");
	//MinHook::Add((DWORD_PTR)
	//	(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlInputExtInputProcessBase.K2_IsBtlButtonRepeated")->GetFunction()),
	//	&K2_IsBtlButtonRepeatedHook, &K2_IsBtlButtonRepeated, "K2_IsBtlButtonRepeated");

	//MinHook::Add((DWORD_PTR)
	//	(UE4::UObject::FindObject<UE4::UFunction>("Function InputExtPlugin.InputExtInputProcessBase.K2_ExecuteProcess")->GetFunction()),
	//	&K2_ExecuteProcessHook, &K2_ExecuteProcess, "K2_ExecuteProcess");
	//MinHook::Add((DWORD_PTR)
	//	(UE4::UObject::FindObject<UE4::UFunction>("Function InputExtPlugin.InputExtInputProcessBase.K2_GetPlayerController")->GetFunction()),
	//	&K2_GetPlayerControllerHook, &K2_GetPlayerController, "K2_GetPlayerController");

	/*MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function BP_AriseGamemode.BP_AriseGamemode_C.ChangeAriseGameScene")->GetFunction()),
		&ChangeAriseGameSceneHook, &ChangeAriseGameScene, "ChangeAriseGameScene");*/

	

}

void MultiplayerMod::OnAction(int index, const UE4::FString &name) {
	DigitalActionParam parm = { index, name };

	ModActor->ProcessEvent(OnActionFn, &parm);
}


void MultiplayerMod::OnActionPressed(int index, const UE4::FString &name) {
	DigitalActionParam parm = { index, name };

	ModActor->ProcessEvent(OnActionPressedFn, &parm);
}

void MultiplayerMod::OnActionReleased(int index, const UE4::FString &name) {
	DigitalActionParam parm = { index, name };

	ModActor->ProcessEvent(OnActionReleasedFn, &parm);
}

void MultiplayerMod::OnAnalogAction(int index, const UE4::FString &name, float x, float y) {
	AnalogActionParam parm = { index, name, x, y };

	ModActor->ProcessEvent(OnAnalogActionFn, &parm);
}
void MultiplayerMod::OnControllerConnected(int index) {
	Log::Info("Controller connected: %d", index);
	ModActor->ProcessEvent(OnControllerConnectedFn, &index);
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

void MultiplayerMod::InitGameState()
{
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

		OnActionFn = Actor->GetFunction("OnAction");
		OnActionPressedFn = Actor->GetFunction("OnActionPressed");
		OnActionReleasedFn = Actor->GetFunction("OnActionReleased");
		OnAnalogActionFn = Actor->GetFunction("OnAnalogAction");
		OnControllerConnectedFn = Actor->GetFunction("OnControllerConnected");
		OnControllerDisconnectedFn = Actor->GetFunction("OnControllerDisconnected");

		IsBattleSceneFn = UE4::UObject::FindObject<UE4::UFunction>("Function BP_GameFunctionLibrary.BP_GameFunctionLibrary_C.GameFunc_IsBattelScene");
		functionLibClazz = UE4::UObject::FindObject<UE4::UClass>("BlueprintGeneratedClass BP_GameFunctionLibrary.BP_GameFunctionLibrary_C");

		auto tickFn = Pattern::Find("48 8B C4 48 89 48 08 55 53 56 57 41 54 41 55 41 56 41 57 48 8D A8 78 FD FF FF");
		Log::Info("%p", tickFn);
		MinHook::Add((DWORD64)tickFn, &FEngineLoop__Tick_Hook, &FEngineLoop__Tick_Orig, "FEngineLoop__Tick_Fn");
		//Actor->CallFunctionByNameWithArguments
	}
}

void MultiplayerMod::OnModMenuButtonPressed()
{
	LogEverything = !LogEverything;
}

void MultiplayerMod::DrawImGui()
{
}

void MultiplayerMod::Tick()
{
	//ModTickParams parms = *(Frame->GetParams<ModTickParams>());

	//Log::Info("Scene: %d Menu: %d", parms.Scene, parms.IsMenuOpen);

	//InputManager::GetInstance()->SetPreventBattleInput(parms.Scene != 3);
	InputManager::GetInstance()->Refresh(NewStates);

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

		if (newState.Move.bActive)
		{
			OnAnalogAction(i, Actions::BATTLE_MOVE, newState.Move.x, newState.Move.y);
		}

		if (newState.CameraAngle.bActive)
		{
			OnAnalogAction(i, Actions::BATTLE_CAMERA_ANGLE, newState.CameraAngle.x, newState.CameraAngle.y);
		}
	}

	std::swap(OldStates, NewStates);
}