#include "Core/OLCEndgameEncounterData.h"

UOLCEndgameEncounterData::UOLCEndgameEncounterData()
{
	// Seed the preparation checklist with sensible defaults (CDO fallback).
	PrepChecklist.Add(FText::FromString(TEXT("Equip best gear on all units (Layer 3 equipment)")));
	PrepChecklist.Add(FText::FromString(TEXT("Assign colony resources to expedition: fuel, ammo, repair materials")));
	PrepChecklist.Add(FText::FromString(TEXT("Choose assault strategy: direct attack (fast, risky) or methodical advance (slow, safe)")));
	PrepChecklist.Add(FText::FromString(TEXT("Verify Quantum Gate network has at least 3 active gates for supply line")));

	TriggerDescription = FText::FromString(
		TEXT("Triggers when the player reaches the Center Galaxy cluster with all TIR 5 research complete, "
			"ship in Flagship state, full squad assembled, and at least 3 active Quantum Gates."));
}
