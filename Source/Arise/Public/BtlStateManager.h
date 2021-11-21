// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BtlStateBase.h"
#include "Enums.h"
#include "UObject/NoExportTypes.h"
#include "BtlStateManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBattleStateDelegate, EBattleState, State);

/**
 * 
 */
UCLASS(BlueprintType)
class ARISE_API UBtlStateManager : public UObject
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleStateDelegate OnBtlStateChange; // 0x0160(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleStateDelegate OnBtlSubStateStart;
	// 0x0170(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleStateDelegate OnBtlSubStateEnd; // 0x0180(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UFUNCTION(BlueprintCallable)
	EBattleState GetCurrentState();

	UFUNCTION(BlueprintCallable)
	EBattleState GetSubState();

	UFUNCTION(BlueprintCallable)
	UBtlStateBase* GetStateAction(EBattleState State);
};
