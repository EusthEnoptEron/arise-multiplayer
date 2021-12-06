// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleUIManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHudVisiblePresetEventDelegate, FName, label);

USTRUCT(BlueprintType)
struct FSTR_BtlHudVisible
{
	GENERATED_BODY()

public:
	bool OperationBar;
	// 0x0000(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	UPROPERTY(BlueprintReadOnly)
	bool TargetBar;
	// 0x0001(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)

	UPROPERTY(BlueprintReadOnly)
	bool PartyBar;
	// 0x0002(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	UPROPERTY(BlueprintReadOnly)
	bool PartyPlayerBar;
	// 0x0003(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	bool BossGauge;
	// 0x0004(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	bool TP;
	// 0x0005(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	bool ItemGauge;
	// 0x0006(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	bool Strike;
	// 0x0007(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	bool StrikePlayerFlag;
	// 0x0008(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	bool EscapeGauge;
	// 0x0009(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	UPROPERTY(BlueprintReadOnly)
	bool ArtsHelpText;
	// 0x000A(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	bool ArtsNotifyText;
	// 0x000B(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	bool ComboDamage;
	// 0x000C(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	bool Damage3D;
	// 0x000D(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	bool Spell;
	// 0x000E(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	bool ShortChat;
	// 0x000F(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	bool Tutorial;
	// 0x0010(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	bool BackUnderMat;
	// 0x0011(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	bool EncountEnemys;
	// 0x0012(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
};

class UBtlCommonViewModelBase;
UCLASS()
class ARISE_API ABattleUIManager : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FName HudVisiblePresetLabel;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FHudVisiblePresetEventDelegate OnChangeVisiblePreset;

	// Sets default values for this actor's properties
	ABattleUIManager();

	UPROPERTY(BlueprintReadOnly)
	UBtlCommonViewModelBase* CommonViewModel;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnBeginChangeTarget();

	UFUNCTION(BlueprintCallable)
	void ShowLockon();
};
