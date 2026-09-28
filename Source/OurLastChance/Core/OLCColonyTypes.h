#pragma once

#include "CoreMinimal.h"
#include "Core/OLCResourceTypes.h"
#include "Core/OLCBuildingData.h"
#include "OLCColonyTypes.generated.h"

// ---------------------------------------------------------------------------
// EOLCColonyState — colony lifecycle state of a solar system cluster (WP-124).
// Drives the galaxy map marker: Visited = green outline, Owned = solid green
// fill (+ role chip), Abandoned = red fill grayed.
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCColonyState : uint8
{
	Unvisited UMETA(DisplayName = "Unvisited"),
	Visited   UMETA(DisplayName = "Visited"),
	Owned     UMETA(DisplayName = "Owned"),
	Abandoned UMETA(DisplayName = "Abandoned"),
};

// ---------------------------------------------------------------------------
// EOLCColonyRole — primary/secondary function assigned to a colony (WP-124).
// None means no role assigned.
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCColonyRole : uint8
{
	None         UMETA(DisplayName = "None"),
	Mining       UMETA(DisplayName = "Mining"),
	Fuel         UMETA(DisplayName = "Fuel"),
	Military     UMETA(DisplayName = "Military"),
	Research     UMETA(DisplayName = "Research"),
	Agricultural UMETA(DisplayName = "Agricultural"),
};

// ---------------------------------------------------------------------------
// EOLCTransportMethod — resource transfer method between colonies (WP-124).
// Tanker: slow/high capacity. Hauler: medium/medium. QuantumGate: instant,
// limited to 50 units per trip, requires operational gates on both ends.
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCTransportMethod : uint8
{
	Tanker      UMETA(DisplayName = "Tanker"),
	Hauler      UMETA(DisplayName = "Hauler"),
	QuantumGate UMETA(DisplayName = "Quantum Gate"),
};

// ---------------------------------------------------------------------------
// FOLCColonyRoleConfig — designer-tunable bonus/eligibility config for one
// colony role. Exactly one of the bonus fields is non-zero per role in the
// built-in defaults; the consuming subsystem applies whichever are set.
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCColonyRoleConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	EOLCColonyRole Role = EOLCColonyRole::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	FText DisplayName;

	/** Production bonus (percent) applied to AffectedResources. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	float ProductionBonusPercent = 0.0f;

	/** Resource types boosted by ProductionBonusPercent. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	TArray<EOLCResourceType> AffectedResources;

	/** +percent unit training speed (Military role). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	float UnitTrainingSpeedBonusPercent = 0.0f;

	/** +percent defense stats (Military role). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	float DefenseStatBonusPercent = 0.0f;

	/** +percent research speed (Research role). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	float ResearchSpeedBonusPercent = 0.0f;

	/** +percent crew capacity (Agricultural role). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	float CrewCapacityBonusPercent = 0.0f;

	/** Biomes on which this role may be assigned (empty = any biome). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	TArray<EOLCBiomeType> EligibleBiomes;

	FOLCColonyRoleConfig() {}
};

// ---------------------------------------------------------------------------
// FOLCTransportMethodConfig — designer-tunable speed/capacity/cost config for
// one transport method.
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCTransportMethodConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	EOLCTransportMethod Method = EOLCTransportMethod::Tanker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	FText DisplayName;

	/** Transit time in seconds (0 = instant). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	float TransitSeconds = 0.0f;

	/** Maximum units per resource moved in one trip. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	float CapacityPerTrip = 0.0f;

	/** Energy deducted from the source colony per trip (0 = free). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	float EnergyCostPerTrip = 0.0f;

	FOLCTransportMethodConfig() {}
};

// ---------------------------------------------------------------------------
// FOLCColonyRecord — authoritative per-colony state owned by the colony
// subsystem (one record per solar system cluster that has a colony).
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCColonyRecord
{
	GENERATED_BODY()

	/** Stable identity: ClusterID from UOLCNavigationSubsystem galaxy clusters. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	int32 ClusterID = 0;

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	FText SystemName;

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	EOLCColonyState State = EOLCColonyState::Unvisited;

	/** Biome of the colonized planet — drives role eligibility. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	EOLCBiomeType Biome = EOLCBiomeType::Desert;

	// -----------------------------------------------------------------------
	// Roles (WP-124 Step 4). A pending role takes effect only after its
	// transition timer reaches zero; until then the old role's bonuses apply.
	// -----------------------------------------------------------------------

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	EOLCColonyRole PrimaryRole = EOLCColonyRole::None;

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	EOLCColonyRole SecondaryRole = EOLCColonyRole::None;

	/** Role waiting to become the primary role (None = no pending change). */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	EOLCColonyRole PendingPrimaryRole = EOLCColonyRole::None;

	/** Seconds remaining until PendingPrimaryRole takes effect. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	float PrimaryRoleTransitionRemaining = 0.0f;

	/** Role waiting to become the secondary role (None = no pending change). */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	EOLCColonyRole PendingSecondaryRole = EOLCColonyRole::None;

	/** Seconds remaining until PendingSecondaryRole takes effect. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	float SecondaryRoleTransitionRemaining = 0.0f;

	// -----------------------------------------------------------------------
	// Quantum gate (WP-124 Step 3)
	// -----------------------------------------------------------------------

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	bool bHasGate = false;

	/** False when the gate exists but is destroyed/damaged (broken link). */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	bool bGateOperational = false;

	// -----------------------------------------------------------------------
	// Storage & production
	// -----------------------------------------------------------------------

	/** Current per-resource storage held by this colony. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	TMap<EOLCResourceType, float> Storage;

	/** Per-resource storage capacity of this colony. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	TMap<EOLCResourceType, float> StorageCapacity;

	/** Base production rate per second per resource (before role multipliers). */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	TMap<EOLCResourceType, float> ProductionRates;

	FOLCColonyRecord() {}
};

