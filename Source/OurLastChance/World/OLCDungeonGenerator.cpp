#include "World/OLCDungeonGenerator.h"

#include "Containers/Queue.h"
#include "Core/OLCRaceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DateTime.h"

// ---------------------------------------------------------------------------
// Local helpers
// ---------------------------------------------------------------------------
namespace OLCDungeonGen
{
	static int32 RoomTileSizeForTier(EOLCDungeonSize Size)
	{
		switch (Size)
		{
			case EOLCDungeonSize::Tiny:     return 3;
			case EOLCDungeonSize::Small:    return 4;
			case EOLCDungeonSize::Medium:   return 4;
			case EOLCDungeonSize::Large:    return 5;
			case EOLCDungeonSize::Fortress: return 6;
			default:                        return 3;
		}
	}

	static int32 DifficultyIndex(EOLCDungeonDifficulty Difficulty)
	{
		switch (Difficulty)
		{
			case EOLCDungeonDifficulty::Easy:     return 0;
			case EOLCDungeonDifficulty::Moderate: return 1;
			case EOLCDungeonDifficulty::Hard:     return 2;
			case EOLCDungeonDifficulty::VeryHard: return 3;
			case EOLCDungeonDifficulty::Extreme:  return 4;
			default:                              return 0;
		}
	}

	static void LootRoomRangeForTier(EOLCDungeonSize Size, int32& OutMin, int32& OutMax)
	{
		switch (Size)
		{
			case EOLCDungeonSize::Tiny:     OutMin = 0; OutMax = 1; break;
			case EOLCDungeonSize::Small:    OutMin = 1; OutMax = 2; break;
			case EOLCDungeonSize::Medium:   OutMin = 2; OutMax = 4; break;
			case EOLCDungeonSize::Large:    OutMin = 3; OutMax = 6; break;
			case EOLCDungeonSize::Fortress: OutMin = 5; OutMax = 8; break;
			default:                        OutMin = 0; OutMax = 1; break;
		}
	}
}

