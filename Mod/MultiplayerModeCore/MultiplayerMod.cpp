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
#include "Utilities/Pattern.h";
#include <thread>
#include "INI.h"
#include "Tracer.h"

#define hasFlag(x,m) ((x&m) > 0)

//typedef void(__thiscall* ActivateActionSet_t)(ISteamInput*, InputHandle_t, InputActionSetHandle_t);
//ActivateActionSet_t origActivateActionSet;


FNativeFuncPtr* MultiplayerMod::GNatives;


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
APlayerController__PrePostProcessInputFn APlayerController__PreProcessInput;
void APlayerController__PreProcessInputHook(UE4::APlayerController* thisptr, const float DeltaTime, const bool bGamePaused) {
	auto instance = (MultiplayerMod*)(Mod::ModRef);

	for (int i = 0; i < MAX_CONTROLLERS; i++) {
		if (instance->Controllers[i] == thisptr) {
			InputManager::GetInstance()->SetIndex(i);
			instance->CurrentPlayer = i;
			break;
		}
	}

	APlayerController__PreProcessInput(thisptr, DeltaTime, bGamePaused);
}

APlayerController__PrePostProcessInputFn APlayerController__PostProcessInput;
void APlayerController__PostProcessInputHook(UE4::APlayerController* thisptr, const float DeltaTime, const bool bGamePaused) {
	APlayerController__PostProcessInput(thisptr, DeltaTime, bGamePaused);
	InputManager::GetInstance()->SetIndex(0);

	auto instance = (MultiplayerMod*)(Mod::ModRef);
	instance->CurrentPlayer = 0;
}

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

UE4::APlayerController* FindPlayerController(const UE4::FFrame& Stack) {
	static auto derivedInputStateComponentClazz = UE4::UObject::FindClass("Class Arise.BtlDerivedInputStateComponent");
	static auto btlProcessorClazz = UE4::UObject::FindClass("Class Arise.BtlInputExtInputProcessBase");
	static auto getOwnerFn = UE4::UObject::FindObject<UE4::UFunction>("Function Engine.ActorComponent.GetOwner");
	static auto getUnitFn = UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlUnitScript.GetUnit");
	static auto unitScriptClazz = UE4::UObject::FindClass("Class Arise.BtlUnitScript");
	static auto pawnGetControllerFn = UE4::UObject::FindObject<UE4::UFunction>("Function Engine.Pawn.GetController");
	static auto btlCharacterClazz = UE4::UObject::FindClass("Class Arise.BtlCharacterBase");

	auto mod = ((MultiplayerMod*)(Mod::ModRef));

	// Search stack for a reference to our own classes
	auto frame = &Stack;
	while (frame != nullptr) {
		auto obj = frame->Object;

		if (FastIsA(obj, derivedInputStateComponentClazz)) {
			UE4::AActor* inputProcessPointer = nullptr;
			obj->ProcessEvent(getOwnerFn, &inputProcessPointer);

			return mod->GetControllerFromInputProcessor(inputProcessPointer);
		}

		if (FastIsA(obj, btlProcessorClazz)) {
			return mod->GetControllerFromInputProcessor((UE4::AActor*)obj);
		}

		if (FastIsA(obj, btlCharacterClazz)) {
			return mod->GetControllerOfCharacter((UE4::APawn*)obj);
		}

		if (FastIsA(obj, unitScriptClazz)) {
			UE4::APawn* unitPointer = nullptr;
			UE4::APlayerController* playerController = nullptr;

			obj->ProcessEvent(getUnitFn, &unitPointer);

			unitPointer->ProcessEvent(pawnGetControllerFn, &playerController);

			if (playerController != nullptr) {
				for (int i = 0; i < MAX_CONTROLLERS; i++) {
					if (mod->Controllers[i] == playerController) {
						return playerController;
					}
				}
			}

			break;
		}

		frame = frame->PreviousFrame;
	}

	return nullptr;
}


