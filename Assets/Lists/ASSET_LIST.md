# Asset List — Complete Game Asset Inventory for AI Image Generation & 3D Modeling

Master asset inventory covering buildings, weapons, ship modules, units, resources, UI elements, and environmental assets with dimensions, titles, descriptions, and AI generation prompts.

---

## Asset Format Standards

### Rendering Pipeline
- **Planet surface assets:** 2.5D spritesheets (4 directions: N/S/E/W + 4 diagonals) OR low-poly 3D models rendered in top-down view
- **Space assets:** 2D sprites with parallax layers OR simple 3D models for ship customization
- **UI elements:** 2D vector graphics, scalable resolution
- **Model format:** GLB/GLTF for 3D models, PNG sprite sheets for 2D
- **Sprite sheet dimensions:** Power-of-two textures (512x512, 1024x1024, 2048x2048)
- **Polygon budget:** 
  - Small buildings: <5,000 triangles
  - Large buildings: <15,000 triangles
  - Units (infantry): <8,000 triangles
  - Vehicles: <20,000 triangles
  - Mechs: <15,000 triangles

### Visual Style Guide
- **Overall aesthetic:** Gritty sci-fi with faction-specific color palettes
- **Lighting:** Dynamic directional light (sun), emissive materials for energy effects
- **Color coding by TIR tier:**
  - TIR 1: Brown/gray/dull colors (rusty, basic)
  - TIR 2: Blue/silver (refined, functional)
  - TIR 3: Green/purple (advanced)
  - TIR 4: Gold/white (elite)
  - TIR 5: Black/violet glow (void-infused)
  - Alien: Iridescent/shimmering colors

---

## 1. BUILDING ASSETS — Planet Facilities [See Planet Buildings](./Buildings/Planet_Buildings.md)

### Default Buildings (TIR 1-2)

#### Mine (4 variants)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-MINE-01 | Stone Quarry Mine | 1x1 | 3D model | Low concrete structure with conveyor belt, gray stone dust clouds | "sci-fi mining outpost, concrete structure, conveyor belt, rocky terrain, top-down view, gritty industrial" | Basic mineral extraction |
| BLD-MINE-02 | Crystal Mine Shaft | 1x1 | 3D model | Metal framework tower with glowing crystal deposits visible below, purple/blue glow | "crystal mining rig, metal scaffolding, glowing crystals underground, sci-fi, isometric view" | Crystal mineral extraction |
| BLD-MINE-03 | Metal Ore Mine | 1x1 | 3D model | Heavy steel gantry with ore processing equipment, sparks flying | "metal ore mine, steel gantry crane, industrial mining rig, sparks, smoke, top-down game asset" | Metal/titanium extraction |
| BLD-MINE-04 | Coal Mine Entrance | 1x1 | 3D model | Tunnel entrance with black smoke exhaust, coal piles nearby, dark aesthetic | "coal mine shaft, black smoke vent, steel support beams, coal piles, sci-fi mining" | Coal fuel production |

#### Oil Pump (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-OIL-01 | Automated Oil Pumpjack | 1x1 | 3D model | Mechanical pumpjack arm oscillating, steel pipe network, warning stripes | "oil pumpjack, mechanical arm pumping, steel pipes, yellow-black hazard stripes, sci-fi desert" | Fuel production from oil deposits |

#### Harvester Post (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-HARV-01 | Dust Harvester Post | 1x1 | 3D model | Tall thin antenna with collection net, small solar panel, passive operation | "dust harvester, tall antenna, collection net, small solar panel, desert planet, sci-fi" | Passive ambient resource collection |

#### Solar Array (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-SOLAR-01 | Photovoltaic Solar Array | 2x1 | 3D model | Row of angled solar panels on articulated mounts, dark blue reflective surfaces | "solar panel array, angled photovoltaic panels, desert planet surface, sci-fi base, top-down" | Energy generation (clear biomes) |

#### Wind Turbine (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-WIND-01 | Horizontal-Axis Wind Turbine | 2x1 | 3D model | Tall tower with 3-blade rotor, white or gray housing, rotating blades animation | "wind turbine, tall tower, three blades, sci-fi design, rocky terrain, game asset" | Energy generation (windy biomes) |

#### Coal Reactor (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-COAL-01 | Coal-Fired Reactor Building | 2x2 | 3D model | Box-shaped reactor with tall smokestack, black smoke plume, coal hopper input | "coal reactor building, smokestack, black smoke, industrial sci-fi, box structure, top-down" | High energy generation (25/turn) |

#### Water Turbine (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-WTURB-01 | Water Wheel Turbine | 1x1 | 3D model | Rotating water wheel in flowing water, wooden/metal construction, placed on water tile | "water wheel turbine, flowing water, rotating blades, sci-fi enhancement, top-down view" | Energy generation (Water/Swamp biomes) |

#### Geothermal Vent (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-GEO-01 | Geothermal Tap Station | 2x2 | 3D model | Metal platform over steam vent, pipes descending into ground, white steam plumes | "geothermal tap, steam vent, metal platform, pipes, volcanic terrain, sci-fi" | Very high energy generation (40/turn) |

#### Camp / Barracks (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-BARR-01 | Military Barracks Container | 2x2 | 3D model | Modified shipping container with satellite dish, watchtower corner, soldier silhouettes visible inside | "military barracks, shipping container, satellite dish, watchtower, sci-fi base camp" | Unit training (4 units capacity) |

#### Habitation Module (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-HAB-01 | Pressurized Habitat Dome | 2x2 | 3D model | Inflatable dome structure with transparent top, internal lighting visible, airlock entrance | "habitat dome, inflatable transparent roof, internal lights, airlock door, sci-fi colony" | Max unit cap increase (+8) |

#### Radar Tower (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-RADAR-01 | Rotating Radar Antenna Tower | 1x1 | 3D model | Tall lattice tower with rotating dish antenna, red warning light on top, cable runs | "radar tower, rotating dish antenna, lattice tower, red beacon light, sci-fi military" | Area scanning and dungeon discovery |

#### Wall Segment (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-WALL-01 | Concrete Wall Segment | 1x1 | 3D model | Modular concrete wall with metal corner brackets, sandbag base optional | "concrete wall segment, metal brackets, modular defense, sci-fi base, top-down" | Base perimeter defense |

#### Gate (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-GATE-01 | Powered Security Gate | 1x1 | 3D model | Sliding metal gate with status light (green=open, red=closed), card reader panel | "security gate, sliding metal door, status lights, sci-fi base entrance" | Controlled base access |

#### Locker Storage (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-LOCKER-01 | Personal Storage Locker | 1x1 | 3D model | Metal locker bank, 4 compartments with labels, small handle | "metal storage locker, military surplus, labeled compartments, sci-fi" | Basic 200 resource capacity storage |

#### Container Storage (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-CONTAINER-01 | Shipping Container Storage | 2x2 or 4x1 | 3D model | Standard cargo container on low trailer, lockable doors, color-coded by resource type | "cargo shipping container, sci-fi paint job, locked doors, modular storage" | 2,000 resource capacity storage |

#### Haul Storage (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-HAUL-01 | Warehouse Storage Complex | 4x4 | 3D model | Large warehouse building with loading bay doors, roof solar panels, forklift visible inside | "warehouse complex, large loading bays, sci-fi storage facility, top-down view" | 10,000 resource capacity storage |

### Advanced Buildings (TIR 2-3)

#### Factory (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-FACT-01 | Vehicle Manufacturing Factory | 4x4 | 3D model | Large industrial building with crane arms, assembly line windows, exhaust stacks | "vehicle factory, large industrial building, crane arms, assembly line windows, sci-fi" | Vehicle and module production |

#### Forge (5 TIR variants)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-FORGE-T1 | Basic Forge Workbench | 3x3 | 3D model | Anvil station with hammer, coal fire glow, tool racks on walls, gray/brown palette | "forge workbench, anvil, coal fire, tool racks, medieval sci-fi blend, top-down" | TIR 2 basic research |
| BLD-FORGE-T2 | Energy Lab | 3x3 | 3D model | Blue-lit laboratory with plasma containment field, holographic displays, silver/blue palette | "energy lab, blue plasma containment, holographic displays, sci-fi laboratory, top-down" | TIR 3 energy research |
| BLD-FORGE-T3 | Dark Matter Lab | 4x4 | 3D model | Purple-glowing chamber with suspended dark matter orb, magnetic field rings, purple/black palette | "dark matter lab, purple glow, floating orb, magnetic fields, sci-fi research" | TIR 4 dark matter research |
| BLD-FORGE-T4 | Void Lab | 4x4 | 3D model | Black chamber with violet void swirl center, energy conduits, black/violet palette | "void lab, black room, purple void swirl, energy conduits, alien technology" | TIR 5 void research |
| BLD-FORGE-T5 | Alien Artifact Decoder | 4x4 | 3D model | Iridescent chamber with floating alien glyphs, crystalline structures, shimmering colors | "alien decoder, iridescent glow, floating glyphs, crystal structures, alien tech" | TIR 5 alien research |

#### Refinery (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-REFIN-01 | Material Refinery Plant | 3x3 | 3D model | Towering distillation columns, pipe network, processing vents with colored steam | "refinery plant, distillation towers, pipe network, colored steam vents, sci-fi industrial" | Raw material processing |

#### Workbench (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-WORKB-01 | Crafting Workbench Station | 2x1 | 3D model | Metal worktable with tool organizer, power outlet, small vise, parts tray | "crafting workbench, metal table, tool wall, sci-fi tools, top-down game asset" | Weapon/armor/small module crafting |

#### Turret Platform (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-TURRET-01 | Automated Turret Platform | 1x1 | 3D model | Elevated concrete platform with rotating turret mount, ammo box nearby, soldier position | "turret platform, elevated concrete base, rotating gun mount, sci-fi defense" | Ground weapon mounting (ballistic/plasma/rocket) |

#### Reinforced Wall (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-REINWALL-01 | Hull-Reinforced Wall | 1x1 | 3D model | Thick composite wall with metal plating, impact scorch marks, reinforced corner joints | "reinforced wall, composite armor plating, sci-fi fortification, top-down" | Enhanced base defense (600 HP) |

#### Med Bay (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-MEDBAY-01 | Field Medical Bay | 2x2 | 3D model | White tent/room with medical cross, examination table visible through window, green lighting | "medical bay, white interior, medical cross symbol, examination table, sci-fi field hospital" | Unit healing and survivability research |

#### Command Center (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-CMD-01 | Base Command Center | 3x3 | 3D model | Two-story structure with observation deck, antenna array, tactical map table visible through windows | "command center, two-story, observation deck, antenna array, sci-fi military HQ" | RTS commands and control groups |

#### Airfield (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-AIRF-01 | Forward Operating Airfield | 6x6 | 3D model | Paved runway section, hangar building, fuel storage tanks, control tower | "airfield, paved runway, hangar building, control tower, sci-fi military base" | Aerial unit deployment and retrieval |

### Elite Buildings (TIR 4-5)

#### Void Lab (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-VLAB-01 | Void Research Laboratory | 4x4 | 3D model | Massive structure with central void chamber, energy conduits from roof, black/violet glow | "void laboratory, massive structure, central void chamber, violet energy glow, top-down" | TIR 5 research facility |

#### Assembly Plant (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-ASMBL-01 | Heavy Assembly Plant | 6x6 | 3D model | Huge industrial complex with multiple crane towers, vehicle bay doors, parts storage yard | "assembly plant, huge industrial complex, multiple cranes, vehicle bays, sci-fi manufacturing" | Mass vehicle/module production |

#### Crystal Synthesizer (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-CRYST-01 | Crystal Growth Chamber | 3x3 | 3D model | Glass dome with crystalline formations growing inside, energy field suspension, purple glow | "crystal synthesizer, glass dome, growing crystals, energy field, sci-fi lab" | Dark matter crystal generation |

#### Energy Shield Generator (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-ESHLD-01 | Shield Generator Tower | 2x2 | 3D model | Tall tower with rotating energy ring base, invisible dome field effect (rendered as particle overlay) | "shield generator, tall tower, rotating energy ring, sci-fi defense, particle effects" | Area-wide defensive shield (+50% defense) |

#### Orbital Strike Beacon (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-ORBSTK-01 | Orbital Strike Beacon | 2x2 | 3D model | Tall antenna array with targeting laser, satellite dish, red targeting light pulsing | "orbital strike beacon, tall antenna array, targeting laser, satellite dish, sci-fi military" | Planet-wide bombardment |

#### Quantum Gate (2 variants - paired)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-QGATE-A | Quantum Gate Terminal A | 3x3 | 3D model | Circular portal frame with energy field, control console, blue shimmering portal surface | "quantum gate, circular portal, energy field, blue shimmer, sci-fi teleporter" | Instant unit transport (end A) |
| BLD-QGATE-B | Quantum Gate Terminal B | 3x3 | 3D model | Identical to A but with green portal hue for differentiation | "quantum gate terminal, circular portal, green energy field, matching pair, sci-fi" | Instant unit transport (end B) |

#### Dungeon Scanner (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-DSCAN-01 | Dungeon Detection Array | 1x1 | 3D model | Tall mast with rotating sensor array, pulsing blue light every 10 seconds, data cables | "dungeon scanner, tall mast, rotating sensors, pulsing blue light, sci-fi exploration" | Reveals nearby dungeons on map |

#### Resource Converter (1 variant)
| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| BLD-RCONV-01 | Material Conversion Chamber | 3x3 | 3D model | Large processing chamber with input/output hoppers, conversion indicator lights | "resource converter, large chamber, input output hoppers, sci-fi industrial" | Resource-to-resource conversion (70% yield) |

### Generated Assets — Style 1: Dark Realistic Hard Sci-Fi (AAA Production Concept Art)

All generated assets use the consistent visual language: dark gunmetal steel with subtle weathering, warm orange utility lights, realistic PBR-inspired materials, controlled edge wear, isolated on transparent background, centered composition. Camera angle: 2.5D isometric view at 40°. All files are RGBA PNGs stored in `Assets/Style/Stil-1/Buildings/`.

#### Resource Extraction — Generated Assets

| Asset ID | Title | Grid Size | File Path | Dimensions | Description |
|----------|-------|-----------|-----------|------------|-------------|
| PB-EX-02 | Oil Pump (Modern Pumpjack) | 1×1 | [oil-pump.png](../Assets/Style/Stil-1/Buildings/oil-pump.png) | 1402×1122 | Compact pumpjack on rectangular service skid — near-black plated steel, broad readable machinery, tiny amber lamps. Animated pump arm oscillates vertically during operation. Validated clean matte edges with no green spill. |
| PB-EX-01-MTL | Metal Ore Mine (Massive Drill) | 1×1 | [metal-ore-mine.png](../Assets/Style/Stil-1/Buildings/metal-ore-mine.png) | 1254×1254 | Heavy rotary drill entering ground with torque-braced support frame. Sealed bore collar with twin extraction/filter stacks. Clean VFX outlet points for mineral-specific smoke/light effects (black=coal, yellow=sulfur, green=uranium). Neutral hardware keeps VFX layers swappable. Validated clean transparency and no green spill. |
| PB-EX-03-Main | Harvester Post Hub | 1×1 | [PB-EX-03-Main.png](../Assets/Style/Stil-1/Buildings/PB-EX-03-Main.png) | 1254×1254 | Compact central hub with four capped utility hardpoints. Simple, unobtrusive structure for early game visual progression. |
| PB-EX-03-Prop-DustCollector | Dust Collector Array | Prop | [PB-EX-03-Prop-DustCollector.png](../Assets/Style/Stil-1/Buildings/PB-EX-03-Prop-DustCollector.png) | 1254×1254 | Ambient dust collection module with compact pedestal. |
| PB-EX-03-Prop-CondensationTower | Water Condensation Tower | Prop | [PB-EX-03-Prop-CondensationTower.png](../Assets/Style/Stil-1/Buildings/PB-EX-03-Prop-CondensationTower.png) | 1254×1254 | Water collection tower module. |
| PB-EX-03-Prop-Antenna | Low-Profile Antenna/Sensor | Prop | [PB-EX-03-Prop-Antenna.png](../Assets/Style/Stil-1/Buildings/PB-EX-03-Prop-Antenna.png) | 1254×1254 | Communication sensor module. |
| PB-EX-03-Prop-StorageContainer | Small Storage Container | Prop | [PB-EX-03-Prop-StorageContainer.png](../Assets/Style/Stil-1/Buildings/PB-EX-03-Prop-StorageContainer.png) | 1254×1254 | Storage module with mounting base. |

#### Power Generation — Generated Assets

| Asset ID | Title | Grid Size | File Path | Dimensions | Description |
|----------|-------|-----------|-----------|------------|-------------|
| PB-PG-01-Main | Solar Array (Neutral) | 2×1 | [solar-array.png](../Assets/Style/Stil-1/Buildings/solar-array.png) | Variable | Circular azimuth bearing, shared tilt axle, visible pistons, single blue status light. Desert variant: [solar-array-desert.png](../Assets/Style/Stil-1/Buildings/solar-array-desert.png). Snow variant: [solar-array-snow.png](../Assets/Style/Stil-1/Buildings/solar-array-snow.png). All variants preserve identical geometry for interchangeable rotation animation. |
| PB-PG-02-Main | Wind Turbine (Neutral) | 2×1 | [wind-turbine.png](../Assets/Style/Stil-1/Buildings/wind-turbine.png) | 1024×1536 | Three-blade rotor and hub as one subassembly, nacelle/yaw housing behind it. Ice variant: [wind-turbine-ice.png](../Assets/Style/Stil-1/Buildings/wind-turbine-ice.png). Animation-ready blade/nacelle split with lightning rod distinct from rotor assembly. |
| PB-PG-03-Main | Coal Reactor | 2×2 | [coal-reactor.png](../Assets/Style/Stil-1/Buildings/coal-reactor.png) | 1402×1122 | 2×2 core with coal visibly feeding reactor. Smokestack and steam vent as external VFX sockets (no baked particles). Coal conveyor shows steady fuel supply. Transformer yard in own quadrant. Blue reactor glow contained within core — animation layer for energy output. Validated clean transparency. |
| PB-PG-04-Main | Water Turbine (Neutral) | 1×1 | [B-PG-04-water-turbine.png](../Assets/Style/Stil-1/Buildings/B-PG-04-water-turbine.png) | 1254×1254 | Six-blade paddle rotor between armored pontoons. Submerged blades visible beneath water surface. Swamp variant: [B-PG-04-water-turbine-swamp.png](../Assets/Style/Stil-1/Buildings/B-PG-04-water-turbine-swamp.png). Self-contained 1×1 water tile with shore cable terminating at tile edge. |

#### Infrastructure — Generated Assets

