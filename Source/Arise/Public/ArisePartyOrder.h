// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "UObject/NoExportTypes.h"
#include "ArisePartyOrder.generated.h"

/**
 * 
 */
UCLASS()
class ARISE_API UArisePartyOrder : public UObject
{
	GENERATED_BODY()


	UFUNCTION(BlueprintCallable)
	EArisePartyID SwitchPartyTop(bool bCW);
	UFUNCTION(BlueprintCallable)
	void SwapPartyOrder(EPCOrder Src, EPCOrder Dst);
	UFUNCTION(BlueprintCallable)
	void SetPartyTop(EArisePartyID PartyId);
	UFUNCTION(BlueprintCallable)
	void SetPartyOrder(EArisePartyID PartyId, EPCOrder Order);
	UFUNCTION(BlueprintCallable)
	void SetOperationMode(EControlPlayer Player, EOperationMode Mode);
	UFUNCTION(BlueprintCallable)
	void SetFirstPartyIn(EArisePartyID PartyId, bool Flag);
	UFUNCTION(BlueprintCallable)
	void ResetGuest();
	UFUNCTION(BlueprintCallable)
	void ResetCompanion();
	UFUNCTION(BlueprintCallable)
	void PushBattleOrder();
	UFUNCTION(BlueprintCallable)
	void PopBattleOrder();
	UFUNCTION(BlueprintCallable)
	void OutPartyAll();
	UFUNCTION(BlueprintCallable)
	void OutParty(EArisePartyID PartyId);
	UFUNCTION(BlueprintCallable)
	void K2_SetEquipmentFlag(EArisePartyID PartyId);
	UFUNCTION(BlueprintCallable)
	bool IsParty(EArisePartyID PartyId);
	UFUNCTION(BlueprintCallable)
	bool IsFirstPartyIn(EArisePartyID PartyId);
	UFUNCTION(BlueprintCallable)
	void InParty(EArisePartyID PartyId);
	UFUNCTION(BlueprintCallable)
	EArisePartyID GetPartyTop();
	UFUNCTION(BlueprintCallable)
	EPCOrder GetPartyOrder(EArisePartyID PartyId);
	UFUNCTION(BlueprintCallable)
	TArray<EArisePartyID> GetPartyList();
	UFUNCTION(BlueprintCallable)
	EArisePartyID GetPartyId(EPCOrder Order);
	UFUNCTION(BlueprintCallable)
	int GetPartyCount();
	UFUNCTION(BlueprintCallable)
	EOperationMode GetOperationMode(EControlPlayer Player);
	UFUNCTION(BlueprintCallable)
	int GetGuestCount();
	UFUNCTION(BlueprintCallable)
	int GetCompanionCount();
	UFUNCTION(BlueprintCallable)
	void Clear();
};
