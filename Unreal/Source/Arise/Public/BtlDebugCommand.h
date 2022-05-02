// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BtlDebugCommand.generated.h"

UCLASS()
class ARISE_API ABtlDebugCommand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABtlDebugCommand();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	static void BtlWin();
	
	UFUNCTION(BlueprintCallable)
	static void BtlEnd();
};