| Asset ID | Title | Grid Size | File Path | Dimensions | Description |
|----------|-------|-----------|-----------|------------|-------------|
| PB-IN-01-Main | Camp/Barracks Main Building | 2×2 | [PB-IN-01-Main.png](../Assets/Style/Stil-1/Buildings/PB-IN-01-Main.png) | 1254×1254 | Broad 2×2 bunker with recessed entrance track and perimeter sockets. Part of 8-piece modular barracks kit. |
| PB-IN-01-Wall-Armored | Armored Wall Section | Prop | [PB-IN-01-Wall-Armored.png](../Assets/Style/Stil-1/Buildings/PB-IN-01-Wall-Armored.png) | 1254×1254 | Perimeter wall module matching barracks aesthetic. |
| PB-IN-01-Prop-VehicleParking | Vehicle Parking Area | Prop | [PB-IN-01-Prop-VehicleParking.png](../Assets/Style/Stil-1/Buildings/PB-IN-01-Prop-VehicleParking.png) | 1254×1254 | Empty vehicle bay module for transport staging. |
| PB-IN-01-Prop-TrainingEquipment | Training Yard Equipment | Prop | [PB-IN-01-Prop-TrainingEquipment.png](../Assets/Style/Stil-1/Buildings/PB-IN-01-Prop-TrainingEquipment.png) | 1254×1254 | Soldier development visual module. |
| PB-IN-01-Tower-Observation | Observation Tower | Prop | [PB-IN-01-Tower-Observation.png](../Assets/Style/Stil-1/Buildings/PB-IN-01-Tower-Observation.png) | 1254×1254 | Corner mount tower for perimeter awareness. |
| PB-IN-01-Mech-BlastDoors | Heavy Blast Doors (Closed) | Mech | [PB-IN-01-Mech-BlastDoors.png](../Assets/Style/Stil-1/Buildings/PB-IN-01-Mech-BlastDoors.png) | 1254×1254 | Closed mechanical state for entrance animation. |
| PB-IN-01-Prop-FlagPole | Flag Pole | Prop | [PB-IN-01-Prop-FlagPole.png](../Assets/Style/Stil-1/Buildings/PB-IN-01-Prop-FlagPole.png) | 1254×1254 | Base identification marker. |
| PB-IN-01-Anim-UnitEntrance | Unit Entrance (Open State) | Anim | [PB-IN-01-Anim-UnitEntrance.png](../Assets/Style/Stil-1/Buildings/PB-IN-01-Anim-UnitEntrance.png) | 1254×1254 | Fully open endpoint keyframe for unit entrance animation — retracted doors, deployed ramp, guide lights, no baked unit. |
| PB-IN-02-Main | Habitation Module Main Shell | 2×2 | [PB-IN-02-Main.png](../Assets/Style/Stil-1/Buildings/PB-IN-02-Main.png) | 1254×1254 | Two-story residential shell with attachment bays (not baked-in components). Rounded roof distinguishes from military bunkers. |
| PB-IN-02-Roof-Rounded | Rounded Roof Section | Prop | [PB-IN-02-Roof-Rounded.png](../Assets/Style/Stil-1/Buildings/PB-IN-02-Roof-Rounded.png) | 1254×1254 | Comfortable modern roof module. |
| PB-IN-02-Prop-Windows | Illuminated Windows | Prop | [PB-IN-02-Prop-Windows.png](../Assets/Style/Stil-1/Buildings/PB-IN-02-Prop-Windows.png) | 1254×1254 | Night-lit window strip module. |
| PB-IN-02-Prop-AirConditioning | Air Conditioning Units | Prop | [PB-IN-02-Prop-AirConditioning.png](../Assets/Style/Stil-1/Buildings/PB-IN-02-Prop-AirConditioning.png) | 1254×1254 | External HVAC rack module. |
| PB-IN-02-Prop-WalkwayConnectors | Walkway Connection Points | Prop | [PB-IN-02-Prop-WalkwayConnectors.png](Assets/Style/Stil-1/Buildings/PB-IN-02-Prop-WalkwayConnectors.png) | 1254×1254 | Repeatable pressure connector — when two habitats are placed adjacent, these props visually link them together creating a modular habitat network. |
| PB-IN-02-Prop-RooftopGarden | Rooftop Garden Detail | Prop | [PB-IN-02-Prop-RooftopGarden.png](../Assets/Style/Stil-1/Buildings/PB-IN-02-Prop-RooftopGarden.png) | 1254×1254 | Contained garden module (magenta chroma key for foliage preservation). Signals habitability and long-term occupation readiness. |
| PB-IN-02-Prop-SolarPanelMount | Solar Panel Mount | Prop | [PB-IN-02-Prop-SolarPanelMount.png](../Assets/Style/Stil-1/Buildings/PB-IN-02-Prop-SolarPanelMount.png) | 1254×1254 | Compact two-panel solar mount. |
| PB-IN-04-Wall | Wall Segment (Neutral — 0° View) | 1×1 | [wall-segment.png](../Assets/Style/Stil-1/Buildings/wall-segment.png) | 1600×983 | Modular connector wall with mirrored posts on both ends. Part of 4-directional set for seamless perimeter construction. Sloped blast skirt and narrow service rail. |
| PB-IN-04-Wall-090 | Wall Segment (90° View) | 1×1 | [wall-segment-090.png](../Assets/Style/Stil-1/Buildings/wall-segment-090.png) | 1600×983 | Perpendicular direction — same geometry, opposite face exposed. |
| PB-IN-04-Wall-180 | Wall Segment (180° View) | 1×1 | [wall-segment-180.png](../Assets/Style/Stil-1/Buildings/wall-segment-180.png) | 1600×983 | Reverse-facing side — original diagonal orientation. |
| PB-IN-04-Wall-270 | Wall Segment (270° View) | 1×1 | [wall-segment-270.png](../Assets/Style/Stil-1/Buildings/wall-segment-270.png) | 1600×983 | Fourth perpendicular direction — completes the four-way set. |
| PB-IN-05-Main | Gate Structural Frame (Open) | 1×1 | [PB-IN-05-Main.png](../Assets/Style/Stil-1/Buildings/PB-IN-05-Main.png) | Variable | Open gate frame matching wall segment language — clean unobstructed vehicle opening. Recessed entrance track for moving leaves. |
| PB-IN-05-Mech-Barriers | Gate Barrier Mechanism (Closed) | Mech | [PB-IN-05-Mech-Barriers.png](../Assets/Style/Stil-1/Buildings/PB-IN-05-Mech-Barriers.png) | Variable | Closed moving leaves defining the opening/closing animation state. Paired with wall segments for complete base perimeter entry point. |

#### Asset Validation Summary

| Category | Assets Generated | Format | Validation Status |
|----------|-----------------|--------|-------------------|
| Resource Extraction | 7 assets | RGBA PNG (1254×1254, 1402×1122) | ✅ All passed alpha/fringe checks |
| Power Generation | 8 assets (3 biome sets + variants) | RGBA PNG (variable, 1024×1536 for turbines) | ✅ All validated clean transparency |
| Infrastructure — Barracks | 8-piece modular kit | RGBA PNG (1254×1254) | ✅ All passed validation |
| Infrastructure — Habitation | 7-piece modular kit | RGBA PNG (1254×1254, magenta key for garden) | ✅ All passed validation |
| Defense — Wall Segments | 4-directional set | RGBA PNG (1600×983) | ✅ Clean matte edges validated |
| Infrastructure — Gate | 2 assets (frame + barriers) | RGBA PNG | ✅ Animation-ready states |

---

## 2. SHIP MODULES [See Ship Modules](./Buildings/Ship_Modules.md)

### Drive Modules

| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| SHP-DRV-R1 | Rocket Drive (Starting) | 1x1 | 3D model | Box thruster with red/orange exhaust plume, fuel lines visible, riveted metal plates | "rocket drive, box thruster, orange exhaust, sci-fi spaceship engine, top-down" | Starting ship drive |
| SHP-DRV-A2 | Atomic Reactor Drive | 2x1 | 3D model | Cylindrical reactor with white/blue atomic exhaust, cooling fins, radiation symbol | "atomic reactor drive, cylindrical, blue-white exhaust, cooling fins, sci-fi engine" | TIR 2 ship propulsion |
| SHP-DRV-I3 | Ionic Reactor Drive | 2x2 | 3D model | Sleek ion emitter with blue/purple stream, smooth curves, energy core visible through glass | "ionic drive, sleek design, purple-blue ion stream, sci-fi spaceship engine" | TIR 3 ship propulsion |
| SHP-DRV-E4 | Energy Stream Drive | 3x2 | 3D model | Energy conduit array with white/gold beam, hexagonal housing, power taps | "energy stream drive, gold-white beam, hexagonal housing, advanced sci-fi engine" | TIR 4 ship propulsion |
| SHP-DRV-V5 | Void Warp Drive | 4x4 | 3D model | Swirling void portal in ship mount, purple-black with starlight edges, energy rings | "void warp drive, swirling portal, purple black, starlight edges, sci-fi engine" | TIR 5 instant travel |
| SHP-DRV-A6 | Alien Warp Drive | 4x4 | 3D model | Iridescent alien engine, shimmering colors shifting, organic-metal hybrid design | "alien warp drive, iridescent, shifting colors, organic metal hybrid, sci-fi" | Endgame unlimited travel |

### Storage Modules (Ship)

| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| SHP-STK-01 | Ship Locker | 1x1 | 3D model | Wall-mounted metal locker, 4 compartments, label slots | "ship locker, wall mounted metal, sci-fi storage compartment" | 200 resource capacity (ship) |
| SHP-STC-01 | Ship Container | 2x2 or 4x1 | 3D model | Cargo container with wheel rotation indicator, reinforced corners | "ship cargo container, reinforced corners, sci-fi storage bay" | 2,000 resource capacity (ship) |
| SHP-STH-01 | Haul Storage Bay | 4x4 | 3D model | Large cargo hold section, multiple container slots, overhead crane rail | "ship haul storage, large cargo hold, container slots, sci-fi" | 10,000 resource capacity (ship) |
| SHP-STV-01 | Reinforced Vault Bay | 4x4 | 3D model | Armored vault section with blast door, internal supports, warning labels | "ship reinforced vault, armored bay, blast door, sci-fi secure storage" | 25,000 capacity (ship) |
| SHP-STB-01 | Black Hole Storage | 3x3 | 3D model | Spherical containment field with swirling mini event horizon visual, purple glow | "black hole storage, spherical field, swirling event horizon, sci-fi" | Effectively unlimited (ship) |

### Protection Modules (Ship)

| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| SHP-HULL1 | Standard Steel Hull | Perimeter | 3D model | Gray steel plating with rivet lines, basic thermal coating | "steel hull plating, gray metal, sci-fi spaceship exterior" | Starting ship protection (500 HP) |
| SHP-HULL2 | Heat-Resistant Alloy Hull | Perimeter | 3D model | Dark heat-resistant tiles with ceramic coating, scorch marks | "heat resistant hull, dark tiles, ceramic coating, sci-fi reentry armor" | TIR 2 atmospheric entry (1000 HP) |
| SHP-HULL3 | Titanium Alloy Hull | Perimeter | 3D model | Silver-titanium plating with brushed finish, reinforced joints | "titanium hull, silver plating, brushed metal, sci-fi spaceship armor" | TIR 3 maximum standard (2000 HP) |
| SHP-HULL4 | Reinforced Composite Hull | Perimeter | 3D model | Multi-layer composite with visible carbon fiber pattern, gold accent lines | "composite hull, carbon fiber pattern, gold accents, advanced sci-fi armor" | TIR 4 elite protection (3500 HP) |
| SHP-SHD1 | Basic Shield Emitter | Integrated | 3D model | Small emitter dish on ship flank, faint blue energy field overlay | "basic shield emitter, small dish, faint blue field, sci-fi" | +100 HP passive shield |
| SHP-SHD2 | Energy Shield Generator | 1x1 (emitter) | 3D model | Rotating ring emitter with visible blue energy dome when active | "energy shield generator, rotating ring, blue dome field, sci-fi" | 200 damage/second absorption |
| SHP-SHD3 | Alien Shield Emitter | 2x2 (emitter) | 3D model | Iridescent emitter array with shimmering barrier effect when active | "alien shield emitter, iridescent array, shimmering barrier, sci-fi" | Physical damage immunity |

### Scanning Modules (Ship)

| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| SHP-RAD1 | Basic Radar Array | 1x1 | 3D model | Small rotating dish antenna, gray housing, cable connections | "ship radar array, small rotating dish, sci-fi sensor" | Short-range scanning |
| SHP-SNL2 | Deep Sonar Emitter | 1x1+1x1 | 3D model | Paired emitter/receiver nodes, underwater-compatible housing | "deep sonar emitter, paired nodes, sci-fi submarine sensor" | Underground structure detection |
| SHP-LAS3 | Laser Targeting System | 1x1 | 3D model | Laser rangefinder pod, red targeting lens, cooling vents | "laser targeting system, rangefinder pod, red lens, sci-fi" | Precise target acquisition |
| SHP-SAT3 | Satellite Deployment Bay | 2x2 | 3D model | Launch tube array with satellite storage slots, deployment mechanism | "satellite bay, launch tubes, storage slots, sci-fi deployer" | Planet-wide monitoring |
| SHP-QSC5 | Quantum Scanner | 2x2 | 3D model | Spherical sensor array with pulsing quantum field, violet glow | "quantum scanner, spherical array, violet pulse, advanced sci-fi" | Full planet instant scan |

### Weapon Modules (Ship) — See also [Weapon Types](./Weapons/Weapon_Types.md)

| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| SHP-WPN-B2 | Ballistic Turret 2-Cannon | 1x1 | 3D model | Dual cannon turret mount, rotating base, ammo feed visible | "ballistic turret, dual cannons, rotating mount, sci-fi ship weapon" | Basic ship defense (TIR 1-2) |
| SHP-WPN-B4 | Ballistic Turret 4-Cannon | 2x2 | 3D model | Quad cannon heavy turret, reinforced base, dual ammo feeds | "heavy ballistic turret, four cannons, sci-fi ship weapon" | High damage output (TIR 2-3) |
| SHP-WPN-R1 | Small Rocket Bay | 2x1 | 3D model | Rocket pod array with 4 tubes, reload mechanism visible | "rocket bay, four tube pods, sci-fi missile launcher" | Area damage (TIR 2) |
| SHP-WPN-R2 | Large Rocket Bay | 3x2 | 3D model | Heavy rocket launcher with 8 tubes, specialized warhead selector | "large rocket bay, eight tubes, sci-fi heavy missile launcher" | Very high damage (TIR 3) |
| SHP-WPN-T4 | Torpedo Launcher | 2x2 | 3D model | Single large launch tube with guidance array, homing sensor dome | "torpedo launcher, single large tube, guidance array, sci-fi" | Extreme homing damage (TIR 4) |
| SHP-WPN-E1 | Energy Emitter | 1x1 | 3D model | Beam emitter barrel with cooling coil, energy tap connection | "energy emitter, beam barrel, cooling coil, sci-fi laser weapon" | Continuous beam (TIR 3) |
| SHP-WPN-PC4 | Plasma Cannon | 2x2 | 3D model | Superheated plasma projector, magnetic containment ring, charge indicator | "plasma cannon, magnetic ring, superheated barrel, sci-fi energy weapon" | Armor melting damage (TIR 4) |
| SHP-WPN-IE1 | Ion Emitter | 2x1 | 3D model | Ion projection array with pulse coils, electronic disruption field | "ion emitter, pulse coils, electronic disruption, sci-fi" | System disabling (TIR 3) |
| SHP-WPN-IS5 | Ion Storm Generator | 3x3 | 3D model | Large ion projector with expanding field rings, triple power taps | "ion storm generator, large projector, expanding fields, sci-fi" | Area electronic warfare (TIR 5) |
| SHP-WPN-VB5 | Void Beam | 3x2 | 3D model | Void energy projector with black-violet beam, space distortion effect | "void beam projector, black violet beam, space distortion, ultimate sci-fi weapon" | Armor-ignoring damage (TIR 5) |
| SHP-WPN-GW | Gravity Well Emitter | 4x4 | 3D model | Alien gravity projector with warping field, purple-black implosion effect | "gravity well emitter, alien projector, space warp, ultimate sci-fi" | Area control/implosion (Alien) |

### Lab/Research Modules (Ship)

| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| SHP-MED2 | Ship Med Bay | 2x1 | 3D model | Compact medical alcove with stasis pods, healing beam emitters | "ship med bay, compact medical, stasis pods, sci-fi healing" | Unit healing during travel (TIR 2) |
| SHP-ELAB3 | Energy Lab Module | 3x3 | 3D model | Miniature plasma containment chamber with holographic research displays | "energy lab module, plasma containment, holographic displays, sci-fi" | TIR 1-3 energy research (TIR 3) |
| SHP-DMLAB4 | Dark Matter Lab Module | 4x4 | 3D model | Suspended dark matter sphere in magnetic field, crystalline research array | "dark matter lab module, floating sphere, magnetic field, sci-fi" | TIR 3-4 dark matter research (TIR 4) |
| SHP-VLAB5 | Void Lab Module | 5x5 | 3D model | Void chamber with swirling center, alien glyph projections, black-violet glow | "void lab module, swirling void, alien glyphs, ultimate sci-fi" | TIR 5 and alien research (TIR 5) |

### Support Modules (Ship)

| Asset ID | Title | Dimensions (Grid) | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------------|------------|-------------|-------------------|----------|
| SHP-HAB1 | Crew Habitation Pod | 1x1 | 3D model | Cryo-stasis pod row, life support panel, emergency light strip | "crew habitation pod, cryo pods, life support, sci-fi sleeping quarters" | 2 crew capacity (starting) |
| SHP-HAB2 | Small Crew Quarters | 1x1 | 3D model | Expanded crew bay with 4 bunks, shared storage locker | "small crew quarters, four bunks, sci-fi ship interior" | +4 crew capacity (TIR 2) |
| SHP-HAB3 | Large Crew Quarters | 2x2 | 3D model | Full crew mess hall with 16 berths, galley corner, recreation screen | "large crew quarters, mess hall, sixteen berths, sci-fi" | +16 crew capacity (TIR 3) |
| SHP-HNG-S2 | Small Hangar | 2x2 | 3D model | Launch bay door with single vehicle slot, maintenance cranes | "small hangar bay, launch door, single vehicle slot, sci-fi" | 1 vehicle slot (TIR 2) |
| SHP-HNG-M3 | Medium Hangar | 4x4 | 3D model | Large hangar with 2 vehicle slots, refueling equipment | "medium hangar bay, two vehicle slots, sci-fi fleet base" | 2 vehicle slots (TIR 3) |
| SHP-HNG-L4 | Large Hangar | 6x6 | 3D model | Massive fleet hangar with 4 slots, full maintenance bay | "large hangar bay, four vehicle slots, sci-fi carrier deck" | 4 vehicle slots (TIR 4) |
| SHP-WRK2 | Workshop Bench | 2x1 | 3D model | Mobile crafting station with tool organizer, parts bins | "workshop bench, mobile crafting, tool organizer, sci-fi" | In-transit crafting (TIR 2) |

---

## 3. WEAPON ASSETS [See Weapon Types](./Weapons/Weapon_Types.md) & [TIR System](./Weapons/TIR_System.md)

### Ballistic Weapons

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| WPN-BALL-RIFLE | Ballistic Rifle (TIR 1) | Unit-held | 3D model | Standard assault rifle, gray metal with wooden stock, iron sights | "sci-fi ballistic rifle, gray metal, wooden stock, iron sights, infantry weapon" | Infantry primary weapon |
| WPN-BALL-MG | Heavy Machine Gun (TIR 2) | Unit-held | 3D model | Belt-fed heavy gun on bipod, drum magazine, heat shield | "sci-fi heavy machine gun, belt fed, bipod, drum mag, infantry weapon" | Squad support fire |
| WPN-BALL-CANNON | Ballistic Cannon (TIR 2) | Vehicle-mounted | 3D model | Large caliber cannon on swivel mount, armored breech, shell rack | "sci-fi ballistic cannon, large caliber, swivel mount, vehicle mounted" | Turret weapon / vehicle armament |
| WPN-BALL-ROT1 | Rotary Cannon (TIR 3) | Vehicle-mounted | 3D model | Gatling-style multi-barrel rotary gun, power cable feed, ammo hopper | "rotary cannon, gatling style, multiple barrels, sci-fi vehicle weapon" | High rate of fire |

