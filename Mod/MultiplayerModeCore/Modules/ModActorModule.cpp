#include "ModActorModule.h"

#include "../Utils.h"

MultiplayerMod* ModActorModule::ModRef;
UE4::UFunction* ModActorModule::K2_GetBattleInputProcessFn;
UE4::UFunction* ModActorModule::K2_GetBattlePCControllerFn;

void ModActorModule::Initialize(MultiplayerMod* mod)
{
	ModRef = mod;

	// ----------------------------------------------------------------------------------
	// Hook into pause functions to react faster to camera changes (might be obsolete)
	// ----------------------------------------------------------------------------------
	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlManager.BattlePause")->GetFunction()),
		&BattlePauseHook, &BattlePause, "BattlePause");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlManager.BattleResume")->GetFunction()),
		&BattleResumeHook, &BattleResume, "BattleResume");

	mod->AddBlueprintHook(
		"ModActor.ModActor_C.OnBeginBattle",
		OnBeginBattle
	);

	mod->AddBlueprintHook(
		"ModActor.ModActor_C.OnEndBattle",
		OnEndBattle
	);

	mod->AddBlueprintHook(
		"ModActor.ModActor_C.Native_OnSubStateStart",
		Native_OnSubStateStartImpl
	);

	mod->AddBlueprintHook(
		"ModActor.ModActor_C.Native_OnSubStateEnd",
		Native_OnSubStateEndImpl
	);

	mod->AddBlueprintHook(
		"ModActor.ModActor_C.Native_OnBeginChangeTarget",
		Native_OnBeginChangeTargetImpl
	);

	mod->AddBlueprintHook(
		"ModActor.ModActor_C.Native_OnEndChangeTarget",
		Native_OnEndChangeTargetImpl
	);

	mod->AddBlueprintHook(
		"ModActor.ModActor_C.Native_SetNearClippingPlane",
		Native_SetNearClippingPlaneImpl
	);

	mod->AddBlueprintHook(
		"ModActor.ModActor_C.Native_ResetNearClippingPlane",
		Native_ResetNearClippingPlaneImpl
	);

	K2_GetBattleInputProcessFn = FindFunction("Function Arise.BtlInputExtInputProcessBase.K2_GetBattleInputProcess");
	K2_GetBattlePCControllerFn = FindFunction("Function Arise.BtlInputExtInputProcessBase.K2_GetBattlePCController");
}

void ModActorModule::OnBeginBattle(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	InputManager::GetInstance()->SetRerouteControllers(true);

	Log::Info("On Begin Battle");

	// Keep reference to first playerC input process
	struct GetBattleInputArgs {
		UE4::UObject* WorldContext;
		UE4::AActor* Result;
	};

	GetBattleInputArgs args = { ModRef->ModActor, nullptr };
	ModRef->ModActor->ProcessEvent(K2_GetBattleInputProcessFn, &args);
	ModRef->InputProcesses[0] = args.Result;
	Log::Info("Setting first player process: %p (%s)", ModRef->InputProcesses[0], ModRef->InputProcesses[0]->GetName().c_str());

	ModRef->ModActor->ProcessEvent(K2_GetBattlePCControllerFn, &args);
	ModRef->Controllers[0] = (UE4::APlayerController*)args.Result;
	Log::Info("Setting first player controller: %p (%s)", ModRef->Controllers[0], ModRef->Controllers[0]->GetName().c_str());

	processFn(Context, Stack, result);
}

void ModActorModule::OnEndBattle(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	InputManager::GetInstance()->SetRerouteControllers(false);

	Log::Info("On End Battle");

	ModRef->ResetNearClippingPlane(); // To be sure

	ModRef->Controllers[0] = nullptr;
	ModRef->InputProcesses[0] = nullptr;

	processFn(Context, Stack, result);
}

void ModActorModule::Native_OnSubStateStartImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	SDK::EBattleState state = *Stack.GetParams<SDK::EBattleState>();

	if (state == SDK::EBattleState::StateMenu) {
		ModRef->CameraFrozen = true;

		for (int i = 0; i < 4; i++) {
			if (ModRef->OldStates[i].IsMenu) {
				Log::Info("Change first player: %d", i);
				InputManager::GetInstance()->SetFirstPlayer(i);
				if (i != 0) {
					ModRef->ChangeFirstPlayerTemporarily(i);
				}

				break;
			}
		}
	}

	processFn(Context, Stack, result);
}

void ModActorModule::Native_OnSubStateEndImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	SDK::EBattleState state = *Stack.GetParams<SDK::EBattleState>();
	if (state == SDK::EBattleState::StateMenu) {
		InputManager::GetInstance()->SetFirstPlayer(0);

		ModRef->RestoreFirstPlayer();
		ModRef->CameraFrozen = false;
	}

	processFn(Context, Stack, result);
}

void ModActorModule::Native_OnBeginChangeTargetImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	ModRef->CameraFrozen = true;
	for (int i = 0; i < 4; i++) {
		if (ModRef->OldStates[i].IsTarget) {
			Log::Info("Change first player: %d", i);
			InputManager::GetInstance()->SetFirstPlayer(i);
			if (i != 0) {
				ModRef->ChangeFirstPlayerTemporarily(i);
			}
			break;
		}
	}

	//InputManager::GetInstance()->SetRerouteControllers(true);

	processFn(Context, Stack, result);
}

void ModActorModule::Native_OnEndChangeTargetImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	// Return control of player 1 to player 1.
	InputManager::GetInstance()->SetFirstPlayer(0);
	ModRef->RestoreFirstPlayer();
	ModRef->CameraFrozen = false;
	//InputManager::GetInstance()->SetRerouteControllers(false);

	processFn(Context, Stack, result);
}

void ModActorModule::Native_SetNearClippingPlaneImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	ModRef->SetNearClippingPlane(*Stack.GetParams<float>());

	processFn(Context, Stack, result);
}

void ModActorModule::Native_ResetNearClippingPlaneImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	ModRef->ResetNearClippingPlane();

	processFn(Context, Stack, result);
}



//Function Arise.BtlManager.BattlePause
FNativeFuncPtr ModActorModule::BattlePause;
void ModActorModule::BattlePauseHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	((MultiplayerMod*)(Mod::ModRef))->OnBeforePause();
	BattlePause(Context, Stack, result);
}

// Function Arise.BtlManager.BattleResume
FNativeFuncPtr ModActorModule::BattleResume;
void ModActorModule::BattleResumeHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result) {
	static auto onBeforePauseFn = UE4::UObject::FindObject<UE4::UFunction>("Function ModActor.ModActor_C.OnBeforePause");

	BattleResume(Context, Stack, result);

	((MultiplayerMod*)(Mod::ModRef))->OnBeforePause();
}