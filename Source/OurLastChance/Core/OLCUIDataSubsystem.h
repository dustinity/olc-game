#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/OLCShipModuleData.h"
#include "Core/OLCResourceTypes.h"
#include "Core/OLCBuildingData.h"
#include "OLCUIDataSubsystem.generated.h"

class AActor;

// ---------------------------------------------------------------------------
// Registered building entry for production tracking (WP-104)
// ---------------------------------------------------------------------------
USTRUCT()
struct FRegisteredBuildingEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<AActor> Actor;

	UPROPERTY()
	TObjectPtr<UOLCBuildingData> BuildingData;

	FRegisteredBuildingEntry() {}

	FRegisteredBuildingEntry(AActor* InActor, UOLCBuildingData* InData)
		: Actor(InActor), BuildingData(InData) {}
};

/**
 * GameInstanceSubsystem that provides fake/test view data for all UI screens.
 * Later bridges to real gameplay systems.
 */
UCLASS()
class OURLASTCHANCE_API UOLCUIDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// -----------------------------------------------------------------------
	// Resource data
	// -----------------------------------------------------------------------
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	const TArray<FOLCResourceCounterViewData>& GetResourceCounters() const { return ResourceCounters; }

	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	TArray<FOLCResourceAmount> GetRawResources() const;

	// -----------------------------------------------------------------------
	// Mission objectives (fake / tutorial)
	// -----------------------------------------------------------------------
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	const TArray<FOLCMissionObjectiveViewData>& GetMissionObjectives() const { return MissionObjectives; }

	/** Set all mission objectives — replaces the current list. Used for tutorial flow. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void SetMissionObjectives(const TArray<FOLCMissionObjectiveViewData>& InObjectives);

	/** Advance a specific objective to completed state by index. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void CompleteObjective(int32 Index);

	/** Activate an objective (move from Idle to Active) by index. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void ActivateObjective(int32 Index);

	/** Increment progress on an objective by a delta amount. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void AdvanceObjectiveProgress(int32 Index, float Delta);

	// -----------------------------------------------------------------------
	// Construction data (fake)
	// -----------------------------------------------------------------------
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	const TArray<FOLCBuildCardViewData>& GetBuildCards() const { return BuildCards; }

	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	TArray<FOLCBuildCardViewData> GetBuildCardsForCategory(EOLCConstructionCategory InCategory) const;

	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	const TArray<EOLCConstructionCategory>& GetConstructionCategories() const { return ConstructionCategories; }

	// -----------------------------------------------------------------------
	// Badge data (fake - planet biome/hazard info)
	// -----------------------------------------------------------------------
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	TArray<FOLCBadgeViewData> GetCurrentPlanetBadges() const;

	// -----------------------------------------------------------------------
	// Minimap markers (fake)
	// -----------------------------------------------------------------------
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	const TArray<FOLCMinimapMarkerViewData>& GetMinimapMarkers() const { return MinimapMarkers; }

	// -----------------------------------------------------------------------
	// Simulation speed
	// -----------------------------------------------------------------------
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	EOLCSimulationSpeed GetCurrentSimulationSpeed() const { return CurrentSimulationSpeed; }

	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void SetCurrentSimulationSpeed(EOLCSimulationSpeed Speed) { CurrentSimulationSpeed = Speed; }

	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void CycleSimulationSpeed();

	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void ResetResourcesToZero();

	/** Add a quantity to a resource counter. Used by building production ticks. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void AddResource(EOLCResourceType ResourceType, float Amount);

	/** Check if we have enough resources for a build cost. Returns true if affordable. */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	bool CanAffordBuild(const TArray<FOLCResourceAmount>& BuildCost) const;

	/** Deduct resources for a confirmed building placement. Returns true on success. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	bool ConsumeResourcesForBuild(const TArray<FOLCResourceAmount>& BuildCost);

	// -----------------------------------------------------------------------
	// Unit capacity tracking
	// -----------------------------------------------------------------------
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	int32 GetCurrentUnitCount() const { return CurrentUnitCount; }

	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	int32 GetMaxUnitCapacity() const { return MaxUnitCapacity; }

	/** Add a unit to the roster (checks capacity). Returns true if successful. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	bool AddUnit();

	/** Remove a unit from the roster. Returns true if successful. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	bool RemoveUnit();

	// -----------------------------------------------------------------------
	// Ship state (S13/S14 — wired to HUD and Solar System)
	// -----------------------------------------------------------------------

	/** Drive status: Installed, Damaged (50% thrust), or Offline (cannot travel). */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	EOLCModuleState GetDriveStatus() const { return DriveStatus; }

	/** Set drive status — called by DropshipRepair / ShipModuleManagement screens. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void SetDriveStatus(EOLCModuleState InStatus);

	/** Current drive tier (1–5). Higher tier = longer travel range. */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	int32 GetDriveTier() const { return DriveTier; }

	/** Set drive tier — updated when a new drive module is installed. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void SetDriveTier(int32 InTier);

	/** Shield status: Installed, Damaged (50% integrity), or Offline (no shield). */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	EOLCModuleState GetShieldStatus() const { return ShieldStatus; }

	/** Set shield status — called by DropshipRepair / ShipModuleManagement screens. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void SetShieldStatus(EOLCModuleState InStatus);

	/** Current shield integrity (0.0–1.0). */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	float GetShieldIntegrity() const { return ShieldIntegrityPercent; }

	/** Per-resource-type storage bonus from installed storage modules. */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	int32 GetStorageBonusPerResource() const { return StorageBonusPerResourceType; }

	/** Add storage bonus — called when a storage module is attached in ShipModuleManagement. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void AddStorageBonus(int32 Amount);

	/** Calculate maximum fuel cost the current drive can handle based on tier and state. */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	int32 GetMaxReachableFuelCost() const;

	// -----------------------------------------------------------------------
	// Actions (fake)
	// -----------------------------------------------------------------------
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	TArray<FOLCActionViewData> GetQuickBuildActions() const;

	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	TArray<FOLCProgressViewData> GetActiveProgresses() const;

	// -----------------------------------------------------------------------
	// Resource Production System (WP-104)
	// -----------------------------------------------------------------------

	/** Start the production tick timer. Call after buildings are placed. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void StartProductionTick();

	/** Stop the production tick timer. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void StopProductionTick();

	/** Set the production tick interval in seconds (default 5.0). */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void SetProductionTickInterval(float Interval);

	/** Get current production tick interval. */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	float GetProductionTickInterval() const { return ProductionTickInterval; }

	/** Register a building for production tracking. Called when building is placed. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void RegisterBuilding(AActor* BuildingActor, UOLCBuildingData* BuildingData);

	/** Unregister a building (destroyed or removed). */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void UnregisterBuilding(AActor* BuildingActor);

	/** Calculate power grid balance: negative=producer, positive=consumer. */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	float CalculateGridBalance() const;

	/** Get total power production (sum of negative PowerConsumption). */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	float GetTotalPowerProduction() const;

	/** Get total power consumption (sum of positive PowerConsumption). */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	float GetTotalPowerConsumption() const;

	/** Check if there is a power deficit (consumption > production). */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	bool HasPowerDeficit() const { return bPowerDeficit; }

	/** Get current power deficit state. */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	bool IsPowerDeficit() const { return bPowerDeficit; }

	/** Get the active biome type from terrain (or default Desert). */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	EOLCBiomeType GetActiveBiome() const { return ActiveBiome; }

	/** Set the active biome type (called by terrain actor or game mode). */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void SetActiveBiome(EOLCBiomeType Biome);

	/** Get max capacity for a resource type (base + locker/storage bonuses). */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	float GetMaxCapacity(EOLCResourceType ResourceType) const;

	/** Get storage pressure state for a resource type. */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	EOLCStoragePressure GetStoragePressure(EOLCResourceType ResourceType) const;

	/** Get per-resource production/consumption rates for HUD display. */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	const TArray<FOLCResourceAmount>& GetProductionRates() const { return ProductionRates; }

	/** Get cumulative energy produced this tick (for tutorial objectives). */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	float GetCumulativeEnergyProduced() const { return CumulativeEnergyProduced; }

	// -----------------------------------------------------------------------
	// Tutorial Objective Integration (WP-104 Step 6)
	// -----------------------------------------------------------------------

	/** Called when a building is placed — advances tutorial objectives. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void OnBuildingPlaced(AActor* BuildingActor, UOLCBuildingData* BuildingData);

	/** Called each production tick to update tutorial progress. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void UpdateTutorialProgress();

	/** Check if we have a Solar Array placed (for tutorial objective completion). */
	bool HasSolarArrayPlaced() const { return bSolarArrayPlaced; }

	// -----------------------------------------------------------------------
	// Ship Module System (WP-106)
	// -----------------------------------------------------------------------

	/** Get all available ship modules for the catalog. */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	const TArray<FOLCShipModuleViewData>& GetAvailableModules() const { return AvailableModules; }

	/** Get installed modules by category. */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	TArray<FOLCShipModuleViewData> GetInstalledModules(EOLCShipModuleCategory Category) const;

	/** Install a module — deducts cost and updates ship state. Returns true on success. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	bool InstallModule(UOLCShipModuleData* ModuleData);

	/** Swap a module — removes old, installs new with 50% refund. Returns true on success. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	bool SwapModule(UOLCShipModuleData* NewModuleData);

	/** Repair a damaged module — deducts repair cost and restores integrity. Returns true on success. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	bool RepairModule(EOLCShipModuleCategory Category);

	/** Get colony TIR for module compatibility check. */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	int32 GetColonyTIR() const { return ColonyTIR; }

	/** Set colony TIR — updated as player progresses. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void SetColonyTIR(int32 InTIR);

	/** Update module state in available modules list (called by ship builder widget). */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void UpdateModuleState(const FText& ModuleName, EOLCModuleState NewState, float NewIntegrity);

	/** Add a module to the installed modules list (called by ship builder widget). */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void AddInstalledModule(const FOLCShipModuleViewData& Module);

	/** Remove an installed module by category (called by ship builder widget for swaps). */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void RemoveInstalledModuleByCategory(EOLCShipModuleCategory Category);

	/** Get refund amount for a module (50% of repair cost). */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	TArray<FOLCResourceAmount> GetModuleRefund(const FOLCShipModuleViewData& Module) const;