### Plasma Weapons

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| WPN-PLAS-RIFLE | Plasma Rifle (TIR 2) | Unit-held | 3D model | Energy weapon with visible plasma coil, power pack on back, blue glow | "sci-fi plasma rifle, energy weapon, blue glow, plasma coil, infantry" | TIR 2 infantry upgrade |
| WPN-PLAS-CANNON | Plasma Cannon (TIR 4) | Vehicle-mounted | 3D model | Superheated plasma projector with magnetic ring, heat sink fins | "plasma cannon, magnetic containment ring, sci-fi vehicle weapon" | Ship/vehicle heavy armament |

### Rocket Weapons

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| WPN-ROK-LAUNCH | Rocket Launcher (TIR 1) | Unit-held | 3D model | Shoulder-fired tube, targeting sight on top, rocket in breech | "sci-fi rocket launcher, shoulder fired, tube design, infantry weapon" | TIR 1 anti-vehicle |
| WPN-ROK-BAY-S | Small Rocket Pod (TIR 2) | Vehicle-mounted | 3D model | 4-tube rocket pod array, reload mechanism, targeting computer | "small rocket pod, four tubes, sci-fi vehicle launcher" | Vehicle secondary weapon |
| WPN-ROK-BAY-L | Large Rocket Pod (TIR 3) | Vehicle-mounted | 3D model | 8-tube heavy rocket array, dual reload, warhead selector | "large rocket pod, eight tubes, sci-fi heavy launcher" | Heavy vehicle armament |
| WPN-TORP-LAUNCH | Torpedo Launcher (TIR 4) | Ship/vehicle | 3D model | Single large launch tube with guidance dome, homing sensor array | "torpedo launcher, single tube, guidance dome, sci-fi heavy weapon" | Homing extreme damage |

### Energy Weapons

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| WPN-ENER-BEAM | Energy Beam Emitter (TIR 3) | Ship-mounted | 3D model | Focused beam projector with cooling vents, energy tap cable | "energy beam emitter, focused projector, cooling vents, sci-fi ship weapon" | Continuous beam damage |
| WPN-ION-EMIT | Ion Emitter (TIR 3) | Ship/vehicle | 3D model | Ion projection array with pulse coils, electronic disruption field | "ion emitter, pulse coils, electronic disruption, sci-fi weapon" | System disabling |

### Void Weapons

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| WPN-VOID-BEAM | Void Beam Projector (TIR 5) | Ship-mounted | 3D model | Black-violet beam emitter with space distortion, void energy core | "void beam projector, black violet beam, space distortion, ultimate sci-fi" | Ultimate armor-piercing |
| WPN-GRAV-EMIT | Gravity Well Emitter (Alien) | Ship-mounted | 3D model | Alien gravity projector with warping field, iridescent implosion effect | "gravity well emitter, alien projector, space warp, ultimate sci-fi" | Ultimate area control |

---

## 4. UNIT ASSETS [See Units](./Units/Units.md) & [Upgrade System](./Units/Upgrade_System.md)

### Infantry Units (6 base types × TIR variants)

| Asset ID | Title | Height (Model) | Poly Count | Description | AI Prompt Keywords | Used For |
|----------|-------|----------------|------------|-------------|-------------------|----------|
| UNT-INF-SCOUT | Scout Infantry | 2.5 units tall | 6,000 | Lightly armored recon soldier with thermal visor, rifle slung, backpack comms | "sci-fi scout soldier, light armor, thermal visor, assault rifle, top-down game model" | Early game exploration |
| UNT-INF-SOLDIER | Standard Soldier | 2.5 units tall | 7,000 | Medium infantry with ballistic rifle, body armor, helmet with comms | "sci-fi soldier, medium armor, assault rifle, helmet, top-down game model" | Core combat unit |
| UNT-INF-HEAVY | Heavy Infantry | 2.8 units tall | 8,000 | Heavily armored trooper with plasma cannon, shoulder-mounted ammo pack | "sci-fi heavy infantry, heavy armor, plasma cannon, bulky, top-down game model" | Frontline combat |
| UNT-INF-MEDIC | Medic Unit | 2.5 units tall | 6,500 | Soldier with red cross armband, med kit backpack, rifle slung | "sci-fi medic soldier, red cross armband, medical kit, top-down game model" | Unit healing support |
| UNT-INF-ENGINEER | Engineer Unit | 2.5 units tall | 7,000 | Technician with tool belt, welding torch, reinforced vest | "sci-fi engineer, tool belt, welding torch, technician gear, top-down" | Structure repair/building |
| UNT-INF-CHAMPION | Champion Unit | 2.8 units tall | 8,500 | Elite champion with unique armor scheme, signature weapon, cape/patch | "sci-fi champion, elite armor, unique color scheme, signature weapon, top-down" | Player-controlled leader |

### Vehicle Units

| Asset ID | Title | Height (Model) | Poly Count | Description | AI Prompt Keywords | Used For |
|----------|-------|----------------|------------|-------------|-------------------|----------|
| UNT-VEH-JEEP | Scout Jeep | 1.5 units tall | 12,000 | Light four-wheel vehicle with mounted turret ring, open top | "sci-fi scout jeep, four wheels, mounted turret ring, light vehicle, top-down" | Fast reconnaissance |
| UNT-VEH-APC | APC (Armored Personnel Carrier) | 2 units tall | 18,000 | Tracked armored transport with rear door, machine gun mount | "sci-fi APC, tracked armor, personnel carrier, machine gun mount, top-down" | Unit transport |
| UNT-VEH-TANK | Main Battle Tank | 2.5 units tall | 20,000 | Heavy tank with rotating turret, main cannon, composite armor | "sci-fi main battle tank, heavy armor, rotating turret, large cannon, top-down" | Heavy combat |
| UNT-VEH-MOBILE | Mobile Factory Platform | 3 units tall | 25,000 | Large tracked vehicle with assembly arms, parts storage bay | "sci-fi mobile factory, tracked platform, assembly arms, heavy vehicle, top-down" | On-site production |

### Aerial Units

| Asset ID | Title | Height (Model) | Poly Count | Description | AI Prompt Keywords | Used For |
|----------|-------|----------------|------------|-------------|-------------------|----------|
| UNT-AIR-DRONE | Scout Drone | 0.5 units tall | 3,000 | Small quadcopter with camera pod, propeller guards | "sci-fi scout drone, quadcopter, camera pod, small aerial, top-down" | Area reconnaissance |
| UNT-AIR-FIGHTER | Fighter Jet | 1.5 units tall | 15,000 | Sleek fighter with swept wings, plasma cannons under wings, afterburner | "sci-fi fighter jet, swept wings, plasma cannons, afterburner, top-down" | Air combat |
| UNT-AIR-BOMBER | Bomber Aircraft | 2 units tall | 18,000 | Wide-wing bomber with bomb bay doors, twin engines, missile rails | "sci-fi bomber aircraft, wide wings, bomb bay, missiles, top-down" | Area bombardment |

### Mech Units

| Asset ID | Title | Height (Model) | Poly Count | Description | AI Prompt Keywords | Used For |
|----------|-------|----------------|------------|-------------|-------------|----------|
| UNT-MECH-LIGHT | Light Mech | 4 units tall | 12,000 | Bipedal mech with arm-mounted cannon, thruster packs on back | "sci-fi light mech, bipedal, arm cannon, thrusters, top-down game model" | Fast melee/ranged combat |
| UNT-MECH-HEAVY | Heavy Mech | 5 units tall | 15,000 | Large bipedal mech with dual plasma cannons, shield generator on back | "sci-fi heavy mech, large bipedal, dual plasma cannons, shield generator, top-down" | Frontline assault |

---

## 5. RESOURCE ASSETS [See Resources](./Resources/TYPES.md)

### Resource Node Visuals (by type and color)

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| RES-NODE-STN | Stone Deposit | 2x2 surface | 3D model | Gray rocky outcrop with visible stone fractures, dust particles | "stone deposit, gray rock outcrop, fractures, dusty, planet surface" | Construction material source |
| RES-NODE-CRY | Crystal Deposit | 2x2 surface | 3D model | Purple/blue crystalline formation, glowing edges, jagged spikes | "crystal deposit, purple blue crystals, glowing edges, jagged spikes, planet surface" | Crystal mineral source |
| RES-NODE-MTL | Metal Ore Deposit | 2x2 surface | 3D model | Metallic gray rock with visible ore veins, iron sparkle | "metal ore deposit, metallic rock, ore veins, iron sparkle, planet surface" | Metal/titanium source |
| RES-NODE-CAL | Coal Deposit | 2x2 surface | 3D model | Black rocky formation with coal seams, dark dust | "coal deposit, black rocks, coal seams, dark dust, planet surface" | Coal fuel source |
| RES-NODE-OIL | Oil Well Head | 1x1 surface | 3D model | Dark liquid pool with bubbling, black viscous fluid, gas vent pipe | "oil well head, black liquid pool, bubbling, viscous, planet surface" | Oil fuel source |
| RES-NODE-WTR | Water Source | 2x2 surface | 3D model | Blue water pool, gentle ripples, reflection, surrounded by rocks | "water source, blue pool, ripples, reflections, planet surface" | Water/survival source |
| RES-NODE-GAS | Gas Deposit | 2x2 surface | 3D model | Greenish gas cloud hovering low, bubbling ground, sulfur smell visible | "gas deposit, green gas cloud, bubbling ground, sulfur, planet surface" | Gas fuel source |
| RES-NODE-URAN | Uranium Deposit | 2x2 surface | 3D model | Green-glowing radioactive rock, radiation symbol etched, steam vents | "uranium deposit, green glow, radioactive, steam vents, planet surface" | Radioactive uranium source |
| RES-NODE-DARKM | Dark Matter Crystal | 1x1 surface | 3D model | Black crystal with purple shimmer, light-bending distortion around it | "dark matter crystal, black with purple shimmer, light bending, alien" | TIR 4+ research fuel |

### Resource Item Sprites (2D inventory icons)

| Asset ID | Title | Sprite Size | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------|-------------|-------------------|----------|
| RES-ITM-CON | Construction Material Stack | 64x64px | Pile of concrete blocks and wooden planks | "icon, pile of concrete blocks and wood planks, isometric, game UI icon" | Inventory display |
| RES-ITM-MIN | Mineral Collection | 64x64px | Mixed mineral rocks in metal tray | "icon, mixed minerals in metal tray, isometric, game UI icon" | Inventory display |
| RES-ITM-FUEL | Fuel Canister | 64x64px | Yellow fuel can with hazard label | "icon, yellow fuel canister, hazard label, isometric, game UI icon" | Inventory display |
| RES-ITM-HULL | Hull Plating Piece | 64x64px | Metal armor plate with rivets | "icon, metal armor plate, riveted, isometric, game UI icon" | Inventory display |
| RES-ITM-SRV | Survival Ration Pack | 64x64px | MRE-style ration pack with medical cross | "icon, survival ration pack, medical cross, isometric, game UI icon" | Inventory display |
| RES-ITM-CRYST | Dark Matter Crystal | 64x64px | Small black crystal with purple glow | "icon, dark matter crystal, purple glow, isometric, game UI icon" | Inventory display |

---

## 6. PLANET / ENVIRONMENT ASSETS [See Planet Types](./Planet/Planet_Types.md) & [Navigation](./Planet/Navigation.md)

### Biome Terrain Textures (seamless tileable)

| Asset ID | Title | Texture Size | Description | AI Prompt Keywords | Used For |
|----------|-------|-------------|-------------|-------------------|----------|
| TMRT-DESERT | Desert Sand Terrain | 1024x1024 | Fine sand dunes with scattered rocks, warm tan color | "seamless desert sand texture, dunes, scattered rocks, warm tan, top-down" | Desert planet surface |
| TMRT-DUSTY | Dusty Plain Terrain | 1024x1024 | Dry cracked earth with dust particles, brown-gray | "seamless dusty terrain, cracked earth, dry, brown gray, top-down" | Dusty planet surface |
| TMRT-ROCKY | Rocky Wasteland Terrain | 1024x1024 | Jagged rocks, gravel, gray-brown with sparse vegetation | "seamless rocky terrain, jagged rocks, gravel, gray brown, top-down" | Rocky planet surface |
| TMRT-WATER | Shallow Water Terrain | 1024x1024 | Clear blue water with sandy bottom visible, gentle ripples | "seamless shallow water texture, blue, sandy bottom, ripples, top-down" | Water planet surface |
| TMRT-SWAMP | Swamp Terrain | 1024x1024 | Murky brown water with green algae patches, mud | "seamless swamp terrain, murky water, green algae, mud, top-down" | Swamp planet surface |
| TMRT-JUNGLE | Jungle Canopy Terrain | 1024x1024 | Dense green canopy, tree tops visible, dappled light | "seamless jungle canopy texture, dense green, tree tops, dappled light, top-down" | Jungle planet surface |
| TMRT-SNOW | Light Snow Terrain | 1024x1024 | White snow cover with some exposed rock, cold blue tint | "seamless snow terrain, white snow, exposed rock, cold blue, top-down" | Light snow planet surface |
| TMRT-ICE | Ice Sheet Terrain | 1024x1024 | Thick ice sheet, cracks and crevasses, bright white-blue | "seamless ice sheet texture, thick ice, cracks, white blue, top-down" | Ice planet surface |

### Environmental Props (decoration)

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| ENV-ROCK-SM | Small Rock | 0.5 units | 3D model | Irregular gray rock, 1m diameter | "small irregular rock, gray, low poly, game prop" | Desert/rocky decoration |
| ENV-ROCK-LG | Large Boulder | 2 units | 3D model | Massive rock formation, 4m diameter, cracked surface | "large boulder, massive rock, cracked, low poly, game prop" | Rocky terrain decoration |
| ENV-TREE-SM | Small Bush | 1 unit | 3D model | Low green bush, sparse leaves | "small green bush, low poly, game prop, top-down" | Jungle/decoration |
| ENV-TREE-LG | Dead Tree | 3 units | 3D model | Leafless tree trunk with branches, gray bark | "dead tree, leafless, gray bark, branches, low poly, game prop" | Desert/dusty decoration |
| ENV-CRST-SM | Small Crystal Cluster | 1 unit | 3D model | Purple crystal cluster, small glowing tips | "small crystal cluster, purple, glowing tips, low poly, game prop" | Crystal cave decoration |
| ENV-CRST-LG | Large Crystal Formation | 3 units | 3D model | Towering crystal spire, bright glow, energy arcs | "large crystal formation, towering spire, bright glow, energy arcs" | Crystal cave feature |
| ENV-VENT | Steam Vent | 1 unit | 3D model | Rock vent with white steam plume rising | "steam vent, rock opening, white steam plume, low poly" | Geothermal area decoration |

### Dungeon Interior Props

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| DNG-DOOR | Metal Door (Closed) | 1 unit wide | 3D model | Heavy steel door with locking mechanism, red status light | "heavy metal door, locked, red light, sci-fi dungeon" | Dungeon room separator |
| DNG-DOOR-O | Metal Door (Open) | 1 unit wide | 3D model | Same as closed but swung open, interior visible | "open metal door, sci-fi dungeon, interior visible" | Accessible passage |
| DNG-CNST | Container/Crate | 1x1x1 units | 3D model | Military-style storage crate, locked, label on side | "military crate, locked, labels, sci-fi storage" | Loot container |
| DNG-LTFL | Flickering Light Fixture | Ceiling-mounted | 3D model | Fluorescent tube light, flickering, exposed wiring | "flickering fluorescent light, exposed wires, dungeon ceiling" | Dungeon atmosphere |
| DNG-BDY | Body/Rubble Pile | 1-2 units | 3D model | Fallen soldier or debris pile, armor fragments visible | "fallen soldier body, armor fragments, debris pile, sci-fi dungeon" | Loot source |

---

## 7. DROPSHIP ASSETS [See Dropship](./Spaceship/Dropship.md) & [Progression](./Spaceship/Progression.md)

### Dropship Components (modular)

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SHP-DROP-COCK | Cockpit Module | 1x1x1 | 3D model | Glass-domed cockpit with pilot seat, instrument panel, control sticks | "dropship cockpit, glass dome, pilot seat, instruments, sci-fi" | Pilot/command center |
| SHP-DROP-RDT | Right Drive (Standard) | 1x1x1 | 3D model | Rocket engine nozzle with orange exhaust, fuel lines | "rocket drive module, orange exhaust, fuel lines, dropship part" | Right side propulsion |
| SHP-DROP-LDT | Left Drive (Damaged) | 1x1x1 | 3D model | Damaged rocket engine, bent nozzle, sparking wires, partial exhaust | "damaged rocket drive, bent nozzle, sparks, dropship part" | Starting left propulsion |
| SHP-DROP-HAB | Habitation Module | 2x2x1 | 3D model | Pressurized hab section with cryo-pod windows, airlock door | "habitation module, pressurized, cryo pod windows, airlock, dropship" | Crew quarters (10 soldiers) |
| SHP-DROP-STR | Storage Module | 1x1x1 | 3D model | Cargo bay with visible supply crates, open hatch | "storage module, cargo bay, supply crates, open hatch, dropship" | Resource storage |
| SHP-DROP-ENC | Energy Core | 1x1x1 | 3D model | Reactor core with glowing blue center, cooling pipes, radiation symbol | "energy core, blue glow, reactor, cooling pipes, dropship power" | Ship power generation |
| SHP-DROP-HULL | Dropship Hull (Damaged) | Perimeter | 3D model | Rusty corrugated metal hull panels, scorch marks, rivet lines | "dropship hull, rusty metal, scorch marks, rivets, damaged" | Outer protection |

### Dropship Landing State

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SHP-DROP-CRASH | Full Dropship (Crashed) | 6x4x2 | 3D model | Crashed dropship half-buried in sand, smoke rising, side view angle | "crashed dropship, half buried in sand, smoke, side view, desert planet" | Opening cinematic |

---

## 8. UI ASSETS [See Core Loop](./Gameplay/Core_Loop.md)

### HUD Elements (2D vector/PNG)

| Asset ID | Title | Size | Description | AI Prompt Keywords | Used For |
|----------|-------|------|-------------|-------------------|----------|
| UI-HUD-RES | Resource Bar | 400x60px | Horizontal bar showing 6 resource icons with numerical counters, colored backgrounds per type | "UI resource bar, six icons, numbers, dark background, sci-fi HUD" | Top HUD display |
| UI-HUD-TIR | TIR Indicator | 100x100px | Circular gauge showing current TIR tier (1-5) with color-coded ring | "TIR indicator, circular gauge, colored ring, sci-fi HUD element" | Progression display |
| UI-HUD-FUEL | Fuel Gauge | 200x40px | Horizontal fuel bar with gradient from red to green | "fuel gauge, horizontal bar, red to green gradient, sci-fi UI" | Ship fuel display |
| UI-HUD-HP | Unit HP Bar | 150x30px | Health bar with red fill, white border, numerical HP text | "HP bar, red fill, white border, numbers, game UI element" | Unit health display |
| UI-HUD-ENERGY | Energy Meter | 200x40px | Energy capacity bar with blue glow effect | "energy meter, blue glow, horizontal bar, sci-fi UI" | Ship energy display |
| UI-MAP | Mini-map Radar | 200x200px | Circular radar display with blips for units and structures | "mini-map radar, circular, blips, green phosphor, sci-fi" | Planet exploration map |

