#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/OLCResourceTypes.h"
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
	FText DisplayName;

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

	/** Faction exclusivity — empty string = all factions. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	FText FactionExclusivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	int32 TIRRequirement = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	FText Description;

	// Fuel consumption for vehicle/aerial units (fuel per minute).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Unit")
	float FuelConsumptionPerMinute = 0.0f;
};
