#pragma once
#include "Mod/Mod.h"
#include "steam/isteaminput.h"
#include "XInput.h"
#include <time.h>
#include "InputManager.h";

typedef void (*FNativeFuncPtr)(UE4::UObject* Context, UE4::FFrame& Stack, void *result);
struct FScriptName {
	/** Index into the Names array (used to find String portion of the string/number pair used for comparison) */
	int32		ComparisonIndex;
	/** Index into the Names array (used to find String portion of the string/number pair used for display) */
	int32		DisplayIndex;
	/** Number portion of the string/number pair (stored internally as 1 more than actual, so zero'd memory will be the default, no-instance case) */
	uint32			Number;

	std::string GetName() {
		return UE4::FName(ComparisonIndex).GetName();
	}
};


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

struct IsBattleSceneParams
{
	class UObject* __WorldContext;                                           // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
	bool   bInBattle;                                                // (Parm, OutParm, ZeroConstructor, IsPlainOldData)
};

struct ModTickParams {
	uint8_t Scene;
};


namespace Actions {
	const UE4::FString BATTLE_CAMERA_RESET = L"BATTLE_CAMERA_RESET";
	const UE4::FString BATTLE_MENU = L"BATTLE_MENU";
	const UE4::FString BATTLE_STRIKE_ATTACK_0 = L"BATTLE_STRIKE_ATTACK_0";
	const UE4::FString BATTLE_STRIKE_ATTACK_1 = L"BATTLE_STRIKE_ATTACK_1";
	const UE4::FString BATTLE_STRIKE_ATTACK_2 = L"BATTLE_STRIKE_ATTACK_2";
	const UE4::FString BATTLE_STRIKE_ATTACK_3 = L"BATTLE_STRIKE_ATTACK_3";
	const UE4::FString BATTLE_PAUSE = L"BATTLE_PAUSE";
	const UE4::FString BATTLE_BASE_ARTS_0 = L"BATTLE_BASE_ARTS_0";
	const UE4::FString BATTLE_BASE_ARTS_1 = L"BATTLE_BASE_ARTS_1";
	const UE4::FString BATTLE_BASE_ARTS_2 = L"BATTLE_BASE_ARTS_2";
	const UE4::FString BATTLE_BASE_JUMP = L"BATTLE_BASE_JUMP";
	const UE4::FString BATTLE_BASE_TARGET = L"BATTLE_BASE_TARGET";
	const UE4::FString BATTLE_BASE_SWAP = L"BATTLE_BASE_SWAP";
	const UE4::FString BATTLE_BASE_ATTACK = L"BATTLE_BASE_ATTACK";
	const UE4::FString BATTLE_BASE_GUARD = L"BATTLE_BASE_GUARD";
	
	const UE4::FString BATTLE_MOVE = L"BATTLE_MOVE";
	const UE4::FString BATTLE_CAMERA_ANGLE = L"BATTLE_CAMERA_ANGLE";
}

typedef void(*FEngineLoop__Tick_Fn)(void* thisptr);

class MultiplayerMod : public Mod
{
public:

	//Basic Mod Info
	MultiplayerMod()
	{
		ModName = "MultiplayerMod"; // Mod Name -- If Using BP ModActor, Should Be The Same Name As Your Pak
		ModVersion = "1.0.0"; // Mod Version
		ModDescription = "HAHAHAHA MOD GO BURR"; // Mod Description
		ModAuthors = "Eusthron"; // Mod Author
		ModLoaderVersion = "2.0.1";

		// Dont Touch The Internal Stuff
		ModRef = this;
		CompleteModCreation();
	}

	//Called When Internal Mod Setup is finished
	virtual void InitializeMod() override;

	//Either ProcessInternals or ProcessLocalScriptFunction which you use to communicate between your BPMod and your C++ Mod
	virtual void ProcessFunction(UE4::UObject* obj, UE4::FFrame* Frame) override;

	//InitGameState Call
	virtual void InitGameState() override;

	//Beginplay Hook of Every Actor
	virtual void BeginPlay(UE4::AActor* Actor) override;

	//PostBeginPlay of EVERY Blueprint ModActor
	virtual void PostBeginPlay(std::wstring ModActorName, UE4::AActor* Actor) override;

	virtual void OnModMenuButtonPressed() override;

	//Call ImGui Here (CALLED EVERY FRAME ON DX HOOK)
	virtual void DrawImGui() override;

	void Tick();
	void OnAction(int index, const UE4::FString &name);
	void OnActionPressed(int index, const UE4::FString &name);
	void OnActionReleased(int index, const UE4::FString &name);
	void OnAnalogAction(int index, const UE4::FString &name, float x, float y);
	void OnControllerConnected(int index);
	void OnControllerDisconnected(int index);
	bool IsBattleScene();


	static FNativeFuncPtr *GNatives;

	GamepadState OldStates[4] = { 0 };
	GamepadState NewStates[4] = { 0 };
	GamepadState JustPressed[4] = { 0 };
	GamepadState JustReleased[4] = { 0 };
	UE4::AActor* ModActor;

private:

	UE4::UFunction* OnActionFn;
	UE4::UFunction* OnActionPressedFn;
	UE4::UFunction* OnActionReleasedFn;
	UE4::UFunction* OnAnalogActionFn;
	UE4::UFunction* OnControllerConnectedFn;
	UE4::UFunction* OnControllerDisconnectedFn;
	UE4::UFunction* IsBattleSceneFn;

	bool LogEverything = false;

	time_t LastCheck;
	InputManager* Manager;

	bool Initialized = false;
	void CompareDigitalStates(bool newValue, bool oldValue, bool *justPressed, bool *justReleased, const UE4::FString &name, int index);
};