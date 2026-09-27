# Space Travel — Navigation, Events, Minigames, and Interplanetary Mechanics

Space travel systems including solar system navigation, wormhole transit, space events, ship-to-ship combat, and in-transit minigames.

---

## Space Travel Overview

### Travel Modes [See Navigation](../../Planets/Navigation/README.md)

| Mode | Description | Speed | Fuel Cost | Unlock Requirement |
|------|-------------|-------|-----------|-------------------|
| Rocket Drive (Standard) | Conventional chemical propulsion | Slow | 100 units per intra-system hop | Left Drive Repair complete [See Dropship](../../Spaceship/Dropship/README.md) |
| Atomic Reactor Drive | Mid-tier nuclear propulsion | Moderate | 50 units per intra-system hop | Atomic Reactor module installed [See Ship Modules](../../ShipModules/README.md) |
| Ionic Reactor Drive | Advanced ion propulsion | Fast | 30 units per intra-system hop | Ionic Reactor research complete [See Tech Tree](../../Tech-Tree/Research-Categories/README.md) |
| Energy Stream Drive | Energy-based propulsion | Very Fast | 20 units per intra-system hop | Energy Lab Ring 2+ |
| Void Warp Drive | Dimensional folding | Instant (within range) | 10 units per warp | Void Lab Outer Ring [See Ship Modules](../../ShipModules/README.md) |

### Travel Distance and Fuel Consumption [See Resources](../../resources/README.md)

| Travel Type | Baseline Fuel | Energy Required | Notes |
|-------------|--------------|-----------------|-------|
| Intra-system hop (1 planet to another) | 100 units | 20 energy for navigation | Baseline, scales with drive efficiency |
| Inter-system jump (to adjacent system) | 500 units | 100 energy for navigation | Requires full fuel tank at minimum |
| Precise planet landing | 500-750 units | 100+50 extra energy | Extra fuel for targeted descent |
| Wormhole transit | 10 units | 100 energy (initiation) | Pre-mapped wormholes only [See Navigation](../../Planets/Navigation/README.md) |
| Void warp (within 5-system radius) | Variable | 50-200 energy | Depends on distance, drive tier matters |

### Solar System Layout [See Navigation](../../Planets/Navigation/README.md)

```
                    [Sun - Center]
                   /    |    \
                  /     |     \
        (Planet 1) — (Planet 2) — (Planet 3)
              \           |          /
               \          |         /
            (Planet 4) — (Planet 5) — (Asteroid Belt)
```

| System Feature | Description | Player Interaction |
|----------------|-------------|-------------------|
| Sun | Central star, provides light and radiation | Solar panel efficiency varies by proximity |
| Planets (5-10 per system) | Orbital bodies with unique biomes [See Planet Types](../../Planets/Planet-Types/README.md) | Landing, resource extraction, colony establishment |
| Asteroid Belt | Rocky debris field between planets | Mining operations, rare mineral deposits |
| Gas Giant | Large atmospheric planet (optional) | Atmospheric sampling, fuel harvesting |

---

## Space Events System

### Event Frequency and Triggers [See Core Loop](../Core-Loop/README.md)

| Trigger Condition | Event Type | Frequency | Notes |
|------------------|------------|-----------|-------|
| During inter-system travel | Random encounter | 15% chance per jump | Combat, trade, or exploration event |
| During intra-system travel | Minor event | 8% chance per hop | Usually resource discovery or weather |
| After entering wormhole | Transit event | 25% chance | Wormhole instability or anomaly |
| Near abandoned ships | Salvage event | Always triggers | Ship condition determines loot quality |
| Approaching unknown planet | Scan event | Every first approach | Reveals planet details [See Navigation](../../Planets/Navigation/README.md) |

### Event Types and Mechanics

#### Combat Events — Enemy Encounters

| Encounter Type | Enemy Composition | Difficulty | Reward |
|---------------|-------------------|------------|--------|
| Pirate Patrol | 2-4 light ships, ballistic weapons | Easy (TIR 1) | 50-200 minerals, 30-80 fuel |
| Alien Scout Fleet | 1-3 fast vessels, plasma weapons | Moderate (TIR 2) | Blueprints, 100-400 minerals |
| Enemy War Party | 4-8 mixed ships, varied weapons | Hard (TIR 3) | Advanced blueprints, 500+ minerals |
| Elite Squadron | 2 heavy cruisers + escorts | Very Hard (TIR 4) | Outer ring blueprint, dark matter crystals |

