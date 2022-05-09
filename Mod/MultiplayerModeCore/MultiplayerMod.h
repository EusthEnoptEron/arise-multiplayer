#pragma once
#include "iostream"
#include <string>
#include "Mod/Mod.h"
#include "steam/isteaminput.h"
#include "InputManager.h"
#include "SDK.h"
#include "SDK/BP_BtlCamera_classes.h"
#include "SDK/STR_BtlHudVisible_structs.h"
#include "SDK/TO14_StateEnum_MNU_SAV_structs.h"
#include "SDK/TO14_ProcessEnum_MNU_SAV_structs.h"
#include "SDK/TO14_ModeEnum_MNU_SAV_structs.h"
#include "SDK/TO14_BPI_GUI_MNU_SAV_classes.h"
#include "SDK/BP_BtlCharacterBase_structs.h"
#include "SDK/BP_BtlCharacterBase_classes.h"
#include "FileWatch.hpp"
#include "Utilities/MinHook.h"

typedef  void (*FNativeFuncPtr)(UE4::UObject* Context, UE4::FFrame& Stack, void* result);
typedef  float ( *FGetBtlAxisValue)(SDK::ABtlInputExtInputProcessBase* thisPtr, const UE4::FName InAxisName);
typedef  void (*FNativeFuncPtr)(UE4::UObject* Context, UE4::FFrame& Stack, void* result);
typedef  void (*FBlueprintHookHandler)(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);


struct FOutParmRec
{
	UE4::UField* Property;
	uint8_t* PropAddr;
	FOutParmRec* NextOutParm;
};

struct IsBattleSceneParams
{
	class UObject* __WorldContext;                                           // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
	bool   bInBattle;                                                // (Parm, OutParm, ZeroConstructor, IsPlainOldData)
};

struct ModTickParams {
	uint8_t Scene;
};

struct GetControlledCharacterParms {
	int Index;
	UE4::APawn* Result;
};

struct GetControllerParms {
	int Index;
	UE4::APlayerController* Result;
};

const int MAX_CONTROLLERS = 10;

typedef void(*FEngineLoop__Tick_Fn)(void* thisptr);
typedef void(*APlayerController__PlayerTickFn)(UE4::APlayerController* thisptr, const float DeltaTime);

//const uint8_t EX_EndFunctionParms = 0x16;

class MultiplayerMod : public Mod
{
public:

	//Basic Mod Info
	MultiplayerMod()
	{
		ModName = "MultiplayerMod"; // Mod Name -- If Using BP ModActor, Should Be The Same Name As Your Pak
		ModVersion = "1.0.0-RC1"; // Mod Version
		ModDescription = "Adds local multiplayer to Tales of Arise."; // Mod Description
		ModAuthors = "Eusthron"; // Mod Author
		ModLoaderVersion = "2.0.2";

		// Dont Touch The Internal Stuff
		ModRef = this;
		CompleteModCreation();
	}

	//Called When Internal Mod Setup is finished
	virtual void InitializeMod() override;

	//Either ProcessInternals or ProcessLocalScriptFunction which you use to communicate between your BPMod and your C++ Mod

	//InitGameState Call
	virtual void InitGameState() override;

	//Beginplay Hook of Every Actor
	virtual void BeginPlay(UE4::AActor* Actor) override;

	//PostBeginPlay of EVERY Blueprint ModActor
	virtual void PostBeginPlay(std::wstring ModActorName, UE4::AActor* Actor) override;

	virtual void OnModMenuButtonPressed() override;

	//Call ImGui Here (CALLED EVERY FRAME ON DX HOOK)
	virtual void DrawImGui() override;

	void RegisterModules();

	void Tick();
	bool IsBattleScene();

	void SetNearClippingPlane(float plane);
	void ResetNearClippingPlane();
	void BP_OnCameraAngle(UE4::FVector2D angle);


	bool OnBeforeVirtualFunction(UE4::UObject* Context, UE4::FFrame& Stack, void* ret);
	void OnAfterVirtualFunction(UE4::UObject* Context, UE4::FFrame& Stack, void* ret);

