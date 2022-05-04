// Fill out your copyright notice in the Description page of Project Settings.

#include "BtlDebugCommand.h"


// Sets default values
ABtlDebugCommand::ABtlDebugCommand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABtlDebugCommand::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABtlDebugCommand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABtlDebugCommand::BtlWin()
{
}

void ABtlDebugCommand::BtlEnd()
{
}
