#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFramework/SaveGame.h"
#include "Core/OLCResourceTypes.h"
#include "OLCTutorialTypes.generated.h"

class UTexture2D;

// ---------------------------------------------------------------------------
// Tutorial objective enum (WP-129 Step 1)
//
// Enum ordering is load-bearing: the five gameplay objectives occupy values
// 1..5 in sequence order, so index i of FOLCTutorialState::bObjectiveCompleted
// maps to value (InspectCrashSite + i). UOLCTutorialSubsystem::GetObjectiveIndex
// relies on this layout.
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCTutorialObjective : uint8
{
	None              UMETA(DisplayName = "None"),
	InspectCrashSite  UMETA(DisplayName = "Inspect Crash Site"),
	CollectMaterials  UMETA(DisplayName = "Collect Materials"),
	DiscoverDeposit   UMETA(DisplayName = "Discover Deposit"),
	PlaceSolarArray   UMETA(DisplayName = "Place Solar Array"),
	BeginDriveRepair  UMETA(DisplayName = "Begin Drive Repair"),
	Complete          UMETA(DisplayName = "Complete")
};

/** Number of gameplay objectives in the tutorial (values 1..5 of EOLCTutorialObjective). */
static constexpr int32 NumOLCTutorialObjectives = 5;

// ---------------------------------------------------------------------------
// FOLCTutorialState — persisted tutorial progress (WP-129 Step 1)
//
// Plain serializable state owned by UOLCTutorialSubsystem and round-tripped
// through UOLCTutorialSaveData (native UE5 SaveGame slot). No actor or world
// references: it must survive map reloads and GameInstance re-instantiation.
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCTutorialState
{
	GENERATED_BODY()

	/**
	 * Index (0..NumOLCTutorialObjectives-1) of the currently active objective.
	 * Equals NumOLCTutorialObjectives once every objective is complete (or the
	 * tutorial was skipped) — i.e. the "Complete" sentinel.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial")
	int32 ActiveObjectiveIndex = 0;

	/** Per-objective completion flags, one entry per objective in sequence order. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial")
	TArray<bool> bObjectiveCompleted;

	/**
	 * True once the tutorial has reached its terminal state (all objectives
	 * complete or skipped), i.e. no further tutorial rewards may be granted.
	 * Guard against duplicate grants on resume/skip: consumers must check this
	 * before applying any reward batch. UOLCTutorialSubsystem::CompleteObjective
	 * sets it only AFTER broadcasting OnObjectiveCompleted for the final
	 * objective, so per-objective reward handlers still see it as false.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial")
	bool bRewardsGranted = false;

	/** True if the player explicitly skipped the tutorial. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial")
	bool bSkipped = false;

	/**
	 * Per-objective reward-granted flags (WP-129 Step 4), one entry per objective
	 * in sequence order. Set by UOLCTutorialSubsystem::ApplyObjectiveRewards once
	 * that objective's reward batch has been added to inventory — guards against
	 * duplicate grants across skip, resume, or any re-trigger of the same objective.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial")
	TArray<bool> bRewardClaimed;

	FOLCTutorialState()
	{
		bObjectiveCompleted.Init(false, NumOLCTutorialObjectives);
		bRewardClaimed.Init(false, NumOLCTutorialObjectives);
	}

	bool IsObjectiveComplete(int32 ObjectiveIndex) const
	{
		return bObjectiveCompleted.IsValidIndex(ObjectiveIndex) && bObjectiveCompleted[ObjectiveIndex];
	}

	bool AreAllObjectivesComplete() const
	{
		for (const bool bComplete : bObjectiveCompleted)
		{
			if (!bComplete)
			{
				return false;
			}
		}
		return true;
	}
};

// ---------------------------------------------------------------------------
// FOLCTutorialHighlightParams — visual highlight tuning for the active target
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCTutorialHighlightParams
{
	GENERATED_BODY()

	/** Base highlight tint applied to the target's material instance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial")
	FLinearColor HighlightColor = FLinearColor(1.0f, 0.78f, 0.25f, 1.0f);

	/** Pulse frequency in Hz for the highlight intensity oscillation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial", meta = (ClampMin = "0.0"))
	float PulseSpeed = 1.5f;

	/** Pulse amplitude as a fraction of base intensity (0 = steady highlight). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float PulseAmplitude = 0.3f;

	/** Extra scale multiplier applied to the target while highlighted. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial", meta = (ClampMin = "0.01"))
	float BaseScale = 1.0f;

	/** Distance (world units) beyond which the highlight fades out completely. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial", meta = (ClampMin = "0.0"))
	float MaxHighlightDistance = 9000.0f;

	/**
	 * Billboard sprite texture shown above the highlighted target (WP-129 Step 3).
	 * Empty → the highlight component falls back to the engine white-square texture.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial")
	TSoftObjectPtr<UTexture2D> BillboardTexture;

	/** World-space radius of the pulsing ground ring around the target (WP-129 Step 3). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial", meta = (ClampMin = "0.0"))
	float RingRadius = 500.0f;
};

// ---------------------------------------------------------------------------
// FOLCTutorialObjectiveReward — reward batch granted when one objective completes
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCTutorialObjectiveReward
{
	GENERATED_BODY()

	/** Resource amounts granted on completion of the corresponding objective (empty = no reward). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial")
	TArray<FOLCResourceAmount> Rewards;
};

// ---------------------------------------------------------------------------
// UOLCTutorialData — designer-tunable tutorial configuration (WP-129 Step 1)
//
// UPrimaryDataAsset so the canonical instance can live in the Primary Asset
// Registry. All fields are null/empty-tolerant: consumers must handle a missing
// asset by falling back to the static defaults below.
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCTutorialData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** One entry per objective, in sequence order (index 0 = InspectCrashSite). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial")
	TArray<FOLCTutorialObjectiveReward> ObjectiveRewards;

	/** Visual highlight parameters for the active objective's world target. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial")
	FOLCTutorialHighlightParams HighlightParams;

	/**
	 * Distance (world units) at which proximity-based objectives trigger
	 * (e.g. "inspect the crash site" completes when the player camera/selection
	 * is within this radius of the target actor).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial", meta = (ClampMin = "0.0"))
	float ObjectiveProximityRadius = 2500.0f;

	/**
	 * Canonical damaged-dropship starting inventory snapshot applied when the
	 * tutorial begins (see OLCMenuGameMode::TransitionToGameplay for the
	 * currently hard-coded values this mirrors).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial")
	TArray<FOLCResourceAmount> StartingInventory;

	/**
	 * Built-in defaults mirroring the canonical dropship starting inventory:
	 * Construction Material 80, Minerals 50, Fuel 30, Survival 40, Hull Parts 25.
	 */
	static TArray<FOLCResourceAmount> GetDefaultStartingInventory()
	{
		TArray<FOLCResourceAmount> Inventory;
		Inventory.Add(FOLCResourceAmount(EOLCResourceType::ConstructionMaterial, 80.0f, 1000.0f));
		Inventory.Add(FOLCResourceAmount(EOLCResourceType::Minerals, 50.0f, 1000.0f));
		Inventory.Add(FOLCResourceAmount(EOLCResourceType::Fuel, 30.0f, 1000.0f));
		Inventory.Add(FOLCResourceAmount(EOLCResourceType::Survival, 40.0f, 1000.0f));
		Inventory.Add(FOLCResourceAmount(EOLCResourceType::HullParts, 25.0f, 1000.0f));
		return Inventory;
	}

	/** Built-in default highlight parameters (matches the struct defaults). */
	static FOLCTutorialHighlightParams GetDefaultHighlightParams()
	{
		return FOLCTutorialHighlightParams();
	}
};

// ---------------------------------------------------------------------------
// UOLCTutorialSaveData — native UE5 SaveGame wrapper for FOLCTutorialState
//
// Round-trips the full tutorial state through a standard save slot
// (Saved/Saves/OLC_TutorialProgress) via UGameplayStatics::SaveGameToSlot /
// LoadGameFromSlot. Kept as a thin wrapper so the state struct stays free of
// UObject dependencies.
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCTutorialSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	/** Full tutorial progress snapshot at save time. */
	UPROPERTY(SaveGame)
	FOLCTutorialState State;
};