### Icon Assets (64x64px each)

| Asset ID | Title | Size | Description | AI Prompt Keywords | Used For |
|----------|-------|------|-------------|-------------------|----------|
| UI-ICN-BLD | Building Placement Icon | 64x64px | Blue building outline with plus sign, grid overlay | "building placement icon, blue outline, plus sign, grid, game UI" | Build mode indicator |
| UI-ICN-EXP | Explore Icon | 64x64px | Compass/eye symbol pointing outward | "explore icon, compass eye, directional arrow, game UI" | Send units to explore |
| UI-ICN-ATT | Attack Icon | 64x64px | Red crossed swords or explosion symbol | "attack icon, red crossed swords, explosion, game UI" | Combat command |
| UI-ICN-DEF | Defend Icon | 64x64px | Blue shield with checkmark | "defend icon, blue shield, checkmark, game UI" | Defensive formation |
| UI-ICN-SCH | Scout Icon | 64x64px | Eye symbol with radar waves | "scout icon, eye, radar waves, game UI" | Reconnaissance order |
| UI-ICN-RSR | Research Icon | 64x64px | Flask/beaker with gear symbol | "research icon, flask beaker, gear, game UI" | Open research tree |

### Menu Screens (full resolution)

| Asset ID | Title | Resolution | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|-------------|-------------------|----------|
| UI-MNU-MAIN | Main Menu Background | 1920x1080 | Space scene with crashed dropship on planet surface, stars above | "main menu background, space scene, crashed ship on planet, stars, sci-fi" | Game start screen |
| UI-MNU-FAC | Faction Selection Screen | 1920x1080 | Four faction portraits in grid, each with color scheme and emblem | "faction selection screen, four portraits, grid layout, emblems, sci-fi" | Choose faction at game start |
| UI-MNU-CHAMP | Champion Selection Screen | 1920x1080 | Twelve champion silhouettes arranged in grid, hover highlight effect | "champion selection, twelve silhouettes, grid, hover highlight, sci-fi" | Choose champion at game start |
| UI-MNU-MAP | Planet Map Screen | 1920x1080 | Top-down planet view with radar overlay, unit blips, terrain colors | "planet map screen, top-down, radar overlay, blips, terrain colors" | In-game exploration map |

---

## 9. ANIMATION ASSETS

### Animation Types Required

| Asset Type | Count | Description | Frame Rate | Notes |
|------------|-------|-------------|------------|-------|
| Building Construction | 21 buildings × 3 stages = 63 frames | Each building has construction animation (foundation → walls → roof) | 30 fps | 2-second build time in-game |
| Resource Extraction | 8 resource types | Continuous extraction animation (conveyor moving, pump oscillating, smoke rising) | 30 fps | Looping animations |
| Unit Walk Cycle | 6 unit types × 4 directions = 24 cycles | Standard walking animation with 8 frames per cycle | 12 fps (retro style) | Each frame is a sprite or model rotation |
| Unit Attack Animation | 6 unit types × 3 weapon types = 18 animations | Attack wind-up, fire, recovery | 15 fps | Weapon-specific variations |
| Building Damage | All buildings | Hit flash (white), damage smoke, collapse animation | 30 fps | Triggered on damage events |
| Dropship Landing | 1 sequence | Descent, thruster firing, touchdown, dust cloud | 30 fps | 5-second cinematic |
| Dropship Launch | 1 sequence | Thrusters ignite, lift off, ascent into sky | 30 fps | Reverse of landing |
| Explosion Effects | 5 sizes (small to planet-wide) | Fireball expansion, smoke ring, debris scatter | 60 fps | Particle effect system |
| Energy Weapon Beam | All energy weapons | Beam projection with glow and particle trail | 60 fps | Real-time shader effect |
| Shield Activation | 3 shield types | Dome materialization from center outward | 30 fps | Transition animation |

---

## 10. SPECIAL EFFECTS (VFX) ASSETS

| Asset ID | Title | Format | Description | AI Prompt Keywords | Used For |
|----------|-------|--------|-------------|-------------------|----------|
| VFX-EXP-SM | Small Explosion | Sprite sheet (8 frames) | Quick fireball, small smoke puff | "explosion sprite frame 1 of 8, orange fireball, game VFX" | Small weapon impacts |
| VFX-EXP-MD | Medium Explosion | Sprite sheet (12 frames) | Larger fireball, expanding smoke ring, debris | "medium explosion sprite sequence, fire and smoke, game VFX" | Vehicle destruction |
| VFX-EXP-LG | Large Explosion | Sprite sheet (16 frames) | Massive fireball, mushroom cloud, shockwave ring | "large explosion sprite sequence, mushroom cloud, shockwave, game VFX" | Building/ship destruction |
| VFX-SMOKE-DK | Dark Smoke Plume | Particle texture (4 frames loop) | Black/dark gray smoke rising and dissipating | "dark smoke particle frame 1 of 4, rising plume, looping" | Coal reactor exhaust |
| VFX-SMOKE-WH | White Steam Plume | Particle texture (4 frames loop) | White/gray steam rising | "white steam particle frame 1 of 4, rising vapor, looping" | Geothermal vent output |
| VFX-ENERGY-BL | Blue Energy Field | Shader material | Transparent blue dome with hexagonal pattern | "blue energy field shader, hexagonal pattern, transparent dome" | Shield generator effect |
| VFX-VOID-PURP | Void Swirl Effect | Shader material | Purple-black swirling distortion with starlight edges | "void swirl shader, purple black, starlight edges, distortion" | Void drive/lab effects |
| VFX-ALien-SHR | Alien Shimmer | Shader material | Iridescent color-shifting field | "alien shimmer shader, iridescent, color shifting, rainbow" | Alien tech active effect |

---

## 11. AUDIO ASSETS (Brief)

| Asset ID | Title | Format | Duration | Description | Used For |
|----------|-------|--------|----------|-------------|----------|
| AUD-BGM-MAIN | Main Menu Music | WAV/OGG | 3:00 loop | Ambient space drone with subtle mechanical pulses | Main menu, menus |
| AUD-BGM-COMBAT | Combat Music | WAV/OGG | 2:30 loop | Driving percussion with synth stabs | Planet combat encounters |
| AUD-SFX-BUILD | Building Placement | WAV | 0.5s | Metallic clang + hydraulic hiss | All building construction |
| AUD-SFX-SELECT | Unit Selection | WAV | 0.2s | Short electronic beep | Selecting units/buildings |
| AUD-SFX-ATTACK | Weapon Fire | WAV | 0.3-1.0s | Varies by weapon type (ballistic crack, plasma zap) | All combat attacks |
| AUD-SFX-EXPLODE | Explosion | WAV | 0.5-2.0s | Low boom with high frequency crack | All explosions |

---

## Asset Summary by Category

| Category | Total Assets | 3D Models | Sprite Sheets | Shaders/VFX | Audio |
|----------|-------------|-----------|---------------|-------------|-------|
| Buildings | 32 | 32 | - | - | - |
| Ship Modules | 35+ | 35+ | - | - | - |
| Weapons (Unit-held) | 12 | 12 | - | - | - |
| Weapons (Ship-mounted) | 12 | 12 | - | - | - |
| Units (Base types) | 10 | 10 | - | - | - |
| Resources (Nodes) | 9 | 9 | - | - | - |
| Resources (Inventory icons) | 6 | - | 6 | - | - |
| Terrain Textures | 8 | - | 8 | - | - |
| Environment Props | 7 | 7 | - | - | - |
| Dungeon Props | 5 | 5 | - | - | - |
| Dropship Components | 8 | 8 | - | - | - |
| HUD Elements | 6 | - | 6 | - | - |
| UI Icons | 6 | - | 6 | - | - |
| Menu Screens | 4 | - | 4 | - | - |
| Animations | ~180+ frame sets | Model rotations | Sprite sequences | - | - |
| VFX | 8 | - | Sprite sheets | 4 shaders | - |
| Audio | 6 | - | - | - | 6 files |
| **TOTAL** | **~350+ assets** | **170+** | **40+** | **4** | **6** |

---

## 12. SPACE ENVIRONMENT ASSETS [See Space Travel](./Gameplay/Space_Travel.md) & [Navigation](./Planet/Navigation.md)

### Solar System Stars/Suns

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SP-SUN-TYPE1 | G-Type Yellow Star | Full-screen background | 3D sphere + shader | Large yellow-white star with surface granulation, corona glow, lens flare effect | "yellow sun star, large sphere, corona glow, lens flare, space background, top-down game" | Standard solar system center |
| SP-SUN-TYPE2 | K-Type Orange Star | Full-screen background | 3D sphere + shader | Medium orange star with visible surface convection cells, warm glow | "orange dwarf star, medium size, warm orange glow, surface convection, space background" | Orange star systems |
| SP-SUN-TYPE3 | M-Type Red Dwarf | Full-screen background | 3D sphere + shader | Small red star with intense surface flares, deep crimson color, dark surroundings | "red dwarf star, small size, intense flares, deep red, space background, dramatic lighting" | Red dwarf systems (common) |
| SP-SUN-TYPE4 | A-Type Blue-White Star | Full-screen background | 3D sphere + shader | Large bright blue-white star with intense UV glow, surrounding atmospheric distortion | "blue white star, large size, intense UV glow, atmospheric distortion, space background" | Rare blue star systems |
| SP-SUN-TYPE5 | Binary Star System | Full-screen background | 2 spheres + shader | Two stars orbiting each other, one yellow + one red/orange, shared corona | "binary stars, two stars orbiting, yellow and red, shared corona glow, space background" | Binary star systems |
| SP-SUN-TYPE6 | Dying Red Giant Star | Full-screen background | 3D sphere + shader | Massive swollen red star with expanding outer atmosphere, pulsating surface, deep red-orange | "red giant star, massive swollen, expanding atmosphere, pulsating surface, space background" | Endgame/center galaxy systems |

### Asteroid Belt Assets

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SP-AST-SM1 | Small Asteroid (Rock) | 2-5 units diameter | 3D model | Irregular rocky body, gray-brown surface with craters, no atmosphere | "small asteroid, irregular rock, gray brown, cratered surface, space asset, low poly" | Asteroid belt decoration |
| SP-AST-SM2 | Small Asteroid (Metallic) | 2-5 units diameter | 3D model | Metallic asteroid with visible ore veins, iron-nickel composition, reflective patches | "metallic asteroid, visible ore veins, iron nickel, reflective patches, space asset" | Metal-rich asteroid belt |
| SP-AST-MD1 | Medium Asteroid (Ice-Rock) | 5-15 units diameter | 3D model | Mixed ice-rock body with white ice caps and gray rocky surface, small craters | "medium asteroid, ice rock mix, white ice caps, gray rock, space asset" | Ice-rich asteroid belt |
| SP-AST-MD2 | Medium Asteroid (Carbonaceous) | 5-15 units diameter | 3D model | Dark carbon-covered asteroid, almost black with subtle blue mineral streaks | "carbonaceous asteroid, dark black surface, blue mineral streaks, space asset" | Carbon-rich belt zone |
| SP-AST-LG1 | Large Asteroid (Molten Core) | 15-40 units diameter | 3D model | Cracked large asteroid with visible molten orange interior through fissures, lava flows | "large cracked asteroid, molten core visible, orange fissures, lava flows, space asset" | Hazardous asteroid zone |
| SP-AST-LG2 | Large Asteroid (Crystalline) | 15-40 units diameter | 3D model | Massive crystal formation embedded in rock surface, purple/blue glow from within | "large crystalline asteroid, crystal formations, purple blue glow, space asset" | Crystal-rich asteroid belt |
| SP-AST-HG1 | Hollow Asteroid (Abandoned) | 20-50 units diameter | 3D model | Hollowed-out asteroid with visible habitat dome, antenna array, docking ports | "hollowed asteroid, habitat dome, antenna array, docking ports, abandoned, space asset" | Abandoned asteroid base |
| SP-AST-HG2 | Asteroid Fortress | 40-80 units diameter | 3D model | Heavily fortified asteroid with outer armor plating, turret mounts, energy shields | "asteroid fortress, armored plating, turret mounts, energy shield dome, space asset" | Enemy asteroid base |

### Space Debris Fields

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SP-DEB-SM1 | Metal Scrap Cluster | 1-3 units | 3D model | Small twisted metal fragments, bolts and wires, rusted appearance | "space debris cluster, twisted metal, bolts wires, rusted, floating in space" | Common debris field |
| SP-DEB-SM2 | Satellite Fragment | 2-4 units | 3D model | Broken solar panel piece with reflective surface, antenna fragment | "broken satellite, solar panel fragment, reflective surface, antenna piece, space debris" | Tech-rich debris |
| SP-DEB-MD1 | Destroyed Scout Ship | 5-8 units | 3D model | Wreckage of small scout vessel, exposed wiring, broken cockpit glass, floating fuel cells | "destroyed scout ship, wreckage, exposed wires, broken cockpit, space debris" | Combat encounter loot |
| SP-DEB-MD2 | Cargo Container Drift | 3-5 units | 3D model | Standard cargo container with torn label, slowly rotating, thruster marks | "drifting cargo container, torn label, slowly rotating, thruster marks, space debris" | Lootable debris field |
| SP-DEB-LG1 | Destroyed Frigate Wreckage | 15-25 units | 3D model | Large frigate broken in half, exposed engine core, floating crew pods, fire remnants | "destroyed frigate, broken in half, exposed engine, floating crew pods, space debris" | Major loot source |
| SP-DEB-LG2 | Space Station Rubble | 20-40 units | 3D model | Fragmented station ring sections, exposed habitation modules, solar panel arrays | "space station rubble, ring fragments, habitation module, solar panels, space debris" | Station loot source |

### Abandoned Space Stations

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SP-STN-SM1 | Small Outpost (Inactive) | 8x8 units | 3D model | Compact station with single rotating ring, dark windows, solar panels folded | "small space outpost, single rotating ring, dark windows, folded solar panels, abandoned" | Early game exploration |
| SP-STN-MD1 | Medium Research Station | 15x15 units | 3D model | Multi-ring station with laboratory modules, antenna array, docking arms | "medium research station, multi-ring, laboratory modules, antenna array, space asset" | Mid-game research loot |
| SP-STN-MD2 | Mining Operations Hub | 12x12 units | 3D model | Industrial station with ore processing bay, cargo loading docks, mining laser mount | "mining hub station, industrial, ore processing bay, cargo docks, mining laser, space asset" | Mining blueprint source |
| SP-STN-LG1 | Military Outpost (Damaged) | 20x20 units | 3D model | Fortified station with turret mounts, hangar bays, damaged shield generator, scorch marks | "military outpost station, fortified, turret mounts, damaged shields, scorch marks, space asset" | Combat encounter + loot |
| SP-STN-LG2 | Trade Station (Ruined) | 18x18 units | 3D model | Large circular station with multiple docking arms, commercial signage faded, fuel depots | "ruined trade station, large circle, multiple docks, faded signage, fuel depots, space asset" | Trading blueprint source |
| SP-STN-XL1 | Alien Observatory Station | 25x25 units | 3D model | Unique organic-metal design with large lens array, crystalline antenna, iridescent hull | "alien observatory station, organic metal design, large lens array, crystalline antenna, iridescent" | Alien tech source |
| SP-STN-XL2 | Void Research Platform | 30x30 units | 3D model | Dark platform with central void chamber, energy conduits from all sides, black-violet glow | "void research platform, dark structure, central void chamber, energy conduits, violet glow" | TIR 5 research source |

### Orbital Satellites

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SP-SAT-SCN1 | Scout Satellite (Inactive) | 2x2 units | 3D model | Small satellite with dish antenna, solar panel wings, status light off | "inactive scout satellite, dish antenna, solar panels, status light off, space asset" | Scanning data loot |
| SP-SAT-NAV1 | Navigation Beacon Satellite | 2x2 units | 3D model | Active satellite with pulsing blue light, multiple antennas, solar array deployed | "navigation beacon satellite, pulsing blue light, multiple antennas, solar array active" | System navigation aid |
| SP-SAT-MIL1 | Military Surveillance Sat | 3x3 units | 3D model | Armed satellite with targeting laser, weapon pods, rotating sensor dome | "military surveillance satellite, targeting laser, weapon pods, rotating sensor, space asset" | Enemy patrol + combat |
| SP-SAT-RES1 | Resource Survey Satellite | 2x2 units | 3D model | Stationary satellite with spectral scanner, data relay antenna, solar wings | "resource survey satellite, spectral scanner, data relay antenna, stationary orbit" | Resource detection |
| SP-SAT-WPN1 | Orbital Defense Platform | 4x4 units | 3D model | Weaponized satellite with dual cannons, missile pods, energy core glow | "orbital defense platform, dual cannons, missile pods, energy core glow, space asset" | Space combat encounter |

### Wormhole / Warp Gate Assets

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SP-WHM-01 | Natural Wormhole Entrance | Full-screen event | Shader + particles | Swirling purple-black portal with starlight edges, gravitational lensing effect, energy particles | "wormhole entrance, swirling purple black, starlight edges, gravitational lensing, space event" | Inter-system travel |
| SP-WHM-02 | Artificial Wormhole Gate | 10x10 units | 3D model + shader | Ring-shaped gate structure with inner portal surface, energy conduits along ring, status lights | "artificial wormhole gate, ring structure, inner portal, energy conduits, space asset" | Player-placed transit |
| SP-WHM-03 | Alien Warp Portal | 15x15 units | Shader + particles | Iridescent oval portal with shifting colors, organic feel, gravitational distortion waves | "alien warp portal, iridescent oval, shifting colors, organic, gravitational distortion" | Alien galaxy access |

### Nebula / Space Phenomena

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SP-NB-SM1 | Small Dust Cloud | 10-20 units radius | Shader overlay | Semi-transparent gray-brown gas cloud, obscures background stars, subtle particle drift | "small nebula dust cloud, semi-transparent gray brown, obscured stars, space phenomenon" | Navigation hazard |
| SP-NB-LG1 | Large Nebula Region | Full-screen background | Shader + particles | Vast colorful gas cloud with embedded star formation, purple-pink-blue gradients, luminous | "large nebula region, colorful gas cloud, star formation, purple pink blue, space phenomenon" | Rare resource zone |
| SP-PLSMA1 | Plasma Storm | 5-15 units radius | Shader + particles | Electric blue-purple energy field with visible lightning arcs, crackling sound effect | "plasma storm, electric blue purple, visible lightning arcs, energy field, space phenomenon" | Energy drain hazard |
| SP-RADZ1 | Radiation Zone | Variable radius | Shader overlay | Green-yellow glow with radiation symbol overlay, particle count increases near center | "radiation zone, green yellow glow, radiation symbol, particle effect, space phenomenon" | Ship damage over time |
| SP-GRAV1 | Gravity Anomaly | 3-8 units radius | Shader + particles | Distorted space visual with nearby objects pulled inward, warping light effect | "gravity anomaly, distorted space, objects pulled inward, light warping, space phenomenon" | Trajectory deviation |

