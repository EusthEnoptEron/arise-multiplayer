#pragma once
#include "BaseModule.h"
class TestWidgetModule :
    public BaseModule
{
public:
    virtual void Initialize(MultiplayerMod* mod) override;
private:
    static void OnLogInfo(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
    static void Native_WinImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
    static void Native_BattleImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);


};

