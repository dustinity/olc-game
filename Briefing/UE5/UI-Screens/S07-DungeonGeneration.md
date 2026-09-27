# Dungeon Generation & Loot Tables — Agent Skill

## What to Create

### Purpose

This skill covers the **procedural generation of dungeons** — creating room layouts, placing enemies and loot, scaling difficulty by TIR and biome, managing boss fight phases, and handling post-combat loot distribution. It's the backend system that Tactical Combat (S08) reads from; the combat UI handles the fighting, but this skill defines what you're fighting in and where everything comes from.

Dungeons are the primary source of equipment drops, resource accumulation, and unit progression. Each dungeon run is a self-contained instance with unique layout, enemies, and loot tables.

### Assets Created Per Dungeon Type

| Asset Type | Quantity | Purpose |
|-----------|----------|---------|
| DataAsset | 1 per dungeon type | Room templates, enemy composition rules, loot pools |
| DataTable rows | ~7 (one per dungeon type) + loot tier entries | Dungeon definitions with size tiers and quantity ranges |

### Data Tables Updated

| Table | What Gets Added/Modified |
|-------|-------------------------|
| `DG_Dungeons` | New dungeon type row: room counts, enemy ranges, loot tables by size tier |
| `DG_LootTables` | Loot entries with weighted random rolls per rarity tier |

## MCP Workflow: Step by Step

### Phase 1 — Inspect & Prepare

1. **Inspect existing data table schemas** for DG_Dungeons and any existing loot tables
   - Toolset: `data-table-tools`
2. **Create directory structure**: `/Game/Gameplay/Dungeons/` and `/Game/Data/Dungeons/`
   - Toolset: `asset-tools`, `editor-toolset`

### Phase 2 — Create Dungeon Type DataAssets

1. **Create `BP_DungeonDefinition` DataAsset** for each of the 7 dungeon types:
   - Toolset: `data-asset-tools`, `blueprint-tools`
   - Properties per dungeon type (Abandoned House, Enemy Camp, Scavenged Vehicle, Military Outpost, Hospital Complex, Alien Structure, Inner Ring Citadel):
     - `DungeonTypeID` (Name) — unique identifier
     - `DungeonName` (String) — display name
     - `RoomCountMin/Max` (int) — total rooms in the dungeon layout
     - `EnemyCountMin/Max` (struct: per size tier Tiny/Small/Medium/Large/Fortress)
     - `SizeTiers` (enum array: Tiny, Small, Medium, Large, Fortress) — each type has different available sizes
     - `BiomeFilter` (GameplayTag) — which biomes this dungeon can spawn in
     - `TIRMin/TIRMax` (int 1-5) — colony TIR range for safe entry
     - `LootTableRef` (Name) — reference to DG_LootTables row for resource drops
     - `EquipmentDropPool` (array of struct: ItemCategory [Weapon/Armor/Utility], RarityWeights [Common/Uncommon/Rare/Epic/Legendary as float percentages])
     - `BossPresent` (bool) — true for Large/Fortress sizes, or always for Alien Structure and Citadel
     - `BossHPMultiplier` (float) — boss HP scales with dungeon size tier
     - `BossPhases` (int) — number of phase transitions (typically 4: at 70%, 40%, 15% HP thresholds)
     - `RespawnTimers` (struct: EnemyHours=48, MaterialHours=72, BossHours=168, BlueprintNever=true)

2. **Create all 7 dungeon DataAssets** with appropriate values matching the briefing specs

**Toolset:** `data-asset-tools`, `blueprint-tools`

### Phase 3 — Create Loot Table DataTable

1. **Create or update DG_LootTables DataTable**:
   - Toolset: `data-table-tools`
   - Columns:
     - `LootTierID` (Name) — e.g., "loot.tier.small.common"
     - `DungeonSize` (Enum: Tiny/Small/Medium/Large/Fortress)
     - `RarityRoll` (Enum: Common/Uncommon/Rare/Epic/Legendary)
     - `Resource_Energy_Min/Max` (int) — quantity range for energy resource drop
     - `Resource_ConstructionMineral_Min/Max` (int)
     - `Resource_Fuel_Min/Max` (int)
     - `Resource_Minerals_Min/Max` (int)
     - `Resource_Survival_Min/Max` (int)
     - `Resource_HullParts_Min/Max` (int)
     - `BlueprintChance` (float 0-1) — probability of blueprint drop (never for bosses per briefing)

