# Storage Research Category

Detailed unlock paths, requirements, and dependencies for all Storage research topics in the tech tree.

---

## Category Summary

| Ring | Lab/Facility | Topics |
|------|-------------|--------|
| Core | Starting | 1 |
| Ring 1 | Basic Forge | 3 |
| Ring 2 | Energy Lab | 3 |
| Ring 3 | Dark Matter Lab | 3 |
| Outer Ring | Void Lab | 3 |

---

## Core — Starting (No Research Required)

### Basic Dropship Storage Locker
- **Time:** Auto-unlock at game start
- **Materials:** None
- **Effect:** 200 resource capacity per type [See Storage System](../../../buildings/Reference/Storage-System.md)
- **Starting Contents:** ~40% full with scattered supplies [See Dropship](../../../Spaceship/Dropship/README.md)
- **Size Required:** 1x1 grid slot on dropship [See Ship Modules](../../../ShipModules/README.md)
- **Organization:** Manual item placement (no auto-sorting)
- **See Also:** [Storage System](../../../buildings/Reference/Storage-System.md), [Dropship](../../../Spaceship/Dropship/README.md)

---

## Ring 1 — Basic Forge Research

### Container Placement
- **Time:** 2 minutes
- **Materials:** 300 construction material, 100 minerals
- **Prerequisites:** None (Ring 1 available at start)
- **Effect:** Enable wheel-shaped container storage with rotation flexibility [See Storage System](../../../buildings/Reference/Storage-System.md)
- **Capacity:** 2,000 resources per type (10x starting locker)
- **Rotation Feature:** Can be placed as 2x2 or 4x1 using wheel mechanic for shape flexibility
- **Size Required:** 2x2 or 4x1 grid slot on planet base [See Planet Buildings](../../../buildings/README.md)
- **Building Required:** Container [See Storage System](../../../buildings/Reference/Storage-System.md)
- **See Also:** [Storage System](../../../buildings/Reference/Storage-System.md), [Planet Buildings](../../../buildings/README.md)

### Storage Organization I
- **Time:** 1 minute
- **Materials:** None (knowledge only)
- **Prerequisites:** Container Placement complete
- **Effect:** UI improvements for inventory management — auto-sort buttons, visual category icons
- **New Features:** One-click sort by resource type, one-click move to nearest container
- **Search Function:** Text-based search for specific items and blueprints
- **See Also:** [Storage System](../../../buildings/Reference/Storage-System.md)

### Resource Labeling
- **Time:** 1 minute
- **Materials:** None (auto-unlock with Storage Organization I)
- **Effect:** Automatic resource type categorization in storage UI
- **Visual Display:** Color-coded labels for each resource category [See Resources](../../../resources/README.md)
- **Stack Sorting:** Automatically groups identical resources together across all containers
- **See Also:** [Storage System](../../../buildings/Reference/Storage-System.md), [Resources](../../../resources/README.md)

---

## Ring 2 — Energy Lab Research

### Haul Storage Construction
- **Time:** 10 minutes
- **Materials:** 1,500 construction material, 500 minerals
- **Prerequisites:** Container Placement + Storage Organization I complete
- **Effect:** 10,000 resources per type capacity [See Storage System](../../../buildings/Reference/Storage-System.md)
- **Capacity Increase:** 5x container capacity (10,000 vs 2,000)
- **Size Required:** 4x4 grid slot on planet base [See Planet Buildings](../../../buildings/README.md)
- **Building Required:** Haul Storage [See Storage System](../../../buildings/Reference/Storage-System.md)
- **Special Feature:** Crash protection — contents survive base destruction (50% chance to recover after attack)
- **See Also:** [Storage System](../../../buildings/Reference/Storage-System.md), [Planet Buildings](../../../buildings/README.md)

