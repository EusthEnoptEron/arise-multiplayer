// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include <stdio.h>

#include "GameFramework/PlayerInput.h"


void AMyPlayerController::PlayerTick(float DeltaTime)
{
	APlayerController::PlayerTick(DeltaTime);
	UPlayerInput* input = this->PlayerInput;
	input->InputKey(FKey("SpaceBar"), EInputEvent::IE_Pressed, 0.0f, 0.0f);
	//->PlayerInput->InputKey(FKey(FName("Space")));
}
