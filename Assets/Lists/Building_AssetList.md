# Planet Buildings Asset List

Generated from Briefing documentation for **Our Last Chance** game project.

This asset list catalogs all planet buildings required for development, organized by tier and category. Each building includes specifications for 3D modeling, AI image generation, and Unreal Engine implementation.

---

## ID Naming Convention

All assets use the following hierarchical ID system:

| Prefix | Category | Examples |
|--------|----------|----------|
| `PB-EX` | Resource Extraction | PB-EX-01, PB-EX-02, PB-EX-03 |
| `PB-PG` | Power Generation | PB-PG-01 through PB-PG-06 |
| `PB-IN` | Infrastructure | PB-IN-01 through PB-IN-05 |
| `PB-ST` | Storage | PB-ST-01 through PB-ST-03 |
| `PB-PF` | Production Facilities | PB-PF-01 through PB-PF-04 |
| `PB-DS` | Defense Structures | PB-DS-01 through PB-DS-03 |
| `PB-SB` | Support Buildings | PB-SB-01 through PB-SB-03 |
| `PB-HP` | High-Tier Production (Elite) | PB-HP-01 through PB-HP-03 |
| `PB-SD` | Super Defense (Elite) | PB-SD-01 through PB-SD-03 |
| `PB-SP` | Special Buildings (Elite) | PB-SP-01 through PB-SP-03 |
| `PB-FS` | Faction Signature Buildings | PB-FS-01 through PB-FS-04 |

**Format:** `PB-{CATEGORY}-{SEQ}` where `PB` = Planet Building, `{CATEGORY}` = category code, `{SEQ}` = sequential number within category.

---

## Style References

### Style 1: Bright Semi-Realistic Sci-Fi
- Bright daylight, clean painted steel, functional engineering
- White, gray and yellow color palette with blue holographic interfaces
- Minimal rust, high gameplay readability
- Modular construction, light weathering

### Style 2: Realistic Industrial Sci-Fi
- Heavy engineering, realistic proportions, industrial complexity
- Darker metals, concrete, dust, oil stains, worn steel
- Military functionality, late afternoon lighting
- Still readable from RTS camera distance

---

## Table of Contents

