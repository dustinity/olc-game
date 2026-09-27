# Tech Tree Overview — Radial Research System with Topic Clustering

3-4 ring radial tech tree from mid-inner to outer, organized by research category with prerequisite chains.

---

## Tech Tree Structure

### Radial Ring Design

The technology tree is organized as concentric rings radiating outward from a central core. Each ring represents a TIR progression tier and contains clustered research topics.

```
                ┌─────────────────────────┐
                │   OUTER RING (TIR 4-5)  │
                │  Void Lab Research      │
                │  • Void Weapons         │
                │  • Void Drives          │
                │  • Alien Integration    │
                └───────────┬─────────────┘
                            │
                ┌───────────▼─────────────┐
                │  RING 3 (TIR 3-4)       │
                │  Dark Matter Lab        │
                │  • Energy Advanced      │
                │  • Armor Elite          │
                │  • Weapons III          │
                └───────────┬─────────────┘
                            │
                ┌───────────▼─────────────┐
                │  RING 2 (TIR 2-3)       │
                │  Energy Lab             │
                │  • Weapons II           │
                │  • Armor Advanced       │
                │  • Vision II            │
                └───────────┬─────────────┘
                            │
                ┌───────────▼─────────────┐
                │  RING 1 (TIR 1-2)       │
                │  Basic Forge            │
                │  • Weapons I            │
                │  • Armor Basic          │
                │  • Vision I             │
                └───────────┬─────────────┘
                            │
                ┌───────────▼─────────────┐
                │    CORE (Starting)      │
                │  Dropship Systems       │
                │  • Basic Combat         │
                │  • Navigation           │
                │  • Resource Basics      │
                └─────────────────────────┘
```

### Ring Progression Requirements

| Ring | TIR Range | Research Building | Unlock Requirement |
|------|-----------|-------------------|--------------------|
| Core | Starting | Dropship systems | Available at game start |
| Ring 1 | TIR 1-2 | Basic Forge (TIR 2) | Complete 2 small dungeons OR forge basic research |
| Ring 2 | TIR 2-3 | Energy Lab (TIR 3) | Complete Ring 1 topics + build Energy Lab |
| Ring 3 | TIR 3-4 | Dark Matter Lab (TIR 4) | Complete Ring 2 topics + build Dark Matter Lab |
| Outer | TIR 4-5 | Void Lab (TIR 5) | Complete Ring 3 topics + build Void Lab |

---

## Research Topic Clusters

Each ring contains clustered research topics. Topics within a cluster are interconnected — completing one unlocks related options within the same cluster.

### Cluster Categories

| Cluster | Core Content | Ring 1 | Ring 2 | Ring 3 | Outer Ring |
|---------|-------------|--------|--------|--------|------------|
| **Weapons** | Weapon types, ammo, turrets | Weapons I | Weapons II | Weapons III | Weapons V |
| **Armor** | Unit protection, ship hull | Armor Basic | Armor Advanced | Armor Elite | Armor Master |
| **Drives** | Travel speed, fuel efficiency | Basic Drive | Drive II | Drive III | Void Warp |
| **Energy** | Power generation, storage | Basic Power | Energy II | Energy III | Energy V |
| **Vision** | Detection, targeting | Vision I | Vision II | Vision III | Quantum Scan |
| **Storage** | Resource capacity, protection | Basic Storage | Storage II | Storage III | Black Hole |
| **Buildings** | Planet facility types | Basic Buildings | Advanced Bldg | Elite Bldg | Alien Bldg |
| **Units** | Unit types, capabilities | Basic Units | Unit II | Unit III | Unit V |

---

## Core Ring — Starting Technology

### Available at Game Start (No Research Required)

| Topic | Description | Unlock |
|-------|-------------|--------|
| Basic Combat | Ballistic rifle, pistol, basic tactics | Dropship start |
| Navigation Basics | Planet map, basic orientation | Dropship start |
| Resource Basics | Mining hand tools, manual collection | Dropship start |
| Dropship Systems | Cockpit, basic life support, cryo pods | Dropship start |
| Basic Construction | Hand placement of walls, floors | Dropship start |

