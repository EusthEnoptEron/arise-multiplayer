// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BtlCommonViewModelBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBattleViewModelDelegate);

/**
 * 
 */
UCLASS(BlueprintType)
class ARISE_API UBtlCommonViewModelBase : public UObject
{
	GENERATED_BODY()

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FBattleViewModelDelegate OnBeginChangeTarget;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FBattleViewModelDelegate OnChangeTarget;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FBattleViewModelDelegate OnChangeTargetLost;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FBattleViewModelDelegate OnEndChangeTarget;
};
