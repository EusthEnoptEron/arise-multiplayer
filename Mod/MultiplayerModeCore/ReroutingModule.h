#pragma once
#include "BaseModule.h"

/// <summary>
/// Module that takes care of rerouting accessors to the right controller depending on context.
/// </summary>
class ReroutingModule :
    public BaseModule
{
public:
	virtual void Initialize(MultiplayerMod* mod) override;

private:

	static FNativeFuncPtr GetPlayerControlledUnit;
	static FNativeFuncPtr GameplayStatics__GetPlayerController;
	static FNativeFuncPtr K2_GetBattleInputProcess;
	static FNativeFuncPtr K2_GetBattlePCController;


	static void K2_GetBattleInputProcessHook(UE4::UObject* Context, UE4::FFrame& Stack, UE4::AActor** result);
	static void GameplayStatics__GetPlayerControllerHook(UE4::UObject* Context, UE4::FFrame& Stack, UE4::APlayerController** result);
	static void GetPlayerControlledUnitHook(UE4::UObject* Context, UE4::FFrame& Stack, void* result);
	static void K2_GetBattlePCControllerHook(UE4::UObject* Context, UE4::FFrame& Stack, UE4::AActor** result);


	/// <summary>
	/// Deduce the responsible player controller from context.
	/// </summary>
	/// <param name="Stack"></param>
	/// <returns></returns>
	static UE4::APlayerController* FindPlayerController(const UE4::FFrame& Stack);

	/// <summary>
	/// Deduce the responsible character from context.
	/// </summary>
	static UE4::AActor* FindCharacter(const UE4::FFrame& Stack);

};