### Storage Organization II
- **Time:** 5 minutes
- **Materials:** None (knowledge only)
- **Prerequisites:** Storage Organization I complete + Haul Storage Construction started
- **Effect:** Advanced filtering and search in storage UI
- **New Features:** Multi-resource selection, drag-and-drop bulk transfers, custom category creation
- **Cross-Storage Search:** View contents of all containers simultaneously from single interface
- **See Also:** [Storage System](../../../buildings/Reference/Storage-System.md)

### Resource Conversion I
- **Time:** 8 minutes
- **Materials:** 800 construction material, 600 minerals
- **Prerequisites:** Storage Organization II complete + Basic Mine operational for 15 minutes
- **Effect:** Convert between resource types at 70% yield [See Planet Buildings](../../../buildings/README.md)
- **Conversion Examples:**
  - Construction Material → Minerals: 100 → 70 minerals
  - Minerals → Construction Material: 100 → 70 construction material
  - Fuel → Energy: 100 fuel → 70 energy (emergency conversion)
- **Building Required:** Resource Converter [See Planet Buildings](../../../buildings/README.md)
- **Energy Cost:** 50 energy per conversion cycle
- **Time per Cycle:** 3 minutes per conversion batch
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Resources](../../../resources/README.md)

---

## Ring 3 — Dark Matter Lab Research

### Reinforced Vault
- **Time:** 15 minutes
- **Materials:** 3,000 construction material, 2,000 minerals
- **Prerequisites:** Haul Storage Construction + Resource Conversion I complete
- **Effect:** 25,000 resources per type with crash protection [See Storage System](../../../buildings/Reference/Storage-System.md)
- **Capacity Increase:** 2.5x haul storage capacity (25,000 vs 10,000)
- **Crash Protection:** 100% content retention after base destruction or planetary events
- **Size Required:** 4x4 grid slot on planet base [See Planet Buildings](../../../buildings/README.md)
- **Building Required:** Reinforced Vault [See Storage System](../../../buildings/Reference/Storage-System.md)
- **Special Feature:** Fireproof, floodproof, and explosion-proof storage environment
- **See Also:** [Storage System](../../../buildings/Reference/Storage-System.md), [Planet Buildings](../../../buildings/README.md)

### Resource Conversion II
- **Time:** 10 minutes
- **Materials:** 800 construction material, 600 minerals
- **Prerequisites:** Resource Conversion I complete + Haul Storage Construction complete
- **Effect:** Improved conversion at 75% yield (up from 70%) [See Planet Buildings](../../../buildings/README.md)
- **Conversion Speed:** -25% cycle time (3 minutes → 2.25 minutes per batch)
- **New Conversions Unlocked:** Dark matter crystals ↔ any standard resource at 60% yield
- **Building Required:** Resource Converter [See Planet Buildings](../../../buildings/README.md)
- **Energy Cost:** 75 energy per conversion cycle (+50% over Conversion I, but higher yield justifies cost)
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Resources](../../../resources/README.md)

### Remote Transfer Network
- **Time:** 12 minutes
- **Materials:** 1,000 construction material, 800 minerals
- **Prerequisites:** Reinforced Vault + Resource Conversion II complete
- **Effect:** Quantum Gate resource sharing between bases [See Planet Buildings](../../../buildings/README.md)
- **Transfer Range:** Unlimited (any two linked bases regardless of distance)
- **Transfer Speed:** Instant material movement through quantum tunnel
- **Capacity per Transfer:** Up to 1,000 resources per type per transfer cycle
- **Building Required:** Quantum Gate Emitter + Receiver [See Planet Buildings](../../../buildings/README.md)
- **Energy Cost:** 100 energy per gate pair (one-time setup), 10 energy/turn per active transfer
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Storage System](../../../buildings/Reference/Storage-System.md)

---

## Outer Ring — Void Lab Research

