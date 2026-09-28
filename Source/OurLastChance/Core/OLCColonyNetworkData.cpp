#include "Core/OLCColonyNetworkData.h"

// ---------------------------------------------------------------------------
// Built-in defaults — the exact WP-124 numbers. These are authoritative when
// no DA_ColonyNetwork asset is registered (or its arrays are empty), so tests
// and gameplay behave identically with or without the persistent asset.
// ---------------------------------------------------------------------------

TArray<FOLCColonyRoleConfig> UOLCColonyNetworkData::GetDefaultRoleConfigs()
{
	TArray<FOLCColonyRoleConfig> Configs;
	Configs.Reserve(5);

	// Mining: +25% resource extraction from local mines; mineral-rich planets.
	{
		FOLCColonyRoleConfig C;
		C.Role = EOLCColonyRole::Mining;
		C.DisplayName = FText::FromString(TEXT("Mining"));
		C.ProductionBonusPercent = 25.0f;
		C.AffectedResources.Add(EOLCResourceType::Minerals);
		C.AffectedResources.Add(EOLCResourceType::ConstructionMaterial);
		C.EligibleBiomes.Add(EOLCBiomeType::Rocky);
		C.EligibleBiomes.Add(EOLCBiomeType::Dusty);
		C.EligibleBiomes.Add(EOLCBiomeType::Desert);
		Configs.Add(C);
	}

	// Fuel: +25% fuel production; gas giant or volcanic planet (no gas-giant
	// biome exists in EOLCBiomeType, so oil/biofuel biomes stand in).
	{
		FOLCColonyRoleConfig C;
		C.Role = EOLCColonyRole::Fuel;
		C.DisplayName = FText::FromString(TEXT("Fuel"));
		C.ProductionBonusPercent = 25.0f;
		C.AffectedResources.Add(EOLCResourceType::Fuel);
		C.EligibleBiomes.Add(EOLCBiomeType::Desert);
		C.EligibleBiomes.Add(EOLCBiomeType::Swamp);
		C.EligibleBiomes.Add(EOLCBiomeType::Water);
		Configs.Add(C);
	}

	// Military: +20% unit training speed, +15% defense stats; rocky/ice planet.
	{
		FOLCColonyRoleConfig C;
		C.Role = EOLCColonyRole::Military;
		C.DisplayName = FText::FromString(TEXT("Military"));
		C.UnitTrainingSpeedBonusPercent = 20.0f;
		C.DefenseStatBonusPercent = 15.0f;
		C.EligibleBiomes.Add(EOLCBiomeType::Rocky);
		C.EligibleBiomes.Add(EOLCBiomeType::Ice);
		Configs.Add(C);
	}

	// Research: +15% research speed; planet with alien artifact presence
	// (biomes whose hostile areas are documented alien structures).
	{
		FOLCColonyRoleConfig C;
		C.Role = EOLCColonyRole::Research;
		C.DisplayName = FText::FromString(TEXT("Research"));
		C.ResearchSpeedBonusPercent = 15.0f;
		C.EligibleBiomes.Add(EOLCBiomeType::Swamp);
		C.EligibleBiomes.Add(EOLCBiomeType::Ice);
		C.EligibleBiomes.Add(EOLCBiomeType::Water);
		Configs.Add(C);
	}

	// Agricultural: +25% survival resource production, +10% crew capacity;
	// water/jungle planet.
	{
		FOLCColonyRoleConfig C;
		C.Role = EOLCColonyRole::Agricultural;
		C.DisplayName = FText::FromString(TEXT("Agricultural"));
		C.ProductionBonusPercent = 25.0f;
		C.AffectedResources.Add(EOLCResourceType::Survival);
		C.CrewCapacityBonusPercent = 10.0f;
		C.EligibleBiomes.Add(EOLCBiomeType::Water);
		C.EligibleBiomes.Add(EOLCBiomeType::Jungle);
		Configs.Add(C);
	}

	return Configs;
}

TArray<FOLCTransportMethodConfig> UOLCColonyNetworkData::GetDefaultTransportConfigs()
{
	TArray<FOLCTransportMethodConfig> Configs;
	Configs.Reserve(3);

	// Tanker: slow (10 s transit), high capacity (500 units/resource/trip),
	// 5 Energy per trip. Best for bulk transfers between adjacent systems.
	{
		FOLCTransportMethodConfig C;
		C.Method = EOLCTransportMethod::Tanker;
		C.DisplayName = FText::FromString(TEXT("Tanker"));
		C.TransitSeconds = 10.0f;
		C.CapacityPerTrip = 500.0f;
		C.EnergyCostPerTrip = 5.0f;
		Configs.Add(C);
	}

	// Hauler: medium speed (5 s transit), medium capacity (200 units),
	// 3 Energy per trip. Best for regular resupply runs.
	{
		FOLCTransportMethodConfig C;
		C.Method = EOLCTransportMethod::Hauler;
		C.DisplayName = FText::FromString(TEXT("Hauler"));
		C.TransitSeconds = 5.0f;
		C.CapacityPerTrip = 200.0f;
		C.EnergyCostPerTrip = 3.0f;
		Configs.Add(C);
	}

	// Quantum Gate: instant transfer, limited capacity (50 units per trip),
	// no energy cost. Requires operational gates on both colonies.
	{
		FOLCTransportMethodConfig C;
		C.Method = EOLCTransportMethod::QuantumGate;
		C.DisplayName = FText::FromString(TEXT("Quantum Gate"));
		C.TransitSeconds = 0.0f;
		C.CapacityPerTrip = 50.0f;
		C.EnergyCostPerTrip = 0.0f;
		Configs.Add(C);
	}

	return Configs;
}
