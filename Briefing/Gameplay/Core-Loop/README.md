# Core Gameplay Loop — Progression Flow from Crash to Galaxy Conquest

Complete gameplay loop covering crash landing, survival, exploration, base building, research progression, and interplanetary expansion.

---

## Game Overview

**Genre:** 2.5D Top-Down RTS with side-view cinematic sequences
**Perspective:** Planet surface = top-down view | Space travel = top-down map | Crash/landing = side-view animation
**Core Loop:** Survive → Explore → Build → Research → Expand → Repair → Depart
**Win Condition:** Reach galaxy center, defeat alien boss, repair dropship enough to warp to alien galaxy and conquer

---

## Phase 1: Crash Landing (Tutorial)

### Animated Sequence
1. **Side-view flight** — Dropship flies low over desert landscape
2. **Crash impact** — Ship crashes into dune, dust cloud animation
3. **Post-crash scene** — Cockpit buried in sand, smoke rising from damaged sections
4. **Door opens** — Player character exits through side door (camera transitions to top-down)

### Dropship Starting State [See Dropship](../../Spaceship/Dropship/README.md)

| Component | Status | Details |
|-----------|--------|---------|
| Cockpit | Operational | Pilot seat, basic instruments, 1 champion slot |
| Right Rocket Drive | Functional (empty fuel) | 50 kN thrust, needs refueling |
| Left Rocket Drive | Damaged (50% thrust) | Needs repair materials |
| Habitation Module | Operational | Cryo-stasis pods for 10 soldiers |
| Storage Module | Partially full | ~80 resources scattered supplies |
| Energy Core | Operational | 50 energy capacity, powers basic systems |

### Tutorial Objectives (First 30 Minutes)
1. **Explore crash site** — Scavenge remaining supplies from storage module [See Resources](../../resources/README.md)
2. **Collect construction material** — Manual mining of stone/concrete from nearby area
3. **Discover first resource deposit** — Find and scan mineral deposits using basic radar [See Navigation](../../Planets/Navigation/README.md)
4. **Build solar panel** — First structure placement tutorial [See Planet Buildings](../../buildings/README.md)
5. **Repair left drive** — First research unlock (15 minutes at forge bench) [See Tech Tree](../../Tech-Tree/Overview/README.md)

---

## Phase 2: Survival & Initial Exploration (TIR 1-2)

### Core Loop Cycle — Early Game

```
┌─────────────────────────────────────────────────────────────┐
│                    EARLY GAME LOOP                          │
│                                                             │
│  Morning:                                                   │
│  1. Check resource production (energy, fuel, materials)     │
│  2. Assign workers to mines/farms                           │
│  3. Send scout units to explore nearby area                 │
│                                                             │
│  Midday:                                                    │
│  4. Discover structures (houses, camps, vehicles)           │
│  5. Combat hostile wildlife or enemy patrols                │
│  6. Collect loot and blueprints from dungeons               │
│                                                             │
│  Evening:                                                   │
│  7. Return to base, process resources                       │
│  8. Conduct research at forge bench                         │
│  9. Build new structures based on research unlocks          │
│ 10. Defend base from nighttime attacks                      │
│                                                             │
│  Night:                                                     │
│  11. Monitor radar for enemy movement                       │
│  12. Assign guard units to defensive positions              │
│  13. Sleep soldiers in habitation module (restores energy)  │
└─────────────────────────────────────────────────────────────┘
```

### Resource Management — Early Game Priorities

| Priority | Resource | Target | Source |
|----------|----------|--------|--------|
| 1 | Energy | 50+ capacity, steady generation | Solar panels (clear biomes), wind turbines |
| 2 | Construction Material | 500+ stockpile | Manual mining → basic mine placement |
| 3 | Fuel | 200+ units (2 intra-system hops) | Scavenged fuel → oil pump refinement |
| 4 | Minerals | 100+ stockpile | Basic mineral mine on discovered deposits |
| 5 | Survival | 40+ units per soldier | Hunting, orchards, or atmospheric extractor |

