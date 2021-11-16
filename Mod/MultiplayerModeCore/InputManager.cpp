#include "InputManager.h"
#include "Mod/Mod.h"
#include <intrin.h>

#pragma intrinsic(_ReturnAddress)

InputManager *InputManager::Instance;
GetConnectedControllers_t InputManager::GetConnectedControllers;
ActivateActionSet_t InputManager::ActivateActionSet;


int InputManager::GetIndexForController(InputHandle_t handle) {
	for (int i = 0; i < Controllers.size(); i++) {
		if (Controllers[i] == handle) return i;
	}
	Controllers.push_back(handle);

	Log::Info("Registering new controller: %p", handle);
	return Controllers.size() - 1;
}

void InputManager::Refresh(GamepadState gamepads[]) {
	InputHandleCount = GetConnectedControllers(Input, InputHandles);
	UpdateCounter++;
	//for (int j = 0; j < InputHandleCount; j++) {
	//	Log::Info("%d: %d", j, Input->GetInputTypeForHandle(InputHandles[j]));
	//}
	//Log::Info("%d", InputHandleCount);

	gamepads[0] = { 0 }; // Clear
	
	for (int i = 0; i < InputHandleCount; i++) {
		InputHandle_t handle = InputHandles[i];
		int index = GetIndexForController(handle);

		if (index == 0) continue; // Ignore first one. Is passed on to game.

		// Make sure battoru is active
		Input->ActivateActionSet(handle, AS_Battle);

		GamepadState state = { 0 };
		state.Handle = handle;
		state.IsConnected = true;
		state.LastUpdated = UpdateCounter;
		state.IsCameraReset = Input->GetDigitalActionData(handle, AD_CameraReset).bState;
		state.IsMenu = Input->GetDigitalActionData(handle, AD_Menu).bState;
		state.IsStrikeAttack0 = Input->GetDigitalActionData(handle, AD_StrikeAttack0).bState;
		state.IsStrikeAttack1 = Input->GetDigitalActionData(handle, AD_StrikeAttack1).bState;
		state.IsStrikeAttack2 = Input->GetDigitalActionData(handle, AD_StrikeAttack2).bState;
		state.IsStrikeAttack3 = Input->GetDigitalActionData(handle, AD_StrikeAttack3).bState;
		state.IsPause = Input->GetDigitalActionData(handle, AD_Pause).bState;
		state.IsArts0 = Input->GetDigitalActionData(handle, AD_BaseArts0).bState;
		state.IsArts1 = Input->GetDigitalActionData(handle, AD_BaseArts1).bState;
		state.IsArts2 = Input->GetDigitalActionData(handle, AD_BaseArts2).bState;
		state.IsJump = Input->GetDigitalActionData(handle, AD_BaseJump).bState;
		state.IsTarget = Input->GetDigitalActionData(handle, AD_BaseTarget).bState;
		state.IsSwap = Input->GetDigitalActionData(handle, AD_BaseSwap).bState;
		state.IsAttack = Input->GetDigitalActionData(handle, AD_BaseAttack).bState;
		state.IsGuard = Input->GetDigitalActionData(handle, AD_BaseGuard).bState;

		state.Move = Input->GetAnalogActionData(handle, AA_Move);
		state.CameraAngle= Input->GetAnalogActionData(handle, AA_CameraAngle);

		if (index < 4) {
			gamepads[index] = state;
		}
	}

	// Clear gamepads that weren't updated
	for (int i = 0; i < 4; i++) {
		if (gamepads[i].LastUpdated != UpdateCounter) {
			gamepads[i] = { 0 };
			gamepads[i].IsConnected = false;
		}
	}
	
	SteamAPI_ReleaseCurrentThreadMemory();
}

void InputManager::ActivateActionSetHook(ISteamInput* self, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle) {
	InputManager* instance = InputManager::GetInstance();

	if (inputHandle == instance->Controllers[0]) {
		if (actionSetHandle == instance->AS_Battle) {
			if (instance->_preventBattleInput || (instance->UpdateCounter - instance->_lastDifferentActionSet) < 2) {
				//	Log::Info("%p", _ReturnAddress());
				return; // Abort
			}
		}
		else {
			instance->_lastDifferentActionSet = instance->UpdateCounter;
		}

	}

	ActivateActionSet(self, inputHandle, actionSetHandle);
}

int InputManager::DetourGetConnectedControllers(ISteamInput* self, STEAM_OUT_ARRAY_COUNT(STEAM_INPUT_MAX_COUNT) InputHandle_t* handlesOut)
{
	//Log::Info("[GET CONTROLLERS]");
	InputManager* instance = InputManager::GetInstance();

	// Get real handles
	instance->InputHandleCount = InputManager::GetConnectedControllers(self, instance->InputHandles);

	if (instance->Controllers.size() > 0) {
		// Return the first controller if found
		InputHandle_t firstHandle = instance->Controllers[0];

		for (int i = 0; i < instance->InputHandleCount; i++) {
			if (instance->InputHandles[i] == firstHandle) {
				handlesOut[0] = firstHandle;
				return 1;
			}
		}
	}

	return 0;
}

