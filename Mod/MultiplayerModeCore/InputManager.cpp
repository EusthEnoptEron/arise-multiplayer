#include "InputManager.h"
#include <intrin.h>
#include <thread>
#include <chrono>

#pragma intrinsic(_ReturnAddress)

InputManager *InputManager::Instance;
GetConnectedControllers_t InputManager::GetConnectedControllers;
ActivateActionSet_t InputManager::ActivateActionSet;


int InputManager::GetIndexForController(InputHandle_t handle) {
	int emptySpot = -1;
	// Search for handle
	for (int i = 0; i < Controllers.size(); i++) {
		if (Controllers[i] == handle) return i;
		if (emptySpot < 0 &&Controllers[i] == 0) {
			emptySpot = i;
		}
	}

	if (emptySpot >= 0) {
		// Found empty spot
		Controllers[emptySpot] = handle;
		Log::Info("Registering new controller: %p => %d", handle, emptySpot);
		return emptySpot;
	}

	Controllers.push_back(handle);
	Log::Info("Registering new controller: %p", handle);
	return Controllers.size() - 1;
}

void InputManager::RemoveObsoleteControllers() {
	for (int i = 0; i < Controllers.size(); i++) {
		if (Controllers[i] != 0) {
			bool found = false;
			// Check if the handle is present in the connected handle array
			for (int j = 0; j < InputHandleCount; j++) {
				if (InputHandles[j] == Controllers[i]) {
					found = true;
					break;
				}
			}

			if (!found) {
				// Not found, remove
				Controllers[i] = 0;
			}
		}
	}
}

void InputManager::Refresh(GamepadState gamepads[]) {
	InputHandleCount = GetConnectedControllers(Input, InputHandles);
	UpdateCounter++;
	//for (int j = 0; j < InputHandleCount; j++) {
	//	Log::Info("%d: %d", j, Input->GetInputTypeForHandle(InputHandles[j]));
	//}
	//Log::Info("%d", InputHandleCount);

	RemoveObsoleteControllers();

	gamepads[0] = { 0 }; // Clear
	
	for (int i = 0; i < InputHandleCount; i++) {
		InputHandle_t handle = InputHandles[i];
		int index = GetIndexForController(handle);

		// Ignore first one (or its override). Is passed on to game.
		if (!(index == 0 || index == FirstPlayerIndex || !_rerouteControllers))
		{
			//Input->ActivateActionSet(handle, AS_Battle);
			// Make sure battoru is active

		}

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
		state.CameraAngle = Input->GetAnalogActionData(handle, AA_CameraAngle);
		//
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
	
	//SteamAPI_ReleaseCurrentThreadMemory();
}

void InputManager::ActivateActionSetHook(ISteamInput* self, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle) {
	InputManager* instance = InputManager::GetInstance();

	// Workaround because the custom InputProcesses seem to interfere with the activated action sets
	if (instance->Controllers.size() > 0) {
		if (inputHandle == instance->Controllers[instance->FirstPlayerIndex] || !instance->_rerouteControllers) {
			if (actionSetHandle == instance->AS_Battle) {
				if (instance->_preventBattleInput || (instance->UpdateCounter - instance->_lastDifferentActionSet) < 2) {
					return; // Abort
				}
			}
			else {
				instance->_lastDifferentActionSet = instance->UpdateCounter;
			}
		}
	}

	ActivateActionSet(self, inputHandle, actionSetHandle);
}

int InputManager::GetConnectedControllersHook(ISteamInput* self, STEAM_OUT_ARRAY_COUNT(STEAM_INPUT_MAX_COUNT) InputHandle_t* handlesOut)
{
	InputManager* instance = InputManager::GetInstance();
	if (!instance->_rerouteControllers) {
		// Return everything as is
		return GetConnectedControllers(self, handlesOut);
	}

	// Get real handles
	instance->InputHandleCount = InputManager::GetConnectedControllers(self, instance->InputHandles);

	if (instance->Controllers.size() > 0) {
		// Return the first controller if found
		InputHandle_t firstHandle = instance->Controllers[instance->ActiveIndex == 0
			? instance->FirstPlayerIndex
		    : instance->ActiveIndex];

		for (int i = 0; i < instance->InputHandleCount; i++) {
			if (instance->InputHandles[i] == firstHandle) {
				handlesOut[0] = firstHandle;
				return 1;
			}
		}
	}

	return 0;
}

void InputManager::SetFirstPlayer(int index) {
	FirstPlayerIndex = index;
}
