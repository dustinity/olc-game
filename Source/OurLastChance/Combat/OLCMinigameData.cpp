#include "Combat/OLCMinigameData.h"

UOLCMinigameData::UOLCMinigameData()
{
	// Two new in-transit minigames (WP-122 step 6). These constructor defaults seed the
	// class CDO, which callers use as their code-side fallback when no authored DataAsset
	// instance is available. Both map onto the manager's token-sequence engine:
	// the UI steps present the actual spatial/timing visuals and submit tokens.

	// Cargo bay management — spatial slot matching: place each cargo crate into its
	// marked slot in order (SLOT_A..SLOT_F), 20 seconds on the clock.
	{
		FOLCMinigameDefinition Def;
		Def.MinigameType = EOLCMinigameType::CargoBayManagement;
		Def.DisplayName = TEXT("Cargo Bay Management");
		Def.InputSequence = { TEXT("SLOT_A"), TEXT("SLOT_B"), TEXT("SLOT_C"),
			TEXT("SLOT_D"), TEXT("SLOT_E"), TEXT("SLOT_F") };
		Def.DurationSeconds = 20.0f;
		MinigameTable.Add(Def);
	}

	// Reactor stabilization — keep-the-reactor-in-zone timing: submit a STABILIZE pulse
	// each time the reactor needle sits inside the safe zone, 8 pulses in 15 seconds.
	{
		FOLCMinigameDefinition Def;
		Def.MinigameType = EOLCMinigameType::ReactorStabilization;
		Def.DisplayName = TEXT("Reactor Stabilization");
		Def.InputSequence = { TEXT("STABILIZE"), TEXT("STABILIZE"), TEXT("STABILIZE"),
			TEXT("STABILIZE"), TEXT("STABILIZE"), TEXT("STABILIZE"), TEXT("STABILIZE"), TEXT("STABILIZE") };
		Def.DurationSeconds = 15.0f;
		MinigameTable.Add(Def);
	}
}

const FOLCMinigameDefinition* UOLCMinigameData::FindData(EOLCMinigameType InType) const
{
	for (const FOLCMinigameDefinition& Def : MinigameTable)
	{
		if (Def.MinigameType == InType)
		{
			return &Def;
		}
	}
	return nullptr;
}
