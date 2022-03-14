#pragma once
#include "BaseModule.h"


struct ApplyConfigParams {

	float MinDistance;
	float MaxDistance;

	float TargetOffset;
	bool TargetEnemies;
	bool TargetHeroes;

	float ZoomInSpeed;
	float ZoomOutSpeed;

	float ZoomInPadding;
	float ZoomOutPadding;

	float RotateSpeedX;
	float RotateSpeedY;

	float MinClip;
	float MaxClip;
	float ClipRatio;

	float FieldOfView;
	float TargetSpeed;
	float TargetRadius;

	float MinPitch;
	float MaxPitch;

	bool AllowSwitchingCharasDuringBattle;
	bool ResetCharacterAssignmentsAfterBattle;

	bool IgnoreDeadPlayers;
	bool UseSinglePlayerCamera;

	float TargetCursorScale;
	bool HideDefaultCursor;

	bool DebugMenu;
};

const std::string INI_FILE_LOCATION = "./MultiplayerMod.ini";
const std::wstring INI_FILE_LOCATION_W = L"./MultiplayerMod.ini";

/// <summary>
/// Allows configuring the mod. Should be the first module.
/// </summary>
class ConfigModule :
    public BaseModule
{
public:
	virtual void Initialize(MultiplayerMod* mod) override;
	virtual void PostBeginPlay(UE4::AActor* modActor) override;
	virtual void Tick() override;

	static bool IsSkipFirstPlayer();
    void RefreshIni();

private:
    UE4::UFunction* OnConfigChangedFn;

	filewatch::FileWatch<std::wstring>* Watch;
	bool IniDirty = true;
	MultiplayerMod* ModRef;
};

