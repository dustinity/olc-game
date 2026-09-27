# Ship Progression — Dropship Repair to Mothership Expansion

Complete ship evolution path from crashed dropship to fully expanded mothership across all TIR tiers.

---

## Progression Overview

Ship progression follows the player's TIR advancement through three major phases:
1. **Phase 1:** Dropship Repair (TIR 1) — Get flight-ready
2. **Phase 2:** Mothership Expansion (TIR 2-3) — Build exploration capability  
3. **Phase 3:** Fleet Command (TIR 4-5) — Deep space dominance

### Progression Milestones

| Phase | TIR Range | Ship State | Key Capability |
|-------|-----------|------------|----------------|
| Crash Landing | TIR 1 | Damaged dropship | Planet exploration, resource gathering |
| Flight Ready | TIR 1-2 | Repaired dropship | Interplanetary travel within starting system |
| Explorer | TIR 2 | Expanded with modules | System-hopping, basic combat capability |
| Command Ship | TIR 3 | Full mothership configuration | Multi-system operations, fleet deployment |
| Flagship | TIR 4-5 | Elite mothership with alien tech | Galaxy traversal, wormhole transit |

---

## Phase 1: Dropship Repair (TIR 1)

### Starting State [See Dropship](../Dropship/README.md)

```
┌─────────────────────────────────────┐
│           ROCKET DRIVE (Right)       │ ← Functional, empty fuel
│              [1x1 Engine]            │
├─────────────────────────────────────┤
│          COCKPIT SECTION             │ ← Operational
│         [1x1 Command Module]        │
├─────────────────────────────────────┤
│       HABITATION MODULE              │ ← 6 capacity + cryo pods
│         [1x1 Habitation]            │
├─────────────────────────────────────┤
│           STORAGE BAY                │ ← 40% full, scattered
│          [1x1 Storage]              │
├─────────────────────────────────────┤
│         ENERGY CORE SECTION          │ ← 50 energy capacity
│         [1x1 Energy Core]           │   60% operational
├─────────────────────────────────────┤
│           DROP MODULE (Rear)         │ ← Deployment bay
│      [2x2 Drop Module/Bay]          │
├─────────────────────────────────────┤
│       ROCKET DRIVE (Left)            │ ← Damaged, 50% thrust
│              [1x1 Engine]            │
└─────────────────────────────────────┘

Size: 8 grid slots | Hull: 35% | Fuel: 0% | Energy: 50 units
```

### Repair Steps (Detailed)

#### Step 1: Secure the Crash Site [Time: 5-10 minutes]
- Collect scattered storage items from drop module bay
- Clear debris blocking rear deployment door
- Assess full hull damage extent
- Wake 2 soldiers from cryo-stasis for assistance

**Requirements:** None (manual labor)

#### Step 2: Establish Basic Power [Time: 15 minutes]
- Restore energy core to 80%+ operational capacity
- Connect power to life support and cryo-stasis pods
- Install temporary solar panels if available [See Planet Buildings](../../buildings/README.md)

**Materials Required:**
| Resource | Amount | Source |
|----------|--------|--------|
| Construction Material | 50 | Recovered from crash site or produced locally |

#### Step 3: Refuel Rocket Drive [Time: 10-30 minutes]
**Option A — Scavenge Fuel:**
- Explore nearby area for fuel deposits [See Resources](../../resources/README.md)
- Collect minimum 50 fuel units from surface deposits
- Transfer to right rocket drive tank

**Option B — Build Fuel Processor:**
- Construct basic oil pump or fuel processor on planet surface
- Process raw fuel (oil/gas) into rocket propellant [See Planet Buildings](../../buildings/README.md)
- Minimum 50 fuel required for orbital escape

#### Step 4: Repair Left Rocket Drive [Time: 20-30 minutes]
- Inspect drive for physical damage
- Replace damaged components from storage bay spare parts
- Calibrate thrust output to minimum 70% capacity

**Materials Required:**
| Resource | Amount | Source |
|----------|--------|--------|
| Construction Material | 100 | Collected from crash site + local mine |

#### Step 5: Mount Basic Weapons [Time: 20 minutes]
- Install 2x Ballistic Turrets on drop module external mounts [See Weapons](../../weapons/Weapon-Types/README.md)
- Connect power lines from energy core to weapon systems
- Test fire both turrets

**Materials Required:**
| Resource | Amount | Source |
|----------|--------|--------|
| Construction Material | 400 | 200 per turret |
| Minerals | 300 | 150 per turret |

### Post-Repair State

