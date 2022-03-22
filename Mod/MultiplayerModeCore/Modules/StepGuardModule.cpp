#include "StepGuardModule.h"

MultiplayerMod* StepGuardModule::ModRef;
FNativeFuncPtr StepGuardModule::IsAutoStepable;
FNativeFuncPtr StepGuardModule::IsAutoGuardable;

void StepGuardModule::Initialize(MultiplayerMod* mod)
{
	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlSemiautoComponent.IsAutoStepable")->GetFunction()),
		&IsAutoStepableHook, &IsAutoStepable, "IsAutoStepable");

	MinHook::Add((DWORD_PTR)
		(UE4::UObject::FindObject<UE4::UFunction>("Function Arise.BtlSemiautoComponent.IsAutoGuardable")->GetFunction()),
		&IsAutoGuardableHook, &IsAutoGuardable, "IsAutoGuardable");

	mod->AddBlueprintHook(
		"BP_BtlCharacterBase.BP_BtlCharacterBase_C.JustStepProcess",
		OnStepOrGuard
	);

	mod->AddBlueprintHook(
		"BP_BtlCharacterBase.BP_BtlCharacterBase_C.JustGuardProcess",
		OnStepOrGuard
	);

	ModRef = mod;
}

void StepGuardModule::IsAutoStepableHook(UE4::UObject* Context, UE4::FFrame& Stack, bool* ret)
{
	IsAutoStepable(Context, Stack, ret);

	if (*ret) {
		auto semiautoComponent = (SDK::UBtlSemiautoComponent*)Context;
		auto ownerActor = semiautoComponent->GetOwner();
		auto mod = ((MultiplayerMod*)Mod::ModRef);

		if (mod->IsControlledCharacter((UE4::AActor*)ownerActor, true)) {
			auto mainController = (SDK::APlayerController*)mod->Controllers[0];
			if (mainController != nullptr) {
				auto character = (SDK::ABtlCharacterBase*)((SDK::APlayerController*)mod->Controllers[0])->K2_GetPawn();
				*ret = character->GetSemiautoComponent()->IsAutoStepable();
				return;
			}

			*ret = false;
		}
	}
}

void StepGuardModule::IsAutoGuardableHook(UE4::UObject* Context, UE4::FFrame& Stack, bool* ret)
{
	IsAutoGuardable(Context, Stack, ret);

	if (*ret) {
		auto semiautoComponent = (SDK::UBtlSemiautoComponent*)Context;
		auto ownerActor = semiautoComponent->GetOwner();
		auto mod = ((MultiplayerMod*)Mod::ModRef);

		if (mod->IsControlledCharacter((UE4::AActor*)ownerActor, true)) {
			auto mainController = (SDK::APlayerController*)mod->Controllers[0];
			if (mainController != nullptr) {
				auto character = (SDK::ABtlCharacterBase*)((SDK::APlayerController*)mod->Controllers[0])->K2_GetPawn();
				*ret = character->GetSemiautoComponent()->IsAutoStepable();
				return;
			}

			*ret = false;
		}
	}
}

void StepGuardModule::OnStepOrGuard(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	// Called on enemy, with our character as argument
	// void JustStepProcess(class ABtlCharacterBase* DmgActor, bool& JustStep);
	// void JustGuardProcess(class ABtlCharacterBase* DmgActor, bool& JustGuard);


	auto params = *Stack.GetParams<JustParams>();

	int index = ModRef->GetPlayerIndex(ModRef->GetControllerOfCharacter(params.DmgActor));

	if (index > 0) {
		int _player = ModRef->CurrentPlayer;
		ModRef->CurrentPlayer = index;

		Log::Info("Set player: %d", index);

		processFn(Context, Stack, result);

		ModRef->CurrentPlayer = _player;

		return;
	}

	processFn(Context, Stack, result);
}
