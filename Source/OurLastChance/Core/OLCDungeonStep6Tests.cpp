// WP-130 Step 6 — in-engine verification of procedural dungeon generation,
// loot rolls, boss phase data, and respawn persistence.
//
// Run via the MCP AutomationTestToolset (RunTests with filter "OLC.DungeonStep6").
// Mirrors Core/OLCNavigationStep4Tests.cpp conventions: IMPLEMENT_SIMPLE_AUTOMATION_TEST,
// EAutomationTestFlags::EditorContext | EngineFilter. Pure-function tests need no
// environment; subsystem tests use the standalone-UGameInstance env helper pattern.

#include "Containers/Queue.h"
#include "Core/OLCBossRaceData.h"
#include "Core/OLCDungeonData.h"
#include "Core/OLCDungeonGenerationData.h"
#include "Core/OLCRaceSubsystem.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Misc/AutomationTest.h"
#include "World/OLCDungeonGenerator.h"

#define LOCTEXT_NAMESPACE "OLCDungeonStep6"

namespace OLCDungeonStep6Testing
{
	/** Isolated game-instance environment for subsystem tests. */
	struct FSubsystemEnv
	{
		UGameInstance* GI = nullptr;
		UOLCDungeonStateSubsystem* DungeonState = nullptr;
		UOLCRaceSubsystem* Races = nullptr;

		bool Init(FAutomationTestBase& Test)
		{
			GI = NewObject<UGameInstance>(GEngine);
			GI->AddToRoot();
			GI->InitializeStandalone(FName(TEXT("OLCDungeonStep6Test")));
			DungeonState = GI->GetSubsystem<UOLCDungeonStateSubsystem>();
			Races = GI->GetSubsystem<UOLCRaceSubsystem>();
			if (!DungeonState || !Races)
			{
				Test.AddError(TEXT("Failed to initialize OLC dungeon-state/race subsystems in standalone game instance"));
				return false;
			}
			Races->RegisterStarterRaces();
			return true;
		}

		~FSubsystemEnv()
		{
			if (GI)
			{
				UWorld* World = GI->GetWorld();
				GI->Shutdown();
				GI->RemoveFromRoot();
				GI = nullptr;
				if (World && World->IsValidLowLevel())
				{
					World->DestroyWorld(true);
				}
			}
			DungeonState = nullptr;
			Races = nullptr;
		}
	};

	/** Builds one archetype definition matching UOLCDungeonEntryWidget::InitializeDungeons, for
	 * tests that don't need a live widget/world (pure generation/loot logic only). */
	UOLCDungeonData* MakeArchetype(UObject* Outer, EOLCDungeonType Type, EOLCDungeonSize Size, EOLCDungeonDifficulty Difficulty,
		int32 RoomMin, int32 RoomMax, int32 Seed, bool bHasBoss, const FString& BossRaceId)
	{
		UOLCDungeonData* Data = NewObject<UOLCDungeonData>(Outer);
		Data->DungeonType = Type;
		Data->DungeonSize = Size;
		Data->Difficulty = Difficulty;
		Data->RoomCountMin = RoomMin;
		Data->RoomCountMax = RoomMax;
		Data->LayoutSeed = Seed;
		Data->bHasBoss = bHasBoss;
		Data->BossRaceId = BossRaceId;
		Data->Enemies = { FOLCDungeonEnemy(FText::FromString(TEXT("Test Enemies")), 4, 10) };
		Data->Rewards = {
			FOLCDungeonReward(FText::FromString(TEXT("Test Reward")), EOLCResourceType::ConstructionMaterial, 10, 50, 0.9f, EOLCDungeonRarityTier::Common),
		};
		Data->RespawnRules = UOLCDungeonData::GetDefaultRespawnRules();
		if (Size == EOLCDungeonSize::Fortress)
		{
			FOLCDungeonReward Blueprint(FText::FromString(TEXT("Test Blueprint")), EOLCResourceType::ConstructionMaterial, 1, 1, 1.0f, EOLCDungeonRarityTier::Legendary);
			Blueprint.bIsBlueprint = true;
			Blueprint.BlueprintName = FText::FromString(TEXT("Test Blueprint"));
			Data->BlueprintRewards = { Blueprint };
		}
		return Data;
	}