```
┌─────────────────────────────────────┐
│           ROCKET DRIVE (Right)       │ ← Full fuel, operational
│              [1x1 Engine]            │
├─────────────────────────────────────┤
│          COCKPIT SECTION             │ ← Fully operational
│         [1x1 Command Module]        │
├─────────────────────────────────────┤
│       HABITATION MODULE              │ ← 6 capacity + cryo pods
│         [1x1 Habitation]            │
├─────────────────────────────────────┤
│           STORAGE BAY                │ ← Organized, ~70% full
│          [1x1 Storage]              │
├─────────────────────────────────────┤
│         ENERGY CORE SECTION          │ ← 50 energy capacity
│         [1x1 Energy Core]           │   85% operational
├─────────────────────────────────────┤
│     DROP MODULE (Rear) + WEAPONS     │ ← 2x Ballistic Turrets
│      [2x2 Drop Module/Bay]          │
├─────────────────────────────────────┤
│       ROCKET DRIVE (Left)            │ ← Repaired, operational
│              [1x1 Engine]            │   90% thrust capacity
└─────────────────────────────────────┘

Size: 8 grid slots | Hull: 85% | Fuel: 100 units | Energy: 50 units
Weapons: 2x Ballistic Turret (15 damage each, 30m range)
```

**Capabilities:**
- Escape planet orbit (single drive sufficient)
- Travel to nearby planets within same solar system
- Basic self-defense against light threats
- Deploy up to 6 combat units on planetary surface

---

## Phase 2: Mothership Expansion (TIR 2-3)

### Expansion Philosophy

Once the dropship can reach orbit, begin systematic expansion by adding modules that enable deeper exploration and sustained operations. Each module requires:
- Discovery of blueprint (dungeon loot or forge research) [See Tech Tree](../../Tech-Tree/Overview/README.md)
- Construction materials from planet surface
- Energy for installation and calibration

### Expansion Slot Acquisition

The base dropship has 8 grid slots with no expansion capacity. New slots are added by:

| Method | Slots Added | TIR Required | Cost |
|--------|-------------|--------------|------|
| Add Hull Section (1x2) | +4 module slots | TIR 2 | 500 construction, 300 minerals |
| Add Hull Section (2x2) | +8 module slots | TIR 2 | 1,000 construction, 600 minerals |
| Add Hull Section (3x3) | +12 module slots | TIR 3 | 2,000 construction, 1,200 minerals |

**Recommended First Expansion:** 2x2 hull section (+8 slots) for maximum flexibility

### Module Bay Layout (Post-First Expansion)

```
┌───────────────────────────────────────────────────┐
│ [Hangar 2x2] [Storage 2x2]                        │ ← New expansion bay
├───────────────────────────────────────────────────┤
│ [Lab 2x1] [Crew Quarters 2x1]                     │
├───────────────────────────────────────────────────┤
│ ROCKET DRIVE (Right) |       COCKPIT              │
│ [1x1 Engine]         |    [1x1 Command Module]   │
├───────────────────────────────────────────────────┤
│ HABITATION MODULE     |        STORAGE BAY        │
│ [1x1 Habitation]      |     [1x1 Storage]         │
├───────────────────────────────────────────────────┤
│       ENERGY CORE SECTION                         │
│         [1x1 Energy Core]                         │
├───────────────────────────────────────────────────┤
│        DROP MODULE (Rear) + WEAPONS               │
│      [2x2 Drop Module/Bay]                        │
├───────────────────────────────────────────────────┤
│    ROCKET DRIVE (Left)                            │
│         [1x1 Engine]                              │
└───────────────────────────────────────────────────┘

Size: 16 grid slots | Hull: 85% | Fuel: 200 units | Energy: 100 units
Module Slots Available: 4 (8 added - 4 used by new modules)
```

### Essential Module Additions (Priority Order)

#### Priority 1: Expanded Storage [See Storage System](../../buildings/Reference/Storage-System.md)
- **Grid Size:** 2x2 Container or save for 4x4 Haul Storage at TIR 3
- **Capacity:** 2,000 resources per type (Container) / 10,000 (Haul)
- **Cost:** 300 construction + 100 minerals (Container)
- **Reason:** Resource accumulation outpaces starting locker quickly

#### Priority 2: Advanced Drive [See Ship Modules](../../ShipModules/README.md)
- **Grid Size:** 2x1 Atomic Reactor Drive
- **TIR:** 2
- **Effect:** -40% fuel consumption, 1.5 hops per tank
- **Cost:** 800 construction material, 600 minerals
- **Reason:** Enables reliable interplanetary travel beyond starting system

#### Priority 3: Radar/Scanning Module [See Navigation](../../Planets/Navigation/README.md)
- **Grid Size:** 1x1 Radar Array
- **TIR:** 2
- **Range:** Medium-long radial scan
- **Cost:** 400 construction material, 300 minerals
- **Reason:** Discover nearby planets and dungeons before landing