### Space Enemy Ships (Pirate/Hostile)

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SP-ENM-SK1 | Pirate Skiff | 3x2 units | 3D model | Small fast ship with patched hull, mismatched panels, single cannon mount, red stripe paint | "pirate skiff, small fast ship, patched hull, mismatched panels, red stripe, space asset" | Early space combat |
| SP-ENM-FGT1 | Hostile Fighter | 4x3 units | 3D model | Sleek interceptor with swept wings, plasma cannons, faction emblem on fuselage | "hostile fighter, sleek interceptor, swept wings, plasma cannons, faction emblem, space asset" | Mid-game space combat |
| SP-ENM-CRV1 | Cargo Raider | 6x4 units | 3D model | Converted cargo ship with reinforced bow, multiple weapon mounts, heavy armor plating | "cargo raider, converted cargo ship, reinforced bow, weapon mounts, armored, space asset" | Loot-rich combat target |
| SP-ENM-CRV2 | War Cruiser | 10x8 units | 3D model | Military cruiser with dual bridge sections, torpedo tubes, energy shield emitter, heavy armor | "war cruiser, military, dual bridge, torpedo tubes, energy shield, space asset" | Late-game space combat |
| SP-ENM-BSS1 | Alien Mothership | 20x15 units | 3D model | Massive organic-metal vessel with iridescent hull, multiple weapon nodes, void energy core glow | "alien mothership, massive organic metal, iridescent hull, weapon nodes, void core glow" | Boss encounter |

### Space Structures / Installations

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SP-STR-DCK1 | Automated Docking Bay | 8x8 units | 3D model | Empty docking station with illuminated landing pads, fuel pumps, repair cranes | "automated docking bay, illuminated landing pads, fuel pumps, repair cranes, space asset" | Ship refuel/repair |
| SP-STR-MNR1 | Asteroid Mining Platform | 10x10 units | 3D model | Stationary platform with drilling arm, ore processing module, cargo launch tube | "asteroid mining platform, drilling arm, ore processing, cargo launch tube, space asset" | Resource generation |
| SP-STR-OBS1 | Deep Space Observatory | 6x6 units | 3D model | Large telescope array with rotating dish, sensor pods, data storage modules | "deep space observatory, large telescope, rotating dish, sensor pods, space asset" | Lore/data discovery |
| SP-STR-WPN1 | Orbital Cannon Battery | 8x8 units | 3D model | Heavy cannon mount on reinforced platform, ammo storage bay, energy capacitor bank | "orbital cannon battery, heavy cannon, reinforced platform, ammo storage, space asset" | Planet bombardment support |
| SP-STR-PRN1 | Abandoned Child Station | 5x5 units | 3D model | Small station with colorful markings, playground module visible through window, broken comms antenna | "abandoned child station, colorful markings, playground module, broken antenna, space asset" | Emotional lore + loot |
| SP-STR-WRP1 | Alien Warp Anchor | 12x12 units | 3D model | Tall crystalline spire with energy ring at top, alien glyphs carved into surface, purple glow | "alien warp anchor, crystalline spire, energy ring, alien glyphs, purple glow" | Wormhole stabilization |

### Space Weather / Background Elements

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SP-BG-STARS | Starfield Background | Full-screen | Shader/texture | Dense star field with varying brightness, color temperature variation (white/blue/yellow/red stars) | "dense starfield background, varying brightness, blue white yellow red stars, space background" | Space travel backdrop |
| SP-BG-NEB | Distant Nebula Layer | Full-screen parallax | Shader overlay | Faint colorful nebula glow in far distance, slow drift animation | "distant nebula layer, faint colorful glow, slow drift, parallax background" | Parallax space background |
| SP-SHY-AST1 | Distant Planet (Background) | Variable size | 3D sphere + shader | Small planet visible on horizon with atmospheric glow, cloud patterns, surface features | "distant planet on horizon, atmospheric glow, cloud patterns, space background element" | Depth in space view |
| SP-RAY-SOL | Solar Ray Effect | Full-screen | Shader overlay | Bright light rays emanating from sun direction, lens flare streaks, atmospheric scattering | "solar ray effect, bright light rays, lens flare streaks, atmospheric scattering, space VFX" | Near-star navigation |

---

## 13. SPACE COMBAT SPECIFIC ASSETS [See Space Travel](./Gameplay/Space_Travel.md)

### Ship-to-Ship Combat Effects

| Asset ID | Title | Format | Description | AI Prompt Keywords | Used For |
|----------|-------|--------|-------------|-------------------|----------|
| SP-CMB-BLT1 | Projectile Trail (Ballistic) | Sprite sheet (4 frames) | Bright orange tracer line with smoke puff at end | "ballistic projectile trail, bright orange tracer, smoke puff, space combat VFX" | Ballistic weapon fire |
| SP-CMB-BLT2 | Plasma Bolt Effect | Sprite sheet (6 frames) | Glowing blue-green plasma sphere with trailing ionization | "plasma bolt effect, glowing blue green sphere, ionization trail, space combat VFX" | Plasma weapon fire |
| SP-CMB-RKT1 | Rocket Flight Trail | Sprite sheet (8 frames) | White smoke trail with orange flame exhaust, accelerating visual | "rocket flight trail, white smoke, orange flame exhaust, accelerating, space combat VFX" | Rocket weapon fire |
| SP-CMB-IMP1 | Ship Hit Flash | Sprite sheet (4 frames) | White flash followed by orange fireball and debris scatter | "ship hit flash, white flash, orange fireball, debris scatter, space combat VFX" | Enemy ship damage |
| SP-CMB-EXP2 | Ship Explosion (Large) | Sprite sheet (16 frames) | Massive fireball expanding to shockwave ring, then debris field with lingering smoke | "ship explosion large, massive fireball, shockwave ring, debris field, space combat VFX" | Enemy ship destruction |
| SP-CMB-SHD1 | Shield Hit Effect | Sprite sheet (4 frames) | Blue hexagonal grid flicker where projectile impacted, energy ripple outward | "shield hit effect, blue hexagonal grid, energy ripple, space combat VFX" | Shield absorption |

### Space Event Encounters

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SP-EVT-MIS1 | Drifting Cargo Pod | 2x2 units | 3D model | Small emergency pod with faded markings, blinking orange beacon, tether line floating | "drifting cargo pod, small emergency, faded markings, orange beacon, space asset" | Resource loot encounter |
| SP-EVT-CRW1 | Cryo-Pod Drift Cluster | 3x3 units | 3D model | Group of 4-6 cryo pods floating together, some cracked open, faint bioluminescent glow | "cryo pod cluster, floating, cracked open, bioluminescent glow, space asset" | Crew member discovery |
| SP-EVT-SGN1 | Alien Signal Beacon | 1x1 units | 3D model | Small triangular beacon with pulsing purple light, rotating slowly, alien glyphs | "alien signal beacon, triangular, pulsing purple light, rotating, alien glyphs, space asset" | Quest trigger |
| SP-EVT-RST1 | Space Ruins (Ancient) | 8x8 units | 3D model | Fragmented ancient structure with non-human architecture, crystalline elements, energy residue | "ancient space ruins, fragmented, non-human architecture, crystalline, energy residue" | Ancient alien loot |

---

## Asset Summary by Category

| Category | Total Assets | 3D Models | Sprite Sheets | Shaders/VFX | Audio |
|----------|-------------|-----------|---------------|-------------|-------|
| Buildings | 32 | 32 | - | - | - |
| Ship Modules | 35+ | 35+ | - | - | - |
| Weapons (Unit-held) | 12 | 12 | - | - | - |
| Weapons (Ship-mounted) | 12 | 12 | - | - | - |
| Units (Infantry/Vehicles/Mechs/Aerial) | 10 | 10 | - | - | - |
| Resources (Nodes + Icons) | 15 | 9 | 6 | - | - |
| Terrain Textures | 8 | - | 8 | - | - |
| Environment Props | 7 | 7 | - | - | - |
| Dungeon Props | 5 | 5 | - | - | - |
| Dropship Components | 8 | 8 | - | - | - |
| HUD Elements | 6 | - | 6 | - | - |
| UI Icons | 6 | - | 6 | - | - |
| Menu Screens | 4 | - | 4 | - | - |
| **Space Environment** | **50+** | **35+** | **-** | **15+** | **-** |
| Space Combat VFX | 7 | - | 7 | - | - |
| Animations | ~180+ frame sets | Model rotations | Sprite sequences | - | - |
| Audio | 6 | - | - | - | 6 files |
| **TOTAL** | **~430+ assets** | **210+** | **50+** | **15+** | **6** |

---

## See Also

- [Planet building specifications](./Buildings/Planet_Buildings.md)
- [Ship module catalog](./Buildings/Ship_Modules.md)
- [Weapon types and TIR progression](./Weapons/Weapon_Types.md)
- [Unit types and upgrade paths](./Units/Units.md)
- [Resource types and modifiers](./Resources/TYPES.md)
- [Dungeon system and loot](./Gameplay/Dungeons.md)
- [Dropship layout and repair sequence](./Spaceship/Dropship.md)
- [Space travel mechanics, events, minigames](./Gameplay/Space_Travel.md)
- [Solar system navigation, wormholes, planet scanning](./Planet/Navigation.md)

---

## 14. MOTHERSHIP INTERIOR ASSETS [See Dropship](./Spaceship/Dropship.md) & [Ship Modules](./Buildings/Ship_Modules.md)

### Cockpit Interior

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SHP-INT-COCK1 | Main Pilot Seat | 1x1 interior | 3D model | Ergonomic pilot chair with harness, control sticks on armrests, multi-screen instrument panel, overhead switch bank | "spaceship cockpit pilot seat, ergonomic chair, control sticks, instrument panels, sci-fi interior" | Player view during flight |
| SHP-INT-COCK2 | Co-Pilot Station | 1x1 interior | 3D model | Secondary control station with navigation screen, weapon targeting display, communication panel | "spaceship co-pilot station, secondary controls, navigation screen, sci-fi interior" | Second crew member seat |
| SHP-INT-COCK3 | Instrument Panel (Main) | Wall-mounted | 3D model + shader | Curved dashboard with glowing screens showing ship status, engine readouts, fuel levels, HP bars | "spaceship instrument panel, curved dashboard, glowing screens, sci-fi HUD display" | Ship status monitoring |
| SHP-INT-COCK4 | Weapon Targeting Screen | Wall-mounted | 3D model + shader | Tactical display with radar sweep, enemy blips, lock-on indicators, fire solution readout | "weapon targeting screen, tactical radar, enemy blips, sci-fi display" | Ship combat targeting |
| SHP-INT-COCK5 | Communication Console | Desk-mounted | 3D model | Holographic comm array with frequency dial, channel selector, recording indicator | "communication console, holographic array, frequency dial, sci-fi interior" | Ship-to-ship/planet comms |

### Interior Corridors & Access

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SHP-INT-CORR1 | Standard Corridor Segment (Straight) | 1x2 interior | 3D model | Narrow hallway with handrails, overhead lighting strips, wall panels with access hatches, grated floor, connector posts on both ends | "spaceship corridor, narrow hallway, handrails, overhead lights, grated floor, connector posts, sci-fi" | Ship interior straight connection |
| SHP-INT-CORR2 | Wide Corridor Segment (Straight) | 2x3 interior | 3D model | Main thoroughfare with wider passage, crew bulletin board, wall-mounted fire extinguisher, connector posts on both ends | "wide spaceship corridor, main passageway, bulletin board, sci-fi interior" | High-traffic ship area |
| SHP-INT-CORR-LTURN | Left-Turn Corridor Segment | 1x2 L-shape | 3D model | L-shaped corner corridor with connector posts on three ends (input, left-output, wall-on-right), handrails continue around bend, overhead lights follow curve | "spaceship left-turn corridor, L-shape corner, connector posts, handrails bend, sci-fi interior" | Directional change left |
| SHP-INT-CORR-RTURN | Right-Turn Corridor Segment | 1x2 L-shape | 3D model | L-shaped corner corridor with connector posts on three ends (input, right-output, wall-on-left), mirrored geometry of left-turn variant | "spaceship right-turn corridor, L-shape corner, connector posts, handrails bend, sci-fi interior" | Directional change right |
| SHP-INT-CORR-TJCTN | T-Junction Corridor | 1x2 + 1x1 branch | 3D model | Three-way branching junction with connector posts on three ends (T-shape), central widened area, overhead lighting grid pattern | "spaceship T-junction corridor, three-way branch, connector posts, central widening, sci-fi interior" | Branching corridor connection |
| SHP-INT-CORR-CROSS | Cross Intersection | 2x2 interior | 3D model | Four-way intersection with connector posts on all four ends, central open area, overhead lighting grid, wider passage for crew flow | "spaceship cross intersection, four-way junction, connector posts, central open area, sci-fi interior" | Main hub corridor connection |
| SHP-INT-CORR-ENDCAP | Corridor Dead-End Cap | 1x1 interior | 3D model | Terminal end-cap with single connector post on one end, sealed bulkhead wall on opposite side, handrail terminates at wall | "spaceship corridor dead-end cap, sealed bulkhead wall, single connector, sci-fi interior" | Terminal endpoint for corridors |
| SHP-INT-LADDER1 | Vertical Ladder Shaft | 1x1 vertical | 3D model | Metal ladder rungs bolted to bulkhead walls, emergency grab handles on sides, safety cage top | "vertical ladder shaft, metal rungs, bulkhead walls, safety cage, sci-fi interior" | Multi-deck ship access |
| SHP-INT-ELEV1 | Cargo Elevator Platform | 2x2 vertical shaft | 3D model | Hydraulic elevator platform with chain hoist, manual crank handle, weight limit indicator (max 500kg) | "cargo elevator, hydraulic platform, chain hoist, manual crank, sci-fi interior" | Heavy item transport between decks |
| SHP-INT-ELEV2 | Passenger Elevator | 2x2 vertical shaft | 3D model | Enclosed elevator cab with floor buttons, status display, emergency stop, handrail | "passenger elevator, enclosed cab, floor buttons, sci-fi interior" | Quick crew transport between decks |
| SHP-INT-STAIR1 | Staircase Segment (Short) | 2x1 steps | 3D model | 8-step metal staircase with anti-slip treads, dual handrails, bulkhead connections | "metal staircase, anti-slip treads, handrails, sci-fi interior" | Deck-to-deck access |

### Hangar Interior Views

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SHP-HNG-INT1 | Empty Hangar Bay | 4x4 interior | 3D model | Large open space with landing lights, maintenance cranes on ceiling tracks, tool racks on walls, oil stains on floor | "empty hangar bay, landing lights, ceiling crane, tool racks, sci-fi interior" | Vehicle storage (no vehicles) |
| SHP-HNG-INT2 | Hangar with Scout Jeep | 4x4 interior | 3D model + vehicle | Parked scout jeep with chocks on wheels, fuel hose connected to pump, mechanic workbench nearby | "hangar bay, parked scout jeep, fuel hose, mechanic bench, sci-fi interior" | Vehicle ready for deployment |
| SHP-HNG-INT3 | Hangar with Tank | 6x6 interior | 3D model + vehicle | Heavy tank occupying center space, support struts on both sides, hydraulic lift under turret, maintenance crew visible | "hangar bay, parked tank, support struts, hydraulic lift, sci-fi interior" | Heavy vehicle storage |
| SHP-HNG-INT4 | Hangar with Fighter Jet | 6x8 interior | 3D model + aircraft | Fighter jet on launch trolley, wing fold position, ordnance technician at weapons rack, safety cones around aircraft | "hangar bay, fighter jet, launch trolley, weapons rack, sci-fi interior" | Aerial vehicle storage |
| SHP-HNG-INT5 | Hangar with Mech | 6x6 interior | 3D model + mech | Upright mech on restraint legs, hydraulic service arm connected to mech torso, coolant lines visible | "hangar bay, upright mech, restraint legs, service arm, sci-fi interior" | Mech storage and maintenance |

### Interior Detail Assets

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SHP-INT-WHL1 | Ship Interior Wheel (Rotation) | 2x2 interior | 3D model | Rotating wall section with sliding door track, allows reconfiguring module layout | "rotating ship wall, sliding door track, modular interior, sci-fi" | Module shape rotation |
| SHP-INT-LIGHT1 | Overhead Panel Light | Ceiling-mounted | 3D model + shader | Rectangular light panel with adjustable brightness, status LED (green=normal, red=fault) | "overhead light panel, adjustable brightness, status LED, sci-fi interior" | Interior illumination |
| SHP-INT-VENT1 | Air Vent Grille | Wall-mounted | 3D model | Circular vent with rotating blades, airflow indicator flag, filter access door | "air vent grille, circular, rotating blades, airflow indicator, sci-fi interior" | Life support visual |
| SHP-INT-FIRE1 | Fire Extinguisher Mount | Wall-mounted | 3D model | Red cylinder in wall bracket, pressure gauge visible, pull pin secured with wire seal | "fire extinguisher mount, red cylinder, wall bracket, pressure gauge, sci-fi" | Safety equipment |
| SHP-INT-CRAT1 | Wall-Mounted Parts Rack | Wall-mounted | 3D model | Pegboard with hanging tools, small parts bins labeled by type, magnetic strip for metal bits | "wall mounted parts rack, pegboard, tool hooks, parts bins, sci-fi interior" | Storage for ship repairs |
| SHP-INT-BULK1 | Bulkhead Door (Closed) | 1x2 door | 3D model | Heavy circular metal door with wheel lock mechanism, status light ring (green=open, red=closed), pressure seal | "bulkhead door, circular metal, wheel lock, sci-fi spaceship interior" | Emergency compartment sealing |
| SHP-INT-BULK2 | Bulkhead Door (Open) | 1x2 door | 3D model | Same door rotated 90 degrees into open position, revealing passage behind | "bulkhead door open, rotated, passage visible, sci-fi interior" | Open corridor access |
| SHP-INT-SLIDE1 | Sliding Automated Door (Closed) | 1x2 door | 3D model | Standard sliding door with automated track overhead, status indicator (green=passable), transparent viewport panel | "spaceship sliding door, automated track, status indicator, viewport panel, sci-fi interior" | Standard passage (non-emergency) |
| SHP-INT-SLIDE2 | Sliding Automated Door (Open) | 1x2 door | 3D model | Same door slid open to side, revealing clear passage, track mechanism visible overhead | "spaceship sliding door open, slid to side, clear passage, sci-fi interior" | Open standard passage |
| SHP-INT-SIGN1 | Directional Signage Plaque | Wall-mounted | 3D model | Rectangular sign with arrow indicator, text slot (blank for customization), status LED, mounted at eye level on corridor wall | "spaceship directional sign, arrow indicator, text slot, LED, wall mounted, sci-fi interior" | Wayfinding in corridors |

### Modular Corridor System — Design Notes

All corridor segments (straight, turns, junctions, end-cap) share the same connector post system on each open end. This allows them to snap together seamlessly for flexible mothership interior layout construction — similar to how wall segments form planet base perimeters.

**Connector System:** Each segment has mirrored connector posts on all open ends. Closed walls have no connectors. The consistent grid alignment (1×1 base units) ensures all pieces align properly when placed adjacently.

**Faster Application:** All assets use the same visual language as planet buildings for consistency: dark gunmetal steel with subtle weathering, warm orange utility lights, realistic PBR-inspired materials, controlled edge wear. Camera angle: 2.5D isometric view at 40°.

---

## 15. PLANET SURFACE ABANDONED ASSETS [See Planet Events](./Planet/Planet_Types.md) & [Dungeons](./Gameplay/Dungeons.md)

