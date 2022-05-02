// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BtlCameraLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARISE_API UBtlCameraLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static ACameraActor* SetActiveCamera(ACameraActor* Camera);

	UFUNCTION(BlueprintCallable)
	static ACameraActor* GetActiveCamera();
};
