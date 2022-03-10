#pragma once
#include "iostream"
#include <string>
#include "Mod/Mod.h"
#include "steam/isteaminput.h"
#include "InputManager.h";
#include "../SDK.h";
#include "../SDK/BP_BtlCamera_classes.h";
#include "../SDK/STR_BtlHudVisible_structs.h";
#include "../SDK/TO14_StateEnum_MNU_SAV_structs.h";
#include "../SDK/TO14_ProcessEnum_MNU_SAV_structs.h";
#include "../SDK/TO14_ModeEnum_MNU_SAV_structs.h";
#include "../SDK/TO14_BPI_GUI_MNU_SAV_classes.h"
#include "../SDK/BP_BtlCharacterBase_structs.h"
#include "../SDK/BP_BtlCharacterBase_classes.h"
#include "FileWatch.hpp"
#include "Utilities/MinHook.h"

typedef  void (*FNativeFuncPtr)(UE4::UObject* Context, UE4::FFrame& Stack, void* result);
typedef  float ( *FGetBtlAxisValue)(SDK::AInputExtPlayerController* thisPtr, const UE4::FName& InAxisName);
typedef  void (*FNativeFuncPtr)(UE4::UObject* Context, UE4::FFrame& Stack, void* result);
typedef  void (*FBlueprintHookHandler)(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);


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

struct FOutParmRec
{
	UE4::UField* Property;
	uint8_t* PropAddr;
	FOutParmRec* NextOutParm;
};

struct ApplyConfigParams {

	float MinDistance;
	float MaxDistance;

	float TargetOffset;
	bool TargetEnemies;
	bool TargetHeroes;

	float ZoomInSpeed;
	float ZoomOutSpeed;

	float ZoomInPadding;
	float ZoomOutPadding;

	float RotateSpeedX;
	float RotateSpeedY;

	float MinClip;
	float MaxClip;
	float ClipRatio;

	float FieldOfView;
	float TargetSpeed;
	float TargetRadius;

	float MinPitch;
	float MaxPitch;

	bool AllowSwitchingCharasDuringBattle;
	bool ResetCharacterAssignmentsAfterBattle;

	bool IgnoreDeadPlayers;
	bool UseSinglePlayerCamera;

	float TargetCursorScale;
	bool HideDefaultCursor;

	bool DebugMenu;
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

struct GetControlledCharacterParms {
	int Index;
	UE4::APawn* Result;
};

struct GetControllerParms {
	int Index;
	UE4::APlayerController* Result;
};

const int MAX_CONTROLLERS = 10;


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

typedef void(*FEngineLoop__Tick_Fn)(void* thisptr);
typedef void(*APlayerController__PlayerTickFn)(UE4::APlayerController* thisptr, const float DeltaTime);
typedef void(*APlayerController__PrePostProcessInputFn)(UE4::APlayerController* thisptr, const float DeltaTime, const bool bGamePaused);

const std::string INI_FILE_LOCATION = "./MultiplayerMod.ini";
const std::wstring INI_FILE_LOCATION_W = L"./MultiplayerMod.ini";

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

	void RegisterModules();

	void Tick();
	void OnAction(int index, const UE4::FString &name);
	void OnActionPressed(int index, const UE4::FString &name);
	void OnActionReleased(int index, const UE4::FString &name);
	void OnAnalogAction(int index, const UE4::FString &name, float x, float y);
	void OnControllerConnected(int index);
	void OnControllerDisconnected(int index);
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

	void AddBlueprintHook(std::string fnName, FBlueprintHookHandler handler) {
		auto fn = UE4::UObject::FindObject<UE4::UFunction>("Function " + fnName);
		bool success = BlueprintHooks.try_emplace(fn, handler).second;
		if (!success) {
			Log::Error("Function handled twice: %s", fn->GetFullName().c_str());
		}
	}

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


	UE4::UFunction* OnActionFn;
	UE4::UFunction* OnActionPressedFn;
	UE4::UFunction* OnActionReleasedFn;
	UE4::UFunction* OnAnalogActionFn;
	UE4::UFunction* OnControllerConnectedFn;
	UE4::UFunction* OnControllerDisconnectedFn;
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

	

	filewatch::FileWatch<std::wstring> *Watch;

	bool Initialized = false;
	bool IniDirty = true;
	void CompareDigitalStates(bool newValue, bool oldValue, bool *justPressed, bool *justReleased, const UE4::FString &name, int index);
};



UE4::UClass* FastGetClass(UE4::UObject* obj);
UE4::UStruct* FastGetSuperField(UE4::UStruct* obj);
bool FastIsA(UE4::UObject* obj, UE4::UClass* cmp);