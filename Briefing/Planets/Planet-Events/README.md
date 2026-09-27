# Planet Events & Dungeons

Abandoned structures, camps, loot tables, and encounter types found on planets.

---

## Dungeon Types

Dungeons are instanced or semi-instanced exploration areas with escalating difficulty, puzzles, and blueprint rewards. They range from simple room discoveries to full hack-and-slay experiences.

### Dungeon Classification by Size/Complexity

| Tier | Size | Discovery Time | Loot Quality | Example |
|------|------|----------------|--------------|---------|
| Small | 1-3 rooms | 2-5 minutes | Low (basic materials, TIR 1 blueprints) | Abandoned shed, crashed satellite |
| Medium | 4-10 rooms | 10-20 minutes | Medium (moderate resources, TIR 1-2 blueprints) | Military outpost, research station wing |
| Large ("Dungeon") | 11-30+ rooms | 30-60 minutes | High (significant resources, TIR 2-4 blueprints) | Lost bunker, alien facility, hospital complex |

### Dungeon Types

#### Abandoned Houses/Shelters
- **Rooms:** 1-3
- **Enemies:** 0-2 (scavengers, animals, or environmental hazards)
- **Loot:** Small amounts of construction material, survival supplies, occasional TIR 1 blueprint
- **Special:** Sometimes entrance is blocked — requires mining tool to access (yields bonus materials)
- **Visual:** Crumbling structures, partial collapse, overgrown vegetation

#### Military Camps
- **Rooms:** 3-8 (tents + armory + command center)
- **Enemies:** 4-12 (armed patrols, turrets)
- **Loot:** Weapon blueprints (TIR 1-2), minerals, fuel canisters
- **Special:** Active radio signal attracts attention if base is nearby [See attraction system](../../Gameplay/Core-Loop/README.md)
- **Visual:** Sandbag fortifications, armored vehicles, searchlights

#### Hospitals
- **Rooms:** 5-15 (wards, labs, storage, basement archives)
- **Enemies:** 2-8 (infected units, automated defenses, security personnel)
- **Loot:** Survivability blueprints (TIR 1-3), medical supplies, oxygen generators
- **Special:** Key source of survivability research — completing hospital run unlocks passive survival bonuses [See resources/types.md](../../resources/README.md)
- **Visual:** Sterile white corridors, blood stains, broken medical equipment

#### Lost Bunkers
- **Rooms:** 8-30 (multi-level underground facility)
- **Enemies:** 6-20 (elite guards, automated turrets, patrol units)
- **Loot:** Weapon blueprints (TIR 2-4), mineral deposits, energy cells
- **Special:** Heavily fortified — may require specific weapon type to breach final door
- **Visual:** Reinforced steel doors, emergency lighting, military-grade construction

#### Alien Structures
- **Rooms:** Variable (non-Euclidean geometry, looping corridors)
- **Enemies:** 4-15 (alien creatures, energy constructs, guardians)
- **Loot:** Alien tech blueprints (TIR 3-5), dark matter crystals, unique minerals
- **Special:** Requires alien language decoding or key items to access all areas
- **Visual:** Smooth curved walls, bioluminescent lighting, unknown symbols

#### Vehicle Graveyards
- **Rooms:** Open-air + hangar bays (5-12 vehicles)
- **Enemies:** 2-6 (scavenger gangs, automated salvage drones)
- **Loot:** Hull materials, construction material (salvageable), vehicle blueprints
- **Special:** Some vehicles are still functional — can be commandeered or salvaged for parts
- **Visual:** Rusted hulls, exposed wiring, scattered debris fields

#### Oil Rig Ruins
- **Rooms:** 3-8 (platform levels, underground storage)
- **Enemies:** 3-10 (oil-spawned creatures, rig workers, pumps)
- **Loot:** Fuel canisters, construction material, energy cells
- **Special:** Active pumps may explode if not shut down (area damage)
- **Visual:** Leaking oil, flickering lights, rusted metal structures

---

## Loot Tables by Dungeon Tier

### Small Dungeon Loot