### Starting Limitations
- No weapon upgrades available
- No building research beyond placement
- Manual resource gathering only
- Single drive operational (left damaged)
- 50 energy capacity limits simultaneous systems

---

## Ring 1 — Basic Forge Research (TIR 1-2)

### Research Building Required: Basic Forge [See Planet Buildings](../../buildings/README.md)

### Unlock Path to Ring 1
Complete at least 2 of the following:
- Explore 1 small dungeon (abandoned house or camp)
- Explore 1 medium dungeon (military outpost or lost bunker)
- Discover 3 new resource deposit types on planet surface
- Defeat first hostile encounter boss/elite unit

### Ring 1 Research Topics

#### Weapons I Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Ballistic Turret Mounting | 2 min | 100 construction, 80 minerals | Enable turret installation on dropship and planet bases |
| Ammo Crafting I | 3 min | 50 minerals, 30 energy | Unlock standard ammunition production |
| Rocket Bay Basic | 5 min | 200 construction, 150 minerals | Enable small rocket bay for ship and turrets |

#### Armor Basic Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Reinforced Plating I | 3 min | 150 construction, 100 minerals | Unit armor upgrade tier 1→2 available |
| Hull Repair Basics | 2 min | 100 construction, 50 minerals | Restore hull integrity to 85%+ on dropship |
| Basic Shield Generation | 5 min | 300 construction, 200 minerals | +100 HP equivalent energy shield [See Ship Modules](../../ShipModules/README.md) |

#### Drive Basics Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Left Drive Repair | 15 min | 100 construction, 50 minerals, 30 energy | Restore left rocket drive to operational status [See Dropship](../../Spaceship/Dropship/README.md) |
| Fuel Processing I | 5 min | 100 construction, 80 minerals | Enable oil pump and basic fuel refinement [See Resources](../../resources/README.md) |
| Drive Calibration I | 3 min | 50 minerals, 20 energy | +10% drive efficiency (both drives) |

#### Energy Basics Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Solar Panel Installation | 3 min | 80 construction, 60 minerals | Enable solar array power generation [See Planet Buildings](../../buildings/README.md) |
| Battery Storage I | 2 min | 100 construction, 80 minerals | +50 energy capacity on dropship |
| Power Distribution I | 3 min | 150 construction, 100 minerals | Enable weapon power routing from core [See Weapons](../../weapons/Weapon-Types/README.md) |

#### Vision Basics Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Basic Radar Array | 5 min | 200 construction, 150 minerals | Short-medium radial scan range [See Navigation](../../Planets/Navigation/README.md) |
| Visual Enhancement I | 2 min | 50 minerals, 20 energy | +20% unit detection radius |
| Signal Processing I | 3 min | 100 construction, 80 minerals | Radar clutter reduction, -15% false positives |

#### Storage Basics Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Container Placement | 2 min | 300 construction, 100 minerals | Enable wheel-shaped container storage [See Storage System](../../buildings/Reference/Storage-System.md) |
| Storage Organization I | 1 min | None (knowledge only) | UI improvements for inventory management |
| Resource Labeling | 1 min | None (auto-unlock) | Automatic resource type categorization in storage UI |

#### Building Basics Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Wall and Gate Placement | 2 min | 80 construction, 50 minerals per wall segment | Defensive structure building [See Planet Buildings](../../buildings/README.md) |
| Mine Placement | 3 min | 100 construction, 80 minerals | Enable mine placement on discovered deposit types |
| Basic Power Grid | 3 min | 150 construction, 100 minerals | Connect buildings to centralized power source |

#### Unit Basics Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Soldier Training I | 5 min | 200 construction, 150 minerals, 50 survival | Unlock basic soldier training at barracks [See Units](../../units/README.md) |
| Unit Control Groups I | 3 min | Command Center required | Enable numbered unit group assignment |
| Basic Formation Commands | 3 min | None (auto-unlock with Command Center) | Squad movement and combat formations |

---

## Ring 2 — Energy Lab Research (TIR 2-3)

### Research Building Required: Energy Lab [See Planet Buildings](../../buildings/README.md)

