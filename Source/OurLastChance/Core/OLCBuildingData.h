#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Combat/OLCWeaponData.h"
#include "Core/OLCResourceTypes.h"
#include "OLCBuildingData.generated.h"

// ---------------------------------------------------------------------------
// Biome types — used by building biome modifier system
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCBiomeType : uint8
{
	Desert     UMETA(DisplayName = "Desert"),
	Dusty      UMETA(DisplayName = "Dusty"),
	Rocky      UMETA(DisplayName = "Rocky"),
	Water      UMETA(DisplayName = "Water"),
	Swamp      UMETA(DisplayName = "Swamp"),
	Jungle     UMETA(DisplayName = "Jungle"),
	LightSnow  UMETA(DisplayName = "Light Snow"),
	Ice        UMETA(DisplayName = "Ice"),
};

// ---------------------------------------------------------------------------
// FOLCBiomeModifier — per-biome production multiplier for a building
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCBiomeModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	EOLCBiomeType BiomeType = EOLCBiomeType::Desert;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	float Multiplier = 1.0f; // 1.0 = no change, 1.5 = +50%, 0.5 = -50%

	FOLCBiomeModifier() {}

	FOLCBiomeModifier(EOLCBiomeType InBiome, float InMultiplier)
		: BiomeType(InBiome), Multiplier(InMultiplier) {}
};

USTRUCT(BlueprintType)
struct FOLCBuildingConfig
{
	GENERATED_BODY()

	/** Stable id used for systems that key off a building type (e.g. animation-set matching). Empty = unset. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	FString BuildingId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	EOLCConstructionCategory Category = EOLCConstructionCategory::Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	FVector2D GridSize = FVector2D(2.0f, 2.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	TArray<FOLCResourceAmount> BuildCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	float PowerConsumption = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	TArray<FOLCResourceAmount> OutputPerTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	int32 TIRRequirement = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	TArray<FOLCBiomeModifier> BiomeModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building|Weapons")
	TArray<FOLCTurretMountConfig> TurretMounts;

	float GetBiomeMultiplier(EOLCBiomeType Biome) const
	{
		for (const FOLCBiomeModifier& Mod : BiomeModifiers)
		{
			if (Mod.BiomeType == Biome)
			{
				return Mod.Multiplier;
			}
		}
		return 1.0f;
	}
};

// ---------------------------------------------------------------------------
// UOLCBuildingData — DataAsset holding configurable building stats
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCBuildingData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UOLCBuildingData();

	// -----------------------------------------------------------------------
	// Stats
	// -----------------------------------------------------------------------
	/** Stable id used for systems that key off a building type (e.g. animation-set matching). Empty = unset. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	FString BuildingId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	EOLCConstructionCategory Category = EOLCConstructionCategory::Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	FVector2D GridSize = FVector2D(2.0f, 2.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	TArray<FOLCResourceAmount> BuildCost;

	/** Negative value = produces power. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	float PowerConsumption = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	TArray<FOLCResourceAmount> OutputPerTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	int32 TIRRequirement = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	FText Description;

	// -----------------------------------------------------------------------
	// Biome modifiers — production multipliers per biome type
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
	TArray<FOLCBiomeModifier> BiomeModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building|Weapons")
	TArray<FOLCTurretMountConfig> TurretMounts;

	// -----------------------------------------------------------------------
	// Helper: get multiplier for a given biome (default 1.0f)
	// -----------------------------------------------------------------------
	UFUNCTION(BlueprintPure, Category = "OLC|Building")
	FOLCBuildingConfig ToConfig() const
	{
		FOLCBuildingConfig Config;
		Config.BuildingId = BuildingId;
		Config.DisplayName = DisplayName;
		Config.Category = Category;
		Config.GridSize = GridSize;
		Config.BuildCost = BuildCost;
		Config.PowerConsumption = PowerConsumption;
		Config.OutputPerTick = OutputPerTick;
		Config.TIRRequirement = TIRRequirement;
		Config.Description = Description;
		Config.BiomeModifiers = BiomeModifiers;
		Config.TurretMounts = TurretMounts;
		return Config;
	}

	UFUNCTION(BlueprintPure, Category = "OLC|Building")
	float GetBiomeMultiplier(EOLCBiomeType Biome) const
	{
		for (const auto& Mod : BiomeModifiers)
		{
			if (Mod.BiomeType == Biome)
				return Mod.Multiplier;
		}
		return 1.0f;
	}
};
