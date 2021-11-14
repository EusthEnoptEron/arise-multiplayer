#include "ExampleMod.h"
#include <Windows.h>

// Only Called Once, if you need to hook shit, declare some global non changing values
void MultiplayerMod::InitializeMod()
{
	UE4::InitSDK();
	SetupHooks();
	//UseMenuButton = true; // Allows Mod Loader To Show Button
}

void MultiplayerMod::ProcessFunction(UE4::UObject* obj, UE4::FFrame* Frame)
{
	if (obj)
	{
		//if(obj == ModActor) // Checks If the actor calling this function is your Mod Actor Function
	}
}

void MultiplayerMod::InitGameState()
{
}

void MultiplayerMod::BeginPlay(UE4::AActor* Actor)
{
}

void MultiplayerMod::PostBeginPlay(std::wstring ModActorName, UE4::AActor* Actor)
{
	// Filters Out All Mod Actors Not Related To Your Mod
	std::wstring TmpModName(ModName.begin(), ModName.end());
	if (ModActorName == TmpModName)
	{
		//Sets ModActor Ref
		ModActor = Actor;
		MessageBoxW(NULL, L"Yay!", L"MinHook Sample", MB_OK);

	}
}

void MultiplayerMod::OnModMenuButtonPressed()
{
}

void MultiplayerMod::DrawImGui()
{
}