### Unlock Path to Ring 2
Complete at least 3 of the following:
- Complete 1 large dungeon (hospital complex or alien structure)
- Build and operate 1 mine + 1 power plant continuously for 30 minutes
- Discover and scan 5 new planets via solar system navigation [See Navigation](../../Planets/Navigation/README.md)
- Defeat TIR 2 elite enemy unit or boss

### Ring 2 Research Topics

#### Weapons II Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Plasma Cannon Mounting | 8 min | 400 construction, 300 minerals | Energy weapon deployment on ship and bases [See Weapons](../../weapons/Weapon-Types/README.md) |
| Ammo Crafting II | 5 min | 200 processed metals, 100 energy | Unlock armor-piercing and incendiary rounds |
| Rocket Bay Advanced | 10 min | 600 construction, 400 minerals | Enable large rocket bay with warhead variety [See Ship Modules](../../ShipModules/README.md) |

#### Armor Advanced Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Titanium Composite Armor | 10 min | 800 construction, 500 minerals, 200 energy | Unit armor upgrade tier 2→3 (+150% total HP) [See Upgrade System](../../units/Upgrade-System/README.md) |
| Hull Reinforcement II | 8 min | 600 construction, 400 minerals | Heat-resistant alloy hull for dropship [See Dropship](../../Spaceship/Dropship/README.md) |
| Energy Shield Generation II | 12 min | 500 construction, 300 minerals, 200 energy | Absorbs 200 damage/second shield [See Ship Modules](../../ShipModules/README.md) |

#### Drive Advanced Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Atomic Reactor Drive | 15 min | 800 construction, 600 minerals | -40% fuel consumption, 1.5 hops per tank [See Ship Modules](../../ShipModules/README.md) |
| Fuel Processing II | 8 min | 400 construction, 300 minerals | Enable advanced fuel types (biofuel, frozen methane) [See Resources](../../resources/README.md) |
| Drive Calibration II | 5 min | 200 minerals, 100 energy | +20% drive efficiency (stacks with I bonus) |

#### Energy Advanced Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Wind Turbine Optimization | 8 min | 400 construction, 300 minerals | +25% output in windy biomes [See Planet Buildings](../../buildings/README.md) |
| Battery Storage II | 5 min | 300 construction, 200 minerals | +200 energy capacity on dropship (total 250+) |
| Power Distribution II | 8 min | 500 construction, 400 minerals | Enable multiple simultaneous weapon systems [See Weapons](../../weapons/Weapon-Types/README.md) |

#### Vision Advanced Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Deep Sonar Array | 10 min | 500 construction, 400 minerals | Underground structure detection [See Navigation](../../Planets/Navigation/README.md) |
| Thermal Imaging | 8 min | 300 construction, 200 minerals | +100% detection range through terrain cover |
| Laser Targeting System | 12 min | 600 construction, 500 minerals | Precise single-target lock for weapons [See Ship Modules](../../ShipModules/README.md) |

#### Storage Advanced Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Haul Storage Construction | 10 min | 1,500 construction, 500 minerals | 10,000 resources per type capacity [See Storage System](../../buildings/Reference/Storage-System.md) |
| Storage Organization II | 5 min | None (knowledge only) | Advanced filtering and search in storage UI |
| Resource Conversion I | 8 min | 800 construction, 600 minerals | Convert between resource types at 70% yield [See Planet Buildings](../../buildings/README.md) |

#### Building Advanced Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Factory Construction | 15 min | 800 construction, 500 minerals, 200 energy | Vehicle and module production [See Planet Buildings](../../buildings/README.md) |
| Forge Upgrade Path | 10 min | 600 construction, 400 minerals | Enable Energy Lab construction from forge |
| Advanced Power Grid | 8 min | 500 construction, 400 minerals | Automated power routing and load balancing |

#### Unit Advanced Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Heavy Vehicle Production | 15 min | 800 construction, 600 minerals, 200 fuel | Tank and walker training at factory [See Units](../../units/README.md) |
| Unit Control Groups II | 5 min | Intelligence stat requirement: 3+ | +3 additional control group slots |
| Aerial Unit Deployment | 10 min | Airfield required, 400 construction, 300 minerals | Scout drone and fighter training [See Planet Buildings](../../buildings/README.md) |

