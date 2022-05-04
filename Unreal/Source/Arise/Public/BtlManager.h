// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BtlManager.generated.h"

class ABtlCutSceneManager;
UCLASS()
class ARISE_API ABtlManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABtlManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite)
	ABtlCutSceneManager* CutSceneManager;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool IsUsableFeedback();

	UFUNCTION(BlueprintCallable)
	bool IsTutorial();

	UFUNCTION(BlueprintCallable)
	bool IsSystemPausePossible();

	UFUNCTION(BlueprintCallable)
	bool IsRetry();

	UFUNCTION(BlueprintCallable)
	bool IsExistAdditionalWall();

	UFUNCTION(BlueprintCallable)
	bool IsDisableOverlimitPlay();

	UFUNCTION(BlueprintCallable)
	bool IsDisableOverlimitAddValue();

	UFUNCTION(BlueprintCallable)
	bool IsDisableMystic();

	UFUNCTION(BlueprintCallable)
	bool IsDisableItem();

	UFUNCTION(BlueprintCallable)
	bool IsDisableBoostStrike();

	UFUNCTION(BlueprintCallable)
	bool IsDisableBoostAttack();
	UFUNCTION(BlueprintCallable)
	bool IsDamageReduceForSC();
	UFUNCTION(BlueprintCallable)
	bool IsCutSceneFinishSlow();
	UFUNCTION(BlueprintCallable)
	bool IsCutSceneFinishFlow();
	UFUNCTION(BlueprintCallable)
	bool IsBattlePause();
};
