// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "BtlCharacterBase.h"
#include "Enums.h"
#include "Btl_Camera.generated.h"

/**
 * 
 */
UCLASS()
class ARISE_API ABtl_Camera : public ACameraActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<EBtlCameraMode> ModeList;

	UPROPERTY(BlueprintReadWrite)
	float InterpolateValue_Native;

	UFUNCTION(BlueprintCallable)
	void UseAutoResetTargetAngleRange(bool on);
	UFUNCTION(BlueprintCallable)
	void UpdateRewindToYawHeightNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void UpdateResetToYawNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void UpdateInterpolateCoefficientNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void UpdateExternalNative2(const FVector& AimPoint, const FVector& CamPoint, FVector& oAimPoint,
	                           FVector& oCamPoint);
	UFUNCTION(BlueprintCallable)
	void UpdateExternalNative(FVector& AimPoint, FVector& CamPoint);
	UFUNCTION(BlueprintCallable)
	void UpdateDistanceTableNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void UpdateCameraModeActionNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void TickNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void SoundPostEvent_BoostEndNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void SetUseCutsceneToNormalInterpolate(bool Enable);
	UFUNCTION(BlueprintCallable)
	void SetRewindFlag(bool on);
	UFUNCTION(BlueprintCallable)
	void SetResetCameraProcessTime(float Time);
	UFUNCTION(BlueprintCallable)
	void SetResetCameraInterpolateTime(float Time);
	UFUNCTION(BlueprintCallable)
	void SetResetCameraFlag(bool on);
	UFUNCTION(BlueprintCallable)
	void SetResetCameraCutsceneEndFlag(bool on);
	UFUNCTION(BlueprintCallable)
	void SetOverlimitUnitCameraNative(ABtlCharacterBase* Unit);
	UFUNCTION(BlueprintCallable)
	void SetOverlimitUnitCamera(ABtlCharacterBase* Unit);
	UFUNCTION(BlueprintCallable)
	void SetOverlimitUnit(ABtlCharacterBase* Unit);
	UFUNCTION(BlueprintCallable)
	void SetOffsetRotation(const FRotator& Rotator);
	UFUNCTION(BlueprintCallable)
	void SetOffsetLocation(const FVector& Location);
	UFUNCTION(BlueprintCallable)
	void SetNormalCameraNative(const FVector& Aim, float Yaw, float Height, float Distance);
	UFUNCTION(BlueprintCallable)
	void SetNormalBtlWinCameraNative();
	UFUNCTION(BlueprintCallable)
	void SetNormalBtlWinCamera();
	UFUNCTION(BlueprintCallable)
	void SetMouseControlNative(bool MouseControl, bool GamepadControl);
	UFUNCTION(BlueprintCallable)
	void SetMode(EBtlCameraMode CameraMode);
	UFUNCTION(BlueprintCallable)
	void SetInterpolateIgnoreFlag(bool on);
	UFUNCTION(BlueprintCallable)
	void SetInterpolateCoefficientNative(float Coefficient, float Time);
	UFUNCTION(BlueprintCallable)
	void SetInterpolateCoefficientIgnoreTimer(float Time);
	UFUNCTION(BlueprintCallable)
	void SetInterpolateCoefficientCamPointNative(float CoefficientCamPoint);
	UFUNCTION(BlueprintCallable)
	void SetInspirationUnitCameraNative(ABtlCharacterBase* Unit);
	UFUNCTION(BlueprintCallable)
	void SetInspirationUnitCamera(ABtlCharacterBase* Unit);
	UFUNCTION(BlueprintCallable)
	void SetInspirationUnit(ABtlCharacterBase* Unit);
	UFUNCTION(BlueprintCallable)
	void SetInputYawAddNative(float Value);
	UFUNCTION(BlueprintCallable)
	void SetInputLimitYawNative(bool Enable, float YawMin, float YawMax);
	UFUNCTION(BlueprintCallable)
	void SetInputLimitPitchNative(bool Enable, float PitchMin, float PitchMax);
	UFUNCTION(BlueprintCallable)
	void SetInputLimitHeightNative(bool Enable, float Low, float High);
	UFUNCTION(BlueprintCallable)
	void SetInputHeightAddNative(float Value);
	UFUNCTION(BlueprintCallable)
	void SetInputAnyControlNative(bool New);
	UFUNCTION(BlueprintCallable)
	void SetFocusUnitCameraNative(bool Enable, ABtlCharacterBase* Unit, USceneComponent* Part);
	//UFUNCTION(BlueprintCallable)
	//void SetFocusUnitCamera(bool Enable, ABtlCharacterBase* Unit, USceneComponent* Part);
	UFUNCTION(BlueprintCallable)
	void SetFocusUnit(ABtlCharacterBase* Unit);
	UFUNCTION(BlueprintCallable)
	void SetFocusTime(float In, float Keep, float Out);
	UFUNCTION(BlueprintCallable)
	void SetFocusPart(USceneComponent* Part);
	UFUNCTION(BlueprintCallable)
	void SetFocusLocationState(EBtlCameraFocusLocationState State);
	UFUNCTION(BlueprintCallable)
	void SetFocusLocationCameraNative(bool Enable, const FVector& Location, float Distance, float InTime,
	                                  float keepTime, float EndTime);
	//UFUNCTION(BlueprintCallable)k
	//void SetFocusLocationCamera(bool Enable, const FVector& Location, float Distance, float InTime, float keepTime, float EndTime);
	UFUNCTION(BlueprintCallable)
	void SetFocusLocation(const FVector& Location);
	UFUNCTION(BlueprintCallable)
	void SetFocusDistance(float Distance);
	UFUNCTION(BlueprintCallable)
	void SetExternalYawOverrideNative(bool UseOverride, float OverrideYaw);
	UFUNCTION(BlueprintCallable)
	void SetExternalYawOffsetNative(float YawOffset);
	UFUNCTION(BlueprintCallable)
	void SetExternalPitchOverrideNative(bool UseOverride, float OverridePitch);
	UFUNCTION(BlueprintCallable)
	void SetExternalPitchOffsetNative(float PitchOffset);
	UFUNCTION(BlueprintCallable)
	void SetExternalDistanceOverrideNative(bool UseOverride, float OverrideDistance);
	UFUNCTION(BlueprintCallable)
	void SetExternalAimOffsetNative(bool UseOverride, float AimOffset);
	UFUNCTION(BlueprintCallable)
	void SetEnableInputYawNative(bool Enable);
	UFUNCTION(BlueprintCallable)
	void SetEnableInputHeightNative(bool Enable);
	UFUNCTION(BlueprintCallable)
	void SetDistanceInterpolateValueNative(float Value);
	UFUNCTION(BlueprintCallable)
	void SetDefaultFOV(float FOV);
	UFUNCTION(BlueprintCallable)
	void SetDefaultAimOffsetHeight(float Height);
	UFUNCTION(BlueprintCallable)
	void SetCameraYaw(float Value);
	UFUNCTION(BlueprintCallable)
	void SetCameraHeight(float Value);
	UFUNCTION(BlueprintCallable)
	void SetBoostAttackUnit(ABtlCharacterBase* Unit);
	UFUNCTION(BlueprintCallable)
	void SetBoostAttackTimer(float Time);
	UFUNCTION(BlueprintCallable)
	void SetActiveFlag(bool Enable, EBtlBitFlagCategory Category);
	UFUNCTION(BlueprintCallable)
	void SavedPrevPointsNative(const FVector& AimPoint, const FVector& CamPoint);
	UFUNCTION(BlueprintCallable)
	void ReturnBeforeFocusUnitNative();
	UFUNCTION(BlueprintCallable)
	void ReturnBeforeFocusUnit();
	UFUNCTION(BlueprintCallable)
	void ResetCameraNative();
	UFUNCTION(BlueprintCallable)
	void ResetCameraInstantNative();
	UFUNCTION(BlueprintCallable)
	void PushMode(EBtlCameraMode CameraMode);
	UFUNCTION(BlueprintCallable)
	void PopMode();
	UFUNCTION(BlueprintCallable)
	void LookupNormalNative2(const FVector& AimPoint, const FVector& CamPoint, FVector& oAimPoint, FVector& oCamPoint);
	UFUNCTION(BlueprintCallable)
	void LookupNormalNative(FVector& AimPoint, FVector& CamPoint);
	UFUNCTION(BlueprintCallable)
	bool IsUseCutsceneToNormalInterpolate();
	UFUNCTION(BlueprintCallable)
	bool IsUseAutoResetTargetAngleRange();
	UFUNCTION(BlueprintCallable)
	bool IsActiveFlag();
	UFUNCTION(BlueprintCallable)
	bool GetRewindFlag();
	UFUNCTION(BlueprintCallable)
	float GetResetCameraProcessTime();
	UFUNCTION(BlueprintCallable)
	float GetResetCameraInterpolateTime();
	UFUNCTION(BlueprintCallable)
	bool GetResetCameraFlag();
	UFUNCTION(BlueprintCallable)
	bool GetResetCameraCutsceneEndFlag();
	UFUNCTION(BlueprintCallable)
	ABtlCharacterBase* GetOverlimitUnit();
	UFUNCTION(BlueprintCallable)
	FRotator GetOffsetRotation();
	UFUNCTION(BlueprintCallable)
	FVector GetOffsetLocation();
	UFUNCTION(BlueprintCallable)
	EBtlCameraMode GetMode();
	UFUNCTION(BlueprintCallable)
	bool GetInterpolateIgnoreFlag();
	UFUNCTION(BlueprintCallable)
	ABtlCharacterBase* GetInspirationUnit();
	/*UFUNCTION(BlueprintCallable)
		TArray<AActor&> GetIgnoreBlockingActors();*/
	UFUNCTION(BlueprintCallable)
	ABtlCharacterBase* GetFocusUnit();
	UFUNCTION(BlueprintCallable)
	USceneComponent* GetFocusPart();
	UFUNCTION(BlueprintCallable)
	float GetFocusOutTime();
	UFUNCTION(BlueprintCallable)
	EBtlCameraFocusLocationState GetFocusLocationState();
	UFUNCTION(BlueprintCallable)
	FVector GetFocusLocation();
	UFUNCTION(BlueprintCallable)
	float GetFocusKeepTime();
	UFUNCTION(BlueprintCallable)
	float GetFocusInTime();
	UFUNCTION(BlueprintCallable)
	float GetFocusDistance();
	UFUNCTION(BlueprintCallable)
	EBtlCameraEasingState GetEasingStateNative(float Time, float InTime, float keepTime, float outTime);
	UFUNCTION(BlueprintCallable)
	float GetDistanceFromDistanceTable(int DistanceTableScore);
	UFUNCTION(BlueprintCallable)
	float GetDefaultFOV();
	UFUNCTION(BlueprintCallable)
	float GetDefaultAimOffsetHeight();
	UFUNCTION(BlueprintCallable)
	FVector GetCamPointNormalNative2(const FVector& AimPoint, const FVector& CamDirectionXY);
	UFUNCTION(BlueprintCallable)
	FVector GetCamPointNormalNative(const FVector& AimPoint, const FVector& CamDirectionXY);
	UFUNCTION(BlueprintCallable)
	FVector GetCamDirectionNormalNative(float Yaw);
	UFUNCTION(BlueprintCallable)
	FVector GetAimPointNormalNative();
	UFUNCTION(BlueprintCallable)
	FVector GetAimOffsetFromDistanceTable(int DistanceTableScore);
	UFUNCTION(BlueprintCallable)
	void ExecutePreBoostStrikeNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void ExecuteNormalNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void ExecuteNormalBattleEndNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void ExecuteInitializeNative();
	UFUNCTION(BlueprintCallable)
	void ExecuteFocusUnitNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void ExecuteFocusLocationNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void ExecuteEasingOverlimitNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void ExecuteEasingInspirationNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void ExecuteCutsceneToNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void ExecuteCounterRaidNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void ExecuteBoostAttackNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void ExecuteAutoCameraResetNative(float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void DetectBlockingNative2(const FVector& AimPoint, const FVector& CamPoint, FVector& EscapePoint);
	UFUNCTION(BlueprintCallable)
	void DetectBlockingNative(FVector& AimPoint, FVector& CamPoint);
	UFUNCTION(BlueprintCallable)
	void ClearOffsetValue();
	UFUNCTION(BlueprintCallable)
	void CalculateNormalCameraNative(float DeltaSeconds, FVector& NewLocation, FRotator& NewRotation,
	                                 FVector& NewAimPoint, FVector& NewCamPoint);
	UFUNCTION(BlueprintCallable)
	int CalculateDistanceTableIndex();
};