| Resource | Quantity | Notes |
|----------|----------|-------|
| Construction Material | 50-150 | Basic materials |
| Minerals | 20-80 | Random type |
| Fuel | 10-30 | Emergency canisters |
| Survival | 10-20 | Rations, oxygen packs |
| TIR 1 Blueprint | 0-1 | 15% chance |

### Medium Dungeon Loot

| Resource | Quantity | Notes |
|----------|----------|-------|
| Construction Material | 200-600 | Includes processed materials |
| Minerals | 100-400 | Higher tier minerals possible |
| Fuel | 50-150 | Refined fuel possible |
| Survival | 30-80 | Extended supply packs |
| TIR 1 Blueprint | 1-2 | Guaranteed at least one |
| TIR 2 Blueprint | 0-1 | 25% chance |

### Large Dungeon Loot

| Resource | Quantity | Notes |
|----------|----------|-------|
| Construction Material | 800-2500 | Includes advanced composites |
| Minerals | 500-2000 | All mineral types including titanium |
| Fuel | 200-600 | Black matter possible |
| Survival | 100-300 | Extended supply + medical |
| TIR 2 Blueprint | 1-3 | Guaranteed at least one |
| TIR 3 Blueprint | 0-2 | 40% chance |
| TIR 4 Blueprint | 0-1 | 10% chance (boss dungeon) |

---

## Abandoned Structures (Non-Dungeon)

Structures that don't qualify as full dungeons but provide resources and information.

### Scavengeable Structures

| Structure | Loot Quality | Time to Clear | Notes |
|-----------|--------------|---------------|-------|
| Crashed Dropship | Small | 1-2 minutes | Early game, may contain partial blueprints |
| Outpost Relay | Small-Medium | 3-5 minutes | Radio signal reveals nearby dungeon direction |
| Mining Shelter | Small | 2-3 minutes | Mine tools blueprint possible |
| Weather Station | Small | 1-2 minutes | Biome modifier info, minor resources |
| Cargo Container Cluster | Small | 2-4 minutes | Random resource cache, may be trapped |
| Satellite Ground Station | Medium | 5-10 minutes | Unlocks long-range scanning when repaired |

### Interactive Structures

| Structure | Interaction | Reward | Notes |
|-----------|-------------|--------|-------|
| Repairable Vehicle | Spend construction material + minerals | Functional vehicle for exploration | One-time use per structure |
| Activated Radar Tower | Costs 50 energy | Reveals 25% of planet map | Cooldown: 30 minutes real-time |
| Ancient Terminal | Solve mini-puzzle (wire connection) | Random TIR 1 blueprint | Difficulty scales with TIR |
| Alien Monolith | Scan with radar (costs 100 energy) | Reveals nearest dungeon location | Cooldown: 60 minutes |

---

## Camp Types & Encounters

Active enemy camps that spawn patrols and serve as forward operating bases for hostile races.

### Camp Classification

| Camp Size | Enemy Count | Patrol Size | Loot Quality | Threat Level |
|-----------|-------------|-------------|--------------|--------------|
| Outpost | 2-5 units | 1-2 patrols | Small | Low (TIR 1) |
| Forward Base | 6-15 units | 2-4 patrols | Medium | Medium (TIR 1-2) |
| Military Camp | 16-40 units | 4-8 patrols | High | High (TIR 2-3) |
| Fortress | 41-100+ units | 8-16 patrols | Very High | Extreme (TIR 3+) |

### Camp Behaviors

- **Patrol Routes:** Camps send patrols that reveal position on map when nearby. Patrols return to camp to report player location.
- **Reinforcement Beacon:** Destroying the beacon prevents reinforcements but alerts nearby camps [See attraction system](../../Gameplay/Core-Loop/README.md)
- **Supply Lines:** Larger camps have supply trucks — destroying them reduces enemy HP regeneration for 10 minutes
- **Capture Point:** Players can capture and temporarily convert camps to their side (lasts until player leaves planet)

### Camp Loot

