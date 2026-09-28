#include "SpaceTravel/OLCSpaceEventData.h"

UOLCSpaceEventData::UOLCSpaceEventData()
{
	// Five default space events (WP-122 step 2). These constructor defaults seed the
	// class CDO, which UOLCTravelSubsystem uses as its code-side fallback when no
	// authored DataAsset instance is assigned.

	// Pirate encounter — combat handoff only; WP-113 owns the fight itself.
	{
		FOLCSpaceEventDefinition Def;
		Def.EventType = EOLCSpaceEventType::PirateEncounter;
		Def.Weight = 1.0f;
		Def.MinTIR = 1;
		EventTable.Add(Def);
	}

	// Derelict ship — explore for salvage: small resource gain.
	{
		FOLCSpaceEventDefinition Def;
		Def.EventType = EOLCSpaceEventType::DerelictShip;
		Def.Weight = 1.2f;
		Def.MinTIR = 1;
		Def.RewardPayload.Add(FOLCResourceAmount(EOLCResourceType::Minerals, 5.0f, 0.0f));
		Def.RewardPayload.Add(FOLCResourceAmount(EOLCResourceType::Fuel, 3.0f, 0.0f));
		EventTable.Add(Def);
	}

	// Anomaly — mystery: half the time a reward, half the time a hull hazard.
	{
		FOLCSpaceEventDefinition Def;
		Def.EventType = EOLCSpaceEventType::Anomaly;
		Def.Weight = 0.8f;
		Def.MinTIR = 2;
		Def.RewardPayload.Add(FOLCResourceAmount(EOLCResourceType::DarkMatterCrystals, 1.0f, 0.0f));
		Def.HazardHullDamagePercent = 10.0f;
		EventTable.Add(Def);
	}

	// Merchant — trade hook (transit trade screen is presented by the UI, later step).
	{
		FOLCSpaceEventDefinition Def;
		Def.EventType = EOLCSpaceEventType::Merchant;
		Def.Weight = 1.0f;
		Def.MinTIR = 1;
		EventTable.Add(Def);
	}

	// Meteor shower — hull hazard plus the AsteroidEvasion minigame via the manager.
	{
		FOLCSpaceEventDefinition Def;
		Def.EventType = EOLCSpaceEventType::MeteorShower;
		Def.Weight = 1.5f;
		Def.MinTIR = 1;
		Def.HazardHullDamagePercent = 15.0f;
		Def.bHasMinigame = true;
		Def.MinigameType = EOLCMinigameType::AsteroidEvasion;
		Def.MinigameDurationSeconds = 15.0f;
		EventTable.Add(Def);
	}
}

const FOLCSpaceEventDefinition* UOLCSpaceEventData::FindEvent(EOLCSpaceEventType InType) const
{
	for (const FOLCSpaceEventDefinition& Def : EventTable)
	{
		if (Def.EventType == InType)
		{
			return &Def;
		}
	}
	return nullptr;
}
