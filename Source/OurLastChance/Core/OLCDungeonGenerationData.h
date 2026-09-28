// WP-130 — procedural dungeon generation, loot, boss encounter, and
// persistence data types, plus the UOLCDungeonStateSubsystem that owns the
// expedition flow (S07 -> S09 -> S08 -> S10) and the native SaveGame slot
// enforcing respawn intervals across sessions.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/OLCDungeonData.h"
#include "Core/OLCEquipmentData.h"
#include "Core/OLCUIDataSubsystem.h" // FOLCSquadDeploymentData
#include "Core/OLCBossRaceData.h" // EOLCBossTier
#include "OLCDungeonGenerationData.generated.h"

class UOLCRaceSubsystem;

// ---------------------------------------------------------------------------
// Room/layout types
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCDungeonRoomType : uint8
{
	Entrance UMETA(DisplayName = "Entrance"),
	Combat   UMETA(DisplayName = "Combat"),
	Loot     UMETA(DisplayName = "Loot"),
	Boss     UMETA(DisplayName = "Boss"),
	Corridor UMETA(DisplayName = "Corridor"),
};

UENUM(BlueprintType)
enum class EOLCDungeonHazardType : uint8
{
	None           UMETA(DisplayName = "None"),
	Geyser         UMETA(DisplayName = "Geyser"),
	Gully          UMETA(DisplayName = "Gully"),
	RadiationField UMETA(DisplayName = "Radiation Field"),
	CollapseZone   UMETA(DisplayName = "Collapse Zone"),
};

USTRUCT(BlueprintType)
struct FOLCDungeonHazard
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	EOLCDungeonHazardType Type = EOLCDungeonHazardType::None;

	/** Damage per second while a unit stands in the hazard. */
	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	float Intensity = 1.0f;

	/** Offset within the room's tile block. */
	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	int32 LocalX = 0;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	int32 LocalY = 0;
};

USTRUCT(BlueprintType)
struct FOLCDungeonRoom
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	int32 RoomId = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	EOLCDungeonRoomType Type = EOLCDungeonRoomType::Combat;

	/** Top-left tile on the global grid. */
	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	int32 GridX = 0;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	int32 GridY = 0;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	int32 WidthTiles = 1;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	int32 HeightTiles = 1;

	/** Adjacent RoomIds (rooms are connected directly, or via a Corridor room). */
	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	TArray<int32> Connections;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	TArray<FOLCDungeonHazard> Hazards;

	/** BFS depth from the entrance room, filled post-generation. */
	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	int32 DepthFromEntrance = 0;

	/** WP-118 RaceIds resolved for this room's enemy group (empty when falling back to the text label). */
	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	TArray<FString> EnemyRaceIds;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	FText EnemyGroupLabel;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	int32 EnemyCount = 0;

	/** Center of this room's tile block, in tile-space (for world/canvas projection). */
	FVector2D GetCenterTile() const
	{
		return FVector2D(GridX + WidthTiles * 0.5f, GridY + HeightTiles * 0.5f);
	}
};

USTRUCT(BlueprintType)
struct FOLCDungeonGenerationResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	int32 Seed = 0;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	EOLCDungeonType DungeonType = EOLCDungeonType::AbandonedHouse;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	EOLCDungeonSize SizeTier = EOLCDungeonSize::Small;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	TArray<FOLCDungeonRoom> Rooms;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	int32 EntranceRoomId = INDEX_NONE;

	/** INDEX_NONE when the dungeon has no boss. */
	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	int32 BossRoomId = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	bool bValid = false;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	FText ValidationReport;

	const FOLCDungeonRoom* FindRoom(int32 RoomId) const
	{
		return Rooms.FindByPredicate([RoomId](const FOLCDungeonRoom& R) { return R.RoomId == RoomId; });
	}

	TArray<const FOLCDungeonRoom*> GetContentRooms() const
	{
		TArray<const FOLCDungeonRoom*> Out;
		for (const FOLCDungeonRoom& R : Rooms)
		{
			if (R.Type != EOLCDungeonRoomType::Corridor)
			{
				Out.Add(&R);
			}
		}
		return Out;
	}

	int32 NumCorridors() const
	{
		int32 Count = 0;
		for (const FOLCDungeonRoom& R : Rooms)
		{
			if (R.Type == EOLCDungeonRoomType::Corridor)
			{
				Count++;
			}
		}
		return Count;
	}
};