	/** The 7 archetypes with their canonical seeds (mirrors OLCDungeonEntryWidget::InitializeDungeons). */
	TArray<UOLCDungeonData*> MakeAllArchetypes(UObject* Outer)
	{
		return {
			MakeArchetype(Outer, EOLCDungeonType::AbandonedHouse, EOLCDungeonSize::Tiny, EOLCDungeonDifficulty::Easy, 1, 2, 20240101, false, TEXT("")),
			MakeArchetype(Outer, EOLCDungeonType::EnemyCamp, EOLCDungeonSize::Small, EOLCDungeonDifficulty::Moderate, 3, 6, 20240102, true, TEXT("HumanoidEliteCommanders")),
			MakeArchetype(Outer, EOLCDungeonType::ScavengedVehicle, EOLCDungeonSize::Tiny, EOLCDungeonDifficulty::Easy, 1, 2, 20240103, false, TEXT("")),
			MakeArchetype(Outer, EOLCDungeonType::MilitaryOutpost, EOLCDungeonSize::Medium, EOLCDungeonDifficulty::Hard, 7, 12, 20240104, true, TEXT("ReptilianHydraColonies")),
			MakeArchetype(Outer, EOLCDungeonType::HospitalComplex, EOLCDungeonSize::Large, EOLCDungeonDifficulty::Hard, 20, 35, 20240105, true, TEXT("MolluskoidVoidOctopuses")),
			MakeArchetype(Outer, EOLCDungeonType::AlienStructure, EOLCDungeonSize::Large, EOLCDungeonDifficulty::VeryHard, 25, 50, 20240106, true, TEXT("CrystalloidDarkMatterShapers")),
			MakeArchetype(Outer, EOLCDungeonType::InnerRingCitadel, EOLCDungeonSize::Fortress, EOLCDungeonDifficulty::Extreme, 40, 80, 20240107, true, TEXT("InsectoidCrystalHive")),
		};
	}
}

using namespace OLCDungeonStep6Testing;

// ---------------------------------------------------------------------------
// 1. Every generated room is reachable; boss sits at maximum traversal depth
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCDungeonReachabilityTest, "OLC.DungeonStep6.Generate.AllArchetypesAllSeedsReachable", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCDungeonReachabilityTest::RunTest(const FString& Parameters)
{
	UObject* Outer = GetTransientPackage();
	TArray<UOLCDungeonData*> Archetypes = MakeAllArchetypes(Outer);
	TArray<int32> ExtraSeeds = { 1, 2, 3 };

	for (UOLCDungeonData* Def : Archetypes)
	{
		TArray<int32> Seeds = { Def->LayoutSeed };
		Seeds.Append(ExtraSeeds);

		for (int32 Seed : Seeds)
		{
			const FOLCDungeonGenerationResult Result = UOLCDungeonGenerator::GenerateDungeon(Def, Seed);
			TestTrue(FString::Printf(TEXT("archetype %d seed %d is valid: %s"), (int32)Def->DungeonType, Seed, *Result.ValidationReport.ToString()), Result.bValid);

			// BFS reachability re-verified independently of ValidateGeneration.
			TSet<int32> Visited;
			TQueue<int32> Queue;
			Queue.Enqueue(Result.EntranceRoomId);
			Visited.Add(Result.EntranceRoomId);
			while (!Queue.IsEmpty())
			{
				int32 CurrentId = INDEX_NONE;
				Queue.Dequeue(CurrentId);
				const FOLCDungeonRoom* Current = Result.FindRoom(CurrentId);
				if (!Current) continue;
				for (int32 NeighborId : Current->Connections)
				{
					if (!Visited.Contains(NeighborId))
					{
						Visited.Add(NeighborId);
						Queue.Enqueue(NeighborId);
					}
				}
			}
			TestEqual(TEXT("every room visited by BFS"), Visited.Num(), Result.Rooms.Num());

			if (Def->bHasBoss)
			{
				const FOLCDungeonRoom* Boss = Result.FindRoom(Result.BossRoomId);
				TestNotNull(TEXT("boss room exists"), Boss);
				if (Boss)
				{
					int32 MaxDepth = -1;
					for (const FOLCDungeonRoom& R : Result.Rooms)
					{
						if (R.Type == EOLCDungeonRoomType::Corridor) continue;
						MaxDepth = FMath::Max(MaxDepth, R.DepthFromEntrance);
					}
					TestEqual(TEXT("boss room is at maximum traversal depth"), Boss->DepthFromEntrance, MaxDepth);
				}
			}
		}
	}
	return true;
}