2. **Populate with loot entries** covering all combinations of dungeon size × rarity tier
   - Common: 60% weight, Small resource quantities
   - Uncommon: 25% weight, Medium resource quantities
   - Rare: 10% weight, Large resource quantities
   - Epic: 4% weight, Very large quantities + blueprint chance 5%
   - Legendary: 1% weight, Maximum quantities + blueprint chance 15%

**Toolset:** `data-table-tools`

### Phase 4 — Create Dungeon Generator Blueprint

1. **Create `BP_DungeonGenerator`**:
   - Variables:
     - `CurrentDungeon` (struct: DungeonTypeID, SizeTier, RoomLayout [array of struct], EnemyPositions [array], LootLocations [array])
     - `PlayerProgress` (struct: RoomsCleared, EnemiesDefeated, CurrentRoomIndex)
   - Functions:
     - `GenerateDungeon(dungeonTypeID, sizeTier, biomeType, colonyTIR)` → DungeonInstance DataAsset
       - Creates a room-by-room layout with enemy placements and loot locations
       - Room count randomized within the dungeon type's Min/Max range
       - Enemy composition scaled by TIR difficulty modifier (1.0x at TIR 1 → 2.0x at TIR 5)
       - Boss placed in final room if BossPresent = true for this size tier

     - `GetRoomAt(index)` → RoomDefinition — returns room data for the current dungeon instance
     - `ClearRoom(roomIndex)` → LootDropResult — processes loot when player clears a room
     - `CalculateTIRDifficultyModifier(colonyTIR, dungeonTIRMax)` → float — enemy stat multiplier based on TIR gap

2. **Create `BP_RoomDefinition` struct** (stored in DataAsset):
   - Properties: RoomIndex, RoomType [Corridor/Combat/Storage/Boss/Checkpoint], Dimensions (Width, Depth), EnemyCount (int), HasLoot (bool), LootRarityRoll (enum), ExitDirections [North/South/East/West array]

**Toolset:** `blueprint-tools`, `data-asset-tools`

### Phase 5 — Create Dungeon Manager Blueprint

1. **Create `BP_DungeonManager`**:
   - Variables:
     - `ActiveDungeons` (array of struct: DungeonInstanceID, DungeonTypeID, SizeTier, Status [Available/InProgress/Cleared/Respawning], EntryCooldown)
     - `CurrentRun` (struct: InstanceID, PlayerSquad [array of UnitIDs], CurrentRoomIndex, HealthRemaining, ResourcesCollected)
   - Functions:
     - `ScanForDungeons(planetPosition, scanRange)` → array of DungeonEntry — reveals dungeon silhouettes within radar range
       - Silhouette reveal system: unscanned dungeons show only a vague outline; scanning reveals type and approximate size
       - Scan cost: energy + fuel deducted from colony resources

     - `EnterDungeon(dungeonID, squadIDs)` → bool — validates squad has enough units (squad size scales with dungeon size), deducts entry resource cost
     - `ClearRoom(roomIndex)` → LootDropResult — processes room clearing, spawns enemies if Combat type, awards loot from Storage type
     - `FightBoss(bossHP, currentPhase)` → BossPhaseResult — handles boss phase transitions at 70%/40%/15% HP thresholds
     - `ExitDungeon(retreat)` → struct (resourcesCollected, unitsDamaged, blueprintFound) — processes end of run
     - `UpdateRespawnTimers(deltaTime)` — decrements respawn timers for cleared dungeons

2. **Boss Phase Logic**:
   - Phase 1 (HP > 70%): Standard attack patterns, baseline stats
   - Phase 2 (HP 40-70%): Adds secondary ability, +25% damage
   - Phase 3 (HP 15-40%): Summons minions (if applicable), +50% damage, faster attack speed
   - Phase 4 (HP < 15%): Enraged state — all stats ×1.5, new attack pattern, desperate last stand

**Toolset:** `blueprint-tools`

### Phase 6 — Create Loot Processing System

