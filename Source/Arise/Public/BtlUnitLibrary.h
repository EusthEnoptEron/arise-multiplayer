// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BtlCharacterBase.h"
#include "BtlUnitLibrary.generated.h"


// Enum Arise.EPCOrder
UENUM(BlueprintType)
enum class EPCOrder : uint8
{
	ORDER_1ST = 0,
	ORDER_2ND = 1,
	ORDER_3RD = 2,
	ORDER_4TH = 3,
	ORDER_5TH = 4,
	ORDER_6TH = 5,
	ORDER_7TH = 6,
	ORDER_8TH = 7,
	ORDER_MAX = 8
};


UENUM(BlueprintType)
enum class EArisePartyID : uint8
{
	ARI = 0,
	SIO = 1,
	NNA = 2,
	ZGU = 3,
	MNK = 4,
	YRA = 5,
	DLC = 6,
	DLC01 = 7,
	MAX = 8,
	INVALID = 9,
	DUMMY_ANONYMOUS_ = 10
};

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
