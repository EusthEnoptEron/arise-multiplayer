// Fill out your copyright notice in the Description page of Project Settings.

#include "BtlCharacterBase.h"
#include "BtlActionProcessor.h"


// Sets default values
ABtlCharacterBase::ABtlCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABtlCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABtlCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABtlCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBtlActionProcessor *ABtlCharacterBase::GetActionProcessor() {
	return NULL;
}




FVector ABtlCharacterBase::GetTargetComponentLocation(bool IgnoreTemporary) {
	return FVector();
}

EBtlUnitGroup ABtlCharacterBase::GetUnitGroup() {
	return (EBtlUnitGroup)0;
}

FName ABtlCharacterBase::GetUnitID() {
	return FName();
}
//
//int ABtlCharacterBase::GetCoreHp(class UBtlDamageSphereComponent* CoreCollision) {
//	return 0;
//}
