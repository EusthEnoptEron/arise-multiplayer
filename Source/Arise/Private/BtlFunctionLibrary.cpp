// Fill out your copyright notice in the Description page of Project Settings.

#include "BtlFunctionLibrary.h"


EPCOrder UBtlFunctionLibrary::VanGuardOrderToRearGuardOrder(EPCOrder VanGuardOrder)
{
	return static_cast<EPCOrder>(0);
}

void UBtlFunctionLibrary::UpdateControllerAssign()
{
}

void UBtlFunctionLibrary::UnregistTickEvenWhenPausedActor(UObject* WorldContextObject, AActor* Actor)
{
}

void UBtlFunctionLibrary::UnregisterSpecialStrike(UObject* WorldContextObject)
{
}

void UBtlFunctionLibrary::SwapPCOrder(ABtlCharacterBase* Src, ABtlCharacterBase* Dst)
{
}

void UBtlFunctionLibrary::SubPartyTP(int SubTP, ABtlCharacterBase* TargetCharacter)
{
}

void UBtlFunctionLibrary::StopCameraShake(UCameraShake* ShakeInstance, bool bImmediately)
{
}

void UBtlFunctionLibrary::StopAllCameraShakes(bool bImmediately)
{
}

bool UBtlFunctionLibrary::SphereAABBIntersection(const FVector& BoxOrigin, const FVector& BoxExtent,
                                                 const FVector& SphereCenter, float Radius)
{
	return false;
}

UParticleSystemComponent* UBtlFunctionLibrary::SpawnBtlEmitterAtLocation(UObject* WorldContextObject,
                                                                         UParticleSystem* EmitterTemplate,
                                                                         const FVector& Location,
                                                                         const FRotator& Rotation, bool bAutoDestroy,
                                                                         ABtlCharacterBase* Sender)
{
	return nullptr;
}

AActor* UBtlFunctionLibrary::SpawnBtlActor(UClass* SpawnClass, const FTransform& SpawnTransform,
                                           ESpawnActorCollisionHandlingMethod CollisionHandlingOverride, AActor* Owner)
{
	return nullptr;
}

void UBtlFunctionLibrary::SetWeaponVisibility(ABtlCharacterBase* Unit, const FName& BoneName, bool bVisibility)
{
}

void UBtlFunctionLibrary::SetVectorDynamicMaterials(const FName& ParameterName, const FVector& Value,
                                                    TArray<UMaterialInstanceDynamic*>& Materials)
{
}

void UBtlFunctionLibrary::SetRewindInterpolateRank_Move(int Rank)
{
}

void UBtlFunctionLibrary::SetRewindInterpolateRank_Attack(int Rank)
{
}

void UBtlFunctionLibrary::SetRareUnitMaterialParameter(TArray<UMaterialInstanceDynamic*> Materials,
                                                       UMaterialParameterCollection* Collection)
{
}

void UBtlFunctionLibrary::SetPartyTP(int NewTp)
{
}

void UBtlFunctionLibrary::SetOperationMode(EOperationMode Mode, bool ApplyControllerAssign)
{
}

void UBtlFunctionLibrary::SetEncountGroup(TArray<FName> EncountGroupLabels)
{
}


void UBtlFunctionLibrary::SetBehindAutoResetFlag(bool Enable)
{
}

void UBtlFunctionLibrary::SetBattleMapId(const FString& MapID)
{
}

void UBtlFunctionLibrary::SetArenaId(int ArenaId)
{
}

void UBtlFunctionLibrary::ScreenCaptchaFadeOut(UObject* WorldContextObject)
{
}

void UBtlFunctionLibrary::SaveBattleLocation()
{
}

void UBtlFunctionLibrary::RetryFinalizeBattleManager()
{
}

void UBtlFunctionLibrary::RegistTickEvenWhenPausedActor(UObject* WorldContextObject, AActor* Actor)
{
}

void UBtlFunctionLibrary::RefreshTactics()
{
}

void UBtlFunctionLibrary::RebuildMesh(EArisePartyID Target)
{
}

bool UBtlFunctionLibrary::PointBoxIntersection(const FVector& BoxOrigin, const FVector& BoxExtent, const FVector& Point)
{
	return false;
}