---

## Ring 3 — Dark Matter Lab Research (TIR 3-4)

### Research Building Required: Dark Matter Lab [See Planet Buildings](../../buildings/README.md)

### Unlock Path to Ring 3
Complete at least 3 of the following:
- Complete dungeon in TIR 4+ planet (inner ring world)
- Build Energy Lab and research all Ring 2 topics
- Discover wormhole system between solar systems [See Navigation](../../Planets/Navigation/README.md)
- Defeat TIR 3 boss with elite unit composition

### Ring 3 Research Topics

#### Weapons III Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Torpedo Launcher | 15 min | 800 construction, 600 minerals, 200 energy | Homing multi-target weapon system [See Weapons](../../weapons/Weapon-Types/README.md) |
| Precision Targeting | 12 min | 600 construction, 400 minerals, 200 energy | +100% damage, +40% range, +25% accuracy [See Upgrade System](../../units/Upgrade-System/README.md) |
| Ion Weapon Integration | 15 min | 700 construction, 500 minerals, 300 energy | EMP and ion storm capabilities [See Weapons](../../weapons/Weapon-Types/README.md) |

#### Armor Elite Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Energy-Infused Armor | 15 min | 1,200 construction, 800 minerals, 500 energy | +350% total HP, 10% melee damage reflection [See Upgrade System](../../units/Upgrade-System/README.md) |
| Hull Reinforcement III | 12 min | 1,000 construction, 700 minerals | Titanium alloy hull, high heat resistance [See Dropship](../../Spaceship/Dropship/README.md) |
| Advanced Shield Generation | 18 min | 800 construction, 600 minerals, 400 energy | Expanded shield coverage, toggleable efficiency mode |

#### Drive Elite Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Ionic Reactor Drive | 20 min | 1,200 construction, 800 minerals | -60% fuel consumption, fast speed [See Ship Modules](../../ShipModules/README.md) |
| Fuel Processing III | 12 min | 600 construction, 500 minerals | Dark matter fuel efficiency +25% [See Resources](../../resources/README.md) |
| Drive Calibration III | 8 min | 400 minerals, 300 energy | +30% drive efficiency total (stacked bonuses) |

#### Energy Elite Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Geothermal Vent Harnessing | 15 min | 800 construction, 600 minerals | Very high energy output from planetary vents [See Planet Buildings](../../buildings/README.md) |
| Battery Storage III | 8 min | 600 construction, 400 minerals | +500 energy capacity on dropship (total 750+) |
| Power Distribution III | 12 min | 800 construction, 600 minerals | Enable Void Beam power requirements [See Weapons](../../weapons/Weapon-Types/README.md) |

#### Vision Elite Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Satellite Deployment Bay | 15 min | 800 construction, 600 minerals | Planet-wide monitoring with up to 3 satellites [See Ship Modules](../../ShipModules/README.md) |
| Tactical AI Assistant | 12 min | 600 construction, 400 minerals, 200 dark matter crystals | Auto-target priority, +150% detection range |
| Quantum Range Finder | 10 min | 500 construction, 300 minerals | Instant distance calculation for all map features |

#### Storage Elite Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Reinforced Vault | 15 min | 3,000 construction, 2,000 minerals | 25,000 resources per type, crash protection [See Storage System](../../buildings/Reference/Storage-System.md) |
| Resource Conversion II | 10 min | 800 construction, 600 minerals | Improved conversion at 75% yield (up from 70%) |
| Remote Transfer Network | 12 min | 1,000 construction, 800 minerals | Quantum Gate resource sharing between bases [See Planet Buildings](../../buildings/README.md) |

#### Building Elite Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Void Lab Construction | 20 min | 2,000 construction, 1,500 minerals, 500 energy | TIR 5 research facility [See Planet Buildings](../../buildings/README.md) |
| Assembly Plant | 25 min | 3,000 construction, 2,000 minerals, 1,000 energy | 3x factory production rate [See Planet Buildings](../../buildings/README.md) |
| Orbital Strike Beacon | 20 min | 2,000 construction, 1,500 minerals, 800 energy | Planet-wide bombardment capability [See Ship Modules](../../ShipModules/README.md) |

