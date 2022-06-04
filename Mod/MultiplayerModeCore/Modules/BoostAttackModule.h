#pragma once
#include "BaseModule.h"


/// <summary>
/// Handles boost attacks, which are... frankly a pain to get right.
/// </summary>
class BoostAttackModule : public BaseModule
{
public:
	virtual void Initialize(MultiplayerMod* mod) override;

private:

	static void OnRunStrike(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
	static void OnRunBoostAttack(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
	static void OnAttackBeginEvent(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);

	static MultiplayerMod* ModRef;

	static int LastStrikeInitiator;
};

