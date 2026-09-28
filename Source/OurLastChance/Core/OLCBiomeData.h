#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/OLCResourceTypes.h"
#include "Core/OLCBuildingData.h"
#include "OLCBiomeData.generated.h"

class UMaterialInterface;

// ---------------------------------------------------------------------------
// UOLCBiomeData — DataAsset holding configurable biome settings
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCBiomeData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UOLCBiomeData();

	// -----------------------------------------------------------------------
	// Identity
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Biome")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Biome")
	EOLCBiomeType BiomeType = EOLCBiomeType::Desert;

	// -----------------------------------------------------------------------
	// Resource multipliers — applied to building OutputPerTick
	// 1.0 = no change, 1.5 = +50%, 0.5 = -50%
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Biome|Modifiers")
	float EnergyMod = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Biome|Modifiers")
	float FuelMod = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Biome|Modifiers")
	float ConstructionMod = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Biome|Modifiers")
	float MineralsMod = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Biome|Modifiers")
	float HullMod = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Biome|Modifiers")
	float SurvivalMod = 1.0f;

	// -----------------------------------------------------------------------
	// Wall HP modifier — affects building footprint collision durability
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Biome|Modifiers")
	float WallHPModifier = 1.0f;

	// -----------------------------------------------------------------------
	// Visual references (set in editor)
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Biome|Visual")
	TObjectPtr<UMaterialInterface> TerrainMaterial;

	// -----------------------------------------------------------------------
	// Helper: get multiplier for a given resource type
	// -----------------------------------------------------------------------
	UFUNCTION(BlueprintPure, Category = "OLC|Biome")
	float GetResourceMultiplier(EOLCResourceType ResourceType) const
	{
		switch (ResourceType)
		{
			case EOLCResourceType::Energy:               return EnergyMod;
			case EOLCResourceType::Fuel:                 return FuelMod;
			case EOLCResourceType::ConstructionMaterial: return ConstructionMod;
			case EOLCResourceType::Minerals:             return MineralsMod;
			case EOLCResourceType::HullParts:            return HullMod;
			case EOLCResourceType::Survival:             return SurvivalMod;
			case EOLCResourceType::DarkMatterCrystals:   return 1.0f; // no biome modifier defined
		}
		return 1.0f;
	}
};