**Space Combat Sequence:**

```
┌─────────────────────────────────────────────────────────────┐
│                  SPACE COMBAT                               │
│                                                             │
│  Phase 1: Engagement (0-30 seconds)                         │
│  - Enemy ship enters firing range                           │
│  - Player chooses: Fight, Flee, or Board                    │
│  - Radar lock establishes target acquisition                │
│                                                             │
│  Phase 2: Exchange Fire (30-120 seconds)                    │
│  - Weapons fire based on mount positions and cooldowns      │
│  - Shield absorption calculates damage reduction            │
│  - Hull integrity tracks structural damage                  │
│                                                             │
│  Phase 3: Tactical Options (ongoing)                        │
│  - EMP pulse disables weapons for 10 seconds                │
│  - Torpedo launch deals heavy damage but long cooldown      │
│  - Boarding action transitions to dungeon raid              │
│                                                             │
│  Phase 4: Resolution (30-60 seconds)                        │
│  - Enemy ship destroyed or flees                            │
│  - Salvage operation begins on wreckage                     │
│  - Damage repair assessment on player ship                  │
└─────────────────────────────────────────────────────────────┘
```

#### Exploration Events — Discoveries

| Event Type | Description | Loot/Information | TIR Required |
|------------|-------------|-----------------|--------------|
| Abandoned Cargo Ship | Derelict freighter drifting in space | 200-800 minerals, 100-300 fuel, blueprint chance | TIR 1+ |
| Alien Probe | Small autonomous scout vessel | Planet scan data, rare mineral sample | TIR 2+ |
| Space Station Ruins | Derelict orbital facility | Full loot table access, multiple blueprints | TIR 3+ |
| Dyson Swarm Fragment | Energy collection structure | +5% solar efficiency permanently if repaired | TIR 4+ |
| Alien Warp Gate | Inter-galactic transit point | Direct travel to alien galaxy (endgame) | TIR 5+ |

#### Resource Events — Harvesting Opportunities

| Event Type | Resource Gained | Collection Method | Notes |
|------------|----------------|-------------------|-------|
| Asteroid Field | 100-500 minerals per field | Mining minigame [See Ship Modules](../../ShipModules/README.md) | Richer fields at TIR 3+ |
| Gas Giant Atmosphere | 200-600 fuel per harvest | Atmospheric scoop operation | Requires gas processing module |
| Solar Flare | 50-150 energy surge | Deploy solar arrays during event | Risk of overcharge damage |
| Ice Comet | 100-300 survival units per intercept | Mining and processing | Rare, appears randomly |

#### Minigame Events — Skill-Based Challenges

| Event Type | Minigame Description | Success Reward | Failure Penalty |
|------------|---------------------|---------------|-----------------|
| Wire Repair (Dropship) | Connect correct wire sequence to restore power | Systems operational | 10-30 minutes delay, possible component damage |
| Asteroid Evasion | Click/tap incoming asteroids to destroy them | Safe passage | Hull damage, fuel leak (-5% per hit) |
| Signal Decoding | Match frequency patterns to decode alien signal | Blueprint or rare data | Missed discovery opportunity |
| Warp Hole Navigation | Guide ship through narrowing tunnel | Bonus materials, no damage | Random system malfunction |

---

## In-Transit Minigames and Activities [See Core Loop](../Core-Loop/README.md)

### Available Minigames During Travel

| Minigame | Type | Duration | Reward | Unlock Requirement |
|----------|------|----------|--------|-------------------|
| Wire Repair | Puzzle (sequence matching) | 2-5 minutes | Systems restored, possible bonus parts | None |
| Asteroid Destruction | Action (click/tap timing) | 1-3 minutes | 50-200 minerals per asteroid destroyed | Basic radar array |
| Signal Decoding | Puzzle (pattern recognition) | 3-8 minutes | Blueprint chance, planet data | Energy Lab Ring 1+ |
| Warp Tunnel Navigation | Reflex (steering through obstacles) | 2-6 minutes | Bonus materials, no damage | Void Warp Drive installed |
| Resource Sorting | Management (categorization) | 5-10 minutes | +10% resource efficiency for next trip | Container Placement researched |
| Crew Rest Rotation | Timing-based scheduling | 1-2 minutes per rotation | Unit energy restored, morale boost | Barracks module operational |

### Wire Repair Minigame — Detailed Rules

