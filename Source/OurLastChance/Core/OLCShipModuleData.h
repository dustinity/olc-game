#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OLCResourceTypes.h"
#include "OLCShipModuleData.generated.h"

// ---------------------------------------------------------------------------
// Module category — matches Briefing/ShipModules/README.md (7 categories)
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCShipModuleCategory : uint8
{
	Drives     UMETA(DisplayName = "Drives"),
	Storage    UMETA(DisplayName = "Storage"),
	Protection UMETA(DisplayName = "Protection"),
	Scanning   UMETA(DisplayName = "Scanning"),
	Weapons    UMETA(DisplayName = "Weapons"),
	Labs       UMETA(DisplayName = "Labs"),
	Support    UMETA(DisplayName = "Support"),
};

// ---------------------------------------------------------------------------
// Module state — whether a module is installed, damaged, or offline
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCModuleState : uint8
{
	Installed UMETA(DisplayName = "Installed"),
	Damaged   UMETA(DisplayName = "Damaged"),
	Offline   UMETA(DisplayName = "Offline"),
};

// ---------------------------------------------------------------------------
// FOLCShipModuleViewData — display data for a ship module
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCShipModuleViewData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	EOLCShipModuleCategory Category = EOLCShipModuleCategory::Drives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	int32 TIRTier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	int32 HullSlotsRequired = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	float PowerConsumption = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	FText EffectDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	EOLCModuleState State = EOLCModuleState::Installed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	float IntegrityPercent = 1.0f; // 0.0–1.0, for damaged modules

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	TArray<FOLCResourceAmount> RepairCost;

	FOLCShipModuleViewData() {}

	FOLCShipModuleViewData(const FText& InName, EOLCShipModuleCategory InCat, int32 Tier, int32 Slots, float Power, const FText& InDesc)
		: DisplayName(InName), Category(InCat), TIRTier(Tier), HullSlotsRequired(Slots),
		  PowerConsumption(Power), EffectDescription(InDesc) {}
};

// ---------------------------------------------------------------------------
// UOLCShipModuleData — DataAsset defining a ship module config
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCShipModuleData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UOLCShipModuleData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Human-readable module name. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	FText DisplayName;

	/** Module category. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	EOLCShipModuleCategory ModuleCategory = EOLCShipModuleCategory::Drives;

	/** TIR requirement to install this module. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	int32 TIRTier = 1;

	/** Hull slots consumed by this module. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	int32 HullSlotsRequired = 1;

	/** Power consumption per tick (negative = produces power). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	float PowerConsumption = 0.0f;

	/** Description of what this module does. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	FText EffectDescription;

	/** Repair cost in resources (for damaged modules). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Ship")
	TArray<FOLCResourceAmount> RepairCost;

	/** Static method for default class. */
	static TSubclassOf<UOLCShipModuleData> GetDefault();
};