// ---------------------------------------------------------------------------
// Loot rolls
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCDungeonLootRoll
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	FOLCDungeonReward Reward;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	EOLCDungeonRarityTier Rarity = EOLCDungeonRarityTier::Common;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	int32 Quantity = 0;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	bool bDropped = false;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	bool bIsUnique = false;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	FText UniqueItemName;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	TWeakObjectPtr<UOLCEquipmentData> EquipmentTemplate;

	/** INDEX_NONE = completion reward (not tied to a specific loot room). */
	UPROPERTY(BlueprintReadOnly, Category = "OLC|Dungeon")
	int32 SourceRoomId = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct FOLCDungeonBossEncounter
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Boss")
	FString BossRaceId;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Boss")
	EOLCBossTier Tier = EOLCBossTier::Standard;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Boss")
	int32 RoomId = INDEX_NONE;
};

// ---------------------------------------------------------------------------
// Persistent per-archetype state (WP-130: 48h/72h/168h respawn, one-time blueprints)
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCDungeonInstanceState
{
	GENERATED_BODY()

	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "OLC|Dungeon")
	EOLCDungeonType DungeonType = EOLCDungeonType::AbandonedHouse;

	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "OLC|Dungeon")
	int32 LayoutSeed = 0;

	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "OLC|Dungeon")
	bool bEverCleared = false;

	/** Real UTC unix seconds of the most recent clear (no in-game clock exists in this codebase). */
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "OLC|Dungeon")
	double ClearedAtUnixSeconds = 0.0;

	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "OLC|Dungeon")
	double NextEnemyRespawnUnix = 0.0;

	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "OLC|Dungeon")
	double NextMaterialRespawnUnix = 0.0;

	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "OLC|Dungeon")
	double NextBossRespawnUnix = 0.0;

	/** Never resets once true — blueprints are obtainable once per archetype. */
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "OLC|Dungeon")
	bool bBlueprintClaimed = false;

	/** Feeds the per-clear run seed: Hash(LayoutSeed, ClearCount). */
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "OLC|Dungeon")
	int32 ClearCount = 0;
};

// ---------------------------------------------------------------------------
// UOLCDungeonSaveData — native UE5 SaveGame wrapper, mirrors UOLCTutorialSaveData
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCDungeonSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame)
	TArray<FOLCDungeonInstanceState> DungeonStates;
};

/** Save slot used for dungeon state persistence (mirrors "OLC_TutorialProgress"). */
static const TCHAR* const OLCDungeonSaveSlotName = TEXT("OLC_DungeonState");

// ---------------------------------------------------------------------------
// Pending expedition flow state (S07 -> S09 -> S08)
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCPendingExpedition
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UOLCDungeonData> Dungeon = nullptr;

	UPROPERTY()
	FOLCDungeonGenerationResult Layout;

	UPROPERTY()
	FOLCSquadDeploymentData Squad;

	UPROPERTY()
	int32 RecommendedSquadMin = 0;

	UPROPERTY()
	int32 RecommendedSquadMax = 16;

	UPROPERTY()
	bool bValid = false;
};

