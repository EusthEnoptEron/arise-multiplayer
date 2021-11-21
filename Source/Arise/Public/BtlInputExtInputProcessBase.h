// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "InputExtInputProcessBase.h"
#include "Btl_Camera.h"
#include "Enums.h"
#include "BtlInputExtInputProcessBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FButtonMulticastDelegate, FName, name, EBtlInputEventType, type);

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_DELEGATE_TwoParams(FButtonCallback, FName, name, EBtlInputEventType, type);

USTRUCT(BlueprintType)
struct FBtlInputConfig_DatatableLabel
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
	FName Label; // 0x0000(0x0008) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
};

USTRUCT(BlueprintType)
struct FBtlInputConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName ButtonName; // 0x0008(0x0008) (Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData)

	UPROPERTY(BlueprintReadOnly)
	EBtlInputEventType EventType; // 0x0010(0x0001) (Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData)

	unsigned char UnknownData00[0x3]; // 0x0011(0x0003) MISSED OFFSET

	uint32 Config; // 0x0014(0x0004) (Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData)
};


// ScriptStruct Arise.BtlInputEventData
// 0x0030
USTRUCT(BlueprintType)
struct FBtlInputEventData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName ButtonName; // 0x0000(0x0008) (Edit, ZeroConstructor, DisableEditOnTemplate, EditConst, IsPlainOldData)

	UPROPERTY(BlueprintReadOnly)
	EBtlInputEventType InputType;
	// 0x0008(0x0001) (Edit, ZeroConstructor, DisableEditOnTemplate, EditConst, IsPlainOldData)

	UPROPERTY(BlueprintReadOnly)
	bool bEnableEvent; // 0x0009(0x0001) (ZeroConstructor, IsPlainOldData)
	unsigned char UnknownData00[0x6]; // 0x000A(0x0006) MISSED OFFSET

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FButtonMulticastDelegate Events;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FButtonMulticastDelegate AlwaysEvents;
};

USTRUCT(BlueprintType)
struct FBtlInputEventConfigData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FBtlInputConfig_DatatableLabel ConfigLabel; // 0x0000(0x0008)

	UPROPERTY(BlueprintReadOnly)
	FBtlInputConfig ConfigData; // 0x0008(0x0018)


	UPROPERTY(BlueprintReadOnly, meta = (GetByRef))
	FButtonCallback Event; // 0x0020(0x0014) (ZeroConstructor, InstancedReference)

	UPROPERTY(BlueprintReadOnly, meta = (GetByRef))
	FButtonCallback ChangeEnableEvent; // 0x0030(0x0014) (ZeroConstructor, InstancedReference)

	UPROPERTY(BlueprintReadOnly)
	bool bEnableEvent; // 0x0040(0x0001) (ZeroConstructor, IsPlainOldData)
	unsigned char UnknownData00[0x7];
};

USTRUCT(BlueprintType)

struct FBtlInputConfigEvent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName ButtonName; // 0x0000(0x0008) (Edit, ZeroConstructor, DisableEditOnTemplate, EditConst, IsPlainOldData)

	UPROPERTY(BlueprintReadOnly)
	EBtlInputEventType InputType;
	// 0x0008(0x0001) (Edit, ZeroConstructor, DisableEditOnTemplate, EditConst, IsPlainOldData)

	unsigned char UnknownData00[0x7]; // 0x0009(0x0007) MISSED OFFSET

	UPROPERTY(BlueprintAssignable, BlueprintCallable, BlueprintReadWrite)
	FButtonMulticastDelegate Events; // 0x0010(0x0010) (ZeroConstructor, InstancedReference)
};


UCLASS()
class ARISE_API ABtlInputExtInputProcessBase : public AInputExtInputProcessBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBtlInputEventData> ButtonEvents;
	// 0x0350(0x0010) (Edit, ZeroConstructor, DisableEditOnTemplate, Transient, EditConst)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBtlInputEventConfigData> ConfigButtons;
	// 0x0360(0x0010) (Edit, ZeroConstructor, DisableEditOnTemplate, Transient, EditConst)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBtlInputConfigEvent> ConfigEvents;


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

	UFUNCTION(BlueprintCallable)
	void RegisterInputEvent(const FName& InButtonName, EBtlInputEventType Type, const FButtonCallback& Event);
};
