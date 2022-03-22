#pragma once

#include "BaseModule.h"

class CameraModule : public BaseModule
{
public:
	virtual void Initialize(MultiplayerMod* mod) override;
	virtual void Tick() override;
private:

	UE4::UFunction* BP_OnCameraAngle;
	void OnCameraAngle(float x, float y);

	static FNativeFuncPtr SetActiveCamera;
	static void SetActiveCameraHook(UE4::UObject* Context, UE4::FFrame& Stack, void* ret);

	static void OnSetFocusUnitCamera(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);

	static MultiplayerMod* ModRef;
};