### Abandoned Houses / Structures

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| PLT-ABN-HSE1 | Collapsed Dwelling Ruins | 2x2 footprint | 3D model | Half-collapsed dome with cracked transparent roof, furniture visible inside through opening, dust-covered | "abandoned dwelling ruins, collapsed dome, cracked roof, interior visible, dusty, sci-fi" | Early game loot (minor resources) |
| PLT-ABN-HSE2 | Intact Abandoned House | 3x3 footprint | 3D model | Sealed habitat module with faded paint, solar panels tilted at wrong angle, door slightly ajar | "abandoned house, sealed habitat, faded paint, solar panels crooked, door ajar, sci-fi" | Medium loot dungeon (2-3 rooms) |
| PLT-ABN-HSE3 | Overgrown Settlement House | 3x3 footprint | 3D model | Structure partially consumed by alien vegetation, vines through windows, roof sagging under plant weight | "overgrown house, alien vines, vegetation consuming building, sci-fi ruins" | Jungle biome abandoned structure |
| PLT-ABN-CMP1 | Abandoned Campsite | 4x4 area | 3D model + props | Tattered tents, cold fire pit with ash, scattered ration packs, broken antenna array | "abandoned campsite, tattered tents, cold fire pit, scattered gear, sci-fi" | Small loot (survival items) |
| PLT-ABN-CMP2 | Abandoned Mining Camp | 6x6 area | 3D model + props | Multiple containers, one overturned vehicle, mining equipment rusted, conveyor belt leading to deposit | "abandoned mining camp, containers, overturned vehicle, rusted equipment, sci-fi" | Medium loot (mining tools blueprint) |
| PLT-ABN-CMP3 | Abandoned Military Outpost | 8x8 area | 3D model + props | Sandbag perimeter with bullet marks, two destroyed APCs, command tent torn open, radar dish pointing wrong way | "abandoned military outpost, sandbags, bullet marks, destroyed APCs, sci-fi ruins" | Large loot (weapon blueprints) |

### Abandoned Cities / Settlements

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| PLT-ABN-CIT1 | Small Abandoned Colony | 20x20 area | 3D model group | Cluster of 5-8 buildings, main habitation dome cracked, water processing plant idle, central plaza with statue | "abandoned colony, cluster of buildings, cracked dome, idle water plant, central plaza, sci-fi" | Medium dungeon (multiple rooms) |
| PLT-ABN-CIT2 | Mining Town Ruins | 30x30 area | 3D model group | Industrial settlement with mine entrance, processing plant, worker barracks, mess hall, all rusted and empty | "abandoned mining town, industrial ruins, mine entrance, processing plant, sci-fi" | Large dungeon (factory + mine) |
| PLT-ABN-CIT3 | Research Station Complex | 25x25 area | 3D model group | Multi-building research facility with lab modules, observation dome, communication array, greenhouse wing | "abandoned research station, lab modules, observation dome, greenhouse, sci-fi ruins" | Research blueprint source |

### Abandoned Vehicles on Planet

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| PLT-ABN-VH1 | Derelict Scout Vehicle | 2x1 units | 3D model | Small ground vehicle with flat tires, open door, dashboard still faintly lit, dust-covered interior | "derelict scout vehicle, flat tires, open door, dusty interior, sci-fi" | Salvage parts (minerals) |
| PLT-ABN-VH2 | Wrecked Transport Truck | 3x1.5 units | 3D model | Long-haul truck with trailer, cargo containers spilled on ground, rear doors hanging open | "wrecked transport truck, spilled cargo, open containers, sci-fi wreckage" | Medium loot (construction material) |
| PLT-ABN-VH3 | Destroyed Tank Hull | 4x2.5 units | 3D model | Tank with blown turret, tracks broken, scorch marks on hull, nameplate still readable | "destroyed tank hull, blown turret, broken tracks, scorch marks, sci-fi" | Heavy loot (weapon blueprints) |
| PLT-ABN-VH4 | Crashed Drop Pod | 2x2 units | 3D model | Single-person crash pod with dented hull, cracked viewport, emergency cord still dangling | "crashed drop pod, dented hull, cracked viewport, emergency cord, sci-fi" | Small loot (survival items) |

### Planet Deposit Visual Variants (by biome)

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| PLT-DPT-DES-STN | Desert Stone Deposit | 2x2 surface | 3D model | Light tan rock formation with wind erosion patterns, sand accumulation at base | "desert stone deposit, tan rocks, wind erosion, sand accumulation, planet surface" | Construction material (Desert biome) |
| PLT-DPT-JNG-STN | Jungle Stone Deposit | 2x2 surface | 3D model | Moss-covered rock formation with roots wrapping around stones, green vegetation growing on surface | "jungle stone deposit, moss covered rocks, roots, green vegetation, planet surface" | Construction material (Jungle biome) |
| PLT-DPT-ICE-CRY | Ice Crystal Deposit | 2x2 surface | 3D model | Blue-white crystalline formation embedded in ice sheet, glowing from within ice | "ice crystal deposit, blue white crystals, embedded in ice, internal glow, planet surface" | Crystal minerals (Ice biome) |
| PLT-DPT-SWP-OIL | Swamp Oil Seep | 1x1 surface | 3D model | Dark viscous pool bubbling through murky water, sulfur smell visible as greenish tint, dead fish floating nearby | "swamp oil seep, dark pool, bubbling, murky water, sulfur tint, planet surface" | Oil fuel (Swamp biome) |
| PLT-DPT-RCK-MTL | Rocky Metal Deposit | 2x2 surface | 3D model | Exposed metal ore veins visible on rock faces, iron sparkle in sunlight, small cave entrance nearby | "rocky metal deposit, exposed ore veins, iron sparkle, cave entrance, planet surface" | Metal/titanium (Rocky biome) |
| PLT-DPT-DSRT-CAL | Desert Coal Deposit | 2x2 surface | 3D model | Black coal seams visible in sandstone cliffs, wind-blown black dust around base | "desert coal deposit, black seams, sandstone cliffs, black dust, planet surface" | Coal fuel (Desert biome) |

---

## 16. CHAMPION VISUAL ASSETS [See Champions](./Factions/Champions.md) & [Character Editor](./Factions/Champions.md)

### Champion Character Models (12 predefined + custom)

| Asset ID | Title | Height | Poly Count | Description | AI Prompt Keywords | Faction | Used For |
|----------|-------|--------|------------|-------------|-------------------|---------|----------|
| CHM-VIS-01 | "Voltage" Kai Nakamura | 2.6 units | 8,500 | Asian male, short spiky black hair with blue streak, neon punk jacket with LED trim, cybernetic left arm, fingerless gloves, cargo pants with tool holsters | "neon punk champion, asian male, spiky black hair blue streak, LED jacket, cybernetic arm, sci-fi character" | Neon Punk | Playable champion |
| CHM-VIS-02 | "Glitch" Alex Chen | 2.4 units | 8,000 | East Asian female, short purple bob cut, holographic visor over right eye, slim tactical suit with camouflage pattern, utility belt | "glitch champion, east asian female, purple bob hair, holographic visor, camo tactical suit, sci-fi character" | Neon Punk | Playable champion |
| CHM-VIS-03 | "Wrench" Sam Okafor | 2.7 units | 9,000 | Black male, bald head with gear tattoos on sides, thick beard, orange mechanic coveralls stained with grease, multi-tool belt, welding torch holster | "wrench champion, black male, bald, gear tattoos, orange mechanic coveralls, tool belt, sci-fi character" | Neon Punk | Playable champion |
| CHM-VIS-04 | "Ironclad" Major Torres | 2.5 units | 9,500 | Latina female, short brown hair in military cut, scar across left cheek, dark green combat armor with faction patch, heavy body armor vest, combat boots | "ironclad champion, latina female, military cut, scar, green combat armor, heavy vest, sci-fi character" | Dark Realistic | Playable champion |
| CHM-VIS-05 | "Sniper" Viktor Petrov | 2.8 units | 8,500 | Russian male, tall build, graying blond hair, cold blue eyes, gray tactical cloak over uniform, sniper scope case slung on back, fingerless shooting gloves | "sniper champion, russian male, tall, graying blond, gray tactical cloak, scope case, sci-fi character" | Dark Realistic | Playable champion |
| CHM-VIS-06 | "Demolitions" Morrison | 2.5 units | 9,000 | Caucasian male, stocky build, red buzz cut, explosive patch on shoulder, brown tactical vest full of C4 bricks, belt with fuse spools, ear protection headset | "demolitions champion, stocky, red buzz cut, c4 bricks, tactical vest, sci-fi character" | Dark Realistic | Playable champion |
| CHM-VIS-07 | "Blaze" Rico Delgado | 2.6 units | 8,500 | Hispanic male, curly dark hair, flame tattoos on both arms, orange and yellow flame-thrower suit with asbestos texture, heavy gloves, gas mask hanging at neck | "blaze champion, hispanic, curly dark hair, flame tattoos, orange flame suit, sci-fi character" | Cartoon SciFi | Playable champion |
| CHM-VIS-08 | "Zoom" Pip Tanaka | 2.2 units | 7,500 | Young East Asian female, short pink hair with blonde streak, oversized goggles on forehead, bright yellow flight suit with rocket boot mounts, backpack jetpack visible | "zoom champion, young asian female, pink hair, yellow flight suit, rocket boots, jetpack, sci-fi character" | Cartoon SciFi | Playable champion |
| CHM-VIS-09 | "Boomstick" Daisy Mayhem | 2.4 units | 8,500 | Caucasian female, long red hair in braid, freckles, purple and gold combat dress with bullet embroidery, bandolier across chest, dual pistols at hips | "boomstick champion, red hair braid, purple gold combat dress, bullet embroidery, bandolier, sci-fi character" | Cartoon SciFi | Playable champion |
| CHM-VIS-10 | "Medic" Dr. Osei | 2.5 units | 8,000 | Black female, short curly black hair, warm brown eyes, white medical coat over green uniform, med-kit backpack with red cross, stethoscope around neck | "medic champion, black female, white medical coat, red cross backpack, stethoscope, sci-fi character" | Bright Realistic | Playable champion |
| CHM-VIS-11 | "Gardener" Elias Greenfield | 2.6 units | 8,000 | Caucasian male, long brown hair in ponytail, green beard, earth-toned farmer overalls with seed pockets, gardening trowol at belt, plant-themed apron | "gardener champion, long brown hair ponytail, green overalls, seed pockets, plant apron, sci-fi character" | Bright Realistic | Playable champion |
| CHM-VIS-12 | "Architect" Morgan Lee | 2.5 units | 8,500 | Non-binary person (ambiguous), short silver hair, blue-tinted glasses, light gray architect uniform with holographic blueprint projector on wrist, measuring tape at belt | "architect champion, silver hair, blue glasses, gray uniform, holographic projector, sci-fi character" | Bright Realistic | Playable champion |

### Character Editor Appearance Options

| Asset ID | Title | Category | Description | AI Prompt Keywords | Used For |
|----------|-------|----------|-------------|-------------------|----------|
| CHM-APP-HAIR1 | Hair Style Set (Short) | Hair | 8 variations: buzz cut, crew cut, short crop, side part, slicked back, mohawk (short), undercut, fade | "sci-fi character hair short styles, 8 variations, game character customization" | Character editor |
| CHM-APP-HAIR2 | Hair Style Set (Long) | Hair | 8 variations: long straight, ponytail, braid, twin tails, loose waves, bun, half-up, flowing | "sci-fi character hair long styles, 8 variations, game character customization" | Character editor |
| CHM-APP-HAIR3 | Hair Style Set (Color) | Hair | 12 color options: black, brown, blond, red, gray, white, blue, purple, pink, green, orange, silver | "sci-fi character hair colors, 12 color swatches, game customization" | Character editor |
| CHM-APP-EYE1 | Eye Shape Set | Eyes | 6 variations: almond, round, hooded, monolid, upturned, downturned | "sci-fi character eye shapes, 6 variations, close-up portrait" | Character editor |
| CHM-APP-EYE2 | Eye Color Set | Eyes | 10 color options: brown, blue, green, gray, hazel, amber, violet, red, black, heterochromia | "sci-fi character eye colors, 10 swatches, close-up portrait" | Character editor |
| CHM-APP-SKIN1 | Skin Tone Set | Skin | 12 tones from very pale to very dark, with varied undertones (warm, cool, neutral) | "sci-fi character skin tones, 12 variations, full arm reference" | Character editor |
| CHM-APP-BODY1 | Body Type Set | Body | 6 builds: slim, athletic, muscular, stocky, tall-thin, heavy | "sci-fi character body types, 6 variations, full body reference" | Character editor |
| CHM-APP-SCAR1 | Scar Overlay Set | Markings | 8 scar types: slash, burn, bullet wound, knife cut, laser mark, shrapnel, bite, surgical | "sci-fi character scars, 8 types, face and arm reference" | Character editor |
| CHM-APP-TATTOO1 | Tattoo Set | Markings | 12 tattoo designs: gear, flame, skull, circuit, star, snake, eye, crystal, rocket, wave, geometric, faction emblem | "sci-fi character tattoos, 12 designs, arm and chest reference" | Character editor |
| CHM-APP-ACC1 | Accessory Set | Accessories | 15 options: goggles, earpiece, necklace, bracelet, ring, headband, face mask, scarf, cap, helmet visor, earring, nose ring, chin strap, jaw wire | "sci-fi character accessories, 15 items, close-up reference" | Character editor |

---

## 17. ENEMY RACE VISUAL ASSETS [See Races](./Factions/Races.md)

### Bug Race Variants (Sub-race: Swarm, Warrior, Drone, Queen)

| Asset ID | Title | Height | Poly Count | Description | AI Prompt Keywords | Race Family | Used For |
|----------|-------|--------|------------|-------------|-------------------|-------------|----------|
| ENM-BUG-SWM1 | Bug Swarm Scout | 0.8 units | 3,000 | Small insectoid creature, red-black carapace, 6 legs, glowing compound eyes, mandibles clicking | "bug swarm scout, small insect, red black carapace, compound eyes, sci-fi enemy" | Bug-Swarm | Common weak enemy |
| ENM-BUG-WAR1 | Bug Warrior Elite | 2.5 units | 8,000 | Large armored insectoid, claw arms, spiked carapace, glowing green eyes, pheromone glands pulsing | "bug warrior elite, large insectoid, claw arms, spiked armor, sci-fi enemy" | Bug-Warrior | Medium threat enemy |
| ENM-BUG-DRN1 | Bug Drone Worker | 1.5 units | 5,000 | Medium insectoid with excavation mandibles, reinforced forelegs, egg-carrying abdomen | "bug drone worker, medium insect, excavation mandibles, sci-fi enemy" | Bug-Drone | Resource collector enemy |
| ENM-BUG-QEN1 | Bug Queen Mother | 6 units tall | 20,000 | Massive insectoid queen, immobile but spawns drones, pulsating egg sacs, multiple eyes, antennae | "bug queen mother, massive insectoid, egg sacs, multiple eyes, sci-fi boss" | Bug-Queen | Boss encounter |

### Dinosaur Race Variants (Sub-race: Raptor, Tyrant, Armored, Flying)

| Asset ID | Title | Height | Poly Count | Description | AI Prompt Keywords | Race Family | Used For |
|----------|-------|--------|------------|-------------|-------------------|-------------|----------|
| ENM-DINO-RAP1 | Dinosaur Raptor Pack Unit | 2 units tall | 7,000 | Feathered theropod with glowing tribal markings, speed-enhancing implants on legs | "dino raptor, feathered theropod, tribal markings, sci-fi implant, enemy" | Dino-Raptor | Fast melee attacker |
| ENM-DINO-TYR1 | Dinosaur Tyrant Alpha | 4 units tall | 12,000 | Massive armored dinosaur with cybernetic jaw, spine-mounted spikes, battle harness | "dino tyrant alpha, massive armored dinosaur, cybernetic jaw, sci-fi boss" | Dino-Tyrant | Heavy enemy unit |
| ENM-DINO-ARM1 | Dinosaur Armored Behemoth | 3.5 units tall | 15,000 | Thick-plated herbivore with horned head, tail club, solar panel sails on back | "dino armored behemoth, thick plates, horned head, tail club, sci-fi enemy" | Dino-Armored | Tank enemy unit |
| ENM-DINO-FLY1 | Dinosaur Sky Stalker | 2 units wingspan | 6,000 | Pterosaur-like flyer with radar dome on head, diving talons, nest acid spit | "dino sky stalker, pterosaur, radar dome, diving talons, sci-fi flying enemy" | Dino-Flying | Aerial attacker |

### Human Sub-Race Variants (Thief, Punk, Raider, Soldier)

| Asset ID | Title | Height | Poly Count | Description | AI Prompt Keywords | Race Family | Used For |
|----------|-------|--------|------------|-------------|-------------------|-------------|----------|
| ENM-HUM-THF1 | Human Thief Scavenger | 2.4 units | 7,000 | Ragged brown cloak, masked face, belt with lockpicks and small knives, satchel full of loot | "human thief scavenger, ragged cloak, masked face, lockpick belt, sci-fi enemy" | Human-Thief | Stealth attacker |
| ENM-HUM-PNK1 | Human Punk Raider | 2.5 units | 8,000 | Spiked leather jacket, neon hair, plasma pipe weapon, knee pads with studs | "human punk raider, spiked leather, neon hair, plasma pipe, sci-fi enemy" | Human-Punk | Aggressive melee |
| ENM-HUM-RD1 | Human Raider Warband | 2.6 units | 9,000 | Military surplus gear, face paint, assault rifle with bayonet, chain necklace | "human raider warband, military surplus, face paint, assault rifle, sci-fi enemy" | Human-Raider | Balanced combatant |
| ENM-HUM-SLD1 | Human Soldier Patrol | 2.5 units | 8,500 | Full combat armor with helmet, laser rifle, radio pack, faction flag on shoulder | "human soldier patrol, full armor, helmet, laser rifle, sci-fi enemy" | Human-Soldier | Military-grade enemy |

### Snail Race Variants (Sub-race: Shell, Slime, Turbo)

| Asset ID | Title | Height | Poly Count | Description | AI Prompt Keywords | Race Family | Used For |
|----------|-------|--------|------------|-------------|-------------------|-------------|----------|
| ENM-SNL-SHL1 | Snail Shell Brawler | 1.5 units wide | 6,000 | Large gastropod with crystalline shell pattern, tentacle arms, acid drip from mouth | "snail shell brawler, large gastropod, crystalline shell, sci-fi enemy" | Snail-Shell | Defensive enemy |
| ENM-SNL-SLM1 | Snail Slime Trapper | 2 units wide | 7,000 | Slimy trail-depositing slug with eye stalks, corrosive spit glands, camouflage skin | "snail slime trapper, slimy slug, eye stalks, camouflage, sci-fi enemy" | Snail-Slime | Area control enemy |
| ENM-SNL-TRB1 | Snail Turbo Charger | 1 unit tall | 5,000 | Small fast snail with jet booster on shell, laser eyes, racing stripe pattern | "snail turbo charger, small fast, jet booster, laser eyes, sci-fi enemy" | Snail-Turbo | Fast hit-and-run enemy |

### Additional Race Visual Templates (30+ races total)

