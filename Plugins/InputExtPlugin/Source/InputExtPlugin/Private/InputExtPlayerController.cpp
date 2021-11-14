// Fill out your copyright notice in the Description page of Project Settings.

#include "InputExtPlayerController.h"

bool  AInputExtPlayerController::K2_ShowSteamBindingPanel() { return false; }
void  AInputExtPlayerController::K2_ShowMouseCursor(bool iShow) {}
void  AInputExtPlayerController::K2_SetPadAssignList(TMap<FName, int> OutAssignList) {}
void  AInputExtPlayerController::K2_SetKeyboardAssignList(TMap<FName, int> OutAssignList) {}
void  AInputExtPlayerController::K2_SetInputModeArise(bool bMouseHold) {}
void  AInputExtPlayerController::K2_SetInputAnalogButtonThreshold(float Value) {}
void  AInputExtPlayerController::K2_SetHoldMousePosition(bool iHold) {}
void  AInputExtPlayerController::K2_SetHasWindowFocus(bool iHold) {}
void  AInputExtPlayerController::K2_SetCategoryFilter(int CategoryFilter) {}
void  AInputExtPlayerController::K2_ResetPadAssignList() {}
void  AInputExtPlayerController::K2_ResetKeyboardAssignList() {}
void  AInputExtPlayerController::K2_ResetKeyboardAndMouseAssignList() {}
void  AInputExtPlayerController::K2_ResetCategoryFilter() {}
void  AInputExtPlayerController::K2_Reset() {}
void  AInputExtPlayerController::K2_RemoveAssign() {}
void  AInputExtPlayerController::K2_PushInputProcess2(UClass* Class) {}
void  AInputExtPlayerController::K2_PushInputProcess(AInputExtInputProcessBase* NewProcess) {}
void  AInputExtPlayerController::K2_PushInputLayer(const FString& Name, TArray<FName> ButtonNames, bool BlockAll) {}
void  AInputExtPlayerController::K2_PopInputProcessLayer(int Layer, bool bDestroyProcess) {}
void  AInputExtPlayerController::K2_PopInputProcess(bool bDestroyProcess) {}
void  AInputExtPlayerController::K2_PopInputLayer(const FString& Name) {}
bool  AInputExtPlayerController::K2_IsSteamControllerEnable() { return false; }
bool  AInputExtPlayerController::K2_IsShowMouseCursor() { return false; }
bool  AInputExtPlayerController::K2_IsMouseMove() { return false; }
bool  AInputExtPlayerController::K2_IsHoldMousePosition() { return false; }
bool  AInputExtPlayerController::K2_IsButtonRepeated(const FName& ButtonName) { return false; }
bool  AInputExtPlayerController::K2_IsButtonPressed(const FName& ButtonName) { return false; }
bool  AInputExtPlayerController::K2_IsButtonJustReleased(const FName& ButtonName) { return false; }
bool  AInputExtPlayerController::K2_IsButtonJustPressed(const FName& ButtonName) { return false; }
bool  AInputExtPlayerController::K2_IsButtonDefined(const FName& ButtonName) { return false; }
bool  AInputExtPlayerController::K2_IsAxisDefined(const FName& AxisName) { return false; }
bool  AInputExtPlayerController::K2_IsAnyMouseButtonPressed() { return false; }
bool  AInputExtPlayerController::K2_IsAnyKeyboardPressed() { return false; }
bool  AInputExtPlayerController::K2_IsAnyGamepadPressed() { return false; }
bool  AInputExtPlayerController::K2_InputActivate(APlayerController* OldPlayerController) { return false; }
bool  AInputExtPlayerController::K2_HasWindowFocus() { return false; }
void  AInputExtPlayerController::K2_GetPadKeys_DEPRECATED(const FName& ButtonName, TArray<FKey>& OutKeys, TArray<FKey>& OutShifts) {}
void  AInputExtPlayerController::K2_GetPadFixedList(TMap<FName, int>& OutAssignList) {}
void  AInputExtPlayerController::K2_GetPadAssignList(TMap<FName, int>& OutAssignList) {}
void  AInputExtPlayerController::K2_GetKeyboardKeys_DEPRECATED(const FName& ButtonName, TArray<FKey>& OutKeys, TArray<FKey>& OutShifts) {}
void  AInputExtPlayerController::K2_GetKeyboardAssignList(TMap<FName, int>& OutAssignList) {}
float  AInputExtPlayerController::K2_GetInputAnalogButtonThreshold() { return 0.0f; }
AInputExtInputProcessBase* AInputExtPlayerController::K2_GetCurrentInputProcess() { return nullptr; }
uint8 AInputExtPlayerController::K2_GetCategoryFilter() { return '0'; }
float  AInputExtPlayerController::K2_GetAxisValue(const FName& AxisName) { return 0.0f; }
void  AInputExtPlayerController::K2_DumpAssign() {}
void  AInputExtPlayerController::K2_AddThresholdTime(const FName& Name, float Time) {}
void  AInputExtPlayerController::K2_AddShift_DEPRECATED(const FName& Name, const FKey& Key) {}
void  AInputExtPlayerController::K2_AddButton(const FName& ButtonName, const FKey& Key) {}
void  AInputExtPlayerController::K2_AddAxis(const FName& AxisName, const FKey& Key, float Scale) {}