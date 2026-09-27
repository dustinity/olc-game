#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OLCResourceTypes.generated.h"

// ---------------------------------------------------------------------------
// Resource type enum
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCResourceType : uint8
{
	Energy               UMETA(DisplayName = "Energy"),
	Fuel                 UMETA(DisplayName = "Fuel"),
	ConstructionMaterial UMETA(DisplayName = "Construction Material"),
	Minerals             UMETA(DisplayName = "Minerals"),
	HullParts            UMETA(DisplayName = "Hull Parts"),
	Survival             UMETA(DisplayName = "Survival"),
	DarkMatterCrystals   UMETA(DisplayName = "Dark Matter Crystals"),
};

// ---------------------------------------------------------------------------
// Storage pressure states for resource counters
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCStoragePressure : uint8
{
	Normal       UMETA(DisplayName = "Normal"),
	Approaching  UMETA(DisplayName = "Approaching Full"),
	Full         UMETA(DisplayName = "Full"),
	Overflow     UMETA(DisplayName = "Overflow"),
};

// ---------------------------------------------------------------------------
// Progress / state helpers
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCProgressState : uint8
{
	Idle      UMETA(DisplayName = "Idle"),
	Active    UMETA(DisplayName = "Active"),
	Complete  UMETA(DisplayName = "Complete"),
	Failed    UMETA(DisplayName = "Failed"),
};

UENUM(BlueprintType)
enum class EOLCColorRole : uint8
{
	Default  UMETA(DisplayName = "Default"),
	Primary  UMETA(DisplayName = "Primary / Orange"),
	Secondary UMETA(DisplayName = "Secondary / Blue"),
	Success  UMETA(DisplayName = "Success / Green"),
	Danger   UMETA(DisplayName = "Danger / Red"),
	Warning  UMETA(DisplayName = "Warning / Yellow"),
};

// ---------------------------------------------------------------------------
// Badge types
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCBadgeType : uint8
{
	TIR          UMETA(DisplayName = "TIR"),
	Biome        UMETA(DisplayName = "Biome"),
	Hazard       UMETA(DisplayName = "Hazard"),
	Modifier     UMETA(DisplayName = "Modifier"),
	Status       UMETA(DisplayName = "Status"),
	Faction      UMETA(DisplayName = "Faction"),
	Scan         UMETA(DisplayName = "Scan"),
};

// ---------------------------------------------------------------------------
// Minimap marker types
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCMinimapMarkerType : uint8
{
	Enemy        UMETA(DisplayName = "Enemy"),
	Ally         UMETA(DisplayName = "Ally"),
	ResourceNode UMETA(DisplayName = "Resource Node"),
	Objective    UMETA(DisplayName = "Objective"),
	Ping         UMETA(DisplayName = "Ping"),
	Building     UMETA(DisplayName = "Building"),
};

// ---------------------------------------------------------------------------
// Construction categories — matches Briefing/Buildings/ category folders
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCConstructionCategory : uint8
{
	Power        UMETA(DisplayName = "Power"),          // PB-PG-*  PowerGeneration
	Extraction   UMETA(DisplayName = "Extraction"),     // PB-EX-*  Extraction
	Infrastructure UMETA(DisplayName = "Infrastructure"), // PB-IN-* Infrastructure
	Storage      UMETA(DisplayName = "Storage"),        // PB-ST-*  Storage
	Production   UMETA(DisplayName = "Production"),     // PB-PF-*  Production
	Defense      UMETA(DisplayName = "Defense"),        // PB-DS-*, PB-SD-*  Defense
	Support      UMETA(DisplayName = "Support"),        // PB-SB-*  Support
	HighTier     UMETA(DisplayName = "High Tier"),      // PB-HP-*  HighTier (endgame)
	Special      UMETA(DisplayName = "Special"),        // PB-SP-*  Special buildings
};

// ---------------------------------------------------------------------------
// Simulation speed
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCSimulationSpeed : uint8
{
	Paused UMETA(DisplayName = "Paused"),
	Normal UMETA(DisplayName = "1x"),
	Fast   UMETA(DisplayName = "2x"),
};

// ---------------------------------------------------------------------------
// FOLCResourceAmount — one resource value with capacity
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCResourceAmount
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	EOLCResourceType ResourceType = EOLCResourceType::Energy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	float CurrentValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	float Capacity = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	float Delta = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	EOLCStoragePressure PressureState = EOLCStoragePressure::Normal;

	FOLCResourceAmount() {}

	FOLCResourceAmount(EOLCResourceType InType, float InCurrent, float InCapacity, float InDelta = 0.0f)
		: ResourceType(InType), CurrentValue(InCurrent), Capacity(InCapacity), Delta(InDelta)
	{
		if (CurrentValue >= Capacity)
			PressureState = EOLCStoragePressure::Full;
		else if (CurrentValue >= Capacity * 0.85f)
			PressureState = EOLCStoragePressure::Approaching;
		else
			PressureState = EOLCStoragePressure::Normal;
	}
};

