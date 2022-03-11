#include "InputModule.h"
#include "Tracer.h"
#include "Utils.h"


MultiplayerMod* InputModule::ModRef;


void InputModule::Initialize(MultiplayerMod* mod)
{
	ModRef = mod;
	//auto fnPointer = (PBYTE)GetModuleHandle(NULL) + 0x770680;
	// 770680
	// 76C780
	// Feed our own axis input into the game

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlInputExtInputProcessBase.K2_GetBtlAxisValue")->GetFunction()),
		&GetBtlAxisValueHook, &GetBtlAxisValue, "K2_GetBtlAxisValue");

	//auto addrPlayerTick = Pattern::Find("40 53 57 41 56 48 81 EC D0 00 00 00 48 8B F9 0F 29 B4 24 C0 00 00 00 48 8B 89 58 04 00 00 45 0F B6 F0 0F 28 F1");
	MinHook::Add((DWORD_PTR)
		Pattern::Find("40 53 57 41 56 48 81 EC ?? ?? ?? ?? 48 8B F9 0F 29 B4 24 ?? ?? ?? ??"),
		&TickPlayerInputHook, &TickPlayerInput, "TickPlayerInput");


	OnActionFn = UE4::UObject::FindObject<UE4::UFunction>("ModActor.ModActor_C.OnAction");
	OnActionPressedFn = UE4::UObject::FindObject<UE4::UFunction>("ModActor.ModActor_C.OnActionPressed");
	OnActionReleasedFn = UE4::UObject::FindObject<UE4::UFunction>("ModActor.ModActor_C.OnActionReleased");
	OnAnalogActionFn = UE4::UObject::FindObject<UE4::UFunction>("ModActor.ModActor_C.OnAnalogAction");
	OnControllerConnectedFn = UE4::UObject::FindObject<UE4::UFunction>("ModActor.ModActor_C.OnControllerConnected");
	OnControllerDisconnectedFn = UE4::UObject::FindObject<UE4::UFunction>("ModActor.ModActor_C.OnControllerDisconnected");
}

void InputModule::Tick()
{
	//if (ModRef->Controllers[0] == ModRef->Controllers[1] && ModRef->Controllers[0] != nullptr) {
	//	Log::Info("STRANGE REFERENCES");
	//}

	//if (ModRef->InputProcesses[0] == ModRef->InputProcesses[1] && ModRef->InputProcesses[0] != nullptr) {
	//	Log::Info("STRANGE REFERENCES 2");
	//}

	// Compare
	for (int i = 1; i < 4; i++) {
		const GamepadState& newState = ModRef->NewStates[i];
		const GamepadState& oldState = ModRef->OldStates[i];
		GamepadState& justPressed = ModRef->JustPressed[i];
		GamepadState& justReleased = ModRef->JustReleased[i];


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
}


FNativeFuncPtr InputModule::GetBtlAxisValue;
void InputModule::GetBtlAxisValueHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	std::string nameString = GetName(Stack);
	GetBtlAxisValue(Context, Stack, result);

	auto mod = ((MultiplayerMod*)Mod::ModRef);
	int id = mod->GetPlayerIndexFromInputProcessor((UE4::AActor*)Context);

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


APlayerController__PrePostProcessInputFn InputModule::TickPlayerInput;
void InputModule::TickPlayerInputHook(UE4::APlayerController* thisptr, const float DeltaTime, const bool bGamePaused) {
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


void InputModule::OnAction(int index, const UE4::FString& name) {
	DigitalActionParam parm = { index, name };

	ModRef->ModActor->ProcessEvent(OnActionFn, &parm);
}


void InputModule::OnActionPressed(int index, const UE4::FString& name) {
	DigitalActionParam parm = { index, name };

#if ENABLE_TRACING
	Tracer::GetInstance()->OnEvent("PLAYER PRESSED: " + name.ToString() + " (" + std::to_string(index) + ")");
#endif

	ModRef->ModActor->ProcessEvent(OnActionPressedFn, &parm);
}

void InputModule::OnActionReleased(int index, const UE4::FString& name) {
	DigitalActionParam parm = { index, name };

#if ENABLE_TRACING
	Tracer::GetInstance()->OnEvent("PLAYER RELEASED: " + name.ToString() + " (" + std::to_string(index) + ")");
#endif

	ModRef->ModActor->ProcessEvent(OnActionReleasedFn, &parm);
}

void InputModule::OnAnalogAction(int index, const UE4::FString& name, float x, float y) {
	AnalogActionParam parm = { index, name, x, y };

	ModRef->ModActor->ProcessEvent(OnAnalogActionFn, &parm);
}
void InputModule::OnControllerConnected(int index) {
	Log::Info("Controller connected: %d", index);
	ModRef->ModActor->ProcessEvent(OnControllerConnectedFn, &index);
	ModRef->Controllers[index] = ModRef->GetController(index);
}

void InputModule::OnControllerDisconnected(int index) {
	Log::Info("Controller disconnected: %d", index);
	ModRef->ModActor->ProcessEvent(OnControllerDisconnectedFn, &index);
}


void InputModule::CompareDigitalStates(bool newValue, bool oldValue, bool* justPressed, bool* justReleased, const UE4::FString& name, int index) {
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