| Camp Type | Resources | Blueprints | Special Items |
|-----------|-----------|------------|---------------|
| Outpost | 50-200 total resources | TIR 1 weapon mod | Radio battery |
| Forward Base | 200-800 total resources | TIR 1-2 blueprint | Camp keycard |
| Military Camp | 800-3000 total resources | TIR 2-3 blueprint | Vehicle part, comms headset |
| Fortress | 3000-10000+ total resources | TIR 3-4 blueprint | Command channel frequency |

---

## Encounter Types

Random encounters that occur during exploration on planet surfaces.

### Environmental Encounters

| Encounter | Trigger | Effect | Counter |
|-----------|---------|--------|---------|
| Dust Storm | Desert/Dusty biome, random | Visibility -60%, movement -40% for 3 min | Seek shelter, wait it out |
| Sand Quicksand | Desert biome, walking in open area | Traps unit for 15 seconds | Nearby ally can dig out |
| Lava Burst | Rocky/Volcanic biome, random | Area damage in 3m radius for 10 seconds | Move away, heat-resistant armor |
| Ice Crack | Ice biome, heavy units on thin ice | Unit falls, takes fall damage | Anchored boots prevent |
| Spore Cloud | Swamp biome, random | Poisoned status (-5 HP/sec for 30 sec) | Gas mask or medkit |
| Crystal Resonance | Ice/Crystal biome near crystals | Sonic pulse deals 20 damage | Cover ears, use dampeners |

### Wildlife Encounters

| Encounter | Biome | Behavior | Aggression |
|-----------|-------|----------|------------|
| Desert Scavengers | Desert | Flees from combat, steals dropped items | Passive |
| Jungle Stalkers | Jungle | Ambush from canopy, retreats after damage | Semi-aggressive |
| Ice Predators | Ice/Light Snow | Packs hunt in groups of 3-5 | Aggressive |
| Swamp Crawlers | Swamp | Slow but tanky, acidic blood splash | Semi-aggressive |
| Sky Razors | All (airborne) | Dive-bomb units, high mobility | Aggressive when provoked |

### Mechanical Encounters

| Encounter | Biome | Description | Loot |
|-----------|-------|-------------|------|
| Rogue Drone | All biomes | Scanning drone, attacks on sight | Energy cells, mineral scrap |
| Automated Turret | All biomes | Stationary, triggers when approached | Metal ore, energy cell |
| Cargo Drop | Random | Airdropped from orbit, guarded or unguarded | Random resources (50-200) |
| Mobile Forge | TIR 3+ planets | Enemy mobile workshop crafting units | Blueprint fragment |

---

## Dungeon Discovery Mechanics

### How Dungeons Are Found

1. **Exploration:** Walking near undiscovered structure reveals silhouette on radar if within range
2. **Radar Scan:** Costs energy, reveals exact location if scan range exceeds distance [See navigation](../Navigation/README.md)
3. **Dungeon Scanner Building:** Elite building that periodically pulses to reveal nearby dungeons [See planet buildings](../../buildings/README.md)
4. **Alien Monolith:** When activated, points toward nearest large dungeon

### Dungeon Entry Indicators

- **Small:** Visible entrance from surface (door, crack in wall, open window)
- **Medium:** Clear entrance with signage or energy field barrier
- **Large:** Massive structure visible on horizon, requires keycard/code/weapon to enter

### Marking Discovered Dungeons

- Discovered dungeons appear on player map permanently (not erased when leaving planet)
- Player can tag dungeons as: Unknown, Safe (cleared), Active (enemies respawn), High Value (blueprint confirmed)
- Tags persist between playthroughs for persistent world [See gameplay core loop](../../Gameplay/Core-Loop/README.md)

---

## See Also

- [Dungeon Types in Gameplay Context](../../Gameplay/Dungeons/Dungeon-Overview/README.md)
- [Planet Biome Modifiers](../Planet-Types/README.md)
- [Resource Loot Values](../../resources/README.md)
- [Building-Based Dungeon Scanning](../../buildings/README.md)
- [Attraction System and Camp Alerts](../../Gameplay/Core-Loop/README.md)
