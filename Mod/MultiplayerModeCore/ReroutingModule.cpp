#include "ReroutingModule.h"
#include <Utilities/MinHook.h>

FNativeFuncPtr ReroutingModule::GetPlayerControlledUnit;
void ReroutingModule::GetPlayerControlledUnitHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	GetPlayerControlledUnit(Context, Stack, result);
	auto charaPointer = FindCharacter(Stack);
	if (charaPointer != nullptr) {
		Log::Info("Swap character (alternative)");
		*(UE4::AActor**)result = charaPointer;
	}
	//else if(mod->Controllers[0] != nullptr) {
	//	*(SDK::AActor**)result =((SDK::APlayerController*)mod->Controllers[0])->Pawn;
	//}
}


FNativeFuncPtr ReroutingModule::GameplayStatics__GetPlayerController;
void ReroutingModule::GameplayStatics__GetPlayerControllerHook(UE4::UObject* Context, UE4::FFrame& Stack, UE4::APlayerController** result) {
	GameplayStatics__GetPlayerController(Context, Stack, result);

	// Replace if we have a better one
	auto alternative = FindPlayerController(Stack);
	if (alternative != nullptr) {
		Log::Info("Swap player controller");
		*result = alternative;
	}
}

FNativeFuncPtr ReroutingModule::K2_GetBattleInputProcess;
void ReroutingModule::K2_GetBattleInputProcessHook(UE4::UObject* Context, UE4::FFrame& Stack, UE4::AActor** result) {
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
		Log::Info("Wrong input process -- fixing it!");
		auto battleManager = ((SDK::UBtlFunctionLibrary*)Stack.Object)->STATIC_GetBattleManager((SDK::UObject*)Stack.Object);
		if (battleManager != nullptr) {
			battleManager->BattleInputProcess = (SDK::ABtlInputExtInputProcessBase*)mod->InputProcesses[0];
		}
		else {
			Log::Info("Unable to acquire battle manager.");
		}

		*result = mod->InputProcesses[0];
	}
}


FNativeFuncPtr ReroutingModule::K2_GetBattlePCController;
void ReroutingModule::K2_GetBattlePCControllerHook(UE4::UObject* Context, UE4::FFrame& Stack, UE4::AActor** result) {
	K2_GetBattlePCController(Context, Stack, result);
	Log::Info("K2_GetBattlePCController (%s)", Stack.Node->GetName().c_str());

	auto mod = ((MultiplayerMod*)Mod::ModRef);

	auto playerController = FindPlayerController(Stack);
	if (playerController != nullptr) {
		*result = playerController;
	}

	if (mod->Controllers[0] != nullptr && *result != mod->Controllers[0]) {
		Log::Info("WRONG PLAYER CONTROLLER: " + (*result)->GetName());
		*result = mod->Controllers[0];
	}
}



UE4::APlayerController* ReroutingModule::FindPlayerController(const UE4::FFrame& Stack) {
	auto mod = ((MultiplayerMod*)(Mod::ModRef));

	if (mod->CurrentPlayer >= 0) {
		return mod->GetController(mod->CurrentPlayer);
	}

	static auto derivedInputStateComponentClazz = UE4::UObject::FindClass("Class Arise.BtlDerivedInputStateComponent");
	static auto btlProcessorClazz = UE4::UObject::FindClass("Class Arise.BtlInputExtInputProcessBase");
	static auto inputExtPlayerController = UE4::UObject::FindClass("Class InputExtPlugin.InputExtPlayerController");
	static auto getOwnerFn = UE4::UObject::FindObject<UE4::UFunction>("Function Engine.ActorComponent.GetOwner");
	static auto getUnitFn = UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlUnitScript.GetUnit");
	static auto unitScriptClazz = UE4::UObject::FindClass("Class Arise.BtlUnitScript");
	static auto pawnGetControllerFn = UE4::UObject::FindObject<UE4::UFunction>("Function Engine.Pawn.GetController");
	static auto btlCharacterClazz = UE4::UObject::FindClass("Class Arise.BtlCharacterBase");

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

		if (FastIsA(obj, inputExtPlayerController)) {
			return (UE4::APlayerController*)obj;
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


UE4::AActor* ReroutingModule::FindCharacter(const UE4::FFrame& Stack) {

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



void ReroutingModule::Initialize(MultiplayerMod* mod)
{
	// -------------------------------------------------------------------------
	// Override static accessors to return P2-P4 player controllers if needed
	// ------------------------------------------------------------------------
	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlUnitLibrary.GetPlayerControlledUnit")->GetFunction()),
		&ReroutingModule::GetPlayerControlledUnitHook, &ReroutingModule::GetPlayerControlledUnit, "GetPlayerControlledUnit");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Engine.GameplayStatics.GetPlayerController")->GetFunction()),
		&ReroutingModule::GameplayStatics__GetPlayerControllerHook, &ReroutingModule::GameplayStatics__GetPlayerController, "GetPlayerController");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlInputExtInputProcessBase.K2_GetBattleInputProcess")->GetFunction()),
		&ReroutingModule::K2_GetBattleInputProcessHook, &ReroutingModule::K2_GetBattleInputProcess, "K2_GetBattleInputProcess");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlInputExtInputProcessBase.K2_GetBattlePCController")->GetFunction()),
		&ReroutingModule::K2_GetBattlePCControllerHook, &ReroutingModule::K2_GetBattlePCController, "K2_GetBattlePCController");
}