1. **Create `BP_LootProcessor`**:
   - Functions:
     - `RollLoot(dungeonSize, rarityTier)` → struct (Resources array, BlueprintChance)
       - Reads from DG_LootTables using dungeon size + rarity as lookup key
       - Returns random quantities within the Min/Max ranges for each resource type
       - Rolls blueprint chance separately (only Epic/Legendary tiers have non-zero chance)

     - `RollEquipmentDrop(itemCategory, rarityWeights)` → ItemID
       - Uses weighted random roll based on dungeon-type-specific rarity distribution
       - Faction-specific equipment drops at Alien Structure and Citadel tiers
       - Returns the specific item ID that was rolled

     - `ProcessCombatResults(combatResult)` → LootDistribution struct
       - Called after tactical combat ends (connects to Combat Results screen S10)
       - Combines room loot + boss loot + equipment drops into final distribution
       - Triggers EquipSwapDialog if any dropped item matches an equipped slot

2. **Create `BP_LootDistribution` DataAsset**:
   - Properties: ResourceDrops (struct array), EquipmentDrops (array of ItemID), BlueprintDrops (array of Name), TotalValueEstimated (int) — sum of all loot values for display in Combat Results UI

**Toolset:** `blueprint-tools`, `data-asset-tools`

### Phase 7 — Create Enemy Composition System

1. **Create `BP_EnemyCompositionDefinition` DataAsset**:
   - Properties: DungeonTypeID, BiomeFilter, TIRMin/TIRMax, EnemyGroups (array of struct: RaceFamily [Insectoid/Reptilian/Molluskoid/Humanoid/Crystalloid], UnitCountMin/Max, SpecialAbilities [array: SwarmAttack/Spawn/Camouflage/Teleportation/Regeneration/ElementalResistance])
   - Function: `GetEnemyGroups(dungeonSize)` → array — scales unit counts based on dungeon size tier

2. **Create `BP_EnemySpawner` Blueprint**:
   - Functions:
     - `SpawnEnemiesInRoom(roomIndex, compositionDef)` → array of Actor references — places enemy actors in the room's grid positions
     - `GetRaceDistributionByRing(ringDistance)` → array of RaceFamily with weights — outer ring favors Insectoid/Humanoid, inner ring favors Crystalloid, center has all families plus alien hybrids

**Toolset:** `blueprint-tools`, `data-asset-tools`

### Phase 8 — Wire Into Existing Systems

1. **Connect to Squad Selection (S07/S09)**:
   - Dungeon size determines minimum squad size requirement
   - Squad must have role balance (Tank + Support) before entering
   - Units take damage during dungeon runs; damaged units appear with reduced HP in the results screen

2. **Connect to Tactical Combat (S08)**:
   - The combat UI reads room layout and enemy positions from the DungeonGenerator's CurrentDungeon struct
   - Enemy stats are multiplied by TIR difficulty modifier before combat begins
   - Boss fights use the 4-phase state machine defined in BP_DungeonManager

3. **Connect to Combat Results (S10)**:
   - LootProcessor.ProcessCombatResults() feeds data into the results screen's loot distribution display
   - Equipment drops trigger the EquipSwapDialog if applicable

4. **Connect to Colony Management (S02)**:
   - Collected resources are added to colony inventory after dungeon exit
   - Respawn timers affect when dungeons become available again for future runs

**Toolset:** `blueprint-tools`

### Phase 9 — Compile & Verify

1. **Compile every Blueprint** after creation/modification
2. **Test generation**:
   - Generate a dungeon of each type (7 types × 3 size tiers) → verify room counts are within Min/Max ranges
   - Verify enemy composition respects TIRMin/TIRMax constraints
   - Check loot table rolls produce quantities within the defined Min/Max ranges
   - Test boss phase transitions: simulate HP dropping through all 4 phases, verify stat changes apply correctly
3. **Test in PIE**:
   - Scan for dungeons → verify silhouette reveal system works (unscanned = vague outline, scanned = type revealed)
   - Enter a dungeon with a valid squad → verify room layout loads and enemies spawn at correct positions
   - Clear rooms → verify loot drops appear in results
   - Fight a boss → verify phase transitions trigger at correct HP thresholds (70%, 40%, 15%)
   - Exit dungeon → verify resources are added to colony inventory and respawn timers start

