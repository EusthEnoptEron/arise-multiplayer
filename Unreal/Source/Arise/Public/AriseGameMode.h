// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "AriseGameMode.generated.h"


UENUM(BlueprintType)
enum class EAriseGameScene : uint8
{
	Title = 0,
	DebugMap = 1,
	Field = 2,
	Battle = 3,
	Menu = 4,
	BattleMenu = 5,
	Camp = 6,
	LongChat = 7,
	Movie = 8,
	Fishing = 9,
	PlayerFree = 10,
	Event = 11,
	Quest = 12,
	Ending = 13,
	Boot = 14,
	Login = 15,
	ToLogin = 16,
	TitleBack = 17,
	MaxScene = 18,
	None = 19,
	EAriseGameScene_MAX = 20
};

/**
 * 
 */
UCLASS()
class ARISE_API AAriseGameMode : public AGameMode
{
	GENERATED_BODY()
	
	
	UFUNCTION(BlueprintCallable)
	EAriseGameScene GetDisplayScene();

	UFUNCTION(BlueprintCallable)
	void DebugCamera(bool bEnable);

	UFUNCTION(BlueprintCallable)
	void SetSystemPauseRequestEnable(bool bEnable);
	
	UFUNCTION(BlueprintCallable)
	void RequestSystemPause();

	UFUNCTION(BlueprintCallable)
	bool IsPausePossibleState();
	
	UFUNCTION(BlueprintCallable)
	bool IsSystemPauseRequestEnabled();

	UFUNCTION(BlueprintCallable)
	void FlushRequestSystemPause();

	UFUNCTION(BlueprintCallable)
	int CheckSystemPauseState();
	
	UFUNCTION(BlueprintCallable)
	void CancelSystemPause();

	UFUNCTION(BlueprintCallable)
	void CancelSystemPauseAll();
};
