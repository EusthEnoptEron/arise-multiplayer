#include "CameraModule.h"
#include "../Utils.h"

MultiplayerMod* CameraModule::ModRef;

void CameraModule::Initialize(MultiplayerMod* mod)
{
	ModRef = mod;

	BP_OnCameraAngle = UE4::UObject::FindObject<UE4::UFunction>("Function ModActor.ModActor_C.BP_OnCameraAngle");

	// Get battle camera as soon as possible
	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlCameraLibrary.SetActiveCamera")->GetFunction()),
		&SetActiveCameraHook, &SetActiveCamera, "SetActiveCamera");


	mod->AddBlueprintHook(
		"BP_BtlCamera.BP_BtlCamera_C.SetFocusUnitCamera",
		OnSetFocusUnitCamera
	);
}

void CameraModule::Tick()
{
	// Calculate accumulate of all camera movements and listen to change chara button
	float x = 0.0f, y = 0.0f;
	for (int i = 0; i < 4; i++) {
		auto angle = ModRef->OldStates[i].CameraAngle;
		if (angle.bActive) {
			if (abs(x) < abs(angle.x)) {
				x = angle.x;
			}
			if (abs(y) < abs(angle.y)) {
				y = angle.y;
			}
		}
	}

	// Update camera angle
	OnCameraAngle(x, y);
}

void CameraModule::OnSetFocusUnitCamera(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn) 
{
	if (!ModRef->CameraFrozen) {
		processFn(Context, Stack, result);
	}
}

FNativeFuncPtr CameraModule::SetActiveCamera;
void CameraModule::SetActiveCameraHook(UE4::UObject* Context, UE4::FFrame& Stack, void* ret) {
	auto obj = GetParam<SDK::ABP_BtlCamera_C*>(Stack, false);
	auto actor = ((MultiplayerMod*)Mod::ModRef)->ModActor;

	actor->ProcessEvent(actor->GetFunction("OnBtlCamera"), &obj);

	SetActiveCamera(Context, Stack, ret);
}

void CameraModule::OnCameraAngle(float x, float y)
{
	auto angle = UE4::FVector2D(x, y);
	ModRef->ModActor->ProcessEvent(BP_OnCameraAngle, &angle);
}