// ---------------------------------------------------------------------------
// 2. Same seed -> identical layout; different seed -> layout differs
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCDungeonDeterminismTest, "OLC.DungeonStep6.Generate.DeterministicPerSeed", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCDungeonDeterminismTest::RunTest(const FString& Parameters)
{
	UObject* Outer = GetTransientPackage();
	UOLCDungeonData* Def = MakeArchetype(Outer, EOLCDungeonType::MilitaryOutpost, EOLCDungeonSize::Medium, EOLCDungeonDifficulty::Hard, 7, 12, 20240104, true, TEXT("ReptilianHydraColonies"));

	const FOLCDungeonGenerationResult A = UOLCDungeonGenerator::GenerateDungeon(Def, 555);
	const FOLCDungeonGenerationResult B = UOLCDungeonGenerator::GenerateDungeon(Def, 555);

	TestEqual(TEXT("same seed: identical room count"), A.Rooms.Num(), B.Rooms.Num());
	bool bIdentical = A.Rooms.Num() == B.Rooms.Num();
	for (int32 i = 0; bIdentical && i < A.Rooms.Num(); i++)
	{
		bIdentical = A.Rooms[i].RoomId == B.Rooms[i].RoomId
			&& A.Rooms[i].Type == B.Rooms[i].Type
			&& A.Rooms[i].GridX == B.Rooms[i].GridX
			&& A.Rooms[i].GridY == B.Rooms[i].GridY
			&& A.Rooms[i].DepthFromEntrance == B.Rooms[i].DepthFromEntrance
			&& A.Rooms[i].Connections == B.Rooms[i].Connections;
	}
	TestTrue(TEXT("same seed reproduces identical layout"), bIdentical);
	TestEqual(TEXT("same seed reproduces boss room"), A.BossRoomId, B.BossRoomId);

	const FOLCDungeonGenerationResult C = UOLCDungeonGenerator::GenerateDungeon(Def, 556);
	bool bDiffers = A.Rooms.Num() != C.Rooms.Num();
	for (int32 i = 0; !bDiffers && i < A.Rooms.Num() && i < C.Rooms.Num(); i++)
	{
		if (A.Rooms[i].GridX != C.Rooms[i].GridX || A.Rooms[i].GridY != C.Rooms[i].GridY)
		{
			bDiffers = true;
		}
	}
	TestTrue(TEXT("different seed yields a different layout"), bDiffers);

	return true;
}

// ---------------------------------------------------------------------------
// 3. Content room count within [Min,Max]; exactly one entrance; loot rooms valid depth
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCDungeonRoomCountTest, "OLC.DungeonStep6.Generate.RoomCountInRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCDungeonRoomCountTest::RunTest(const FString& Parameters)
{
	UObject* Outer = GetTransientPackage();
	TArray<UOLCDungeonData*> Archetypes = MakeAllArchetypes(Outer);

	for (UOLCDungeonData* Def : Archetypes)
	{
		const FOLCDungeonGenerationResult Result = UOLCDungeonGenerator::GenerateDungeon(Def);
		const int32 ContentCount = Result.GetContentRooms().Num();
		TestTrue(FString::Printf(TEXT("archetype %d content room count %d in [%d,%d]"), (int32)Def->DungeonType, ContentCount, Def->RoomCountMin, Def->RoomCountMax),
			ContentCount >= Def->RoomCountMin && ContentCount <= Def->RoomCountMax);

		int32 EntranceCount = 0;
		int32 MaxDepth = 0;
		for (const FOLCDungeonRoom& R : Result.Rooms)
		{
			if (R.Type == EOLCDungeonRoomType::Entrance) EntranceCount++;
			MaxDepth = FMath::Max(MaxDepth, R.DepthFromEntrance);
		}
		TestEqual(TEXT("exactly one entrance"), EntranceCount, 1);

		for (const FOLCDungeonRoom& R : Result.Rooms)
		{
			if (R.Type == EOLCDungeonRoomType::Loot)
			{
				TestTrue(TEXT("loot room depth > 0"), R.DepthFromEntrance > 0);
				TestTrue(TEXT("loot room depth < max depth"), R.DepthFromEntrance < MaxDepth);
			}
		}
	}
	return true;
}

