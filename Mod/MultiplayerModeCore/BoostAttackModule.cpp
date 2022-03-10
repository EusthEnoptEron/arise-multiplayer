#include "BoostAttackModule.h"

MultiplayerMod* BoostAttackModule::ModRef;
int BoostAttackModule::LastStrikeInitiator = -1;

void BoostAttackModule::Initialize(MultiplayerMod* mod)
{
	mod->AddBlueprintHook(
		"BP_BTL_PCInputProcess.BP_BTL_PCInputProcess_C.RunStrike",
		OnRunStrike
	);

	mod->AddBlueprintHook(
		"BP_BTL_BoostLibrary.BP_BTL_BoostLibrary_C.RunBoostAttack",
		OnRunBoostAttack
	);

	mod->AddBlueprintHook(
		"BP_BtlCharacterBase.BP_BtlCharacterBase_C.OnAttackBeginEvent",
		OnAttackBeginEvent
	);

	ModRef = mod;
}

void BoostAttackModule::OnRunStrike(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	// Someone clicked the strike button, oversteer the static player controller methods to consider the current user the main player
	// At this step, we don't yet know if a boost attack can be executed.
	int playerBefore = ModRef->CurrentPlayer;
	ModRef->CurrentPlayer = ModRef->GetPlayerIndexFromInputProcessor((UE4::AActor*)Stack.Object);
	Log::Info("RunStrike %d => %d", playerBefore, ModRef->CurrentPlayer);

	processFn(Context, Stack, result);

	ModRef->CurrentPlayer = playerBefore;
}

void BoostAttackModule::OnRunBoostAttack(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{
	Log::Info("RunBoostAttack (%d)", ModRef->CurrentPlayer);
	// Called inside of "RunStrike" -- we now know that a boost attack will occur
	// We now change the first player character to the initiator of the boost attack in order to satisfy the native TOARISE code I cannot
	// be bothered to locate in the debugger...

	LastStrikeInitiator = ModRef->CurrentPlayer;
	if (ModRef->RestrictBoostAttacksToP1 || ModRef->RestrictBoostAttacksToCpuAndSelf) {
		// Only apply this if we're actually in a multiplayer battle (as opposed to tutorial, etc.)
		if (ModRef->IsMultiplayerBattle()) {
			auto executor = Stack.GetParams<UE4::APawn*>();
			auto controller = ModRef->GetControllerOfCharacter(*executor);
			auto index = ModRef->GetPlayerIndex(controller);

			if (ModRef->RestrictBoostAttacksToP1 && ModRef->CurrentPlayer != 0
				|| ModRef->RestrictBoostAttacksToCpuAndSelf && (index != ModRef->CurrentPlayer && index >= 0)) {
				// Ignore boost attack
				Log::Info("Ignoring boost attack because of restriction.");
				ModRef->CurrentPlayer = -1;

				return;
			}
		}
	}

	if (ModRef->CurrentPlayer > 0) {
		int playerIndex = ModRef->CurrentPlayer;
		ModRef->CurrentPlayer = 0;
		ModRef->ChangeFirstPlayerTemporarily(playerIndex);
	}

	// Call original function
	processFn(Context, Stack, result);
}

void BoostAttackModule::OnAttackBeginEvent(UE4::UObject* Context, UE4::FFrame& Stack, void* result, FNativeFuncPtr processFn)
{

	// Event handler for any attack -- also boost attacks.
		// At this point, the native side has finished setting up the boost attack and we can revert our changes.

	struct Params {
		SDK::ABtlCharacterBase* SelfCharacter;                                            // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
		SDK::FBtlArtsData                                NowArts;                                                  // (BlueprintVisible, BlueprintReadOnly, Parm)
		SDK::EBattleActionState                                 PreState;
	};
	auto params = Stack.GetParams<Params>();

	auto type = params->NowArts.Type;
	switch (type) {
	case SDK::EBtlArtsType::STR_ATK:
	case SDK::EBtlArtsType::STR_ATK_AIR:
	case SDK::EBtlArtsType::STR_ATK_SUB:
	case SDK::EBtlArtsType::STR_ATK_AIR_SUB:
		Log::Info("Doing a boost attack (%d => %d)", ModRef->CurrentPlayer, LastStrikeInitiator);
		int playerBefore = ModRef->CurrentPlayer;
		ModRef->CurrentPlayer = LastStrikeInitiator;

		processFn(Context, Stack, result);

		ModRef->CurrentPlayer = playerBefore;
		LastStrikeInitiator = -1;

		Log::Info("Restore first player");
		ModRef->RestoreFirstPlayer();
		return;
		break;
	}

	processFn(Context, Stack, result);
}
