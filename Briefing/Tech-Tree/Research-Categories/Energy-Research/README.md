# Energy Research Category

Comprehensive breakdown of all energy-related research topics with prerequisites, material costs, and cross-references. Covers power generation, battery storage, power distribution, geothermal harnessing, crystal synthesis, and zero-point extraction — from compact fusion reactors to near-infinite vacuum energy.

## Category Summary

| Total Topics | Starting | Ring 1 | Ring 2 | Ring 3 | Outer Ring |
|-------------|----------|--------|--------|--------|------------|
| 12 | 1 | 3 | 3 | 3 | 3 |

---

## Core — Starting (No Research Required)

### Compact Fusion Reactor (Starting Energy Core)

- **Time:** Auto-unlock at game start
- **Materials:** None
- **Effect:** 50 energy units total capacity, powers all dropship systems [See Dropship](../../../Spaceship/Dropship/README.md)
- **Power Distribution:** Life support (10), Systems (5), Cryo-stasis (8), Lights/HVAC (7), Reserve (20)
- **Backup Battery:** 30-minute reserve if core shuts down
- **See Also:** [Dropship](../../../Spaceship/Dropship/README.md), [Ship Modules](../../../ShipModules/README.md)

---

## Ring 1 — Basic Forge Research

### Solar Panel Installation

- **Time:** 3 minutes
- **Materials:** 80 construction material, 60 minerals
- **Prerequisites:** None (Ring 1 available at start)
- **Effect:** Enable solar array power generation on planet surface [See Planet Buildings](../../../buildings/README.md)
- **Output:** 20 energy/turn in direct sunlight (daytime or clear sky biomes)
- **Size Required:** 2x1 grid slot per panel array
- **Biome Modifier:** -50% output in Jungle (canopy cover), -30% in Dusty/Swamp (atmospheric particulates), +20% in Desert/Ice (clear skies)
- **Building Required:** Solar Array [See Planet Buildings](../../../buildings/README.md)
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Resources](../../../resources/README.md)

### Battery Storage I

- **Time:** 2 minutes
- **Materials:** 100 construction material, 80 minerals
- **Prerequisites:** None (Ring 1 available at start)
- **Effect:** +50 energy capacity on dropship (total 100 units from starting 50) [See Dropship](../../../Spaceship/Dropship/README.md)
- **Storage Type:** Chemical battery bank
- **Charge Rate:** 5 energy/turn maximum input rate
- **Discharge Rate:** Unlimited (draws as needed from storage)
- **Size Required:** 1x1 grid slot on dropship [See Ship Modules](../../../ShipModules/README.md)
- **See Also:** [Dropship](../../../Spaceship/Dropship/README.md), [Ship Modules](../../../ShipModules/README.md)

### Power Distribution I

- **Time:** 3 minutes
- **Materials:** 150 construction material, 100 minerals
- **Prerequisites:** Battery Storage I complete + Solar Panel Installation recommended
- **Effect:** Enable weapon power routing from energy core [See Weapons](../../../weapons/Weapon-Types/README.md)
- **New Capability:** Turrets and mounted weapons can now draw power from ship/core for active operation
- **Power Routing:** Automatic load balancing between life support, systems, and weapons
- **Max Weapon Power Allocation:** 20 energy/turn (sufficient for 2x Ballistic Turrets at standby)
- **See Also:** [Weapons](../../../weapons/Weapon-Types/README.md), [Ship Modules](../../../ShipModules/README.md)

---

## Ring 2 — Energy Lab Research

### Wind Turbine Optimization

- **Time:** 8 minutes
- **Materials:** 400 construction material, 300 minerals
- **Prerequisites:** Solar Panel Installation complete + Power Distribution I complete
- **Effect:** +25% output in windy biomes for wind turbine generation [See Planet Buildings](../../../buildings/README.md)
- **Base Output (optimized):** 15 energy/turn in moderate winds, 30 energy/turn in high winds
- **Biome Availability:** Desert (+40% wind), Rocky (+20% wind), Ice (+60% wind), Light Snow (+50% wind)
- **Size Required:** 1x1 grid slot per turbine unit (minimum 3-unit array for optimal output)
- **Building Required:** Wind Turbine Array [See Planet Buildings](../../../buildings/README.md)
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Resources](../../../resources/README.md)