// ---------------------------------------------------------------------------
// Layout generation
// ---------------------------------------------------------------------------
FOLCDungeonGenerationResult UOLCDungeonGenerator::GenerateDungeon(const UOLCDungeonData* Def, int32 SeedOverride)
{
	using namespace OLCDungeonGen;

	FOLCDungeonGenerationResult Result;
	if (!Def)
	{
		Result.bValid = false;
		Result.ValidationReport = FText::FromString(TEXT("No dungeon definition provided"));
		return Result;
	}

	const int32 Seed = SeedOverride >= 0 ? SeedOverride : Def->LayoutSeed;
	Result.Seed = Seed;
	Result.DungeonType = Def->DungeonType;
	Result.SizeTier = Def->DungeonSize;

	FRandomStream Rng(Seed);

	const int32 RoomSize = RoomTileSizeForTier(Def->DungeonSize);
	const int32 MinRooms = FMath::Max(1, Def->RoomCountMin);
	const int32 MaxRooms = FMath::Max(MinRooms, Def->RoomCountMax);
	int32 TargetContentRooms = Rng.RandRange(MinRooms, MaxRooms);
	if (Def->bHasBoss)
	{
		TargetContentRooms = FMath::Max(TargetContentRooms, 2);
	}

	TSet<FIntPoint> Occupancy;
	int32 NextRoomId = 0;

	auto OccupyRect = [&Occupancy](int32 X, int32 Y, int32 W, int32 H)
	{
		for (int32 dx = 0; dx < W; dx++)
			for (int32 dy = 0; dy < H; dy++)
				Occupancy.Add(FIntPoint(X + dx, Y + dy));
	};
	auto IsRectFree = [&Occupancy](int32 X, int32 Y, int32 W, int32 H) -> bool
	{
		for (int32 dx = 0; dx < W; dx++)
			for (int32 dy = 0; dy < H; dy++)
				if (Occupancy.Contains(FIntPoint(X + dx, Y + dy))) return false;
		return true;
	};

	FOLCDungeonRoom Entrance;
	Entrance.RoomId = NextRoomId++;
	Entrance.Type = EOLCDungeonRoomType::Entrance;
	Entrance.GridX = 0; Entrance.GridY = 0;
	Entrance.WidthTiles = RoomSize; Entrance.HeightTiles = RoomSize;
	OccupyRect(0, 0, RoomSize, RoomSize);
	Result.Rooms.Add(Entrance);
	Result.EntranceRoomId = Entrance.RoomId;

	// Cardinal directions: East, South, West, North.
	const int32 DirOffsets[4][2] = { {1,0}, {0,1}, {-1,0}, {0,-1} };

	int32 ContentRoomsPlaced = 1;
	const int32 MaxAttemptsPerRoom = 400;

	while (ContentRoomsPlaced < TargetContentRooms)
	{
		bool bPlaced = false;
		for (int32 Attempt = 0; Attempt < MaxAttemptsPerRoom && !bPlaced; Attempt++)
		{
			TArray<int32> ContentIndices;
			for (int32 i = 0; i < Result.Rooms.Num(); i++)
			{
				if (Result.Rooms[i].Type != EOLCDungeonRoomType::Corridor)
				{
					ContentIndices.Add(i);
				}
			}
			const int32 ParentIdx = ContentIndices[Rng.RandRange(0, ContentIndices.Num() - 1)];
			const FOLCDungeonRoom Parent = Result.Rooms[ParentIdx]; // copy: Result.Rooms may reallocate below

			const int32 DirStart = Rng.RandRange(0, 3);
			for (int32 d = 0; d < 4 && !bPlaced; d++)
			{
				const int32 Dir = (DirStart + d) % 4;
				const int32 Gap = Rng.RandRange(1, 2);
				const int32 DX = DirOffsets[Dir][0];
				const int32 DY = DirOffsets[Dir][1];

				int32 NewX, NewY, CorX, CorY, CorW, CorH;
				if (DX != 0)
				{
					NewX = Parent.GridX + (DX > 0 ? Parent.WidthTiles + Gap : -(RoomSize + Gap));
					NewY = Parent.GridY;
					CorX = DX > 0 ? Parent.GridX + Parent.WidthTiles : NewX + RoomSize;
					CorY = Parent.GridY;
					CorW = Gap;
					CorH = RoomSize;
				}
				else
				{
					NewX = Parent.GridX;
					NewY = Parent.GridY + (DY > 0 ? Parent.HeightTiles + Gap : -(RoomSize + Gap));
					CorX = Parent.GridX;
					CorY = DY > 0 ? Parent.GridY + Parent.HeightTiles : NewY + RoomSize;
					CorW = RoomSize;
					CorH = Gap;
				}

				if (!IsRectFree(NewX, NewY, RoomSize, RoomSize)) continue;
				if (!IsRectFree(CorX, CorY, CorW, CorH)) continue;

				FOLCDungeonRoom NewRoom;
				NewRoom.RoomId = NextRoomId++;
				NewRoom.Type = EOLCDungeonRoomType::Combat;
				NewRoom.GridX = NewX; NewRoom.GridY = NewY;
				NewRoom.WidthTiles = RoomSize; NewRoom.HeightTiles = RoomSize;
				OccupyRect(NewX, NewY, RoomSize, RoomSize);

				FOLCDungeonRoom Corridor;
				Corridor.RoomId = NextRoomId++;
				Corridor.Type = EOLCDungeonRoomType::Corridor;
				Corridor.GridX = CorX; Corridor.GridY = CorY;
				Corridor.WidthTiles = CorW; Corridor.HeightTiles = CorH;
				OccupyRect(CorX, CorY, CorW, CorH);

				Corridor.Connections.Add(Parent.RoomId);
				Corridor.Connections.Add(NewRoom.RoomId);
				NewRoom.Connections.Add(Corridor.RoomId);

				Result.Rooms[ParentIdx].Connections.Add(Corridor.RoomId);
				Result.Rooms.Add(NewRoom);
				Result.Rooms.Add(Corridor);

				ContentRoomsPlaced++;
				bPlaced = true;
			}
		}
		if (!bPlaced)
		{
			// Unbounded grid: this is only reachable if something is structurally wrong.
			break;
		}
	}

	// BFS depth from entrance over the full graph (content rooms + corridors).
	{
		TMap<int32, int32> RoomIndexById;
		for (int32 i = 0; i < Result.Rooms.Num(); i++)
		{
			RoomIndexById.Add(Result.Rooms[i].RoomId, i);
		}

		TQueue<int32> Queue;
		TSet<int32> Visited;
		Result.Rooms[RoomIndexById[Result.EntranceRoomId]].DepthFromEntrance = 0;
		Queue.Enqueue(Result.EntranceRoomId);
		Visited.Add(Result.EntranceRoomId);

		while (!Queue.IsEmpty())
		{
			int32 CurrentId = INDEX_NONE;
			Queue.Dequeue(CurrentId);
			const int32 CurrentIdx = RoomIndexById[CurrentId];
			const int32 CurrentDepth = Result.Rooms[CurrentIdx].DepthFromEntrance;

			for (int32 NeighborId : Result.Rooms[CurrentIdx].Connections)
			{
				if (Visited.Contains(NeighborId)) continue;
				Visited.Add(NeighborId);
				Result.Rooms[RoomIndexById[NeighborId]].DepthFromEntrance = CurrentDepth + 1;
				Queue.Enqueue(NeighborId);
			}
		}
	}

	// Boss room: deepest non-corridor, non-entrance room (tie-break: lowest RoomId).
	if (Def->bHasBoss)
	{
		int32 BestId = INDEX_NONE;
		int32 BestDepth = -1;
		for (const FOLCDungeonRoom& R : Result.Rooms)
		{
			if (R.Type == EOLCDungeonRoomType::Corridor || R.Type == EOLCDungeonRoomType::Entrance) continue;
			if (R.DepthFromEntrance > BestDepth || (R.DepthFromEntrance == BestDepth && (BestId == INDEX_NONE || R.RoomId < BestId)))
			{
				BestDepth = R.DepthFromEntrance;
				BestId = R.RoomId;
			}
		}
		if (BestId != INDEX_NONE)
		{
			Result.BossRoomId = BestId;
			for (FOLCDungeonRoom& R : Result.Rooms)
			{
				if (R.RoomId == BestId)
				{
					R.Type = EOLCDungeonRoomType::Boss;
					break;
				}
			}
		}
	}

	// Loot rooms: distinct eligible rooms with 0 < depth < maxDepth.
	{
		int32 MaxDepth = 0;
		for (const FOLCDungeonRoom& R : Result.Rooms)
		{
			MaxDepth = FMath::Max(MaxDepth, R.DepthFromEntrance);
		}

		TArray<int32> Eligible;
		for (const FOLCDungeonRoom& R : Result.Rooms)
		{
			if (R.Type == EOLCDungeonRoomType::Combat && R.DepthFromEntrance > 0 && R.DepthFromEntrance < MaxDepth)
			{
				Eligible.Add(R.RoomId);
			}
		}

		int32 LMin, LMax;
		LootRoomRangeForTier(Def->DungeonSize, LMin, LMax);
		const int32 L = FMath::Min(Rng.RandRange(LMin, LMax), Eligible.Num());

		for (int32 i = 0; i < L; i++)
		{
			const int32 SwapIdx = Rng.RandRange(i, Eligible.Num() - 1);
			Eligible.Swap(i, SwapIdx);
		}
		for (int32 i = 0; i < L; i++)
		{
			for (FOLCDungeonRoom& R : Result.Rooms)
			{
				if (R.RoomId == Eligible[i])
				{
					R.Type = EOLCDungeonRoomType::Loot;
					break;
				}
			}
		}
	}

	// Hazards: non-entrance, non-boss, non-corridor rooms have a difficulty-scaled chance of one hazard.
	{
		const int32 DiffIdx = DifficultyIndex(Def->Difficulty);
		const float HazardChance = 0.15f + 0.05f * DiffIdx;
		for (FOLCDungeonRoom& R : Result.Rooms)
		{
			if (R.Type == EOLCDungeonRoomType::Entrance || R.Type == EOLCDungeonRoomType::Boss || R.Type == EOLCDungeonRoomType::Corridor) continue;
			if (Rng.FRand() < HazardChance)
			{
				FOLCDungeonHazard Hazard;
				const int32 TypeRoll = Rng.RandRange(0, 3); // Geyser, Gully, RadiationField, CollapseZone
				Hazard.Type = static_cast<EOLCDungeonHazardType>(TypeRoll + 1);
				Hazard.Intensity = 1.0f + DiffIdx * 0.5f;
				Hazard.LocalX = Rng.RandRange(0, FMath::Max(0, R.WidthTiles - 1));
				Hazard.LocalY = Rng.RandRange(0, FMath::Max(0, R.HeightTiles - 1));
				R.Hazards.Add(Hazard);
			}
		}
	}

	Result.bValid = ValidateGeneration(Result, Def, Result.ValidationReport);
	return Result;
}

