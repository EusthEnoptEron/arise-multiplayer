#pragma once

#include "BaseModule.h"
typedef  void (*ClientStopForceFeedbackPtr)(SDK::APlayerController* thisPtr, SDK::UForceFeedbackEffect* ForceFeedbackEffect, const SDK::FName& Tag);
typedef  void (*ClientPlayForceFeedbackPtr)(SDK::APlayerController* thisPtr, SDK::UForceFeedbackEffect* ForceFeedbackEffect, bool bLooping, bool bIgnoreTimeDilation, const SDK::FName& Tag, float LargePow, float SmallPow);

struct AttackerReceiver {
	SDK::ABtlCharacterBase *Attacker;
	SDK::ABtlCharacterBase *Receiver;
};

/// <summary>
/// Module that implements vibrations over all controllers.
/// </summary>
class VibrationModule : public BaseModule
{
public:
	virtual void Initialize(MultiplayerMod *mod) override;

private:
	static ClientStopForceFeedbackPtr ClientStopForceFeedback;
	static void ClientStopForceFeedbackHook(SDK::APlayerController* thisPtr, SDK::UForceFeedbackEffect* ForceFeedbackEffect, const SDK::FName& Tag);

	static ClientPlayForceFeedbackPtr ClientPlayForceFeedback;
	static void ClientPlayForceFeedbackHook(SDK::APlayerController* thisPtr, SDK::UForceFeedbackEffect* ForceFeedbackEffect, bool bLooping, bool bIgnoreTimeDilation, const SDK::FName& Tag, float LargePow, float SmallPow);

	static void OnDamage(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);

	static AttackerReceiver *CurrentContext;

};

