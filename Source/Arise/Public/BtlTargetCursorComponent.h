// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BtlCharacterBase.h"
#include "Enums.h"
#include "Components/SceneComponent.h"
#include "BtlTargetCursorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARISE_API UBtlTargetCursorComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBtlTargetCursorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetUseTarget(bool on);

	UFUNCTION(BlueprintCallable)
	void SetTargetableFlag(EBtlBitFlagCategory Cause, bool Targetable);

	UFUNCTION(BlueprintCallable)
	bool IsUseTarget();
	UFUNCTION(BlueprintCallable)
	bool IsUseDefault();
	UFUNCTION(BlueprintCallable)
	bool IsTargetableByCondition(EBtlBitFlagCategory IgnoreFlags);
	UFUNCTION(BlueprintCallable)
	bool IsTargetable();
	
	UFUNCTION(BlueprintCallable)
	ABtlCharacterBase* GetBtlUnit();
};
