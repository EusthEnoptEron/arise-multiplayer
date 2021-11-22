// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BtlActionQuery.generated.h"

/**
 * 
 */
UCLASS()
class ARISE_API UBtlActionQuery : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	bool IsReserved();

	UFUNCTION(BlueprintCallable)
	bool IsQueryEnd();
	UFUNCTION(BlueprintCallable)
	bool IsProcessing();
	UFUNCTION(BlueprintCallable)
	bool IsNotAccepted();
	UFUNCTION(BlueprintCallable)
	bool IsJustAccept();
	UFUNCTION(BlueprintCallable)
	bool IsInterrupted();
	UFUNCTION(BlueprintCallable)
	bool IsFailedEnd();
	UFUNCTION(BlueprintCallable)
	bool IsCompleted();
};
