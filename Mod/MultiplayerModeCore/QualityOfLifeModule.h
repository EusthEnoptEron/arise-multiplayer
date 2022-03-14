#pragma once

#include "BaseModule.h";

typedef  SDK::UCameraShake* (*FPlayCameraShakePtr)(SDK::UClass* ShakeClass, float Scale, SDK::TEnumAsByte<SDK::ECameraAnimPlaySpace> PlaySpace, const SDK::FRotator& UserPlaySpaceRot);

class QualityOfLifeModule : public BaseModule
{
public:
	virtual void Initialize(MultiplayerMod* mod) override;

private:
	static void OnProcessHitStop(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);

	static FPlayCameraShakePtr PlayCameraShake;
	static SDK::UCameraShake* PlayCameraShakeHook(SDK::UClass* ShakeClass, float Scale, SDK::TEnumAsByte<SDK::ECameraAnimPlaySpace> PlaySpace, const SDK::FRotator& UserPlaySpaceRot);
};