bool UOLCDungeonGenerator::ValidateGeneration(const FOLCDungeonGenerationResult& Result, const UOLCDungeonData* Def, FText& OutReport)
{
	if (!Def)
	{
		OutReport = FText::FromString(TEXT("No dungeon definition"));
		return false;
	}

	int32 EntranceCount = 0;
	for (const FOLCDungeonRoom& R : Result.Rooms)
	{
		if (R.Type == EOLCDungeonRoomType::Entrance) EntranceCount++;
	}
	if (EntranceCount != 1)
	{
		OutReport = FText::Format(FText::FromString(TEXT("Expected exactly one entrance room, found {0}")), FText::AsNumber(EntranceCount));
		return false;
	}

	// Reachability: BFS from the entrance must visit every room.
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
	for (const FOLCDungeonRoom& R : Result.Rooms)
	{
		if (!Visited.Contains(R.RoomId))
		{
			OutReport = FText::Format(FText::FromString(TEXT("Room {0} is unreachable from the entrance")), FText::AsNumber(R.RoomId));
			return false;
		}
	}

	if (Def->bHasBoss)
	{
		const FOLCDungeonRoom* Boss = Result.FindRoom(Result.BossRoomId);
		if (!Boss || Boss->Type != EOLCDungeonRoomType::Boss)
		{
			OutReport = FText::FromString(TEXT("Dungeon has a boss but no boss room was placed"));
			return false;
		}
		if (Boss->RoomId == Result.EntranceRoomId)
		{
			OutReport = FText::FromString(TEXT("Boss room cannot be the entrance"));
			return false;
		}
		int32 MaxDepth = -1;
		for (const FOLCDungeonRoom& R : Result.Rooms)
		{
			if (R.Type == EOLCDungeonRoomType::Corridor) continue;
			MaxDepth = FMath::Max(MaxDepth, R.DepthFromEntrance);
		}
		if (Boss->DepthFromEntrance != MaxDepth)
		{
			OutReport = FText::FromString(TEXT("Boss room is not at maximum traversal depth"));
			return false;
		}
	}

	const int32 ContentCount = Result.GetContentRooms().Num();
	if (ContentCount < Def->RoomCountMin)
	{
		OutReport = FText::Format(FText::FromString(TEXT("Content room count {0} is below the minimum {1}")), FText::AsNumber(ContentCount), FText::AsNumber(Def->RoomCountMin));
		return false;
	}

	// No overlaps.
	TMap<FIntPoint, int32> Occ;
	for (const FOLCDungeonRoom& R : Result.Rooms)
	{
		for (int32 dx = 0; dx < R.WidthTiles; dx++)
		{
			for (int32 dy = 0; dy < R.HeightTiles; dy++)
			{
				const FIntPoint P(R.GridX + dx, R.GridY + dy);
				if (const int32* Existing = Occ.Find(P))
				{
					OutReport = FText::Format(FText::FromString(TEXT("Rooms {0} and {1} overlap")), FText::AsNumber(*Existing), FText::AsNumber(R.RoomId));
					return false;
				}
				Occ.Add(P, R.RoomId);
			}
		}
	}

	OutReport = FText::FromString(TEXT("OK"));
	return true;
}

