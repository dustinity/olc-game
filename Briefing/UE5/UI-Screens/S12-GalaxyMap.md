# Solar System Generation — Agent Skill

## What to Create

### Purpose

This skill covers the **procedural generation of solar systems** — creating a star, placing planets in orbital positions, assigning biomes and resources, generating planet events and hostile encounters, and calculating travel costs between worlds. It's the backend system that makes each playthrough feel unique while respecting the game's progression rules (TIR ring difficulty scaling).

The galaxy map UI (S11/S12) displays what this system generates — but the generation logic is entirely separate from the display layer.

### Assets Created Per Solar System

| Asset Type | Quantity | Purpose |
|-----------|----------|---------|
| DataAsset | 1 per solar system | Contains all generated data: star type, planets, events, travel routes |
| DataTable rows | ~5-10 per system | Planet definitions with biome, resources, hostility level |

### Data Tables Updated

| Table | What Gets Added/Modified |
|-------|-------------------------|
| `DG_SolarSystems` | New solar system entry with generated planets and metadata |
| `DG_Planets` | Individual planet rows (or stored inline in the DataAsset) |
| `DG_BiomeModifiers` | Reference to active biome modifiers for this system |

## MCP Workflow: Step by Step

### Phase 1 — Inspect & Prepare

1. **Inspect existing solar systems** (if any) to understand generation conventions
   - Toolset: `editor-toolset`, `asset-tools`
2. **Check DG_BiomeModifiers schema** — need exact column structure for biome definitions
   - Toolset: `data-table-tools`
3. **Create directory**: `/Game/Gameplay/SolarSystem/` and `/Game/Data/SolarSystem/`
   - Toolset: `asset-tools`, `editor-toolset`

### Phase 2 — Create Biome Modifier DataAssets

Each biome has a DataAsset defining its properties, modifiers, and hazards. These are referenced during solar system generation to assign characteristics to each planet.

1. **Create `BP_BiomeDefinition` DataAsset** for each of the 8 biomes:
   - Toolset: `data-asset-tools`, `blueprint-tools`
   - Properties:
     - `BiomeName` (String) — e.g., "Desert", "Ice", "Swamp"
     - `VisualSignature` (String) — description for VFX/material reference
     - `Temperature` (float, Celsius equivalent) — affects unit performance and building efficiency
     - `WaterLevel` (float 0-1) — percentage of surface covered by water
     - `AccessibleEarlyGame` (bool) — whether TIR 1-2 can land here safely
     - `ResourceModifiers` (struct array: Resource type → percentage modifier, e.g., +50% minerals on Rocky)
     - `SecondaryModifiers` (array of struct: ModifierName, EffectType [MovementSpeed/ProductionRate/RadarRange/SurvivalConsumption], Value float as multiplier) — 1-3 random modifiers per planet
     - `EnvironmentalHazards` (struct array: HazardName, TriggerCondition [Continuous/Random/Event-based], DamagePerTick or EventFrequency, MitigationRequired [bool])
     - `HostileAreaBossMechanic` (String) — unique boss mechanic for hostile areas in this biome

2. **Create all 8 biome DataAssets**: Desert, Dusty, Rocky, Water, Swamp, Jungle, Light Snow, Ice
   - Each with appropriate modifier values matching the briefing specs

**Toolset:** `data-asset-tools`, `blueprint-tools`

### Phase 3 — Create Solar System Generator Blueprint

1. **Create `BP_SolarSystemGenerator`**:
   - Variables:
     - `StarType` (Enum: Red Dwarf, Yellow Star, Blue Giant, Binary Pair) — affects visual and game balance
     - `PlanetCount` (int, random 5-10 per briefing)
     - `Planets` (array of struct: PlanetID, Name, OrbitalDistance [float], Size [enum: Tiny/Small/Medium/Large/Gas Giant], BiomeType [GameplayTag], Resources [struct array], ScanLevelRequired [int 1-4], HostilePresence [bool], BossPresent [bool])
     - `AsteroidBeltPresent` (bool) — random chance based on star type
     - `SpaceWeatherPhenomena` (array of enum: SolarFlare/RadiationBelt/AsteroidStorm/GravityWell/NebulaCloud) — 0-3 per system
   - Functions:
     - `GenerateSystem(seed, galaxyCenterDistance)` → BP_SolarSystemDefinition DataAsset
       - `seed`: deterministic random seed for reproducible generation
       - `galaxyCenterDistance`: distance from galaxy center (affects difficulty scaling)
       - Returns a fully populated solar system DataAsset with all planets generated

     - `CalculateFuelCost(fromPlanet, toPlanet)` → float — path-based fuel calculation (not direct orbital distance; accounts for travel mode and space weather)
     - `GetScanRequirements(planetIndex)` → int — scan tier needed to reveal planet details
     - `ApplyTIRDifficultyScaling(baseStats, currentColonyTIR)` → struct — scales enemy stats from 1.0x (TIR 1) to 2.0x (TIR 5) based on ring distance

**Toolset:** `blueprint-tools`

### Phase 4 — Create Solar System Definition DataAsset

