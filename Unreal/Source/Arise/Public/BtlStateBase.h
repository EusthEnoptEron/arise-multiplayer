// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "UObject/NoExportTypes.h"
#include "BtlStateBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ARISE_API UBtlStateBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	EBattleStateAction                                 Process;                                                  // 0x0028(0x0001) (BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData)

};
