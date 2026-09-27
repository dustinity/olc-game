# Buildings Research Category

Detailed unlock paths, requirements, and dependencies for all Buildings research topics in the tech tree.

---

## Category Summary

| Ring | Lab/Facility | Topics |
|------|-------------|--------|
| Core | Starting | 1 |
| Ring 1 | Basic Forge | 2 |
| Ring 2 | Energy Lab | 3 |
| Ring 3 | Dark Matter Lab | 3 |
| Outer Ring | Void Lab | 3 |

---

## Core — Starting (No Research Required)

### Basic Wall and Gate Placement
- **Time:** Auto-unlock at game start
- **Materials:** 80 construction material, 50 minerals per wall segment
- **Effect:** Defensive structure building on planet surface [See Planet Buildings](../../../buildings/README.md)
- **Wall Stats:** 200 HP, 2m height, blocks infantry and light vehicles
- **Gate Stats:** 100 HP, opens/closes via control panel or proximity sensor
- **Placement Range:** Within 50m of existing structure or dropship [See Dropship](../../../Spaceship/Dropship/README.md)
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Storage System](../../../buildings/Reference/Storage-System.md)

---

## Ring 1 — Basic Forge Research

### Mine Placement
- **Time:** 3 minutes
- **Materials:** 100 construction material, 80 minerals
- **Prerequisites:** None (Ring 1 available at start)
- **Effect:** Enable mine placement on any discovered resource deposit type [See Planet Buildings](../../../buildings/README.md)
- **Mine Types:** Standard mineral mine, Oil pump (requires Fuel Processing I), Crystal extractor (requires crystal deposits)
- **Output per Mine:** 10 resources/turn (scales with biome modifier and research level) [See Resources](../../../resources/README.md)
- **Size Required:** 2x2 grid slot per mine [See Planet Buildings](../../../buildings/README.md)
- **Worker Requirement:** 2 workers per active mine (assigned from habitation capacity)
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Resources](../../../resources/README.md)

### Basic Power Grid
- **Time:** 3 minutes
- **Materials:** 150 construction material, 100 minerals
- **Prerequisites:** Solar Panel Installation + Power Distribution I complete
- **Effect:** Connect buildings to centralized power source on planet surface [See Planet Buildings](../../../buildings/README.md)
- **Grid Range:** 200m radius from power generation source
- **Power Distribution:** Automatic load balancing across all connected buildings
- **Building Requirement:** All energy-consuming buildings must be within grid range to operate
- **Out-of-Grid Effect:** Buildings outside grid range operate at 50% efficiency or not at all (depending on building)
- **Size Required:** Visual power line connections between buildings [See Planet Buildings](../../../buildings/README.md)
- **Energy Cost:** 5% transmission loss over distance (1% per 50m from source)
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Resources](../../../resources/README.md)

---

## Ring 2 — Energy Lab Research

### Factory Construction
- **Time:** 15 minutes
- **Materials:** 800 construction material, 500 minerals, 200 energy
- **Prerequisites:** Basic Power Grid + Mine Placement complete
- **Effect:** Vehicle and module production on planet surface [See Planet Buildings](../../../buildings/README.md)
- **Production Capabilities:** Light vehicles (tanks, walkers), ship modules [See Ship Modules](../../../ShipModules/README.md), Unit upgrades [See Upgrade System](../../../units/Upgrade-System/README.md)
- **Size Required:** 4x4 grid slot per factory [See Planet Buildings](../../../buildings/README.md)
- **Worker Requirement:** 8 workers minimum for basic operation, 16 for full production capacity
- **Energy Cost:** 50 energy/turn while operational
- **Output Speed:** Standard production rate (1 unit per 10 minutes for light vehicles)
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Ship Modules](../../../ShipModules/README.md)

### Forge Upgrade Path
- **Time:** 10 minutes
- **Materials:** 600 construction material, 400 minerals
- **Prerequisites:** Factory Construction + Basic Mine operational for 30 minutes
- **Effect:** Enable Energy Lab construction from basic forge [See Planet Buildings](../../../buildings/README.md)
- **Upgrade Path:** Basic Forge → Energy Lab (TIR 3 research facility)
- **Energy Lab Capabilities:** Weapons II, Armor Advanced, Drive Advanced, and all Ring 2 research topics [See Overview](../../Overview/README.md)
- **Size Required:** Energy Lab requires 3x3 grid slot [See Planet Buildings](../../../buildings/README.md)
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Tech Tree Overview](../../Overview/README.md)

