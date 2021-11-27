// Fill out your copyright notice in the Description page of Project Settings.

#include "BtlCutSceneManager.h"


// Sets default values
ABtlCutSceneManager::ABtlCutSceneManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABtlCutSceneManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABtlCutSceneManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABtlCutSceneManager::IsPlayingCutScene()
{
	return false;
}