// ---------------------------------------------------------------------------
// FOLCResourceCounterViewData — display data for one resource counter widget
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCResourceCounterViewData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	EOLCResourceType ResourceType = EOLCResourceType::Energy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	float Value = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	float Capacity = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	float Delta = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	EOLCStoragePressure PressureState = EOLCStoragePressure::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText TooltipText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	bool bVisible = true;

	FOLCResourceCounterViewData() {}

	FOLCResourceCounterViewData(EOLCResourceType InType, float InValue, float InCapacity, float InDelta = 0.0f)
		: ResourceType(InType), Value(InValue), Capacity(InCapacity), Delta(InDelta)
	{
		switch (InType)
		{
			case EOLCResourceType::Energy: DisplayName = FText::FromString(TEXT("ENERGY")); break;
			case EOLCResourceType::Fuel: DisplayName = FText::FromString(TEXT("FUEL")); break;
			case EOLCResourceType::ConstructionMaterial: DisplayName = FText::FromString(TEXT("CONSTR. MAT.")); break;
			case EOLCResourceType::Minerals: DisplayName = FText::FromString(TEXT("MINERALS")); break;
			case EOLCResourceType::HullParts: DisplayName = FText::FromString(TEXT("HULL PARTS")); break;
			case EOLCResourceType::Survival: DisplayName = FText::FromString(TEXT("SURVIVAL")); break;
			case EOLCResourceType::DarkMatterCrystals: DisplayName = FText::FromString(TEXT("DARK MATTER")); break;
		}

		if (Value >= Capacity)
			PressureState = EOLCStoragePressure::Full;
		else if (Value >= Capacity * 0.85f)
			PressureState = EOLCStoragePressure::Approaching;
		else
			PressureState = EOLCStoragePressure::Normal;

		TooltipText = FText::Format(
			FText::FromString(TEXT("{0}: {1} / {2} ({3}/s)")),
			DisplayName,
			FText::AsNumber(FMath::RoundToInt(Value)),
			FText::AsNumber(FMath::RoundToInt(Capacity)),
			FText::AsNumber(Delta));
	}
};

// ---------------------------------------------------------------------------
// FOLCBadgeViewData — TIR, biome, hazard, faction, scan, status chips
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCBadgeViewData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	EOLCBadgeType BadgeType = EOLCBadgeType::TIR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText Label;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText TooltipText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	EOLCColorRole ColorRole = EOLCColorRole::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	bool bVisible = true;

	FOLCBadgeViewData() {}

	FOLCBadgeViewData(EOLCBadgeType InType, const FText& InLabel, const FText& InTooltip = FText(), EOLCColorRole InColor = EOLCColorRole::Default)
		: BadgeType(InType), Label(InLabel), TooltipText(InTooltip), ColorRole(InColor) {}
};

// ---------------------------------------------------------------------------
// FOLCActionViewData — label, icon, enabled, tooltip, command id
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCActionViewData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText ActionLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText TooltipText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	bool bEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	int32 CommandId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText IconName;

	FOLCActionViewData() {}

	FOLCActionViewData(const FText& InLabel, const FText& InTooltip, bool InEnabled = true, int32 InId = 0)
		: ActionLabel(InLabel), TooltipText(InTooltip), bEnabled(InEnabled), CommandId(InId) {}
};

// ---------------------------------------------------------------------------
// FOLCProgressViewData — label, current, max, state, color role
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCProgressViewData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText Label;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	float CurrentValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	float MaxValue = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	EOLCProgressState State = EOLCProgressState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	EOLCColorRole ColorRole = EOLCColorRole::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText TooltipText;

	FOLCProgressViewData() {}

	FOLCProgressViewData(const FText& InLabel, float InCurrent, float InMax, EOLCProgressState InState = EOLCProgressState::Active, EOLCColorRole InColor = EOLCColorRole::Default)
		: Label(InLabel), CurrentValue(InCurrent), MaxValue(InMax), State(InState), ColorRole(InColor)
	{
		if (InState == EOLCProgressState::Complete)
			TooltipText = FText::Format(FText::FromString(TEXT("{0}: Complete")), Label);
		else
			TooltipText = FText::Format(
				FText::FromString(TEXT("{0}: {1} / {2}")),
				Label,
				FText::AsNumber(FMath::RoundToInt(InCurrent)),
				FText::AsNumber(FMath::RoundToInt(InMax)));
	}
};

// ---------------------------------------------------------------------------
// FOLCMinimapMarkerViewData — position, type, color role, tooltip, discovered state
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCMinimapMarkerViewData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FVector2D Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	EOLCMinimapMarkerType MarkerType = EOLCMinimapMarkerType::Ally;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	EOLCColorRole ColorRole = EOLCColorRole::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText TooltipText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	bool bDiscovered = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	bool bVisible = true;

	FOLCMinimapMarkerViewData() {}

	FOLCMinimapMarkerViewData(const FVector2D& InPos, EOLCMinimapMarkerType InType, const FText& InTooltip, EOLCColorRole InColor = EOLCColorRole::Default)
		: Position(InPos), MarkerType(InType), ColorRole(InColor), TooltipText(InTooltip) {}
};

// ---------------------------------------------------------------------------
// FOLCBuildCardViewData — building card data for construction mode
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCBuildCardViewData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText BuildingName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	EOLCConstructionCategory Category = EOLCConstructionCategory::Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	int32 TIRRequirement = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FVector2D GridSize; // in grid cells (e.g. 2x2)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	TArray<FOLCResourceAmount> BuildCost;

	/** Negative value produces power; positive value consumes power. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	float PowerConsumption = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	TArray<FOLCResourceAmount> ExpectedOutputPerTick; // WP-104: production preview for unplaced buildings

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	bool bAvailable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	bool bSelected = false;

	FOLCBuildCardViewData() : GridSize(2.0f, 2.0f) {}
};

// ---------------------------------------------------------------------------
// FOLCMissionObjectiveViewData — mission progress items
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCMissionObjectiveViewData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText ObjectiveName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	float Progress = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	float TargetProgress = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	EOLCProgressState State = EOLCProgressState::Active;

	FOLCMissionObjectiveViewData() {}
};
