#pragma once
#include "MultiplayerMod.h";


class BaseModule
{
public:
	virtual void Initialize(MultiplayerMod *mod) = 0;
	virtual void PostBeginPlay(UE4::AActor* modActor) {};
	virtual void Tick() {};
};

