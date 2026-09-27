# Drives Research Category

Comprehensive breakdown of all drive-related research topics with prerequisites, material costs, and cross-references. Covers propulsion systems, fuel processing, drive calibration, and interplanetary travel — from basic chemical rockets to alien warp integration.

## Category Summary

| Total Topics | Starting | Ring 1 | Ring 2 | Ring 3 | Outer Ring |
|-------------|----------|--------|--------|--------|------------|
| 10 | 1 | 3 | 3 | 2 | 2 |

---

## Core — Starting (No Research Required)

### Basic Chemical Rocket Drive

- **Time:** Auto-unlock at game start
- **Materials:** None
- **Effect:** Standard dropship drive, 50 kN thrust per engine [See Dropship](../../../Spaceship/Dropship/README.md)
- **Starting State:** Right drive functional (empty fuel), Left drive damaged (50% thrust)
- **Fuel Type:** Chemical propellant (basic grade)
- **Fuel Consumption:** 100 units per interplanetary hop within same solar system
- **See Also:** [Dropship](../../../Spaceship/Dropship/README.md), [Ship Modules](../../../ShipModules/README.md)

---

## Ring 1 — Basic Forge Research

### Left Drive Repair

- **Time:** 15 minutes
- **Materials:** 100 construction material, 50 minerals, 30 energy
- **Prerequisites:** None (Ring 1 available at start)
- **Effect:** Restore left rocket drive to operational status [See Dropship](../../../Spaceship/Dropship/README.md)
- **Thrust Output Restored:** 90% of original capacity (slight wear from crash damage)
- **Fuel Consumption Impact:** Both drives now require full 100 fuel units per intra-system hop
- **See Also:** [Dropship](../../../Spaceship/Dropship/README.md), [Ship Modules](../../../ShipModules/README.md)

### Fuel Processing I

- **Time:** 5 minutes
- **Materials:** 100 construction material, 80 minerals
- **Prerequisites:** None (Ring 1 available at start)
- **Effect:** Enable oil pump and basic fuel refinement on planet surface [See Resources](../../../resources/README.md)
- **Fuel Types Unlocked:** Chemical propellant (basic), Crude oil refined to rocket fuel
- **Processing Output:** 50 fuel units per cycle from crude oil, 30 fuel units per cycle from gas deposits
- **Building Required:** Oil Pump [See Planet Buildings](../../../buildings/README.md)
- **See Also:** [Resources](../../../resources/README.md), [Planet Buildings](../../../buildings/README.md)

### Drive Calibration I

- **Time:** 3 minutes
- **Materials:** 50 minerals, 20 energy
- **Prerequisites:** Left Drive Repair complete
- **Effect:** +10% drive efficiency for both rocket drives (stacks additively with later calibrations)
- **Fuel Savings:** Reduces fuel consumption from 100 to 90 units per intra-system hop
- **Thrust Bonus:** +5% thrust output per drive (+10 kN total)
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Dropship](../../../Spaceship/Dropship/README.md)

---

## Ring 2 — Energy Lab Research

### Atomic Reactor Drive

- **Time:** 15 minutes
- **Materials:** 800 construction material, 600 minerals
- **Prerequisites:** Left Drive Repair complete + Fuel Processing I complete
- **Effect:** -40% fuel consumption, 1.5 hops per tank [See Ship Modules](../../../ShipModules/README.md)
- **Fuel Consumption:** 60 units per intra-system hop (was 100 with Calibration I: 81 units)
- **Thrust Output:** 75 kN per drive (+50% over chemical drives)
- **Size Required:** 2x1 grid slot per drive (replaces existing 1x1 engines)
- **Energy Generation:** Produces 30 energy/turn as byproduct (powers ship systems)
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Dropship](../../../Spaceship/Dropship/README.md)

### Fuel Processing II

- **Time:** 8 minutes
- **Materials:** 400 construction material, 300 minerals
- **Prerequisites:** Fuel Processing I complete + Advanced fuel deposits discovered [See Resources](../../../resources/README.md)
- **Effect:** Enable advanced fuel types — biofuel and frozen methane [See Resources](../../../resources/README.md)
- **Biofuel Output:** 80 fuel units per cycle (60% more efficient than crude oil refinement)
- **Frozen Methane Output:** 120 fuel units per cycle (140% more efficient than crude oil)
- **Building Required:** Fuel Processor [See Planet Buildings](../../../buildings/README.md)
- **Biome Availability:** Frozen methane only in Ice and Light Snow biomes, biofuel from organic matter in Jungle/Water/Swamp
- **See Also:** [Resources](../../../resources/README.md), [Planet Buildings](../../../buildings/README.md)

### Drive Calibration II

- **Time:** 5 minutes
- **Materials:** 200 minerals, 100 energy
- **Prerequisites:** Drive Calibration I complete + Atomic Reactor Drive installed
- **Effect:** +20% drive efficiency (stacks with Calibration I bonus for total +30%)
- **Fuel Savings:** Reduces atomic drive consumption from 60 to 42 units per intra-system hop
- **Thrust Bonus:** Additional +10% thrust output (+15 kN per drive)
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Dropship](../../../Spaceship/Dropship/README.md)

---

## Ring 3 — Dark Matter Lab Research

### Ionic Reactor Drive

- **Time:** 20 minutes
- **Materials:** 1,200 construction material, 800 minerals
- **Prerequisites:** Atomic Reactor Drive + Fuel Processing II complete
- **Effect:** -60% fuel consumption from base rate, fast speed [See Ship Modules](../../../ShipModules/README.md)
- **Fuel Consumption:** 40 units per intra-system hop (base 100 -> 40 with Calibration I+II: 32 units)
- **Thrust Output:** 100 kN per drive (+100% over chemical drives, +33% over atomic)
- **Size Required:** 2x1 grid slot per drive (replaces existing engines)
- **Energy Generation:** Produces 50 energy/turn as byproduct (+67% over atomic reactor)
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Dropship](../../../Spaceship/Dropship/README.md)