private:
	// Tutorial state (WP-104 Step 6)
	UPROPERTY()
	bool bSolarArrayPlaced = false;

	UPROPERTY()
	float CumulativeEnergyForTutorial = 0.0f;

	UPROPERTY()
	int32 LastCompletedObjectiveIndex = -1;

private:
	// -----------------------------------------------------------------------
	// Ship module management (WP-106)
	// -----------------------------------------------------------------------
	void PopulateShipModules();
	bool CanInstallModule(UOLCShipModuleData* ModuleData) const;
	void ApplyModuleEffects(UOLCShipModuleData* ModuleData);
	float CalculateRepairCost(EOLCShipModuleCategory Category) const;

	// -----------------------------------------------------------------------
	// Ship module state (WP-106)
	// -----------------------------------------------------------------------
	UPROPERTY()
	TArray<FOLCShipModuleViewData> AvailableModules;

	UPROPERTY()
	TArray<FOLCShipModuleViewData> InstalledModules;

	UPROPERTY()
	int32 ColonyTIR = 1; // Starting TIR

private:
	// -----------------------------------------------------------------------
	// Production tick system (WP-104)
	// -----------------------------------------------------------------------
	void PerformProductionTick();
	void UpdateResourceCounters();
	void CalculateProductionRates();
	void CheckPowerDeficit();
	void ApplyBiomeModifiers(TArray<FOLCResourceAmount>& Outputs, EOLCBiomeType Biome) const;

	/** Handle power deficit state change. */
	void OnPowerDeficitStateChanged(bool bNewDeficit);

	// -----------------------------------------------------------------------
	// Production tick state
	// -----------------------------------------------------------------------
	UPROPERTY()
	FTimerHandle ProductionTickTimer;

	UPROPERTY()
	float ProductionTickInterval = 5.0f;

	UPROPERTY()
	TArray<FRegisteredBuildingEntry> RegisteredBuildings;

	UPROPERTY()
	TArray<FOLCResourceAmount> ProductionRates;

	UPROPERTY()
	float CumulativeEnergyProduced = 0.0f;

	// -----------------------------------------------------------------------
	// Power grid state
	// -----------------------------------------------------------------------
	UPROPERTY()
	bool bPowerDeficit = false;

	UPROPERTY()
	float TotalPowerProduction = 0.0f;

	UPROPERTY()
	float TotalPowerConsumption = 0.0f;

	// -----------------------------------------------------------------------
	// Biome state
	// -----------------------------------------------------------------------
	UPROPERTY()
	EOLCBiomeType ActiveBiome = EOLCBiomeType::Desert;

	// -----------------------------------------------------------------------
	// Production Visualization (WP-104 Step 7)
	// -----------------------------------------------------------------------

	/** Update minimap markers for all registered buildings based on production status. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI|Data")
	void UpdateBuildingVisualization();

	/** Get the production state of a specific building actor. */
	UFUNCTION(BlueprintPure, Category = "OLC|UI|Data")
	bool GetBuildingProductionState(AActor* BuildingActor, bool& bIsProducing, bool& bHasPowerDeficit) const;

