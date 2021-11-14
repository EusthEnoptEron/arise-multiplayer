// Fill out your copyright notice in the Description page of Project Settings.

#include "Btl_Camera.h"


void  ABtl_Camera::UseAutoResetTargetAngleRange(bool on) {}
void  ABtl_Camera::UpdateRewindToYawHeightNative(float DeltaSeconds) {}
void  ABtl_Camera::UpdateResetToYawNative(float DeltaSeconds) {}
void  ABtl_Camera::UpdateInterpolateCoefficientNative(float DeltaSeconds) {}
void  ABtl_Camera::UpdateExternalNative2(const FVector& AimPoint, const FVector& CamPoint, FVector& oAimPoint, FVector& oCamPoint) {}
void  ABtl_Camera::UpdateExternalNative(FVector& AimPoint, FVector& CamPoint) {}
void  ABtl_Camera::UpdateDistanceTableNative(float DeltaSeconds) {}
void  ABtl_Camera::UpdateCameraModeActionNative(float DeltaSeconds) {}
void  ABtl_Camera::TickNative(float DeltaSeconds) {}
void  ABtl_Camera::SoundPostEvent_BoostEndNative(float DeltaSeconds) {}
void  ABtl_Camera::SetUseCutsceneToNormalInterpolate(bool Enable) {}
void  ABtl_Camera::SetRewindFlag(bool on) {}
void  ABtl_Camera::SetResetCameraProcessTime(float Time) {}
void  ABtl_Camera::SetResetCameraInterpolateTime(float Time) {}
void  ABtl_Camera::SetResetCameraFlag(bool on) {}
void  ABtl_Camera::SetResetCameraCutsceneEndFlag(bool on) {}
void  ABtl_Camera::SetOverlimitUnitCameraNative(ABtlCharacterBase* Unit) {}
void  ABtl_Camera::SetOverlimitUnitCamera(ABtlCharacterBase* Unit) {}
void  ABtl_Camera::SetOverlimitUnit(ABtlCharacterBase* Unit) {}
void  ABtl_Camera::SetOffsetRotation(const FRotator& Rotator) {}
void  ABtl_Camera::SetOffsetLocation(const FVector& Location) {}
void  ABtl_Camera::SetNormalCameraNative(const FVector& Aim, float Yaw, float Height, float Distance) {}
void  ABtl_Camera::SetNormalBtlWinCameraNative() {}
void  ABtl_Camera::SetNormalBtlWinCamera() {}
void  ABtl_Camera::SetMouseControlNative(bool MouseControl, bool GamepadControl) {}
void  ABtl_Camera::SetMode(EBtlCameraMode CameraMode) {}
void  ABtl_Camera::SetInterpolateIgnoreFlag(bool on) {}
void  ABtl_Camera::SetInterpolateCoefficientNative(float Coefficient, float Time) {}
void  ABtl_Camera::SetInterpolateCoefficientIgnoreTimer(float Time) {}
void  ABtl_Camera::SetInterpolateCoefficientCamPointNative(float CoefficientCamPoint) {}
void  ABtl_Camera::SetInspirationUnitCameraNative(ABtlCharacterBase* Unit) {}
void  ABtl_Camera::SetInspirationUnitCamera(ABtlCharacterBase* Unit) {}
void  ABtl_Camera::SetInspirationUnit(ABtlCharacterBase* Unit) {}
void  ABtl_Camera::SetInputYawAddNative(float Value) {}
void  ABtl_Camera::SetInputLimitYawNative(bool Enable, float YawMin, float YawMax) {}
void  ABtl_Camera::SetInputLimitPitchNative(bool Enable, float PitchMin, float PitchMax) {}
void  ABtl_Camera::SetInputLimitHeightNative(bool Enable, float Low, float High) {}
void  ABtl_Camera::SetInputHeightAddNative(float Value) {}
void  ABtl_Camera::SetInputAnyControlNative(bool New) {}
void  ABtl_Camera::SetFocusUnitCameraNative(bool Enable, ABtlCharacterBase* Unit, USceneComponent* Part) {}
//void  ABtl_Camera::SetFocusUnitCamera(bool Enable, ABtlCharacterBase* Unit, USceneComponent* Part) {}
void  ABtl_Camera::SetFocusUnit(ABtlCharacterBase* Unit) {}
void  ABtl_Camera::SetFocusTime(float In, float Keep, float Out) {}
void  ABtl_Camera::SetFocusPart(USceneComponent* Part) {}
void  ABtl_Camera::SetFocusLocationState(EBtlCameraFocusLocationState State) {}
void  ABtl_Camera::SetFocusLocationCameraNative(bool Enable, const FVector& Location, float Distance, float InTime, float keepTime, float EndTime) {}
//void  ABtl_Camera::SetFocusLocationCamera(bool Enable, const FVector& Location, float Distance, float InTime, float keepTime, float EndTime) {}
void  ABtl_Camera::SetFocusLocation(const FVector& Location) {}
void  ABtl_Camera::SetFocusDistance(float Distance) {}
void  ABtl_Camera::SetExternalYawOverrideNative(bool UseOverride, float OverrideYaw) {}
void  ABtl_Camera::SetExternalYawOffsetNative(float YawOffset) {}
void  ABtl_Camera::SetExternalPitchOverrideNative(bool UseOverride, float OverridePitch) {}
void  ABtl_Camera::SetExternalPitchOffsetNative(float PitchOffset) {}
void  ABtl_Camera::SetExternalDistanceOverrideNative(bool UseOverride, float OverrideDistance) {}
void  ABtl_Camera::SetExternalAimOffsetNative(bool UseOverride, float AimOffset) {}
void  ABtl_Camera::SetEnableInputYawNative(bool Enable) {}
void  ABtl_Camera::SetEnableInputHeightNative(bool Enable) {}
void  ABtl_Camera::SetDistanceInterpolateValueNative(float Value) {}
void  ABtl_Camera::SetDefaultFOV(float FOV) {}
void  ABtl_Camera::SetDefaultAimOffsetHeight(float Height) {}
void  ABtl_Camera::SetCameraYaw(float Value) {}
void  ABtl_Camera::SetCameraHeight(float Value) {}
void  ABtl_Camera::SetBoostAttackUnit(ABtlCharacterBase* Unit) {}
void  ABtl_Camera::SetBoostAttackTimer(float Time) {}
void  ABtl_Camera::SetActiveFlag(bool Enable, EBtlBitFlagCategory Category) {}
void  ABtl_Camera::SavedPrevPointsNative(const FVector& AimPoint, const FVector& CamPoint) {}
void  ABtl_Camera::ReturnBeforeFocusUnitNative() {}
void  ABtl_Camera::ReturnBeforeFocusUnit() {}
void  ABtl_Camera::ResetCameraNative() {}
void  ABtl_Camera::ResetCameraInstantNative() {}
void  ABtl_Camera::PushMode(EBtlCameraMode CameraMode) {}
void  ABtl_Camera::PopMode() {}
void  ABtl_Camera::LookupNormalNative2(const FVector& AimPoint, const FVector& CamPoint, FVector& oAimPoint, FVector& oCamPoint) {}
void  ABtl_Camera::LookupNormalNative(FVector& AimPoint, FVector& CamPoint) {}
bool  ABtl_Camera::IsUseCutsceneToNormalInterpolate() { return false; }
bool  ABtl_Camera::IsUseAutoResetTargetAngleRange() { return false; }
bool  ABtl_Camera::IsActiveFlag() { return false; }
bool  ABtl_Camera::GetRewindFlag() { return false; }
float  ABtl_Camera::GetResetCameraProcessTime() { return 0.0f; }
float  ABtl_Camera::GetResetCameraInterpolateTime() { return 0.0f; }
bool  ABtl_Camera::GetResetCameraFlag() { return false; }
bool  ABtl_Camera::GetResetCameraCutsceneEndFlag() { return false; }
ABtlCharacterBase* ABtl_Camera::GetOverlimitUnit() {
	return nullptr;
}
FRotator  ABtl_Camera::GetOffsetRotation() { return FRotator(); }
FVector  ABtl_Camera::GetOffsetLocation() { return FVector(); }
EBtlCameraMode  ABtl_Camera::GetMode() { return (EBtlCameraMode)0; }
bool  ABtl_Camera::GetInterpolateIgnoreFlag() { return false; }
ABtlCharacterBase* ABtl_Camera::GetInspirationUnit() { return nullptr; }
//TArray< ABtl_Camera::AActor&> GetIgnoreBlockingActors() {}
ABtlCharacterBase* ABtl_Camera::GetFocusUnit() { return nullptr; }
USceneComponent* ABtl_Camera::GetFocusPart() { return nullptr; }
float  ABtl_Camera::GetFocusOutTime() { return 0.0f; }
EBtlCameraFocusLocationState  ABtl_Camera::GetFocusLocationState() { return (EBtlCameraFocusLocationState)0; }
FVector  ABtl_Camera::GetFocusLocation() { return FVector(); }
float  ABtl_Camera::GetFocusKeepTime() { return 0.0f; }
float  ABtl_Camera::GetFocusInTime() { return 0.0f; }
float  ABtl_Camera::GetFocusDistance() { return 0.0f; }
EBtlCameraEasingState  ABtl_Camera::GetEasingStateNative(float Time, float InTime, float keepTime, float outTime) { return (EBtlCameraEasingState)0; }
float  ABtl_Camera::GetDistanceFromDistanceTable(int DistanceTableScore) { return 0.0f; }
float  ABtl_Camera::GetDefaultFOV() { return 0.0f; }
float  ABtl_Camera::GetDefaultAimOffsetHeight() { return 0.0f; }
FVector  ABtl_Camera::GetCamPointNormalNative2(const FVector& AimPoint, const FVector& CamDirectionXY) { return FVector(); }
FVector  ABtl_Camera::GetCamPointNormalNative(const FVector& AimPoint, const FVector& CamDirectionXY) { return FVector(); }
FVector  ABtl_Camera::GetCamDirectionNormalNative(float Yaw) { return FVector(); }
FVector  ABtl_Camera::GetAimPointNormalNative() { return FVector(); }
FVector  ABtl_Camera::GetAimOffsetFromDistanceTable(int DistanceTableScore) { return FVector(); }
void  ABtl_Camera::ExecutePreBoostStrikeNative(float DeltaSeconds) {}
void  ABtl_Camera::ExecuteNormalNative(float DeltaSeconds) {}
void  ABtl_Camera::ExecuteNormalBattleEndNative(float DeltaSeconds) {}
void  ABtl_Camera::ExecuteInitializeNative() {}
void  ABtl_Camera::ExecuteFocusUnitNative(float DeltaSeconds) {}
void  ABtl_Camera::ExecuteFocusLocationNative(float DeltaSeconds) {}
void  ABtl_Camera::ExecuteEasingOverlimitNative(float DeltaSeconds) {}
void  ABtl_Camera::ExecuteEasingInspirationNative(float DeltaSeconds) {}
void  ABtl_Camera::ExecuteCutsceneToNative(float DeltaSeconds) {}
void  ABtl_Camera::ExecuteCounterRaidNative(float DeltaSeconds) {}
void  ABtl_Camera::ExecuteBoostAttackNative(float DeltaSeconds) {}
void  ABtl_Camera::ExecuteAutoCameraResetNative(float DeltaSeconds) {}
void  ABtl_Camera::DetectBlockingNative2(const FVector& AimPoint, const FVector& CamPoint, FVector& EscapePoint) {}
void  ABtl_Camera::DetectBlockingNative(FVector& AimPoint, FVector& CamPoint) {}
void  ABtl_Camera::ClearOffsetValue() {}
void  ABtl_Camera::CalculateNormalCameraNative(float DeltaSeconds, FVector& NewLocation, FRotator& NewRotation, FVector& NewAimPoint, FVector& NewCamPoint) {}
int  ABtl_Camera::CalculateDistanceTableIndex() { return 0; }