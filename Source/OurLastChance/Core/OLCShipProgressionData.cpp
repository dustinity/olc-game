#include "Core/OLCShipProgressionData.h"

UOLCShipProgressionData::UOLCShipProgressionData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PopulateDefaults();
}

void UOLCShipProgressionData::PopulateDefaults()
{
	// -----------------------------------------------------------------------
	// Energy core tiers (WP-123 Step 4).
	// Five tiers: 50 / 100 / 200 / 500 / 1000 energy capacity.
	// UpgradeCost is the cost to upgrade FROM the previous tier TO this tier;
	// Tier 1 is the crash-start default and carries no incoming cost.
	// -----------------------------------------------------------------------
	EnergyCoreTiers.Reset();

	{
		FOLCEnergyCoreTier& T = EnergyCoreTiers.AddDefaulted_GetRef();
		T.Tier = 1;
		T.Capacity = 50.0f;
	}
	{
		FOLCEnergyCoreTier& T = EnergyCoreTiers.AddDefaulted_GetRef();
		T.Tier = 2;
		T.Capacity = 100.0f;
		T.UpgradeCost.Emplace(EOLCResourceType::ConstructionMaterial, 50.0f, 50.0f);
		T.UpgradeCost.Emplace(EOLCResourceType::Minerals, 35.0f, 35.0f);
		T.UpgradeCost.Emplace(EOLCResourceType::Fuel, 15.0f, 15.0f);
	}
	{
		FOLCEnergyCoreTier& T = EnergyCoreTiers.AddDefaulted_GetRef();
		T.Tier = 3;
		T.Capacity = 200.0f;
		T.UpgradeCost.Emplace(EOLCResourceType::ConstructionMaterial, 80.0f, 80.0f);
		T.UpgradeCost.Emplace(EOLCResourceType::Minerals, 60.0f, 60.0f);
		T.UpgradeCost.Emplace(EOLCResourceType::Fuel, 30.0f, 30.0f);
	}
	{
		FOLCEnergyCoreTier& T = EnergyCoreTiers.AddDefaulted_GetRef();
		T.Tier = 4;
		T.Capacity = 500.0f;
		T.UpgradeCost.Emplace(EOLCResourceType::ConstructionMaterial, 150.0f, 150.0f);
		T.UpgradeCost.Emplace(EOLCResourceType::Minerals, 100.0f, 100.0f);
		T.UpgradeCost.Emplace(EOLCResourceType::Fuel, 60.0f, 60.0f);
	}
	{
		FOLCEnergyCoreTier& T = EnergyCoreTiers.AddDefaulted_GetRef();
		T.Tier = 5;
		T.Capacity = 1000.0f;
		T.UpgradeCost.Emplace(EOLCResourceType::ConstructionMaterial, 300.0f, 300.0f);
		T.UpgradeCost.Emplace(EOLCResourceType::Minerals, 200.0f, 200.0f);
		T.UpgradeCost.Emplace(EOLCResourceType::Fuel, 120.0f, 120.0f);
		T.UpgradeCost.Emplace(EOLCResourceType::Survival, 50.0f, 50.0f);
	}

	// -----------------------------------------------------------------------
	// Hull section expansion stages (WP-123 Step 5).
	// 1×2 → 2×2 → 3×3 with new slot acquisition: 8/16/36 slots, 2/4/6 weapon.
	// ExpansionCost is the cost to expand FROM the previous stage TO this one;
	// Stage 1 is the crash state and carries no incoming cost.
	// -----------------------------------------------------------------------
	HullStages.Reset();

	{
		FOLCHullStage& S = HullStages.AddDefaulted_GetRef();
		S.StageIndex = 1;
		S.WidthCells = 1;
		S.HeightCells = 2;
		S.TotalSlots = 8;
		S.WeaponSlots = 2;
	}
	{
		FOLCHullStage& S = HullStages.AddDefaulted_GetRef();
		S.StageIndex = 2;
		S.WidthCells = 2;
		S.HeightCells = 2;
		S.TotalSlots = 16;
		S.WeaponSlots = 4;
		S.ExpansionCost.Emplace(EOLCResourceType::ConstructionMaterial, 100.0f, 100.0f);
		S.ExpansionCost.Emplace(EOLCResourceType::Minerals, 75.0f, 75.0f);
		S.ExpansionCost.Emplace(EOLCResourceType::Fuel, 40.0f, 40.0f);
	}
	{
		FOLCHullStage& S = HullStages.AddDefaulted_GetRef();
		S.StageIndex = 3;
		S.WidthCells = 3;
		S.HeightCells = 3;
		S.TotalSlots = 36;
		S.WeaponSlots = 6;
		S.ExpansionCost.Emplace(EOLCResourceType::ConstructionMaterial, 250.0f, 250.0f);
		S.ExpansionCost.Emplace(EOLCResourceType::Minerals, 200.0f, 200.0f);
		S.ExpansionCost.Emplace(EOLCResourceType::Fuel, 100.0f, 100.0f);
		S.ExpansionCost.Emplace(EOLCResourceType::Survival, 100.0f, 100.0f);
	}

	// -----------------------------------------------------------------------
	// Drive repair/upgrade config (WP-123 Step 3).
	// Damaged: 50% thrust → Functional: 100% → Upgraded: 125%.
	// Repair cost matches the existing ION DRIVE T1 RepairCost in
	// UOLCUIDataSubsystem::PopulateShipModules (20 CM + 15 Minerals).
	// -----------------------------------------------------------------------
	DriveRepair.DamagedThrustPercent = 50.0f;
	DriveRepair.FunctionalThrustPercent = 100.0f;
	DriveRepair.UpgradedThrustPercent = 125.0f;

	DriveRepair.RepairCost.Reset();
	DriveRepair.RepairCost.Emplace(EOLCResourceType::ConstructionMaterial, 20.0f, 20.0f);
	DriveRepair.RepairCost.Emplace(EOLCResourceType::Minerals, 15.0f, 15.0f);

	DriveRepair.UpgradeCost.Reset();
	DriveRepair.UpgradeCost.Emplace(EOLCResourceType::ConstructionMaterial, 35.0f, 35.0f);
	DriveRepair.UpgradeCost.Emplace(EOLCResourceType::HullParts, 25.0f, 25.0f);

	// -----------------------------------------------------------------------
	// Hull repair cost (WP-123 Step 2): 4 CM + 2 HullParts per 1% integrity.
	// -----------------------------------------------------------------------
	HullRepair.CostPerPercent.Reset();
	HullRepair.CostPerPercent.Emplace(EOLCResourceType::ConstructionMaterial, 4.0f, 4.0f);
	HullRepair.CostPerPercent.Emplace(EOLCResourceType::HullParts, 2.0f, 2.0f);

	// Crash landing leaves the hull at 50% (WP-123 Step 2).
	StartingHullIntegrity = 0.5f;
}