### Battery Storage II

- **Time:** 5 minutes
- **Materials:** 300 construction material, 200 minerals
- **Prerequisites:** Battery Storage I complete + Power Distribution II started
- **Effect:** +200 energy capacity on dropship (total 300 units from starting 50) [See Dropship](../../../Spaceship/Dropship/README.md)
- **Storage Type:** Advanced lithium-ion battery bank
- **Charge Rate:** 15 energy/turn maximum input rate (+200% over Storage I)
- **Discharge Rate:** Unlimited (draws as needed from storage)
- **Size Required:** 2x1 grid slot on dropship [See Ship Modules](../../../ShipModules/README.md)
- **See Also:** [Dropship](../../../Spaceship/Dropship/README.md), [Ship Modules](../../../ShipModules/README.md)

### Power Distribution II

- **Time:** 8 minutes
- **Materials:** 500 construction material, 400 minerals
- **Prerequisites:** Power Distribution I complete + Battery Storage II installed
- **Effect:** Enable multiple simultaneous weapon systems [See Weapons](../../../weapons/Weapon-Types/README.md)
- **Max Weapon Power Allocation:** 60 energy/turn (triple Ring 1 allocation)
- **New Capability:** Can power Plasma Cannons, Rocket Bays, and Ion Weapons simultaneously
- **Load Balancing:** Automatic priority system — life support first, then systems, then weapons
- **See Also:** [Weapons](../../../weapons/Weapon-Types/README.md), [Ship Modules](../../../ShipModules/README.md)

---

## Ring 3 — Dark Matter Lab Research

### Geothermal Vent Harnessing

- **Time:** 15 minutes
- **Materials:** 800 construction material, 600 minerals
- **Prerequisites:** Solar Panel Installation + Wind Turbine Optimization complete
- **Effect:** Very high energy output from planetary geothermal vents [See Planet Buildings](../../../buildings/README.md)
- **Output:** 100 energy/turn (5x solar panel base output in direct sunlight)
- **Biome Availability:** Rocky, Desert, Light Snow (geothermal activity correlates with tectonic stress)
- **Size Required:** 3x2 grid slot per vent harnessing station
- **Building Required:** Geothermal Plant [See Planet Buildings](../../../buildings/README.md)
- **Maintenance Cost:** 10 construction material/turn for cooling system upkeep
- **Net Output:** 90 energy/turn after maintenance (still 4.5x solar in optimal conditions)
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Resources](../../../resources/README.md)

### Battery Storage III

- **Time:** 8 minutes
- **Materials:** 600 construction material, 400 minerals
- **Prerequisites:** Battery Storage II complete + Power Distribution II complete
- **Effect:** +500 energy capacity on dropship (total 800 units from starting 50) [See Dropship](../../../Spaceship/Dropship/README.md)
- **Storage Type:** Superconducting magnetic energy storage
- **Charge Rate:** 40 energy/turn maximum input rate (+700% over Storage I, +167% over Storage II)
- **Discharge Rate:** Unlimited (draws as needed from storage)
- **Size Required:** 2x2 grid slot on dropship [See Ship Modules](../../../ShipModules/README.md)
- **Self-Discharge:** Negligible (<1%/hour when idle)
- **See Also:** [Dropship](../../../Spaceship/Dropship/README.md), [Ship Modules](../../../ShipModules/README.md)

### Power Distribution III

- **Time:** 12 minutes
- **Materials:** 800 construction material, 600 minerals
- **Prerequisites:** Power Distribution II complete + Battery Storage III installed
- **Effect:** Enable Void Beam power requirements [See Weapons](../../../weapons/Weapon-Types/README.md)
- **Max Weapon Power Allocation:** 150 energy/turn (2.5x Ring 2 allocation)
- **New Capability:** Sustained Void Beam firing (requires 50 energy/turn continuously)
- **Emergency Override:** Can divert up to 200 energy/turn for 30 seconds at cost of 10% system efficiency
- **See Also:** [Weapons](../../../weapons/Weapon-Types/README.md), [Ship Modules](../../../ShipModules/README.md)

