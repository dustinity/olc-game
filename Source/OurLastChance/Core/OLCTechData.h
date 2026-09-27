#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OLCResourceTypes.h"
#include "OLCTechData.generated.h"

// ---------------------------------------------------------------------------
// Tech categories — matches Briefing 8-category cluster system
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCTechCategory : uint8
{
	Weapons    UMETA(DisplayName = "Weapons"),
	Armor      UMETA(DisplayName = "Armor"),
	Drives     UMETA(DisplayName = "Drives"),
	Energy     UMETA(DisplayName = "Energy"),
	Vision     UMETA(DisplayName = "Vision"),
	Storage    UMETA(DisplayName = "Storage"),
	Buildings  UMETA(DisplayName = "Buildings"),
	Units      UMETA(DisplayName = "Units"),
};

// ---------------------------------------------------------------------------
// Ring tiers — concentric progression from core to outer galaxy
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class ERingTier : uint8
{
	Core   UMETA(DisplayName = "Core (Starting)"),
	Ring1  UMETA(DisplayName = "Ring 1 (TIR 1-2)"),
	Ring2  UMETA(DisplayName = "Ring 2 (TIR 2-3)"),
	Ring3  UMETA(DisplayName = "Ring 3 (TIR 3-4)"),
	Outer  UMETA(DisplayName = "Outer Ring (TIR 4-5)"),
};

// ---------------------------------------------------------------------------
// Research state — tracks where a tech is in the pipeline
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCTechState : uint8
{
	Locked        UMETA(DisplayName = "Locked"),
	Available     UMETA(DisplayName = "Available to Research"),
	Researching   UMETA(DisplayName = "Currently Researching"),
	Completed     UMETA(DisplayName = "Completed — Unlocked"),
};

/** A single tech topic in the research tree. */
UCLASS()
class OURLASTCHANCE_API UOLCTechData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UOLCTechData();

	/** Get default class. */
	static UClass* GetDefaultClass() { return StaticClass(); }

	// -----------------------------------------------------------------------
	// Core identity
	// -----------------------------------------------------------------------
	/** Display name shown in the tech tree UI. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OLC|Tech")
	FText DisplayName;

	/** Which research category this topic belongs to. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OLC|Tech")
	EOLCTechCategory Category = EOLCTechCategory::Buildings;

	/** Ring tier — gates availability by progression. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OLC|Tech")
	ERingTier RingTier = ERingTier::Core;

	// -----------------------------------------------------------------------
	// Prerequisites & dependencies
	// -----------------------------------------------------------------------
	/** Other tech DataAssets that must be completed before this becomes available. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OLC|Tech")
	TArray<TObjectPtr<UOLCTechData>> Prerequisites;

	// -----------------------------------------------------------------------
	// Research cost and duration
	// -----------------------------------------------------------------------
	/** Resource costs to begin research. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OLC|Tech")
	TArray<FOLCResourceAmount> MaterialCost;

	/** Research time in seconds. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OLC|Tech")
	float ResearchTimeSeconds = 60.0f;

	// -----------------------------------------------------------------------
	// Effect description
	// -----------------------------------------------------------------------
	/** What this research unlocks or enables when completed. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OLC|Tech")
	FText EffectDescription;

	/** Which build card this tech unlocks (empty = no build unlock). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OLC|Tech")
	FText UnlocksBuildCardName;

	// -----------------------------------------------------------------------
	// Auto-unlock flag for Core ring topics
	// -----------------------------------------------------------------------
	/** If true, this tech is auto-completed at game start (no research needed). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OLC|Tech")
	bool bAutoUnlock = false;

	// -----------------------------------------------------------------------
	// Helpers
	// -----------------------------------------------------------------------
	/** Check if all prerequisites are completed. */
	bool ArePrerequisitesMet(const TArray<TObjectPtr<UOLCTechData>>& CompletedTechs) const;

	/** Get the total material cost as a string for UI display. */
	UFUNCTION(BlueprintPure, Category = "OLC|Tech")
	FText GetCostSummary() const;

	/** Get ring tier as an integer (0-4). */
	UFUNCTION(BlueprintPure, Category = "OLC|Tech")
	int32 GetRingTierIndex() const;
};