void UOLCDungeonGenerator::PopulateEnemies(FOLCDungeonGenerationResult& Result, const UOLCDungeonData* Def, FRandomStream& Rng, UOLCRaceSubsystem* Races)
{
	if (!Def) return;

	TArray<int32> CombatRoomIds;
	for (FOLCDungeonRoom& R : Result.Rooms)
	{
		if (R.Type == EOLCDungeonRoomType::Combat)
		{
			R.EnemyCount = 0;
			R.EnemyRaceIds.Reset();
			R.EnemyGroupLabel = FText::GetEmpty();
			CombatRoomIds.Add(R.RoomId);
		}
	}
	if (CombatRoomIds.Num() == 0) return;

	CombatRoomIds.Sort([&Result](int32 A, int32 B)
	{
		const FOLCDungeonRoom* RA = Result.FindRoom(A);
		const FOLCDungeonRoom* RB = Result.FindRoom(B);
		return (RA ? RA->DepthFromEntrance : 0) > (RB ? RB->DepthFromEntrance : 0);
	});

	for (const FOLCDungeonEnemy& Entry : Def->Enemies)
	{
		const int32 Budget = Rng.RandRange(Entry.MinCount, Entry.MaxCount);
		if (Budget <= 0) continue;

		TArray<int32> Weights;
		int32 TotalWeight = 0;
		for (int32 RoomId : CombatRoomIds)
		{
			const FOLCDungeonRoom* Room = Result.FindRoom(RoomId);
			const int32 W = (Room ? Room->DepthFromEntrance : 0) + 1;
			Weights.Add(W);
			TotalWeight += W;
		}

		TArray<UOLCRaceData*> Pool;
		if (Races && Entry.PreferredFamily != EOLCRaceFamily::Unknown)
		{
			for (UOLCRaceData* Race : Races->GetRacesByFamily(Entry.PreferredFamily))
			{
				if (Race && !Cast<UOLCBossRaceData>(Race))
				{
					Pool.Add(Race);
				}
			}
		}

		int32 Remaining = Budget;
		for (int32 i = 0; i < CombatRoomIds.Num() && Remaining > 0; i++)
		{
			const int32 Share = (i == CombatRoomIds.Num() - 1)
				? Remaining
				: FMath::Min(Remaining, FMath::RoundToInt(static_cast<float>(Budget) * Weights[i] / static_cast<float>(FMath::Max(1, TotalWeight))));
			if (Share <= 0) continue;
			Remaining -= Share;

			for (FOLCDungeonRoom& R : Result.Rooms)
			{
				if (R.RoomId != CombatRoomIds[i]) continue;
				R.EnemyCount += Share;
				if (R.EnemyGroupLabel.IsEmpty())
				{
					R.EnemyGroupLabel = Entry.EnemyType;
				}
				for (int32 n = 0; n < Share && Pool.Num() > 0; n++)
				{
					R.EnemyRaceIds.Add(Pool[Rng.RandRange(0, Pool.Num() - 1)]->RaceId);
				}
				break;
			}
		}
	}
}

