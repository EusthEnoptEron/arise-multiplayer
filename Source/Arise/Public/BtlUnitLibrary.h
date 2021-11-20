// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BtlCharacterBase.h"
#include "Enums.h"
#include "BtlUnitLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARISE_API UBtlUnitLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
	
	UFUNCTION(BlueprintCallable)
	static ABtlCharacterBase* GetPartyUnitByPartyId(EArisePartyID PartyId);
	
	UFUNCTION(BlueprintCallable)
	static ABtlCharacterBase* GetPartyUnitByOrder(EPCOrder Order);

	UFUNCTION(BlueprintCallable)
	static ABtlCharacterBase* GetPartyUnit(const FName& UnitID);
};
