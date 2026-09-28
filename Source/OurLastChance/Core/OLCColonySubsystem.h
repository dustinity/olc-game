#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/OLCColonyTypes.h"
#include "OLCColonySubsystem.generated.h"

class UOLCColonyNetworkData;
class UOLCResearchSubsystem;
class UOLCUIDataSubsystem;
class UOLCTechData;

/** Delegate fired when a colony's state changes (visited/owned/abandoned). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOLCColonyStateChanged, int32, ClusterID, EOLCColonyState, NewState);

/** Delegate fired whenever the quantum gate network topology changes (WP-124 Step 3 — populated by later steps). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOLCGateNetworkChanged);

/** Delegate fired when a resource transfer completes (instant quantum-gate transfers fire with TimeRemaining = 0). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOLCTransferCompleted, FOLCActiveTransfer, CompletedTransfer);

/**
 * GameInstanceSubsystem that owns all multi-planet colony state (WP-124):
 * per-system colony records (state, biome, roles, storage, gates), role
 * assignment with 30 s transitions, and unlock gating against the research
 * subsystem. Single source of truth for colony storage — the global
 * UOLCUIDataSubsystem counters are only touched for home-colony seeding and
 * recolonize costs.
 */
UCLASS()
class OURLASTCHANCE_API UOLCColonySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	 * Advance colony timers by DeltaTime: role-transition countdowns and
	 * in-flight resource transfers (Tanker/Hauler). Plain public method —
	 * UGameInstanceSubsystem has no virtual Tick (see Knowledge/runs/WP-124/OPEN.md).
	 * Driven in-game by StartColonyTick(); called directly with controlled
	 * deltas by automation tests.
	 */
	void Tick(float DeltaTime);

	// -----------------------------------------------------------------------
	// Colony state machine (WP-124 Step 1)
	// -----------------------------------------------------------------------

	/** Mark a system as visited (Unvisited -> Visited). Creates the record if missing; Abandoned/Owned states are never downgraded. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Colony")
	void MarkSystemVisited(int32 ClusterID);

	/**
	 * Found a colony on an unvisited/visited system (state -> Owned) with the
	 * given planet biome. When the colony is the player's current system its
	 * storage is seeded from the global UOLCUIDataSubsystem counters; other
	 * colonies start with empty storage at default capacity. Abandoned
	 * systems must use RecolonizeColony instead.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Colony")
	bool FoundColony(int32 ClusterID, EOLCBiomeType Biome);

	/** Destroy the colony (hostile takeover): state -> Abandoned, storage/roles/gates cleared. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Colony")
	bool AbandonColony(int32 ClusterID);

	/**
	 * Recolonize an abandoned system: deducts the recolonize cost (default
	 * 150 Construction Material + 75 Minerals, tunable via
	 * UOLCColonyNetworkData) from the global counters (two-phase check like
	 * UOLCNavigationSubsystem::StartScan) and restores state -> Owned with
	 * fresh empty storage. Returns false without deducting when unaffordable.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Colony")
	bool RecolonizeColony(int32 ClusterID);

	/** Colony state for a cluster (Unvisited when no record exists). */
	UFUNCTION(BlueprintPure, Category = "OLC|Colony")
	EOLCColonyState GetColonyState(int32 ClusterID) const;

	/**
	 * Direct record access (null when no colony exists for the cluster).
	 * Plain C++ (not UFUNCTION) — same precedent as
	 * UOLCNavigationSubsystem::FindTierConfig.
	 */
	const FOLCColonyRecord* GetColony(int32 ClusterID) const;

	/** Copy of every colony record (all states). */
	UFUNCTION(BlueprintPure, Category = "OLC|Colony")
	TArray<FOLCColonyRecord> GetAllColonies() const;

	/** Set the colony the player is currently operating from. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Colony")
	void SetActiveColony(int32 ClusterID);

	UFUNCTION(BlueprintPure, Category = "OLC|Colony")
	int32 GetActiveColonyID() const { return ActiveColonyID; }

	// -----------------------------------------------------------------------
	// Unlock gating (WP-124: whole package at Outer ring; gates on the
	// existing "Quantum Gate Network" tech from WP-120)
	// -----------------------------------------------------------------------

	/** True once any Outer-ring (index 4) tech has completed research. */
	UFUNCTION(BlueprintPure, Category = "OLC|Colony")
	bool IsColonySystemUnlocked() const;

	/** True once a tech whose DisplayName is "Quantum Gate Network" has completed (bound OnTechCompleted + init-time recheck). */
	UFUNCTION(BlueprintPure, Category = "OLC|Colony")
	bool IsQuantumGateUnlocked() const;

	// -----------------------------------------------------------------------
	// Resource transfers (WP-124 Step 2) — Tanker / Hauler / Quantum Gate
	// -----------------------------------------------------------------------

	/**
	 * Start a resource transfer between two owned colonies.
	 * Common validation: distinct Owned colonies, colony system unlocked
	 * (Outer ring), Amount > 0, Amount <= the method's CapacityPerTrip
	 * (QuantumGate hard-capped at 50 per call — documented interpretation of
	 * "50 units per tick"), and Amount <= source storage.
	 * - Tanker/Hauler: two-phase deduction from the SOURCE colony's storage —
	 *   verify payload + EnergyCostPerTrip are available, then deduct both;
	 *   creates an FOLCActiveTransfer with TimeRemaining = TransitSeconds.
	 *   The destination is credited (clamped to per-resource capacity) when
	 *   Tick() runs the transfer down, which broadcasts OnTransferCompleted.
	 * - QuantumGate: additionally requires IsQuantumGateUnlocked() and
	 *   operational gates on both colonies; moves the amount instantly with
	 *   no energy cost and no active-transfer entry (OnTransferCompleted is
	 *   still broadcast, with TimeRemaining = 0).
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Colony")
	bool StartTransfer(int32 SourceClusterID, int32 DestClusterID, EOLCResourceType ResourceType, float Amount, EOLCTransportMethod Method);

	/** Side-effect-free validation for UI enable-state (same rules as StartTransfer). */
	UFUNCTION(BlueprintPure, Category = "OLC|Colony")
	bool CanStartTransfer(int32 SourceClusterID, int32 DestClusterID, EOLCResourceType ResourceType, float Amount, EOLCTransportMethod Method) const;

	/** Copy of all in-flight Tanker/Hauler transfers (quantum-gate moves are instant and never listed). */
	UFUNCTION(BlueprintPure, Category = "OLC|Colony")
	TArray<FOLCActiveTransfer> GetActiveTransfers() const;

	// -----------------------------------------------------------------------
	// Quantum Gate Network (WP-124 Step 3) — build/repair, max-5 cap, links
	// -----------------------------------------------------------------------

	/**
	 * Build a quantum gate on an owned colony. Requires the colony system to
	 * be unlocked, the "Quantum Gate Network" tech completed, no existing gate
	 * on that colony, and fewer than MaxQuantumGates (5) gates built overall.
	 * Deducts exactly 200 CM + 150 Minerals + 100 Energy + 50 DarkMatterCrystals
	 * from THAT colony's storage (two-phase: verify all components, then deduct
	 * all). Sets the gate operational and broadcasts OnGateNetworkChanged.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Colony")
	bool BuildQuantumGate(int32 ClusterID);

	/** Remove a gate entirely (frees its build slot). Returns false when the colony has no gate. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Colony")
	bool DestroyGate(int32 ClusterID);

	/** Mark an existing gate operational or not (broken/repairable state). No-op success when already in the requested state; false when the colony has no gate. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Colony")
	bool SetGateOperational(int32 ClusterID, bool bOperational);

	/**
	 * Repair a non-operational gate: deducts 100 CM + 75 Minerals (half build
	 * cost, documented default) from the colony's storage (two-phase) and
	 * restores operation. Returns false when there is no broken gate or the
	 * colony cannot afford it.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Colony")
	bool RepairGate(int32 ClusterID);

	/** True iff both clusters exist and have operational gates (a direct link exists). */
	UFUNCTION(BlueprintPure, Category = "OLC|Colony")
	bool AreGatesLinked(int32 ClusterA, int32 ClusterB) const;

	/**
	 * All unordered pairs of clusters whose gates are operational (first ID < second ID per pair, sorted).
	 * Non-operational gates are excluded — the path is broken. Plain C++ (not
	 * UFUNCTION): TPair has no reflection; consumed by native Slate widgets.
	 */
	TArray<TPair<int32, int32>> GetGateNetworkLinks() const;

	/** Number of colonies with a built gate (operational or not). */
	UFUNCTION(BlueprintPure, Category = "OLC|Colony")
	int32 CountActiveGates() const;

	// -----------------------------------------------------------------------
	// Colony roles (WP-124 Step 4)
	// -----------------------------------------------------------------------

	/**
	 * Assign a primary or secondary role. Validates the colony is Owned and
	 * that the role's config EligibleBiomes includes the colony biome
		(accepts when the list is empty). Starts the 30 s transition: the old
	 * role's bonuses keep applying until Tick() runs down the timer.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Colony")
	bool AssignRole(int32 ClusterID, EOLCColonyRole Role, bool bSecondary);

	/**
	 * 1.0 + primary production bonus (when ResourceType is in the role's
	 * AffectedResources) + secondary bonus / 2. Pending roles do not count —
	 * only the currently active role does.
	 */
	UFUNCTION(BlueprintPure, Category = "OLC|Colony")
	float GetEffectiveRoleMultiplier(int32 ClusterID, EOLCResourceType ResourceType) const;

	/** Sum of unit-training-speed bonuses (percent) across Owned colonies whose primary role is Military. */
	UFUNCTION(BlueprintPure, Category = "OLC|Colony")
	float GetAggregateUnitTrainingSpeedBonusPercent() const;

	/** Sum of defense-stat bonuses (percent) across Owned colonies whose primary role is Military. */
	UFUNCTION(BlueprintPure, Category = "OLC|Colony")
	float GetAggregateDefenseStatBonusPercent() const;

	/** 15 x the number of Owned colonies whose primary role is Research (percent). */
	UFUNCTION(BlueprintPure, Category = "OLC|Colony")
	float GetAggregateResearchSpeedBonusPercent() const;

	/** 10 x the number of Owned colonies whose primary role is Agricultural (percent). */
	UFUNCTION(BlueprintPure, Category = "OLC|Colony")
	float GetAggregateCrewCapacityBonusPercent() const;

	// -----------------------------------------------------------------------
	// Production dashboard (WP-124 Step 5)
	// -----------------------------------------------------------------------

	/**
	 * Build the aggregate multi-planet production dashboard payload:
	 * - TotalPerTick: per-resource sum over Owned colonies of base rate x
	 *   current effective role multiplier (rates are per second; 1 tick = 1 s).
	 * - PerColony: one row per Owned colony (storage, effective rates,
	 *   in-flight transfer-in/out amounts and implied Amount/TimeRemaining rates).
	 * - Network status: ActiveGateCount = operational gates; BrokenLinkCount =
	 *   gates that exist but are non-operational.
	 * - CapacityUtilization: sum(storage)/sum(capacity) over Owned colonies, all resources.
	 * - EfficiencyScore: 100 x actual/potential where potential uses the best
	 *   single-role bonus eligible for each colony's biome per resource
	 *   (documented interpretation of "percentage of potential production
	 *   being utilized"; 100 when potential is 0).
	 * - Alerts: low stock (any Owned colony, any resource < 15% of capacity —
	 *   zero counts), broken quantum gate link (per affected colony), role
	 *   mismatch (active primary role not eligible for the colony biome).
	 */
	UFUNCTION(BlueprintPure, Category = "OLC|Colony")
	FOLCProductionDashboardViewData BuildProductionDashboard() const;

	/**
	 * Set the per-colony base production rates (units/second per resource) —
	 * gameplay layers/tests feed these in because off-screen colonies have no
	 * building registry yet (documented limitation). Replaces previously set
	 * rates. Returns false for unknown/non-owned colonies.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Colony")
	bool SetColonyProductionRates(int32 ClusterID, const TArray<FOLCResourceAmount>& Rates);

	// -----------------------------------------------------------------------
	// Config (UOLCColonyNetworkData — mirrors UOLCNavigationSubsystem::SetScanTierData)
	// -----------------------------------------------------------------------

	/** Register the colony network DataAsset. Null restores built-in defaults. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Colony")
	void SetColonyNetworkData(UOLCColonyNetworkData* InData);

	UFUNCTION(BlueprintPure, Category = "OLC|Colony")
	UOLCColonyNetworkData* GetColonyNetworkData() const { return ColonyNetworkData; }

	// -----------------------------------------------------------------------
	// Tick driver (world timer — same pattern as UOLCUIDataSubsystem production tick)
	// -----------------------------------------------------------------------

	/** Start the repeating world timer that advances Tick() in-game. No-op when already running or no world. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Colony")
	void StartColonyTick();

	/** Stop the colony tick timer. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Colony")
	void StopColonyTick();

	// -----------------------------------------------------------------------
	// Events (for Blueprint binding)
	// -----------------------------------------------------------------------

	UPROPERTY(BlueprintAssignable, Category = "OLC|Colony")
	FOnOLCColonyStateChanged OnColonyStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "OLC|Colony")
	FOnOLCGateNetworkChanged OnGateNetworkChanged;

	UPROPERTY(BlueprintAssignable, Category = "OLC|Colony")
	FOnOLCTransferCompleted OnTransferCompleted;

private:
	/** ERingTier::Outer as UOLCTechData::GetRingTierIndex() (Core=0 ... Outer=4). */
	static constexpr int32 OuterRingIndex = 4;

	/** Seconds a role change takes to take effect (WP-124 Step 4). */
	static constexpr float RoleTransitionSeconds = 30.0f;

	/** Fallback recolonize cost — used when no UOLCColonyNetworkData asset is registered (the asset's fields take precedence). */
	static constexpr float RecolonizeCostConstructionMaterial = 150.0f;
	static constexpr float RecolonizeCostMinerals = 75.0f;

	/** Default per-resource storage capacity for a fresh colony. */
	static constexpr float DefaultStorageCapacityPerResource = 1000.0f;

	/** Colony tick timer interval (seconds). */
	static constexpr float ColonyTickIntervalSeconds = 0.25f;

	/** DisplayName of the WP-120 tech that unlocks quantum gates. */
	static constexpr const TCHAR* QuantumGateTechName = TEXT("Quantum Gate Network");

	/** Hard cap on a quantum-gate transfer per call (WP-124: "50 units per tick"). */
	static constexpr float QuantumGateHardCapPerCall = 50.0f;

	/** Balance constraint: max quantum gates per save game (WP-124 Step 3). */
	static constexpr int32 MaxQuantumGates = 5;

	/** Gate build cost — exact WP-124 numbers, deducted from the colony's own storage. */
	static constexpr float QuantumGateBuildCostConstructionMaterial = 200.0f;
	static constexpr float QuantumGateBuildCostMinerals = 150.0f;
	static constexpr float QuantumGateBuildCostEnergy = 100.0f;
	static constexpr float QuantumGateBuildCostDarkMatterCrystals = 50.0f;

	/** Gate repair cost — half build (documented default), CM + Minerals only. */
	static constexpr float QuantumGateRepairCostConstructionMaterial = 100.0f;
	static constexpr float QuantumGateRepairCostMinerals = 75.0f;

	/** Dashboard low-stock threshold: alert when a resource is below this percent of capacity (WP-124 Step 5). */
	static constexpr float LowStockThresholdPercent = 15.0f;

	/** Get-or-create the colony record for a cluster. */
	FOLCColonyRecord& GetOrCreateColony(int32 ClusterID);

	/** Fill Storage/StorageCapacity with all resource types at 0 / default capacity. */
	void InitializeEmptyStorage(FOLCColonyRecord& Record) const;

	/** Copy the global UOLCUIDataSubsystem counters into a colony's storage (home colony). */
	void SeedStorageFromGlobalCounters(FOLCColonyRecord& Record) const;

	/** True when ClusterID is the system the player currently occupies. */
	bool IsCurrentSystem(int32 ClusterID) const;

	/** Look up a role config from the registered asset, else built-in defaults. */
	const FOLCColonyRoleConfig* FindRoleConfig(EOLCColonyRole Role) const;

	/** Production bonus (percent) a role grants for one resource type (0 when not affected). */
	float GetRoleProductionBonusPercent(EOLCColonyRole Role, EOLCResourceType ResourceType) const;

	/** Best single-role production multiplier achievable for a resource on a biome (>= 1.0, no-role baseline). */
	float GetBestRoleMultiplierForBiome(EOLCBiomeType Biome, EOLCResourceType ResourceType) const;

	/** Look up a transport method config from the registered asset, else built-in defaults. */
	const FOLCTransportMethodConfig* FindTransportConfig(EOLCTransportMethod Method) const;

	/** Full StartTransfer validation without side effects (logs the rejection reason). */
	bool ValidateTransfer(int32 SourceClusterID, int32 DestClusterID, EOLCResourceType ResourceType, float Amount, EOLCTransportMethod Method) const;

	/** Credit a colony's storage for one resource, clamped to its per-resource capacity (overflow dropped + logged). */
	void CreditToColonyStorage(FOLCColonyRecord& Record, EOLCResourceType ResourceType, float Amount) const;

	/** Advance in-flight transfers by DeltaTime; deliver and broadcast completions. Called from Tick(). */
	void TickActiveTransfers(float DeltaTime);

	/** Null-safe fetch of the UI data subsystem. */
	UOLCUIDataSubsystem* GetUIData() const;

	/** Null-safe fetch of the research subsystem. */
	UOLCResearchSubsystem* GetResearch() const;

	/** Handler for UOLCResearchSubsystem::OnTechCompleted — null-safe (CancelResearch broadcasts nullptr). */
	UFUNCTION()
	void HandleTechCompleted(UOLCTechData* CompletedTech);

	/** Colony tick timer callback: advances Tick() by the fixed colony tick interval. */
	void PerformColonyTick();

	/** All colony records, keyed by galaxy cluster ID. */
	TMap<int32, FOLCColonyRecord> Colonies;

	/** In-flight Tanker/Hauler transfers (quantum-gate moves are instant and never listed). */
	TArray<FOLCActiveTransfer> ActiveTransfers;

	/** Colony the player is currently operating from (0-based cluster ID). */
	int32 ActiveColonyID = 0;

	/** Set when a "Quantum Gate Network" tech completes (delegate or init-time recheck). */
	bool bQuantumGateUnlocked = false;

	/** Repeating world timer driving Tick() in-game. */
	FTimerHandle ColonyTickTimer;

	UPROPERTY()
	TObjectPtr<UOLCColonyNetworkData> ColonyNetworkData;
};
