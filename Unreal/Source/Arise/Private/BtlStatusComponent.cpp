// Fill out your copyright notice in the Description page of Project Settings.

#include "BtlStatusComponent.h"


// Sets default values for this component's properties
UBtlStatusComponent::UBtlStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBtlStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBtlStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBtlStatusComponent::UpdateEquipment()
{
}

void UBtlStatusComponent::SubStrikeAttack(float Value)
{
}

void UBtlStatusComponent::SubSPInternal(float Value)
{
}

void UBtlStatusComponent::SubSP(int Value)
{
}

void UBtlStatusComponent::SubOverlimitTime(float Value)
{
}

void UBtlStatusComponent::SubOverlimit(int Value)
{
}

void UBtlStatusComponent::SubHP(int Value)
{
}

void UBtlStatusComponent::SubGuardBreak(int Value)
{
}

void UBtlStatusComponent::SubEnduranceValue(int Value)
{
}

void UBtlStatusComponent::SubCP(int Value)
{
}

void UBtlStatusComponent::SetUnlimitedOverlimit(bool IsUnlimited)
{
}

void UBtlStatusComponent::SetTotalExp(int Value)
{
}

void UBtlStatusComponent::SetStrikeAttackRecoverValue(bool Overwrite, float Value)
{
}

void UBtlStatusComponent::SetStrikeAttack(float Value)
{
}

void UBtlStatusComponent::SetSPInternal(float Value)
{
}

void UBtlStatusComponent::SetSP(int Value)
{
}

void UBtlStatusComponent::SetPenetrate(int Value)
{
}

void UBtlStatusComponent::SetOverwriteDamageEffect(const FName& Label, bool bClearOnDamage)
{
}

void UBtlStatusComponent::SetOverlimitTimeMax(float Value)
{
}

void UBtlStatusComponent::SetOverlimitTime(float Value)
{
}

void UBtlStatusComponent::SetOverLimit(int Value)
{
}

void UBtlStatusComponent::SetLv(int Value)
{
}

void UBtlStatusComponent::SetHPSection(TArray<float> HPSectionList)
{
}

void UBtlStatusComponent::SetHPRate(float HpRate)
{
}

void UBtlStatusComponent::SetHPMax(int Value)
{
}

void UBtlStatusComponent::SetHP(int Value)
{
}

void UBtlStatusComponent::SetHateRate(float Value)
{
}

void UBtlStatusComponent::SetHalfSPTriggerWeight(float Weight)
{
}

void UBtlStatusComponent::SetGuardBreak(int Value)
{
}

void UBtlStatusComponent::SetEnduranceValue(int Value)
{
}

void UBtlStatusComponent::SetEndurance(int Value)
{
}

void UBtlStatusComponent::SetElementDef(int Value)
{
}

void UBtlStatusComponent::SetElementAtk(int Value)
{
}

void UBtlStatusComponent::SetDrawAbnormalTime(float Value)
{
}

void UBtlStatusComponent::SetDef(int Value)
{
}

void UBtlStatusComponent::SetDamageEffect(const FName& Label)
{
}

void UBtlStatusComponent::SetBombBullet(int BulletNum)
{
}

void UBtlStatusComponent::SetAtk(int Value)
{
}

void UBtlStatusComponent::SetArtsLearn(const FName& ArtsLabel)
{
}

void UBtlStatusComponent::SetAlleviation(int Value)
{
}

void UBtlStatusComponent::RemoveSimpleBuff(const FString& BuffName)
{
}

void UBtlStatusComponent::RemoveDamageEffectSkill(const FName& SkillLabel)
{
}

void UBtlStatusComponent::OnPostBeginBattle()
{
}

void UBtlStatusComponent::OnParameterHandleChanged(int HandleIndex, float OldValue, float NewValue)
{
}

void UBtlStatusComponent::OnHpBonusChanged(int HandleIndex, float OldValue, float NewValue)
{
}

void UBtlStatusComponent::OnEndTimeStop()
{
}

void UBtlStatusComponent::OnEndBattle()
{
}

void UBtlStatusComponent::OnBeginTimeStop()
{
}

void UBtlStatusComponent::OnBaseHpBonusChanged(int HandleIndex, float OldValue, float NewValue)
{
}

void UBtlStatusComponent::K2_GetSkillParameterByInt(const FString& SkillName, bool& IsActiveSkill,
	float& SkillParameter)
{
}

void UBtlStatusComponent::K2_GetSkillParameter(const FString& SkillName, bool& IsActiveSkill, float& SkillParameter)
{
}

bool UBtlStatusComponent::IsUnlimitedOverlimit()
{
	return false;
}

bool UBtlStatusComponent::IsStrikeFull()
{
	return false;
}

bool UBtlStatusComponent::IsSpecificSkillLearned(const FString& SkillName)
{
	return false;
}

void UBtlStatusComponent::AddOverlimitTime(float Value)
{
}

void UBtlStatusComponent::AddOverLimit(int Value)
{
}