// ---------------------------------------------------------------------------
// Loot
// ---------------------------------------------------------------------------
EOLCDungeonRarityTier UOLCDungeonGenerator::RollRarity(FRandomStream& Rng)
{
	const float R = Rng.FRandRange(0.0f, 100.0f);
	if (R < 60.0f) return EOLCDungeonRarityTier::Common;
	if (R < 85.0f) return EOLCDungeonRarityTier::Uncommon;
	if (R < 95.0f) return EOLCDungeonRarityTier::Rare;
	if (R < 99.0f) return EOLCDungeonRarityTier::Epic;
	return EOLCDungeonRarityTier::Legendary;
}

EOLCEquipmentRarity UOLCDungeonGenerator::ToEquipmentRarity(EOLCDungeonRarityTier Tier)
{
	switch (Tier)
	{
		case EOLCDungeonRarityTier::Common:    return EOLCEquipmentRarity::Common;
		case EOLCDungeonRarityTier::Uncommon:  return EOLCEquipmentRarity::Uncommon;
		case EOLCDungeonRarityTier::Rare:      return EOLCEquipmentRarity::Rare;
		case EOLCDungeonRarityTier::Epic:      return EOLCEquipmentRarity::Epic;
		case EOLCDungeonRarityTier::Legendary: return EOLCEquipmentRarity::Legendary;
		default:                               return EOLCEquipmentRarity::Common;
	}
}

