#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OLCResourceTypes.h"
#include "OLCDungeonData.generated.h"

// ---------------------------------------------------------------------------
// Dungeon difficulty tiers — maps to TIR recommendations from Briefing
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCDungeonDifficulty : uint8
{
	Easy       UMETA(DisplayName = "Easy"),      // TIR 1
	Moderate   UMETA(DisplayName = "Moderate"),  // TIR 1-2
	Hard       UMETA(DisplayName = "Hard"),      // TIR 2-3
	VeryHard   UMETA(DisplayName = "Very Hard"), // TIR 3+
	Extreme    UMETA(DisplayName = "Extreme"),   // TIR 4+
};

// ---------------------------------------------------------------------------
// Dungeon size classification (from Briefing/Dungeon-Overview)
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCDungeonSize : uint8
{
	Tiny     UMETA(DisplayName = "Tiny"),      // 1-2 rooms, Scavenged Vehicle
	Small    UMETA(DisplayName = "Small"),     // 3-6 rooms, Abandoned House
	Medium   UMETA(DisplayName = "Medium"),    // 7-15 rooms, Enemy Camp / Military Outpost
	Large    UMETA(DisplayName = "Large"),     // 16-30 rooms, Hospital Complex / Alien Structure
	Fortress UMETA(DisplayName = "Fortress"),  // 30+ rooms, Inner Ring Citadel
};

// ---------------------------------------------------------------------------
// Dungeon type — matches Briefing/Dungeon-Types/README.md
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCDungeonType : uint8
{
	AbandonedHouse     UMETA(DisplayName = "Abandoned House"),
	EnemyCamp          UMETA(DisplayName = "Enemy Camp"),
	ScavengedVehicle   UMETA(DisplayName = "Scavenged Vehicle"),
	MilitaryOutpost    UMETA(DisplayName = "Military Outpost"),
	HospitalComplex    UMETA(DisplayName = "Hospital Complex"),
	AlienStructure     UMETA(DisplayName = "Alien Structure"),
	InnerRingCitadel   UMETA(DisplayName = "Inner Ring Citadel"),
};

// ---------------------------------------------------------------------------
// FOLCDungeonReward — loot item dropped on dungeon completion
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCDungeonReward
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	FText RewardName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	EOLCResourceType ResourceType = EOLCResourceType::ConstructionMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	int32 MinQuantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	int32 MaxQuantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	float DropChance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	bool bIsBlueprint = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	FText BlueprintName;

	FOLCDungeonReward() {}

	FOLCDungeonReward(const FText& InName, EOLCResourceType InType, int32 Min, int32 Max, float Chance = 1.0f)
		: RewardName(InName), ResourceType(InType), MinQuantity(Min), MaxQuantity(Max), DropChance(Chance) {}
};

// ---------------------------------------------------------------------------
// FOLCDungeonEnemy — enemy composition for a dungeon type
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCDungeonEnemy
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	FText EnemyType; // e.g. "Wildlife", "Bandits", "Soldiers"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	int32 MinCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	int32 MaxCount = 0;

	FOLCDungeonEnemy() {}

	FOLCDungeonEnemy(const FText& InType, int32 Min, int32 Max)
		: EnemyType(InType), MinCount(Min), MaxCount(Max) {}
};

// ---------------------------------------------------------------------------
// UOLCDungeonData — DataAsset defining a dungeon instance/config
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCDungeonData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UOLCDungeonData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Human-readable name for this dungeon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	FText DisplayName;

	/** Dungeon type from the Briefing catalog. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	EOLCDungeonType DungeonType = EOLCDungeonType::AbandonedHouse;

	/** Size classification. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	EOLCDungeonSize DungeonSize = EOLCDungeonSize::Small;

	/** Recommended TIR difficulty. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	EOLCDungeonDifficulty Difficulty = EOLCDungeonDifficulty::Easy;

	/** Estimated clear time in minutes (from Briefing). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	float EstimatedClearMinutes = 15.0f;

	/** Enemy composition for this dungeon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	TArray<FOLCDungeonEnemy> Enemies;

	/** Loot rewards on completion. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	TArray<FOLCDungeonReward> Rewards;

	/** Whether this dungeon has a boss fight. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	bool bHasBoss = false;

	/** Boss name (if applicable). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	FText BossName;

	/** Number of rooms in the dungeon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	int32 RoomCountMin = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	int32 RoomCountMax = 0;

	/** Static method for default class. */
	static TSubclassOf<UOLCDungeonData> GetDefault();
};
