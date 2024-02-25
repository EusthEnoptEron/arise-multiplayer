#include "ControlModeModule.h"
#include "Utils.h"
#include "Tracer.h"

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

	mod->AddBlueprintHook(
		"BP_BtlCharacterBase.BP_BtlCharacterBase_C.SetTurnTarget",
		OnSetTurnTargetArts
	);

	DerivedInputStateComponent__GetOwnerFn = FindFunction("Function Engine.ActorComponent.GetOwner");
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

// For some reason, the determination of IsTurnTarget does not work correctly for P1 (uses dirs from P2-P4) and P2-P4 (zero values?),
// so we do the calculation ourselves. The code is taken from the original implementation, with some adjustments.
void ControlModeModule::OnSetTurnTargetArts(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{

	const auto chara = (SDK::ABP_BtlCharacterBase_C*)Stack.Object;
	const auto operationMode = ((SDK::UBtlFunctionLibrary*)chara)->STATIC_GetOperationMode();
	struct ABP_BtlCharacterBase_C_SetTurnTarget_Params
	{
		float Range;                                                    // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
		float Speed;                                                    // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
		float Time;                                                     // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
	};

	if (operationMode == SDK::EOperationMode::OPERATION_MODE_MANUAL) {
		const auto mod = MultiplayerMod::GetInstance();
		const auto controller = mod->GetControllerOfCharacter((UE4::APawn*)chara);
		const auto playerIndex = mod->GetPlayerIndex(controller);

	
		if (playerIndex >= 0) {
			// ----------------------------
			// Deal with logic ourselves
			// ----------------------------

			const auto inputProcess = mod->GetInputProcess(playerIndex);

			if (inputProcess == nullptr) {
				Log::Info("No input process");
				return;
			};
			
			const auto params = Stack.GetParams<ABP_BtlCharacterBase_C_SetTurnTarget_Params>();

			chara->InterpTurnAngleRange = 360.0;
			chara->InterpTurnSpeed = params->Speed;
			chara->InterpTurnTime = params->Time;

			const auto moveToWorld = inputProcess->GetMoveToWorld();
			const float distance = sqrt(moveToWorld.X * moveToWorld.X + moveToWorld.Y * moveToWorld.Y + moveToWorld.Z * moveToWorld.Z);

			
			if (distance >= 0.8) {
				// Apply
				if (playerIndex >= 0) {
					const auto rotator = ((SDK::UKismetMathLibrary*)inputProcess)->STATIC_MakeRotFromX(moveToWorld);
					chara->K2_SetActorRotation(rotator, true);
				}

				return;
			}

			auto targetChara = chara->GetTargetCharacter(false);
			if (!((SDK::UKismetSystemLibrary*)chara)->STATIC_IsValid(targetChara)) {
				return;
			}

			const auto charaPos = chara->GetTransform().Translation;
			const auto targetPos = targetChara->GetTransform().Translation;


			const auto diff = SDK::FVector{
				targetPos.X - charaPos.X,
				targetPos.Y - charaPos.Y ,
				0.0,
			};

			const auto diffDistance = sqrt(diff.X * diff.X + diff.Y * diff.Y);

			if (diffDistance >= params->Range) {
				return;
			}
			
			// Apply
			const auto rotator = ((SDK::UKismetMathLibrary*)inputProcess)->STATIC_MakeRotFromX(diff);
			chara->K2_SetActorRotation(rotator, true);
			return;

			// --------------------------
		}
	}

	// Fallback to default processing
	processFn(Context, Stack, result);
}
