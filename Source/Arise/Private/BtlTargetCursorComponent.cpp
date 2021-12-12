// Fill out your copyright notice in the Description page of Project Settings.

#include "BtlTargetCursorComponent.h"


// Sets default values for this component's properties
UBtlTargetCursorComponent::UBtlTargetCursorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBtlTargetCursorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UBtlTargetCursorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBtlTargetCursorComponent::SetUseTarget(bool on)
{
}

void UBtlTargetCursorComponent::SetTargetableFlag(EBtlBitFlagCategory Cause, bool Targetable)
{
}

bool UBtlTargetCursorComponent::IsUseTarget()
{
	return false;
}

bool UBtlTargetCursorComponent::IsUseDefault()
{
	return false;
}

bool UBtlTargetCursorComponent::IsTargetableByCondition(EBtlBitFlagCategory IgnoreFlags)
{
	return false;
}

bool UBtlTargetCursorComponent::IsTargetable()
{
	return false;
}

ABtlCharacterBase* UBtlTargetCursorComponent::GetBtlUnit()
{
	return nullptr;
}
