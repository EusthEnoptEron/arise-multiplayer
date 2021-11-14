// Fill out your copyright notice in the Description page of Project Settings.

#include "InputExtInputProcessBase.h"


// Sets default values
AInputExtInputProcessBase::AInputExtInputProcessBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInputExtInputProcessBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInputExtInputProcessBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInputExtInputProcessBase::SetCurrentProcess(bool Enable) {}

