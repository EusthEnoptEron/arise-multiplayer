// Fill out your copyright notice in the Description page of Project Settings.

#include "BtlInputExtInputProcessBase.h"


// Sets default values
ABtlInputExtInputProcessBase::ABtlInputExtInputProcessBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABtlInputExtInputProcessBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABtlInputExtInputProcessBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABtlInputExtInputProcessBase::SetMoveToWorld(FVector Vector) {

}

void ABtlInputExtInputProcessBase::SetInputEnable(bool Enable, EBtlBitFlagCategory Category) {
}

void ABtlInputExtInputProcessBase::SetInputConfigFlag(EBtlInputConfigType ConfigType, bool bEnable) {}

void ABtlInputExtInputProcessBase::OnChangeAriseGameState(EAriseGameState ChangedState, bool bNewState) {}

void ABtlInputExtInputProcessBase::RegisterInputEvent(const FName& InButtonName, EBtlInputEventType Type, const FButtonCallback& Event) {

}

ABtlInputExtInputProcessBase* ABtlInputExtInputProcessBase::K2_GetBattleInputProcess(UObject* WorldContextObject)
{
	return nullptr;
}
