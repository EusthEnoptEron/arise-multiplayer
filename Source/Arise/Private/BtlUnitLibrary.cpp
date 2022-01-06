// Fill out your copyright notice in the Description page of Project Settings.

#include "BtlUnitLibrary.h"


ABtlCharacterBase* UBtlUnitLibrary::GetPartyUnitByPartyId(EArisePartyID PartyId) { return nullptr; }
ABtlCharacterBase* UBtlUnitLibrary::GetPartyUnitByOrder(EPCOrder Order) { return nullptr; };
ABtlCharacterBase* UBtlUnitLibrary::GetPartyUnit(const FName& UnitID) { return nullptr; }

ABtlCharacterBase* UBtlUnitLibrary::GetPlayerControlledUnit()
{
	return nullptr;
}

EPCOrder UBtlUnitLibrary::GetPartyOrder(ABtlCharacterBase* Actor)
{
	return {};
}