#### Priority 4: Crew Quarters [See Ship Modules](../../ShipModules/README.md)
- **Grid Size:** 1x1 Small Crew Quarters
- **Capacity:** +4 personnel (total 10 with existing habitation)
- **Cost:** 300 construction material, 200 minerals
- **Reason:** More units for planetary operations and base building

#### Priority 5: Energy Core Upgrade [See Ship Modules](../../ShipModules/README.md)
- **Grid Size:** Replace existing 1x1 with upgraded version
- **TIR:** 3
- **Capacity:** 200 energy (4x starting capacity)
- **Cost:** 1,500 construction material, 1,000 minerals
- **Reason:** Powers additional modules and weapons simultaneously

### TIR 3 State — Full Explorer Configuration

```
┌───────────────────────────────────────────────────┐
│ [Hangar 2x2] [Storage Haul 4x4]                   │ ← Heavy storage capacity
├───────────────────────────────────────────────────┤
│ [Energy Lab 3x3] [Crew Quarters Large 2x2]        │
├───────────────────────────────────────────────────┤
│ [Atomic Drive 2x1] |     [COCKPIT + Radar]        │
│                    |    [1x1 Command Module]      │
├───────────────────────────────────────────────────┤
│ [Habitation]       |        [Storage Container]   │
│ [1x1 Habitation]   |     [2x2/4x1 Storage]       │
├───────────────────────────────────────────────────┤
│    [ENERGY CORE UPGRADED TIR 3]                   │
│      [1x1 Energy Core - 200 capacity]             │
├───────────────────────────────────────────────────┤
│   DROP MODULE + 2x Ballistic Turrets              │
│      [2x2 Drop Module/Bay]                        │
├───────────────────────────────────────────────────┤
│    [Atomic Drive 2x1 - Left Side]                 │
└───────────────────────────────────────────────────┘

Size: 24 grid slots | Hull: 90% | Fuel: 300 units | Energy: 200 units
Weapons: 2x Ballistic Turret + optional additional mounts
Modules Active: Drive, Storage, Radar, Crew Quarters, Energy Core Upgrade
```

**Capabilities:**
- Reliable interplanetary travel (both atomic drives)
- Store 10,000+ resources per type for extended missions
- Scan entire planets before landing [See Navigation](../../Planets/Navigation/README.md)
- Deploy up to 14 combat units on planetary surface
- Research TIR 3 technology in mobile Energy Lab [See Tech Tree](../../Tech-Tree/Research-Categories/README.md)

---

## Phase 3: Fleet Command (TIR 4-5)

### Flagship Configuration

At TIR 4+, the mothership transitions from exploration vessel to command ship capable of fleet operations.

#### Required Upgrades

| Module | Grid Size | TIR | Cost | Effect |
|--------|-----------|-----|------|--------|
| Void Warp Drive | 4x4 | 5 | 3,000 construction, 2,000 dark matter crystals | Instant travel within 5-system radius |
| Reinforced Vault Storage | 4x4 | 3+ | 3,000 construction, 2,000 minerals | 25,000 resources per type, crash protection |
| Alien Shield Generator | 2x2 emitter | Alien | 5,000 construction, 3,000 dark matter crystals | Immune to physical/projectile damage |
| Quantum Scanner | 2x2 | 5 | 2,000 construction, 1,500 minerals | Planet-wide instant map reveal |
| Void Beam Weapon | 3x2 mount | 5 | 4,000 construction, 2,000 minerals | Extreme damage, ignores all armor |
| Heavy Mech Hangar | 6x6 | 4 | 5,000 construction, 3,000 minerals | 4 vehicle/ship slots for fleet deployment |
| Void Lab Research | 5x5 | 5 | 10,000 construction, 5,000 dark matter crystals | Ultimate research facility [See Tech Tree](../../Tech-Tree/Research-Categories/README.md) |
| Large Crew Quarters ×2 | 2x2 each | 3 | 3,000 construction, 1,600 minerals total | +32 personnel capacity (total 42+) |

#### Flagship Layout Example (TIR 5)