1. **Create `BP_SolarSystemDefinition` DataAsset**:
   - Properties:
     - `SystemName` (String) — procedurally generated or player-named
     - `StarType` (Enum)
     - `Planets` (array of Planet struct with all properties from Phase 3)
     - `AsteroidBeltPresent` (bool)
     - `SpaceWeatherPhenomena` (array)
     - `TravelRoutes` (struct array: FromPlanetID, ToPlanetID, FuelCost, TravelModeRequired [enum: Rocket/Atomic/Ionic/EnergyStream/VoidWarp/AlienWarp])
     - `ScanTierProgression` (int array of length PlanetCount — each planet starts at scan level 0, progresses to 4)

2. **Function**: `GetPlanetAtOrbitalIndex(index)` → Planet struct
3. **Function**: `IsPlanetRevealed(planetID)` → bool — checks current scan level
4. **Function**: `GetHostilePlanets()` → array of planet IDs where HostilePresence = true

**Toolset:** `data-asset-tools`, `blueprint-tools`

### Phase 5 — Create Planet Event System

1. **Create `BP_PlanetEventManager` Blueprint**:
   - Variables:
     - `ActiveEvents` (array of struct: EventType, TriggerCondition, Duration, Intensity)
     - `EventCooldowns` (struct array: EventType, LastTriggered, CooldownDuration)
   - Functions:
     - `RollForEncounter(biomeType, currentTIR, ringDistance)` → EncounterDefinition — determines what encounter spawns based on biome and difficulty scaling
     - `SpawnEncounter(encounterDef, location)` → Actor reference — creates the actual encounter actor in the world
     - `TriggerEnvironmentalEvent(hazardType, intensity)` — activates environmental hazards (dust storms, toxic spores, ice cracking)
     - `GetSafeLandingSpots()` → array of grid positions — returns valid landing coordinates excluding hostile areas

2. **Create `BP_EncounterDefinition` DataAsset**:
   - Properties: EncounterName, EncounterType [Environmental/Wildlife/Mechanical], BiomeFilter [GameplayTag], TIRMin/TIRMax, EnemyComposition (struct: RaceFamily, UnitCountMin/Max, SpecialAbilities array), LootReward (resource struct), CounterStrategy (String)

**Toolset:** `blueprint-tools`, `data-asset-tools`

### Phase 6 — Create Travel Mode System

1. **Create `BP_TravelModeDefinition` DataAsset** for each of the 6 travel modes:
   - Toolset: `data-asset-tools`, `blueprint-tools`
   - Properties per mode (Rocket, Atomic, Ionic, EnergyStream, VoidWarp, AlienWarp):
     - `FuelCostPerAU` (float) — fuel consumed per astronomical unit of distance
     - `EnergyCost` (float) — energy required to initiate travel
     - `ScanTierRequired` (int 1-4) — minimum scan level on destination planet
     - `SpeedMultiplier` (float) — relative speed compared to base Rocket
     - `HazardVulnerability` (array of SpaceWeatherPhenomena this mode is vulnerable to)

