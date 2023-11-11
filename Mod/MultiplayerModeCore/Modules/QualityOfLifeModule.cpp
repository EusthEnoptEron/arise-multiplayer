#include "QualityOfLifeModule.h"

FPlayCameraShakePtr QualityOfLifeModule::PlayCameraShake;
void QualityOfLifeModule::Initialize(MultiplayerMod* mod)
{
	// B34C80
	MinHook::Add((DWORD_PTR)
		Pattern::Find("48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 57 48 83 EC 50 0F 29 ?? ?? ?? 49 8B F1"),
		&PlayCameraShakeHook, &PlayCameraShake, "PlayCameraShake");

	mod->AddBlueprintHook(
		"BP_BtlCharacterBase.BP_BtlCharacterBase_C.HitStopProcess",
		OnProcessHitStop
	);

	mod->AddBlueprintHook(
		"BP_BattleHudHelper.BP_BattleHudHelper_C.GetHudVisible",
		OnGetHudVisible
	);
}

void QualityOfLifeModule::OnProcessHitStop(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	if (MultiplayerMod::GetInstance()->DisableHitStop) {
		// Ignore hit stop
	}
	else {
		processFn(Context, Stack, result);
	}
}


void QualityOfLifeModule::OnGetHudVisible(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	processFn(Context, Stack, result);

	/*const auto uiManager = ((SDK::UBtlFunctionLibrary*)Context)->STATIC_GetUIManager();
	const auto params = *(Stack.GetParams<SDK::FName>());*/

	if (MultiplayerMod::GetInstance()->ShowP1Health
		//&& params == uiManager->DefaultHudVisiblePresetLabel.Label &&
	) {
		// Make no difference between the player bar and the party player bar

		const auto res = ((SDK::FSTR_BtlHudVisible*)((FOutParmRec*)Stack.OutParms)->PropAddr);
		const auto prev = res->PartyPlayerBar_37_8969DF6849BC10BB4074B298A5BA4AA5;
		res->PartyPlayerBar_37_8969DF6849BC10BB4074B298A5BA4AA5 = res->PartyBar_25_759288464FB4ED93215E998E9D09F40E;
	}
}



SDK::UCameraShake* QualityOfLifeModule::PlayCameraShakeHook(SDK::UClass* ShakeClass, float Scale, SDK::TEnumAsByte<SDK::ECameraAnimPlaySpace> PlaySpace, const SDK::FRotator& UserPlaySpaceRot) {
	return PlayCameraShake(ShakeClass, Scale * ((MultiplayerMod*)Mod::ModRef)->CameraShakeScale, PlaySpace, UserPlaySpaceRot);
}