```
┌─────────────────────────────────────────────────────────────┐
│              WIRE REPAIR MINIGAME                           │
│                                                             │
│  Setup:                                                     │
│  - 4-6 colored wires on damaged panel                       │
│  - 4-6 corresponding port slots (scrambled order)           │
│  - Player must match correct wire to correct port           │
│                                                             │
│  Difficulty by TIR:                                         │
│  TIR 1: 3 wires, 2 attempts                                 │
│  TIR 2: 4 wires, 2 attempts                                 │
│  TIR 3: 5 wires, 3 attempts                                 │
│  TIR 4+: 6 wires, 3 attempts                                │
│                                                             │
│  Success:                                                   │
│  - All wires matched correctly → Systems fully restored     │
│  - Partial match (>50%) → Systems partially functional      │
│  - Failed completely → Component damage, retry required     │
│                                                             │
│  Rewards for Success:                                       │
│  - Basic systems operational                                │
│  - 10-20% chance of bonus hull parts                        │
│  - 5% chance of bonus blueprint (TIR 3+)                    │
└─────────────────────────────────────────────────────────────┘
```

### Signal Decoding Minigame — Detailed Rules

```
┌─────────────────────────────────────────────────────────────┐
│              SIGNAL DECODING MINIGAME                       │
│                                                             │
│  Setup:                                                     │
│  - Alien signal appears as frequency wave pattern           │
│  - Player must identify and match repeating patterns        │
│  - Patterns grow more complex with each decoded segment     │
│                                                             │
│  Difficulty by TIR:                                         │
│  TIR 1-2: 3 segments, simple patterns                       │
│  TIR 3: 4 segments, moderate complexity                     │
│  TIR 4+: 5 segments, complex overlapping patterns           │
│                                                             │
│  Success:                                                   │
│  - Full decode → Blueprint reward + planet scan data        │
│  - Partial decode (>60%) → Planet scan data only            │
│  - Failed (<40%) → Signal lost, no reward                   │
│                                                             │
│  Rewards for Full Decode:                                   │
│  - 1 blueprint from appropriate TIR tier                    │
│  - Complete planet scan (all features revealed)             │
│  - Rare mineral sample chance (TIR 4+)                      │
└─────────────────────────────────────────────────────────────┘
```

---

## Ship-to-Ship Combat [See Weapons](../../weapons/Weapon-Types/README.md)

### Weapon Mounting Positions on Dropship [See Ship Modules](../../ShipModules/README.md)

| Mount Position | Arc of Fire | Compatible Weapons | Upgrade Slots |
|---------------|-------------|-------------------|---------------|
| Nose (Front) | Forward 120° | Ballistic, Plasma, Torpedo | Up to TIR 5 [See TIR System](../../weapons/TIR-System/README.md) |
| Port (Left) | Left side 90° | Ballistic, Energy Stream | Up to TIR 4 |
| Starboard (Right) | Right side 90° | Ballistic, Energy Stream | Up to TIR 4 |
| Tail (Rear) | Rearward 180° | Ballistic, Rocket | Up to TIR 3 |
| Dome (Top) | 360° coverage | Plasma, Ion Weapon | Up to TIR 5 [See Tech Tree](../../Tech-Tree/Research-Categories/README.md) |

### Space Combat Stats

| Stat | Value | Effect | Notes |
|------|-------|--------|-------|
| Ship HP (base) | 500 HP | Structural integrity before critical damage | Repaired at base or via minigames |
| Shield HP (with generator) | +250 HP equivalent | Absorbs incoming damage first | Recharges after 30 seconds out of combat |
| Evasion Chance | 10-25% | Probability to dodge incoming fire | Increases with Vision upgrades [See Upgrade System](../../units/Upgrade-System/README.md) |
| Turn Speed | Variable | How quickly ship can reorient | Affected by drive type and weight |

### Damage Calculation — Space Combat

| Attacking Weapon | Base Damage | Shield Penetration | Hull Damage | Notes |
|-----------------|-------------|-------------------|-------------|-------|
| Ballistic Turret (TIR 1) | 25 damage | 30% | Direct to hull after shield | Effective against light ships |
| Plasma Cannon (TIR 2) | 40 damage | 60% | High hull damage | Melts through shields efficiently |
| Torpedo Launcher (TIR 3) | 80 damage | 100% | Massive hull damage | Slow cooldown, guaranteed hit if locked |
| Ion Weapon (TIR 4) | 30 damage | 50% | Disables systems temporarily | Ship drifts for 10 seconds, vulnerable |
| Void Beam (Outer Ring) | 60 damage | 100% | Ignores armor completely | Pierces shields and hull equally |