2. **Create `BP_TravelManager` Blueprint**:
   - Variables:
     - `CurrentSystem` (reference to BP_SolarSystemDefinition)
     - `InstalledDriveModules` (array of module IDs from dropship modules)
     - `CurrentPosition` (int — index of current planet in system's PlanetCount array)
   - Functions:
     - `GetAvailableDestinations()` → array of planet structs — planets with scan level >= required and compatible with installed drives
     - `CalculateTravelCost(fromPlanet, toPlanet, travelMode)` → struct (fuel cost, energy cost, estimated time)
     - `InitiateTravel(destinationIndex, travelMode)` → bool — validates requirements, deducts resources, triggers transition animation
     - `ApplySpaceWeatherEffect(phenomenonType, currentTravelMode)` → struct (hull damage, delay minutes, resource loss)

**Toolset:** `blueprint-tools`, `data-asset-tools`

### Phase 7 — Create Landing Hazard System

1. **Create `BP_LandingHazardDefinition` DataAsset** per biome:
   - Properties: BiomeType, HazardName, HullDamageOnUnprepared (int), FuelPenalty (float multiplier), MitigationRequired [bool], MitigationMethod (String)
   - Examples:
     - Desert → Sand Ingestion: 20-50 hull damage if no atmospheric filters installed
     - Ice → Thruster Instability: crash risk, requires heated landing gear
     - Water → Hull Breach on Impact: requires amphibious landing configuration

2. **Create `BP_LandingSystem` Blueprint**:
   - Functions:
     - `AssessLandingSafety(planetBiome, shipPreparationLevel)` → float (0-1 safety score)
     - `CalculateLandingDamage(biome, preparationLevel)` → struct (hull damage, fuel loss, delay minutes)
     - `ApplyLandingHazards(landingResult)` — deducts resources, triggers VFX based on biome

**Toolset:** `blueprint-tools`, `data-asset-tools`

### Phase 8 — Wire Into Galaxy Map UI

1. **Connect SolarSystemGenerator to galaxy map display**:
   - The galaxy map UI (S11/S12) reads from BP_SolarSystemDefinition DataAssets
   - When player enters a solar system, the UI queries `GetPlanetAtOrbitalIndex()` for each planet's position and displays orbital animations
   - Scan level progression updates the UI's fog-of-war overlay per planet

2. **Connect TravelManager to dropship modules**:
   - Available travel modes depend on which drive modules are installed in the mothership (from S13/S14)
   - The UI should only show destinations compatible with current ship configuration

**Toolset:** `blueprint-tools`

### Phase 9 — Compile & Verify

1. **Compile every Blueprint** after creation/modification
2. **Test generation**:
   - Run GenerateSystem() with a known seed → verify planets are placed at consistent orbital distances
   - Verify planet count is between 5-10
   - Check that biome assignment respects accessibility rules (early-game biomes available to TIR 1-2)
   - Test fuel cost calculation between adjacent and distant planets
   - Verify scan tier progression: unscanned planets show only silhouette, scan level 4 reveals full details including hostile presence
3. **Test in PIE**:
   - Enter a generated solar system from the galaxy map
   - Verify orbital view shows correct number of planets at proper distances
   - Click on an unscanned planet → verify fog-of-war overlay
   - Select a scanned planet → verify travel cost calculation matches installed drive modules
   - Initiate travel → verify resource deduction and transition animation

## Key Patterns & Gotchas

### Orbital Animation is Visual-Only
The orbital positions of planets in the solar system view are **purely aesthetic**. The actual gameplay uses linear planet indices (0 through N-1). A planet at "orbital distance 3.2 AU" doesn't affect travel cost — fuel cost is calculated from the pre-computed TravelRoutes table, which accounts for path complexity and space weather.

### Fuel Cost is Path-Based, Not Distance-Based
Traveling from Planet 1 → Planet 5 costs more than Planet 1 → Planet 2 even if they're adjacent in orbital distance. The system calculates fuel based on:
- Number of planets crossed (you can't "skip" intermediate orbits without additional fuel)
- Space weather phenomena along the route (gravity wells add penalty, nebula clouds reduce visibility)
- Travel mode efficiency (Void Warp is fast but requires high scan tiers and specific drive modules)

### TIR Difficulty Scaling is Ring-Based, Not Planet-Based
The galaxy center distance determines which "ring" a planet belongs to. Inner rings have higher difficulty multipliers:
- Outer ring: base enemy stats (1.0x), common biomes
- Middle ring: 1.3x enemy stats, some hostile areas appear
- Inner ring: 1.6x enemy stats, more hostile areas, rare biomes with unique hazards
- Center: 2.0x enemy stats, all biome types possible, boss encounters guaranteed on hostile planets

### Scan Level Progression is Per-Planet
Each planet starts at scan level 0 (completely unknown — not even visible on the map). Players must spend fuel and energy to scan it:
- **Scan Level 1**: Planet visible as a dot, name revealed
- **Scan Level 2**: Biome type revealed, basic resources shown
- **Scan Level 3**: Hostile presence indicated (yes/no), boss location hinted
- **Scan Level 4**: Full details — exact resources, enemy composition, safe landing spots

Higher scan levels require better scanner modules installed on the mothership.

### Space Weather is Persistent Per System
Once a solar system is generated with space weather phenomena (e.g., SolarFlare + RadiationBelt), those effects persist until the player leaves and returns to that system. They affect travel safety and can damage ships mid-transit if not properly shielded.

## File Structure Summary

```
/Game/Gameplay/SolarSystem/
├── BP_SolarSystemGenerator.uasset (Blueprint — procedural generation logic)
├── BP_PlanetEventManager.uasset (Blueprint — encounter spawning, environmental events)
├── BP_TravelManager.uasset (Blueprint — travel mode selection, route calculation)
└── BP_LandingSystem.uasset (Blueprint — landing hazard assessment and damage)

/Game/Data/SolarSystem/
├── BP_SolarSystemDefinition.uasset (DataAsset — generated system data)
├── BP_BiomeDefinition_Desert.uasset (and 7 other biome DataAssets)
├── BP_EncounterDefinition.uasset (DataAsset — encounter templates)
└── DG_TravelModes.uasset (DataTable — 6 travel mode definitions with costs)

/Game/Data/
├── DG_Planets.uasset (DataTable — planet rows, or data stored inline in SolarSystemDefinition)
└── DG_BiomeModifiers.uasset (DataTable — biome modifier lookup table)
```

## Next Steps After This Skill

Solar System Generation connects to:
- **Galaxy Map / Solar System UI (S11/S12)** — displays generated systems, handles player interaction with planets and travel routes
- **Dropship Modules (S13/S14)** — installed drive modules determine available travel modes; scanner modules affect scan speed
- **Planet Overview (S01)** — once landed on a planet, the colony management UI shows that planet's biome-specific resource yields
- **Construction Mode (S04)** — building placement validates against biome compatibility and environmental hazards
- **Tactical Combat (S08)** — dungeon encounters use the Planet Event Manager to determine enemy composition based on biome and TIR
