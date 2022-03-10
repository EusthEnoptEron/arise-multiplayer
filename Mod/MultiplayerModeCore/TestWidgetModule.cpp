#include "TestWidgetModule.h"

void TestWidgetModule::Initialize(MultiplayerMod* mod)
{

	mod->AddBlueprintHook(
		"TestWidget.TestWidget_C.LogInfo",
		OnLogInfo
	);

	mod->AddBlueprintHook(
		"TestWidget.TestWidget_C.Native_Win",
		Native_WinImpl
	);

	mod->AddBlueprintHook(
		"TestWidget.TestWidget_C.Native_Battle",
		Native_BattleImpl
	);
}

void TestWidgetModule::OnLogInfo(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	struct param {
		UE4::FString A;
		UE4::FString B;
	};

	param* parms = Stack.GetParams<param>();
	//Log::Info("[Player] %s%s", parms->A.IsValid() ? parms->A.ToString().c_str() : "", parms->B.IsValid() ? parms->B.ToString().c_str() : "");

	processFn(Context, Stack, result);
}

void TestWidgetModule::Native_WinImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	auto modActor = MultiplayerMod::GetInstance()->ModActor;
	((SDK::UBtlFunctionLibrary*)modActor)->STATIC_GetBattleManager((SDK::AActor*)modActor)->MetaScript->SetBattleWin(0.0f);

	processFn(Context, Stack, result);
}

void TestWidgetModule::Native_BattleImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	struct PseudoTArray {
		SDK::FBtlEncountGroupParam* group;
		int count = 1;
	};

	SDK::FBtlEncountGroupParam param;
	PseudoTArray params = { &param, 1 };
	param.Label = "EGR_DEBUG_BID_EFR_001";


	((SDK::UBtlLauncherWorkerLibrary*)Context)->STATIC_SpawnBtlLauncherWorker((SDK::UObject*)Context, SDK::FString(L"MIT_B04"), *((SDK::TArray<SDK::FBtlEncountGroupParam>*) & params), 0.0f, true, false, L"");


	processFn(Context, Stack, result);
}
