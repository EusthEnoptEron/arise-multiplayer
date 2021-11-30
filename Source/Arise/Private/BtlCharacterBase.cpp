// Fill out your copyright notice in the Description page of Project Settings.

#include "BtlCharacterBase.h"
#include "BtlActionProcessor.h"


// Sets default values
ABtlCharacterBase::ABtlCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

UBtlUnitScriptComponent* ABtlCharacterBase::GetUnitScriptComponent()
{
	return nullptr;
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

bool ABtlCharacterBase::UseItemProcess(int ItemID) { return false; }

//void ABtlCharacterBase::SetTargetComponent(class UBtlTargetCursorComponent* Component) {}

void ABtlCharacterBase::SetTargetCharacter(class ABtlCharacterBase* Character) {}

bool ABtlCharacterBase::SetPlayerOperation(bool Enable) { return false; }

bool ABtlCharacterBase::SetEnableRestartAI(bool on) { return false; }

void ABtlCharacterBase::SetBtlAIController(class ABtl_AIControllerBase* AIController) {}

void ABtlCharacterBase::SetAIFlag(bool Enable, EBtlBitFlagCategory Category) {}

void ABtlCharacterBase::DestroyBtlAIController() {}

bool ABtlCharacterBase::SetOverLimit(bool Enable)
{
	return false;
}

//
//int ABtlCharacterBase::GetCoreHp(class UBtlDamageSphereComponent* CoreCollision) {
//	return 0;
//}
