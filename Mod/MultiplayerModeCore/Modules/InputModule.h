#pragma once
#include "BaseModule.h"


typedef void(*APlayerController__PrePostProcessInputFn)(UE4::APlayerController* thisptr, const float DeltaTime, const bool bGamePaused);



namespace Actions {
	const UE4::FString BATTLE_CAMERA_RESET = L"BATTLE_CAMERA_RESET";
	const UE4::FString BATTLE_MENU = L"BATTLE_MENU";
	const UE4::FString BATTLE_STRIKE_ATTACK_0 = L"BATTLE_STRIKE_ATTACK_0";
	const UE4::FString BATTLE_STRIKE_ATTACK_1 = L"BATTLE_STRIKE_ATTACK_1";
	const UE4::FString BATTLE_STRIKE_ATTACK_2 = L"BATTLE_STRIKE_ATTACK_2";
	const UE4::FString BATTLE_STRIKE_ATTACK_3 = L"BATTLE_STRIKE_ATTACK_3";
	const UE4::FString BATTLE_PAUSE = L"BATTLE_PAUSE";
	const UE4::FString BATTLE_BASE_ARTS_0 = L"BATTLE_ARTS_0";
	const UE4::FString BATTLE_BASE_ARTS_1 = L"BATTLE_ARTS_1";
	const UE4::FString BATTLE_BASE_ARTS_2 = L"BATTLE_ARTS_2";
	const UE4::FString BATTLE_BASE_JUMP = L"BATTLE_JUMP";
	const UE4::FString BATTLE_BASE_TARGET = L"BATTLE_TARGET";
	const UE4::FString BATTLE_BASE_TARGET_QUICK = L"BATTLE_QUICK_TARGET";
	const UE4::FString BATTLE_BASE_SWAP = L"BATTLE_STRIKE_SWAP";
	const UE4::FString BATTLE_BASE_ATTACK = L"BATTLE_NORMAL_ATTACK";
	const UE4::FString BATTLE_BASE_GUARD = L"BATTLE_GUARD";

	const UE4::FString BATTLE_MOVE = L"BATTLE_MOVE";
	const UE4::FString BATTLE_CAMERA_ANGLE = L"BATTLE_CAMERA_ANGLE";
}


struct DigitalActionParam
{
	int Index;
	UE4::FString Action;
};

struct AnalogActionParam
{
	int Index;
	UE4::FString Action;

	float X;
	float Y;
};


/// <summary>
/// Handles the bulk of the core input processing.
/// </summary>
class InputModule :
    public BaseModule
{

public:
    virtual void Initialize(MultiplayerMod* mod) override;
    virtual void Tick() override;
private:
    static FNativeFuncPtr GetBtlAxisValue;
    static void GetBtlAxisValueHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result);

    static APlayerController__PrePostProcessInputFn TickPlayerInput;
    static void TickPlayerInputHook(UE4::APlayerController* thisptr, const float DeltaTime, const bool bGamePaused);


	void OnAction(int index, const UE4::FString& name);
	void OnActionPressed(int index, const UE4::FString& name);
	void OnActionReleased(int index, const UE4::FString& name);
	void OnAnalogAction(int index, const UE4::FString& name, float x, float y);
	void OnControllerConnected(int index);
	void OnControllerDisconnected(int index);

	void CompareDigitalStates(bool newValue, bool oldValue, bool* justPressed, bool* justReleased, const UE4::FString& name, int index);

	UE4::UFunction* OnActionFn;
	UE4::UFunction* OnActionPressedFn;
	UE4::UFunction* OnActionReleasedFn;
	UE4::UFunction* OnAnalogActionFn;
	UE4::UFunction* OnControllerConnectedFn;
	UE4::UFunction* OnControllerDisconnectedFn;

    static MultiplayerMod* ModRef;
};