## Key Patterns & Gotchas

### Respawn Timers are Absolute, Not Relative
Respawn timers use absolute game time, not relative to when the dungeon was cleared:
- **Enemies**: 48 hours — room resets with fresh enemies (same composition)
- **Materials**: 72 hours — storage rooms reset with new loot
- **Bosses**: 168 hours (1 week) — boss room resets for another run
- **Blueprints**: Never respawn — once a blueprint is found, it's gone from that dungeon

This means players can plan runs around known respawn schedules. The Colony Management UI should display countdown timers for each active dungeon.

### Squad Size Scales with Dungeon Size
The minimum squad size requirement scales with the dungeon's size tier:
- **Tiny**: 2-4 units (minimum 1 Tank + 1 Support)
- **Small**: 4-6 units
- **Medium**: 6-8 units
- **Large**: 8-12 units
- **Fortress**: 12-16 units (maximum squad size)

The Squad Selection screen should validate this before allowing entry. If the player's roster doesn't have enough units, they can't enter that dungeon size.

### TIR Difficulty Scaling is Exponential, Not Linear
Enemy stat multipliers scale from 1.0x at TIR 1 to 2.0x at TIR 5:
- TIR 1: 1.0x (baseline)
- TIR 2: 1.2x
- TIR 3: 1.4x
- TIR 4: 1.7x
- TIR 5: 2.0x

This means a TIR 5 colony fighting in an inner-ring dungeon faces enemies with double the base HP and damage of a TIR 1 colony. The Tactical Combat UI should display this multiplier so players understand why higher-TIR dungeons feel harder despite their units being stronger.

### Boss Blueprint Drops are Guaranteed at Fortress Size
While regular blueprint drops have a small chance (5% Epic, 15% Legendary), **Fortress-size dungeons and Alien Structure/Citadel types guarantee at least one blueprint drop**. This is the primary source of new equipment blueprints for players to craft more gear.

### Room Types Have Specific Behaviors
- **Corridor**: No enemies, no loot — just passage between rooms (may have environmental hazards)
- **Combat**: Enemies spawn here; clearing requires defeating all enemies
- **Storage**: Loot appears after clearing adjacent Combat rooms; no enemies
- **Boss**: Final room; contains the boss encounter with 4-phase mechanics
- **Checkpoint**: Safe room where player HP is partially restored between sections

The dungeon layout generator must ensure at least one of each type (except Checkpoint which is optional) and that Corridors connect all other rooms in a navigable path.

## File Structure Summary

```
/Game/Gameplay/Dungeons/
├── BP_DungeonGenerator.uasset (Blueprint — procedural room/enemy placement)
├── BP_DungeonManager.uasset (Blueprint — dungeon lifecycle, boss phases, respawn timers)
├── BP_LootProcessor.uasset (Blueprint — loot rolls, equipment drops, combat results processing)
└── BP_EnemySpawner.uasset (Blueprint — enemy composition, race distribution by ring)

/Game/Data/Dungeons/
├── BP_DungeonDefinition_AbandonedHouse.uasset (and 6 other dungeon type DataAssets)
├── BP_EnemyCompositionDefinition.uasset (DataAsset — encounter templates per dungeon/biome)
└── DG_LootTables.uasset (DataTable — loot quantity ranges by size tier × rarity)

/Game/Data/
├── DG_Dungeons.uasset (DataTable — dungeon type definitions with metadata)
└── DG_Races.uasset (DataTable — 30 hostile sub-race definitions referenced by composition system)
```

## Next Steps After This Skill

Dungeon Generation connects to:
- **Squad Selection (S07/S09)** — validates squad size and role balance before dungeon entry; shows dungeon preview with estimated difficulty
- **Tactical Combat (S08)** — reads room layout, enemy positions, and boss phase data from the DungeonGenerator during combat
- **Combat Results (S10)** — receives loot distribution data from LootProcessor for post-combat display
- **Unit Upgrade + Equipment** — equipment drops from dungeons are processed here; new blueprints enable crafting more gear
- **Colony Management (S02)** — collected resources added to colony inventory; respawn timers displayed in the management UI
- **Planet Events** — dungeon discovery uses the same radar scan system as planet events; hostile areas on planets may contain dungeon entrances