// ---------------------------------------------------------------------------
// 4. Rarity distribution over many rolls matches 60/25/10/4/1 within tolerance
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCLootRarityDistributionTest, "OLC.DungeonStep6.Loot.RarityDistributionFixedSeed", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCLootRarityDistributionTest::RunTest(const FString& Parameters)
{
	FRandomStream Rng(778899);
	const int32 NumRolls = 10000;
	int32 Counts[5] = { 0, 0, 0, 0, 0 };

	for (int32 i = 0; i < NumRolls; i++)
	{
		const EOLCDungeonRarityTier Tier = UOLCDungeonGenerator::RollRarity(Rng);
		Counts[static_cast<int32>(Tier)]++;
	}

	const float ExpectedPct[5] = { 60.0f, 25.0f, 10.0f, 4.0f, 1.0f };
	for (int32 i = 0; i < 5; i++)
	{
		const float ActualPct = 100.0f * Counts[i] / static_cast<float>(NumRolls);
		TestTrue(FString::Printf(TEXT("rarity tier %d: expected ~%.1f%%, got %.2f%%"), i, ExpectedPct[i], ActualPct),
			FMath::Abs(ActualPct - ExpectedPct[i]) <= 3.0f);
	}
	return true;
}

// ---------------------------------------------------------------------------
// 5. Deterministic roll lists; Fortress blueprint guarantee on first clear only
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCLootDeterministicBlueprintTest, "OLC.DungeonStep6.Loot.DeterministicRollsAndBlueprintGuarantee", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCLootDeterministicBlueprintTest::RunTest(const FString& Parameters)
{
	UObject* Outer = GetTransientPackage();
	UOLCDungeonData* Fortress = MakeArchetype(Outer, EOLCDungeonType::InnerRingCitadel, EOLCDungeonSize::Fortress, EOLCDungeonDifficulty::Extreme, 40, 80, 20240107, true, TEXT("InsectoidCrystalHive"));
	const FOLCDungeonGenerationResult Layout = UOLCDungeonGenerator::GenerateDungeon(Fortress);

	FRandomStream RngA(4242);
	const TArray<FOLCDungeonLootRoll> RollsA = UOLCDungeonGenerator::RollDungeonLoot(Fortress, Layout, RngA, /*bBlueprintClaimed=*/false, Outer);
	FRandomStream RngB(4242);
	const TArray<FOLCDungeonLootRoll> RollsB = UOLCDungeonGenerator::RollDungeonLoot(Fortress, Layout, RngB, /*bBlueprintClaimed=*/false, Outer);

	TestEqual(TEXT("same run seed: identical roll count"), RollsA.Num(), RollsB.Num());
	bool bIdentical = RollsA.Num() == RollsB.Num();
	for (int32 i = 0; bIdentical && i < RollsA.Num(); i++)
	{
		bIdentical = RollsA[i].bDropped == RollsB[i].bDropped
			&& RollsA[i].Quantity == RollsB[i].Quantity
			&& RollsA[i].Rarity == RollsB[i].Rarity
			&& RollsA[i].bIsUnique == RollsB[i].bIsUnique;
	}
	TestTrue(TEXT("same run seed reproduces identical roll list"), bIdentical);

	bool bHasBlueprintFirstClear = false;
	for (const FOLCDungeonLootRoll& Roll : RollsA)
	{
		if (Roll.Reward.bIsBlueprint && Roll.bDropped) bHasBlueprintFirstClear = true;
	}
	TestTrue(TEXT("Fortress first clear (unclaimed) grants >=1 blueprint"), bHasBlueprintFirstClear);

	FRandomStream RngClaimed(4242);
	const TArray<FOLCDungeonLootRoll> RollsClaimed = UOLCDungeonGenerator::RollDungeonLoot(Fortress, Layout, RngClaimed, /*bBlueprintClaimed=*/true, Outer);
	bool bHasBlueprintClaimed = false;
	for (const FOLCDungeonLootRoll& Roll : RollsClaimed)
	{
		if (Roll.Reward.bIsBlueprint && Roll.bDropped) bHasBlueprintClaimed = true;
	}
	TestFalse(TEXT("second clear (claimed) grants zero blueprints"), bHasBlueprintClaimed);

	return true;
}

