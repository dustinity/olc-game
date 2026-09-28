#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Combat/OLCCombatTypes.h"
#include "OLCMinigameData.generated.h"

/**
 * One row of the data-driven minigame table (WP-122 step 6).
 * UOLCMinigameManager::StartMinigameFromData configures the manager from one of these,
 * so any EOLCMinigameType can be driven by authored data instead of the legacy
 * hardcoded switch (which stays untouched for the original four minigames).
 */
USTRUCT(BlueprintType)
struct FOLCMinigameDefinition
{
	GENERATED_BODY()

	/** Minigame type this row configures. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Minigames")
	EOLCMinigameType MinigameType = EOLCMinigameType::WireRepair;

	/** Display name for the minigame UI. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Minigames")
	FString DisplayName;

	/** Input tokens the player must submit in order (matched case-insensitively by the manager). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Minigames")
	TArray<FString> InputSequence;

	/** Time limit for the minigame in seconds (presented/enforced by the UI; the manager does not tick it). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Minigames")
	float DurationSeconds = 20.0f;

	/** Reference scoring: points per correct input (the legacy manager core uses +25). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Minigames")
	int32 PointsPerSuccess = 25;

	/** Reference scoring: points lost per wrong input (the legacy manager core uses -10). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Minigames")
	int32 PointsPerFailure = -10;
};

/**
 * Data-driven minigame table (WP-122 step 6).
 *
 * The constructor seeds the two new in-transit minigames (CargoBayManagement,
 * ReactorStabilization), so the class CDO doubles as the code-side fallback table:
 * callers read an authored instance when one exists and otherwise fall back to
 * GetDefault<UOLCMinigameData>(). The legacy four minigames remain configured by
 * UOLCMinigameManager::StartMinigame's hardcoded switch.
 */
UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCMinigameData : public UDataAsset
{
	GENERATED_BODY()

public:
	UOLCMinigameData();

	/** The minigame table (looked up by MinigameType, order-independent). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Minigames")
	TArray<FOLCMinigameDefinition> MinigameTable;

	/** @return pointer into MinigameTable, or nullptr if the type has no row. */
	const FOLCMinigameDefinition* FindData(EOLCMinigameType InType) const;
};