private:
	void PopulateFakeResources();
	void PopulateFakeMissionObjectives();
	void PopulateFakeBuildCards();
	void PopulateFakeMinimapMarkers();

	TArray<FOLCResourceCounterViewData> ResourceCounters;
	TArray<FOLCMissionObjectiveViewData> MissionObjectives;
	TArray<FOLCBuildCardViewData> BuildCards;
	TArray<EOLCConstructionCategory> ConstructionCategories;
	TArray<FOLCMinimapMarkerViewData> MinimapMarkers;

	EOLCSimulationSpeed CurrentSimulationSpeed = EOLCSimulationSpeed::Normal;

	// -----------------------------------------------------------------------
	// Unit capacity tracking
	// -----------------------------------------------------------------------
	int32 CurrentUnitCount = 0;
	int32 MaxUnitCapacity = 8; // +8 base from Habitation Module (WP-04)

	// -----------------------------------------------------------------------
	// Ship state (S13/S14 — wired to HUD and Solar System)
	// -----------------------------------------------------------------------
	EOLCModuleState DriveStatus = EOLCModuleState::Damaged; // Dropship left drive damaged on crash
	int32 DriveTier = 1;

	EOLCModuleState ShieldStatus = EOLCModuleState::Offline; // No shield after crash
	float ShieldIntegrityPercent = 0.0f;

	// +200 per resource type from each storage module installed
	int32 StorageBonusPerResourceType = 0;
};