### Fuel Processing III

- **Time:** 12 minutes
- **Materials:** 600 construction material, 500 minerals
- **Prerequisites:** Fuel Processing II complete + Dark Matter Crystals available [See Resources](../../../resources/README.md)
- **Effect:** Dark matter fuel efficiency bonus of +25% to all processed fuels [See Resources](../../../resources/README.md)
- **Biofuel Output with Bonus:** 100 fuel units per cycle (100% more than crude oil base)
- **Frozen Methane Output with Bonus:** 150 fuel units per cycle (200% more than crude oil base)
- **Dark Matter Fuel Unlocked:** 200 fuel units per cycle (300% more than crude oil base)
- **See Also:** [Resources](../../../resources/README.md), [Planet Buildings](../../../buildings/README.md)

### Drive Calibration III

- **Time:** 8 minutes
- **Materials:** 400 minerals, 300 energy
- **Prerequisites:** Drive Calibration II complete + Ionic Reactor Drive installed
- **Effect:** +30% total drive efficiency bonus (stacked: I=+10%, II=+20%, III=+30% = +60% total)
- **Fuel Savings:** Reduces ionic drive consumption from 40 to 16 units per intra-system hop
- **Thrust Bonus:** Additional +15% thrust output (+30 kN per drive, total 130 kN each)
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Dropship](../../../Spaceship/Dropship/README.md)

---

## Outer Ring — Void Lab Research

### Energy Stream Drive

- **Time:** 25 minutes
- **Materials:** 1,800 construction material, 1,200 minerals
- **Prerequisites:** Ionic Reactor Drive + Drive Calibration III complete
- **Effect:** -80% fuel consumption from base rate, very fast speed [See Ship Modules](../../../ShipModules/README.md)
- **Fuel Consumption:** 20 units per intra-system hop (with all calibrations: 8 units)
- **Thrust Output:** 150 kN per drive (+200% over chemical drives, +50% over ionics)
- **Size Required:** 4x2 grid slot per drive (larger but more efficient)
- **Energy Generation:** Produces 80 energy/turn as byproduct (+60% over ionic reactors)
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Dropship](../../../Spaceship/Dropship/README.md)

### Void Warp Drive

- **Time:** 30 minutes
- **Materials:** 2,500 construction material, 2,000 dark matter crystals
- **Prerequisites:** Energy Stream Drive + Fuel Processing III complete
- **Effect:** Instant travel within 5-system radius [See Ship Modules](../../../ShipModules/README.md)
- **Warp Range:** Up to 5 solar systems away (must have been previously discovered and scanned)
- **Fuel Consumption:** 10 units per warp (+50% savings over Energy Stream with calibrations)
- **Size Required:** 4x4 grid slot per drive
- **Energy Cost:** 100 energy to initiate warp sequence, then free transit
- **Warp Time:** 10 seconds opening wormhole + 5 seconds transit through it [See Navigation](../../../Planets/Navigation/README.md)
- **Cooldown:** 60 seconds between warps (system recharge)
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Navigation](../../../Planets/Navigation/README.md)

### Alien Warp Integration

- **Time:** 40 minutes
- **Materials:** 4,000 construction material, 3,000 dark matter crystals
- **Prerequisites:** Void Warp Drive + Center Galaxy reward (Alien tech)
- **Effect:** Unlimited range wormhole transit with essentially free fuel consumption [See Navigation](../../../Planets/Navigation/README.md)
- **Warp Range:** Any discovered solar system (no distance limit)
- **Fuel Consumption:** 0.01 units per warp (effectively free)
- **Size Required:** Replaces existing Void Warp Drive (4x4 grid slot)
- **Energy Cost:** 50 energy to initiate warp sequence (half of standard void warp)
- **Warp Time:** 8 seconds opening wormhole + 3 seconds transit through it (-20% faster than standard void warp)
- **Cooldown:** 30 seconds between warps (half of standard void warp cooldown)
- **Special Feature:** Wormholes connect directly to previously visited systems only (no random jumps)
- **See Also:** [Navigation](../../../Planets/Navigation/README.md), [Ship Modules](../../../ShipModules/README.md)

---

## See Also

### Related Research Categories

- [Weapons Research](../Weapons-Research/README.md) — Weapon systems powered by drive energy output
- [Energy Research](../Energy-Research/README.md) — Power generation and battery storage for ship drives
- [Armor Research](../Armor-Research/README.md) — Hull integrity and shield generation for the dropship
- [Vision Research](../Vision-Research/README.md) — Navigation sensors and scanning systems for interplanetary travel
- [Buildings Research](../Buildings-Research/README.md) — Planet-side infrastructure including fuel processing facilities
- [Storage Research](../Storage-Research/README.md) — Resource storage including fuel reserves

### Cross-References

- [Dropship Overview](../../../Spaceship/Dropship/README.md) — Ship hull, engines, and starting state details
- [Ship Modules](../../../ShipModules/README.md) — Mountable drive systems and engine specifications
- [Planet Buildings](../../../buildings/README.md) — Fuel processors, oil pumps, and power plants
- [Resources](../../../resources/README.md) — Fuel types, dark matter crystals, and material costs
- [Navigation](../../../Planets/Navigation/README.md) — Interplanetary travel mechanics and warp system rules
- [Tech Tree Overview](../../Overview/README.md) — Ring progression, unlock conditions, and critical paths