### Exploration Radius Progression

| Time Played | Exploration Range | Discovery Method |
|-------------|-------------------|-----------------|
| 0-10 minutes | 100m from dropship | On-foot scouting |
| 10-30 minutes | 500m from base | Basic radar array (200m) + foot patrol |
| 30-60 minutes | 2km from base | Deep sonar array (500m radius) + scout units |
| 1-3 hours | Planet surface | Satellite deployment (planet-wide monitoring) |

### Dungeon Types — Early Game Accessible [See Dungeons](../Dungeons/Dungeon-Overview/README.md)

| Dungeon Type | Size | Difficulty | Loot Tier | TIR Required |
|--------------|------|------------|-----------|--------------|
| Abandoned House | Small (1-3 rooms) | Easy | Basic weapons, 50-100 minerals | TIR 1 |
| Enemy Camp | Medium (4-8 rooms + outdoor area) | Moderate | Blueprints for barracks, 200-500 minerals | TIR 1-2 |
| Scavenged Vehicle | Tiny (1 room + vehicle bay) | Easy | Fuel cans, basic parts, 30-80 hull | TIR 1 |
| Military Outpost | Medium-Large (8-15 rooms) | Hard | Weapon blueprints, 500-1000 minerals, armor upgrades | TIR 2 |

### Unlock Path to Ring 2 Research [See Tech Tree](../../Tech-Tree/Overview/README.md)

Complete at least **3 of the following** to unlock Energy Lab construction:
- [ ] Complete 1 large dungeon (hospital complex or alien structure fragment)
- [ ] Build and operate 1 mine + 1 power plant continuously for 30 minutes
- [ ] Discover and scan 5 new planets via solar system navigation
- [ ] Defeat TIR 2 elite enemy unit or boss

---

## Phase 3: Base Building & Expansion (TIR 2-3)

### Core Loop Cycle — Mid Game

```
┌─────────────────────────────────────────────────────────────┐
│                    MID GAME LOOP                            │
│                                                             │
│  Morning:                                                   │
│  1. Review production metrics across all buildings          │
│  2. Adjust worker assignments based on research priorities  │
│  3. Launch aerial scouts (drones) for extended reconnaissance│
│                                                             │
│  Midday:                                                    │
│  4. Deploy combat squads to secure resource-rich areas      │
│  5. Engage hostile forces in tactical combat                │
│  6. Capture and convert enemy structures                    │
│  7. Extract dungeon blueprints from fortified locations     │
│                                                             │
│  Evening:                                                   │
│  8. Research new technologies at Energy Lab                 │
│  9. Construct advanced buildings (factory, airfield)        │
│ 10. Manufacture vehicles and units at factory               │
│ 11. Upgrade existing units with new armor/weapon tiers      │
│                                                             │
│  Night:                                                     │
│  12. Activate defensive systems (turrets, orbital strike)   │
│  13. Monitor deep sonar for underground threats             │
│  14. Rotate guard shifts, heal wounded soldiers at med bay  │
└─────────────────────────────────────────────────────────────┘
```

### Building Progression — Mid Game Construction Order

| Priority | Building | Grid Size | Purpose | Research Prerequisite |
|----------|----------|-----------|---------|----------------------|
| 1 | Energy Lab | 3x3 | Ring 2+ research facility | Forge Upgrade Path complete |
| 2 | Factory | 4x4 | Vehicle and module production | Basic Power Grid + Mine Placement |
| 3 | Barracks (upgraded) | 2x2 | Unit training and healing | Soldier Training I complete |
| 4 | Airfield | 6x6 | Aerial unit deployment | Heavy Vehicle Production started |
| 5 | Haul Storage | 4x4 | 10,000 resource capacity | Container Placement + Organization I |
| 6 | Advanced Power Grid | Variable | Automated power routing | Factory Construction complete |

### Worker Allocation System

