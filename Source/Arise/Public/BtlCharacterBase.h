// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums.h"
#include "BtlActionProcessor.h"
#include "BtlStatusComponent.h"
#include "BtlStatusViewModelBase.h"
#include "BtlUnitScriptComponent.h"
#include "Btl_AIControllerBase.h"
#include "Btl_Camera.h"
#include "BtlCharacterBase.generated.h"

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
	UBtlActionProcessor* GetActionProcessor();

	UFUNCTION(BlueprintCallable)
	FVector GetTargetComponentLocation(bool IgnoreTemporary);


	UFUNCTION(BlueprintCallable)
	EBtlUnitGroup GetUnitGroup();

	UFUNCTION(BlueprintCallable)
	FName GetUnitID();

	UFUNCTION(BlueprintCallable)
	bool UseItemProcess(int ItemID);

	//UFUNCTION(BlueprintCallable)
	//void SetTargetComponent(UBtlTargetCursorComponent* Component);

	UFUNCTION(BlueprintCallable)
	void SetTargetCharacter(ABtlCharacterBase* Character);

	UFUNCTION(BlueprintCallable)
	bool SetPlayerOperation(bool Enable);
	UFUNCTION(BlueprintCallable)
	bool SetEnableRestartAI(bool on);

	UFUNCTION(BlueprintCallable)
	void SetBtlAIController(ABtl_AIControllerBase* AIController);

	UFUNCTION(BlueprintCallable)
	void SetAIFlag(bool Enable, EBtlBitFlagCategory Category);

	UFUNCTION(BlueprintCallable)
	void DestroyBtlAIController();

	UFUNCTION(BlueprintCallable)
	bool SetOverLimit(bool Enable);
	
	UFUNCTION(BlueprintCallable)
	UBtlUnitScriptComponent* GetUnitScriptComponent();
	
	UFUNCTION(BlueprintCallable)
	UBtlStatusComponent* GetStatus();
	
	UPROPERTY(BlueprintReadWrite)
	/*UBtlStatusViewModelBase*/UBtlStatusViewModelBase* ViewModel;
	/*
	
	
		UFUNCTION(BlueprintCallable)
		int GetCoreHp(class UBtlDamageSphereComponent* CoreCollision);
	*/
};