// ---------------------------------------------------------------------------
// FDungeonCompletionResult — moved here from UI/OLCCombatResultsWidget.h so
// Core can own it without a Core -> UI include. The results widget keeps an
// include of this header; the struct itself is unchanged aside from the
// UnitXPGains key (FText is not a valid TMap key for UHT reflection, so the
// key is the unit's display name as an FString).
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FDungeonCompletionResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Results")
	bool bVictory = false;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Results")
	TMap<EOLCResourceType, float> ResourcesGained;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Results")
	TArray<FText> BlueprintGrants;

	/** Keyed by the surviving unit's display name. */
	UPROPERTY(BlueprintReadOnly, Category = "OLC|Results")
	TMap<FString, float> UnitXPGains;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Results")
	TArray<FText> Casualties;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Results")
	TArray<FText> ResearchUnlocks;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Results")
	TArray<FOLCDungeonLootRoll> LootRolls;
};

// ---------------------------------------------------------------------------
// UOLCDungeonStateSubsystem — expedition flow + respawn persistence
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCDungeonStateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// -----------------------------------------------------------------------
	// Expedition flow (S07 -> S09 -> S08)
	// -----------------------------------------------------------------------

	/** Generates the layout for Dungeon (deterministic on LayoutSeed) and stores it as the pending expedition. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Dungeon")
	void BeginExpedition(const UOLCDungeonData* Dungeon);

	const FOLCPendingExpedition& GetPendingExpedition() const { return PendingExpedition; }

	/** Called from S09's OnSquadReady — stores the confirmed squad on the pending expedition. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Dungeon")
	void ConfirmSquad(const FOLCSquadDeploymentData& Squad);

	/** Called by S08 init: returns and clears the pending expedition. False if none is pending. */
	bool ConsumePendingExpedition(FOLCPendingExpedition& OutExpedition);

	// -----------------------------------------------------------------------
	// Respawn / persistence
	// -----------------------------------------------------------------------

	const FOLCDungeonInstanceState* GetInstanceState(EOLCDungeonType Type) const;

	/** Stamps clear timers + blueprint claim (on victory) and persists to the save slot. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Dungeon")
	void RecordClear(const UOLCDungeonData* Dungeon, bool bVictory, bool bBlueprintGranted);

	UFUNCTION(BlueprintPure, Category = "OLC|Dungeon")
	bool AreEnemiesRespawned(EOLCDungeonType Type, double NowUnixSeconds) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Dungeon")
	bool AreMaterialsRespawned(EOLCDungeonType Type, double NowUnixSeconds) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Dungeon")
	bool IsBossAvailable(EOLCDungeonType Type, double NowUnixSeconds) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Dungeon")
	bool HasBlueprintClaimed(EOLCDungeonType Type) const;

	/** Pure timer resolution — no wall-clock access, so it is directly testable at exact boundaries. */
	static FOLCDungeonInstanceState ComputeRespawnState(const FOLCDungeonInstanceState& In, const TArray<FOLCDungeonRespawnRule>& Rules, double NowUnix);

	void SaveState();
	void LoadState();

	void SetLastResult(const FDungeonCompletionResult& Result) { LastResult = Result; bHasLastResult = true; }
	const FDungeonCompletionResult& GetLastResult() const { return LastResult; }

	/** False until SetLastResult has been called at least once (distinguishes "no dungeon run yet" from a recorded defeat). */
	bool HasLastResult() const { return bHasLastResult; }

	/** One-shot consume, mirroring ConsumePendingExpedition — avoids S10 reusing a stale result from an earlier dungeon run. */
	bool ConsumeLastResult(FDungeonCompletionResult& OutResult)
	{
		if (!bHasLastResult) return false;
		OutResult = LastResult;
		bHasLastResult = false;
		return true;
	}

private:
	FOLCDungeonInstanceState& FindOrAddState(EOLCDungeonType Type);

	UPROPERTY()
	TMap<int32, FOLCDungeonInstanceState> InstanceStates; // keyed by (int32)EOLCDungeonType

	UPROPERTY()
	FOLCPendingExpedition PendingExpedition;

	UPROPERTY()
	FDungeonCompletionResult LastResult;

	bool bHasLastResult = false;
};
