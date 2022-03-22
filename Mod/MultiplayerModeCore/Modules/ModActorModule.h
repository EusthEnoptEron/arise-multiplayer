#pragma once
#include "BaseModule.h"


/// <summary>
/// Module that handles the integration with ModActor.
/// </summary>
class ModActorModule : public BaseModule
{
public:
	virtual void Initialize(MultiplayerMod* mod) override;

private:
	static void OnBeginBattle(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
	static void OnEndBattle(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);

	static void Native_OnSubStateStartImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
	static void Native_OnSubStateEndImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
	static void Native_OnBeginChangeTargetImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
	static void Native_OnEndChangeTargetImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
	static void Native_SetNearClippingPlaneImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
	static void Native_ResetNearClippingPlaneImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
	static FNativeFuncPtr BattlePause;
	static void BattlePauseHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result);

	static FNativeFuncPtr BattleResume;
	static void BattleResumeHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result);


	static MultiplayerMod* ModRef;

	static UE4::UFunction *K2_GetBattleInputProcessFn;
	static UE4::UFunction *K2_GetBattlePCControllerFn;
};