1. [Default Buildings (TIR 1-2)](#default-buildings-tir-1-2)
   - [Resource Extraction](#resource-extraction)
   - [Power Generation](#power-generation)
   - [Infrastructure](#infrastructure)
   - [Default Storage](#default-storage)
2. [Advanced Buildings (TIR 2-3)](#advanced-buildings-tir-2-3)
   - [Production Facilities](#production-facilities)
   - [Defense Structures](#defense-structures)
   - [Support Buildings](#support-buildings)
3. [Elite Buildings (TIR 4-5)](#elite-buildings-tir-4-5)
   - [High-Tier Production](#high-tier-production)
   - [Super Defense](#super-defense)
   - [Special Buildings](#special-buildings)
4. [Faction Signature Buildings](#faction-signature-buildings)
5. [Building Variants Summary](#building-variants-summary)

---

## Default Buildings (TIR 1-2)

### Resource Extraction

#### PB-EX-01: Mine
| Property | Value |
|----------|-------|
| Asset ID | PB-EX-01 |
| Grid Size | 1x1 |
| Cost | 100 construction material, 50 minerals |
| Power | Requires 10 energy/turn |
| Category | Resource Extraction |

**Asset Requirements:**
- [ ] Base mine structure (1x1 footprint) — `PB-EX-01-Main`
- [ ] Stone Mine variant — `PB-EX-01-Variant-Stone`
- [ ] Crystal Mine variant — `PB-EX-01-Variant-Crystal`
- [x] Metal Ore Mine variant — `PB-EX-01-Variant-MetalOre` → **[metal-ore-mine.png](Style/Stil-1/Buildings/metal-ore-mine.png)** (1254×1254 RGBA, validated clean transparency)
- [ ] Coal Mine variant — `PB-EX-01-Variant-Coal`
- [ ] Visual indicators: smoke/light effects per mineral type (black=coal, yellow=sulfur, green=uranium) — `PB-EX-01-VFX-SmokeEffects`
- [ ] Conveyor belt attachment points — `PB-EX-01-Prop-Conveyor`
- [x] Support frame and drill mechanism — `PB-EX-01-Mech-Drill` (included in metal-ore-mine.png asset)

**Visual Notes:** Massive drill entering the ground with heavy support frame. Dust extraction systems active. Service vehicles nearby. Metal Ore variant features a heavy rotary drill with torque-braced support frame, sealed bore collar, and twin extraction/filter stacks with clean VFX outlet points for mineral-specific smoke/light effects.

---

#### PB-EX-02: Oil Pump
| Property | Value |
|----------|-------|
| Asset ID | PB-EX-02 |
| Grid Size | 1x1 |
| Cost | 200 construction material, 100 minerals |
| Power | Requires 15 energy/turn |
| Category | Resource Extraction |

**Asset Requirements:**
- [x] Modern pumpjack structure — `PB-EX-02-Main` → **[oil-pump.png](Style/Stil-1/Buildings/oil-pump.png)** (1402×1122 RGBA, validated clean matte edges)
- [ ] Moving pump arm animation (vertical oscillation) — `PB-EX-02-Anim-PumpArm` (animation-driven at runtime)
- [ ] Storage tanks (2x adjacent) — `PB-EX-02-Prop-Tanks`
- [ ] Pipeline connections — `PB-EX-02-Prop-Pipelines`
- [ ] Pressure valves detail — `PB-EX-02-Prop-Valves`
- [ ] Maintenance platform — `PB-EX-02-Prop-MaintenancePlatform` (included in base asset)
- [ ] Small control building attachment — `PB-EX-02-Building-ControlRoom`

**Visual Notes:** Vertical moving pump arm is the key silhouette identifier. Oil stains on ground around base. Asset uses compact pumpjack on rectangular service skid — near-black plated steel, broad readable machinery, tiny amber lamps.

---

#### PB-EX-03: Harvester Post
| Property | Value |
|----------|-------|
| Asset ID | PB-EX-03 |
| Grid Size | 1x1 |
| Cost | 50 construction material, 30 minerals |
| Power | Requires 5 energy/turn |
| Category | Resource Extraction |

**Asset Requirements:**
- [x] Passive collection structure (1x1) — `PB-EX-03-Main` → **[PB-EX-03-Main.png](Style/Stil-1/Buildings/PB-EX-03-Main.png)** (1254×1254 RGBA)
- [x] Ambient dust collector array — `PB-EX-03-Prop-DustCollector` → **[PB-EX-03-Prop-DustCollector.png](Style/Stil-1/Buildings/PB-EX-03-Prop-DustCollector.png)** (1254×1254 RGBA)
- [x] Water condensation tower — `PB-EX-03-Prop-CondensationTower` → **[PB-EX-03-Prop-CondensationTower.png](Style/Stil-1/Buildings/PB-EX-03-Prop-CondensationTower.png)** (1254×1254 RGBA)
- [x] Low-profile antenna/sensor — `PB-EX-03-Prop-Antenna` → **[PB-EX-03-Prop-Antenna.png](Style/Stil-1/Buildings/PB-EX-03-Prop-Antenna.png)** (1254×1254 RGBA)
- [x] Small storage container — `PB-EX-03-Prop-StorageContainer` → **[PB-EX-03-Prop-StorageContainer.png](Style/Stil-1/Buildings/PB-EX-03-Prop-StorageContainer.png)** (1254×1254 RGBA)

**Visual Notes:** Simple, unobtrusive structure. Good for early game visual progression. Complete 5-piece modular set with compact central hub and four capped utility hardpoints — props can be added as player advances through TIR tiers.

---

### Power Generation

#### PB-PG-01: Solar Array
| Property | Value |
|----------|-------|
| Asset ID | PB-PG-01 |
| Grid Size | 2x1 (wheel-shaped rotation) |
| Cost | 80 construction material, 60 minerals |
| Output | Low energy (5/turn base) |
| Category | Power Generation |

**Asset Requirements:**
- [x] Solar panel array (2x1 footprint) — `PB-PG-01-Main` → **[solar-array.png](Style/Stil-1/Buildings/solar-array.png)** (RGBA, circular azimuth bearing + shared tilt axle)
- [ ] Wheel-shaped rotating mount mechanism — `PB-PG-01-Mech-RotatingMount` (animation-driven at runtime)
- [ ] Panel rotation animation (follows sun) — `PB-PG-01-Anim-SunTrack` (animation-driven at runtime)
- [ ] Support struts and joints — `PB-PG-01-Prop-SupportStruts` (included in base asset)
- [ ] Blue energy indicator light — `PB-PG-01-VFX-EnergyIndicator` (visible on generated asset)
- [x] Dust accumulation effect (Desert variant) — `PB-PG-01-Variant-Desert` → **[solar-array-desert.png](Style/Stil-1/Buildings/solar-array-desert.png)**
- [ ] Light snow cover variant — `PB-PG-01-Variant-Snow` → **[solar-array-snow.png](Style/Stil-1/Buildings/solar-array-snow.png)**

**Visual Notes:** Desert biome gets +30% efficiency. Panels should track sunlight angle. Light snow cover variant for Ice/Snow biomes. All variants preserve identical geometry for interchangeable rotation animation.

---

#### PB-PG-02: Wind Turbine
| Property | Value |
|----------|-------|
| Asset ID | PB-PG-02 |
| Grid Size | 2x1 (tall, narrow footprint) |
| Cost | 100 construction material, 80 minerals |
| Output | Medium energy (10/turn base) |
| Category | Power Generation |

**Asset Requirements:**
- [x] Tall turbine tower (narrow base) — `PB-PG-02-Main` → **[wind-turbine.png](Style/Stil-1/Buildings/wind-turbine.png)** (1024×1536 RGBA, animation-ready blade/nacelle split)
- [x] Rotating blade assembly (3 blades) — `PB-PG-02-Mech-Blades` (rotor + hub as one subassembly)
- [ ] Blade rotation animation (continuous) — `PB-PG-02-Anim-Rotate` (animation-driven at runtime)
- [x] Nacelle and housing detail — `PB-PG-02-Prop-Nacelle` (nacelle/yaw housing behind rotor)
- [x] Lightning rod on top — `PB-PG-02-Prop-LightningRod` (distinct from rotor assembly)
- [x] Ice accumulation variant (Ice biome) — `PB-PG-02-Variant-Ice` → **[wind-turbine-ice.png](Style/Stil-1/Buildings/wind-turbine-ice.png)**

**Visual Notes:** Storm bonus +50% output — blades spin faster during weather events. Rocky biome gets +15%. Ice accumulation concentrates on leading edges while keeping moving hub clear for animation compatibility.

---

#### PB-PG-03: Coal Reactor
| Property | Value |
|----------|-------|
| Asset ID | PB-PG-03 |
| Grid Size | 2x2 |
| Cost | 150 construction material, 100 minerals |
| Output | High energy (25/turn base) |
| Category | Power Generation |

**Asset Requirements:**
- [x] Reactor core building (2x2 footprint) — `PB-PG-03-Main` → **[coal-reactor.png](Style/Stil-1/Buildings/coal-reactor.png)** (1402×1122 RGBA, validated clean transparency)
- [x] Coal input hopper/conveyor — `PB-PG-03-Prop-Hopper` (visible coal conveyor belt in asset)
- [ ] Smokestack with emission animation — `PB-PG-03-Anim-SmokeEmission` (VFX outlet point, animation-driven at runtime)
- [ ] Steam vent effects — `PB-PG-03-VFX-SteamVents` (VFX outlet point, animation-driven at runtime)
- [x] Cooling tower attachment — `PB-PG-03-Prop-CoolingTower` (included in base asset)
- [x] Transformer yard nearby — `PB-PG-03-Prop-TransformerYard` (occupies own quadrant in asset)
- [x] Blue reactor glow effect — `PB-PG-03-VFX-ReactorGlow` (contained within core structure, animation layer)

**Visual Notes:** Reliable high output. Steady coal supply visible on conveyor. Smoke/emissions are separate VFX layers for runtime control. Transformer yard occupies its own quadrant.

---

#### PB-PG-04: Water Turbine
| Property | Value |
|----------|-------|
| Asset ID | PB-PG-04 |
| Grid Size | 1x1 (placed on water tile) |
| Cost | 120 construction material, 80 minerals |
| Output | Continuous energy (15/turn base) |
| Category | Power Generation |

**Asset Requirements:**
- [x] Water-mounted turbine structure — `PB-PG-04-Main` → **[B-PG-04-water-turbine.png](Style/Stil-1/Buildings/B-PG-04-water-turbine.png)** (1254×1254 RGBA)
- [x] Rotating water wheel/blades — `PB-PG-04-Mech-WaterWheel` (six-blade paddle rotor between armored pontoons)
- [x] Submerged portion (visible through water) — `PB-PG-04-Variant-Submerged` (submerged blades visible beneath water surface)
- [x] Floating platform/base — `PB-PG-04-Prop-FloatingPlatform` (armored pontoons serve as floating base)
- [ ] Water splash effects — `PB-PG-04-VFX-WaterSplash` (two compact splash points in asset, animation-driven at runtime)
- [x] Cable connection to shore — `PB-PG-04-Prop-CableConnection` (shore cable terminates at tile edge)
- [x] Swamp contamination variant — `PB-PG-04-Variant-Swamp` → **[B-PG-04-water-turbine-swamp.png](Style/Stil-1/Buildings/B-PG-04-water-turbine-swamp.png)**

**Visual Notes:** Requires adjacent water tile. Swamp output reduced by contamination (-30%). Self-contained 1×1 water tiles remain fully opaque for grid alignment.

---

#### PB-PG-05: Geothermal Vent
| Property | Value |
|----------|-------|
| Asset ID | PB-PG-05 |
| Grid Size | 2x2 |
| Cost | 200 construction material, 150 minerals |
| Output | Very high energy (40/turn base) |
| Category | Power Generation |

**Asset Requirements:**
- [ ] Geothermal drilling rig (2x2 footprint) — `PB-PG-05-Main`
- [ ] Steam vent pipes — `PB-PG-05-Prop-SteamPipes`
- [ ] Steam emission animation (continuous) — `PB-PG-05-Anim-SteamEmission`
- [ ] Heat distortion effects — `PB-PG-05-VFX-HeatDistortion`
- [ ] Pressure gauge details — `PB-PG-05-Prop-PressureGauge`
- [ ] Ventilation towers — `PB-PG-05-Prop-VentilationTowers`
- [ ] Ground crack/vent visual near structure — `PB-PG-05-TerraceVentCrack`

**Visual Notes:** Unstable — occasional outage requiring repair. Best output through ice sheets (+50%). Rocky/volcanic biome variant preferred.

---

### Infrastructure

#### PB-IN-01: Camp / Barracks (8-piece modular kit)
| Property | Value |
|----------|-------|
| Asset ID | PB-IN-01 |
| Grid Size | 2x2 |
| Cost | 200 construction material, 100 minerals |
| Power | Requires 10 energy/turn |
| Category | Infrastructure |

**Asset Requirements:**
- [x] Fortified barracks building (2×2 footprint) — `PB-IN-01-Main` → **[PB-IN-01-Main.png](Style/Stil-1/Buildings/PB-IN-01-Main.png)** (1254×1254 RGBA)
- [x] Armored wall sections — `PB-IN-01-Wall-Armored` → **[PB-IN-01-Wall-Armored.png](Style/Stil-1/Buildings/PB-IN-01-Wall-Armored.png)** (1254×1254 RGBA)
- [x] Vehicle parking area — `PB-IN-01-Prop-VehicleParking` → **[PB-IN-01-Prop-VehicleParking.png](Style/Stil-1/Buildings/PB-IN-01-Prop-VehicleParking.png)** (1254×1254 RGBA)
- [x] Training yard equipment — `PB-IN-01-Prop-TrainingEquipment` → **[PB-IN-01-Prop-TrainingEquipment.png](Style/Stil-1/Buildings/PB-IN-01-Prop-TrainingEquipment.png)** (1254×1254 RGBA)
- [x] Observation tower (corner mount) — `PB-IN-01-Tower-Observation` → **[PB-IN-01-Tower-Observation.png](Style/Stil-1/Buildings/PB-IN-01-Tower-Observation.png)** (1254×1254 RGBA)
- [x] Heavy blast doors — `PB-IN-01-Mech-BlastDoors` → **[PB-IN-01-Mech-BlastDoors.png](Style/Stil-1/Buildings/PB-IN-01-Mech-BlastDoors.png)** (1254×1254 RGBA, closed state)
- [x] Flag pole — `PB-IN-01-Prop-FlagPole` → **[PB-IN-01-Prop-FlagPole.png](Style/Stil-1/Buildings/PB-IN-01-Prop-FlagPole.png)** (1254×1254 RGBA)
- [x] Unit entrance/exit animation — `PB-IN-01-Anim-UnitEntrance` → **[PB-IN-01-Anim-UnitEntrance.png](Style/Stil-1/Buildings/PB-IN-01-Anim-UnitEntrance.png)** (1254×1254 RGBA, fully open endpoint keyframe)

**Visual Notes:** Required for producing any soldier units. Dark Realistic faction bonus: training speed +25%. Bright Realistic faction has Hydroponics Bay variant. The 8-piece modular kit communicates progression — observation tower for perimeter awareness, armored walls for defense integration, vehicle parking for transport staging, training equipment for soldier development visuals. Unit entrance animation uses the open-state asset as the fully deployed endpoint keyframe.

---

#### PB-IN-02: Habitation Module (7-piece modular kit)
| Property | Value |
|----------|-------|
| Asset ID | PB-IN-02 |
| Grid Size | 2x2 |
| Cost | 300 construction material, 150 minerals, 50 survival |
| Power | Requires 15 energy/turn |
| Category | Infrastructure |

**Asset Requirements:**
- [x] Two-story modular habitat building — `PB-IN-02-Main` → **[PB-IN-02-Main.png](Style/Stil-1/Buildings/PB-IN-02-Main.png)** (1254×1254 RGBA)
- [x] Rounded roof section — `PB-IN-02-Roof-Rounded` → **[PB-IN-02-Roof-Rounded.png](Style/Stil-1/Buildings/PB-IN-02-Roof-Rounded.png)** (1254×1254 RGBA)
- [x] Large windows (illuminated at night) — `PB-IN-02-Prop-Windows` → **[PB-IN-02-Prop-Windows.png](Style/Stil-1/Buildings/PB-IN-02-Prop-Windows.png)** (1254×1254 RGBA)
- [x] External air conditioning units — `PB-IN-02-Prop-AirConditioning` → **[PB-IN-02-Prop-AirConditioning.png](Style/Stil-1/Buildings/PB-IN-02-Prop-AirConditioning.png)** (1254×1254 RGBA)
- [x] Walkway connection points (to other habitats) — `PB-IN-02-Prop-WalkwayConnectors` → **[PB-IN-02-Prop-WalkwayConnectors.png](Style/Stil-1/Buildings/PB-IN-02-Prop-WalkwayConnectors.png)** (1254×1254 RGBA)
- [x] Small rooftop garden detail — `PB-IN-02-Prop-RooftopGarden` → **[PB-IN-02-Prop-RooftopGarden.png](Style/Stil-1/Buildings/PB-IN-02-Prop-RooftopGarden.png)** (1254×1254 RGBA, magenta chroma key)
- [x] Solar panel attachment — `PB-IN-02-Prop-SolarPanelMount` → **[PB-IN-02-Prop-SolarPanelMount.png](Style/Stil-1/Buildings/PB-IN-02-Prop-SolarPanelMount.png)** (1254×1254 RGBA)

**Visual Notes:** Comfortable, modern, organized appearance. Mothership version more expensive but permanent. Bright Realistic faction: -40% survival consumption. When two Habitation Modules are placed adjacent to each other, the walkway connector props visually link them together creating a modular habitat network. The rounded roof section distinguishes it from military bunkers — signaling habitability and long-term occupation readiness.

---

#### PB-IN-03: Radar Tower
| Property | Value |
|----------|-------|
| Asset ID | PB-IN-03 |
| Grid Size | 1x1 (tall structure) |
| Cost | 150 construction material, 200 minerals |
| Power | Requires 20 energy/turn |
| Category | Infrastructure |

**Asset Requirements:**
- [ ] Tall antenna tower (1x1 footprint, tall silhouette) — `PB-IN-03-Main`
- [ ] Rotating radar dish animation — `PB-IN-03-Anim-RadarRotate`
- [ ] Satellite dishes (multiple angles) — `PB-IN-03-Prop-SatelliteDishes`
- [ ] Communication arrays — `PB-IN-03-Prop-CommunicationArrays`
- [ ] Control room building at base — `PB-IN-03-Building-ControlRoom`
- [ ] Backup power systems — `PB-IN-03-Prop-BackupPower`
- [ ] Blue scanning beam effect (when active) — `PB-IN-03-VFX-ScanBeam`

**Visual Notes:** High vertical silhouette. Essential for dungeon discovery. Each TIR level adds +50% scan range visual (stronger beam).

---

#### PB-IN-04: Wall Segment (4-directional modular set)
| Property | Value |
|----------|-------|
| Asset ID | PB-IN-04 |
| Grid Size | 1x1 (single wall segment) |
| Cost | 30 construction material, 20 minerals |
| Power | No power required |
| Category | Infrastructure |

**Asset Requirements:**
- [x] Standard wall segment (1×1) — `PB-IN-04-Main` → **[wall-segment.png](Style/Stil-1/Buildings/wall-segment.png)** (1600×983 RGBA, 0° view)
- [x] 90° direction — **[wall-segment-090.png](Style/Stil-1/Buildings/wall-segment-090.png)** (1600×983 RGBA)
- [x] 180° direction — **[wall-segment-180.png](Style/Stil-1/Buildings/wall-segment-180.png)** (1600×983 RGBA)
- [x] 270° direction — **[wall-segment-270.png](Style/Stil-1/Buildings/wall-segment-270.png)** (1600×983 RGBA)
- [ ] Reinforced wall variant (thicker) — `PB-IN-04-Variant-Reinforced`
- [ ] Hull-reinforced wall variant (metal plating) — `PB-IN-04-Variant-HullReinforced`
- [ ] Corner wall piece — `PB-IN-04-Corner`
- [ ] Natural rock formation variant (Rocky biome bonus) — `PB-IN-04-Variant-NaturalRock`
- [ ] Damage/destruction states — `PB-IN-04-Damage-State1, PB-IN-04-Damage-State2, PB-IN-04-Damage-Destroyed`

**Visual Notes:** Blocks enemy movement. Can be reinforced with hull materials for extra HP. Natural rock formations in Rocky biome provide +15% wall HP. The 4-directional set features mirrored connector posts on both ends for seamless perimeter construction — each segment has a sloped blast skirt and narrow service rail. Place segments adjacently to form continuous defensive walls around your base.

---

#### PB-IN-05: Gate (animation-ready kit)
| Property | Value |
|----------|-------|
| Asset ID | PB-IN-05 |
| Grid Size | 1x1 (gate opening) |
| Cost | 80 construction material, 50 minerals |
| Power | Requires 5 energy/turn |
| Category | Infrastructure |

**Asset Requirements:**
- [x] Gate structure with opening/closing animation — `PB-IN-05-Main` → **[PB-IN-05-Main.png](Style/Stil-1/Buildings/PB-IN-05-Main.png)** (open structural frame, animation-ready)
- [ ] Powered gate indicator light — `PB-IN-05-VFX-GateIndicatorLight` (VFX layer, runtime effect)
- [ ] Biometric scanner detail — `PB-IN-05-Prop-BiometricScanner`
- [x] Gate barrier mechanism — `PB-IN-05-Mech-Barriers` → **[PB-IN-05-Mech-Barriers.png](Style/Stil-1/Buildings/PB-IN-05-Mech-Barriers.png)** (closed moving leaves defining opening/closing motion)
- [ ] Destruction state — `PB-IN-05-Damage-Destroyed`

**Visual Notes:** Friendly-only entry when powered. Unpowered gates block everyone. Can be destroyed by enemy attacks. The main gate frame matches wall segment language with a clean unobstructed vehicle opening. Paired with wall segments to form complete base perimeter entry point.

---

### Default Storage

#### PB-ST-01: Locker (1x1)
| Property | Value |
|----------|-------|
| Asset ID | PB-ST-01 |
| Grid Size | 1x1 |
| Cost | 50 construction material |
| Capacity | 200 resources per type |
| Category | Default Storage |

**Asset Requirements:**
- [ ] Personal storage locker (1x1) — `PB-ST-01-Main`
- [ ] Opening door animation — `PB-ST-01-Anim-DoorOpen`
- [ ] Status indicator light — `PB-ST-01-VFX-StatusLight`
- [ ] Label/tag slot — `PB-ST-01-Prop-LabelSlot`

**Visual Notes:** Basic personal storage. First storage available at game start.

---

#### PB-ST-02: Container (2x2 or 4x1 wheel-shaped)
| Property | Value |
|----------|-------|
| Asset ID | PB-ST-02 |
| Grid Size | 2x2 or 4x1 (rotatable with wheel interface) |
| Cost | 300 construction material, 100 minerals |
| Capacity | 2,000 resources per type |
| Category | Default Storage |

**Asset Requirements:**
- [ ] Container - 2x2 configuration — `PB-ST-02-Config-2x2`
- [ ] Container - 4x1 (linear) configuration — `PB-ST-02-Config-4x1`
- [ ] Wheel-shaped rotation mechanism animation — `PB-ST-02-Anim-Rotate`
- [ ] Shape-changing transition animation — `PB-ST-02-Anim-ShapeChange`
- [ ] Loading door/hatch — `PB-ST-02-Mech-LoadingDoor`
- [ ] Reinforced corner posts — `PB-ST-02-Prop-CornerPosts`

**Visual Notes:** Shape-changing via rotation. Must maintain same grid slot count. Visual transformation between shapes.

---

#### PB-ST-03: Haul Storage (4x4)
| Property | Value |
|----------|-------|
| Asset ID | PB-ST-03 |
| Grid Size | 4x4 |
| Cost | 1,500 construction material, 500 minerals |
| Capacity | 10,000 resources per type |
| Category | Default Storage |

**Asset Requirements:**
- [ ] Large-scale storage facility (4x4 footprint) — `PB-ST-03-Main`
- [ ] Multiple container slots visible — `PB-ST-03-Prop-ContainerSlots`
- [ ] Loading dock area — `PB-ST-03-Prop-LoadingDock`
- [ ] Overhead crane or forklift path — `PB-ST-03-Prop-CraneForkliftPath`
- [ ] Security fencing — `PB-ST-03-Prop-SecurityFencing`
- [ ] Lighting system — `PB-ST-03-VFX-LightingSystem`

**Visual Notes:** Large-scale permanent storage. Required for mid-game resource management.

---

## Advanced Buildings (TIR 2-3)

### Production Facilities

#### PB-PF-01: Factory
| Property | Value |
|----------|-------|
| Asset ID | PB-PF-01 |
| Grid Size | 4x4 |
| Cost | 800 construction material, 500 minerals, 200 energy (startup) |
| Power | Requires 30 energy/turn |
| Category | Production Facilities |

**Asset Requirements:**
- [ ] Wide production hall building (4x4 footprint) — `PB-PF-01-Main`
- [ ] Multiple loading docks — `PB-PF-01-Prop-LoadingDocks`
- [ ] Robotic assembly arms (visible through windows/openings) — `PB-PF-01-Mech-AssemblyArms`
- [ ] Container storage area — `PB-PF-01-Prop-ContainerStorage`
- [ ] Roof ventilation system — `PB-PF-01-Prop-VentilationSystem`
- [ ] Forklift movement animation — `PB-PF-01-Anim-Forklift`
- [ ] Cargo cranes — `PB-PF-01-Prop-CargoCranes`
- [ ] Chimney with smoke emission — `PB-PF-01-VFX-ChimneySmoke`

**Visual Notes:** Requires raw material input from mines/refineries. Neon Punk faction: production speed +25%. Long rectangular hall with multiple chimneys.

---

#### PB-PF-02: Forge
| Property | Value |
|----------|-------|
| Asset ID | PB-PF-02 |
| Grid Size | 3x3 |
| Cost | 600 construction material, 400 minerals, 100 energy (startup) |
| Power | Requires 25 energy/turn |
| Category | Production Facilities |

**Asset Requirements:**
- [ ] Basic Forge (TIR 2) - gray/basic color scheme — `PB-PF-02-TIR2`
- [ ] Energy Lab (TIR 3) - blue glow variant — `PB-PF-02-TIR3`
- [ ] Dark Matter Lab (TIR 4) - purple glow variant — `PB-PF-02-TIR4`
- [ ] Void Lab (TIR 5) - black/void effect variant — `PB-PF-02-TIR5`
- [ ] Anvil and hammer animation — `PB-PF-02-Anim-Hammer`
- [ ] Forge fire/spark effects — `PB-PF-02-VFX-FireSparks`
- [ ] Research equipment interior visible through windows — `PB-PF-02-Prop-ResearchEquipment`
- [ ] Tier progression visual markers — `PB-PF-02-VFX-TierIndicators`

**Visual Notes:** Planet-based research. Color changes per tier — gray/basic, blue/energy, purple/dark matter, black/void. Placing forge on planet unlocks planet tech tree.

---

#### PB-PF-03: Refinery
| Property | Value |
|----------|-------|
| Asset ID | PB-PF-03 |
| Grid Size | 3x3 |
| Cost | 500 construction material, 300 minerals |
| Power | Requires 20 energy/turn |
| Category | Production Facilities |

**Asset Requirements:**
- [ ] Large industrial refinery complex (3x3 footprint) — `PB-PF-03-Main`
- [ ] Storage tanks (multiple sizes) — `PB-PF-03-Prop-Tanks`
- [ ] Distillation towers (vertical) — `PB-PF-03-Prop-DistillationTowers`
- [ ] Pipeline network (extensive) — `PB-PF-03-Prop-PipelineNetwork`
- [ ] Steam vent effects — `PB-PF-03-VFX-SteamVents`
- [ ] Cooling systems — `PB-PF-03-Prop-CoolingSystems`
- [ ] Maintenance stairs and walkways — `PB-PF-03-Prop-MaintenanceWalkways`
- [ ] Pressure gauge details — `PB-PF-03-Prop-PressureGauges`

**Visual Notes:** Dominated by vertical tanks. Processes raw materials into usable forms. Neon Punk faction: refining cost -30%.

---

#### PB-PF-04: Workbench
| Property | Value |
|----------|-------|
| Asset ID | PB-PF-04 |
| Grid Size | 2x1 |
| Cost | 150 construction material, 100 minerals |
| Power | Requires 5 energy/turn |
| Category | Production Facilities |

**Asset Requirements:**
- [ ] Workbench station (2x1 footprint) — `PB-PF-04-Main`
- [ ] Tool storage wall — `PB-PF-04-Prop-ToolStorageWall`
- [ ] Crafting area with materials — `PB-PF-04-Prop-CraftingArea`
- [ ] Small vise or press — `PB-PF-04-Prop-VisePress`
- [ ] Lighting lamp — `PB-PF-04-Prop-LightingLamp`
- [ ] Output conveyor/drop zone — `PB-PF-04-Prop-OutputConveyor`

**Visual Notes:** Basic crafting station. Required for all hand-crafted items. Available recipes scale with player TIR.

---

### Defense Structures

#### PB-DS-01: Turret Platform
| Property | Value |
|----------|-------|
| Asset ID | PB-DS-01 |
| Grid Size | 1x1 (with elevated mount) |
| Cost | 200 construction material, 200 minerals |
| Power | Requires 10 energy/turn + ammo |
| Category | Defense Structures |

**Asset Requirements:**
- [ ] Concrete bunker base (1x1) — `PB-DS-01-Main`
- [ ] Elevated turret mount — `PB-DS-01-Mech-TurretMount`
- [ ] Rotating turret animation — `PB-DS-01-Anim-TurretRotate`
- [ ] Ballistic cannon variant (2 or 4 cannons) — `PB-DS-01-Variant-BallisticCannon`
for the turret add
- [ ] Ballistic cannon variant (2 cannons) — `PB-DS-02-Variant-BallisticCannonDual` good against light vehicle
- [ ] Ballistic cannon `PB-DS-03-Variant-BallisticCannonSingle` slow reload, but heavy damage good against heavy vehicle
- [ ] Ballistic artilery `PB-DS-03-Variant-BallisticCannonArty` slow reload, but heavy splash damage good against large ground groups vehicle
- [ ] Energy emitter variant — `PB-DS-01-Variant-EnergyEmitter`
- [ ] Rocket bay mount variant — `PB-DS-01-Variant-RocketBay`
- [ ] Searchlight attachment — `PB-DS-01-Prop-Searchlight`
- [ ] Ammo storage nearby — `PB-DS-01-Prop-AmmoStorage`

**Visual Notes:** Dark Realistic faction: range +20%, HP +40%. Requires line of sight. Tall narrow silhouette.

---

#### PB-DS-02: Wall Gate (Advanced)
| Property | Value |
|----------|-------|
| Asset ID | PB-DS-02 |
| Grid Size | 1x1 (gate) + optional 1x1 turret above |
| Cost | 200 construction material, 150 minerals |
| Power | Requires 10 energy/turn |
| Category | Defense Structures |

**Asset Requirements:**
- [ ] Reinforced gate structure with opening animation — `PB-DS-02-Main`
- [ ] Integrated turret mount (above gate) — `PB-DS-02-Mech-TurretMount`
- [ ] Biometric scanner detail — `PB-DS-02-Prop-BiometricScanner`
- [ ] Friendly unit indicator light — `PB-DS-02-VFX-FriendlyIndicatorLight`
- [ ] Manual override controls — `PB-DS-02-Prop-ManualOverride`
- [ ] Turret rotation capability — `PB-DS-02-Anim-TurretRotate`

**Visual Notes:** Friendly units pass automatically. Can be manually opened/closed. Reinforced compared to basic Gate.

---

#### PB-DS-03: Reinforced Wall
| Property | Value |
|----------|-------|
| Asset ID | PB-DS-03 |
| Grid Size | 1x1 |
| Cost | 80 construction material, 60 minerals (standard) / 120 construction, 100 minerals (hull-reinforced) |
| HP | 300 base (600 if hull-reinforced) |
| Category | Defense Structures |

**Asset Requirements:**
- [ ] Standard reinforced wall segment — `PB-DS-03-Main`
- [ ] Hull-reinforced variant (metal plating visible) — `PB-DS-03-Variant-HullReinforced`
- [ ] Corner piece — `PB-DS-03-Corner`
- [ ] Damage states (intact, damaged, destroyed) — `PB-DS-03-Damage-State1, PB-DS-03-Damage-State2, PB-DS-03-Damage-Destroyed`
- [ ] TIR-dependent material variants — `PB-DS-03-Variant-TIR1, PB-DS-03-Variant-TIR2, PB-DS-03-Variant-TIR3`

**Visual Notes:** TIR-dependent materials. Can be upgraded with hull materials for extra HP.

---

### Support Buildings

#### PB-SB-01: Med Bay
| Property | Value |
|----------|-------|
| Asset ID | PB-SB-01 |
| Grid Size | 2x2 |
| Cost | 400 construction material, 200 minerals, 100 survival |
| Power | Requires 15 energy/turn |
| Category | Support Buildings |

**Asset Requirements:**
- [ ] Clean white medical building (2x2 footprint) — `PB-SB-01-Main`
- [ ] Emergency landing platform (helipad) — `PB-SB-01-Prop-Helipad`
- [ ] Medical cross lighting (red/green) — `PB-SB-01-VFX-MedicalCrossLighting`
- [ ] Large glass entrance lobby — `PB-SB-01-Prop-GlassLobby`
- [ ] Backup generators visible — `PB-SB-01-Prop-BackupGenerators`
- [ ] Treatment bay windows (interior visible) — `PB-SB-01-Prop-TreatmentBayWindows`
- [ ] Ambulance bay door — `PB-SB-01-Mech-AmbulanceDoor`

**Visual Notes:** Bright, safe, modern appearance. Heals units over time. Researches survivability blueprints. Hospital dungeon blueprints unlock upgrades. Bright Realistic faction: HP regen +50%.

---

#### PB-SB-02: Command Center
| Property | Value |
|----------|-------|
| Asset ID | PB-SB-02 |
| Grid Size | 3x3 |
| Cost | 600 construction material, 400 minerals, 200 energy (startup) |
| Power | Requires 20 energy/turn |
| Category | Support Buildings |

**Asset Requirements:**
- [ ] Large rectangular headquarters building (3x3 footprint) — `PB-SB-02-Main`
- [ ] Multiple roof levels — `PB-SB-02-Roof-MultiLevel`
- [ ] Tall communications tower — `PB-SB-02-Tower-Communications`
- [ ] Glass observation bridge — `PB-SB-02-Prop-ObservationBridge`
- [ ] Blue holographic antenna display — `PB-SB-02-VFX-HolographicAntenna`
- [ ] Solar panel array — `PB-SB-02-Prop-SolarPanels`
- [ ] Cargo entrance — `PB-SB-02-Mech-CargoEntrance`
- [ ] Landing pad on one side — `PB-SB-02-Prop-LandingPad`
- [ ] Tactical map HUD projection effect — `PB-SB-02-VFX-TacticalMapHUD`

**Visual Notes:** The tallest civilian building. Should immediately communicate "base headquarters." Unlocks unit control groups, base overview HUD, tactical map. Architect champion: placement range +15%.

---

#### PB-SB-03: Airfield
| Property | Value |
|----------|-------|
| Asset ID | PB-SB-03 |
| Grid Size | 6x6 (runway + facilities) |
| Cost | 2000 construction material, 1000 minerals, 500 energy (startup) |
| Power | Requires 40 energy/turn |
| Category | Support Buildings |

**Asset Requirements:**
- [ ] Runway section (long strip) — `PB-SB-03-Runway`
- [ ] Control tower — `PB-SB-03-Tower-ControlTower`
- [ ] Hangar building — `PB-SB-03-Building-Hangar`
- [ ] Fuel storage tanks — `PB-SB-03-Prop-FuelTanks`
- [ ] Parking/apron area — `PB-SB-03-Area-ParkingApron`
- [ ] Navigation lights along runway — `PB-SB-03-VFX-NavigationLights`
- [ ] Taxiway markings — `PB-SB-03-Terrace-TaxiwayMarkings`
- [ ] Maintenance facilities — `PB-SB-03-Building-MaintenanceFacilities`

**Visual Notes:** Required for aerial combat units. Cartoon SciFi faction: vehicle speed +25%. Flat terrain preferred (jungle/light snow acceptable). Large 6x6 footprint.

---

## Elite Buildings (TIR 4-5)

### High-Tier Production

#### PB-HP-01: Void Lab
| Property | Value |
|----------|-------|
| Asset ID | PB-HP-01 |
| Grid Size | 4x4 |
| Cost | 2000 construction material, 1500 minerals (including dark matter), 500 energy (startup) |
| Power | Requires 50 energy/turn |
| Category | High-Tier Production |

**Asset Requirements:**
- [ ] Elite research facility (4x4 footprint) — `PB-HP-01-Main`
- [ ] Black/void visual effect (highest tier) — `PB-HP-01-VFX-VoidEffect`
- [ ] Dark matter crystal formations visible — `PB-HP-01-Prop-CrystalFormations`
- [ ] Advanced research equipment interior — `PB-HP-01-Prop-ResearchEquipmentInterior`
- [ ] Energy containment fields — `PB-HP-01-VFX-EnergyContainmentFields`
- [ ] Alien technology integration points — `PB-HP-01-Prop-AlienTechIntegrationPoints`
- [ ] Void energy emission effects — `PB-HP-01-VFX-VoidEnergyEmission`
- [ ] Satellite uplink dish — `PB-HP-01-Prop-SatelliteUplinkDish`

**Visual Notes:** Highest tier research facility. Required for endgame content and TIR 5 items plus alien tech integration. Black/void visual with purple energy effects.

---

#### PB-HP-02: Assembly Plant
| Property | Value |
|----------|-------|
| Asset ID | PB-HP-02 |
| Grid Size | 6x6 |
| Cost | 3000 construction material, 2000 minerals, 1000 energy (startup) |
| Power | Requires 60 energy/turn |
| Category | High-Tier Production |

**Asset Requirements:**
- [ ] Massive assembly complex (6x6 footprint) — `PB-HP-02-Main`
- [ ] Multiple production bays visible — `PB-HP-02-Prop-ProductionBays`
- [ ] Overhead crane system — `PB-HP-02-Mech-OverheadCraneSystem`
- [ ] Vehicle ramp entrances — `PB-HP-02-Mech-VehicleRamps`
- [ ] Assembly line animation — `PB-HP-02-Anim-AssemblyLine`
- [ ] Large loading docks — `PB-HP-02-Prop-LoadingDocks`
- [ ] Maintenance equipment areas — `PB-HP-02-Area-MaintenanceEquipment`
- [ ] Production output indicators — `PB-HP-02-VFX-ProductionOutputIndicators`

**Visual Notes:** Mass-produces vehicles and large modules. Requires Factory to have been researched first. Produces at 3x normal factory rate. Massive front gate silhouette.

---

#### PB-HP-03: Crystal Synthesizer
| Property | Value |
|----------|-------|
| Asset ID | PB-HP-03 |
| Grid Size | 3x3 |
| Cost | 1500 construction material, 800 crystal minerals, 400 energy (startup) |
| Power | Requires 30 energy/turn |
| Category | High-Tier Production |

**Asset Requirements:**
- [ ] Crystal formation chamber (3x3 footprint) — `PB-HP-03-Main`
- [ ] Energy-to-crystal conversion animation — `PB-HP-03-Anim-CrystalConversion`
- [ ] Growing crystal formations (time-lapse effect) — `PB-HP-03-VFX-GrowingCrystals`
- [ ] Energy containment field visualization — `PB-HP-03-VFX-EnergyContainmentField`
- [ ] Cooling system details — `PB-HP-03-Prop-CoolingSystemDetails`
- [ ] Dark matter crystal output slot — `PB-HP-03-Prop-CrystalOutputSlot`
- [ ] Ice biome variant (+50% output visual) — `PB-HP-03-Variant-IceBiome`

**Visual Notes:** Converts raw energy into crystal formations for advanced research. Generates dark matter crystals over time. Essential for Void Lab fuel. Ice biome crystals provide +50% output.

---

### Super Defense

#### PB-SD-01: Energy Shield Generator
| Property | Value |
|----------|-------|
| Asset ID | PB-SD-01 |
| Grid Size | 2x2 (generator) + invisible field area |
| Cost | 3000 construction material, 2000 minerals, 1000 energy (startup) |
| Power | Requires 100 energy/turn (while active) |
| Category | Super Defense |

**Asset Requirements:**
- [ ] Generator unit (2x2 footprint) — `PB-SD-01-Main`
- [ ] Shield dome activation animation — `PB-SD-01-Anim-ShieldActivation`
- [ ] Energy field visualization (toggleable) — `PB-SD-01-VFX-EnergyFieldVisualization`
- [ ] Power conduit connections — `PB-SD-01-Prop-PowerConduitConnections`
- [ ] Status indicator lights — `PB-SD-01-VFX-StatusIndicatorLights`
- [ ] Overheat/damage effects — `PB-SD-01-VFX-OverheatDamageEffects`

**Visual Notes:** Creates area-wide protective dome (50m radius). All units inside receive +50% defense. Massive energy drain. Can be toggled on/off strategically.

---

#### PB-SD-02: Orbital Strike Beacon
| Property | Value |
|----------|-------|
| Asset ID | PB-SD-02 |
| Grid Size | 2x2 |
| Cost | 2000 construction material, 1500 minerals, 800 energy (startup) |
| Power | Requires 30 energy/turn + strike cost (50 energy per strike) |
| Category | Super Defense |

**Asset Requirements:**
- [ ] Beacon tower structure (2x2 footprint) — `PB-SD-02-Main`
- [ ] Targeting laser animation — `PB-SD-02-Anim-TargetingLaser`
- [ ] Orbital strike call-in effect (beam from space) — `PB-SD-02-VFX-OrbitalStrikeBeam`
- [ ] Satellite link indicator — `PB-SD-02-VFX-SatelliteLinkIndicator`
- [ ] Cooldown timer visual — `PB-SD-02-VFX-CoolDownTimer`
- [ ] Strike impact zone marker — `PB-SD-02-VFX-ImpactZoneMarker`
- [ ] Warning light sequence — `PB-SD-02-Anim-WarningLights`

**Visual Notes:** Calls down targeted bombardment on target area. Requires compatible mothership weaponry. Planet-wide range if satellite link active. 60 seconds cooldown between strikes. Demolitions champion: damage +30%.

---

#### PB-SD-03: Quantum Gate
| Property | Value |
|----------|-------|
| Asset ID | PB-SD-03 |
| Grid Size | 3x3 (each end) |
| Cost | 4000 construction material, 3000 minerals, 2000 energy (startup per gate) |
| Power | Requires 50 energy/turn per gate |
| Category | Super Defense |

**Asset Requirements:**
- [ ] Quantum gate structure (3x3 footprint) - Unit A — `PB-SD-03-GateA`
- [ ] Quantum gate structure (3x3 footprint) - Unit B (linked pair) — `PB-SD-03-GateB`
- [ ] Portal activation animation — `PB-SD-03-Anim-PortalActivation`
- [ ] Quantum energy field visualization — `PB-SD-03-VFX-QuantumEnergyField`
- [ ] Unit transport effect (disappear/appear) — `PB-SD-03-VFX-UnitTransportEffect`
- [ ] Link status indicator — `PB-SD-03-VFX-LinkStatusIndicator`
- [ ] Power conduit connections — `PB-SD-03-Prop-PowerConduitConnections`

**Visual Notes:** Instant unit transport between two linked gates. Requires both gates to be active and linked. Strategic repositioning tool. Base-to-base or base-to-ship transport.

---

### Special Buildings

#### PB-SP-01: Dungeon Scanner
| Property | Value |
|----------|-------|
| Asset ID | PB-SP-01 |
| Grid Size | 1x1 (tall antenna) |
| Cost | 1000 construction material, 800 minerals, 500 energy (startup) |
| Power | Requires 25 energy/turn (continuous) |
| Category | Special Buildings |

**Asset Requirements:**
- [ ] Tall antenna structure (1x1 footprint) — `PB-SP-01-Main`
- [ ] Scanning pulse animation (periodic radial wave) — `PB-SP-01-Anim-ScanningPulse`
- [ ] Radar dish rotation — `PB-SP-01-Mech-RadarDishRotate`
- [ ] Signal visualization (reveals dungeons within 5km) — `PB-SP-01-VFX-SignalVisualization`
- [ ] Control room at base — `PB-SP-01-Building-ControlRoom`
- [ ] Pulse cooldown indicator — `PB-SP-01-VFX-PulseCooldownIndicator`

**Visual Notes:** Periodically pulses to reveal nearby dungeons on map. Pulse every 10 minutes (in-game time). Essential for late-game exploration efficiency.

---

#### PB-SP-02: Resource Converter
| Property | Value |
|----------|-------|
| Asset ID | PB-SP-02 |
| Grid Size | 3x3 |
| Cost | 1500 construction material, 1000 minerals, 600 energy (startup) |
| Power | Requires 20 energy/turn |
| Category | Special Buildings |

**Asset Requirements:**
- [ ] Conversion chamber complex (3x3 footprint) — `PB-SP-02-Main`
- [ ] Input resource hoppers — `PB-SP-02-Prop-InputHoppers`
- [ ] Output material slots — `PB-SP-02-Prop-OutputMaterialSlots`
- [ ] Conversion process animation — `PB-SP-02-Anim-ConversionProcess`
- [ ] Energy flow visualization — `PB-SP-02-VFX-EnergyFlowVisualization`
- [ ] Efficiency display screen — `PB-SP-02-Prop-EfficiencyDisplayScreen`
- [ ] Material transfer pipes — `PB-SP-02-Prop-MaterialTransferPipes`

**Visual Notes:** Converts any resource to any other at 70% yield. Emergency fallback when specific resource is unavailable. Can convert excess into needed resources.

---

#### PB-SP-03: Alien Artifact Decoder
| Property | Value |
|----------|-------|
| Asset ID | PB-SP-03 |
| Grid Size | 4x4 |
| Cost | 3000 construction material, 2000 dark matter crystals, 1500 energy (startup) |
| Power | Requires 60 energy/turn |
| Category | Special Buildings |

**Asset Requirements:**
- [ ] Alien technology decoding facility (4x4 footprint) — `PB-SP-03-Main`
- [ ] Alien artifact display chamber — `PB-SP-03-Chamber-ArtifactDisplay`
- [ ] Decoding beam animation — `PB-SP-03-Anim-DecodingBeam`
- [ ] Holographic translation interface — `PB-SP-03-VFX-HolographicTranslationInterface`
- [ ] Ancient alien structure fragments visible — `PB-SP-03-Prop-AlienStructureFragments`
- [ ] Dark matter power source glow — `PB-SP-03-VFX-DarkMatterPowerGlow`
- [ ] Blueprint projection effect — `PB-SP-03-VFX-BlueprintProjectionEffect`

**Visual Notes:** Found in center galaxy ruins. Unlocks alien tech paths. Decodes ancient alien structures for unique blueprints. Endgame building. Required to access alien galaxy content.

---

## Faction Signature Buildings

#### PB-FS-01: Neon Punk - Quantum Mine
| Property | Value |
|----------|-------|
| Asset ID | PB-FS-01 |
| Base Building | Mine (PB-EX-01) variant |
| Category | Faction Signature |

**Asset Requirements:**
- [ ] Quantum-enhanced mine structure — `PB-FS-01-Main`
- [ ] Excess resource feed pipeline to refinery — `PB-FS-01-Prop-ResourceFeedPipeline`
- [ ] +50% output visual indicator — `PB-FS-01-VFX-OutputIndicator`
- [ ] Neon Punk color scheme (neon accents) — `PB-FS-01-Material-NeonPunkColors`
- [ ] Quantum energy effects — `PB-FS-01-VFX-QuantumEnergyEffects`

---

#### PB-FS-02: Dark Realistic - Fortress Wall
| Property | Value |
|----------|-------|
| Asset ID | PB-FS-02 |
| Base Building | Reinforced Wall (PB-DS-03) variant |
| Category | Faction Signature |

**Asset Requirements:**
- [ ] Massive fortress wall section (3x standard HP) — `PB-FS-02-Main`
- [ ] Turret mount points (multiple) — `PB-FS-02-Mech-TurretMountPoints`
- [ ] Defensive perimeter integration — `PB-FS-02-Wall-PerimeterSection`
- [ ] Dark Realistic military aesthetic — `PB-FS-02-Material-DarkRealisticColors`
- [ ] Heavy armor plating visible — `PB-FS-02-Prop-ArmorPlating`

---

#### PB-FS-03: Cartoon SciFi - Ion Thruster Pad
| Property | Value |
|----------|-------|
| Asset ID | PB-FS-03 |
| Base Building | Airfield (PB-SB-03) variant |
| Category | Faction Signature |

**Asset Requirements:**
- [ ] Ion thruster launch pad — `PB-FS-03-Main`
- [ ] Rapid deployment animation — `PB-FS-03-Anim-RapidDeployment`
- [ ] Unit quick-deploy effect — `PB-FS-03-VFX-QuickDeployEffect`
- [ ] Cartoon SciFi bright color scheme — `PB-FS-03-Material-CartoonSciFiColors`
- [ ] Vehicle speed boost visual indicator — `PB-FS-03-VFX-SpeedBoostIndicator`

---

#### PB-FS-04: Bright Realistic - Hydroponics Bay
| Property | Value |
|----------|-------|
| Asset ID | PB-FS-04 |
| Base Building | Med Bay (PB-SB-01) variant |
| Category | Faction Signature |

**Asset Requirements:**
- [ ] Hydroponic farm structure — `PB-FS-04-Main`
- [ ] Plant growth animation — `PB-FS-04-Anim-PlantGrowth`
- [ ] Food production visualization — `PB-FS-04-VFX-FoodProductionVisualization`
- [ ] Oxygen generation effect — `PB-FS-04-VFX-OxygenGenerationEffect`
- [ ] Bright Realistic clean aesthetic — `PB-FS-04-Material-BrightRealisticColors`
- [ ] Sustains long-term occupation visual — `PB-FS-04-VFX-LongTermOccupationVisual`

---

## Building Variants Summary

### Mine Variants (5 total)
| Variant | Color/Effect Indicator |
|---------|----------------------|
| Stone Mine | Gray/rock dust |
| Crystal Mine | Green glow |
| Metal Ore Mine | Silver/metallic |
| Coal Mine | Black smoke/dust |
| Sulfur Mine | Yellow smoke |

### Forge Tier Progression (4 tiers)
| Tier | Name | Color Scheme |
|------|------|-------------|
| TIR 2 | Basic Forge | Gray/basic |
| TIR 3 | Energy Lab | Blue glow |
| TIR 4 | Dark Matter Lab | Purple glow |
| TIR 5 | Void Lab | Black/void effect |

### Wall Variants (3 total)
| Variant | HP | Material |
|---------|-----|----------|
| Standard Wall | 100 | Basic construction |
| Reinforced Wall | 300 | TIR materials |
| Hull-Reinforced Wall | 600 | Hull plating |

### Power Generation Biome Variants
| Building | Special Variants |
|----------|-----------------|
| Solar Array | Desert (+30%), Ice (-30%), Jungle (-50%) |
| Wind Turbine | Ice accumulation, Rocky (+15%), Storm bonus |
| Water Turbine | Swamp contamination variant |
| Geothermal Vent | Ice sheet bonus (+50%), Volcanic preferred |

---

## Asset Count Summary

| Category | Default (TIR 1-2) | Advanced (TIR 2-3) | Elite (TIR 4-5) | Total |
|----------|------------------:|-------------------:|----------------:|------:|
| Resource Extraction | 3 | - | - | 3 |
| Power Generation | 6 | - | - | 6 |
| Infrastructure | 5 | - | - | 5 |
| Storage | 3 | - | - | 3 |
| Production Facilities | - | 4 | 2 | 6 |
| Defense Structures | - | 3 | 3 | 6 |
| Support Buildings | - | 3 | - | 3 |
| Special Buildings | - | - | 3 | 3 |
| Faction Signatures | - | - | - | 4 |
| **Base Building Count** | **17** | **10** | **8** | **35** |

### Including Variants:
- Mine variants: +4 additional models
- Forge tier progression: +3 additional models
- Wall variants: +2 additional models
- Power biome variants: ~8 additional variations
- **Total unique asset configurations: ~50+**

---

## Technical Requirements

### Grid Sizes Reference
| Size | Footprint | Buildings Using This Size |
|------|-----------|--------------------------|
| 1x1 | Small | Mine, Oil Pump, Harvester Post, Radar Tower, Wall Segment, Gate, Locker, Turret Platform, Reinforced Wall, Dungeon Scanner |
| 2x1 | Narrow | Solar Array, Wind Turbine, Workbench |
| 2x2 | Medium | Coal Reactor, Geothermal Vent, Camp/Barracks, Habitation Module, Med Bay, Energy Shield Generator, Orbital Strike Beacon, Water Turbine* |
| 3x3 | Large-Medium | Forge, Refinery, Crystal Synthesizer, Command Center, Resource Converter, Alien Artifact Decoder*, Quantum Gate (each end) |
| 4x4 | Large | Factory (4x4), Void Lab, Haul Storage, Assembly Plant (6x6*), Alien Artifact Decoder |
| 6x6 | Very Large | Airfield, Assembly Plant |

*Water Turbine is 1x1 but requires water tile placement
*Assembly Plant is listed as both 6x6 and some sources say 4x4 - verify with design

### Required Animation Types
| Animation | Buildings |
|-----------|----------|
| Rotation (continuous) | Wind Turbine, Radar Tower, Turret Platform |
| Rotation (sun-tracking) | Solar Array |
| Oscillation | Oil Pump (pump arm) |
| Emission effects | Coal Reactor, Geothermal Vent, Forge, Crystal Synthesizer |
| Gate opening/closing | Gate, Wall Gate (Advanced) |
| Portal/transport | Quantum Gate |
| Shield activation/deactivation | Energy Shield Generator |
| Scanning pulse | Dungeon Scanner |
| Conversion process | Resource Converter |
| Strike beam | Orbital Strike Beacon |

### Required VFX Effects
| Effect Type | Buildings |
|-------------|----------|
| Smoke/Emissions | Coal Reactor, Geothermal Vent, Factory |
| Glow effects | Forge (tier-dependent), Void Lab, Crystal Synthesizer |
| Energy beams | Orbital Strike Beacon, Quantum Gate |
| Shield visualization | Energy Shield Generator |
| Scanning waves | Dungeon Scanner |
| Spark/fire | Forge |
| Water splash | Water Turbine |
| Steam vents | Geothermal Vent, Refinery |

---

## See Also

- [Planet surfaces, biomes, resource deposits, weather, and dungeon entrances](./Environment_AssetList.md)
- [Playable units, champions, enemies, weapons, and combat VFX](./Unit_AssetList.md)
- [Planet Buildings design document](../Briefing/buildings/planet_buildings.md)
- [Storage system and grid placement](./storage_system.md)
- [Ship modules and cross-compatibility](./ship_modules.md)
- [Building research via tech tree](../Briefing/Tech_Tree/overview.md)
- [Biome-specific resource modifiers](../Briefing/resources/types.md)
- [Example buildings visual reference](./Example_Buildings.md)
