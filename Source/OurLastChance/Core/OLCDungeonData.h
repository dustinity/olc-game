#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OLCResourceTypes.h"
#include "OLCRaceFamily.h"
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
// Dungeon loot rarity tiers — maps to WP-130 rarity distribution
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCDungeonRarityTier : uint8
{
	Common       UMETA(DisplayName = "Common"),     // 60% distribution
	Uncommon     UMETA(DisplayName = "Uncommon"),   // 25% distribution
	Rare         UMETA(DisplayName = "Rare"),       // 10% distribution
	Epic         UMETA(DisplayName = "Epic"),       // 4% distribution
	Legendary    UMETA(DisplayName = "Legendary"),   // 1% distribution
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	EOLCDungeonRarityTier RarityTier = EOLCDungeonRarityTier::Common;

	FOLCDungeonReward() {}

	FOLCDungeonReward(const FText& InName, EOLCResourceType InType, int32 Min, int32 Max, float Chance = 1.0f, EOLCDungeonRarityTier InTier = EOLCDungeonRarityTier::Common)
		: RewardName(InName), ResourceType(InType), MinQuantity(Min), MaxQuantity(Max), DropChance(Chance), RarityTier(InTier) {}
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

	/** WP-130: preferred race family for concrete enemy composition (falls back to EnemyType label when Unknown or the race subsystem is unavailable). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	EOLCRaceFamily PreferredFamily = EOLCRaceFamily::Unknown;

	FOLCDungeonEnemy() {}

	FOLCDungeonEnemy(const FText& InType, int32 Min, int32 Max)
		: EnemyType(InType), MinCount(Min), MaxCount(Max) {}

	FOLCDungeonEnemy(const FText& InType, int32 Min, int32 Max, EOLCRaceFamily InFamily)
		: EnemyType(InType), MinCount(Min), MaxCount(Max), PreferredFamily(InFamily) {}
};

// ---------------------------------------------------------------------------
// FOLCDungeonBossPhase — defines a boss phase with HP threshold and ability set
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCDungeonBossPhase
{
	GENERATED_BODY()

	/** HP threshold percentage at which this phase activates (e.g., 70 means activate when HP <= 70%). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Boss")
	float HPThreshold = 0.0f;

	/** Human-readable name for this phase. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Boss")
	FText PhaseName;

	/** Whether this is the final enraged phase. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Boss")
	bool bIsEnragedPhase = false;

	FOLCDungeonBossPhase() : HPThreshold(0.0f), bIsEnragedPhase(false) {}

	FOLCDungeonBossPhase(float InThreshold, FText InName, bool bEnraged = false)
		: HPThreshold(InThreshold), PhaseName(MoveTemp(InName)), bIsEnragedPhase(bEnraged) {}
};

// ---------------------------------------------------------------------------
// EOLCDungeonContentCategory — the canonical respawn bucket a rule applies to.
// Supersedes FOLCDungeonRespawnRule::ContentType (which is semantically
// mistyped as EOLCDungeonType); that field is kept for compatibility but
// ContentCategory is authoritative going forward.
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCDungeonContentCategory : uint8
{
	Enemies    UMETA(DisplayName = "Enemies"),
	Materials  UMETA(DisplayName = "Materials"),
	Bosses     UMETA(DisplayName = "Bosses"),
	Blueprints UMETA(DisplayName = "Blueprints"),
};

// ---------------------------------------------------------------------------
// FOLCDungeonRespawnRule — respawn timing per dungeon content type
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCDungeonRespawnRule
{
	GENERATED_BODY()

	/** Content type for this respawn rule (legacy/mistyped field — kept for compatibility). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Respawn")
	EOLCDungeonType ContentType = EOLCDungeonType::AbandonedHouse;

	/** Canonical content bucket this rule applies to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Respawn")
	EOLCDungeonContentCategory ContentCategory = EOLCDungeonContentCategory::Enemies;

	/** Respawn time in hours. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Respawn")
	float RespawnHours = 48.0f;

	/** False for content that never respawns once claimed (e.g. blueprints). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Respawn")
	bool bRespawns = true;

	FOLCDungeonRespawnRule() : RespawnHours(48.0f) {}

	FOLCDungeonRespawnRule(EOLCDungeonType InType, float InHours)
		: ContentType(InType), RespawnHours(InHours) {}

	FOLCDungeonRespawnRule(EOLCDungeonContentCategory InCategory, float InHours, bool bInRespawns = true)
		: ContentCategory(InCategory), RespawnHours(InHours), bRespawns(bInRespawns) {}
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

	/** Layout seed for deterministic, guaranteed-reachable room/corridor arrangement. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	int32 LayoutSeed = 0;

	/** Boss phase thresholds at 70%, 40%, and 15% HP with race-specific behavior. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Boss")
	TArray<FOLCDungeonBossPhase> BossPhaseThresholds;

	/** Persistent respawn rules: enemies 48h, materials 72h, bosses 168h, blueprints once. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Respawn")
	TArray<FOLCDungeonRespawnRule> RespawnRules;

	/** WP-118 RaceId of this dungeon's boss (empty when bHasBoss == false). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Boss")
	FString BossRaceId;

	/** Blueprint-only rewards, granted at most once per archetype (bIsBlueprint == true on every entry). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Dungeon")
	TArray<FOLCDungeonReward> BlueprintRewards;

	/** Static method for default class. */
	static TSubclassOf<UOLCDungeonData> GetDefault();

	/** Recommended minimum squad size for this size tier (WP-130 size-scaled squads). */
	static int32 GetRecommendedSquadMin(EOLCDungeonSize Size);

	/** Recommended maximum squad size for this size tier (WP-130 size-scaled squads). */
	static int32 GetRecommendedSquadMax(EOLCDungeonSize Size);

	/** Canonical respawn rules: Enemies 48h, Materials 72h, Bosses 168h, Blueprints (never respawn). */
	static TArray<FOLCDungeonRespawnRule> GetDefaultRespawnRules();
};