TArray<FOLCDungeonLootRoll> UOLCDungeonGenerator::RollDungeonLoot(const UOLCDungeonData* Def, const FOLCDungeonGenerationResult& Layout, FRandomStream& Rng, bool bBlueprintClaimed, UObject* Outer)
{
	TArray<FOLCDungeonLootRoll> Rolls;
	if (!Def) return Rolls;

	static const TCHAR* Adjectives[] = { TEXT("Ancient"), TEXT("Void-Touched"), TEXT("Reinforced"), TEXT("Blazing"), TEXT("Silent") };
	static const TCHAR* Nouns[] = { TEXT("Vanguard"), TEXT("Warden"), TEXT("Reaper"), TEXT("Aegis"), TEXT("Oracle") };

	TArray<int32> RollSourceRoomIds;
	for (const FOLCDungeonRoom& R : Layout.Rooms)
	{
		if (R.Type == EOLCDungeonRoomType::Loot)
		{
			RollSourceRoomIds.Add(R.RoomId);
		}
	}
	RollSourceRoomIds.Add(INDEX_NONE); // completion roll

	for (int32 SourceRoomId : RollSourceRoomIds)
	{
		for (const FOLCDungeonReward& RewardEntry : Def->Rewards)
		{
			FOLCDungeonLootRoll Roll;
			Roll.Reward = RewardEntry;
			Roll.SourceRoomId = SourceRoomId;
			Roll.Rarity = RewardEntry.RarityTier;
			Roll.bDropped = Rng.FRand() < RewardEntry.DropChance;
			Roll.Quantity = Roll.bDropped ? Rng.RandRange(RewardEntry.MinQuantity, RewardEntry.MaxQuantity) : 0;
			Rolls.Add(Roll);
		}

		FOLCDungeonLootRoll EquipRoll;
		EquipRoll.SourceRoomId = SourceRoomId;
		EquipRoll.bDropped = true;
		EquipRoll.Rarity = RollRarity(Rng);

		TArray<UOLCEquipmentData*> Candidates = UOLCEquipmentData::GetEquipmentByRarity(Outer, ToEquipmentRarity(EquipRoll.Rarity));
		if (Candidates.Num() > 0)
		{
			UOLCEquipmentData* Chosen = Candidates[Rng.RandRange(0, Candidates.Num() - 1)];
			EquipRoll.EquipmentTemplate = Chosen;
			EquipRoll.Reward.RewardName = Chosen->DisplayName;
		}

		bool bUnique = false;
		switch (EquipRoll.Rarity)
		{
			case EOLCDungeonRarityTier::Rare:      bUnique = Rng.FRand() < 0.2f; break;
			case EOLCDungeonRarityTier::Epic:      bUnique = true; break;
			case EOLCDungeonRarityTier::Legendary: bUnique = true; break;
			default:                               bUnique = false; break;
		}
		EquipRoll.bIsUnique = bUnique;
		if (bUnique)
		{
			const FString Adj = Adjectives[Rng.RandRange(0, UE_ARRAY_COUNT(Adjectives) - 1)];
			const FString Noun = Nouns[Rng.RandRange(0, UE_ARRAY_COUNT(Nouns) - 1)];
			const FString BaseName = EquipRoll.EquipmentTemplate.IsValid() ? EquipRoll.EquipmentTemplate->DisplayName.ToString() : TEXT("Artifact");
			EquipRoll.UniqueItemName = FText::FromString(FString::Printf(TEXT("%s %s %s"), *Adj, *Noun, *BaseName));
		}
		Rolls.Add(EquipRoll);
	}

	// Fortress blueprint guarantee: first unclaimed clear only. Select the first entry actually
	// flagged bIsBlueprint (BlueprintRewards is not guaranteed to contain only blueprint entries).
	if (Def->DungeonSize == EOLCDungeonSize::Fortress && !bBlueprintClaimed)
	{
		const FOLCDungeonReward* BlueprintEntry = nullptr;
		for (const FOLCDungeonReward& BP : Def->BlueprintRewards)
		{
			if (BP.bIsBlueprint)
			{
				BlueprintEntry = &BP;
				break;
			}
		}

		if (BlueprintEntry)
		{
			FOLCDungeonLootRoll BlueprintRoll;
			BlueprintRoll.SourceRoomId = INDEX_NONE;
			BlueprintRoll.Reward = *BlueprintEntry;
			BlueprintRoll.bDropped = true;
			BlueprintRoll.Quantity = 1;
			BlueprintRoll.Rarity = BlueprintEntry->RarityTier;
			Rolls.Add(BlueprintRoll);
		}
	}

	return Rolls;
}