UE4::AActor* FindCharacter(const UE4::FFrame& Stack) {

	auto playerController = FindPlayerController(Stack);
	if (playerController != nullptr) {
		auto mod = ((MultiplayerMod*)(Mod::ModRef));
		int index = mod->GetPlayerIndex(playerController);
		if (index > 0) {
			return ((MultiplayerMod*)(Mod::ModRef))->GetControlledCharacter(index);
		}
	}
	return nullptr;
}

//Function Arise.BtlManager.BattlePause
FNativeFuncPtr BattlePause;
void BattlePauseHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	static auto onBeforePauseFn = UE4::UObject::FindObject<UE4::UFunction>("Function ModActor.ModActor_C.OnBeforePause");
	
	((MultiplayerMod*)(Mod::ModRef))->ModActor->ProcessEvent(onBeforePauseFn, nullptr);

	BattlePause(Context, Stack, result);
}

// Function Arise.BtlManager.BattleResume
FNativeFuncPtr BattleResume;
void BattleResumeHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	static auto onBeforePauseFn = UE4::UObject::FindObject<UE4::UFunction>("Function ModActor.ModActor_C.OnBeforePause");

	BattleResume(Context, Stack, result);

	((MultiplayerMod*)(Mod::ModRef))->ModActor->ProcessEvent(onBeforePauseFn, nullptr);
}


//Function Engine.GameplayStatics.GetPlayerController(int index) => APlayerController
FNativeFuncPtr ABtlCharacterBase__SetTemporaryTargetCharacter;
void ABtlCharacterBase__SetTemporaryTargetCharacterHook(UE4::UObject* Context, UE4::FFrame& Stack, void *result) {
	ABtlCharacterBase__SetTemporaryTargetCharacter(Context, Stack, result);

	// Replace if we have a better one
	auto mod = ((MultiplayerMod*)(Mod::ModRef));

	static auto getTargetCharacterFn = UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlCharacterBase.GetTargetCharacter");
	// 	class ABtlCharacterBase* GetTargetCharacter(bool IgnoreTemporary);
	struct params {
		bool IgnoreTemporary;
		SDK::ABtlCharacterBase* ReturnValue;
	};
	Log::Info("Whoo %s, %s, %s", Context->GetName().c_str(), Stack.Object->GetName().c_str(), Stack.Node->GetName().c_str());

	if (mod->IsSettingUpStrikeAttack) {
		Log::Info("Whee %s, %s, %s", Context->GetName().c_str(), Stack.Object->GetName().c_str(), Stack.Node->GetName().c_str());

		/*auto targetRef = (SDK::ABtlCharacterBase *)Context;
		auto states = mod->OldStates;
		for (int i = 0; i < 4; i++) {
			if (states[i].IsStrikeAttack0 || states[i].IsStrikeAttack1 || states[i].IsStrikeAttack2 || states[i].IsStrikeAttack3) {
				auto chara = mod->GetControlledCharacter(i);
				if (chara != nullptr) {

					params parms = { true, nullptr };
					chara->ProcessEvent(getTargetCharacterFn, &parms);

					if (parms.ReturnValue != nullptr) {
						targetRef->TemporaryTargetCharacter = parms.ReturnValue;
						Log::Info("Replaced target with target of %s", chara->GetName().c_str());
					}
					return;
				}
			}
		}*/
	}
}



FNativeFuncPtr GameplayStatics__GetPlayerController;
void GameplayStatics__GetPlayerControllerHook(UE4::UObject* Context, UE4::FFrame& Stack, UE4::APlayerController** result) {
	GameplayStatics__GetPlayerController(Context, Stack, result);

	// Replace if we have a better one
	auto alternative = FindPlayerController(Stack);
	if (alternative != nullptr) {
		Log::Info("Swap player controller");
		*result = alternative;
	}
}


