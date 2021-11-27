// Fill out your copyright notice in the Description page of Project Settings.

#include "BtlManager.h"


// Sets default values
ABtlManager::ABtlManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABtlManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABtlManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABtlManager::IsUsableFeedback()
{
	return false;
}

bool ABtlManager::IsTutorial()
{
	return false;
}

bool ABtlManager::IsSystemPausePossible()
{
	return false;
}

bool ABtlManager::IsRetry()
{
	return false;
}

bool ABtlManager::IsExistAdditionalWall()
{
	return false;
}

bool ABtlManager::IsDisableOverlimitPlay()
{
	return false;
}

bool ABtlManager::IsDisableOverlimitAddValue()
{
	return false;
}

bool ABtlManager::IsDisableMystic()
{
	return false;
}

bool ABtlManager::IsDisableItem()
{
	return false;
}

bool ABtlManager::IsDisableBoostStrike()
{
	return false;
}

bool ABtlManager::IsDisableBoostAttack()
{
	return false;
}

bool ABtlManager::IsDamageReduceForSC()
{
	return false;
}

bool ABtlManager::IsCutSceneFinishSlow()
{
	return false;
}

bool ABtlManager::IsCutSceneFinishFlow()
{
	return false;
}

bool ABtlManager::IsBattlePause()
{
	return false;
}