FOLCDungeonBossEncounter UOLCDungeonGenerator::ResolveBossEncounter(const UOLCDungeonData* Def, const FOLCDungeonGenerationResult& Layout, UOLCRaceSubsystem* Races)
{
	FOLCDungeonBossEncounter Encounter;
	if (!Def || !Def->bHasBoss || Layout.BossRoomId == INDEX_NONE) return Encounter;
	Encounter.BossRaceId = Def->BossRaceId;
	Encounter.RoomId = Layout.BossRoomId;

	if (Races)
	{
		if (UOLCBossRaceData* BossData = Cast<UOLCBossRaceData>(Races->FindRaceById(Def->BossRaceId)))
		{
			Encounter.Tier = BossData->BossTier;
		}
	}
	return Encounter;
}

int32 UOLCDungeonGenerator::ComputeRunSeed(int32 LayoutSeed, int32 ClearCount)
{
	const uint32 Combined = HashCombine(GetTypeHash(static_cast<uint32>(LayoutSeed)), GetTypeHash(static_cast<uint32>(ClearCount)));
	return static_cast<int32>(Combined);
}

// ---------------------------------------------------------------------------
// UOLCDungeonStateSubsystem — expedition flow + respawn persistence
// (declared in Core/OLCDungeonGenerationData.h per the WP-130 file manifest;
// implemented here alongside the generator it depends on.)
// ---------------------------------------------------------------------------
void UOLCDungeonStateSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadState();
}

void UOLCDungeonStateSubsystem::BeginExpedition(const UOLCDungeonData* Dungeon)
{
	PendingExpedition = FOLCPendingExpedition();
	if (!Dungeon) return;

	PendingExpedition.Dungeon = const_cast<UOLCDungeonData*>(Dungeon);
	PendingExpedition.Layout = UOLCDungeonGenerator::GenerateDungeon(Dungeon);
	PendingExpedition.RecommendedSquadMin = UOLCDungeonData::GetRecommendedSquadMin(Dungeon->DungeonSize);
	PendingExpedition.RecommendedSquadMax = UOLCDungeonData::GetRecommendedSquadMax(Dungeon->DungeonSize);
	PendingExpedition.bValid = PendingExpedition.Layout.bValid;
}

void UOLCDungeonStateSubsystem::ConfirmSquad(const FOLCSquadDeploymentData& Squad)
{
	PendingExpedition.Squad = Squad;
}

bool UOLCDungeonStateSubsystem::ConsumePendingExpedition(FOLCPendingExpedition& OutExpedition)
{
	if (!PendingExpedition.bValid || !PendingExpedition.Dungeon)
	{
		return false;
	}
	OutExpedition = PendingExpedition;
	PendingExpedition = FOLCPendingExpedition();
	return true;
}

FOLCDungeonInstanceState& UOLCDungeonStateSubsystem::FindOrAddState(EOLCDungeonType Type)
{
	const int32 Key = static_cast<int32>(Type);
	if (FOLCDungeonInstanceState* Existing = InstanceStates.Find(Key))
	{
		return *Existing;
	}
	FOLCDungeonInstanceState NewState;
	NewState.DungeonType = Type;
	return InstanceStates.Add(Key, NewState);
}

const FOLCDungeonInstanceState* UOLCDungeonStateSubsystem::GetInstanceState(EOLCDungeonType Type) const
{
	return InstanceStates.Find(static_cast<int32>(Type));
}

