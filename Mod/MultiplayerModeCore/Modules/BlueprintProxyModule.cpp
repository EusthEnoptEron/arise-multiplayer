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
		"BP_ModHelper.BP_ModHelper_C.Native_GetControllers",
		Native_GetControllersImpl
	);

	mod->AddBlueprintHook(
		"BP_ModHelper.BP_ModHelper_C.Native_GetControllerCount",
		Native_GetControllerCountImpl
	);

	mod->AddBlueprintHook(
		"BP_ModHelper.BP_ModHelper_C.Native_SetControllers",
		Native_SetControllersImpl
	);

	mod->AddBlueprintHook(
		"BP_ModHelper.BP_ModHelper_C.Native_SetVibration",
		Native_SetVibrationImpl
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

void BlueprintProxyModule::Native_GetControllerCountImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	*((int32_t*)((FOutParmRec*)Stack.OutParms)->PropAddr) = InputManager::GetInstance()->Controllers.size();
}

void BlueprintProxyModule::Native_GetControllersImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	static const auto controllerDataClazz = SDK::UClass::FindClass("Class ControllerData.ControllerData_C");
	static const auto controllerDataSetId = SDK::UFunction::FindObject<SDK::UFunction>("Function ControllerData.ControllerData_C.SetId");

	const auto controllerArray = *Stack.GetParams<SDK::TArray<SDK::UObject*>>();
	const auto controllerArrayData = *(SDK::UObject ***)(&controllerArray);
	
	int i = 0;
	for (const auto controller : InputManager::GetInstance()->Controllers) {
		if (controller != 0) {
			auto controllerData = ((SDK::UGameplayStatics*)Context)->STATIC_SpawnObject(controllerDataClazz, (SDK::UObject*)Context);
			SDK::FString idString(std::to_wstring(controller).c_str());
			controllerData->ProcessEvent(controllerDataSetId, &idString);
			controllerArrayData[i++] = controllerData;
		}
		else {
			i++;
		}
	}

	//*((UE4::AActor**)((FOutParmRec*)Stack.OutParms)->PropAddr) = ModRef->InputProcesses[0];
}

void BlueprintProxyModule::Native_SetControllersImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	static const auto controllerDataGetId = SDK::UFunction::FindObject<SDK::UFunction>("Function ControllerData.ControllerData_C.GetId");

	const auto inputManager = InputManager::GetInstance();
	const auto controllerArray = *Stack.GetParams<SDK::TArray<SDK::UObject*>>();

	inputManager->Controllers.clear();

	for (int i = 0; i < controllerArray.Num(); i++) {
		const auto obj = controllerArray[i];
		
		if (((SDK::UKismetSystemLibrary *)Context)->STATIC_IsValid(obj)) {
			SDK::FString idString;
			obj->ProcessEvent(controllerDataGetId, &idString);

			const auto handle = std::stoul(idString.ToString());
		
			inputManager->Controllers.push_back(handle);
		}
		else {
			inputManager->Controllers.push_back(0);
		}
	}

}

struct SetVibrationParms {
	SDK::FString Id;
	bool IsOn;
};

void BlueprintProxyModule::Native_SetVibrationImpl(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	const auto parms = Stack.GetParams<SetVibrationParms>();
	const auto handle = std::stoul(parms->Id.ToString());
	const auto isOn = parms->IsOn;

	InputManager::GetInstance()->GetInput()->TriggerVibration(handle, isOn ? USHRT_MAX / 2 : 0, isOn ? USHRT_MAX / 2 : 0);
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