FNativeFuncPtr GetPlayerControlledUnit;
void GetPlayerControlledUnitHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	GetPlayerControlledUnit(Context, Stack, result);
	auto mod = ((MultiplayerMod*)(Mod::ModRef));

	auto resultPointer = (UE4::AActor**)result;
	if (mod->CurrentPlayer > 0) {
		auto character = ((MultiplayerMod*)(Mod::ModRef))->GetControlledCharacter(mod->CurrentPlayer);

		if (character != nullptr) {
			Log::Info("Swap character");
			*(UE4::AActor**)result = character;
		}
		else {
			Log::Info("Nullptr on %d", character);
		}
	}
	else {
		auto charaPointer = FindCharacter(Stack);
		if (charaPointer != nullptr) {
			Log::Info("Swap character (alternative)");
			*(UE4::AActor**)result = charaPointer;
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

FNativeFuncPtr K2_GetBattleInputProcess;
void K2_GetBattleInputProcessHook(UE4::UObject* Context, UE4::FFrame& Stack, UE4::AActor** result) {
	K2_GetBattleInputProcess(Context, Stack, result);
	Log::Info("GetBattleInputProcess (%s)", Stack.Node->GetName().c_str());

	auto mod = ((MultiplayerMod*)Mod::ModRef);

	auto playerController = FindPlayerController(Stack);
	if (playerController != nullptr) {
		int index = mod->GetPlayerIndex(playerController);
		Log::Info("Candidate? %d", index);

		if (index > 0) {
			Log::Info("Replace Input Process => %d", index);
			*result = mod->InputProcesses[index];
			return;
		}
	}

	if (mod->InputProcesses[0] != nullptr && *result != mod->InputProcesses[0]) {
		*result = mod->InputProcesses[0];
	}
}

FNativeFuncPtr K2_GetBattlePCController;
void K2_GetBattlePCControllerHook(UE4::UObject* Context, UE4::FFrame& Stack, UE4::AActor** result) {
	K2_GetBattlePCController(Context, Stack, result);
	Log::Info("K2_GetBattlePCController (%s)", Stack.Node->GetName().c_str());

	auto mod = ((MultiplayerMod*)Mod::ModRef);

	auto playerController = FindPlayerController(Stack);
	if (playerController != nullptr) {
		*result = playerController;
	}

	if (mod->Controllers[0] != nullptr && *result != mod->Controllers[0]) {
		*result = mod->Controllers[0];
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
//	Log::Info("Start: %s", Stack.Node->GetName().c_str());
//
//	K2_ExecuteProcess(Context, Stack, ret);
//	Log::Info("End");
//
//};

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

FNativeFuncPtr GetPlayerOperation;
void GetPlayerOperationHook(UE4::UObject* Context, UE4::FFrame& Stack, void* ret) {

	Log::Info("GetPlayerOperation: %s (%s)", Context->GetName().c_str(), Stack.Object->GetName().c_str());
	UE4::FFrame* frame = &Stack;
	while (frame != nullptr) {
		Log::Info("%s::%s", frame->Object->GetName().c_str(), frame->Node->GetName().c_str());
		frame = frame->PreviousFrame;
	}
	Log::Info("");
	GetPlayerOperation(Context, Stack, ret);
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

	static bool hooked = false;
	if (!hooked) {
		Log::Info("Hooking PlayerController at %p", *((void**)result));
		APlayerController__PreProcessInput = (APlayerController__PrePostProcessInputFn)HookMethod((LPVOID) * ((void**)result), (PVOID)APlayerController__PreProcessInputHook, 0xA50);
		APlayerController__PostProcessInput = (APlayerController__PrePostProcessInputFn)HookMethod((LPVOID) * ((void**)result), (PVOID)APlayerController__PostProcessInputHook, 0xA58);

		hooked = true;
	}
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
	//SDK::InitSdk("Tales of Arise", GameProfile::SelectedGameProfile.GObject, GameProfile::SelectedGameProfile.GName, GameProfile::SelectedGameProfile.ProcessEvent);

	SDK::UObject::GObjects = (SDK::FUObjectArray*)GameProfile::SelectedGameProfile.GObject;
	SDK::FName::GNames = (SDK::TNameEntryArray*)GameProfile::SelectedGameProfile.GName;
	SDK::UObject::ProcessEventPtr = (SDK::ProcessEventFn)GameProfile::SelectedGameProfile.ProcessEvent;

	SetupHooks();

	auto offset = (DWORD64)GetModuleHandleW(0);
	GNatives = (FNativeFuncPtr*)((DWORD64)(offset + 0x4BC4D80));
	//Log::Info("GNatives: %p", GNatives[1]);

	//Function Arise.BtlInputExtInputProcessBase.K2_IsBtlButtonJustPressed

	UseMenuButton = true; // Allows Mod Loader To Show Button

	MinHook::Init();
	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlInputExtInputProcessBase.K2_GetBtlAxisValue")->GetFunction()),
		&GetBtlAxisValueHook, &GetBtlAxisValue, "K2_GetBtlAxisValue");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlCameraLibrary.SetActiveCamera")->GetFunction()),
		&SetActiveCameraHook, &SetActiveCamera, "SetActiveCamera");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlUnitLibrary.GetPlayerControlledUnit")->GetFunction()),
		&GetPlayerControlledUnitHook, &GetPlayerControlledUnit, "GetPlayerControlledUnit");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Engine.GameplayStatics.GetPlayerController")->GetFunction()),
		&GameplayStatics__GetPlayerControllerHook, &GameplayStatics__GetPlayerController, "GetPlayerController");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlInputExtInputProcessBase.K2_GetBattleInputProcess")->GetFunction()),
		&K2_GetBattleInputProcessHook, &K2_GetBattleInputProcess, "K2_GetBattleInputProcess");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlInputExtInputProcessBase.K2_GetBattlePCController")->GetFunction()),
		&K2_GetBattlePCControllerHook, &K2_GetBattlePCController, "K2_GetBattlePCController");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function InputExtPlugin.InputExtInputProcessBase.K2_GetPlayerController")->GetFunction()),
		&K2_GetPlayerControllerHook, &K2_GetPlayerController, "K2_GetPlayerController");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlManager.BattlePause")->GetFunction()),
		&BattlePauseHook, &BattlePause, "BattlePause");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlManager.BattleResume")->GetFunction()),
		&BattleResumeHook, &BattleResume, "BattleResume");

	//
	// BUGGY?
	/*MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlCharacterBase.SetTemporaryTargetCharacter")->GetFunction()),
		&ABtlCharacterBase__SetTemporaryTargetCharacterHook, &ABtlCharacterBase__SetTemporaryTargetCharacter, "SetTemporaryTargetCharacter");*/

	auto tickFn = Pattern::Find("48 8B C4 48 89 48 08 55 53 56 57 41 54 41 55 41 56 41 57 48 8D A8 78 FD FF FF");
	MinHook::Add((DWORD64)tickFn, &FEngineLoop__Tick_Hook, &FEngineLoop__Tick_Orig, "FEngineLoop__Tick_Fn");
	//MinHook::Add((DWORD_PTR)
	//	(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlCharacterBase.GetPlayerOperation")->GetFunction()),
	//	&GetPlayerOperationHook, &GetPlayerOperation, "GetPlayerOperation");


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

	void* ProcessInterals = UE4::UObject::FindObject<UE4::UFunction>("Function InputExtPlugin.InputExtInputProcessBase.ReceiveBeginProcess")->GetFunction();

	MinHook::Add((DWORD_PTR)ProcessInterals,
		&ProcessInternalHook, &ProcessInternal, "ProcessInternal");

