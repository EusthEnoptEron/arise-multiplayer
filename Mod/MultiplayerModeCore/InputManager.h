#pragma once
#include "steam/isteaminput.h"
#include "Mod/Mod.h"
#include "steam/steam_api.h"
#include "VMTHook.h"
#include <vector>

typedef int(__thiscall* GetConnectedControllers_t)(ISteamInput*, STEAM_OUT_ARRAY_COUNT(STEAM_INPUT_MAX_COUNT, Receives list of connected controllers) InputHandle_t* handlesOut);
typedef void(__thiscall* ActivateActionSet_t)(ISteamInput*, InputHandle_t inputHandle, InputActionSetHandle_t);

// Signature differs from interface for some reason. See https://github.com/GloriousEggroll/lsteamclient/blob/master/winISteamInput.c
typedef InputAnalogActionData_t *(__thiscall* GetAnalogActionData_t)(ISteamInput*, InputAnalogActionData_t*, InputHandle_t, InputAnalogActionHandle_t);

struct GamepadState {
	InputHandle_t Handle;

	bool IsConnected;
	unsigned long LastUpdated;

	bool IsCameraReset;
	bool IsMenu;
	bool IsStrikeAttack0;
	bool IsStrikeAttack1;
	bool IsStrikeAttack2;
	bool IsStrikeAttack3;
	bool IsPause;
	bool IsArts0;
	bool IsArts1;
	bool IsArts2;
	bool IsJump;
	bool IsTarget;
	bool IsSwap;
	bool IsAttack;
	bool IsGuard;

	InputAnalogActionData_t Move;
	InputAnalogActionData_t CameraAngle;
};

class InputManager
{
public:

	void Refresh(GamepadState stateArray[]);
	static InputManager* GetInstance() {
		return Instance;
	}

	static void Initialize() {
		if (Instance == NULL) {
			Instance = new InputManager();
		}
	}

	/// <summary>
	/// Temporarily swaps the first player with another player of choice.
	/// </summary>
	/// <param name="index">The index of the other player</param>
	void SetFirstPlayer(int index);
	
	int GetFirstPlayer() {
		return FirstPlayerIndex;
	}
	void SetRerouteControllers(bool reroute) {
		_rerouteControllers = reroute;
	}
	bool IsRerouteControllers() {
		return _rerouteControllers;
	}

	void SetPreventBattleInput(bool val) {
		_preventBattleInput = val;
	}

	static ISteamInput *GetInput() {
		return GetInstance()->Input;
	}
private:
	InputManager() {
		Input = SteamInput();
		GetConnectedControllers = (GetConnectedControllers_t)HookMethod((LPVOID)Input, (PVOID)GetConnectedControllersHook, 3 * 8);
		ActivateActionSet = (ActivateActionSet_t)HookMethod((LPVOID)Input, (PVOID)ActivateActionSetHook, 5 * 8);
		GetAnalogActionData = (GetAnalogActionData_t)HookMethod((LPVOID)Input, (PVOID)GetAnalogActionDataHook, 15 * 8);

		Input->Init();
		Log::Info("Input located @ %p", Input);

		AS_Battle = Input->GetActionSetHandle("Battle");

		AD_CameraReset = Input->GetDigitalActionHandle("BATTLE_CAMERA_RESET");
		AD_Menu = Input->GetDigitalActionHandle("BATTLE_MENU");
		AD_StrikeAttack0 = Input->GetDigitalActionHandle("BATTLE_STRIKE_ATTACK_0");
		AD_StrikeAttack1 = Input->GetDigitalActionHandle("BATTLE_STRIKE_ATTACK_1");
		AD_StrikeAttack2 = Input->GetDigitalActionHandle("BATTLE_STRIKE_ATTACK_2");
		AD_StrikeAttack3 = Input->GetDigitalActionHandle("BATTLE_STRIKE_ATTACK_3");
		AD_Pause = Input->GetDigitalActionHandle("BATTLE_PAUSE");
		AD_BaseArts0 = Input->GetDigitalActionHandle("BATTLE_BASE_ARTS_0");
		AD_BaseArts1 = Input->GetDigitalActionHandle("BATTLE_BASE_ARTS_1");
		AD_BaseArts2 = Input->GetDigitalActionHandle("BATTLE_BASE_ARTS_2");
		AD_BaseJump = Input->GetDigitalActionHandle("BATTLE_BASE_JUMP");
		AD_BaseTarget = Input->GetDigitalActionHandle("BATTLE_BASE_TARGET");
		AD_BaseSwap = Input->GetDigitalActionHandle("BATTLE_BASE_SWAP");
		AD_BaseAttack = Input->GetDigitalActionHandle("BATTLE_BASE_ATTACK");
		AD_BaseGuard = Input->GetDigitalActionHandle("BATTLE_BASE_GUARD");

		AA_Move = Input->GetAnalogActionHandle("BATTLE_MOVE");
		AA_CameraAngle = Input->GetAnalogActionHandle("BATTLE_CAMERA_ANGLE");
	}

	static InputManager* Instance;
	ISteamInput* Input;
	InputHandle_t InputHandles[STEAM_INPUT_MAX_COUNT];
	int InputHandleCount;
	std::vector<InputHandle_t> Controllers;
	int FirstPlayerIndex;

	unsigned long UpdateCounter = 0;

	static GetConnectedControllers_t GetConnectedControllers;
	static ActivateActionSet_t ActivateActionSet;
	static GetAnalogActionData_t GetAnalogActionData;

	static int GetConnectedControllersHook(ISteamInput* self, STEAM_OUT_ARRAY_COUNT(STEAM_INPUT_MAX_COUNT, Receives list of connected controllers) InputHandle_t* handlesOut);
	static void ActivateActionSetHook(ISteamInput* self, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle);
	static InputAnalogActionData_t* GetAnalogActionDataHook(ISteamInput* self, InputAnalogActionData_t* data,InputHandle_t inputHandle, InputAnalogActionHandle_t analogActionHandle);

	InputAnalogActionData_t GetAnalogActionDataSimple(InputHandle_t inputHandle, InputAnalogActionHandle_t analogActionHandle);
	int GetIndexForController(InputHandle_t handle);

	InputActionSetHandle_t AS_Battle;
	InputDigitalActionHandle_t AD_CameraReset;
	InputDigitalActionHandle_t AD_Menu;
	InputDigitalActionHandle_t AD_StrikeAttack0;
	InputDigitalActionHandle_t AD_StrikeAttack1;
	InputDigitalActionHandle_t AD_StrikeAttack2;
	InputDigitalActionHandle_t AD_StrikeAttack3;
	InputDigitalActionHandle_t AD_Pause;
	InputDigitalActionHandle_t AD_BaseArts0;
	InputDigitalActionHandle_t AD_BaseArts1;
	InputDigitalActionHandle_t AD_BaseArts2;
	InputDigitalActionHandle_t AD_BaseJump;
	InputDigitalActionHandle_t AD_BaseTarget;
	InputDigitalActionHandle_t AD_BaseSwap;
	InputDigitalActionHandle_t AD_BaseAttack;
	InputDigitalActionHandle_t AD_BaseGuard;

	InputAnalogActionHandle_t AA_Move;
	InputAnalogActionHandle_t AA_CameraAngle;

	// Hack for the time being
	bool _preventBattleInput = false;
	unsigned long _lastDifferentActionSet;
	bool _rerouteControllers = false;
	float _cameraSpeedRatio = .75f;

	void RemoveObsoleteControllers();
};