### Black Hole Storage
- **Time:** 30 minutes
- **Materials:** 5,000 construction material, 3,000 dark matter crystals
- **Prerequisites:** Reinforced Vault + Resource Conversion II complete
- **Effect:** Effectively unlimited storage while powered [See Storage System](../../../buildings/Reference/Storage-System.md)
- **Capacity:** Unlimited (theoretical limit of 9.2 × 10^61 units per Planck volume)
- **Power Requirement:** Must be connected to Energy Core with minimum 500 energy output [See Dropship](../../../Spaceship/Dropship/README.md)
- **Size Required:** 3x3 grid slot on planet base [See Planet Buildings](../../../buildings/README.md)
- **Building Required:** Black Hole Storage Emitter [See Storage System](../../../buildings/Reference/Storage-System.md)
- **Energy Cost:** 50 energy/turn to maintain active singularity field
- **Failure Mode:** If power drops below 500, stored contents slowly evaporate at rate of 1% per minute until power restored or all contents lost
- **See Also:** [Storage System](../../../buildings/Reference/Storage-System.md), [Planet Buildings](../../../buildings/README.md)

### Cross-Planet Sync Network
- **Time:** 20 minutes
- **Materials:** 2,000 construction material, 1,500 minerals
- **Prerequisites:** Remote Transfer Network + Black Hole Storage started
- **Effect:** All storage emitters share inventory across planets simultaneously [See Storage System](../../../buildings/Reference/Storage-System.md)
- **Sync Scope:** Every storage container, haul, vault, and black hole emitter on every visited planet shares a single unified inventory
- **Access:** View and withdraw from any stored resource from any connected storage point
- **Size Required:** 2x2 grid slot per sync node (one required per planet base) [See Planet Buildings](../../../buildings/README.md)
- **Energy Cost:** 50 energy/turn per active sync node
- **Building Required:** Sync Node Emitter [See Storage System](../../../buildings/Reference/Storage-System.md)
- **See Also:** [Storage System](../../../buildings/Reference/Storage-System.md), [Planet Buildings](../../../buildings/README.md)

### Matter Compressor
- **Time:** 15 minutes
- **Materials:** 1,500 construction material, 1,000 minerals
- **Prerequisites:** Cross-Planet Sync Network + Black Hole Storage complete
- **Effect:** Reduce bulk items to 10% original volume [See Storage System](../../../buildings/Reference/Storage-System.md)
- **Volume Reduction:** All stored resources occupy only 10% of their normal storage capacity
- **Effective Capacity Multiplier:** 10x increase for all connected storage (Black Hole remains unlimited, standard storage gets 10x boost)
- **Compression Speed:** Instant on storage, takes 30 seconds to decompress items for retrieval
- **Size Required:** 2x1 grid slot per compressor unit [See Planet Buildings](../../../buildings/README.md)
- **Energy Cost:** 25 energy/turn per active compressor unit
- **Building Required:** Matter Compressor [See Planet Buildings](../../../buildings/README.md)
- **See Also:** [Storage System](../../../buildings/Reference/Storage-System.md), [Planet Buildings](../../../buildings/README.md)

---

## See Also

### Related Research Categories
- [Buildings Research](../Buildings-Research/README.md) — Storage buildings and facility placement
- [Energy Research](../../../resources/Energy/README.md) — Power requirements for storage systems
- [Drives Research](../Drives-Research/README.md) — Fuel storage tied to drive efficiency

### Related Documentation
- [Storage System](../../../buildings/Reference/Storage-System.md) — Storage buildings, containers, and quantum gate integration
- [Planet Buildings](../../../buildings/README.md) — Building definitions, grid sizes, and placement rules
- [Resources](../../../resources/README.md) — Resource types stored and converted by storage systems
- [Dropship](../../../Spaceship/Dropship/README.md) — Starting platform with initial storage locker

### Tech Tree Navigation
- [Tech Tree Overview](../../Overview/README.md) — Ring progression and unlock conditions
- [Research Categories Index](../README.md) — All categories master index
