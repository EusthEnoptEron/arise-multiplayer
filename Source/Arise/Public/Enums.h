#pragma once
#include "CoreMinimal.h"


// Enum Arise.EBtlUnitGroup
UENUM(BlueprintType)
enum class EBtlUnitGroup : uint8
{
	GROUP_UNKNOWN = 0,
	GROUP_PARTY = 1,
	GROUP_ENEMY = 2,
	GROUP_GUEST = 3,
	GROUP_MAX = 4
};


UENUM(BlueprintType)
enum class EBtlCameraFocusLocationState : uint8
{
	NONE = 0,
	FOCUS_LOCATION_IN = 1,
	FOCUS_LOCATION_KEEP = 2,
	FOCUS_LOCATION_OUT = 3,
	EBtlCameraFocusLocationState_MAX = 4
};


UENUM(BlueprintType)
enum class EBtlCameraEasingState : uint8
{
	Easing_IN = 0,
	Easing_KEEP = 1,
	Easing_OUT = 2,
	Easing_END = 3,
	Easing_MAX = 4
};


UENUM(BlueprintType)
enum class EBtlBitFlagCategory : uint8
{
	Default = 0,
	Scenario = 1,
	CutScene = 2,
	Mystic = 3,
	StrikeAttack = 4,
	StrikeSmash = 5,
	BtlCamera = 6,
	VanguardMember = 7,
	Dead = 8,
	Arts = 9,
	Semiauto = 10,
	FlashRaid = 11,
	Runaway = 12,
	Animation = 13,
	DeathStroke = 14,
	Script = 15,
	UserInput = 16,
	UniqueSkill = 17,
	Win = 18,
	Lose = 19,
	Down = 20,
	Freeze = 21,
	Inspiration = 22,
	AI = 23,
	AssignControl = 24,
	RaiseDead = 25,
	Hidden = 26,
	Item = 27,
	MysticFade = 28,
	ScreenCaptcha = 29,
	DevelopmentSimulator = 30,
	Development = 31,
	EBtlBitFlagCategory_MAX = 32
};

UENUM(BlueprintType)
enum class EBtlCameraMode : uint8
{
	NORMAL = 0,
	LOCKON = 1,
	STRIKE_ATTACK = 2,
	FLASH_RAID = 3,
	STRIKE_SMASH = 4,
	FOCUS_UNIT = 5,
	SPECIAL_STRIKE_ATTACK = 6,
	EVENT = 7,
	CUTSCENE_TO = 8,
	FOCUS_LOCATION = 9,
	NORMAL_BATTLE_WIN = 10,
	OVERLIMIT = 11,
	INSPIRATION = 12,
	MODE_END = 13,
	MODE_BOGUS = 14,
	EBtlCameraMode_MAX = 15
};