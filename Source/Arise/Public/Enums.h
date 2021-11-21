#pragma once
#include "CoreMinimal.h"
#include "ObjectMacros.h"


UENUM(BlueprintType)
enum class EBtlInputConfigType : uint8
{
	InputEnable = 0,
	BattleFinish = 1,
	MainStateOnly = 2,
	CutScene = 3,
	Pause = 4,
	DebugWindow = 5,
	Tutorial = 6,
	SystemPause = 7,
	MetaScript = 8,
	PlayerChangeFormation = 9,
	MapChange = 10,
	BoostStrike = 11,
	EBtlInputConfigType_MAX = 12
};


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
enum class EBattleStateAction : uint8
{
	None                           = 0,
	Initialize                     = 1,
	WaitComplete                   = 2,
	Activate                       = 3,
	Execute                        = 4,
	Finalize                       = 5,
	WaitFinalize                   = 6,
	EBattleStateAction_MAX         = 7
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

UENUM(BlueprintType)
enum class EAriseGameState : uint8
{
	GameState_Pause = 0,
	GameState_ScreenMask = 1,
	GameState_Nowloading = 2,
	GameState_StreamingLevelLoading = 3,
	GameState_PlayerFreeLock = 4,
	GameState_PlayCutSceneEvent = 5,
	GameState_MapChanging = 6,
	GameState_Battle = 7,
	GameState_BattleTraining = 8,
	GameState_PlayEvent = 9,
	GameState_PlayQuest = 10,
	GameState_PlayLongChat = 11,
	GameState_EventFade = 12,
	GameState_ActionLadder = 13,
	GameState_ActionSwim = 14,
	GameState_ActionGrab = 15,
	GameState_Fishing = 16,
	GameState_AutoSaving = 17,
	GameState_EventContinue = 18,
	GameState_EncountLock = 19,
	GameState_ActionDash = 20,
	GameState_ActionFloating = 21,
	GameState_PlayScript = 22,
	GameState_Camp = 23,
	GameState_QuestContinue = 24,
	GameState_PlayLookAtCamera = 25,
	GameState_BattleResultShowHud = 26,
	GameState_PlayInteract = 27,
	GameState_ExecuteEnableControlDelegate = 28,
	GameState_RequestEnableControlDelegate = 29,
	GameState_Menu = 30,
	GameState_MenuClosing = 31,
	GameState_NearMapLink = 32,
	GameState_DoFastTravel = 33,
	GameState_GoBackToTitle = 34,
	GameState_UIControl = 35,
	GameState_Encounting = 36,
	GameState_PostGameDataLoaded = 37,
	GameState_PassBlocked = 38,
	GameState_PlayerFreeLockUntilMapJump = 39,
	GameState_SaveLoading = 40,
	GameState_TextureStreaming = 41,
	GameState_QuestAutoSaveProcess = 42,
	GameState_DebugMenuOpen = 43,
	GameState_Sandbox = 44,
	GameState_FlyMode = 45,
	GameState_MAX = 46
};


UENUM(BlueprintType)
enum class EBtlInputEventType : uint8
{
	ButtonPressed = 0,
	ButtonJustPressed = 1,
	ButtonJustReleased = 2,
	ButtonRepeated = 3,
	EBtlInputEventType_MAX = 4
};

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

// Enum Arise.EOperationMode
UENUM(BlueprintType)
enum class EOperationMode : uint8
{
	OPERATION_MODE_AUTO = 0,
	OPERATION_MODE_SEMI_AUTO = 1,
	OPERATION_MODE_MANUAL = 2,
	OPERATION_MODE_MAX = 3
};


// Enum Arise.EBattleState
UENUM(BlueprintType)
enum class EBattleState : uint8
{
	StateNone = 0,
	StateInitialize = 1,
	StatePrev = 2,
	StateMain = 3,
	StatePostMain = 4,
	StateMenu = 5,
	StateEvent = 6,
	StateGameover = 7,
	StateRetry = 8,
	StateResult = 9,
	StateRunaway = 10,
	StateTutorial = 11,
	StatePause = 12,
	StateFinalize = 13,
	EBattleState_MAX = 14
};