#### Unit Elite Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Heavy Mech Production | 25 min | 2,500 construction, 1,500 minerals, 500 fuel | Ultimate ground combat unit [See Units](../../units/README.md) |
| Unit Control Groups III | 8 min | Intelligence stat requirement: 6+ | +5 additional control group slots (total 10+) |
| Elite Squad Training | 15 min | 1,000 construction, 800 minerals, 300 survival | Shock Trooper and elite variant training |

---

## Outer Ring — Void Lab Research (TIR 4-5)

### Research Building Required: Void Lab [See Planet Buildings](../../buildings/README.md)

### Unlock Path to Outer Ring
Complete at least 4 of the following:
- Complete dungeon in TIR 5 planet (inner ring fortress)
- Build Dark Matter Lab and research all Ring 3 topics
- Reach galaxy center system (TIR 5 planet)
- Defeat TIR 4 boss guarding inner ring access

### Outer Ring Research Topics

#### Weapons V Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Void Beam | 20 min | 1,500 construction, 1,000 minerals, 500 energy | Extreme damage, ignores all armor [See Weapons](../../weapons/Weapon-Types/README.md) |
| Void Core Weaponry | 25 min | 2,000 construction, 1,500 dark matter crystals | +150% damage, +60% range, ignores 30% enemy armor [See Upgrade System](../../units/Upgrade-System/README.md) |
| Gravity Well Emitter (Alien) | 30 min | 2,500 construction, 2,000 dark matter crystals | Ultimate area control, implosion damage [See Weapons](../../weapons/Weapon-Types/README.md) |

#### Armor Master Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Void-Infused Armor | 25 min | 3,000 construction, 1,500 dark matter crystals | +600% total HP, +60 defense, 20% damage reflection [See Upgrade System](../../units/Upgrade-System/README.md) |
| Reinforced Composite Hull | 20 min | 3,000 construction, 2,000 minerals | Maximum standard hull tier, resists all known damage types [See Dropship](../../Spaceship/Dropship/README.md) |
| Alien Shield Generator | 30 min | 5,000 construction, 3,000 dark matter crystals | Immune to physical/projectile damage [See Ship Modules](../../ShipModules/README.md) |

#### Drive Void Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Energy Stream Drive | 25 min | 1,800 construction, 1,200 minerals | -80% fuel consumption, very fast speed [See Ship Modules](../../ShipModules/README.md) |
| Void Warp Drive | 30 min | 2,500 construction, 2,000 dark matter crystals | Instant travel within 5-system radius [See Ship Modules](../../ShipModules/README.md) |
| Alien Warp Integration | 40 min | 4,000 construction, 3,000 dark matter crystals | Unlimited range wormhole transit, 0.01x fuel [See Navigation](../../Planets/Navigation/README.md) |

#### Energy V Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Crystal Synthesizer Integration | 20 min | 1,500 construction, 800 crystal minerals | Converts raw energy to dark matter crystals [See Planet Buildings](../../buildings/README.md) |
| Battery Storage V | 10 min | 1,000 construction, 800 minerals | +1,000 energy capacity on dropship (total 1,500+) |
| Zero-Point Energy Extraction | 30 min | 2,000 construction, 1,500 dark matter crystals | Near-infinite energy from vacuum fluctuations |

#### Vision Quantum Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Quantum Scanner | 20 min | 2,000 construction, 1,500 minerals | Planet-wide instant map reveal [See Ship Modules](../../ShipModules/README.md) |
| Deep Void Sensors | 15 min | 1,000 construction, 800 dark matter crystals | Detect objects across light-years in space |
| Temporal Radar | 25 min | 1,500 construction, 1,200 dark matter crystals | Predict enemy movement patterns 3 seconds ahead |

#### Storage Ultimate Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Black Hole Storage | 30 min | 5,000 construction, 3,000 dark matter crystals | Effectively unlimited while powered [See Storage System](../../buildings/Reference/Storage-System.md) |
| Cross-Planet Sync Network | 20 min | 2,000 construction, 1,500 minerals | All storage emitters share inventory across planets |
| Matter Compressor | 15 min | 1,500 construction, 1,000 minerals | Reduce bulk items to 10% original volume |