#if ENABLE_TRACING
	Tracer::GetInstance()->Hook();
#endif
	/*MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function BP_AriseGamemode.BP_AriseGamemode_C.ChangeAriseGameScene")->GetFunction()),
		&ChangeAriseGameSceneHook, &ChangeAriseGameScene, "ChangeAriseGameScene");*/



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
	auto currentFn = Frame->Node;

	static auto BindUnitEvent = UE4::UObject::FindObject<UE4::UFunction>("Function BP_DerivedInputStateComponent.BP_DerivedInputStateComponent_C.BindUnitEvent");
	if (currentFn == BindUnitEvent) {
		Log::Info("%s [%s] (BindUnitEvent)", obj->GetName().c_str(), obj->GetClass()->GetName().c_str());

		UE4::FFrame* frame = Frame;
		while (frame != nullptr) {
			Log::Info("%s::%s", frame->Object->GetName().c_str(), frame->Node->GetName().c_str());
			frame = frame->PreviousFrame;
		}
		Log::Info("");
	}

	static auto Native_SetProcess = UE4::UObject::FindObject<UE4::UFunction>("Function MultiPlayerController.MultiPlayerController_C.Native_SetProcess");
	if (currentFn == Native_SetProcess) {

		int index = GetPlayerIndex((UE4::APlayerController*)(Frame->Object));
		UE4::AActor* process = *(Frame->GetParams<UE4::AActor*>());

		InputProcesses[index] = process;

		Log::Info("Set process %d to %p / %p", index, process, InputProcesses[index]);

	}

	if (Frame->Object == ModActor) {
		// Make sure that our own functions get the real values
		/*int tempCurrentPlayer = CurrentPlayer;
		CurrentPlayer = 0;*/
		static auto ModActor__OnBeginBattle = UE4::UObject::FindObject<UE4::UFunction>("Function ModActor.ModActor_C.OnBeginBattle");
		static auto ModActor__OnEndBattle = UE4::UObject::FindObject<UE4::UFunction>("Function ModActor.ModActor_C.OnEndBattle");
		static auto ModActor__Native_OnSubStateStart = UE4::UObject::FindObject<UE4::UFunction>("Function ModActor.ModActor_C.Native_OnSubStateStart");
		static auto ModActor__Native_OnSubStateEnd = UE4::UObject::FindObject<UE4::UFunction>("Function ModActor.ModActor_C.Native_OnSubStateEnd");
		static auto ModActor__Native_OnBeginChangeTarget = UE4::UObject::FindObject<UE4::UFunction>("Function ModActor.ModActor_C.Native_OnBeginChangeTarget");
		static auto ModActor__Native_OnEndChangeTarget = UE4::UObject::FindObject<UE4::UFunction>("Function ModActor.ModActor_C.Native_OnEndChangeTarget");
		static auto ModActor__Native_SetNearClippingPlane = UE4::UObject::FindObject<UE4::UFunction>("Function ModActor.ModActor_C.Native_SetNearClippingPlane");
		static auto ModActor__Native_ResetNearClippingPlane = UE4::UObject::FindObject<UE4::UFunction>("Function ModActor.ModActor_C.Native_ResetNearClippingPlane");

		if (ModActor__OnBeginBattle == nullptr) {
			Log::Error("ModActor__OnBeginBattle not found");
		}
		if (ModActor__OnEndBattle == nullptr) {
			Log::Error("ModActor__OnEndBattle not found");
		}
		if (ModActor__Native_OnSubStateStart == nullptr) {
			Log::Error("ModActor__Native_OnSubStateStart not found");
		}
		if (ModActor__Native_OnSubStateEnd == nullptr) {
			Log::Error("ModActor__Native_OnSubStateEnd not found");
		}
		if (ModActor__Native_OnBeginChangeTarget == nullptr) {
			Log::Error("ModActor__Native_OnBeginChangeTarget not found");
		}
		if (ModActor__Native_OnEndChangeTarget == nullptr) {
			Log::Error("ModActor__Native_OnEndChangeTarget not found");
		}
		if (ModActor__Native_SetNearClippingPlane == nullptr) {
			Log::Error("ModActor__Native_SetNearClippingPlane not found");
		}
		if (ModActor__Native_ResetNearClippingPlane == nullptr) {
			Log::Error("ModActor__Native_ResetNearClippingPlane not found");
		}

		if (currentFn == ModActor__OnBeginBattle) {
			InputManager::GetInstance()->SetRerouteControllers(true);

			Log::Info("On Begin Battle");

			static auto K2_GetBattleInputProcessFn = UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlInputExtInputProcessBase.K2_GetBattleInputProcess");
			static auto K2_GetBattlePCControllerFn = UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlInputExtInputProcessBase.K2_GetBattlePCController");

			// Keep reference to first player input process
			struct GetBattleInputArgs {
				UE4::UObject* WorldContext;
				UE4::AActor* Result;
			};

			GetBattleInputArgs args = { ModActor, nullptr };
			ModActor->ProcessEvent(K2_GetBattleInputProcessFn, &args);
			InputProcesses[0] = args.Result;
			Log::Info("Setting first player process: %p (%s)", InputProcesses[0], InputProcesses[0]->GetName().c_str());

			ModActor->ProcessEvent(K2_GetBattlePCControllerFn, &args);
			Controllers[0] = (UE4::APlayerController *)args.Result;
			Log::Info("Setting first player controller: %p (%s)", Controllers[0], Controllers[0]->GetName().c_str());
		}
		else if (currentFn == ModActor__OnEndBattle) {
			InputManager::GetInstance()->SetRerouteControllers(false);

			Log::Info("On End Battle");

			ResetNearClippingPlane(); // To be sure

			Controllers[0] = nullptr;
			InputProcesses[0] = nullptr;
		}
		else if (currentFn == ModActor__Native_OnSubStateStart) {
			SDK::EBattleState state = *Frame->GetParams<SDK::EBattleState>();
			if (state == SDK::EBattleState::StateMenu) {
				for (int i = 0; i < 4; i++) {
					if (OldStates[i].IsMenu) {
						Log::Info("Change first player: %d", i);
						InputManager::GetInstance()->SetFirstPlayer(i);
						if (i != 0) {
							ModActor->ProcessEvent(OnChangeFirstPlayerTemporarilyFn, &i);
						}
						break;
					}
				}
			}
		}
		else if (currentFn == ModActor__Native_OnSubStateEnd) {
			SDK::EBattleState state = *Frame->GetParams<SDK::EBattleState>();
			if (state == SDK::EBattleState::StateMenu) {
				InputManager::GetInstance()->SetFirstPlayer(0);
				ModActor->ProcessEvent(OnRestoreFirstPlayerFn, nullptr);
			}
		}
		else if (currentFn == ModActor__Native_OnBeginChangeTarget) {
			for (int i = 0; i < 4; i++) {
				if (OldStates[i].IsTarget) {
					Log::Info("Change first player: %d", i);
					InputManager::GetInstance()->SetFirstPlayer(i);
					if (i != 0) {
						ModActor->ProcessEvent(OnChangeFirstPlayerTemporarilyFn, &i);
					}
					break;
				}
			}
			//InputManager::GetInstance()->SetRerouteControllers(true);
		}
		else if (currentFn == ModActor__Native_OnEndChangeTarget) {
			InputManager::GetInstance()->SetFirstPlayer(0);
			ModActor->ProcessEvent(OnRestoreFirstPlayerFn, nullptr);
			//InputManager::GetInstance()->SetRerouteControllers(false);
		}
		else if (currentFn == ModActor__Native_SetNearClippingPlane) {
			SetNearClippingPlane(*Frame->GetParams<float>());
		}
		else if (currentFn == ModActor__Native_ResetNearClippingPlane) {
			ResetNearClippingPlane();
		}

		//CurrentPlayer = tempCurrentPlayer;
	}

	if (Frame->Node->GetName() == "LogInfo") {
		struct param {
			UE4::FString Prefix;
			UE4::FString Param;
		};

		param* parms = Frame->GetParams<param>();

		Log::Info("[Player] %s%s", parms->Prefix.IsValid() ? parms->Prefix.ToString().c_str() : "", parms->Param.IsValid() ? parms->Param.ToString().c_str() : "");
	}
}