| Asset ID | Title | Height | Poly Count | Description | AI Prompt Keywords | Race Family | Used For |
|----------|-------|--------|------------|-------------|-------------------|-------------|----------|
| ENM-ADD-TEMPLATE1 | Generic Alien Template A | Variable | 8,000 | Bipedal alien with scaled skin, third eye on forehead, elongated head, thin limbs | "bipedal alien, scaled skin, third eye, elongated head, sci-fi enemy template" | Generic-Alien | Base for race variants |
| ENM-ADD-TEMPLATE2 | Generic Alien Template B | Variable | 8,000 | Quadrupedal alien with chitinous armor, multiple arms (4), compound vision cluster | "quadrupedal alien, chitin armor, four arms, compound eyes, sci-fi enemy template" | Generic-Alien | Base for race variants |
| ENM-ADD-TEMPLATE3 | Plant-Based Alien Template | Variable | 7,000 | Flora-animal hybrid with leafy appendages, flower head, photosynthetic glow | "plant alien, leafy appendages, flower head, glowing, sci-fi enemy template" | Plant-Alien | Flora enemies |
| ENM-ADD-TEMPLATE4 | Energy-Based Alien Template | Variable | 5,000 | Semi-transparent energy being with core glow, floating limbs, electric aura | "energy alien, translucent, core glow, floating limbs, electric aura, sci-fi enemy" | Energy-Alien | Ethereal enemies |
| ENM-ADD-TEMPLATE5 | Aquatic Alien Template | Variable | 7,500 | Fish-like humanoid with gills, webbed fingers, pressure-bladder neck ring, bioluminescent spots | "aquatic alien, fish humanoid, gills, webbed fingers, bioluminescent, sci-fi enemy" | Aquatic-Alien | Water biome enemies |

---

## 18. DROP SHIP CRASH SEQUENCE ASSETS [See Dropship](./Spaceship/Dropship.md)

### Pre-Crash (Flight Animation)

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Cinematic Use |
|----------|-------|------------|------------|-------------|-------------------|---------------|
| DS-FLY-NRM1 | Dropship Normal Flight | 6x4x2 | 3D model + animation | Clean dropship with blue engine glow, smooth flight over desert terrain, slight bank angle | "dropship normal flight, clean ship, blue engines, desert below, sci-fi cinematic" | Opening: low flight over desert |
| DS-FLY-DMG1 | Dropship Damaged Flight | 6x4x2 | 3D model + animation | Rusty dropship with smoking left engine, sparks trailing, listing to starboard, smoke plume | "dropship damaged flight, rusty ship, smoking engine, sparks, listing, sci-fi cinematic" | Opening: entering atmosphere |

### Crash Impact

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Cinematic Use |
|----------|-------|------------|------------|-------------|-------------------|---------------|
| DS-CRS-IMP1 | Impact Moment | 8x6x3 | 3D model + VFX | Dropship nose-first in dune, shockwave ring expanding, sand explosion outward, debris flying | "dropship crash impact, nose in dune, shockwave ring, sand explosion, sci-fi cinematic" | Frame-accurate crash moment |
| DS-CRS-SMO1 | Post-Crash Smoke | 8x6x3 | 3D model + VFX | Settled dropship with rising smoke from engines, dust cloud dissipating, steam vents from hull cracks | "post crash dropship, rising smoke, dust cloud, steam vents, sci-fi cinematic" | After crash settles |
| DS-CRS-DUN1 | Buried Cockpit View | 3x2x1 | 3D model + animation | Cockpit half-buried in sand, sand covering lower windows, emergency light flickering, dust motes in air | "cockpit buried in sand, sand on windows, emergency light, sci-fi cinematic" | Player exits cockpit |

### Crashed Dropship State (Start of Gameplay)

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Game Use |
|----------|-------|------------|------------|-------------|-------------------|----------|
| DS-GM-CRS1 | Full Crashed Dropship | 6x4x2 | 3D model | Complete crashed dropship, nose buried at 30-degree angle, right engine smoking, left engine intact, hull scorch marks, side door open | "crashed dropship full view, nose in sand, smoking engine, open door, sci-fi game asset" | Player starting position |
| DS-GM-CRS2 | Dropship Side View | 6x4x2 | 3D model | Profile of crashed ship showing all modules: cockpit (damaged), habitation (intact), drop module (open), storage (dented), energy core (pulsing) | "crashed dropship side view, all modules visible, sci-fi game asset" | Module identification |
| DS-GM-CRS3 | Dropship Rear View | 6x4x2 | 3D model | Back of ship showing dual engines, right engine bent nozzle, fuel line dangling, rear ramp lowered | "crashed dropship rear view, dual engines, fuel line, rear ramp, sci-fi game asset" | Engine repair quest |
| DS-GM-CRS4 | Dropship Top View | 6x4 footprint | 3D model | Overhead showing module layout: cockpit forward, habitation center-left, storage center-right, energy core aft, drop module open at side | "crashed dropship top view, module layout visible, sci-fi game asset" | Module repair planning |

---

## Asset Summary by Category

| Category | Total Assets | 3D Models | Sprite Sheets | Shaders/VFX | Audio |
|----------|-------------|-----------|---------------|-------------|-------|
| Buildings | 32 | 32 | - | - | - |
| Ship Modules | 35+ | 35+ | - | - | - |
| Weapons (Unit-held) | 12 | 12 | - | - | - |
| Weapons (Ship-mounted) | 12 | 12 | - | - | - |
| Units (Infantry/Vehicles/Mechs/Aerial) | 10 | 10 | - | - | - |
| Resources (Nodes + Icons) | 15 | 9 | 6 | - | - |
| Terrain Textures | 8 | - | 8 | - | - |
| Environment Props | 7 | 7 | - | - | - |
| Dungeon Props | 5 | 5 | - | - | - |
| Dropship Components | 8 | 8 | - | - | - |
| HUD Elements | 6 | - | 6 | - | - |
| UI Icons | 6 | - | 6 | - | - |
| Menu Screens | 4 | - | 4 | - | - |
| Space Environment | 50+ | 35+ | - | 15+ | - |
| Space Combat VFX | 7 | - | 7 | - | - |
| **Mothership Interior** | **20+** | **20+** | **-** | **-** | **-** |
| **Planet Abandoned** | **16+** | **16+** | **-** | **-** | **-** |
| **Champion Visuals** | **15+** | **15+** | **-** | **-** | **-** |
| **Enemy Race Variants** | **20+** | **20+** | **-** | **-** | **-** |
| **Dropship Crash Sequence** | **8+** | **8+** | **-** | **-** | **-** |
| Animations | ~180+ frame sets | Model rotations | Sprite sequences | - | - |
| Audio | 6 | - | - | - | 6 files |
| **TOTAL** | **~530+ assets** | **270+** | **50+** | **15+** | **6** |

---

## 19. PLANET SURFACE BIOME PROPS [See Planet Types](./Planet/Planet_Types.md) & [Planet Events](./Planet/Planet_Events.md)

### Desert Biome Props

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| PLT-BMP-DSR-CCT1 | Dead Cactus Cluster | 1.5 units tall | 3D model | Tall dried cactus with ribbed surface, thorn-covered, leaning at odd angles | "dead cactus cluster, tall dried ribbed, thorns, desert biome prop, low poly" | Desert decoration |
| PLT-BMP-DSR-RUB1 | Desert Rock Formation | 2 units tall | 3D model | Wind-sculpted rock with smooth curves and sharp edges, tan coloration | "desert wind-sculpted rock, smooth curves, tan, erosion patterns, low poly" | Desert terrain decoration |
| PLT-BMP-DSR-SRP1 | Scorpion (Wildlife) | 0.5 units wide | 3D model | Large desert scorpion with pincers, segmented tail with stinger, brown carapace | "large desert scorpion, pincers, segmented tail, stinger, brown carapace, game enemy" | Desert wildlife encounter |

### Dusty Biome Props

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| PLT-BMP-DST-DRF1 | Driftwood Cluster | 1 unit tall | 3D model | Weathered wood pieces scattered, sun-bleached gray, wind-polished surfaces | "driftwood cluster, weathered wood, sun-bleached gray, wind-polished, dusty biome" | Dusty terrain decoration |
| PLT-BMP-DST-PRT1 | Dust Devil Vortex | 3 units tall | Shader + particles | Swirling column of dust particles, brown-tan coloration, visible vortex shape | "dust devil vortex, swirling dust column, brown tan, particle effect, game VFX" | Dynamic weather event |

### Rocky Biome Props

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| PLT-BMP-RCK-CLF1 | Cliff Face Section | 5 units tall | 3D model | Exposed rock strata with visible layers, gray-brown coloration, loose scree at base | "cliff face, exposed rock strata, visible layers, gray brown, rocky biome" | Rocky terrain boundary |
| PLT-BMP-RCK-GEODE1 | Geode Cluster | 0.5 units | 3D model | Cracked open rocks revealing crystal interiors, purple/blue crystalline formations inside | "geode cluster, cracked rock, crystal interior, purple blue crystals, rocky biome" | Crystal mineral indicator |

### Water Biome Props

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| PLT-BMP-WTR-SUR1 | Water Surface Tile | Seamless | Shader overlay | Clear blue water with gentle ripple animation, sandy bottom visible through transparency | "clear blue water surface, gentle ripples, sandy bottom visible, seamless shader" | Water planet terrain |
| PLT-BMP-Wtr-CRL1 | Coral Reef Formation | 3 units wide | 3D model | Colorful coral structures in various shapes, pink/orange/green hues, fish swimming nearby | "coral reef formation, colorful coral, pink orange green, underwater biome prop" | Water biome decoration |
| PLT-BMP-WTR-PAL1 | Palm Tree (Coastal) | 5 units tall | 3D model | Tall palm tree with curved trunk, large frond canopy, coconuts visible at top | "palm tree coastal, curved trunk, large fronds, coconuts, tropical biome" | Coastal decoration |
| PLT-BMP-WTR-DRK1 | Driftwood on Shore | 2 units long | 3D model | Water-worn wood pieces with smooth surfaces, salt-bleached white-gray coloration | "driftwood shore, water-worn wood, smooth, salt-bleached, coastal biome" | Coastal decoration |

### Swamp Biome Props

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| PLT-BMP-SWP-MOS1 | Moss-Covered Log | 3 units long | 3D model | Fallen tree trunk completely covered in thick green moss, water pooling around base | "moss-covered log, fallen tree, thick green moss, swamp water, low poly" | Swamp decoration |
| PLT-BMP-SWP-TRP1 | Hanging Spanish Moss | Variable | 3D model + shader | Gray-green moss strands hanging from invisible branches above, swaying animation | "spanish moss hanging, gray green strands, swaying, swamp canopy, game prop" | Swamp atmosphere |
| PLT-BMP-SWP-FNG1 | Fungal Mushroom Cluster | 0.5 units tall | 3D model | Glowing bioluminescent mushrooms in purple/green, clustered around decaying wood | "bioluminescent fungi cluster, purple green glow, swamp mushroom, low poly" | Swamp loot indicator |
| PLT-BMP-SWP-SPR1 | Spore Cloud (Environmental) | 2 units radius | Shader + particles | Greenish-yellow toxic cloud hovering near ground, slow drift animation, visible particles | "swamp spore cloud, green yellow toxic, ground hugging, particle effect" | Toxic spore event VFX |

### Jungle Biome Props

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| PLT-BMP-JNG-TRH1 | Jungle Canopy Tree | 8 units tall | 3D model | Massive tree with wide spreading canopy, thick trunk with buttress roots, vine drapes | "jungle canopy tree, massive wide canopy, buttress roots, hanging vines, low poly" | Jungle terrain structure |
| PLT-BMP-JNG-VIN1 | Thick Vine Strand | Variable length | 3D model | Thick rope-like vine hanging from canopy, spiral texture, green-brown coloration | "thick vine strand, rope-like, spiral texture, jungle canopy, game prop" | Jungle traversal element |
| PLT-BMP-JNG-FLR1 | Jungle Floor Fern | 1 unit wide | 3D model | Large tropical fern with spreading fronds, deep green, wet appearance | "jungle floor fern, large tropical, spreading fronds, deep green, low poly" | Understory decoration |
| PLT-BMP-JNG-ANM1 | Jungle Stalker (Wildlife) | 1.5 units tall | 3D model | Camouflaged feline predator with striped pattern, glowing yellow eyes, stealth posture | "jungle stalker predator, camouflaged stripes, glowing yellow eyes, stealth, game enemy" | Jungle wildlife encounter |

### Light Snow / Ice Biome Props

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Used For |
|----------|-------|------------|------------|-------------|-------------------|----------|
| PLT-BMP-SNW-DRF1 | Snow-Covered Rock | 2 units tall | 3D model | Gray rock with white snow accumulation on top surfaces, icicles hanging from crevices | "snow covered rock, white snow top, icicles, gray rock base, low poly" | Light snow decoration |
| PLT-BMP-ICE-SHR1 | Ice Shard Formation | 3 units tall | 3D model | Sharp jagged ice spikes, translucent blue-white, light refraction visible | "ice shard formation, sharp jagged, translucent blue white, light refraction" | Ice biome decoration |
| PLT-BMP-ICE-SRG1 | Ice Shelf Edge | 5 units wide | 3D model + shader | Overhanging ice shelf with undercut curves, deep blue ice interior, snow on top surface | "ice shelf edge, overhanging, undercut curves, deep blue ice, game prop" | Ice biome boundary |
| PLT-BMP-ICE-PND1 | Penguin-like Creature (Wildlife) | 1 unit tall | 3D model | Small flightless bird with black-white plumage, orange beak, waddling animation | "small flightless bird, black white plumage, orange beak, ice biome wildlife" | Ice biome wildlife |

---

## 20. WEATHER & ENVIRONMENTAL VFX ASSETS [See Planet Events](./Planet/Planet_Events.md)

### Environmental Hazard Visual Effects

| Asset ID | Title | Format | Description | AI Prompt Keywords | Used For |
|----------|-------|--------|-------------|-------------------|----------|
| WTH-DST-STR1 | Dust Storm Vortex | Shader + particles (full-screen overlay) | Brown-tan particle field with reduced visibility, swirling wind patterns, horizon obscured | "dust storm vortex, brown tan particles, swirling wind, reduced visibility, full screen VFX" | Desert/Dusty biome hazard |
| WTH-SND-QSK1 | Sand Quicksand Pit | Shader + particles (2x2 area) | Golden sand with visible swirl pattern indicating liquid underneath, unit sinking animation overlay | "sand quicksand pit, golden swirling, liquid appearance, sinking effect, game VFX" | Desert biome hazard |
| WTH-LVA-BST1 | Lava Burst Effect | Sprite sheet (8 frames) + shader | Orange-red magma eruption from ground crack, expanding shockwave ring, molten splash particles | "lava burst eruption, orange red magma, shockwave ring, molten splash, game VFX" | Rocky/Volcanic biome hazard |
| WTH-ICE-CRK1 | Ice Crack Formation | Shader + particles (2x2 area) | Spiderweb crack pattern spreading across ice surface, blue light from below, unit falling through | "ice crack formation, spiderweb pattern, blue light from below, falling effect, game VFX" | Ice biome hazard |
| WTH-SPR-CLD1 | Toxic Spore Cloud | Shader + particles (3 units radius) | Greenish-yellow hovering cloud with visible spore particles, slow drift, bubbling ground underneath | "toxic spore cloud, green yellow, hovering, visible spores, swamp VFX" | Swamp biome hazard |
| WTH-CRY-RNS1 | Crystal Resonance Pulse | Shader + particles (2 units radius) | Expanding sonic wave ring from crystal, purple-blue distortion, visible sound wave lines | "crystal resonance pulse, expanding wave ring, purple blue distortion, sonic VFX" | Ice/Crystal biome hazard |
| WTH-FST-ACT1 | Frost Event Overlay | Shader overlay (full-screen) | Blue-white frost creeping across screen edges, ice crystal patterns on viewport, temperature drop visual | "frost event overlay, blue white creeping, ice crystals on viewport, game VFX" | Light Snow/Ice biome hazard |
| WTH-SLM-TRP1 | Slime Trail Effect | Sprite sheet (4 frames loop) + shader | Green viscous trail left by snail enemies, bubbling slowly, corrosive ground effect indicator | "slime trail effect, green viscous, bubbling, corrosive ground, game VFX" | Snail enemy visual |

---

## 21. MINIGAME UI ASSETS [See Space Travel](./Gameplay/Space_Travel.md) & [Core Loop](./Gameplay/Core_Loop.md)

### Wire Repair Minigame Visuals

| Asset ID | Title | Size | Description | AI Prompt Keywords | Used For |
|----------|-------|------|-------------|-------------------|----------|
| MNG-WIR-PNL1 | Damaged Wire Panel | 400x300px | 2D panel showing damaged circuit board with exposed wires, scorch marks, broken connectors | "damaged wire panel, circuit board, exposed wires, scorch marks, sci-fi UI" | Minigame background |
| MNG-WIR-CBL1 | Colored Wire Set (6 variants) | 64x32px each | Red, blue, green, yellow, white, black wire segments with stripped ends visible | "colored wires set, red blue green yellow white black, stripped ends, game UI icon" | Minigame wire elements |
| MNG-WIR-SLT1 | Wire Selection Highlight | Variable | Green glow outline indicating selected wire, pulsing animation frame | "wire selection highlight, green glow outline, pulsing, sci-fi UI element" | Active wire indicator |
| MNG-WIR-PRB1 | Port Socket Set (6 variants) | 64x32px each | Matching colored port sockets with pin indicators, labeled slots | "port socket set, colored slots, pin indicators, labeled, sci-fi UI" | Minigame target ports |

### Signal Decoding Minigame Visuals

| Asset ID | Title | Size | Description | AI Prompt Keywords | Used For |
|----------|-------|------|-------------|-------------------|----------|
| MNG-SGN-WVE1 | Frequency Wave Display | 500x200px | Oscilloscope-style waveform display with repeating alien signal pattern, green phosphor screen look | "frequency wave display, oscilloscope waveform, alien signal, green phosphor, sci-fi UI" | Minigame main area |
| MNG-SGN-PAT1 | Pattern Match Indicator | 64x64px | Highlighted section of waveform showing correct match, checkmark overlay | "pattern match indicator, highlighted waveform, checkmark, sci-fi UI element" | Correct pattern highlight |
| MNG-SGN-ERR1 | Error Flash Effect | 500x200px | Red X overlay with screen shake distortion frame, error buzz visual | "error flash effect, red X, screen shake distortion, sci-fi UI" | Incorrect match feedback |

### Asteroid Destruction Minigame Visuals

| Asset ID | Title | Size | Description | AI Prompt Keywords | Used For |
|----------|-------|------|-------------|-------------------|----------|
| MNG-AST-TGT1 | Asteroid Target Sprite (4 sizes) | 64x64 to 256x256px | Rock-like asteroid shapes with visible crack lines, rotating slowly, health bar overlay | "asteroid target sprite, rock shape, crack lines, rotating, game UI element" | Clickable asteroid targets |
| MNG-AST-BST1 | Laser Blast Effect (4 frames) | 128x128px each | Orange laser beam impact expanding to fireball, debris scatter particles | "laser blast effect, orange beam impact, fireball expansion, game VFX sprite" | Asteroid destruction feedback |

### Warp Tunnel Navigation Minigame Visuals