### Advanced Power Grid
- **Time:** 8 minutes
- **Materials:** 500 construction material, 400 minerals
- **Prerequisites:** Basic Power Grid + Factory Construction complete
- **Effect:** Automated power routing and load balancing across all planet bases [See Planet Buildings](../../../buildings/README.md)
- **Grid Range:** Expanded to 500m radius from power source (+150% over basic grid)
- **Transmission Loss:** Reduced to 2% total (from 5% for basic grid)
- **Automated Routing:** Automatically reroutes power if primary line is damaged or destroyed
- **Multi-Base Support:** Can connect multiple planet bases into single network via Quantum Gate [See Storage System](../../../buildings/Reference/Storage-System.md)
- **Size Required:** Visual power line connections between buildings with automated switching nodes
- **Energy Cost:** 10 energy/turn for grid maintenance (flat rate regardless of size)
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Storage System](../../../buildings/Reference/Storage-System.md)

---

## Ring 3 — Dark Matter Lab Research

### Void Lab Construction
- **Time:** 20 minutes
- **Materials:** 2,000 construction material, 1,500 minerals, 500 energy
- **Prerequisites:** Forge Upgrade Path + Factory Construction complete
- **Effect:** TIR 5 research facility for Outer Ring technology [See Planet Buildings](../../../buildings/README.md)
- **Research Topics Unlocked:** All Outer Ring weapons, armor, drives, energy, vision, storage, buildings, and units [See Overview](../../Overview/README.md)
- **Size Required:** 5x5 grid slot per Void Lab [See Planet Buildings](../../../buildings/README.md)
- **Worker Requirement:** 4 workers minimum for basic operation, 12 for full research capacity
- **Energy Cost:** 100 energy/turn while operational
- **Research Speed Bonus:** +100% all research conducted at this facility [See Overview](../../Overview/README.md)
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Tech Tree Overview](../../Overview/README.md)

### Assembly Plant
- **Time:** 25 minutes
- **Materials:** 3,000 construction material, 2,000 minerals, 1,000 energy
- **Prerequisites:** Factory Construction + Advanced Power Grid complete
- **Effect:** 3x factory production rate for all manufactured items [See Planet Buildings](../../../buildings/README.md)
- **Production Speed:** Light vehicles in ~3.3 minutes (vs 10 min standard), modules in proportionally reduced time
- **Size Required:** 6x6 grid slot per assembly plant [See Planet Buildings](../../../buildings/README.md)
- **Worker Requirement:** 16 workers minimum for basic operation, 32 for full production capacity
- **Energy Cost:** 150 energy/turn while operational (+100% over standard factory)
- **Output Quality:** +10% durability on all manufactured items (permanent bonus to produced vehicles and modules)
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Ship Modules](../../../ShipModules/README.md)

### Orbital Strike Beacon
- **Time:** 20 minutes
- **Materials:** 2,000 construction material, 1,500 minerals, 800 energy
- **Prerequisites:** Assembly Plant + Satellite Deployment Bay (Vision category) complete
- **Effect:** Planet-wide bombardment capability from orbit [See Ship Modules](../../../ShipModules/README.md)
- **Strike Types:** Artillery barrage (1km radius), Precision laser strike (50m radius), EMP pulse (2km radius, disables all electronics for 60 seconds)
- **Cooldown per Strike Type:** Artillery — 5 minutes, Laser — 10 minutes, EMP — 30 minutes
- **Size Required:** 3x3 grid slot per beacon [See Planet Buildings](../../../buildings/README.md)
- **Energy Cost:** 200 energy per strike (varies by type: Artillery 150, Laser 250, EMP 300)
- **Satellite Requirement:** Minimum 1 satellite in orbit for targeting lock [See Vision](../Vision-Research/README.md#ring-3-dark-matter-lab-research)
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Planet Buildings](../../../buildings/README.md)

---

## Outer Ring — Void Lab Research