// ---------------------------------------------------------------------------
// 6. Unique-item rules: Epic/Legendary always unique+named, Rare ~20%, Common/Uncommon never
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCLootUniqueRulesTest, "OLC.DungeonStep6.Loot.UniqueRules", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCLootUniqueRulesTest::RunTest(const FString& Parameters)
{
	UObject* Outer = GetTransientPackage();
	UOLCDungeonData* Def = MakeArchetype(Outer, EOLCDungeonType::AlienStructure, EOLCDungeonSize::Large, EOLCDungeonDifficulty::VeryHard, 25, 50, 20240106, true, TEXT("CrystalloidDarkMatterShapers"));
	const FOLCDungeonGenerationResult Layout = UOLCDungeonGenerator::GenerateDungeon(Def);

	int32 RareTotal = 0, RareUnique = 0;
	for (int32 Trial = 0; Trial < 2000; Trial++)
	{
		FRandomStream Rng(9000 + Trial);
		const TArray<FOLCDungeonLootRoll> Rolls = UOLCDungeonGenerator::RollDungeonLoot(Def, Layout, Rng, false, Outer);
		for (const FOLCDungeonLootRoll& Roll : Rolls)
		{
			if (!Roll.EquipmentTemplate.IsValid()) continue; // Only equipment rolls carry a rarity/unique verdict.

			switch (Roll.Rarity)
			{
				case EOLCDungeonRarityTier::Common:
				case EOLCDungeonRarityTier::Uncommon:
					TestFalse(TEXT("Common/Uncommon never unique"), Roll.bIsUnique);
					break;
				case EOLCDungeonRarityTier::Rare:
					RareTotal++;
					if (Roll.bIsUnique) RareUnique++;
					break;
				case EOLCDungeonRarityTier::Epic:
				case EOLCDungeonRarityTier::Legendary:
					TestTrue(TEXT("Epic/Legendary always unique"), Roll.bIsUnique);
					TestTrue(TEXT("Epic/Legendary unique name is non-empty"), !Roll.UniqueItemName.IsEmpty());
					break;
			}
		}
	}

	if (RareTotal > 0)
	{
		const float RareUniquePct = 100.0f * RareUnique / static_cast<float>(RareTotal);
		TestTrue(FString::Printf(TEXT("Rare unique frequency ~20%%, got %.1f%% over %d samples"), RareUniquePct, RareTotal),
			FMath::Abs(RareUniquePct - 20.0f) <= 6.0f);
	}
	return true;
}

