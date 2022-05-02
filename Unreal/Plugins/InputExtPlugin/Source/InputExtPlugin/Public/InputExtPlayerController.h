// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputExtInputProcessBase.h"
#include "InputExtPlayerController.generated.h"

/**
 *
 */
UCLASS()
class INPUTEXTPLUGIN_API AInputExtPlayerController : public APlayerController
{
public:

	bool                                               bIsValid;                                                 // 0x0678(0x0001) (Edit, BlueprintVisible, ZeroConstructor, EditConst, IsPlainOldData)
	unsigned char                                      UnknownData00[0x12F];                                     // 0x0679(0x012F) MISSED OFFSET

	UPROPERTY(BlueprintReadWrite)
		TArray<AInputExtInputProcessBase*>           InputProcessStackLayer0;                                  // 0x07A8(0x0010) (ZeroConstructor)

	UPROPERTY(BlueprintReadWrite)
		TArray<AInputExtInputProcessBase*>           InputProcessStackLayer1;


	GENERATED_BODY()


		UFUNCTION(BlueprintCallable)
		bool K2_ShowSteamBindingPanel();
	UFUNCTION(BlueprintCallable)
		void K2_ShowMouseCursor(bool iShow);
	UFUNCTION(BlueprintCallable)
		void K2_SetPadAssignList(TMap<FName, int> OutAssignList);
	UFUNCTION(BlueprintCallable)
		void K2_SetKeyboardAssignList(TMap<FName, int> OutAssignList);
	UFUNCTION(BlueprintCallable)
		void K2_SetInputModeArise(bool bMouseHold);
	UFUNCTION(BlueprintCallable)
		void K2_SetInputAnalogButtonThreshold(float Value);
	UFUNCTION(BlueprintCallable)
		void K2_SetHoldMousePosition(bool iHold);
	UFUNCTION(BlueprintCallable)
		void K2_SetHasWindowFocus(bool iHold);
	UFUNCTION(BlueprintCallable)
		void K2_SetCategoryFilter(int CategoryFilter);
	UFUNCTION(BlueprintCallable)
		void K2_ResetPadAssignList();
	UFUNCTION(BlueprintCallable)
		void K2_ResetKeyboardAssignList();
	UFUNCTION(BlueprintCallable)
		void K2_ResetKeyboardAndMouseAssignList();
	UFUNCTION(BlueprintCallable)
		void K2_ResetCategoryFilter();
	UFUNCTION(BlueprintCallable)
		void K2_Reset();
	UFUNCTION(BlueprintCallable)
		void K2_RemoveAssign();
	UFUNCTION(BlueprintCallable)
		void K2_PushInputProcess2(UClass* Class);
	UFUNCTION(BlueprintCallable)
		void K2_PushInputProcess(AInputExtInputProcessBase* NewProcess);
	UFUNCTION(BlueprintCallable)
		void K2_PushInputLayer(const FString& Name, TArray<FName> ButtonNames, bool BlockAll);
	UFUNCTION(BlueprintCallable)
		void K2_PopInputProcessLayer(int Layer, bool bDestroyProcess);
	UFUNCTION(BlueprintCallable)
		void K2_PopInputProcess(bool bDestroyProcess);
	UFUNCTION(BlueprintCallable)
		void K2_PopInputLayer(const FString& Name);
	UFUNCTION(BlueprintCallable)
		bool K2_IsSteamControllerEnable();
	UFUNCTION(BlueprintCallable)
		bool K2_IsShowMouseCursor();
	UFUNCTION(BlueprintCallable)
		bool K2_IsMouseMove();
	UFUNCTION(BlueprintCallable)
		bool K2_IsHoldMousePosition();
	UFUNCTION(BlueprintCallable)
		bool K2_IsButtonRepeated(const FName& ButtonName);
	UFUNCTION(BlueprintCallable)
		bool K2_IsButtonPressed(const FName& ButtonName);
	UFUNCTION(BlueprintCallable)
		bool K2_IsButtonJustReleased(const FName& ButtonName);
	UFUNCTION(BlueprintCallable)
		bool K2_IsButtonJustPressed(const FName& ButtonName);
	UFUNCTION(BlueprintCallable)
		bool K2_IsButtonDefined(const FName& ButtonName);
	UFUNCTION(BlueprintCallable)
		bool K2_IsAxisDefined(const FName& AxisName);
	UFUNCTION(BlueprintCallable)
		bool K2_IsAnyMouseButtonPressed();
	UFUNCTION(BlueprintCallable)
		bool K2_IsAnyKeyboardPressed();
	UFUNCTION(BlueprintCallable)
		bool K2_IsAnyGamepadPressed();
	UFUNCTION(BlueprintCallable)
		bool K2_InputActivate(APlayerController* OldPlayerController);
	UFUNCTION(BlueprintCallable)
		bool K2_HasWindowFocus();
	UFUNCTION(BlueprintCallable)
		void K2_GetPadKeys_DEPRECATED(const FName& ButtonName, TArray<FKey>& OutKeys, TArray<FKey>& OutShifts);
	UFUNCTION(BlueprintCallable)
		void K2_GetPadFixedList(TMap<FName, int>& OutAssignList);
	UFUNCTION(BlueprintCallable)
		void K2_GetPadAssignList(TMap<FName, int>& OutAssignList);
	UFUNCTION(BlueprintCallable)
		void K2_GetKeyboardKeys_DEPRECATED(const FName& ButtonName, TArray<FKey>& OutKeys, TArray<FKey>& OutShifts);
	UFUNCTION(BlueprintCallable)
		void K2_GetKeyboardAssignList(TMap<FName, int>& OutAssignList);
	UFUNCTION(BlueprintCallable)
		float K2_GetInputAnalogButtonThreshold();
	UFUNCTION(BlueprintCallable)
		AInputExtInputProcessBase* K2_GetCurrentInputProcess();
	UFUNCTION(BlueprintCallable)
		uint8 K2_GetCategoryFilter();
	UFUNCTION(BlueprintCallable)
		float K2_GetAxisValue(const FName& AxisName);
	UFUNCTION(BlueprintCallable)
		void K2_DumpAssign();
	UFUNCTION(BlueprintCallable)
		void K2_AddThresholdTime(const FName& Name, float Time);
	UFUNCTION(BlueprintCallable)
		void K2_AddShift_DEPRECATED(const FName& Name, const FKey& Key);
	UFUNCTION(BlueprintCallable)
		void K2_AddButton(const FName& ButtonName, const FKey& Key);
	UFUNCTION(BlueprintCallable)
		void K2_AddAxis(const FName& AxisName, const FKey& Key, float Scale);

};
