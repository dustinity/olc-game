// WP-130 — deterministic dungeon layout generation, loot rolls, and boss
// encounter resolution. All randomness flows through a caller-supplied
// FRandomStream so results are reproducible for a fixed seed.

#pragma once

#include "CoreMinimal.h"
#include "Core/OLCDungeonGenerationData.h"
#include "OLCDungeonGenerator.generated.h"

class UOLCRaceSubsystem;

UCLASS()
class OURLASTCHANCE_API UOLCDungeonGenerator : public UObject
{
	GENERATED_BODY()

public:
	/** Deterministic generation. SeedOverride < 0 uses Def->LayoutSeed. */
	static FOLCDungeonGenerationResult GenerateDungeon(const UOLCDungeonData* Def, int32 SeedOverride = -1);

	/** Pure validation: reachability, boss-at-max-depth, room-count range, no grid overlaps, exactly one entrance. */
	static bool ValidateGeneration(const FOLCDungeonGenerationResult& Result, const UOLCDungeonData* Def, FText& OutReport);

	/** Rarity roll on the canonical 60/25/10/4/1 distribution. */
	static EOLCDungeonRarityTier RollRarity(FRandomStream& Rng);

	/** One full loot pass (one call per clear, driven by the per-clear run seed). */
	static TArray<FOLCDungeonLootRoll> RollDungeonLoot(const UOLCDungeonData* Def, const FOLCDungeonGenerationResult& Layout, FRandomStream& Rng, bool bBlueprintClaimed, UObject* Outer);

	/** Dungeon rarity tier -> equipment rarity tier (Alien is not reachable from dungeon loot). */
	static EOLCEquipmentRarity ToEquipmentRarity(EOLCDungeonRarityTier Tier);

	/** Resolves which boss (WP-118 race) sits in which room. Phase behavior itself lives on UOLCBossRaceData. */
	static FOLCDungeonBossEncounter ResolveBossEncounter(const UOLCDungeonData* Def, const FOLCDungeonGenerationResult& Layout, UOLCRaceSubsystem* Races = nullptr);

	/** Run seed for one clear attempt: deterministic function of the stable layout seed and the clear counter. */
	static int32 ComputeRunSeed(int32 LayoutSeed, int32 ClearCount);

	/**
	 * Resolves concrete enemy composition (race ids per room) for one clear attempt, driven by the
	 * caller-supplied Rng (the run seed, not the layout seed — enemy composition varies per clear).
	 * Races may be null, in which case rooms fall back to the free-text EnemyType label only.
	 */
	static void PopulateEnemies(FOLCDungeonGenerationResult& Result, const UOLCDungeonData* Def, FRandomStream& Rng, UOLCRaceSubsystem* Races);
};
