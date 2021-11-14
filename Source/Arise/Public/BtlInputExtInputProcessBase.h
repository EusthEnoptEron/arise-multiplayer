// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InputExtInputProcessBase.h"
#include "Btl_Camera.h"
#include "BtlInputExtInputProcessBase.generated.h"

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

UCLASS()
class ARISE_API ABtlInputExtInputProcessBase : public AInputExtInputProcessBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABtlInputExtInputProcessBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetMoveToWorld(FVector Vector);

	UFUNCTION(BlueprintCallable)
	void SetInputEnable(bool Enable, EBtlBitFlagCategory Category);
	
	UFUNCTION(BlueprintCallable)
	void SetInputConfigFlag(EBtlInputConfigType ConfigType, bool bEnable);

	UFUNCTION(BlueprintCallable)
	void OnChangeAriseGameState(EAriseGameState ChangedState, bool bNewState);

};
