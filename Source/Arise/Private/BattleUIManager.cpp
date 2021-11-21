// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleUIManager.h"


// Sets default values
ABattleUIManager::ABattleUIManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABattleUIManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABattleUIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABattleUIManager::OnBeginChangeTarget()
{
}

void ABattleUIManager::ShowLockon()
{
}
