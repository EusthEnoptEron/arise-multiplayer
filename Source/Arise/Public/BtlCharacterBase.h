// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BtlActionProcessor.h"
#include "BtlCharacterBase.generated.h"

// Enum Arise.EBtlUnitGroup
UENUM(BlueprintType)
enum class EBtlUnitGroup : uint8
{
	GROUP_UNKNOWN = 0,
	GROUP_PARTY = 1,
	GROUP_ENEMY = 2,
	GROUP_GUEST = 3,
	GROUP_MAX = 4
};

UCLASS()
class ARISE_API ABtlCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABtlCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	UBtlActionProcessor *GetActionProcessor();
	
	UFUNCTION(BlueprintCallable)
	FVector GetTargetComponentLocation(bool IgnoreTemporary);


	UFUNCTION(BlueprintCallable)
	EBtlUnitGroup GetUnitGroup();

	UFUNCTION(BlueprintCallable)
	FName GetUnitID();
/*
	UFUNCTION(BlueprintCallable)
	int GetCoreHp(class UBtlDamageSphereComponent* CoreCollision);
*/
};
