#include "ConfigModule.h"
#include <INI.h>
#include "Utils.h";

void ConfigModule::Initialize(MultiplayerMod* mod)
{
	ModRef = mod;
}

void ConfigModule::PostBeginPlay(UE4::AActor* modActor)
{
}


void ConfigModule::Tick()
{
	if (IniDirty) {
		Log::Info("Update INI");
		RefreshIni();
		IniDirty = false;

		// Putting this in initialize() does not seem to work
		if (Watch == nullptr && std::ifstream(INI_FILE_LOCATION).good()) {
			Watch = new filewatch::FileWatch<std::wstring>(
				INI_FILE_LOCATION_W.c_str(),
				[this](const std::wstring& path, const filewatch::Event change_type) {
				IniDirty = true;
			}
			);
		}
	}

}


bool ConfigModule::IsSkipFirstPlayer()
{
	INI::PARSE_FLAGS = INI::PARSE_COMMENTS_ALL | INI::PARSE_COMMENTS_SLASH | INI::PARSE_COMMENTS_HASH;
	INI config(INI_FILE_LOCATION, true);
	config.select("MISC");

	return std::stoi(config.get("FirstPlayerIsKeyboard", "0"));
}

void ConfigModule::RefreshIni() {
	static auto onConfigChangedFn = ModRef->ModActor->GetFunction("OnConfigChanged");
	ApplyConfigParams parms;

	INI::PARSE_FLAGS = INI::PARSE_COMMENTS_ALL | INI::PARSE_COMMENTS_SLASH | INI::PARSE_COMMENTS_HASH;
	INI config(INI_FILE_LOCATION, true);

	config.select("CAMERA");
	parms.MinDistance = std::stof(config.get("MinDistance", "1500"));
	parms.MaxDistance = std::stof(config.get("MaxDistance", "3500"));
	parms.MinClip = std::stof(config.get("MinClip", "10.0"));
	parms.MaxClip = std::stof(config.get("MaxClip", "100000.0"));
	parms.ClipRatio = std::stof(config.get("ClipRatio", "0.5"));
	parms.TargetOffset = std::stof(config.get("TargetOffset", "50"));
	parms.TargetEnemies = std::stoi(config.get("TargetEnemies", "1"));
	parms.TargetHeroes = std::stoi(config.get("TargetHeroes", "1"));
	parms.ZoomInSpeed = std::stof(config.get("ZoomInSpeed", "1000"));
	parms.ZoomOutSpeed = std::stof(config.get("ZoomOutSpeed", "1000"));
	parms.ZoomInPadding = std::stof(config.get("ZoomInPadding", "0.2"));
	parms.ZoomOutPadding = std::stof(config.get("ZoomOutPadding", "0.1"));
	parms.RotateSpeedX = std::stof(config.get("RotateSpeedX", "60"));
	parms.RotateSpeedY = std::stof(config.get("RotateSpeedY", "40"));
	parms.FieldOfView = std::stof(config.get("FieldOfView", "60"));
	parms.TargetSpeed = std::stof(config.get("TargetSpeed", "0.75"));
	parms.TargetRadius = std::stof(config.get("TargetRadius", "50000"));
	parms.MinPitch = std::stof(config.get("MinPitch", "-75"));
	parms.MaxPitch = std::stof(config.get("MaxPitch", "-1"));
	parms.IgnoreDeadPlayers = std::stoi(config.get("IgnoreDeadPlayers", "1"));
	parms.UseSinglePlayerCamera = std::stoi(config.get("UseSinglePlayerCamera", "0"));
	ModRef->CameraShakeScale = std::stof(config.get("CameraShakeScale", "1.0"));

	config.select("MISC");

	parms.AllowSwitchingCharasDuringBattle = std::stoi(config.get("AllowSwitchingCharasDuringBattle", "1"));
	parms.ResetCharacterAssignmentsAfterBattle = std::stoi(config.get("ResetCharacterAssignmentsAfterBattle", "1"));
	parms.TargetCursorScale = std::stof(config.get("TargetCursorScale", "0.75"));
	parms.HideDefaultCursor = std::stoi(config.get("HideDefaultCursor", "1"));

	ModRef->DisableHitStop = std::stoi(config.get("DisableHitStop", "0"));

	parms.DebugMenu = std::stoi(config.get("DebugMenu", "0"));

	ModRef->AutoChangeCharas = std::stoi(config.get("AutoChangeCharas", "0"));
	ModRef->RestrictBoostAttacksToCpuAndSelf = std::stoi(config.get("RestrictBoostAttacksToCpuAndSelf", "0"));
	ModRef->RestrictBoostAttacksToP1 = std::stoi(config.get("RestrictBoostAttacksToP1", "0"));

	if (!onConfigChangedFn) {
		Log::Warn("OnConfigChangedFn not found");
		return;
	}
	ModRef->ModActor->ProcessEvent(onConfigChangedFn, &parms);
}