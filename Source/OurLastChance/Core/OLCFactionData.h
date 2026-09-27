#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OLCFactionData.generated.h"

// ---------------------------------------------------------------------------
// UOLCFactionData — DataAsset for faction selection screen data
// Backs WBP_FactionSelect with configurable stats, bonuses, and champions.
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCFactionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UOLCFactionData();

	// -----------------------------------------------------------------------
	// Identity
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Faction|Identity")
	FString FactionId; // e.g. "NeonPunk", "DarkRealistic"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Faction|Identity")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Faction|Identity")
	FText ThemeDescription;

	// -----------------------------------------------------------------------
	// Ratings — 1 to 5 scale (displayed as stat bars in dossier)
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Faction|Stats")
	int32 MiningRating = 3; // 1-5

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Faction|Stats")
	int32 DefenseRating = 3; // 1-5

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Faction|Stats")
	int32 MobilityRating = 3; // 1-5

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Faction|Stats")
	int32 SurvivalRating = 3; // 1-5

	// -----------------------------------------------------------------------
	// Faction-specific bonuses
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Faction|Bonuses")
	FString PreferredRaceBonus; // Name of race that gets bonus vs this faction

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Faction|Bonuses")
	FText SignatureBuilding; // Faction's unique building name

	// -----------------------------------------------------------------------
	// Champions — array of champion IDs available to this faction (3 each)
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Faction|Champions")
	TArray<FString> AvailableChampions;

	// -----------------------------------------------------------------------
	// Visual — texture for preview/icon in dossier panel
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Faction|Visual")
	UTexture2D* PreviewTexture = nullptr;

	// -----------------------------------------------------------------------
	// Helpers
	// -----------------------------------------------------------------------
	UFUNCTION(BlueprintPure, Category = "OLC|Faction")
	int32 GetTotalStats() const
	{
		return MiningRating + DefenseRating + MobilityRating + SurvivalRating;
	}
};