bool UBtlFunctionLibrary::OrderUseItem(EArisePartyID User, int ItemID)
{
	return false;
}

void UBtlFunctionLibrary::OrderRetry()
{
}

void UBtlFunctionLibrary::OrderEscape()
{
}

void UBtlFunctionLibrary::OrderArtsMagic(EArisePartyID User, const FName& ArtsId, ABtlCharacterBase* Target)
{
}

bool UBtlFunctionLibrary::LoadBattleLocation()
{
	return false;
}

bool UBtlFunctionLibrary::LineSphereIntersection(const FVector& SphereOrigin, float SphereRadius,
                                                 const FVector& LineStart, const FVector& LineDirection,
                                                 float LineLength)
{
	return false;
}

bool UBtlFunctionLibrary::LineBoxIntersection(const FVector& BoxOrigin, const FVector& BoxExtent,
                                              const FVector& LineStart, const FVector& LineEnd)
{
	return false;
}

bool UBtlFunctionLibrary::IsVanGuard(ABtlCharacterBase* Owner)
{
	return false;
}

bool UBtlFunctionLibrary::IsRightSideActor(AActor* Owner, AActor* Target)
{
	return false;
}

bool UBtlFunctionLibrary::IsRegisteredSpecialStrike(UObject* WorldContextObject)
{
	return false;
}

bool UBtlFunctionLibrary::IsRearGuard(ABtlCharacterBase* Owner)
{
	return false;
}

bool UBtlFunctionLibrary::IsOnNavMeshPoint(AActor* Unit, const FVector& Location)
{
	return false;
}

bool UBtlFunctionLibrary::IsOnNavMesh(AActor* Unit)
{
	return false;
}

bool UBtlFunctionLibrary::IsNormalBattle()
{
	return false;
}

bool UBtlFunctionLibrary::IsMoveRightSide(ABtlCharacterBase* Owner, float Distance)
{
	return false;
}

bool UBtlFunctionLibrary::IsMoveLeftSide(ABtlCharacterBase* Owner, float Distance)
{
	return false;
}

bool UBtlFunctionLibrary::IsMoveForward(ABtlCharacterBase* Owner, float Distance)
{
	return false;
}

bool UBtlFunctionLibrary::IsMoveBackward(ABtlCharacterBase* Owner, float Distance)
{
	return false;
}

bool UBtlFunctionLibrary::IsMoveAvailable(ABtlCharacterBase* Owner, const FVector& Location)
{
	return false;
}

bool UBtlFunctionLibrary::IsLeftSideActor(AActor* Owner, AActor* Target)
{
	return false;
}

bool UBtlFunctionLibrary::IsInFructum(AActor* Actor)
{
	return false;
}

bool UBtlFunctionLibrary::IsInBattle(ABtlCharacterBase* Owner)
{
	return false;
}

bool UBtlFunctionLibrary::IsFullCp(UObject* WorldContextObject)
{
	return false;
}


bool UBtlFunctionLibrary::IsForwardCheck(AActor* Owner, AActor* Target, float Angle)
{
	return false;
}

bool UBtlFunctionLibrary::IsForwardActor(AActor* Owner, AActor* Target)
{
	return false;
}

bool UBtlFunctionLibrary::IsEscaping()
{
	return false;
}

bool UBtlFunctionLibrary::IsEscapeComplete()
{
	return false;
}

bool UBtlFunctionLibrary::IsEffectiveArmor(ABtlCharacterBase* Attacker, ABtlCharacterBase* Receiver)
{
	return false;
}

bool UBtlFunctionLibrary::IsBossBattle()
{
	return false;
}

bool UBtlFunctionLibrary::IsBoostAttacking_AnyCharacter(ABtlCharacterBase* IgnoreUnit)
{
	return false;
}

bool UBtlFunctionLibrary::IsBackwardActor(AActor* Owner, AActor* Target)
{
	return false;
}

bool UBtlFunctionLibrary::IsAvailableStrikeAttackUnit(ABtlCharacterBase* Unit)
{
	return false;
}

bool UBtlFunctionLibrary::IsAvailableStrikeAttack(ABtlCharacterBase* Unit, ABtlCharacterBase* Striker)
{
	return false;
}

