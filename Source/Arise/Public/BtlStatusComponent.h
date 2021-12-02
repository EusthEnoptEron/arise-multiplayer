// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BtlStatusComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARISE_API UBtlStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBtlStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable)
	void UpdateEquipment();

	UFUNCTION(BlueprintCallable)
	void SubStrikeAttack(float Value);

	UFUNCTION(BlueprintCallable)
	void SubSPInternal(float Value);

	UFUNCTION(BlueprintCallable)
	void SubSP(int Value);

	UFUNCTION(BlueprintCallable)
	void SubOverlimitTime(float Value);

	UFUNCTION(BlueprintCallable)
	void SubOverlimit(int Value);

	UFUNCTION(BlueprintCallable)
	void SubHP(int Value);

	UFUNCTION(BlueprintCallable)
	void SubGuardBreak(int Value);

	UFUNCTION(BlueprintCallable)
	void SubEnduranceValue(int Value);

	UFUNCTION(BlueprintCallable)
	void SubCP(int Value);
	//
	// UFUNCTION(BlueprintCallable)
	// void SetWeakPowerStrengthenRate(EBtlElementType Type, float Value);
	//
	UFUNCTION(BlueprintCallable)
	void SetUnlimitedOverlimit(bool IsUnlimited);

	// UFUNCTION(BlueprintCallable)
	// void SetUnitParameterBySlot(const FBtlUnitParameterLabel& Label, const FName& SlotLabel, float Value);
	//
	// UFUNCTION(BlueprintCallable)
	// void SetUnitParameter(const FBtlUnitParameterLabel& Label, float Value);
	//
	UFUNCTION(BlueprintCallable)
	void SetTotalExp(int Value);

	UFUNCTION(BlueprintCallable)
	void SetStrikeAttackRecoverValue(bool Overwrite, float Value);

	UFUNCTION(BlueprintCallable)
	void SetStrikeAttack(float Value);

	UFUNCTION(BlueprintCallable)
	void SetSPInternal(float Value);

	UFUNCTION(BlueprintCallable)
	void SetSP(int Value);

	UFUNCTION(BlueprintCallable)
	void SetPenetrate(int Value);

	UFUNCTION(BlueprintCallable)
	void SetOverwriteDamageEffect(const FName& Label, bool bClearOnDamage);

	UFUNCTION(BlueprintCallable)
	void SetOverlimitTimeMax(float Value);

	UFUNCTION(BlueprintCallable)
	void SetOverlimitTime(float Value);

	UFUNCTION(BlueprintCallable)
	void SetOverLimit(int Value);

	UFUNCTION(BlueprintCallable)
	void SetLv(int Value);

	UFUNCTION(BlueprintCallable)
	void SetHPSection(TArray<float> HPSectionList);

	UFUNCTION(BlueprintCallable)
	void SetHPRate(float HpRate);

	UFUNCTION(BlueprintCallable)
	void SetHPMax(int Value);

	UFUNCTION(BlueprintCallable)
	void SetHP(int Value);

	UFUNCTION(BlueprintCallable)
	void SetHateRate(float Value);

	UFUNCTION(BlueprintCallable)
	void SetHalfSPTriggerWeight(float Weight);

	UFUNCTION(BlueprintCallable)
	void SetGuardBreak(int Value);

	UFUNCTION(BlueprintCallable)
	void SetEnduranceValue(int Value);

	UFUNCTION(BlueprintCallable)
	void SetEndurance(int Value);

	UFUNCTION(BlueprintCallable)
	void SetElementDef(int Value);

	UFUNCTION(BlueprintCallable)
	void SetElementAtk(int Value);
	//
	// UFUNCTION(BlueprintCallable)
	// void SetDrawAbnormalType(EAbnormalType Type);
	//
	UFUNCTION(BlueprintCallable)
	void SetDrawAbnormalTime(float Value);

	UFUNCTION(BlueprintCallable)
	void SetDef(int Value);

	UFUNCTION(BlueprintCallable)
	void SetDamageEffect(const FName& Label);

	UFUNCTION(BlueprintCallable)
	void SetBombBullet(int BulletNum);

	UFUNCTION(BlueprintCallable)
	void SetAtk(int Value);

	UFUNCTION(BlueprintCallable)
	void SetArtsLearn(const FName& ArtsLabel);

	UFUNCTION(BlueprintCallable)
	void SetAlleviation(int Value);
	//
	// UFUNCTION(BlueprintCallable)
	// void SetAbnormalValue(EAbnormalType Type, int Value);
	//
	// UFUNCTION(BlueprintCallable)
	// void SetAbnormalTimer(EAbnormalType Type, float Value);
	//
	// UFUNCTION(BlueprintCallable)
	// void SetAbnormalCutRate(EAbnormalType Type, float Value);
	//
	// UFUNCTION(BlueprintCallable)
	// void SetAbnormalCount(EAbnormalType Type, int Value);
	//
	UFUNCTION(BlueprintCallable)
	void RemoveSimpleBuff(const FString& BuffName);

	UFUNCTION(BlueprintCallable)
	void RemoveDamageEffectSkill(const FName& SkillLabel);

	UFUNCTION(BlueprintCallable)
	void OnPostBeginBattle();

	UFUNCTION(BlueprintCallable)
	void OnParameterHandleChanged(int HandleIndex, float OldValue, float NewValue);

	UFUNCTION(BlueprintCallable)
	void OnHpBonusChanged(int HandleIndex, float OldValue, float NewValue);

	UFUNCTION(BlueprintCallable)
	void OnEndTimeStop();

	UFUNCTION(BlueprintCallable)
	void OnEndBattle();

	UFUNCTION(BlueprintCallable)
	void OnBeginTimeStop();

	// UFUNCTION(BlueprintCallable)
	// void OnBattleFinished(EBtlResultType Type);
	//
	UFUNCTION(BlueprintCallable)
	void OnBaseHpBonusChanged(int HandleIndex, float OldValue, float NewValue);

	// UFUNCTION(BlueprintCallable)
	// FName STATIC_MakeUnitParameterLabel(const FBtlUnitParameterLabel& Label);
	//
	UFUNCTION(BlueprintCallable)
	void K2_GetSkillParameterByInt(const FString& SkillName, bool& IsActiveSkill, float& SkillParameter);

	UFUNCTION(BlueprintCallable)
	void K2_GetSkillParameter(const FString& SkillName, bool& IsActiveSkill, float& SkillParameter);

	// UFUNCTION(BlueprintCallable)
	// bool STATIC_IsValidAbnormalType(EAbnormalType Type);
	//
	UFUNCTION(BlueprintCallable)
	bool IsUnlimitedOverlimit();

	UFUNCTION(BlueprintCallable)
	bool IsStrikeFull();

	UFUNCTION(BlueprintCallable)
	bool IsSpecificSkillLearned(const FString& SkillName);
	//
	// UFUNCTION(BlueprintCallable)
	// bool IsPinchHP();
	//
	// UFUNCTION(BlueprintCallable)
	// bool IsFullHP();
	//
	// UFUNCTION(BlueprintCallable)
	// bool IsEndurance();
	//
	// UFUNCTION(BlueprintCallable)
	// bool IsArtsSealedById(int ArtsId);
	//
	// UFUNCTION(BlueprintCallable)
	// bool IsArtsSealed(const FName& ArtsLabel);
	//
	// UFUNCTION(BlueprintCallable)
	// bool IsArtsLearnedById(int ArtsId);
	//
	// UFUNCTION(BlueprintCallable)
	// bool IsArtsLearned(const FName& ArtsLabel);
	//
	// UFUNCTION(BlueprintCallable)
	// bool IsAnyAbnormal();
	//
	// UFUNCTION(BlueprintCallable)
	// bool IsActiveSimpleBuff(const FString& BuffName);
	//
	// UFUNCTION(BlueprintCallable)
	// bool IsAbnormal(EAbnormalType Type);
	//
	// UFUNCTION(BlueprintCallable)
	// void IncrementAbnormalCount(EAbnormalType Type);
	//
	// UFUNCTION(BlueprintCallable)
	// float GetWeakPowerStrengthenRate(EBtlElementType Type);
	//
	// UFUNCTION(BlueprintCallable)
	// float GetUnitParameterBySlot(const FBtlUnitParameterLabel& Label, const FName& SlotLabel);
	//
	// UFUNCTION(BlueprintCallable)
	// float GetUnitParameterByIndex(int Index);
	//
	// UFUNCTION(BlueprintCallable)
	// int GetUnitParameterAsInt(const FBtlUnitParameterLabel& Label, int DefaultValue);
	//
	// UFUNCTION(BlueprintCallable)
	// bool GetUnitParameterAsBool(const FBtlUnitParameterLabel& Label);
	//
	// UFUNCTION(BlueprintCallable)
	// float GetUnitParameter(const FBtlUnitParameterLabel& Label, float DefaultValue);
	//
	// UFUNCTION(BlueprintCallable)
	// int GetTotalExp();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetTargetedCount();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetStrikeAttackRate();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetStrikeAttackMax();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetStrikeAttack();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetSPMaxInternal();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetSPMaxHUDInternal();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetSPMaxHUD();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetSPMax();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetSPInternal();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetSpeedRate();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetSP();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetPenetrateBuff();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetPenetrate();
	//
	// UFUNCTION(BlueprintCallable)
	// FName GetOverwriteDamageEffect(bool& bClearOnDamage);
	//
	// UFUNCTION(BlueprintCallable)
	// FName GetOverrideMysticArts();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetOverlimitTimeRate();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetOverlimitTimeMax();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetOverlimitTime();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetOverlimitRate();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetOverlimitPrepare();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetOverlimitMaxRear();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetOverlimitMax();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetOverLimit();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetNormalAttackMax();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetLv();
	//
	// // UFUNCTION(BlueprintCallable)
	// // float GetKnockbackTimeRate(ABtlCharacterBase* Target, const FName& ArtsLabel);
	// //
	// UFUNCTION(BlueprintCallable)
	// int GetHPSectionValue(int SectionIndex);
	//
	// UFUNCTION(BlueprintCallable)
	// TArray<float> GetHPSection();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetHPRate();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetHPPercent();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetHPMax();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetHp();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetHateRate();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetHalfSPTriggerWeight();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetGuardDamageRate();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetGuardBreakMax();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetGuardBreak();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetEnduranceValueMax();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetEnduranceValue();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetEnduranceBuff();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetEndurance();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetElementDefBuff();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetElementDef();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetElementAtkBuff();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetElementAtk();
	//
	// // UFUNCTION(BlueprintCallable)
	// // EAbnormalType GetDrawAbnormalType();
	// //
	// UFUNCTION(BlueprintCallable)
	// float GetDrawAbnormalTime();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetDefBuff();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetDef();
	//
	// UFUNCTION(BlueprintCallable)
	// FName GetDamageEffect();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetChantSpeedRate(int ComboArtsCount);
	//
	// // UFUNCTION(BlueprintCallable)
	// // float GetBuffParameterValue(EBtlStatusBuffParameterType Type, bool bIgnoreBonus);
	// //
	// UFUNCTION(BlueprintCallable)
	// int GetBombBulletMax();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetBombBullet();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetBasePhysicalAttack();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetBaseElementAttack();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetAvoidanceDistanceRate();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetAtkBuff();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetAtk();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetArtsUseCount(const FName& ArtsLabel);
	//
	// UFUNCTION(BlueprintCallable)
	// float GetArtsPower();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetArtsHealPower();
	//
	// UFUNCTION(BlueprintCallable)
	// float GetArtsDownRate();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetAlleviation();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetAbnormalValueMax();
	//
	// // UFUNCTION(BlueprintCallable)
	// // int GetAbnormalValue(EAbnormalType Type);
	// //
	// // UFUNCTION(BlueprintCallable)
	// // float GetAbnormalTimer(EAbnormalType Type);
	// //
	// // UFUNCTION(BlueprintCallable)
	// // float GetAbnormalProcessTimer(EAbnormalType Type);
	// //
	// UFUNCTION(BlueprintCallable)
	// float GetAbnormalProcessMaxTime();
	//
	// // UFUNCTION(BlueprintCallable)
	// // int GetAbnormalGuardValue(EAbnormalType Type);
	// //
	// // UFUNCTION(BlueprintCallable)
	// // float GetAbnormalCutRate(EAbnormalType Type);
	// //
	// UFUNCTION(BlueprintCallable)
	// int GetAbnormalCountMax();
	// //
	// // UFUNCTION(BlueprintCallable)
	// // int GetAbnormalCount(EAbnormalType Type);
	// //
	// UFUNCTION(BlueprintCallable)
	// void CutSurplusOverlimit();
	//
	// UFUNCTION(BlueprintCallable)
	// void ClearAbnormalAll();
	// //
	// // UFUNCTION(BlueprintCallable)
	// // void ClearAbnormal(EAbnormalType Type);
	// //
	// UFUNCTION(BlueprintCallable)
	// int CheckHpSectionOver(int PrevHp, int CheckHp, bool SkipOneSection);
	//
	// UFUNCTION(BlueprintCallable)
	// bool ChangeSimpleBuff(const FString& BuffName, float NewBuffValue);
	//
	// // UFUNCTION(BlueprintCallable)
	// // float CalculateDamageDecrease(float DefaultDamage, ABtlCharacterBase* AttackerActor, const TScriptInterface<UBtlAttackCollisionInterface>& AttackCollision, ABtlCharacterBase* DmgActor, UBtlDamageSphereComponent* DmgCollision, const FBtlArtsData& ArtsData, bool IsGuard, bool IsCritical, bool IsWeakness, bool IsResistance);
	// //
	// // UFUNCTION(BlueprintCallable)
	// // float CalculateDamageAdd(float DefaultDamage, ABtlCharacterBase* AttackerActor, const TScriptInterface<UBtlAttackCollisionInterface>& AttackCollision, ABtlCharacterBase* DmgActor, UBtlDamageSphereComponent* DmgCollision, const FBtlArtsData& ArtsData, bool IsGuard, bool IsCritical, bool IsWeakness, bool IsResistanc);
	// //
	// // UFUNCTION(BlueprintCallable)
	// // void BtlStatusAbnormalTickSignature__DelegateSignature(UBtlStatusComponent* SelfComponent, EAbnormalType AbnormalType, float DeltaTime);
	// //
	// // UFUNCTION(BlueprintCallable)
	// // void BtlStatusAbnormalChangeTimeSignature__DelegateSignature(UBtlStatusComponent* SelfComponent, EAbnormalType AbnormalType, float PrevTime, float Time);
	// //
	// UFUNCTION(BlueprintCallable)
	// void BtlSpInternalChangeSignature__DelegateSignature(UBtlStatusComponent* SelfComponent, float PrevSp, float NewSp);
	//
	// UFUNCTION(BlueprintCallable)
	// void BtlSpChangeSignature__DelegateSignature(UBtlStatusComponent* SelfComponent, int PrevSp, int NewSp);
	//
	// UFUNCTION(BlueprintCallable)
	// void BtlPrepareOverlimitSignature__DelegateSignature(UBtlStatusComponent* SelfComponent);
	//
	// UFUNCTION(BlueprintCallable)
	// void BtlPinchToNormalHpSignature__DelegateSignature(UBtlStatusComponent* SelfComponent);
	//
	// UFUNCTION(BlueprintCallable)
	// void BtlNothingOverlimitTimeSignature__DelegateSignature(UBtlStatusComponent* SelfComponent);
	//
	// UFUNCTION(BlueprintCallable)
	// void BtlNormalToPinchHpSignature__DelegateSignature(UBtlStatusComponent* SelfComponent);
	//
	// UFUNCTION(BlueprintCallable)
	// void BtlMaxStrikeSignature__DelegateSignature(UBtlStatusComponent* SelfComponent);
	//
	// UFUNCTION(BlueprintCallable)
	// void BtlMaxSpSignature__DelegateSignature(UBtlStatusComponent* SelfComponent);
	//
	// UFUNCTION(BlueprintCallable)
	// void BtlMaxOverlimitSignature__DelegateSignature(UBtlStatusComponent* SelfComponent);
	//
	// UFUNCTION(BlueprintCallable)
	// void BtlMaxHpSignature__DelegateSignature(UBtlStatusComponent* SelfComponent);
	//
	// UFUNCTION(BlueprintCallable)
	// void BtlHpChangeSignature__DelegateSignature(UBtlStatusComponent* SelfComponent, int PrevHp, int NewHp);
	//
	// UFUNCTION(BlueprintCallable)
	// void BtlEnduranceBreakSignature__DelegateSignature();
	//
	// // UFUNCTION(BlueprintCallable)
	// // void BtlEndAbnormalSignature__DelegateSignature(UBtlStatusComponent* SelfComponent, EAbnormalType AbnormalType);
	// //
	// UFUNCTION(BlueprintCallable)
	// void BtlChangeStrikeSignature__DelegateSignature(UBtlStatusComponent* SelfComponent);
	//
	// UFUNCTION(BlueprintCallable)
	// void BtlChangeOverlimitTimeSignature__DelegateSignature(UBtlStatusComponent* SelfComponent);
	//
	// UFUNCTION(BlueprintCallable)
	// void BtlChangeOverlimitSignature__DelegateSignature(UBtlStatusComponent* SelfComponent);
	//
	// UFUNCTION(BlueprintCallable)
	// void BtlBeginBuffSignature__DelegateSignature();
	// //
	// // UFUNCTION(BlueprintCallable)
	// // void BtlBeginAbnormalSignature__DelegateSignature(UBtlStatusComponent* SelfComponent, EAbnormalType AbnormalType);
	// //
	// UFUNCTION(BlueprintCallable)
	// void AddTotalExp(int Value);
	//
	// UFUNCTION(BlueprintCallable)
	// void AddStrikeAttack(float Value);
	//
	// UFUNCTION(BlueprintCallable)
	// void AddSPInternal(float Value);
	//
	// UFUNCTION(BlueprintCallable)
	// void AddSP(int Value);
	//
	// // UFUNCTION(BlueprintCallable)
	// // void AddSkillBuff(const FBtlSkill_XmlDatatableLabel& SkillLabel);
	// //
	// // UFUNCTION(BlueprintCallable)
	// // void AddSimpleBuff(const FString& BuffName, EBtlSimpleBuffType BuffType, float BuffValue, float BuffTime, bool IsOverlap);
	//
	UFUNCTION(BlueprintCallable)
	void AddOverlimitTime(float Value);
	//
	UFUNCTION(BlueprintCallable)
	void AddOverLimit(int Value);
	//
	// UFUNCTION(BlueprintCallable)
	// void AddHPSection(float Rate);
	//
	// UFUNCTION(BlueprintCallable)
	// void AddHP(int Value);
	//
	// UFUNCTION(BlueprintCallable)
	// void AddGuardBreak(int Value);
	//
	// UFUNCTION(BlueprintCallable)
	// void AddEnduranceValue(int Value);
	//
	// UFUNCTION(BlueprintCallable)
	// void AddDamageEffectSkill(const FName& SkillLabel);
	//
	// UFUNCTION(BlueprintCallable)
	// void AddBuff(const FName& BuffID);
	//
	// UFUNCTION(BlueprintCallable)
	// void AddArtsUseCount(const FName& ArtsLabel);
	//
	// // UFUNCTION(BlueprintCallable)
	// // void AddArtsBuff(const FBtlArts_XmlDatatableLabel& ArtsLabel);
	// //
	// UFUNCTION(BlueprintCallable)
	// void AddAllParamRate(float Rate);

	// UFUNCTION(BlueprintCallable)
	// void AddAbnormalValue(EAbnormalType Type, int Value, int HitCount, UBtlStatusComponent* Attacker);
	//
	// UFUNCTION(BlueprintCallable)
	// void AddAbnormalCount(EAbnormalType Type, int Value);
};
