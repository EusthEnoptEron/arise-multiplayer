// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARISE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	void PlayerTick(float DeltaTime) override;
};