| Building Type | Workers Required | Output per Worker/Turn | Optimal Range |
|---------------|-----------------|----------------------|---------------|
| Basic Mine | 2 workers | 10 resources/turn | 4-6 workers (diminishing after 4) |
| Oil Pump | 3 workers | 50 fuel/cycle | 6-9 workers (diminishing after 7) |
| Solar Array | 1 worker | 20 energy/turn | Any (automated, no optimization needed) |
| Factory | 8 workers min | 1 vehicle per 10 min | 16 workers for full capacity |
| Hydroponic Farm | 2 workers | 25 survival/cycle | 4-6 workers (diminishing after 5) |

### Combat Loop — Planet Surface

```
┌─────────────────────────────────────────────────────────────┐
│                    COMBAT SEQUENCE                          │
│                                                             │
│  Phase 1: Detection (0-5 seconds)                           │
│  - Radar/sonar detects enemy presence                       │
│  - Visual enhancement determines engagement range           │
│  - Thermal imaging reveals hidden enemies                   │
│                                                             │
│  Phase 2: Positioning (5-15 seconds)                        │
│  - Issue formation commands (Line, Column, Diamond, V-Shape)|
│  - Deploy turrets and defensive structures                  │
│  - Launch aerial scouts for flanking                        │
│                                                             │
│  Phase 3: Engagement (15-60 seconds)                        │
│  - Open fire with available weapons                         │
│  - Use terrain cover for +20% defense bonus                 │
│  - Deploy special abilities (EMP, ion storm, orbital strike)|
│                                                             │
│  Phase 4: Resolution (60-120 seconds)                       │
│  - Assess casualties and retreat wounded                    │
│  - Scavenge enemy wreckage for materials                    │
│  - Collect dungeon blueprints from defeated bosses          │
│  - Repair defensive structures                              │
└─────────────────────────────────────────────────────────────┘
```

### Unit Upgrade Priority — Mid Game [See Upgrade System](../../units/Upgrade-System/README.md)

| Tier | Upgrade Focus | Material Cost | Combat Impact |
|------|--------------|---------------|---------------|
| TIR 2 | Armor 1→2 (+50% HP, +10 Defense) | 80 minerals | Significant survivability increase |
| TIR 2 | Weapons I → Basic turrets unlocked | 150 minerals | +50% damage output |
| TIR 3 | Armor 2→3 (+100% total HP) | 150 titanium | Heavy units become tank-like |
| TIR 3 | Vision upgrades (+100% detection) | 300 construction, 200 minerals | Early warning against ambushes |

---

## Phase 4: Interplanetary Expansion (TIR 3-4)

### Core Loop Cycle — Late Game

```
┌─────────────────────────────────────────────────────────────┐
│                    LATE GAME LOOP                           │
│                                                             │
│  Morning:                                                   │
│  1. Review multi-planet production metrics                  │
│  2. Allocate resources between home planet and colonies     │
│  3. Launch deep space scouts with quantum scanner           │
│                                                             │
│  Midday:                                                    │
│  4. Deploy mech squads to secure high-TIR planets           │
│  5. Establish mining operations on resource-rich worlds     │
│  6. Engage elite enemy forces in TIR 4+ combat              │
│  7. Extract advanced blueprints from inner ring dungeons    │
│                                                             │
│  Evening:                                                   │
│  8. Conduct research at Dark Matter Lab                     │
│  9. Construct Void Lab for TIR 5 technology                 │
│ 10. Manufacture heavy mechs and fighter jets                │
│ 11. Upgrade dropship to mothership status                   │
│                                                             │
│  Night:                                                     │
│  12. Monitor satellite constellation for planet-wide threats|
│  13. Launch orbital strike on enemy stronghold              │
│  14. Quantum gate transfer units between bases instantly    │
└─────────────────────────────────────────────────────────────┘
```

### Interplanetary Travel — Fuel Management [See Navigation](../../Planets/Navigation/README.md)