bool UBtlFunctionLibrary::IntersectHeight(float CheckHeightBtm, float CheckHeightTop, ABtlCharacterBase* Unit)
{
	return false;
}

void UBtlFunctionLibrary::InputControl(bool Enable)
{
}

void UBtlFunctionLibrary::InitializeBattleManager()
{
}

TArray<ABtlCharacterBase*> UBtlFunctionLibrary::InFructumList(UObject* WorldContextObject,
                                                              TArray<ABtlCharacterBase*> Actors)
{
	return {};
}

int UBtlFunctionLibrary::InFructumCountOf(UObject* WorldContextObject, TArray<AActor*> Actors)
{
	return 0;
}

bool UBtlFunctionLibrary::InArena()
{
	return false;
}

ABattleUIManager* UBtlFunctionLibrary::GetUIManager()
{
	return nullptr;
}

int UBtlFunctionLibrary::GetTPPercent()
{
	return 0;
}

int UBtlFunctionLibrary::GetTPMax(UObject* WorldContextObject)
{
	return 0;
}

int UBtlFunctionLibrary::GetTPCostByPartyId(EArisePartyID User, int TP)
{
	return 0;
}

int UBtlFunctionLibrary::GetTp()
{
	return 0;
}

int UBtlFunctionLibrary::GetTargetCursorComponentNum(ABtlCharacterBase* Target)
{
	return 0;
}

float UBtlFunctionLibrary::GetTargetComponentDistanceXY(ABtlCharacterBase* Unit)
{
	return 0;
}

ABtlCharacterBase* UBtlFunctionLibrary::GetSwitchingUnit(ABtlCharacterBase* Unit)
{
	return nullptr;
}

float UBtlFunctionLibrary::GetStepReserveTime(ABtlCharacterBase* Unit)
{
	return 0;
}

UBtlStateManager* UBtlFunctionLibrary::GetStateManager()
{
	return nullptr;
}

ABtlCharacterBase* UBtlFunctionLibrary::GetStartControlUnit(UObject* WorldContextObject)
{
	return nullptr;
}

float UBtlFunctionLibrary::GetSizeComponentUpperZ(ABtlCharacterBase* Unit)
{
	return 0;
}

float UBtlFunctionLibrary::GetRunawayTimer()
{
	return 0;
}

float UBtlFunctionLibrary::GetRunawaySuccessfulTime()
{
	return 0;
}

float UBtlFunctionLibrary::GetRareSymbolRate()
{
	return 0;
}

void UBtlFunctionLibrary::GetPcOrder(ABtlCharacterBase* Owner, bool& IsValid, EPCOrder& Order)
{
}

EOperationMode UBtlFunctionLibrary::GetOperationMode()
{
	return {};
}


FVector UBtlFunctionLibrary::GetMarkerLocation(const FName& FindMarker)
{
	return {};
}

FVector UBtlFunctionLibrary::GetLocationBtlUnitSizeComponent(ABtlCharacterBase* Unit)
{
	return {};
}

float UBtlFunctionLibrary::GetJumpReserveTime(ABtlCharacterBase* Unit)
{
	return 0;
}

float UBtlFunctionLibrary::GetItemRecastTimerRate()
{
	return 0;
}

float UBtlFunctionLibrary::GetItemRecastTimer()
{
	return 0;
}

float UBtlFunctionLibrary::GetInputReserveTime()
{
	return 0;
}

float UBtlFunctionLibrary::GetInputBtlCameraYaw(float Delta, float InputValue)
{
	return 0;
}

float UBtlFunctionLibrary::GetInputBtlCameraHeight(float Delta, float InputValue)
{
	return 0;
}

float UBtlFunctionLibrary::GetHeightDifference(ABtlCharacterBase* UnitA, ABtlCharacterBase* UnitB)
{
	return 0;
}

int UBtlFunctionLibrary::GetGigantTypeNumForPlayRecord()
{
	return 0;
}

TArray<FName> UBtlFunctionLibrary::GetEncountGroup()
{
	return {};
}

float UBtlFunctionLibrary::GetDistanceBtlUnitToLocationXY(ABtlCharacterBase* UnitA, const FVector& Location)
{
	return 0;
}