// ---------------------------------------------------------------------------
// FOLCActiveTransfer — one in-flight resource transfer between colonies.
// Instant methods (QuantumGate) never enter the active list.
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCActiveTransfer
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	int32 SourceClusterID = 0;

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	int32 DestClusterID = 0;

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	EOLCResourceType ResourceType = EOLCResourceType::Energy;

	/** Units in transit. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	float Amount = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	EOLCTransportMethod Method = EOLCTransportMethod::Tanker;

	/** Seconds remaining until the transfer completes and credits the destination. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	float TimeRemaining = 0.0f;

	FOLCActiveTransfer() {}
};

// ---------------------------------------------------------------------------
// FOLCColonyMetricsViewData — per-colony row for the production dashboard
// (WP-124 Step 5).
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCColonyMetricsViewData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	int32 ClusterID = 0;

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	FText SystemName;

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	EOLCColonyState State = EOLCColonyState::Unvisited;

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	EOLCColonyRole PrimaryRole = EOLCColonyRole::None;

	/** Current storage per resource (value + capacity). */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	TArray<FOLCResourceAmount> Storage;

	/** Effective production rate per second per resource (base × role multiplier). */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	TMap<EOLCResourceType, float> ProductionRatesPerSecond;

	/** Total units currently in flight into this colony. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	float TransferInAmount = 0.0f;

	/** Implied arrival rate: sum of Amount/TimeRemaining over in-flight incoming transfers. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	float TransferInRatePerSecond = 0.0f;

	/** Total units currently in flight out of this colony. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	float TransferOutAmount = 0.0f;

	/** Implied departure rate: sum of Amount/TimeRemaining over in-flight outgoing transfers. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	float TransferOutRatePerSecond = 0.0f;

	FOLCColonyMetricsViewData() {}
};

// ---------------------------------------------------------------------------
// FOLCProductionDashboardViewData — aggregate multi-planet dashboard payload
// (WP-124 Step 5).
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCProductionDashboardViewData
{
	GENERATED_BODY()

	/** Aggregate production per second across all owned colonies, per resource. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	TArray<FOLCResourceAmount> TotalPerTick;

	/** Per-colony breakdown rows (owned colonies only). */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	TArray<FOLCColonyMetricsViewData> PerColony;

	/** Quantum gates currently operational. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	int32 ActiveGateCount = 0;

	/** Gates that exist but are non-operational (broken links). */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	int32 BrokenLinkCount = 0;

	/** Σstorage / Σcapacity across owned colonies and all resources (0.0–1.0). */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	float CapacityUtilization = 0.0f;

	/** 0–100: actual production vs potential at best eligible role per resource. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	float EfficiencyScore = 0.0f;

	/** Human-readable alerts (low stock, broken gate link, role mismatch). */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Colony")
	TArray<FText> Alerts;

	FOLCProductionDashboardViewData() {}
};