| Travel Type | Fuel Required | Energy Required | Scan Time Before Landing |
|-------------|--------------|-----------------|-------------------------|
| Intra-system hop | 100 units (baseline) | 20 energy for navigation | 5 minutes basic scan |
| Inter-system jump | 500 units (baseline) | 100 energy for navigation | 15 minutes deep scan |
| Precise planet landing | 500-750 units | 100 energy + 50 extra | 20 minutes detailed scan |
| Wormhole transit | 10 units | 100 energy (initiation) | Instant (wormhole pre-mapped) |

### Planet Scanning Requirements Before Landing [See Navigation](../../Planets/Navigation/README.md)

| Scan Tier | Energy Cost | Time Required | Information Revealed |
|-----------|-------------|---------------|---------------------|
| Basic Radar (TIR 1) | 10 energy | 5 minutes | Biome type, basic resources |
| Deep Sonar (TIR 2) | 25 energy | 10 minutes | Underground structures, dungeon locations |
| Satellite Array (TIR 3) | 40 energy/turn | Continuous | Full planet monitoring, enemy positions |
| Quantum Scanner (TIR 5) | 100 energy one-time | Instant | Complete map reveal, all features visible |

### Multi-Base Resource Flow

```
Home Planet (TIR 5 Fortress)
    │
    ├──→ Colony Alpha (TIR 3 Mining Outpost) ──→ 500 minerals/turn
    │       │
    │       └──→ Quantum Gate transfer to home base
    │
    ├──→ Colony Beta (TIR 2 Fuel Production) ──→ 200 fuel/turn
    │       │
    │       └──→ Fuel tanker delivery every 3 turns
    │
    └──→ Colony Gamma (TIR 4 Military Base) ──→ 1 mech squad/week
            │
            └──→ Reinforcements via Quantum Gate Network
```

### Research Priority — Late Game [See Tech Tree](../../Tech-Tree/Overview/README.md)

| Priority | Research Topic | Time Required | Strategic Impact |
|----------|---------------|---------------|-----------------|
| 1 | Void Warp Drive | 30 minutes | Instant travel within 5-system radius |
| 2 | Zero-Point Energy | 30 minutes | Near-infinite energy production |
| 3 | Heavy Mech Production | 25 minutes | Ultimate ground combat unit |
| 4 | Orbital Strike Beacon | 20 minutes | Planet-wide bombardment capability |
| 5 | Alien Artifact Decoder | 30 minutes | Center galaxy ruins blueprints |

---

## Phase 5: Galaxy Conquest (TIR 5)

### Core Loop Cycle — End Game

```
┌─────────────────────────────────────────────────────────────┐
│                    END GAME LOOP                            │
│                                                             │
│  Morning:                                                   │
│  1. Review galaxy-wide production network                   │
│  2. Launch Dyson Swarm component for +50% research speed    │
│  3. Deploy Void-Trooper elite squad to final assault planet │
│                                                             │
│  Midday:                                                    │
│  4. Engage alien boss force in epic battle                  │
│  5. Use all available units (mechs, fighters, drones)       │
│  6. Activate orbital strikes and ion storms                 │
│  7. Extract alien technology from defeated forces           │
│                                                             │
│  Evening:                                                   │
│  8. Research Alien Warp Integration for unlimited travel    │
│  9. Construct Alien Artifact Decoder on center galaxy ruins │
│ 10. Decode final blueprints for victory                     │
│                                                             │
│  Night:                                                     │
│  11. Monitor temporal radar for enemy movement prediction   │
│  12. Launch pre-emptive orbital strike on alien fortress    │
│  13. Quantum gate transfer elite reinforcements instantly   │
└─────────────────────────────────────────────────────────────┘
```

### Final Assault Preparation Checklist