struct JustParams {
	UE4::APawn* DmgActor;
	bool JustGuard;
};

bool MultiplayerMod::OnBeforeVirtualFunction(UE4::UObject* Context, UE4::FFrame& Stack, void* ret) {

	static auto BtlCharacterBase__JustStepProcess = UE4::UObject::FindObject<UE4::UFunction>("Function BP_BtlCharacterBase.BP_BtlCharacterBase_C.JustStepProcess");
	static auto BtlCharacterBase__JustGuardProcess = UE4::UObject::FindObject<UE4::UFunction>("Function BP_BtlCharacterBase.BP_BtlCharacterBase_C.JustGuardProcess");
	static auto DerivedInputStateComponent__OnOperationUnitChanged = UE4::UObject::FindObject<UE4::UFunction>("Function BP_DerivedInputStateComponent.BP_DerivedInputStateComponent_C.OnOperationUnitChanged");
	if (Stack.Node == BtlCharacterBase__JustStepProcess || Stack.Node == BtlCharacterBase__JustGuardProcess) {
		// Called on enemy, with our character as argument
		// void JustStepProcess(class ABtlCharacterBase* DmgActor, bool& JustStep);
		// void JustGuardProcess(class ABtlCharacterBase* DmgActor, bool& JustGuard);



		auto params = *Stack.GetParams<JustParams>();

		int index = GetPlayerIndex(GetControllerOfCharacter(params.DmgActor));

		if (index > 0) {
			int _player = CurrentPlayer;
			CurrentPlayer = index;

			Log::Info("Set player: %d", index);

			ProcessInternal(Context, Stack, ret);

			CurrentPlayer = _player;

			return false;
		}
	}
	else
		if (Stack.Node == DerivedInputStateComponent__OnOperationUnitChanged)
		{
			// Ignore if this unit is controlled by ourselves
			static auto getOwnerFn = UE4::UObject::FindObject<UE4::UFunction>("Function Engine.ActorComponent.GetOwner");
			UE4::AActor* owner; // InputProcess
			Stack.Object->ProcessEvent(getOwnerFn, &owner);

			int index = GetPlayerIndexFromInputProcessor(owner);
			if (index > 0) {
				// Ignore!
				Log::Info("Ignore operationunitchanged event");
				return false;
			}

		}
	static auto Native_GetHudVisibility = UE4::UObject::FindObject<UE4::UFunction>("Function BP_ModHelper.BP_ModHelper_C.Native_GetHudVisibility");

	if (Stack.Node == Native_GetHudVisibility) {
		struct params {
			UE4::FString Input;
			SDK::ESlateVisibility Visibility;
		};

		static auto fn = UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlFunctionLibrary.GetUIManager");

		SDK::ABattleUIManager* uiManager;
		Context->ProcessEvent(fn, &uiManager);

		static auto fn2 = UE4::UObject::FindObject<UE4::UFunction>("Function BP_BattleHudHelper.BP_BattleHudHelper_C.GetHudVisible");
		struct UBP_BattleHudHelper_C_GetHudVisible_Params
		{
			SDK::FName                                       RowName;                                                  // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
			UE4::UObject* __WorldContext;                                           // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
			SDK::FSTR_BtlHudVisible                          Result;                                                   // (Parm, OutParm)
		};


		UBP_BattleHudHelper_C_GetHudVisible_Params args = {
			uiManager->HudVisiblePresetLabel.Label,
			Context
		};

		Context->ProcessEvent(fn2, &args);

		auto inputString = Stack.GetParams<UE4::FString>()->ToString();
		bool visible = false;

		if (inputString == "Target") {
			visible = args.Result.TargetBar_13_83206C884702689F60AAC6AF3DF8818C;
		}
		else if (inputString == "PlayerBar") {
			visible = args.Result.OperationBar_12_5EEA505A4C436902CEE0C8B6EEE41597;
		}
		else if (inputString == "ArtsHelp") {

			visible = args.Result.ArtsHelpText_15_86BBD9224BA6A4D3FB62968F8B2CC8F8;
		}
		else {
			Log::Info("Unknown: %s", inputString.c_str());
		}

		auto ret2 = ((FOutParmRec*)Stack.OutParms)->PropAddr;
		
		// I have no idea why this isn't just ret
		*ret2 = static_cast<uint8_t>(visible ? SDK::ESlateVisibility::Visible : SDK::ESlateVisibility::Hidden);
		return false;
	}
	//static auto BP_BtlCharacterBase__UseStrikeResource = UE4::UObject::FindObject<UE4::UFunction>("Function BP_BtlCharacterBase.BP_BtlCharacterBase_C.UseStrikeResource");
	//if (Stack.Node == BP_BtlCharacterBase__UseStrikeResource) {
	//	IsSettingUpStrikeAttack = true;
	//	Log::Info("Preparing for strike attack");
	//	ProcessInternal(Context, Stack, ret);
	//	Log::Info("Preparing for strike attack done");

	//	IsSettingUpStrikeAttack = false;
	//	return false;
	//}
	//static auto BP_BattleHudHelper__GetHudVisible = UE4::UObject::FindObject<UE4::UFunction>("Function BP_BattleHudHelper.BP_BattleHudHelper_C.GetHudVisible");
	//if (Stack.Node == BP_BattleHudHelper__GetHudVisible) {
	//	Log::Info("GetHUDVisibility");
	//	auto name = *Stack.GetParams<UE4::FName>();
	//	Log::Info("GetHUDVisibility(%s)", name.GetName().c_str());
	//	return true;
	//}
	//


	return true;

}

