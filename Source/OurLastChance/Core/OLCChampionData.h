#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OLCChampionData.generated.h"

// ---------------------------------------------------------------------------
// Champion role enum
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCChampionRole : uint8
{
	Combatant	UMETA(DisplayName = "Combatant"),
	Engineer	UMETA(DisplayName = "Engineer"),
	Scout		UMETA(DisplayName = "Scout"),
	Support		UMETA(DisplayName = "Support"),
	Specialist	UMETA(DisplayName = "Specialist"),
};

// ---------------------------------------------------------------------------
// Champion ability descriptor
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCChampionAbility
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion")
	FString AbilityId; // e.g. "CloakField", "RapidBuild"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion")
	FText AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion")
	FText AbilityDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion")
	int32 UnlockLevel = 5; // Level at which ability unlocks (5 / 10 / 20)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion")
	int32 AbilityIconIndex = 0; // Index into T_CS_Ability_Icon_Atlas (128px cells)
};

// ---------------------------------------------------------------------------
// UOLCChampionData — DataAsset for champion selection screen data
// Backs WBP_ChampionSelect with configurable stats, abilities, and gear.
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCChampionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UOLCChampionData();

	// -----------------------------------------------------------------------
	// Identity
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion|Identity")
	FString ChampionId; // e.g. "CH-NP-01", "CH-DR-02"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion|Identity")
	FString FactionId; // e.g. "NeonPunk", "DarkRealistic"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion|Identity")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion|Identity")
	FText RealName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion|Identity")
	EOLCChampionRole Role = EOLCChampionRole::Combatant;

	// -----------------------------------------------------------------------
	// Ratings — 1 to 5 scale (displayed as stat bars in dossier)
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion|Stats")
	int32 CombatRating = 3; // 1-5

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion|Stats")
	int32 EngineeringRating = 3; // 1-5

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion|Stats")
	int32 MobilityRating = 3; // 1-5

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion|Stats")
	int32 SurvivalRating = 3; // 1-5

	// -----------------------------------------------------------------------
	// Abilities — typically 3 abilities per champion (Lvl 5 / 10 / 20)
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion|Abilities")
	TArray<FOLCChampionAbility> Abilities;

	// -----------------------------------------------------------------------
	// Starting gear — array of equipment IDs the champion begins with
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion|Gear")
	TArray<FString> StartingGearIds;

	// -----------------------------------------------------------------------
	// Passive bonus — short text description of the champion's passive effect
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion|Passive")
	FText PassiveDescription;

	// -----------------------------------------------------------------------
	// Benefit summary — combined text shown in CampaignBenefitReview panel
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion|Display")
	FText BenefitSummary;

	// -----------------------------------------------------------------------
	// Visual — portrait and full body textures for dossier panel
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion|Visual")
	UTexture2D* PortraitTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion|Visual")
	UTexture2D* FullBodyTexture = nullptr;

	// -----------------------------------------------------------------------
	// Helpers
	// -----------------------------------------------------------------------
	UFUNCTION(BlueprintPure, Category = "OLC|Champion")
	int32 GetTotalStats() const
	{
		return CombatRating + EngineeringRating + MobilityRating + SurvivalRating;
	}

	UFUNCTION(BlueprintPure, Category = "OLC|Champion")
	int32 GetAbilityCount() const
	{
		return Abilities.Num();
	}
};
