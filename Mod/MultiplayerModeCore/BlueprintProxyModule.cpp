#include "BlueprintProxyModule.h"

MultiplayerMod * BlueprintProxyModule::ModRef = nullptr;


void BlueprintProxyModule::Initialize(MultiplayerMod* mod)
{
	mod->AddBlueprintHook(
		"BP_ModHelper.BP_ModHelper_C.Native_GetHudVisibility",
		Native_GetHudVisibilityImpl
	);

	mod->AddBlueprintHook(
		"BP_ModHelper.BP_ModHelper_C.Native_GetRootWidget",
		Native_GetRootWidgetImpl
	);

	mod->AddBlueprintHook(
		"BP_ModHelper.BP_ModHelper_C.Native_PrintWidgetHierarchy",
		Native_PrintWidgetHierarchyImpl
	);

	mod->AddBlueprintHook(
		"MultiPlayerController.MultiPlayerController_C.Native_GetPlayerController",
		Native_GetPlayerControllerImpl
	);	
	
	mod->AddBlueprintHook(
		"MultiPlayerController.MultiPlayerController_C.Native_GetInputProcess",
		Native_GetInputProcessImpl
	);	
	
	mod->AddBlueprintHook(
		"MultiPlayerController.MultiPlayerController_C.Native_SetProcess",
		Native_SetProcessImpl
	);

	ModRef = mod;

}

void BlueprintProxyModule::Native_GetHudVisibilityImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	static auto BtlFunctionLibrary__GetUIManager = UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlFunctionLibrary.GetUIManager");
	static auto BP_BattleHudHelper__GetHudVisible = UE4::UObject::FindObject<UE4::UFunction>("Function BP_BattleHudHelper.BP_BattleHudHelper_C.GetHudVisible");

	struct params {
		UE4::FString Input;
		SDK::ESlateVisibility Visibility;
	};


	SDK::ABattleUIManager* uiManager;
	Context->ProcessEvent(BtlFunctionLibrary__GetUIManager, &uiManager);

	struct UBP_BattleHudHelper_C_GetHudVisible_Params
	{
		SDK::FName                                       RowName;                                                  // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
		UE4::UObject* __WorldContext;                                           // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
		SDK::FSTR_BtlHudVisible                          Result;                                                   // (Parm, OutParm)
	};


	UBP_BattleHudHelper_C_GetHudVisible_Params args = {
		uiManager->HudVisiblePresetLabel.Label,
		Context
	};

	Context->ProcessEvent(BP_BattleHudHelper__GetHudVisible, &args);

	auto inputString = Stack.GetParams<UE4::FString>()->ToString();
	bool visible = false;

	if (inputString == "Target") {
		visible = args.Result.TargetBar_13_83206C884702689F60AAC6AF3DF8818C;
	}
	else if (inputString == "PlayerBar") {
		visible = args.Result.OperationBar_12_5EEA505A4C436902CEE0C8B6EEE41597;
	}
	else if (inputString == "ArtsHelp") {

		visible = args.Result.ArtsHelpText_15_86BBD9224BA6A4D3FB62968F8B2CC8F8;
	}
	else {
		Log::Info("Unknown: %s", inputString.c_str());
	}

	auto ret2 = ((FOutParmRec*)Stack.OutParms)->PropAddr;

	// I have no idea why this isn't just ret
	*ret2 = static_cast<uint8_t>(visible ? SDK::ESlateVisibility::Visible : SDK::ESlateVisibility::Hidden);

}

void BlueprintProxyModule::Native_GetRootWidgetImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	auto widget = (SDK::UUserWidget*)*(Stack.GetParams<UE4::UObject*>());

	//PrintHierarchy(widget, 0);

	if (widget == nullptr) {
		Log::Info("[GetRootWidget] Got NULL reference.");
		processFn(Context, Stack, result);
	}

	auto ret2 = (SDK::UObject**)((FOutParmRec*)Stack.OutParms)->PropAddr;
	*ret2 = widget->WidgetTree->RootWidget;
}

void BlueprintProxyModule::Native_PrintWidgetHierarchyImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	auto widget = (SDK::UUserWidget*)*(Stack.GetParams<UE4::UObject*>());
	PrintHierarchy(widget, 0);

	processFn(Context, Stack, result);
}

void BlueprintProxyModule::Native_GetPlayerControllerImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	*((UE4::APlayerController**)((FOutParmRec*)Stack.OutParms)->PropAddr) = ModRef->Controllers[0];
}

void BlueprintProxyModule::Native_GetInputProcessImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	*((UE4::AActor**)((FOutParmRec*)Stack.OutParms)->PropAddr) = ModRef->InputProcesses[0];
}

void BlueprintProxyModule::Native_SetProcessImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	struct params {
		UE4::AActor* Process;
		int Index;
	};
	auto args = *(Stack.GetParams<params>());

	ModRef->InputProcesses[args.Index] = args.Process;

	Log::Info("Set process %d to %p / %p", args.Index, args.Process, ModRef->InputProcesses[args.Index]);

	processFn(Context, Stack, result);
}


void BlueprintProxyModule::PrintHierarchy(SDK::UWidget* widget, int depth) {
	Log::Info("%s%s (%s)", std::string(depth * 2, ' ').c_str(), widget->GetName().c_str(), widget->Class->GetName().c_str());
	if (widget->IsA(SDK::UPanelWidget::StaticClass())) {
		auto panelWidget = (SDK::UPanelWidget*)widget;

		for (int i = 0; i < panelWidget->Slots.Num(); i++) {
			PrintHierarchy(panelWidget->Slots[i]->Content, depth + 1);
		}
	}
	else if (widget->IsA(SDK::UUserWidget::StaticClass())) {
		auto userWidget = (SDK::UUserWidget*)widget;
		PrintHierarchy(userWidget->WidgetTree->RootWidget, depth + 1);
	}
}
