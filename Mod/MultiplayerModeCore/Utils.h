#pragma once
#include "Mod/Mod.h"
#include "MultiplayerMod.h";
#include "../SDK.h";


struct FScriptName {
	/** Index into the Names array (used to find String portion of the string/number pair used for comparison) */
	int32_t		ComparisonIndex;
	/** Index into the Names array (used to find String portion of the string/number pair used for display) */
	int32_t		DisplayIndex;
	/** Number portion of the string/number pair (stored internally as 1 more than actual, so zero'd memory will be the default, no-instance case) */
	uint32_t			Number;

	std::string GetName() {
		return UE4::FName(ComparisonIndex).GetName();
	}
};


UE4::UClass* FastGetClass(UE4::UObject* obj);
UE4::UStruct* FastGetSuperField(UE4::UStruct* obj);
bool FastIsA(UE4::UObject* obj, UE4::UClass* cmp);

// Template function, hence definition must be known in the header file.
template<typename T> T GetParam(UE4::FFrame& Stack, bool progress) {
	T val;

	auto saveCode = Stack.Code;
	int32 comp = *(Stack.Code++);
	MultiplayerMod::GNatives[comp](Stack.Object, Stack, &val);

	if (!progress) {
		Stack.Code = saveCode;
	}

	return val;
}

std::string GetName(UE4::FFrame& Stack);

void PrintStackTrace(UE4::UObject* Context, UE4::FFrame& Stack);

SDK::FTransform GetTransform(const SDK::AActor* actor);

UE4::UFunction* FindFunction(const std::string path);