FOLCDungeonInstanceState UOLCDungeonStateSubsystem::ComputeRespawnState(const FOLCDungeonInstanceState& In, const TArray<FOLCDungeonRespawnRule>& Rules, double NowUnix)
{
	float EnemyHours = 48.0f, MaterialHours = 72.0f, BossHours = 168.0f;
	for (const FOLCDungeonRespawnRule& Rule : Rules)
	{
		switch (Rule.ContentCategory)
		{
			case EOLCDungeonContentCategory::Enemies:   EnemyHours = Rule.RespawnHours; break;
			case EOLCDungeonContentCategory::Materials: MaterialHours = Rule.RespawnHours; break;
			case EOLCDungeonContentCategory::Bosses:    BossHours = Rule.RespawnHours; break;
			default: break;
		}
	}

	// NowUnix here is used as the new clear timestamp: this function is pure (no wall-clock
	// access), so tests can probe exact boundary behavior for arbitrary "clear happened at T" values.
	FOLCDungeonInstanceState Out = In;
	Out.bEverCleared = true;
	Out.ClearedAtUnixSeconds = NowUnix;
	Out.NextEnemyRespawnUnix = NowUnix + EnemyHours * 3600.0;
	Out.NextMaterialRespawnUnix = NowUnix + MaterialHours * 3600.0;
	Out.NextBossRespawnUnix = NowUnix + BossHours * 3600.0;
	Out.ClearCount = In.ClearCount + 1;
	return Out;
}

void UOLCDungeonStateSubsystem::RecordClear(const UOLCDungeonData* Dungeon, bool bVictory, bool bBlueprintGranted)
{
	if (!Dungeon || !bVictory) return;

	FOLCDungeonInstanceState Previous = FindOrAddState(Dungeon->DungeonType);
	Previous.DungeonType = Dungeon->DungeonType;
	Previous.LayoutSeed = Dungeon->LayoutSeed;

	const TArray<FOLCDungeonRespawnRule>& Rules = Dungeon->RespawnRules.Num() > 0 ? Dungeon->RespawnRules : UOLCDungeonData::GetDefaultRespawnRules();
	const double Now = FDateTime::UtcNow().ToUnixTimestamp();

	FOLCDungeonInstanceState NewState = ComputeRespawnState(Previous, Rules, Now);
	NewState.bBlueprintClaimed = Previous.bBlueprintClaimed || bBlueprintGranted;

	InstanceStates.Add(static_cast<int32>(Dungeon->DungeonType), NewState);
	SaveState();
}

bool UOLCDungeonStateSubsystem::AreEnemiesRespawned(EOLCDungeonType Type, double NowUnixSeconds) const
{
	const FOLCDungeonInstanceState* State = GetInstanceState(Type);
	return !State || !State->bEverCleared || NowUnixSeconds >= State->NextEnemyRespawnUnix;
}

bool UOLCDungeonStateSubsystem::AreMaterialsRespawned(EOLCDungeonType Type, double NowUnixSeconds) const
{
	const FOLCDungeonInstanceState* State = GetInstanceState(Type);
	return !State || !State->bEverCleared || NowUnixSeconds >= State->NextMaterialRespawnUnix;
}

bool UOLCDungeonStateSubsystem::IsBossAvailable(EOLCDungeonType Type, double NowUnixSeconds) const
{
	const FOLCDungeonInstanceState* State = GetInstanceState(Type);
	return !State || !State->bEverCleared || NowUnixSeconds >= State->NextBossRespawnUnix;
}

bool UOLCDungeonStateSubsystem::HasBlueprintClaimed(EOLCDungeonType Type) const
{
	const FOLCDungeonInstanceState* State = GetInstanceState(Type);
	return State && State->bBlueprintClaimed;
}

void UOLCDungeonStateSubsystem::SaveState()
{
	UOLCDungeonSaveData* SaveData = Cast<UOLCDungeonSaveData>(UGameplayStatics::CreateSaveGameObject(UOLCDungeonSaveData::StaticClass()));
	if (!SaveData) return;

	InstanceStates.GenerateValueArray(SaveData->DungeonStates);
	UGameplayStatics::SaveGameToSlot(SaveData, OLCDungeonSaveSlotName, 0);
}

void UOLCDungeonStateSubsystem::LoadState()
{
	if (!UGameplayStatics::DoesSaveGameExist(OLCDungeonSaveSlotName, 0)) return;

	UOLCDungeonSaveData* SaveData = Cast<UOLCDungeonSaveData>(UGameplayStatics::LoadGameFromSlot(OLCDungeonSaveSlotName, 0));
	if (!SaveData) return;

	InstanceStates.Reset();
	for (const FOLCDungeonInstanceState& State : SaveData->DungeonStates)
	{
		InstanceStates.Add(static_cast<int32>(State.DungeonType), State);
	}
}