#### Building Alien Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Alien Artifact Decoder | 30 min | 3,000 construction, 2,000 dark matter crystals | Decode center galaxy ruins for unique blueprints [See Planet Buildings](../../buildings/README.md) |
| Quantum Gate Network | 40 min | 8,000 construction, 6,000 minerals, 4,000 energy | Instant unit transport between linked gates [See Planet Buildings](../../buildings/README.md) |
| Dyson Swarm Component (Endgame) | 60 min | 10,000 construction, 8,000 dark matter crystals | Capture star energy for permanent +50% all research speed |

#### Unit Ultimate Cluster
| Research | Time | Materials | Effect |
|----------|------|-----------|--------|
| Void-Trooper Elite Squad | 30 min | 2,000 construction, 1,500 dark matter crystals, 500 survival | TIR 5 infantry with void-infused armor and weapons [See Units](../../units/README.md) |
| Unit Control Groups V | 10 min | Intelligence stat requirement: 10+ | Unlimited control group slots |
| Autonomous Drone Army | 40 min | 3,000 construction, 2,500 dark matter crystals, 1,000 fuel | AI-controlled drone fleet operates independently |

---

## Research Prerequisite Chains

### Weapons Progression Chain
```
Basic Combat (Core)
    ↓
Ballistic Turret (Ring 1)
    ↓
Plasma Cannon (Ring 2)
    ↓
Torpedo Launcher (Ring 3)
    ↓
Void Beam (Outer Ring)
    ↓
Gravity Well Emitter (Outer Ring, requires Alien tech)
```

### Armor Progression Chain
```
Hull Repair Basics (Core)
    ↓
Reinforced Plating I (Ring 1)
    ↓
Titanium Composite (Ring 2)
    ↓
Energy-Infused Armor (Ring 3)
    ↓
Void-Infused Armor (Outer Ring)
    ↓
Alien Shield (Outer Ring, requires center galaxy reward)
```

### Drive Progression Chain
```
Left Drive Repair (Ring 1)
    ↓
Atomic Reactor Drive (Ring 2)
    ↓
Ionic Reactor Drive (Ring 3)
    ↓
Energy Stream Drive (Outer Ring)
    ↓
Void Warp Drive (Outer Ring)
    ↓
Alien Warp Integration (Outer Ring, requires center galaxy reward)
```

---

## Research Speed Modifiers

### Building-Based Bonuses
| Building | Research Speed Bonus | Notes |
|----------|---------------------|-------|
| Basic Forge (TIR 2) | Base speed | Starting research facility |
| Energy Lab (TIR 3) | +25% | Requires Ring 1 complete |
| Dark Matter Lab (TIR 4) | +50% | Requires Ring 2 complete |
| Void Lab (TIR 5) | +100% | Requires Ring 3 complete |

### Faction-Based Bonuses
| Faction | Research Speed Bonus | Special |
|---------|---------------------|---------|
| Neon Punk | +10% material efficiency | Quantum Mine auto-refines 10% of materials |
| Dark Realistic | +15% weapon research speed | Military doctrine prioritizes combat tech |
| Cartoon SciFi | +20% random discovery chance | Wild experiments yield unexpected findings |
| Bright Realistic | +10% all research speed | Civilian efficiency across all fields |

### Champion-Based Bonuses
Champion Intelligence stat directly affects research:
- **Intelligence 1-3:** No bonus (baseline)
- **Intelligence 4-6:** +5% research speed per point
- **Intelligence 7-9:** +10% research speed per point
- **Intelligence 10+:** +20% base + 15% per additional point [See Champions](../../factions/Champions/README.md)

---

## See Also

- [Research category details and unlock paths](../Research-Categories/README.md)
- [Forge building requirements](../../buildings/README.md)
- [Ship module research dependencies](../../ShipModules/README.md)
- [Unit upgrade research prerequisites](../../units/Upgrade-System/README.md)
- [Dungeon blueprint discovery methods](../../Gameplay/Dungeons/Dungeon-Overview/README.md)