| Asset ID | Title | Size | Description | AI Prompt Keywords | Used For |
|----------|-------|------|-------------|-------------------|----------|
| MNG-WRP-TNL1 | Wormhole Tunnel View | Full-screen shader | Narrowing purple-black tunnel with starlight edge particles, speed lines indicating forward motion | "wormhole tunnel view, purple black, starlight edges, speed lines, full screen VFX" | Minigame environment |
| MNG-WRP-OBST1 | Tunnel Obstacle (3 variants) | Variable size | Rock fragments, energy pulses, and debris fields floating in tunnel path | "wormhole obstacles, rock fragments, energy pulses, debris, space VFX" | Minigame avoidance targets |

---

## 22. TECH TREE UI ASSETS [See Tech Tree](../Tech_Tree/Overview.md) & [Research Categories](../Tech_Tree/Research_Categories.md)

### Radial Tech Tree Display Elements

| Asset ID | Title | Size | Description | AI Prompt Keywords | Used For |
|----------|-------|------|-------------|-------------------|----------|
| TCH-RAD-BCK1 | Radial Tech Tree Background | 1920x1080px | Dark circular grid with concentric ring divisions, center glow point, sector lines for research categories | "radial tech tree background, dark circular grid, concentric rings, sci-fi UI" | Main tech tree canvas |
| TCH-RNG-OUTR | Outer Ring Segment (TIR 5) | Variable arc | Black-violet colored ring segment with void swirl pattern, locked state overlay | "outer ring segment, black violet, void swirl, locked state, sci-fi UI" | TIR 5 research slot background |
| TCH-RNG-INNER | Inner Ring Segment (TIR 4) | Variable arc | Gold-white colored ring segment with energy glow, unlocked state indicator | "inner ring segment, gold white, energy glow, unlocked, sci-fi UI element" | TIR 4 research slot background |
| TCH-NDE-RES1 | Research Node Icon (Available) | 64x64px | Glowing blue circle with plus symbol, pulsing animation frame, hover highlight border | "research node available, glowing blue circle, plus symbol, sci-fi UI icon" | Unlockable research display |
| TCH-NDE-RES2 | Research Node Icon (Completed) | 64x64px | Green checkmark inside circle with subtle glow, locked state overlay | "research node completed, green checkmark, glowing circle, sci-fi UI icon" | Completed research display |
| TCH-LNK-ACT1 | Active Connection Line | Variable width | Bright blue line with particle flow animation connecting unlocked nodes | "active tech connection line, bright blue, particle flow, sci-fi UI element" | Research path visualization |
| TCH-LNK-LCK1 | Locked Connection Line | Variable width | Dim gray dashed line indicating locked research prerequisite path | "locked tech connection, dim gray dashed line, sci-fi UI element" | Prerequisite path display |

---

## 23. MAP & RADAR DISPLAY ASSETS [See Navigation](./Planet/Navigation.md) & [Core Loop](./Gameplay/Core_Loop.md)

### Radar Blip Icons (64x64px each)

| Asset ID | Title | Size | Description | AI Prompt Keywords | Used For |
|----------|-------|------|-------------|-------------------|----------|
| RDR-BLP-PLR1 | Player Unit Blip | 64x64px | Green triangle pointing up, pulsing outline, visible on radar when in range | "player unit blip, green triangle, pulsing outline, radar UI icon" | Player unit display on map |
| RDR-BLP-FND1 | Friendly Building Blip | 64x64px | Blue square with building silhouette inside, static glow | "friendly building blip, blue square, building silhouette, radar UI icon" | Base structure on map |
| RDR-BLP-ENM1 | Enemy Unit Blip | 64x64px | Red diamond shape with spike, flickering animation frame | "enemy unit blip, red diamond, spike shape, flickering, radar UI icon" | Enemy detection on map |
| RDR-BLP-DNG1 | Dungeon Blip | 64x64px | Purple question mark inside circle, pulsing slowly, glow intensity varies with TIR | "dungeon blip, purple question mark, circle, pulsing, radar UI icon" | Undiscovered dungeon marker |
| RDR-BLP-RES1 | Resource Node Blip | 64x64px | Yellow exclamation point inside triangle, static indicator | "resource node blip, yellow exclamation, triangle, radar UI icon" | Discovered resource on map |
| RDR-BLP-HOS1 | Hostile Area / Boss Blip | 64x64px | Orange skull symbol with radiating lines, pulsing rapidly | "hostile area boss blip, orange skull, radiating lines, pulsing fast, radar UI icon" | Planet boss location marker |

### Map Marker Icons (128x128px each)

| Asset ID | Title | Size | Description | AI Prompt Keywords | Used For |
|----------|-------|------|-------------|-------------------|----------|
| RDR-MRK-COL1 | Colony Flag Marker | 128x128px | Faction flag on pole, planted at location, wind animation frame | "colony flag marker, faction flag on pole, wind animation, map UI icon" | Player colony indicator on galaxy map |
| RDR-MRK-WPM1 | Wormhole Marker | 128x128px | Purple swirling portal symbol with directional arrow | "wormhole marker, purple swirling portal, directional arrow, map UI icon" | Wormhole location on galaxy map |
| RDR-MRK-STN1 | Station Marker | 128x128px | Hexagonal station outline with status light (green=active, red=damaged) | "station marker, hexagonal outline, status light, map UI icon" | Space station location indicator |

---

## 24. AUDIO ASSETS — Complete Sound Library [See Core Loop](./Gameplay/Core_Loop.md)

### Music Tracks

| Asset ID | Title | Format | Duration | BPM | Description | Used For |
|----------|-------|--------|----------|-----|-------------|----------|
| AUD-BGM-MAIN | Main Menu Drone | WAV/OGG | 3:00 loop | N/A | Ambient space drone with mechanical pulses, low frequency rumble, subtle high-frequency chimes | Main menu, lobby screens |
| AUD-BGM-SURV | Survival Tension Track | WAV/OGG | 2:45 loop | 80 BPM | Slow building tension with deep percussion, rising synth strings, occasional alarm bell | Early game / low HP situations |
| AUD-BGM-COMBAT | Combat Stinger + Loop | WAV/OGG | 0:15 stinger + 2:30 loop | 140 BPM | Driving percussion, brass stabs, electric guitar distortion | Planet combat encounters |
| AUD-BGM-SPACE | Space Travel Ambient | WAV/OGG | 4:00 loop | N/A | Ethereal synth pads, distant engine hum, occasional radio static crackle | Space travel between planets |
| AUD-BGM-DNGN | Dungeon Exploration Tension | WAV/OGG | 3:30 loop | 90 BPM | Pulsing bass, intermittent metallic clicks, rising tension sweeps | Dungeon exploration |
| AUD-BGM-BOSS | Boss Fight Epic Track | WAV/OGG | 3:00 loop | 160 BPM | Full orchestra, choir chants, timpani rolls, brass fanfares | Boss encounters |
| AUD-BGM-VICT | Victory Fanfare | WAV/OGG | 0:30 one-shot | N/A | Triumphant brass melody, cymbal crash, ascending chord progression | Dungeon completion, boss defeat |

### Sound Effects — UI/HUD

| Asset ID | Title | Format | Duration | Description | Used For |
|----------|-------|--------|----------|-------------|----------|
| AUD-SFX-UISEL | Unit Selection Beep | WAV | 0.15s | Short electronic chirp, ascending tone | Selecting units/buildings |
| AUD-SFX-UIBLD | Building Placement Clang | WAV | 0.4s | Metallic impact with hydraulic hiss tail | All building construction |
| AUD-SFX-UICMB | Combat Command Acknowledge | WAV | 0.2s | Sharp digital beep followed by low rumble | Attack/defend/scout commands |
| AUD-SFX-UIERR | Error Buzz | WAV | 0.3s | Low buzzing tone with static crackle | Invalid action feedback |
| AUD-SFX-UITCH | Tech Research Complete | WAV | 0.5s | Rising synth sweep ending in bright chime | Blueprint discovery notification |

### Sound Effects — Combat

| Asset ID | Title | Format | Duration | Description | Used For |
|----------|-------|--------|----------|-------------|----------|
| AUD-SFX-BALL-FIR1 | Ballistic Weapon Fire | WAV | 0.3s | Sharp crack with metallic ring, distance reverb | Ballistic rifle/cannon fire |
| AUD-SFX-PLAS-FIR1 | Plasma Weapon Fire | WAV | 0.4s | High-pitched zap with energy discharge tail | Plasma rifle/cannon fire |
| AUD-SFX-ROKT-FIR1 | Rocket Launcher Fire | WAV | 0.5s | Whoosh launch followed by distant boom | Rocket weapon fire |
| AUD-SFX-TORP-FIR1 | Torpedo Launch | WAV | 0.8s | Deep pressurized whoosh, magnetic lock release click | Torpedo launcher fire |
| AUD-SFX-ENER-FIR1 | Energy Beam Continuous | WAV | Loop (variable) | Sustained high-frequency hum with crackling arcs | Energy beam weapon fire |
| AUD-SFX-PLAS-HIT1 | Plasma Hit Impact | WAV | 0.4s | Hissing steam explosion with material melt sound | Weapon impact on armor |
| AUD-SFX-BALL-HIT1 | Ballistic Hit Impact | WAV | 0.3s | Sharp metallic ping with debris scatter | Ballistic weapon impact |
| AUD-SFX-EXPLO-SM1 | Small Explosion | WAV | 0.8s | Quick crack-pop with high-frequency ring down | Small weapon impacts |
| AUD-SFX-EXPLO-MD1 | Medium Explosion | WAV | 1.5s | Deep boom with mid-range crack and debris rain | Vehicle destruction |
| AUD-SFX-EXPLO-LG1 | Large Explosion | WAV | 2.5s | Massive low-frequency boom, shockwave whoosh, lingering rumble | Building/ship destruction |
| AUD-SFX-BOS-HIT1 | Boss Hit Impact | WAV | 1.0s | Deep resonant thud with metallic groan and energy discharge | Boss unit damage |

### Sound Effects — Environment

| Asset ID | Title | Format | Duration | Description | Used For |
|----------|-------|--------|----------|-------------|----------|
| AUD-SFX-WND-DSRT1 | Desert Wind Ambience | WAV/OGG | 30s loop | Low howling wind with periodic sand hiss | Desert biome background |
| AUD-SFX-WND-JNGL1 | Jungle Ambient Loop | WAV/OGG | 30s loop | Distant animal calls, leaf rustle, insect buzz | Jungle biome background |
| AUD-SFX-RN-DSRT1 | Desert Footstep | WAV | 0.2s | Soft crunching sand sound, slight whoosh | Unit walking on desert |
| AUD-SFX-RN-JNGL1 | Jungle Footstep | WAV | 0.3s | Wet leaf crunch with mud squelch | Unit walking on jungle |
| AUD-SFX-RN-ICE1 | Ice Footstep | WAV | 0.25s | Crisp cracking ice with metallic ring | Unit walking on ice |
| AUD-SFX-RN-STN1 | Metal Grating Footstep | WAV | 0.15s | Hollow metallic clang with echo tail | Unit walking on metal floors |

### Sound Effects — Structures

| Asset ID | Title | Format | Duration | Description | Used For |
|----------|-------|--------|----------|-------------|----------|
| AUD-SFX-BLD-CON1 | Building Construction Start | WAV | 0.5s | Hydraulic press sound with metal bending creak | Building placement begin |
| AUD-SFX-BLD-COM1 | Building Construction Complete | WAV | 0.8s | Metallic clang sequence ending in solid thud | Building construction finish |
| AUD-SFX-BLD-DMG1 | Building Damage Hit | WAV | 0.4s | Structural groan with debris fall sound | Building taking damage |
| AUD-SFX-BLD-DEM1 | Building Demolition | WAV | 1.5s | Progressive collapse with dust cloud whoosh | Building destruction |

---

## 25. DROP SHIP INTERIOR FLIGHT VIEWS [See Dropship](./Spaceship/Dropship.md)

### Cockpit View During Flight

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Cinematic Use |
|----------|-------|------------|------------|-------------|-------------------|---------------|
| SHP-INT-COCK-FLY1 | Cockpit Forward View (Normal) | Full-screen shader + 3D | Dashboard instruments at bottom, glass canopy showing sky/terrain ahead, horizon line visible | "spaceship cockpit forward view, dashboard instruments, glass canopy, sky terrain ahead, sci-fi" | Normal flight over planet |
| SHP-INT-COCK-FLY2 | Cockpit Forward View (Atmosphere Entry) | Full-screen shader + 3D | Orange plasma glow on canopy edges, instrument flickering, horizon tilted, atmospheric distortion | "cockpit atmosphere entry, orange plasma glow, instrument flickering, sci-fi cinematic" | Re-entry sequence |
| SHP-INT-COCK-LND1 | Cockpit Landing View | Full-screen shader + 3D | Ground approaching rapidly, landing gear indicators flashing, runway lights visible below | "cockpit landing view, ground approaching, landing gear lights, sci-fi cinematic" | Landing sequence |

### Dropship Exterior Views (for module customization screen)

| Asset ID | Title | Dimensions | Model Type | Description | AI Prompt Keywords | Game Use |
|----------|-------|------------|------------|-------------|-------------------|----------|
| SHP-EXT-CUST1 | Dropship Customization View (3/4 Front) | 800x600px | Clean dropship on hangar bay floor, camera at 3/4 angle showing front and side module layout | "dropship customization view, clean ship, hangar bay, three quarter angle, sci-fi" | Module attachment preview |
| SHP-EXT-CUST2 | Dropship Customization View (Side Profile) | 800x600px | Side profile of dropship with module slot indicators (glowing outlines), upgrade path arrows | "dropship side profile, module slot indicators, glowing outlines, sci-fi UI" | Module slot visualization |

---

## Asset Summary by Category — Updated Totals

| Category | Total Assets | 3D Models | Sprite Sheets | Shaders/VFX | Audio |
|----------|-------------|-----------|---------------|-------------|-------|
| Buildings | 32 | 32 | - | - | - |
| Ship Modules | 35+ | 35+ | - | - | - |
| Weapons (Unit-held) | 12 | 12 | - | - | - |
| Weapons (Ship-mounted) | 12 | 12 | - | - | - |
| Units (Infantry/Vehicles/Mechs/Aerial) | 10 | 10 | - | - | - |
| Resources (Nodes + Icons) | 15 | 9 | 6 | - | - |
| Terrain Textures | 8 | - | 8 | - | - |
| Environment Props | 7 | 7 | - | - | - |
| Dungeon Props | 5 | 5 | - | - | - |
| Dropship Components | 8 | 8 | - | - | - |
| HUD Elements | 6 | - | 6 | - | - |
| UI Icons | 6 | - | 6 | - | - |
| Menu Screens | 4 | - | 4 | - | - |
| Space Environment | 50+ | 35+ | - | 15+ | - |
| Space Combat VFX | 7 | - | 7 | - | - |
| Mothership Interior | 20+ | 20+ | - | - | - |
| Planet Abandoned | 16+ | 16+ | - | - | - |
| Champion Visuals | 15+ | 15+ | - | - | - |
| Enemy Race Variants | 20+ | 20+ | - | - | - |
| Dropship Crash Sequence | 8+ | 8+ | - | - | - |
| **Planet Biome Props** | **24+** | **24+** | **-** | **-** | **-** |
| **Weather/Environmental VFX** | **8+** | **-** | **-** | **8+** | **-** |
| **Minigame UI Assets** | **13+** | **-** | **13+** | **-** | **-** |
| **Tech Tree UI Assets** | **7+** | **-** | **7+** | **-** | **-** |
| **Map/Radar Display Assets** | **9+** | **-** | **9+** | **-** | **-** |
| **Audio Assets** | **24+** | **-** | **-** | **-** | **24+** |
| **Dropship Interior Flight Views** | **5+** | **3+** | **-** | **2+** | **-** |
| Animations | ~180+ frame sets | Model rotations | Sprite sequences | - | - |
| **TOTAL** | **~670+ assets** | **320+** | **65+** | **30+** | **30+** |

---

## See Also

- [SHARED_UI.md](../Style/SHARED_UI.md) — Faction-agnostic UI contract (scenes, components, layout rules)
- [STYLE_MAP.md](../Style/STYLE_MAP.md) — Which faction uses which visual style
- [Stil-1/UI_THEME.md](../Style/Stil-1/UI_THEME.md) — Dark Realistic theme specification
- [Building_AssetList.md](./Building_AssetList.md) — Planet buildings asset inventory (detailed)
- [UI_AssetList.md](./UI_AssetList.md) — UI component inventory
- [Unit_AssetList.md](./Unit_AssetList.md) — Unit and champion asset inventory
- [Environment_AssetList.md](./Environment_AssetList.md) — Planet environment and biome assets

### Phase 1 — Core Space Navigation (Start Immediately)
These assets are needed first for basic space travel between planets:
| Priority | Asset ID | Title | Reason |
|----------|----------|-------|--------|
| P0 | SP-BG-STARS | Starfield Background | Required for all space views |
| P0 | SP-SUN-TYPE1 | G-Type Yellow Star | Center of most solar systems |
| P0 | SP-AST-SM1 | Small Asteroid (Rock) | Common belt decoration |
| P0 | SP-ENM-SK1 | Pirate Skiff | First space encounter |
| P1 | SP-SAT-NAV1 | Navigation Beacon Satellite | System navigation aid |
| P1 | SP-STR-DCK1 | Automated Docking Bay | Refuel/repair location |
| P1 | SP-AST-MD1 | Medium Asteroid (Ice-Rock) | Belt variety |

### Phase 2 — Mid-Game Space Exploration
These assets unlock as player reaches TIR 2-3 and explores further systems:
| Priority | Asset ID | Title | Reason |
|----------|----------|-------|--------|
| P2 | SP-STN-SM1 | Small Outpost (Inactive) | First station exploration |
| P2 | SP-ENM-FGT1 | Hostile Fighter | Mid-game space combat |
| P2 | SP-WHM-01 | Natural Wormhole Entrance | Inter-system travel visual |
| P2 | SP-DEB-MD1 | Destroyed Scout Ship | Combat loot source |
| P2 | SP-SAT-MIL1 | Military Surveillance Sat | Patrol encounters |
| P3 | SP-STN-MD1 | Medium Research Station | Research loot |
| P3 | SP-STR-OBS1 | Deep Space Observatory | Lore discovery |

### Phase 3 — Late-Game Space Systems
These assets unlock at TIR 4-5 and in the center galaxy:
| Priority | Asset ID | Title | Reason |
|----------|----------|-------|--------|
| P3 | SP-STN-LG1 | Military Outpost (Damaged) | Major combat encounter |
| P3 | SP-ENM-CRV2 | War Cruiser | Late-game boss ship |
| P3 | SP-WHM-03 | Alien Warp Portal | Alien galaxy access |
| P3 | SP-STN-XL1 | Alien Observatory Station | Alien tech source |
| P4 | SP-ENM-BSS1 | Alien Mothership | Endgame boss encounter |
| P4 | SP-STN-XL2 | Void Research Platform | TIR 5 research |
| P4 | SP-WHM-02 | Artificial Wormhole Gate | Player transit network |

### Phase 4 — Atmospheric/Environmental Polish
These assets enhance the space experience but are not critical for gameplay:
| Priority | Asset ID | Title | Reason |
|----------|----------|-------|--------|
| P4 | SP-NB-LG1 | Large Nebula Region | Visual polish |
| P4 | SP-PLSMA1 | Plasma Storm | Weather hazard |
| P4 | SP-RADZ1 | Radiation Zone | Navigation hazard |
| P4 | SP-EVT-SGN1 | Alien Signal Beacon | Quest flavor |
| P4 | SP-EVT-RST1 | Space Ruins (Ancient) | Lore flavor |