---

## Outer Ring — Void Lab Research

### Crystal Synthesizer Integration

- **Time:** 20 minutes
- **Materials:** 1,500 construction material, 800 crystal minerals
- **Prerequisites:** Geothermal Vent Harnessing + Battery Storage III complete
- **Effect:** Converts raw energy to dark matter crystals for advanced research [See Planet Buildings](../../../buildings/README.md)
- **Output Rate:** 1 dark matter crystal per 50 energy input (sustained operation)
- **Input Requirement:** Minimum 200 energy/turn available from geothermal or other high-output sources
- **Size Required:** 3x3 grid slot on planet base [See Planet Buildings](../../../buildings/README.md)
- **Building Required:** Crystal Synthesizer [See Planet Buildings](../../../buildings/README.md)
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Resources](../../../resources/README.md)

### Battery Storage V

- **Time:** 10 minutes
- **Materials:** 1,000 construction material, 800 minerals
- **Prerequisites:** Battery Storage III complete + Power Distribution III complete
- **Effect:** +1,000 energy capacity on dropship (total 1,800 units from starting 50) [See Dropship](../../../Spaceship/Dropship/README.md)
- **Storage Type:** Quantum capacitive storage array
- **Charge Rate:** 100 energy/turn maximum input rate (+1900% over Storage I)
- **Discharge Rate:** Unlimited (draws as needed from storage)
- **Size Required:** 3x2 grid slot on dropship [See Ship Modules](../../../ShipModules/README.md)
- **Self-Discharge:** Zero (quantum-stored energy is permanent until discharged)
- **See Also:** [Dropship](../../../Spaceship/Dropship/README.md), [Ship Modules](../../../ShipModules/README.md)

### Zero-Point Energy Extraction

- **Time:** 30 minutes
- **Materials:** 2,000 construction material, 1,500 dark matter crystals
- **Prerequisites:** Crystal Synthesizer Integration + Power Distribution III complete
- **Effect:** Near-infinite energy from vacuum fluctuations [See Planet Buildings](../../../buildings/README.md)
- **Output:** Unlimited energy generation (capped only by distribution capacity at 1,800 energy/turn)
- **Size Required:** 5x5 grid slot on planet base [See Planet Buildings](../../../buildings/README.md)
- **Building Required:** Zero-Point Extractor [See Planet Buildings](../../../buildings/README.md)
- **Maintenance Cost:** 10 dark matter crystals/turn to maintain quantum field stability
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Resources](../../../resources/README.md)

---

## See Also

### Related Research Categories

- [Weapons Research](../Weapons-Research/README.md) — Weapon systems requiring power distribution upgrades
- [Armor Research](../Armor-Research/README.md) — Shield generation dependent on Power Distribution tiers
- [Drives Research](../Drives-Research/README.md) — Propulsion systems with energy generation as a byproduct
- [Vision Research](../Vision-Research/README.md) — Scanning and targeting systems consuming ship energy
- [Buildings Research](../Buildings-Research/README.md) — Planet-side power grids, factories, and energy infrastructure
- [Storage Research](../Storage-Research/README.md) — Resource storage including energy reserves and dark matter crystals

### Cross-References

- [Dropship Overview](../../../Spaceship/Dropship/README.md) — Ship energy core, battery slots, and power budget
- [Ship Modules](../../../ShipModules/README.md) — Mountable energy systems and power distribution hardware
- [Planet Buildings](../../../buildings/README.md) — Solar arrays, wind turbines, geothermal plants, crystal synthesizers, zero-point extractors
- [Resources](../../../resources/README.md) — Energy as a resource type, dark matter crystals, and material costs
- [Weapons](../../../weapons/Weapon-Types/README.md) — Energy weapons and power allocation requirements
- [Tech Tree Overview](../../Overview/README.md) — Ring progression, unlock conditions, and critical paths