---

## Planet Landing Procedures [See Navigation](../../Planets/Navigation/README.md)

### Pre-Landing Scan Requirements

| Scan Tier | Energy Cost | Time Required | Information Revealed |
|-----------|-------------|---------------|---------------------|
| Basic Radar (TIR 1) | 10 energy | 5 minutes | Biome type, temperature range, basic resources |
| Deep Sonar (TIR 2) | 25 energy | 10 minutes | Underground structures, dungeon locations, water tables |
| Satellite Array (TIR 3) | 40 energy/turn | Continuous deployment | Full planet monitoring, enemy positions, resource nodes |
| Quantum Scanner (TIR 5) | 100 energy one-time | Instant activation | Complete map reveal, all features visible including secrets |

### Landing Sequence Animation

| Phase | Description | Duration | Player Input |
|-------|-------------|----------|--------------|
| Atmosphere Entry | Ship enters atmosphere, heat shield engages | 5 seconds | Automatic |
| Descent Control | Pilot adjusts thrusters for controlled landing | 10 seconds | Minor steering adjustments |
| Touchdown | Ship contacts surface, dust cloud dissipates | 3 seconds | Timing-based stability check |
| Systems Check | Post-landing diagnostic sequence | 5 seconds | Wire repair minigame possible if damaged |

### Landing Zone Safety Assessment

| Factor | Safe Condition | Moderate Risk | Dangerous |
|--------|---------------|---------------|-----------|
| Enemy Presence | None detected | 1-3 hostile units | 4+ units or boss nearby |
| Terrain Stability | Flat, solid ground | Slight incline, loose surface | Cliff edge, unstable dune |
| Weather Conditions | Clear skies, moderate temp | Light wind, temperature extremes | Storm, blizzard, volcanic activity |
| Resource Proximity | Within 500m of basic resources | 500-2000m from resources | >2km from any resources |

---

## Multi-Planet Logistics [See Core Loop](../Core-Loop/README.md)

### Colony Network Management

| Colony Role | Primary Function | Required Buildings | Resource Output |
|-------------|-----------------|-------------------|-----------------|
| Mining Outpost (TIR 2+) | Extract minerals from rich deposits | Mine, Power Plant, Haul Storage | 500+ minerals/turn [See Planet Buildings](../../buildings/README.md) |
| Fuel Production Base (TIR 2+) | Refine and store fuel reserves | Oil Pump, Refinery, Container Storage | 200+ fuel/turn [See Resources](../../resources/README.md) |
| Military Fortress (TIR 3+) | Secure high-TIR planets and dungeons | Barracks, Factory, Turret Network | 1 mech squad/week [See Units](../../units/README.md) |
| Research Station (TIR 3+) | Accelerate technology discovery | Energy Lab, Dark Matter Lab | +25% research speed in network |
| Agricultural World (TIR 2+) | Mass survival production | Hydroponic Farm, Atmospheric Extractor | 100+ survival/turn [See Resources](../../resources/README.md) |

### Resource Transfer Methods Between Planets

| Method | Capacity | Speed | Energy Cost | Unlock Requirement |
|--------|----------|-------|-------------|-------------------|
| Fuel Tanker (physical ship) | 500 fuel per trip | Slow (hours real-time) | 50 energy per trip | Factory Construction + Hangar Module [See Ship Modules](../../ShipModules/README.md) |
| Mineral Hauler (cargo vessel) | 2000 minerals per trip | Moderate (30 min real-time) | 30 energy per trip | Container Placement + Mine Placement |
| Quantum Gate Network | Unlimited | Instant | 10 energy per transfer | Void Lab Outer Ring research [See Tech Tree](../../Tech-Tree/Research-Categories/README.md) |
| Atmospheric Transfer (gas planets only) | Variable | Slow | 20 energy per cycle | Gas Processing Module installed |

### Colony Resource Flow Example

```
Home Planet (TIR 5 Command Center)
    │
    ├──→ Quantum Gate ←── Mining Outpost Alpha ──→ 500 minerals/turn
    │       │
    │       ├──→ Quantum Gate ←── Fuel Base Beta ──→ 200 fuel/turn
    │       │
    │       └──→ Quantum Gate ←── Fortress Gamma ──→ 1 mech squad/week
    │
    └──→ Fuel Tanker Route:
            ├──→ Agricultural World Delta ──→ 100 survival/turn
            └──→ Research Station Epsilon ──→ +25% research speed network-wide
```

