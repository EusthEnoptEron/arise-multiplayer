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

SDK::UCameraShake* QualityOfLifeModule::PlayCameraShakeHook(SDK::UClass* ShakeClass, float Scale, SDK::TEnumAsByte<SDK::ECameraAnimPlaySpace> PlaySpace, const SDK::FRotator& UserPlaySpaceRot) {
	return PlayCameraShake(ShakeClass, Scale * ((MultiplayerMod*)Mod::ModRef)->CameraShakeScale, PlaySpace, UserPlaySpaceRot);
}
