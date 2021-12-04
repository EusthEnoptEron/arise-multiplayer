// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BtlEventDispatcher.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBattleEventDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeOperatiopnCaracterEventDelegate, class ABtlCharacterBase*,
                                             PrevOperationUnit, class ABtlCharacterBase*, NowOperationUnit);

/**
 * 
 */
UCLASS(BlueprintType)
class ARISE_API UBtlEventDispatcher : public UObject
{
	GENERATED_BODY()


	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnEquipmentChanged;
	// 0x0028(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnChangeOrderRequest;
	// 0x0038(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnPreInitializeBattle;
	// 0x0048(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnPostInitializeBattle;
	// 0x0058(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBeginBattle; // 0x0068(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnPostBeginBattle; // 0x0078(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnFinalMatchdownBattle;
	// 0x0088(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnPreEndBattleMaskComplated;
	// 0x0098(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnApplyReward; // 0x00A8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnReceivedReward; // 0x00B8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnEndResult; // 0x00C8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnEndBattle; // 0x00D8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnPostEndBattle; // 0x00E8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBattlePause; // 0x00F8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBattleResume; // 0x0108(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnChangePCOrder; // 0x0138(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnChangeFormation; // 0x0148(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnChangeMenuFormationRequest;
	// 0x0158(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnSpawnCharacter; // 0x0168(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnDestoryCharacter;
	// 0x0178(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FChangeOperatiopnCaracterEventDelegate OnChangeOperationCharacter;
	// 0x0188(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnChangeOperationMode;
	// 0x0198(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnChangeSemiautoState;
	// 0x01A8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnChangePCArts; // 0x01B8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBeginEscape; // 0x01C8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnCancelEscape; // 0x01D8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnSuccessEscape; // 0x01E8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBeginGameover; // 0x01F8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnRequestUseItem; // 0x0208(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnRequestUseItemBegin;
	// 0x0218(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnRequestUseItemEnd;
	// 0x0228(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnUseItem; // 0x0238(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnReusableItem; // 0x0248(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnRequestUseArts; // 0x0258(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnHitCombo; // 0x0268(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnEndCombo; // 0x0278(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnChangeTactics; // 0x0288(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnInspirationArts; // 0x0298(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnInspirationArtsEnd;
	// 0x02A8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnInspirationArtsFinalize;
	// 0x02B8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBattleVoiceEnd; // 0x02C8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBeginTutorial; // 0x02D8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnSuccessTutorial; // 0x02E8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnArtsEffect; // 0x02F8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnHealCharacter; // 0x0308(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnRaiseDeadCharacter;
	// 0x0318(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnDamageCharacter; // 0x0328(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnRecoilCharacter; // 0x0338(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBlownCharacter; // 0x0348(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnChangeHp; // 0x0358(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnDeadCharacter; // 0x0368(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnDefeatCountChanged;
	// 0x0378(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBeginOverlimitCharacter;
	// 0x0388(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnEndOverlimitCharacter;
	// 0x0398(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnSpecialDown; // 0x03A8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnTech; // 0x03B8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnAvoid; // 0x03C8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnJustAvoid; // 0x03D8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnJustGuard; // 0x03E8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBeginGuardAction;
	// 0x03F8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnEndGuardAction; // 0x0408(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBeginMomentaryRush;
	// 0x0418(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnEndMomentaryRush;
	// 0x0428(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBeginRingOut; // 0x0438(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnChangeTarget; // 0x0448(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnTargetedChanged; // 0x0458(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnTargetLost; // 0x0468(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnCursorTargetableChanged;
	// 0x0478(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnDownFlagOn; // 0x0488(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnDownFlagOff; // 0x0498(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBeginAbnormal; // 0x04A8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnEndAbnormal; // 0x04B8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnAbnormalStateChanged;
	// 0x04C8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnFakeDead; // 0x04D8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnReserveArtsMystic;
	// 0x04E8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnPreBeginArtsMystic;
	// 0x04F8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBeginArtsMystic; // 0x0508(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnEndArtsMystic; // 0x0518(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnEndArtsMysticFadeComplated;
	// 0x0528(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnStrikeSmashEnabled;
	// 0x0538(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnReserveArtsStrikeSmash;
	// 0x0548(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnChangeStrikeSmashState;
	// 0x0558(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBeginArtsStrikeSmash;
	// 0x0568(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnEndArtsStrikeSmash;
	// 0x0578(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnChangePlayerTargetSmashCondition;
	// 0x0588(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnStrikeSmashPossible;
	// 0x0598(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnReserveArtsSpecialStrike;
	// 0x05A8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBeginArtsSpecialStrike;
	// 0x05B8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnEndArtsSpecialStrike;
	// 0x05C8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnStrikeAttackEnabled;
	// 0x05D8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBeginArtsStrikeAttack;
	// 0x05E8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnEndArtsStrikeAttack;
	// 0x05F8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBeginArtsFlashRaid;
	// 0x0608(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnEndArtsFlashRaid;
	// 0x0618(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnChangeFlashRaidState;
	// 0x0628(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnReserveEvent; // 0x0638(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBeginEvent; // 0x0648(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnEndEvent; // 0x0658(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnCutSceneStart; // 0x0668(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnCutSceneEnd; // 0x0678(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnCutSceneReserveActionStart;
	// 0x0688(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnCutSceneReserveActionEnd;
	// 0x0698(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnCutSceneFinalize;
	// 0x06A8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnVanguardsDead; // 0x06B8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnSwapRearMembers; // 0x06C8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnPreStrikeSmashBegin;
	// 0x06D8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnPreStrikeSmashBeginWarpStriker;
	// 0x06E8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnPreStrikeSmashEnd;
	// 0x06F8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnStrikeAttackDownBegin;
	// 0x0708(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnStrikeAttackReserveDown;
	// 0x0718(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnCoreBreak; // 0x0728(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBoostAttackBreak;
	// 0x0738(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnMapChangeBegin; // 0x0748(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnMapChangeEnd; // 0x0758(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnDynamicEncountBegin;
	// 0x0768(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnDynamicEncountEnd;
	// 0x0778(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnGroundBegin; // 0x0788(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnAirBegin; // 0x0798(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnSystemBattleFinished;
	// 0x07A8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnDecideWin; // 0x07B8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnDecideLose; // 0x07C8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnRequestEnemyReinforcement;
	// 0x07D8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnFinishedEnemyReinforcement;
	// 0x07E8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBtlCameraModeChange;
	// 0x07F8(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBtlFollowingPlayerBegin;
	// 0x0808(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnBtlFollowingPlayerEnd;
	// 0x0818(0x0010) (ZeroConstructor, InstancedReference, BlueprintAssignable)

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBattleEventDelegate OnChangeTp;
};