---

## Space Weather and Environmental Effects [See Planet Types](../../Planets/Planet-Types/README.md)

### Solar System Phenomena

| Phenomenon | Effect on Travel | Mitigation | TIR Required to Counter |
|------------|-----------------|------------|------------------------|
| Solar Flare | +20% navigation error, possible system damage | Deploy magnetic shield | TIR 2+ |
| Radiation Belt | Shield degradation over time | Route through belt quickly (<5 min) | TIR 1+ basic shielding |
| Asteroid Storm | Collision risk, hull damage | Evasion minigame required | Any drive tier |
| Gravity Well | Pulls ship off course, increased fuel use | Thrust against pull (minigame) | TIR 3+ for easy counter |
| Nebula Cloud | Radar interference, blind navigation | Visual piloting only | TIR 4+ sensors pierce cloud |

### Biome-Specific Landing Hazards [See Planet Types](../../Planets/Planet-Types/README.md)

| Biome Type | Landing Hazard | Damage if Unprepared | Preparation Required |
|------------|---------------|---------------------|---------------------|
| Desert | Sand ingestion into engines | 20-50 hull damage per landing | Heat-resistant hull plating [See Dropship](../../Spaceship/Dropship/README.md) |
| Ice | Slippery surface, thruster instability | Crash if not on flat ground | Ice-grip thrusters, precision landing minigame |
| Swamp | Mud suction, delayed takeoff | +50% fuel cost for departure | Reinforced landing gear |
| Rocky | Uneven terrain, sharp debris | 30-80 hull damage from debris | Reinforced underbelly armor [See Ship Modules](../../ShipModules/README.md) |
| Jungle | Dense canopy obscures landing zone | Crash into trees (20-40 HP ship damage) | Canopy-piercing radar array |
| Swamp/Water | Liquid ingress into intake valves | Engine failure on next departure | Water-sealed intakes, pump system check |

---

## Endgame Space Travel [See Tech Tree](../../Tech-Tree/Overview/README.md)

### Void Warp System — Ultimate Mobility

| Feature | Details |
|---------|---------|
| Range | 5-system radius from current position |
| Transit Time | 8 seconds wormhole opening + 3 seconds through |
| Energy Cost | 50-200 energy depending on distance and drive tier |
| Fuel Cost | 10 units per warp (negligible) |
| Unlock Requirement | Void Warp Drive module [See Ship Modules](../../ShipModules/README.md) + Void Lab Outer Ring research |

### Alien Warp Integration — Unlimited Travel

| Feature | Details |
|---------|---------|
| Range | Any distance, any galaxy |
| Transit Time | 8 seconds opening + 3 seconds through (same as void warp) |
| Energy Cost | 100 energy per jump |
| Fuel Cost | 20 units per jump |
| Unlock Requirement | Alien Warp Integration blueprint from Inner Ring Citadel boss [See Dungeons](../Dungeons/Dungeon-Overview/README.md) |

### Post-Victory: Alien Galaxy Exploration [See Spaceship Progression](../../Spaceship/Progression/README.md)

After warp transit to alien galaxy:
1. **New starfield** — Different visual aesthetic, purple/blue nebulae instead of yellow stars
2. **Abandoned space stations** — Salvage for advanced blueprints and rare materials
3. **Energy fields** — Replaces asteroid belts, provides energy surge events (+50% solar efficiency if harvested)
4. **Warfields** — Battle-scarred planets with elite alien remnants (TIR 5+ enemies)
5. **Alien planets** — Unique biome types not found in home galaxy
6. **New progression curve** — Similar TIR system but energy-based challenges and rewards

---

## See Also

- [Core gameplay loop and phase progression](../Core-Loop/README.md)
- [Dungeon types, loot tables, and blueprint rewards](../Dungeons/Dungeon-Overview/README.md)
- [Solar system navigation and planet scanning](../../Planets/Navigation/README.md)
- [Dropship layout, components, and repair](../../Spaceship/Dropship/README.md)
- [Ship evolution across TIR tiers](../../Spaceship/Progression/README.md)
- [Weapon types and mounting positions](../../weapons/Weapon-Types/README.md)
- [TIR progression impact on all systems](../../weapons/TIR-System/README.md)
- [Radial tech tree structure and ring progression](../../Tech-Tree/Overview/README.md)
