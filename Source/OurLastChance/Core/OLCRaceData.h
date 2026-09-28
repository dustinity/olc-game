#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OLCRaceFamily.h"
#include "OLCBiomeTypes.h"
#include "OLCRaceData.generated.h"

// ---------------------------------------------------------------------------
// UOLCRaceData — Base DataAsset for all races
// Contains stats, biome preferences, and family classification.
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCRaceData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UOLCRaceData();

	// -----------------------------------------------------------------------
	// Identity
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Race|Identity")
	FString RaceId; // e.g. "PrismGuardians", "RustbornClan"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Race|Identity")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Race|Identity")
	EOLCRaceFamily RaceFamily = EOLCRaceFamily::Unknown;

	// -----------------------------------------------------------------------
	// Base Stats
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Race|Stats")
	float BaseHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Race|Stats")
	float BaseDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Race|Stats")
	float MovementSpeed = 300.0f;

	// -----------------------------------------------------------------------
	// Biome Preferences
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Race|Biomes")
	TArray<EOLCRaceBiomeType> PreferredBiomes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Race|Biomes")
	TMap<EOLCRaceBiomeType, float> BiomeSpawnWeights;

	// -----------------------------------------------------------------------
	// Combat
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Race|Combat")
	FText SpecialAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Race|Combat")
	FText Weakness;

	// -----------------------------------------------------------------------
	// Helpers
	// -----------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "OLC|Race")
	float GetSpawnWeightForBiome(EOLCRaceBiomeType InBiome) const;

	UFUNCTION(BlueprintCallable, Category = "OLC|Race")
	bool IsRaceInFamily(EOLCRaceFamily InFamily) const;
};
