// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BtlStatusViewModelBase.h"
#include "BtlStatusViewBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ARISE_API UBtlStatusViewBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	UBtlStatusViewModelBase* ViewModel;

	UFUNCTION(BlueprintCallable)
	void SetViewModel(UBtlStatusViewModelBase* NewViewModel);

	UFUNCTION(BlueprintCallable)
	void ChangeViewModelRequest(UBtlStatusViewModelBase* NewViewModel);

	UFUNCTION(BlueprintCallable)
	void BindEvent();
};
