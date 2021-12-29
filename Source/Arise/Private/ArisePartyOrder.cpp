// Fill out your copyright notice in the Description page of Project Settings.

#include "ArisePartyOrder.h"


EArisePartyID UArisePartyOrder::SwitchPartyTop(bool bCW)
{
	return {};
}

void UArisePartyOrder::SwapPartyOrder(EPCOrder Src, EPCOrder Dst)
{
}

void UArisePartyOrder::SetPartyTop(EArisePartyID PartyId)
{
}

void UArisePartyOrder::SetPartyOrder(EArisePartyID PartyId, EPCOrder Order)
{
}

void UArisePartyOrder::SetOperationMode(EControlPlayer Player, EOperationMode Mode)
{
}

void UArisePartyOrder::SetFirstPartyIn(EArisePartyID PartyId, bool Flag)
{
}

void UArisePartyOrder::ResetGuest()
{
}

void UArisePartyOrder::ResetCompanion()
{
}

void UArisePartyOrder::PushBattleOrder()
{
}

void UArisePartyOrder::PopBattleOrder()
{
}

void UArisePartyOrder::OutPartyAll()
{
}

void UArisePartyOrder::OutParty(EArisePartyID PartyId)
{
}

void UArisePartyOrder::K2_SetEquipmentFlag(EArisePartyID PartyId)
{
}

bool UArisePartyOrder::IsParty(EArisePartyID PartyId)
{
	return false;
}

bool UArisePartyOrder::IsFirstPartyIn(EArisePartyID PartyId)
{
	return false;
}

void UArisePartyOrder::InParty(EArisePartyID PartyId)
{
}

EArisePartyID UArisePartyOrder::GetPartyTop()
{
	return {};
}

EPCOrder UArisePartyOrder::GetPartyOrder(EArisePartyID PartyId)
{
	return {};
}

TArray<EArisePartyID> UArisePartyOrder::GetPartyList()
{
	return {};
}

EArisePartyID UArisePartyOrder::GetPartyId(EPCOrder Order)
{
	return {};
}

int UArisePartyOrder::GetPartyCount()
{
	return 0;
}

EOperationMode UArisePartyOrder::GetOperationMode(EControlPlayer Player)
{
	return {};
}

int UArisePartyOrder::GetGuestCount()
{
	return 0;
}

int UArisePartyOrder::GetCompanionCount()
{
	return 0;
}

void UArisePartyOrder::Clear()
{
}