// ---------------------------------------------------------------------------
// 7. All 6 WP-118 bosses expose phase thresholds exactly {100,70,40,15}
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCBossPhaseThresholdsTest, "OLC.DungeonStep6.Boss.PhaseThresholdsAllSixRaces", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCBossPhaseThresholdsTest::RunTest(const FString& Parameters)
{
	FSubsystemEnv Env;
	TestTrue(TEXT("standalone environment initialized"), Env.Init(*this));

	TArray<UOLCBossRaceData*> Bosses = Env.Races->GetAllBosses();
	TestTrue(TEXT("at least 6 bosses registered"), Bosses.Num() >= 6);

	const float TestPoints[] = { 100.0f, 75.0f, 70.0f, 45.0f, 40.0f, 20.0f, 15.0f, 10.0f };

	for (UOLCBossRaceData* Boss : Bosses)
	{
		if (!Boss) continue;
		if (Boss->RaceId == TEXT("CenterGalaxyOverlord")) continue; // WP-125 endgame boss, outside WP-130's 6-boss roster.

		TestEqual(FString::Printf(TEXT("%s has 4 phases"), *Boss->RaceId), Boss->Phases.Num(), 4);
		if (Boss->Phases.Num() == 4)
		{
			TestEqual(TEXT("phase thresholds exactly {100,70,40,15}"), Boss->Phases[0].HPThresholdPercent, 100.0f);
			TestEqual(TEXT("phase thresholds exactly {100,70,40,15}"), Boss->Phases[1].HPThresholdPercent, 70.0f);
			TestEqual(TEXT("phase thresholds exactly {100,70,40,15}"), Boss->Phases[2].HPThresholdPercent, 40.0f);
			TestEqual(TEXT("phase thresholds exactly {100,70,40,15}"), Boss->Phases[3].HPThresholdPercent, 15.0f);

			TestTrue(TEXT("damage multipliers strictly increase"),
				Boss->Phases[0].DamageMultiplier < Boss->Phases[1].DamageMultiplier
				&& Boss->Phases[1].DamageMultiplier < Boss->Phases[2].DamageMultiplier
				&& Boss->Phases[2].DamageMultiplier < Boss->Phases[3].DamageMultiplier);
		}

		FOLCBossPhaseData Prev;
		bool bFirst = true;
		for (float HP : TestPoints)
		{
			const FOLCBossPhaseData Phase = Boss->GetPhaseForHPPercent(HP);
			if (!bFirst)
			{
				TestTrue(TEXT("phase threshold non-increasing as HP falls"), Phase.HPThresholdPercent <= Prev.HPThresholdPercent);
			}
			Prev = Phase;
			bFirst = false;
		}
		TestTrue(TEXT("final phase carries a positive enrage timer"), Boss->FinalPhaseEnrageTimerSeconds > 0.0f);
	}
	return true;
}

// ---------------------------------------------------------------------------
// 8. Pure respawn timer math at exact boundaries
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCRespawnTimerBoundaryTest, "OLC.DungeonStep6.Respawn.TimerMathBoundaries", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCRespawnTimerBoundaryTest::RunTest(const FString& Parameters)
{
	const TArray<FOLCDungeonRespawnRule> Rules = UOLCDungeonData::GetDefaultRespawnRules();
	const double ClearT = 1000000.0;

	const FOLCDungeonInstanceState State = UOLCDungeonStateSubsystem::ComputeRespawnState(FOLCDungeonInstanceState(), Rules, ClearT);

	TestEqual(TEXT("enemy respawn at T+48h"), State.NextEnemyRespawnUnix, ClearT + 48.0 * 3600.0);
	TestEqual(TEXT("material respawn at T+72h"), State.NextMaterialRespawnUnix, ClearT + 72.0 * 3600.0);
	TestEqual(TEXT("boss respawn at T+168h"), State.NextBossRespawnUnix, ClearT + 168.0 * 3600.0);

	// Enemies: not respawned at T+48h-1s, respawned at T+48h+1s.
	TestTrue(TEXT("enemies not respawned just before 48h"), (ClearT + 48.0 * 3600.0 - 1.0) < State.NextEnemyRespawnUnix);
	TestTrue(TEXT("enemies respawned just after 48h"), (ClearT + 48.0 * 3600.0 + 1.0) >= State.NextEnemyRespawnUnix);

	// Blueprint claim survives any elapsed time (never reset by ComputeRespawnState).
	FOLCDungeonInstanceState Claimed;
	Claimed.bBlueprintClaimed = true;
	const FOLCDungeonInstanceState AfterClear = UOLCDungeonStateSubsystem::ComputeRespawnState(Claimed, Rules, ClearT + 1e9);
	TestTrue(TEXT("blueprint claim is preserved by ComputeRespawnState"), AfterClear.bBlueprintClaimed);

	return true;
}