	static FNativeFuncPtr *GNatives;
	static float* GNearClippingPlane;
	static float GNearOriginal;

	GamepadState OldStates[4] = { 0 };
	GamepadState NewStates[4] = { 0 };
	GamepadState JustPressed[4] = { 0 };
	GamepadState JustReleased[4] = { 0 };

	// Array of MultiPlayerControllers
	UE4::APlayerController* Controllers[MAX_CONTROLLERS] = { nullptr };
	UE4::AActor* InputProcesses[MAX_CONTROLLERS] = { nullptr };

	UE4::AActor* ModActor;

	UE4::APawn* GetControlledCharacter(int index);
	UE4::APlayerController* GetController(int index);
	SDK::ABtlInputExtInputProcessBase* GetInputProcess(int index);
	UE4::APlayerController* GetControllerOfCharacter(UE4::APawn *pawn);
	int GetPlayerIndex(UE4::APlayerController* playerController);
	bool IsControlledCharacter(UE4::AActor* actor, bool ignoreP1);

	UE4::APlayerController* GetControllerFromInputProcessor(UE4::AActor *inputProcess);
	int GetPlayerIndexFromInputProcessor(UE4::AActor* inputProcess);

	int CurrentPlayer = -1;
	bool IsSettingUpStrikeAttack = false;
	bool CameraFrozen = false;
	float CameraShakeScale = 1.0f;
	bool DisableHitStop = false;

	bool AutoChangeCharas = false;
	bool RestrictBoostAttacksToCpuAndSelf = false;
	bool RestrictBoostAttacksToP1 = false;


	bool IsMultiplayerBattle();

	/*void SetOperationCharacter(int index) {
		if (IsMultiplayerBattle()) {
			for (int i = 0; i < MAX_CONTROLLERS; i++) {
				if (Controllers[i] != nullptr) {
					auto chara = (SDK::ABP_BtlCharacterBase_C*)((SDK::APlayerController*)Controllers[i])->K2_GetPawn();
					if (chara != nullptr) {
						*GetPlayerOperationFlag(chara) = i == index;
					}
				}
			}
		}
	}*/

	void OnBeforePause();

	static MultiplayerMod *GetInstance() {
		return ((MultiplayerMod*)Mod::ModRef);
	}

	void AddBlueprintHook(std::string fnName, FBlueprintHookHandler handler);
	void AddBlueprintHook(UE4::UFunction* fn, FBlueprintHookHandler handler);

	/// <summary>
	/// Temporarily assigns player at <c>playerIndex</c> to the first player (i.e. giving it the flag)
	/// </summary>
	/// <param name="playerIndex"></param>
	void ChangeFirstPlayerTemporarily(int playerIndex);

	/// <summary>
	/// Restores the first player to what it was set to before the call to ChangeFirstPlayerTemporarily.
	/// </summary>
	void RestoreFirstPlayer();
private:
	UE4::UFunction* IsBattleSceneFn;
	UE4::UFunction* OnChangeFirstPlayerTemporarilyFn;
	UE4::UFunction* OnRestoreFirstPlayerFn;
	UE4::UFunction* GetControlledCharacterFn;
	UE4::UFunction* OnBeforePauseFn;

	std::vector<int> PlayerStack;
	std::map<UE4::UFunction*, FBlueprintHookHandler> BlueprintHooks;
	std::vector<class BaseModule *> Modules;

	//bool *GetPlayerOperationFlag(SDK::ABtlCharacterBase *chara) {
	//	// Found this address by doing a diff
	//	// UnknownData07 = 608
	//	return (bool *)(((bool *)(&(chara->ComboNextArts))) + 644);
	//}


	void ChangePartyTop(int index);

	void RefreshIni();

	int MenuCandidate = 0;
	bool LogEverything = false;

	time_t LastCheck;
	InputManager* Manager;

	bool Initialized = false;
};