void MultiplayerMod::OnAfterVirtualFunction(UE4::UObject* Context, UE4::FFrame& Stack, void* ret) {

	
}

// FF FF FF FF ?? ?? ?? ?? 84 3C EB F0 F7 7F 00 00 20 46 21 F1
static float* GNearClippingPlane = (float*)((DWORD64)GetModuleHandleW(0) + 0x42644E4);
static float GNearOriginal = *GNearClippingPlane;

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

		IsBattleSceneFn = UE4::UObject::FindObject<UE4::UFunction>("Function BP_GameFunctionLibrary.BP_GameFunctionLibrary_C.GameFunc_IsBattelScene");
		functionLibClazz = UE4::UObject::FindObject<UE4::UClass>("BlueprintGeneratedClass BP_GameFunctionLibrary.BP_GameFunctionLibrary_C");

		//Actor->CallFunctionByNameWithArguments

		Watch = new filewatch::FileWatch<std::wstring>(
			L"./MultiplayerMod.ini",
			[this](const std::wstring& path, const filewatch::Event change_type) {
			IniDirty = true;
		}
		);
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
	return 0;
}

void MultiplayerMod::RefreshIni() {
	static auto applyConfigFn = ModActor->GetFunction("OnConfigChanged");
	ApplyConfigParams parms;

	INI::PARSE_FLAGS = INI::PARSE_COMMENTS_ALL | INI::PARSE_COMMENTS_SLASH | INI::PARSE_COMMENTS_HASH;
	INI config("MultiplayerMod.ini", true);

	config.select("CAMERA");
	parms.MinDistance = std::stof(config.get("MinDistance", "1500"));
	parms.MaxDistance = std::stof(config.get("MaxDistance", "3000"));
	parms.MinClip = std::stof(config.get("MinClip", "10.0"));
	parms.MaxClip = std::stof(config.get("MaxClip", "100000.0"));
	parms.ClipRatio = std::stof(config.get("ClipRatio", "0.5"));
	parms.TargetOffset = std::stof(config.get("TargetOffset", "500"));
	parms.TargetEnemies = std::stoi(config.get("TargetEnemies", "1"));
	parms.TargetHeroes = std::stoi(config.get("TargetHeroes", "1"));
	parms.ZoomInSpeed = std::stof(config.get("ZoomInSpeed", "1000"));
	parms.ZoomOutSpeed = std::stof(config.get("ZoomOutSpeed", "1000"));
	parms.ZoomInPadding = std::stof(config.get("ZoomInPadding", "0.2"));
	parms.ZoomOutPadding = std::stof(config.get("ZoomOutPadding", "0.1"));
	parms.RotateSpeedX = std::stof(config.get("RotateSpeedX", "60"));
	parms.RotateSpeedY = std::stof(config.get("RotateSpeedY", "40"));
	parms.FieldOfView = std::stof(config.get("FieldOfView", "75"));
	parms.TargetSpeed = std::stof(config.get("TargetSpeed", "3"));
	parms.TargetRadius = std::stof(config.get("TargetRadius", "500"));

	config.select("MISC");
	parms.DebugMenu = std::stoi(config.get("DebugMenu", "0"));

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

		CurrentPlayer = i;

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

		CurrentPlayer = 0;
	}
	std::swap(OldStates, NewStates);


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
	}

	BP_OnCameraAngle(UE4::FVector2D(x, y));


	static auto modTickFn = ModActor->GetFunction("ModTick");
	ModActor->ProcessEvent(modTickFn, nullptr);
}