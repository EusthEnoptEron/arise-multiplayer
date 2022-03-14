#include "VibrationModule.h"
#include <Utilities/MinHook.h>

AttackerReceiver *VibrationModule::CurrentContext;

ClientPlayForceFeedbackPtr VibrationModule::ClientPlayForceFeedback;
ClientStopForceFeedbackPtr VibrationModule::ClientStopForceFeedback;


void VibrationModule::Initialize(MultiplayerMod *mod)
{
	// Hook into native functions
	auto addr = Pattern::Find("48 85 D2 0F 84 ?? ?? ?? ?? 48 8B C4 44 88 40 18 55 41 54 41 55 41 56 48 83 EC 38 48 89 58 08 45 0F B6 E9");
	Log::Info("PlayVibration address: %p", addr);
	MinHook::Add((DWORD_PTR)
		addr,
		&VibrationModule::ClientPlayForceFeedbackHook, &VibrationModule::ClientPlayForceFeedback, "PlayForceFeedback");

	addr = Pattern::Find("40 53 41 54 41 57 48 83 ?? ?? 45 33 E4 49 8B D8 4C 8B FA 48 85 D2");
	Log::Info("StopVibration address: %p", addr);
	MinHook::Add((DWORD_PTR)
		addr,
		&VibrationModule::ClientStopForceFeedbackHook, &VibrationModule::ClientStopForceFeedback, "StopForceFeedback");


	mod->AddBlueprintHook(
		"BP_BattleManagerBase.BP_BattleManagerBase_C.BndEvt__EventDispatcher_K2Node_ComponentBoundEvent_12_BtlDamageSignature__DelegateSignature", 
		VibrationModule::OnDamage
	);
}


void VibrationModule::OnDamage(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	// make it so that we know down the line what's the context
	CurrentContext = Stack.GetParams<AttackerReceiver>();

	// Execute as normal
	processFn(Context, Stack, result);

	CurrentContext = nullptr;
}


void VibrationModule::ClientPlayForceFeedbackHook(SDK::APlayerController* thisPtr, SDK::UForceFeedbackEffect* ForceFeedbackEffect, bool bLooping, bool bIgnoreTimeDilation, const SDK::FName& Tag, float LargePow, float SmallPow) {
	//Log::Info("Play Vibration (%s)", ForceFeedbackEffect->GetName().c_str());
	if (CurrentContext != nullptr) {
		// We have a context, so only let the player handle the event that's affected!
		//Log::Info("Attacker: %s, Receiver: %s", CurrentContext->Attacker->GetName().c_str(), CurrentContext->Receiver->GetName().c_str());

		auto attackerController = (UE4::APlayerController *)CurrentContext->Attacker->GetController();
		if (MultiplayerMod::GetInstance()->GetPlayerIndex(attackerController) >= 0) {
			VibrationModule::ClientPlayForceFeedback((SDK::APlayerController *)attackerController, ForceFeedbackEffect, bLooping, bIgnoreTimeDilation, Tag, LargePow, SmallPow);
			return;
		}

		auto receiverController = (UE4::APlayerController*)CurrentContext->Receiver->GetController();
		if (MultiplayerMod::GetInstance()->GetPlayerIndex(receiverController) >= 0) {
			VibrationModule::ClientPlayForceFeedback((SDK::APlayerController*)receiverController, ForceFeedbackEffect, bLooping, bIgnoreTimeDilation, Tag, LargePow, SmallPow);
			return;
		}
	}

	// Forward to other players
	VibrationModule::ClientPlayForceFeedback(thisPtr, ForceFeedbackEffect, bLooping, bIgnoreTimeDilation, Tag, LargePow, SmallPow);

	auto mod = MultiplayerMod::GetInstance();
	for (int i = 1; i < MAX_CONTROLLERS; i++) {
		if (mod->Controllers[i] != nullptr) {
			// Forward message to controller.
			auto controller = ((SDK::APlayerController*)mod->Controllers[i]);

			VibrationModule::ClientPlayForceFeedback(controller, ForceFeedbackEffect, bLooping, bIgnoreTimeDilation, Tag, LargePow, SmallPow);
		}
	}
}

void VibrationModule::ClientStopForceFeedbackHook(SDK::APlayerController* thisPtr, SDK::UForceFeedbackEffect* ForceFeedbackEffect, const SDK::FName& Tag) {
	VibrationModule::ClientStopForceFeedback(thisPtr, ForceFeedbackEffect, Tag);

	auto mod = MultiplayerMod::GetInstance();
	// Forward stop event to other players
	for (int i = 1; i < MAX_CONTROLLERS; i++) {
		if (mod->Controllers[i] != nullptr) {
			// Forward message to controller.
			auto controller = ((SDK::APlayerController*)mod->Controllers[i]);

			VibrationModule::ClientStopForceFeedback(controller, ForceFeedbackEffect, Tag);
		}
	}
}