// ---------------------------------------------------------------------------
// 9. Save/load round trip preserves timestamps, blueprint claim, clear count
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCRespawnSaveRoundTripTest, "OLC.DungeonStep6.Respawn.SaveRoundTrip", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCRespawnSaveRoundTripTest::RunTest(const FString& Parameters)
{
	UObject* Outer = GetTransientPackage();
	UOLCDungeonData* Def = MakeArchetype(Outer, EOLCDungeonType::InnerRingCitadel, EOLCDungeonSize::Fortress, EOLCDungeonDifficulty::Extreme, 40, 80, 20240107, true, TEXT("InsectoidCrystalHive"));

	FSubsystemEnv Env;
	TestTrue(TEXT("standalone environment initialized"), Env.Init(*this));

	Env.DungeonState->RecordClear(Def, true, true);
	const FOLCDungeonInstanceState* Before = Env.DungeonState->GetInstanceState(Def->DungeonType);
	TestNotNull(TEXT("instance state recorded"), Before);
	if (!Before) return false;

	const double SavedClearedAt = Before->ClearedAtUnixSeconds;
	const double SavedNextBoss = Before->NextBossRespawnUnix;
	const bool SavedClaimed = Before->bBlueprintClaimed;
	const int32 SavedClearCount = Before->ClearCount;

	// Destroy and recreate the subsystem (fresh standalone instance) and load from the same slot.
	FSubsystemEnv Env2;
	TestTrue(TEXT("second standalone environment initialized"), Env2.Init(*this));
	Env2.DungeonState->LoadState();

	const FOLCDungeonInstanceState* After = Env2.DungeonState->GetInstanceState(Def->DungeonType);
	TestNotNull(TEXT("instance state reloaded"), After);
	if (After)
	{
		TestEqual(TEXT("ClearedAt survives round trip"), After->ClearedAtUnixSeconds, SavedClearedAt);
		TestEqual(TEXT("NextBossRespawnUnix survives round trip"), After->NextBossRespawnUnix, SavedNextBoss);
		TestEqual(TEXT("bBlueprintClaimed survives round trip"), After->bBlueprintClaimed, SavedClaimed);
		TestEqual(TEXT("ClearCount survives round trip"), After->ClearCount, SavedClearCount);
	}
	return true;
}

// ---------------------------------------------------------------------------
// 10. Expedition flow: BeginExpedition -> ConfirmSquad -> ConsumePendingExpedition
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCExpeditionFlowTest, "OLC.DungeonStep6.Integration.ExpeditionFlow", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCExpeditionFlowTest::RunTest(const FString& Parameters)
{
	UObject* Outer = GetTransientPackage();
	UOLCDungeonData* Def = MakeArchetype(Outer, EOLCDungeonType::EnemyCamp, EOLCDungeonSize::Small, EOLCDungeonDifficulty::Moderate, 3, 6, 20240102, true, TEXT("HumanoidEliteCommanders"));

	FSubsystemEnv Env;
	TestTrue(TEXT("standalone environment initialized"), Env.Init(*this));

	Env.DungeonState->BeginExpedition(Def);
	TestTrue(TEXT("pending expedition is valid after BeginExpedition"), Env.DungeonState->GetPendingExpedition().bValid);
	TestTrue(TEXT("pending expedition layout is valid"), Env.DungeonState->GetPendingExpedition().Layout.bValid);

	FOLCSquadDeploymentData Squad;
	Squad.UnitIds = { TEXT("TestUnit1"), TEXT("TestUnit2") };
	Squad.bIsValid = true;
	Env.DungeonState->ConfirmSquad(Squad);
	TestEqual(TEXT("squad stored on pending expedition"), Env.DungeonState->GetPendingExpedition().Squad.UnitIds.Num(), 2);

	FOLCPendingExpedition Consumed;
	TestTrue(TEXT("first consume succeeds"), Env.DungeonState->ConsumePendingExpedition(Consumed));
	TestTrue(TEXT("consumed expedition carries the layout"), Consumed.Layout.bValid);
	TestEqual(TEXT("consumed expedition carries the squad"), Consumed.Squad.UnitIds.Num(), 2);

	FOLCPendingExpedition Second;
	TestFalse(TEXT("second consume fails (pending cleared)"), Env.DungeonState->ConsumePendingExpedition(Second));

	return true;
}

#undef LOCTEXT_NAMESPACE
