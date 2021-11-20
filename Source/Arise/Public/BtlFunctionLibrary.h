// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BtlCharacterBase.h"
#include "BtlEventDispatcher.h"
#include "BtlStateManager.h"
#include "Enums.h"
#include "Camera/CameraShake.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Particles/ParticleSystemComponent.h"
#include "BtlFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARISE_API UBtlFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	
	UFUNCTION(BlueprintCallable)
	static EPCOrder VanGuardOrderToRearGuardOrder(EPCOrder VanGuardOrder);
	
	UFUNCTION(BlueprintCallable)
	static void UpdateControllerAssign();
	
	UFUNCTION(BlueprintCallable)
	static void UnregistTickEvenWhenPausedActor(UObject* WorldContextObject, AActor* Actor);
	
	UFUNCTION(BlueprintCallable)
	static void UnregisterSpecialStrike(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable)
	static void SwapPCOrder(ABtlCharacterBase* Src, ABtlCharacterBase* Dst);
	
	UFUNCTION(BlueprintCallable)
	static void SubPartyTP(int SubTP, ABtlCharacterBase* TargetCharacter);
	
	UFUNCTION(BlueprintCallable)
	static void StopCameraShake(UCameraShake* ShakeInstance, bool bImmediately);
	
	UFUNCTION(BlueprintCallable)
	static void StopAllCameraShakes(bool bImmediately);
	
	UFUNCTION(BlueprintCallable)
	static bool SphereAABBIntersection(const FVector& BoxOrigin, const FVector& BoxExtent, const FVector& SphereCenter,
	                            float Radius);
	// UParticleSystemComponent* SpawnBtlEmitterAttached(UParticleSystem* EmitterTemplate,
	//                                                   USceneComponent* AttachToComponent, const FName& AttachPointName,
	//                                                   const FVector& Location, const FRotator& Rotation,
	//                                                   TEnumAsByte<EAttachLocation> LocationType, bool bAutoDestroy,
	//                                                   ABtlCharacterBase* Sender);
	
	UFUNCTION(BlueprintCallable)
	static UParticleSystemComponent* SpawnBtlEmitterAtLocation(UObject* WorldContextObject, UParticleSystem* EmitterTemplate,
	                                                    const FVector& Location, const FRotator& Rotation,
	                                                    bool bAutoDestroy, ABtlCharacterBase* Sender);
	
	UFUNCTION(BlueprintCallable)
	static AActor* SpawnBtlActor(UClass* SpawnClass, const FTransform& SpawnTransform,
	                      ESpawnActorCollisionHandlingMethod CollisionHandlingOverride, AActor* Owner);
	
	UFUNCTION(BlueprintCallable)
	static void SetWeaponVisibility(ABtlCharacterBase* Unit, const FName& BoneName, bool bVisibility);
	
	UFUNCTION(BlueprintCallable)
	static void SetVectorDynamicMaterials(const FName& ParameterName, const FVector& Value,
	                                      TArray<UMaterialInstanceDynamic*>& Materials);
	// void SetTargetCursorEnableByCollision(ABtlCharacterBase* Target, bool on,
	//                                       UBtlDamageSphereComponent* ReferenceDmgCollision);
	
	
	UFUNCTION(BlueprintCallable)
	static void SetRewindInterpolateRank_Move(int Rank);
	
	UFUNCTION(BlueprintCallable)
	static void SetRewindInterpolateRank_Attack(int Rank);
	
	UFUNCTION(BlueprintCallable)
	static void SetRareUnitMaterialParameter(TArray<UMaterialInstanceDynamic*> Materials,
	                                  UMaterialParameterCollection* Collection);
	
	UFUNCTION(BlueprintCallable)
	static void SetPartyTP(int NewTp);
	
	UFUNCTION(BlueprintCallable)
	static void SetOperationMode(EOperationMode Mode, bool ApplyControllerAssign);
	// void SetEncountParam(TArray<FBtlEncountGroupParam> EncountParam);
	
	UFUNCTION(BlueprintCallable)
	static void SetEncountGroup(TArray<FName> EncountGroupLabels);
	
	
	UFUNCTION(BlueprintCallable)
	static void SetBehindAutoResetFlag(bool Enable);
	
	UFUNCTION(BlueprintCallable)
	static void SetBattleMapId(const FString& MapID);
	// void SetBattleLocation(ABtl_Location* BtlLocation);
	
	UFUNCTION(BlueprintCallable)
	static void SetArenaId(int ArenaId);
	
	UFUNCTION(BlueprintCallable)
	static void ScreenCaptchaFadeOut(UObject* WorldContextObject);
	// void ScreenCaptchaAddViewport(UObject* WorldContextObject,
	//                               const FBtlScreenCaptchaWidget_DatabaseLabel& WidgetLabel);
	
	UFUNCTION(BlueprintCallable)
	static void SaveBattleLocation();
	
	UFUNCTION(BlueprintCallable)
	static void RetryFinalizeBattleManager();
	
	UFUNCTION(BlueprintCallable)
	static void RegistTickEvenWhenPausedActor(UObject* WorldContextObject, AActor* Actor);
	// bool RegisterSpecialStrike(UObject* WorldContextObject, const FName& StrikeLabel,
	//                            TArray<EArisePartyID> AboutPartyId, const FBtlUnitID& TargetUnitId,
	//                            const FTransform& Transform, bool RecoveryTransform, bool RecoveryMeshYaw,
	//                            bool IgnoreBoostStrike);
	
	UFUNCTION(BlueprintCallable)
	static void RefreshTactics();
	
	UFUNCTION(BlueprintCallable)
	static void RebuildMesh(EArisePartyID Target);
	
	UFUNCTION(BlueprintCallable)
	static bool PointBoxIntersection(const FVector& BoxOrigin, const FVector& BoxExtent, const FVector& Point);
	// UCameraShake* PlayCameraShake(UClass* ShakeClass, float Scale, TEnumAsByte<ECameraAnimPlaySpace> PlaySpace,
	//                               const FRotator& UserPlaySpaceRot);
	
	UFUNCTION(BlueprintCallable)
	static bool OrderUseItem(EArisePartyID User, int ItemID);
	// ETacticsIndex OrderTacticsPrev();
	// ETacticsIndex OrderTacticsNext();
	// ETacticsIndex OrderTactics(ETacticsIndex Index);
	
	UFUNCTION(BlueprintCallable)
	static void OrderRetry();
	
	UFUNCTION(BlueprintCallable)
	static void OrderEscape();
	
	UFUNCTION(BlueprintCallable)
	static void OrderArtsMagic(EArisePartyID User, const FName& ArtsId, ABtlCharacterBase* Target);
	
	UFUNCTION(BlueprintCallable)
	static bool LoadBattleLocation();
	
	UFUNCTION(BlueprintCallable)
	static bool LineSphereIntersection(const FVector& SphereOrigin, float SphereRadius, const FVector& LineStart,
	                            const FVector& LineDirection, float LineLength);
	
	UFUNCTION(BlueprintCallable)
	static bool LineBoxIntersection(const FVector& BoxOrigin, const FVector& BoxExtent, const FVector& LineStart,
	                         const FVector& LineEnd);
	
	UFUNCTION(BlueprintCallable)
	static bool IsVanGuard(ABtlCharacterBase* Owner);
	
	UFUNCTION(BlueprintCallable)
	static bool IsRightSideActor(AActor* Owner, AActor* Target);
	
	UFUNCTION(BlueprintCallable)
	static bool IsRegisteredSpecialStrike(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable)
	static bool IsRearGuard(ABtlCharacterBase* Owner);
	
	UFUNCTION(BlueprintCallable)
	static bool IsOnNavMeshPoint(AActor* Unit, const FVector& Location);
	
	UFUNCTION(BlueprintCallable)
	static bool IsOnNavMesh(AActor* Unit);
	// bool IsNotEqual_ArtsHandle_ArtsLabel(const FBtlArtsHandle& Lhs, const FBtlArts_XmlDatatableLabel& Rhs);
	// bool IsNotEqual_ArtsHandle(const FBtlArtsHandle& Lhs, const FBtlArtsHandle& Rhs);
	
	UFUNCTION(BlueprintCallable)
	static bool IsNormalBattle();
	
	UFUNCTION(BlueprintCallable)
	static bool IsMoveRightSide(ABtlCharacterBase* Owner, float Distance);
	
	UFUNCTION(BlueprintCallable)
	static bool IsMoveLeftSide(ABtlCharacterBase* Owner, float Distance);
	
	UFUNCTION(BlueprintCallable)
	static bool IsMoveForward(ABtlCharacterBase* Owner, float Distance);
	
	UFUNCTION(BlueprintCallable)
	static bool IsMoveBackward(ABtlCharacterBase* Owner, float Distance);
	
	UFUNCTION(BlueprintCallable)
	static bool IsMoveAvailable(ABtlCharacterBase* Owner, const FVector& Location);
	
	UFUNCTION(BlueprintCallable)
	static bool IsLeftSideActor(AActor* Owner, AActor* Target);
	
	UFUNCTION(BlueprintCallable)
	static bool IsInFructum(AActor* Actor);
	
	UFUNCTION(BlueprintCallable)
	static bool IsInBattle(ABtlCharacterBase* Owner);
	
	UFUNCTION(BlueprintCallable)
	static bool IsFullCp(UObject* WorldContextObject);
	
	// UFUNCTION(BlueprintCallable)
	// static bool IsForwardSmallFellow(ABtlCharacterBase* Owner, float DistanceRange, float AngleRange,
	//                           ABtlCharacterBase** FindFellow);
	
	UFUNCTION(BlueprintCallable)
	static bool IsForwardCheck(AActor* Owner, AActor* Target, float Angle);
	
	UFUNCTION(BlueprintCallable)
	static bool IsForwardActor(AActor* Owner, AActor* Target);
	
	UFUNCTION(BlueprintCallable)
	static bool IsEscaping();
	
	UFUNCTION(BlueprintCallable)
	static bool IsEscapeComplete();
	// bool IsEqual_ArtsHandle_ArtsLabel(const FBtlArtsHandle& Lhs, const FBtlArts_XmlDatatableLabel& Rhs);
	// bool IsEqual_ArtsHandle(const FBtlArtsHandle& Lhs, const FBtlArtsHandle& Rhs);
	
	UFUNCTION(BlueprintCallable)
	static bool IsEffectiveArmor(ABtlCharacterBase* Attacker, ABtlCharacterBase* Receiver);
	
	UFUNCTION(BlueprintCallable)
	static bool IsBossBattle();
	
	UFUNCTION(BlueprintCallable)
	static bool IsBoostAttacking_AnyCharacter(ABtlCharacterBase* IgnoreUnit);
	
	UFUNCTION(BlueprintCallable)
	static bool IsBackwardActor(AActor* Owner, AActor* Target);
	
	UFUNCTION(BlueprintCallable)
	static bool IsAvailableStrikeAttackUnit(ABtlCharacterBase* Unit);
	
	UFUNCTION(BlueprintCallable)
	static bool IsAvailableStrikeAttack(ABtlCharacterBase* Unit, ABtlCharacterBase* Striker);
	
	UFUNCTION(BlueprintCallable)
	static bool IntersectHeight(float CheckHeightBtm, float CheckHeightTop, ABtlCharacterBase* Unit);
	
	UFUNCTION(BlueprintCallable)
	static void InputControl(bool Enable);
	
	UFUNCTION(BlueprintCallable)
	static void InitializeBattleManager();
	
	UFUNCTION(BlueprintCallable)
	static TArray<ABtlCharacterBase*> InFructumList(UObject* WorldContextObject, TArray<ABtlCharacterBase*> Actors);
	
	UFUNCTION(BlueprintCallable)
	static int InFructumCountOf(UObject* WorldContextObject, TArray<AActor*> Actors);
	
	UFUNCTION(BlueprintCallable)
	static bool InArena();
	// ABtlVoiceManager* GetVoiceManager(UObject* WorldContextObject);
	// ABattleUIManager* GetUIManager();
	
	UFUNCTION(BlueprintCallable)
	static int GetTPPercent();
	
	UFUNCTION(BlueprintCallable)
	static int GetTPMax(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable)
	static int GetTPCostByPartyId(EArisePartyID User, int TP);
	
	UFUNCTION(BlueprintCallable)
	static int GetTp();
	// UBtlTargetCursorManager* GetTargetCursorManager();
	
	UFUNCTION(BlueprintCallable)
	static int GetTargetCursorComponentNum(ABtlCharacterBase* Target);
	
	UFUNCTION(BlueprintCallable)
	static float GetTargetComponentDistanceXY(ABtlCharacterBase* Unit);
	
	UFUNCTION(BlueprintCallable)
	static ABtlCharacterBase* GetSwitchingUnit(ABtlCharacterBase* Unit);
	
	UFUNCTION(BlueprintCallable)
	static float GetStepReserveTime(ABtlCharacterBase* Unit);

	UFUNCTION(BlueprintCallable)
	static UBtlStateManager* GetStateManager();
	
	UFUNCTION(BlueprintCallable)
	static ABtlCharacterBase* GetStartControlUnit(UObject* WorldContextObject);
	// ABtl_SpectatorPawn* GetSpectatorPawn();
	
	UFUNCTION(BlueprintCallable)
	static float GetSizeComponentUpperZ(ABtlCharacterBase* Unit);
	
	UFUNCTION(BlueprintCallable)
	static float GetRunawayTimer();
	
	UFUNCTION(BlueprintCallable)
	static float GetRunawaySuccessfulTime();
	// ABtlResultManager* GetResultManager(UObject* WorldContextObject);
	// FBtlSpecialStrikePeriod GetRegisteredSpecialStrike(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable)
	static float GetRareSymbolRate();
	// ABtlPostProcessVolumeManager* GetPostProcessVolumeManager();
	// AArisePlayerCameraManager* GetPlayerCameraManager();
	
	UFUNCTION(BlueprintCallable)
	static void GetPcOrder(ABtlCharacterBase* Owner, bool& IsValid, EPCOrder& Order);
	// UArisePartyOrder* GetPartyOrder();
	
	UFUNCTION(BlueprintCallable)
	static EOperationMode GetOperationMode();
	
	
	UFUNCTION(BlueprintCallable)
	static FVector GetMarkerLocation(const FName& FindMarker);
	// ABtlMagicManagerActor* GetMagicManager();
	
	UFUNCTION(BlueprintCallable)
	static FVector GetLocationBtlUnitSizeComponent(ABtlCharacterBase* Unit);
	
	UFUNCTION(BlueprintCallable)
	static float GetJumpReserveTime(ABtlCharacterBase* Unit);
	
	UFUNCTION(BlueprintCallable)
	static float GetItemRecastTimerRate();
	
	UFUNCTION(BlueprintCallable)
	static float GetItemRecastTimer();
	
	UFUNCTION(BlueprintCallable)
	static float GetInputReserveTime();
	
	UFUNCTION(BlueprintCallable)
	static float GetInputBtlCameraYaw(float Delta, float InputValue);
	
	UFUNCTION(BlueprintCallable)
	static float GetInputBtlCameraHeight(float Delta, float InputValue);
	
	UFUNCTION(BlueprintCallable)
	static float GetHeightDifference(ABtlCharacterBase* UnitA, ABtlCharacterBase* UnitB);
	
	UFUNCTION(BlueprintCallable)
	static int GetGigantTypeNumForPlayRecord();
	// int GetEQSBestIndex(UEnvQueryInstanceBlueprintWrapper* EQSQueryInstance);
	// bool GetEncountTopParam(UObject* WorldContextObject, FBtlEncountGroupParam* EncountParam);
	// TArray<FBtlEncountGroupParam> GetEncountParam();
	
	UFUNCTION(BlueprintCallable)
	static TArray<FName> GetEncountGroup();
	
	UFUNCTION(BlueprintCallable)
	static float GetDistanceBtlUnitToLocationXY(ABtlCharacterBase* UnitA, const FVector& Location);
	
	UFUNCTION(BlueprintCallable)
	static float GetDistanceBtlUnitSizeComponentZ(ABtlCharacterBase* UnitA, ABtlCharacterBase* UnitB);
	
	UFUNCTION(BlueprintCallable)
	static float GetDistanceBtlUnitSizeComponentXY(ABtlCharacterBase* UnitA, ABtlCharacterBase* UnitB);
	
	UFUNCTION(BlueprintCallable)
	static float GetDistanceBtlUnitCollisions(ABtlCharacterBase* Owner, ABtlCharacterBase* Other);
	
	UFUNCTION(BlueprintCallable)
	static float GetDistanceBtlCapsuleToLocationXY(ABtlCharacterBase* UnitA, const FVector& Location);
	// FString GetDisplayName_ArtsHandle(const FBtlArtsHandle& Handle);
	
	UFUNCTION(BlueprintCallable)
	static float GetDifferenceYaw(AActor* From, AActor* To);
	// UBtlTargetCursorComponent* GetDefaultTargetCursorComponent(ABtlCharacterBase* Target);
	// ABtlDebugCommand* GetDebugCommand(UObject* WorldContextObject);
	// ETacticsIndex GetCurrentTactics();
	// UBtlStateMain* GetCurrentStateMain();
	// ABtlCollisionManager* GetCollisionManager(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable)
	static FVector GetClosestIn4Directions(ABtlCharacterBase* Unit, const FVector& Check);
	// int GetBtlRankOrder(EBtlRank CheckRank);
	
	UFUNCTION(BlueprintCallable)
	static FString GetBattleMapId();
	// ABtlManager* GetBattleManager(UObject* WorldContextObject);
	// ABtl_Location* GetBattleLocation();
	
	UFUNCTION(BlueprintCallable)
	static UBtlEventDispatcher* GetBattleEventDispatcher(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable)
	static UDataTable* GetBattleDataTable(UObject* WorldContextObject, const FName& TableName);
	
	UFUNCTION(BlueprintCallable)
	static ABtl_Camera* GetBattleCamera();
	
	UFUNCTION(BlueprintCallable)
	static int GetArenaId();
	
	UFUNCTION(BlueprintCallable)
	static int GetActualTPCost(int TP, ABtlCharacterBase* UseCharacter);
	
	UFUNCTION(BlueprintCallable)
	static void FocusOnUnit(ABtlCharacterBase* TargetUnit, USceneComponent* Part);
	
	UFUNCTION(BlueprintCallable)
	static float FindDeltaAngleDegrees(float From, float To);
	
	UFUNCTION(BlueprintCallable)
	static void FinalizeBattleManager();
	
	UFUNCTION(BlueprintCallable)
	static void DestroyEffects();
	
	UFUNCTION(BlueprintCallable)
	static EArisePartyID ConvertBIDToArisePartyId(const FName& BID);
	// EAriseGuestID ConvertBIDToAriseGuestId(const FName& BID);
	
	UFUNCTION(BlueprintCallable)
	static FName ConvertArisePartyIdToBID(EArisePartyID PartyId);
	// FName ConvertAriseGuestIdToBID(EAriseGuestID GuestId);
	
	UFUNCTION(BlueprintCallable)
	static ABtlCharacterBase* CheckAutoTarget(ABtlCharacterBase* Unit);
	
	UFUNCTION(BlueprintCallable)
	static void ChangeOperationMode(EOperationMode NewMode, bool ApplyControllerAssign);
	
	UFUNCTION(BlueprintCallable)
	static void ChangeControlUnitProv(EArisePartyID PartyId);
	
	UFUNCTION(BlueprintCallable)
	static void ChangeControlCharacter(ABtlCharacterBase* ChangeCharacter, bool ApplyControllerAssign);
	
	UFUNCTION(BlueprintCallable)
	static bool CanUseItem();
	
	UFUNCTION(BlueprintCallable)
	static bool CanRetry();
	
	UFUNCTION(BlueprintCallable)
	static bool CanEscape();
	
	UFUNCTION(BlueprintCallable)
	static void CancelEscape();
	
	UFUNCTION(BlueprintCallable)
	static int CalculateHealMagic(EArisePartyID User, EArisePartyID Target, const FName& HealMagic);
	
	UFUNCTION(BlueprintCallable)
	static FTransform CalcSmashTransform(const FName& StrikeLabel);
	
	UFUNCTION(BlueprintCallable)
	static void BtlPrintString(UObject* WorldContextObject, const FString& inString, bool bPrintToScreen, bool bPrintToLog,
	                    const FLinearColor& TextColor, float Duration);
	// void ApplyDebugParamPreset(const FBtlDebugParamPreset& Preset);
	// void ApplyDebugGameProgressSetup(const FBtlDebugGameProgressPresetParam& Preset,
	//                                  const FBtlDebugGameProgressAliasSkillPriorityTable& Priority);
	
	UFUNCTION(BlueprintCallable)
	static void ApplyArtsSettingsChange(EArisePartyID User);
	
	UFUNCTION(BlueprintCallable)
	static void AddPartyTP(int AddTP);
	
	UFUNCTION(BlueprintCallable)
	static void AddContinuousBattleBonus(const FName& TableID);
};