### Alien Artifact Decoder
- **Time:** 30 minutes
- **Materials:** 3,000 construction material, 2,000 dark matter crystals
- **Prerequisites:** Void Lab Construction + Orbital Strike Beacon complete + Center Galaxy reward (Alien tech)
- **Effect:** Decode center galaxy ruins for unique blueprints [See Planet Buildings](../../../buildings/README.md)
- **Blueprints Unlocked:** Random selection from Outer Ring exclusive pool plus 2-3 unique Alien-exclusive technologies
- **Decoding Speed:** 1 blueprint per 10 minutes of continuous operation
- **Size Required:** 4x4 grid slot decoder station [See Planet Buildings](../../../buildings/README.md)
- **Energy Cost:** 75 energy/turn while decoding
- **Location:** Must be placed within 500m of center galaxy ruin structure to function
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Tech Tree Overview](../../Overview/README.md)

### Quantum Gate Network
- **Time:** 40 minutes
- **Materials:** 8,000 construction material, 6,000 minerals, 4,000 energy
- **Prerequisites:** Orbital Strike Beacon + Remote Transfer Network (Storage category) complete
- **Effect:** Instant unit transport between linked gates [See Planet Buildings](../../../buildings/README.md)
- **Transport Range:** Unlimited (any two linked gates regardless of distance or planet)
- **Capacity per Transport:** Up to 10 units or 1 light vehicle per transit cycle
- **Transit Time:** 3 seconds from gate departure to gate arrival
- **Size Required:** 4x4 grid slot per gate emitter + 4x4 grid slot per gate receiver [See Planet Buildings](../../../buildings/README.md)
- **Energy Cost:** 200 energy/turn per active gate pair
- **Cooldown:** 60 seconds between transports through same gate pair
- **Building Required:** Quantum Gate Emitter + Receiver [See Planet Buildings](../../../buildings/README.md)
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Storage System](../../../buildings/Reference/Storage-System.md)

### Dyson Swarm Component (Endgame)
- **Time:** 60 minutes
- **Materials:** 10,000 construction material, 8,000 dark matter crystals
- **Prerequisites:** Alien Artifact Decoder + Zero-Point Energy Extraction (Energy category) complete
- **Effect:** Capture star energy for permanent +50% all research speed [See Planet Buildings](../../../buildings/README.md)
- **Research Speed Bonus:** +50% to ALL research across ALL facilities permanently (forge, energy lab, dark matter lab, void lab)
- **Star Required:** Must be placed in solar system with active star (not all planets have visible stars)
- **Size Required:** Multiple structures orbiting the star — represented as 1 grid slot on dropship [See Ship Modules](../../../ShipModules/README.md)
- **Energy Cost:** None (Dyson swarm generates 500 energy/turn surplus that feeds into local power grid)
- **Building Required:** Dyson Swarm Collector Array (orbiting, not planet-based)
- **Activation Time:** 60 minutes of continuous operation to achieve stable energy capture
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Ship Modules](../../../ShipModules/README.md)

---

## See Also

### Related Research Categories
- [Weapons Research](../Weapons-Research/README.md) — Weapon systems mountable on buildings and turrets
- [Energy Research](../Energy-Research/README.md) — Power generation and distribution for building grids
- [Storage Research](../Storage-Research/README.md) — Storage facilities and resource transfer networks
- [Vision Research](../Vision-Research/README.md) — Satellite deployment required for Orbital Strike Beacons
- [Units Research](../Units-Research/README.md) — Units produced by Factory, Assembly Plant, and Airfield buildings
- [Drives Research](../Drives-Research/README.md) — Fuel processing tied to mine and oil pump placement

### Related Documentation
- [Planet Buildings](../../../buildings/README.md) — Building definitions, grid sizes, and biome compatibility
- [Ship Modules](../../../ShipModules/README.md) — Ship-mounted structures produced by factories
- [Storage System](../../../buildings/Reference/Storage-System.md) — Storage buildings and quantum gate integration
- [Resources](../../../resources/README.md) — Resource types extracted and consumed by buildings
- [Dropship](../../../Spaceship/Dropship/README.md) — Starting platform and placement anchor for base structures

### Tech Tree Navigation
- [Tech Tree Overview](../../Overview/README.md) — Ring progression and unlock conditions
- [Research Categories Index](../README.md) — All categories master index
