#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/OLCResourceTypes.h"
#include "Core/OLCTerrainTypes.h"
#include "OLCUnitData.generated.h"

// ---------------------------------------------------------------------------
// Unit categories — matches Briefing unit types classification
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCUnitCategory : uint8
{
	Infantry     UMETA(DisplayName = "Infantry"),
	LightVehicle UMETA(DisplayName = "Light Vehicle"),
	HeavyVehicle UMETA(DisplayName = "Heavy Vehicle"),
	Aerial       UMETA(DisplayName = "Aerial"),
	Support      UMETA(DisplayName = "Support"),
};

UENUM(BlueprintType)
enum class EOLCUnitType : uint8
{
	Infantry UMETA(DisplayName = "Infantry"),
	Vehicle UMETA(DisplayName = "Vehicle"),
	Aerial UMETA(DisplayName = "Aerial"),
	Champion UMETA(DisplayName = "Champion")
};

USTRUCT(BlueprintType)
struct FOLCUnitBiomeModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	EOLCBiomeType Biome = EOLCBiomeType::Desert;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	float SpeedMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	float RangeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	bool bCanSpawn = true;
};

// ---------------------------------------------------------------------------
// UOLCUnitData — DataAsset holding configurable unit stats
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCUnitData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UOLCUnitData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	FString UnitId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	EOLCUnitType UnitType = EOLCUnitType::Infantry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	EOLCUnitCategory UnitCategory = EOLCUnitCategory::Infantry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	float MovementSpeed = 200.0f; // world units per second

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	float AttackDamage = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	float Range = 20.0f; // world units

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	TArray<FOLCResourceAmount> BuildCost;

	/** Training/production time in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	float TrainingTime = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	float ProductionTimeSeconds = 30.0f;

	/** Faction exclusivity — empty string = all factions. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	FText FactionExclusivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	FString FactionId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	int32 TIRRequirement = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	FText Description;

	// Fuel consumption for vehicle/aerial units (fuel per minute).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	float FuelConsumptionPerMinute = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	float FuelConsumptionPerTick = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	FIntPoint GridSize = FIntPoint(1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	bool bRequiresAirfield = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	TArray<FOLCUnitBiomeModifier> BiomeModifiers;

	UFUNCTION(BlueprintPure, Category = "OLC|Unit")
	FOLCUnitBiomeModifier GetModifierForBiome(EOLCBiomeType Biome) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Unit")
	bool IsAvailableForFaction(const FString& InFactionId) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Unit")
	bool IsUnlockedAtTIR(int32 ColonyTIR) const { return ColonyTIR >= TIRRequirement; }

	UFUNCTION(BlueprintCallable, Category = "OLC|Unit")
	static TArray<UOLCUnitData*> GetDefaultUnitCatalog(UObject* Outer);

	UFUNCTION(BlueprintCallable, Category = "OLC|Unit")
	static TArray<UOLCUnitData*> GetUnitsByType(UObject* Outer, EOLCUnitType Type);

	UFUNCTION(BlueprintCallable, Category = "OLC|Unit")
	static TArray<UOLCUnitData*> GetUnitsByFaction(UObject* Outer, const FString& InFactionId, int32 ColonyTIR);
};
