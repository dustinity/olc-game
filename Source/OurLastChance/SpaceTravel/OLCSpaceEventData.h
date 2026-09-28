#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Combat/OLCCombatTypes.h"
#include "Core/OLCResourceTypes.h"
#include "OLCSpaceEventData.generated.h"

/**
 * Space event types rolled during the EventCheck travel state (WP-122 step 4).
 * None is the sentinel for "no event this trip".
 */
UENUM(BlueprintType)
enum class EOLCSpaceEventType : uint8
{
	None            UMETA(DisplayName = "None"),
	PirateEncounter UMETA(DisplayName = "Pirate Encounter"),
	DerelictShip    UMETA(DisplayName = "Derelict Ship"),
	Anomaly         UMETA(DisplayName = "Anomaly"),
	Merchant        UMETA(DisplayName = "Merchant"),
	MeteorShower    UMETA(DisplayName = "Meteor Shower")
};

/**
 * One row of the space event table (WP-122 step 4).
 * Weight/MinTIR drive the roll; RewardPayload / HazardHullDamagePercent / minigame
 * fields describe the outcome applied when the event resolves.
 */
USTRUCT(BlueprintType)
struct FOLCSpaceEventDefinition
{
	GENERATED_BODY()

	/** Event type this row defines. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceEvents")
	EOLCSpaceEventType EventType = EOLCSpaceEventType::None;

	/** Relative weight in the weighted roll (0 disables the row). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceEvents")
	float Weight = 1.0f;

	/** Minimum colony TIR at which this event can occur. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceEvents")
	int32 MinTIR = 1;

	/** Resources granted when the event resolves favorably (CurrentValue = amount). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceEvents")
	TArray<FOLCResourceAmount> RewardPayload;

	/** Hull damage in percent applied when the event resolves as a hazard (0-100). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceEvents")
	float HazardHullDamagePercent = 0.0f;

	/** Whether resolving this event launches a minigame via UOLCMinigameManager. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceEvents")
	bool bHasMinigame = false;

	/** Minigame launched when bHasMinigame is set (e.g. AsteroidEvasion for meteor showers). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceEvents")
	EOLCMinigameType MinigameType = EOLCMinigameType::AsteroidEvasion;

	/** Duration passed to UOLCMinigameManager::StartMinigame. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceEvents")
	float MinigameDurationSeconds = 15.0f;
};

/**
 * Data-driven space event table (WP-122 step 4).
 *
 * The constructor seeds the five default events, so the class CDO doubles as the
 * code-side fallback table: UOLCTravelSubsystem reads an authored instance when one
 * is assigned and otherwise falls back to GetDefault<UOLCSpaceEventData>().
 */
UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCSpaceEventData : public UDataAsset
{
	GENERATED_BODY()

public:
	UOLCSpaceEventData();

	/** Base chance that an event occurs on a trip (before distance scaling). ~15% per WP-122. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceEvents")
	float BaseEventChance = 0.15f;

	/** Max multiplier applied to BaseEventChance by route distance (longer = more encounters). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceEvents")
	float MaxDistanceChanceMultiplier = 3.0f;

	/** When fuel is below this fraction of capacity, pirate weight is boosted. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceEvents")
	float LowFuelThreshold = 0.25f;

	/** Pirate weight multiplier while fuel is below LowFuelThreshold. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceEvents")
	float LowFuelPirateWeightMultiplier = 2.0f;

	/** The event table (looked up by EventType, order-independent). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceEvents")
	TArray<FOLCSpaceEventDefinition> EventTable;

	/** @return pointer into EventTable, or nullptr if the type has no row. */
	const FOLCSpaceEventDefinition* FindEvent(EOLCSpaceEventType InType) const;
};
