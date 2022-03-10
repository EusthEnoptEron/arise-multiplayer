#pragma once
#include "BaseModule.h"


/// <summary>
/// Module that takes care of the communication between native code and blueprints.
/// </summary>
class BlueprintProxyModule :
    public BaseModule
{
public:
	virtual void Initialize(MultiplayerMod* mod) override;

private:
	static void Native_GetHudVisibilityImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
	static void Native_GetRootWidgetImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
	static void Native_PrintWidgetHierarchyImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
	
	static void Native_GetPlayerControllerImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
	static void Native_GetInputProcessImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);
	static void Native_SetProcessImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn);

	static void PrintHierarchy(SDK::UWidget* widget, int depth = 0);

	static MultiplayerMod* ModRef;
};

