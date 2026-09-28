#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/OLCResourceTypes.h"
#include "OLCShipProgressionData.generated.h"

// ---------------------------------------------------------------------------
// FOLCEnergyCoreTier — one tier of the dropship energy core upgrade path.
// WP-123 Step 4: five tiers with capacities 50 / 100 / 200 / 500 / 1000.
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCEnergyCoreTier
{
	GENERATED_BODY()

	/** Tier number (1–5). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	int32 Tier = 1;

	/** Energy capacity granted by this tier. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	float Capacity = 50.0f;

	/**
	 * Cost to upgrade FROM the previous tier TO this tier.
	 * Empty for Tier 1 — it is the crash-start default and has no incoming cost.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	TArray<FOLCResourceAmount> UpgradeCost;
};

// ---------------------------------------------------------------------------
// FOLCHullStage — one stage of hull section expansion (1×2 → 2×2 → 3×3).
// WP-123 Step 5: each expansion acquires new module/weapon slots.
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCHullStage
{
	GENERATED_BODY()

	/** Stage number (1–3). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	int32 StageIndex = 1;

	/** Hull grid width in cells. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	int32 WidthCells = 1;

	/** Hull grid height in cells. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	int32 HeightCells = 2;

	/** Total module slots available on the hull at this stage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	int32 TotalSlots = 8;

	/** Weapon hardpoint slots available at this stage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	int32 WeaponSlots = 2;

	/**
	 * Cost to expand FROM the previous stage TO this one.
	 * Empty for Stage 1 — it is the crash state and has no incoming cost.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	TArray<FOLCResourceAmount> ExpansionCost;
};

// ---------------------------------------------------------------------------
// FOLCDriveStateConfig — left drive repair/upgrade progression values.
// WP-123 Step 3: Damaged (50% thrust) → Functional (100%) → Upgraded (125%).
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCDriveStateConfig
{
	GENERATED_BODY()

	/** Thrust output while the drive is Damaged. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	float DamagedThrustPercent = 50.0f;

	/** Thrust output once the drive is repaired to Functional. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	float FunctionalThrustPercent = 100.0f;

	/** Thrust output after the drive is upgraded. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	float UpgradedThrustPercent = 125.0f;

	/** Cost to repair the left drive from Damaged to Functional (20 CM + 15 Minerals). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	TArray<FOLCResourceAmount> RepairCost;

	/** Cost to upgrade the left drive from Functional to Upgraded (35 CM + 25 HullParts). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	TArray<FOLCResourceAmount> UpgradeCost;
};

// ---------------------------------------------------------------------------
// FOLCHullRepairConfig — hull integrity repair cost.
// WP-123 Step 2: repairs consume Construction Material + Hull Parts.
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCHullRepairConfig
{
	GENERATED_BODY()

	/**
	 * Resource cost to restore ONE percentage point (0.01) of hull integrity.
	 * Default: 4 ConstructionMaterial + 2 HullParts per percent.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	TArray<FOLCResourceAmount> CostPerPercent;
};

/**
 * UOLCShipProgressionData — single source of truth for all WP-123 dropship
 * repair and mothership progression values: energy core tiers, hull section
 * expansion stages, drive repair/upgrade costs, and hull repair costs.
 *
 * The constructor populates every table with the work-package numbers so a
 * freshly created asset instance is fully usable without manual population;
 * individual values remain editable on the asset (DA_ShipProgression).
 */
UCLASS()
class OURLASTCHANCE_API UOLCShipProgressionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UOLCShipProgressionData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Energy core upgrade path — 5 tiers, capacities 50/100/200/500/1000. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	TArray<FOLCEnergyCoreTier> EnergyCoreTiers;

	/** Hull section expansion stages — 1×2 → 2×2 → 3×3 with slot acquisition. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	TArray<FOLCHullStage> HullStages;

	/** Left drive repair/upgrade config (thrust per condition + costs). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	FOLCDriveStateConfig DriveRepair;

	/** Hull integrity repair cost. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	FOLCHullRepairConfig HullRepair;

	/** Hull integrity at crash landing (0.0–1.0). WP-123 Step 2: starts at 50%. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	float StartingHullIntegrity = 0.5f;

private:
	/** Populate all tables with the WP-123 default values (called from the constructor). */
	void PopulateDefaults();
};
