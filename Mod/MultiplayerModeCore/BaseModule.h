#pragma once
#include "MultiplayerMod.h";


class BaseModule
{
public:
	virtual void Initialize(MultiplayerMod *mod) = 0;
	virtual void Tick() = 0;
};