| Requirement | Status | Notes |
|-------------|--------|-------|
| Void-Infused Armor on all units | ☐ Pending | Requires Dark Matter Crystals from Synthesizer |
| Void Beam weapons equipped | ☐ Pending | Research requires Precision Targeting + Torpedo Launcher |
| Alien Shield Generator installed | ☐ Pending | Immune to physical damage, 50% energy resistance |
| Heavy Mech squad (minimum 3) | ☐ Pending | Each mech: 500 HP, dual plasma cannons, shield generator |
| Fighter jet wing (minimum 12) | ☐ Pending | Plasma cannon + 4 missiles per jet |
| Orbital Strike Beacons (minimum 2) | ☐ Pending | Requires satellite deployment for targeting lock |
| Dropship fully upgraded [See Dropship](../../Spaceship/Dropship/README.md) | ☐ Pending | Composite hull, Alien Shield, Void Warp Drive |
| Quantum Gate Network established | ☐ Pending | Instant unit transport between linked gates |

### Victory Conditions

| Condition | Requirement | Reward |
|-----------|-------------|--------|
| Defeat Alien Boss | Destroy boss unit on center galaxy planet | Unlocks alien warp integration |
| Decode All Artifacts | 10+ blueprints from Alien Artifact Decoder | Grants unique alien technologies |
| Repair Dropship to TIR 5 | Full mothership upgrade complete | Enables travel to alien galaxy |
| Construct Dyson Swarm | Complete endgame research | Permanent +50% all research speed |

### Post-Victory: Alien Galaxy Expansion [See Spaceship Progression](../../Spaceship/Progression/README.md)

After defeating the aliens and repairing the dropship:
1. **Warp transit** — 8 seconds opening wormhole + 3 seconds through it [See Navigation](../../Planets/Navigation/README.md)
2. **Arrive in alien galaxy** — New starfield, abandoned space stations, energy fields
3. **New progression curve** — Similar TIR system but with energy-based challenges
4. **Abandoned ships** — Salvage for advanced blueprints and rare materials
5. **Warfields** | Battle-scarred planets with elite alien remnants

---

## Game Length Estimates

| Phase | Estimated Playtime | Key Milestones |
|-------|-------------------|----------------|
| Crash to Ring 2 Unlock | 2-4 hours | Tutorial, first base, drive repair |
| Ring 2 to Ring 3 Unlock | 8-15 hours | First factory, heavy vehicles, inter-system travel |
| Ring 3 to Ring 4 Unlock | 15-30 hours | Dark matter lab, wormhole discovery, galaxy center approach |
| Ring 4 to Victory | 20-40 hours | Void lab construction, alien boss defeat, warp to alien galaxy |
| **Total Main Story** | **45-89 hours** | Varies by exploration depth and difficulty |
| **100% Completion** | **100-150+ hours** | All dungeons, all blueprints, all planets visited |

---

## Difficulty Scaling

### Easy Mode Modifiers
- +25% resource generation rates
- -20% enemy unit stats
- +30% research speed
- Dungeons have 25% fewer enemies but same loot

### Normal Mode (Baseline)
- Standard values as documented in all briefing files
- Balanced progression curve
- Moderate dungeon difficulty

### Hard Mode Modifiers
- -25% resource generation rates
- +25% enemy unit stats
- -20% research speed
- Dungeons have 50% more enemies, elite variants common
- Nighttime attacks deal 2x damage

### Extreme Mode Modifiers
- -40% resource generation rates
- +50% enemy unit stats
- -35% research speed
- Dungeons feature boss-tier enemies throughout
- Planetary events occur twice as frequently
- Survival consumption doubled

---

## See Also

- [Dungeon types, loot tables, and blueprint rewards](../Dungeons/Dungeon-Overview/README.md)
- [Space travel mechanics and events](../Space-Travel/README.md)
- [Dropship layout and component breakdown](../../Spaceship/Dropship/README.md)
- [Ship evolution across TIR tiers](../../Spaceship/Progression/README.md)
- [Radial tech tree structure and ring progression](../../Tech-Tree/Overview/README.md)
- [Detailed research topics by category](../../Tech-Tree/Research-Categories/README.md)