float UBtlFunctionLibrary::GetDistanceBtlUnitSizeComponentZ(ABtlCharacterBase* UnitA, ABtlCharacterBase* UnitB)
{
	return 0;
}

float UBtlFunctionLibrary::GetDistanceBtlUnitSizeComponentXY(ABtlCharacterBase* UnitA, ABtlCharacterBase* UnitB)
{
	return 0;
}

float UBtlFunctionLibrary::GetDistanceBtlUnitCollisions(ABtlCharacterBase* Owner, ABtlCharacterBase* Other)
{
	return 0;
}

float UBtlFunctionLibrary::GetDistanceBtlCapsuleToLocationXY(ABtlCharacterBase* UnitA, const FVector& Location)
{
	return 0;
}

float UBtlFunctionLibrary::GetDifferenceYaw(AActor* From, AActor* To)
{
	return 0;
}

FVector UBtlFunctionLibrary::GetClosestIn4Directions(ABtlCharacterBase* Unit, const FVector& Check)
{
	return {};
}

FString UBtlFunctionLibrary::GetBattleMapId()
{
	return {};
}

ABtlManager* UBtlFunctionLibrary::GetBattleManager(UObject* WorldContextObject)
{
	return nullptr;
}

UBtlEventDispatcher* UBtlFunctionLibrary::GetBattleEventDispatcher(UObject* WorldContextObject)
{
	return nullptr;
}

UDataTable* UBtlFunctionLibrary::GetBattleDataTable(UObject* WorldContextObject, const FName& TableName)
{
	return nullptr;
}

ABtl_Camera* UBtlFunctionLibrary::GetBattleCamera()
{
	return nullptr;
}

int UBtlFunctionLibrary::GetArenaId()
{
	return 0;
}

int UBtlFunctionLibrary::GetActualTPCost(int TP, ABtlCharacterBase* UseCharacter)
{
	return 0;
}

void UBtlFunctionLibrary::FocusOnUnit(ABtlCharacterBase* TargetUnit, USceneComponent* Part)
{
}

float UBtlFunctionLibrary::FindDeltaAngleDegrees(float From, float To)
{
	return 0;
}

void UBtlFunctionLibrary::FinalizeBattleManager()
{
}

void UBtlFunctionLibrary::DestroyEffects()
{
}

EArisePartyID UBtlFunctionLibrary::ConvertBIDToArisePartyId(const FName& BID)
{
	return {};
}

FName UBtlFunctionLibrary::ConvertArisePartyIdToBID(EArisePartyID PartyId)
{
	return {};
}

ABtlCharacterBase* UBtlFunctionLibrary::CheckAutoTarget(ABtlCharacterBase* Unit)
{
	return nullptr;
}

void UBtlFunctionLibrary::ChangeOperationMode(EOperationMode NewMode, bool ApplyControllerAssign)
{
}

void UBtlFunctionLibrary::ChangeControlUnitProv(EArisePartyID PartyId)
{
}

void UBtlFunctionLibrary::ChangeControlCharacter(ABtlCharacterBase* ChangeCharacter, bool ApplyControllerAssign)
{
}

bool UBtlFunctionLibrary::CanUseItem()
{
	return false;
}

bool UBtlFunctionLibrary::CanRetry()
{
	return false;
}

bool UBtlFunctionLibrary::CanEscape()
{
	return false;
}

void UBtlFunctionLibrary::CancelEscape()
{
}

int UBtlFunctionLibrary::CalculateHealMagic(EArisePartyID User, EArisePartyID Target, const FName& HealMagic)
{
	return 0;
}

FTransform UBtlFunctionLibrary::CalcSmashTransform(const FName& StrikeLabel)
{
	return {};
}

void UBtlFunctionLibrary::BtlPrintString(UObject* WorldContextObject, const FString& inString, bool bPrintToScreen,
                                         bool bPrintToLog, const FLinearColor& TextColor, float Duration)
{
}

void UBtlFunctionLibrary::ApplyArtsSettingsChange(EArisePartyID User)
{
}

void UBtlFunctionLibrary::AddPartyTP(int AddTP)
{
}

void UBtlFunctionLibrary::AddContinuousBattleBonus(const FName& TableID)
{
}
