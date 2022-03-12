#include "ControlModeModule.h"

UE4::UFunction* ControlModeModule::DerivedInputStateComponent__GetOwnerFn;
void ControlModeModule::Initialize(MultiplayerMod* mod)
{
	// -------------------------------------------------------------------------------------------------
	// Instead of using SetPlayerOperation, simply override the accessors (has less bad side-effects)
	// -------------------------------------------------------------------------------------------------
	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlCharacterBase.GetPlayerOperation")->GetFunction()),
		&GetPlayerOperationHook, &GetPlayerOperation, "GetPlayerOperation");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlCharacterBase.IsAutoOperation")->GetFunction()),
		&IsAutoOperationHook, &IsAutoOperation, "IsAutoOperation");


	mod->AddBlueprintHook(
		"BP_DerivedInputStateComponent.BP_DerivedInputStateComponent_C.OnOperationUnitChanged",
		OnOperationUnitChanged
	);

	DerivedInputStateComponent__GetOwnerFn = UE4::UObject::FindObject<UE4::UFunction>("Function Engine.ActorComponent.GetOwner");
}

//Function Arise.BtlCharacterBase.GetPlayerOperation
FNativeFuncPtr ControlModeModule::GetPlayerOperation;
void ControlModeModule::GetPlayerOperationHook(UE4::UObject* Context, UE4::FFrame& Stack, bool* result) {
	auto ctx = (SDK::ABtlCharacterBase*)Context;
	auto controller = ctx->Controller;
	auto mod = ((MultiplayerMod*)(Mod::ModRef));

	GetPlayerOperation(Context, Stack, result);

	if (controller != nullptr) {
		int charaIndex = mod->GetPlayerIndex((UE4::APlayerController*)controller);
		if (mod->CurrentPlayer >= 0 && mod->CurrentPlayer != charaIndex) {
			Log::Info("Forcing %s (%d) to NOT player (current player = %d)", controller->GetName().c_str(), charaIndex, mod->CurrentPlayer);
			*result = false;
		}
		else if (charaIndex > 0) {
			*result = true;

		}
	}
}


//Function Arise.BtlCharacterBase.IsAutoOperation
FNativeFuncPtr ControlModeModule::IsAutoOperation;
void ControlModeModule::IsAutoOperationHook(UE4::UObject* Context, UE4::FFrame& Stack, bool* result) {
	auto ctx = (SDK::ABtlCharacterBase*)Context;
	auto controller = ctx->Controller;
	auto mod = ((MultiplayerMod*)(Mod::ModRef));

	IsAutoOperation(Context, Stack, result);

	if (controller != nullptr) {
		int charaIndex = mod->GetPlayerIndex((UE4::APlayerController*)controller);
		if (mod->CurrentPlayer >= 0 && mod->CurrentPlayer != charaIndex) {
			Log::Info("Forcing %s to AUTO", controller->GetName().c_str());

			*result = true;
		}
		else if (charaIndex > 0) {
			*result = false;
		}
	}
}

void ControlModeModule::OnOperationUnitChanged(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	// Ignore if this unit is controlled by ourselves
	UE4::AActor* owner; // InputProcess
	Stack.Object->ProcessEvent(DerivedInputStateComponent__GetOwnerFn, &owner);

	int index = MultiplayerMod::GetInstance()->GetPlayerIndexFromInputProcessor(owner);
	if (index > 0) {
		// Ignore!
		Log::Info("Ignore operationunitchanged event");
		return;
	}

	processFn(Context, Stack, result);
}
