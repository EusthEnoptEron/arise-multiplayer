#pragma once
#include "BaseModule.h"

struct JustParams {
	UE4::APawn* DmgActor;
	bool JustGuard;
};

/// <summary>
/// Module that takes care of stepping and guarding.
/// </summary>
class StepGuardModule :
    public BaseModule
{
public:
	virtual void Initialize(MultiplayerMod* mod) override;

private:
	static FNativeFuncPtr IsAutoStepable;
	static FNativeFuncPtr IsAutoGuardable;

	static void IsAutoStepableHook(UE4::UObject* Context, UE4::FFrame& Stack, bool* ret);
	static void IsAutoGuardableHook(UE4::UObject* Context, UE4::FFrame& Stack, bool* ret);

	static void OnStepOrGuard(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);

	static MultiplayerMod* ModRef;

};