```
┌───────────────────────────────────────────────────────┐
│ [Heavy Mech Hangar 6x6] [Reinforced Vault 4x4]       │ ← Fleet + storage
├───────────────────────────────────────────────────────┤
│ [Void Lab 5x5] [Crew Quarters Large ×2]               │
├───────────────────────────────────────────────────────┤
│ [Quantum Scanner 2x2] |    [COCKPIT + Radar Array]   │
│                       | [1x1 Command Module]          │
├───────────────────────────────────────────────────────┤
│ [Void Warp Drive 4x4] |      [Storage Container ×2]  │
│                       |     [2x2/4x1 Storage]        │
├───────────────────────────────────────────────────────┤
│       [ENERGY CORE UPGRADED TIR 5 - 1000 capacity]   │
│                    [1x1 Energy Core]                   │
├───────────────────────────────────────────────────────┤
│ DROP MODULE + Weapons ×4 (2x Ballistic, 1x Rocket,   │
│          1x Void Beam on external mounts)             │
├───────────────────────────────────────────────────────┤
│         [Void Warp Drive 4x4 - Rear]                  │
└───────────────────────────────────────────────────────┘

Size: 40+ grid slots | Hull: 95% (+Alien Shield) | Fuel: 500 units | Energy: 1000 units
Weapons: 2x Ballistic Turret, 1x Rocket Bay, 1x Void Beam
Modules Active: Drives, Storage ×3, Radar, Scanner, Hangar, Lab, Crew Quarters ×3, Core
```

**Capabilities:**
- Wormhole transit between discovered systems [See Navigation](../../Planets/Navigation/README.md)
- Instant planet-wide scanning and mapping
- Deploy fleet of 4 vehicles + up to 42 personnel
- Store effectively unlimited resources with upgraded storage chain
- Engage any enemy type including TIR 5 and alien variants [See Races](../../factions/Races/README.md)
- Research all technology tiers including Void and Alien [See Tech Tree](../../Tech-Tree/Research-Categories/README.md)

### Alien Drive Integration (Post-Center Galaxy)

After defeating the final boss at galaxy center, integrate Alien Warp Drive:

```
┌───────────────────────────────────────────────────────┐
│ [Alien Warp Core 4x4] replaces standard Void Drive    │ ← Instant transit
├───────────────────────────────────────────────────────┤
│ All other modules remain functional                    │
└───────────────────────────────────────────────────────┘

New Capability: Unlimited range wormhole transit to any discovered system
Fuel Consumption: 0.01x (essentially free)
Wormhole Opening Time: 10 seconds
Transit Time: 5 seconds through wormhole
```

---

## Progression Summary Table

| Milestone | TIR | Ship Size | Fuel Capacity | Energy Output | Crew Cap | Weapon Slots | Key Capability |
|-----------|-----|-----------|---------------|---------------|----------|--------------|----------------|
| Crash Landing | 1 | 8 slots | 100 units | 50 units | 6+10 stasis | 2 (empty) | Planet exploration |
| Flight Ready | 1-2 | 8 slots | 100 units | 50 units | 6+10 stasis | 2 (turrets) | Orbital escape |
| Explorer | 2 | 16-24 slots | 200-300 units | 100-200 units | 10-14 | 4+ | Interplanetary travel |
| Command Ship | 3 | 24-32 slots | 300-500 units | 200-500 units | 14-30 | 6+ | Fleet deployment |
| Flagship | 4-5 | 32-48 slots | 500+ units | 500-1000 units | 30-42+ | 8+ | Galaxy traversal |
| Alien Flagship | Alien | 40+ slots | 500+ units | 1000+ units | 42+ | 10+ | Wormhole transit |

---

## Module Installation Requirements

### Forge Research Required [See Tech Tree](../../Tech-Tree/Overview/README.md)

| Module Category | Research Building | Time Required |
|-----------------|-------------------|---------------|
| Drive Upgrades | Forge → Energy Lab → Dark Matter Lab → Void Lab | 2min to 20min |
| Storage Expansion | Forge (basic) or Factory (advanced) | 1min to 5min |
| Weapon Systems | Forge (ballistic/rocket) or Energy Lab (energy/ion) | 2min to 10min |
| Scanning Arrays | Forge (radar) or Energy Lab (sonar/laser) | 1min to 8min |
| Crew Quarters | Basic construction, no research required | Varies by size |
| Energy Core | Energy Lab minimum for TIR 3+ upgrades | 5min to 20min |

### Material Costs Scale with Ship Size

As ship grid slots increase, module costs scale:

| Base Ship Slots | Cost Multiplier | Example (2x2 Module) |
|-----------------|-----------------|---------------------|
| 8 (Starting) | 1.0× | 300 construction + 100 minerals |
| 16 | 1.2× | 360 construction + 120 minerals |
| 24 | 1.5× | 450 construction + 150 minerals |
| 32+ | 2.0× | 600 construction + 200 minerals |

---

## See Also

- [Dropship starting configuration and repair](../Dropship/README.md)
- [Ship module catalog and specifications](../../ShipModules/README.md)
- [Storage system grid placement](../../buildings/Reference/Storage-System.md)
- [Weapon types for ship mounting](../../weapons/Weapon-Types/README.md)
- [Drive efficiency comparison](../../resources/README.md)
- [Tech tree research requirements](../../Tech-Tree/Overview/README.md)
- [Solar system navigation and wormholes](../../Planets/Navigation/README.md)
