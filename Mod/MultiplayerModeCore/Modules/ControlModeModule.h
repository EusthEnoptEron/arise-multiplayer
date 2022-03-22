#pragma once
#include "BaseModule.h"
class ControlModeModule :
    public BaseModule
{
public:
    virtual void Initialize(MultiplayerMod* mod) override;

private:
    // Hooks
    static void GetPlayerOperationHook(UE4::UObject* Context, UE4::FFrame& Stack, bool* result);
    static void IsAutoOperationHook(UE4::UObject* Context, UE4::FFrame& Stack, bool* result);
    static FNativeFuncPtr GetPlayerOperation;
    static FNativeFuncPtr IsAutoOperation;

    static void OnOperationUnitChanged(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
    static void OnSetTurnTargetArts(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);


    static UE4::UFunction* DerivedInputStateComponent__GetOwnerFn;
};

