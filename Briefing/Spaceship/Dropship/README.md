# Dropship — Starting Ship Layout and Repair Mechanics

Initial dropship configuration, component breakdown, atmospheric entry sequence, and repair requirements to make flight-ready.

---

## Dropship Overview

The dropship is the player's starting vessel — a heavily used military transport that crashes on arrival at the target planet. It serves as both the introductory gameplay sequence and the foundation for later mothership expansion.

### Crash Sequence Plates

Temporary cinematic replacement plates are registered for the campaign-opening crash sequence:

| Beat | Style Reference | UE5 Source Asset |
|------|-----------------|------------------|
| Night approach / wing shear | [Crash-01-Night-Approach-Wing-Shear.png](../../../Assets/Stil-1/View/Cinematics/Crash-01-Night-Approach-Wing-Shear.png) | [UE5/Assets/Cinematics/DropshipCrash/Crash-01-Night-Approach-Wing-Shear.png](../../../UE5-Generated-Assets/Cinematics/DropshipCrash/Crash-01-Night-Approach-Wing-Shear.png) |
| Camera-side rock strike / wing loss | [Crash-02-Night-Camera-Rock-Strike.png](../../../Assets/Stil-1/View/Cinematics/Crash-02-Night-Camera-Rock-Strike.png) | [UE5/Assets/Cinematics/DropshipCrash/Crash-02-Night-Camera-Rock-Strike.png](../../../UE5-Generated-Assets/Cinematics/DropshipCrash/Crash-02-Night-Camera-Rock-Strike.png) |
| Pre-dawn hard ground crash | [Crash-03-Predawn-Hard-Ground-Crash.png](../../../Assets/Stil-1/View/Cinematics/Crash-03-Predawn-Hard-Ground-Crash.png) | [UE5/Assets/Cinematics/DropshipCrash/Crash-03-Predawn-Hard-Ground-Crash.png](../../../UE5-Generated-Assets/Cinematics/DropshipCrash/Crash-03-Predawn-Hard-Ground-Crash.png) |

Implementation notes live in [UE5/Assets/Cinematics/DropshipCrash/ASSET_MANIFEST.md](../../../UE5-Generated-Assets/Cinematics/DropshipCrash/ASSET_MANIFEST.md).

### Initial State

| Component | Status | Details |
|-----------|--------|---------|
| Hull Integrity | 35% (Damaged) | Scratches, dents, heat shield degradation |
| Fuel | 0% (Empty) | Depleted during atmospheric entry and crash |
| Energy Core | 60% (Functional) | Powers onboard systems, weapons, lights |
| Cockpit | Operational | Pilot seat, basic instruments, control panels |
| Rocket Drives | 2x (1 Functional, 1 Damaged) | Left drive damaged on impact, right drive functional but low fuel |
| Habitation Module | Operational | Crew quarters for 6 (currently empty except player champion + 10 soldiers in cryo-stasis) |
| Drop Module | Operational | Rear bay door for unit deployment, landing struts intact |
| Storage | 40% capacity | Initial supplies scattered due to crash |
| Energy Core Output | 50 energy units total | Powers all ship systems |

### Dropship Component Layout (Top-Down View)

```
┌─────────────────────────────────────┐
│           ROCKET DRIVE (Right)       │ ← Functional, low fuel
│              [1x1 Engine]            │
├─────────────────────────────────────┤
│          COCKPIT SECTION             │ ← Pilot + co-pilot seat
│         [1x1 Command Module]        │   Instruments, navigation
├─────────────────────────────────────┤
│       HABITATION MODULE              │ ← 6-person quarters
│         [1x1 Habitation]            │   Cryo-stasis pods (soldiers)
├─────────────────────────────────────┤
│                                     │
│        STORAGE BAY                   │ ← 40% full, scattered loot
│          [1x1 Storage]              │   Initial supplies
│                                     │
├─────────────────────────────────────┤
│       ENERGY CORE SECTION            │ ← Power generation
│         [1x1 Energy Core]           │   50 energy capacity
│                                     │
├─────────────────────────────────────┤
│        DROP MODULE (Rear)            │ ← Deployment bay
│      [2x2 Drop Module/Bay]          │   Rear door, landing struts
├─────────────────────────────────────┤
│       ROCKET DRIVE (Left)            │ ← Damaged, needs repair
│              [1x1 Engine]            │   50% functionality
└─────────────────────────────────────┘

Total Ship Footprint: ~8 grid slots (excluding external modules)
```

---

## Atmospheric Entry Sequence (Animated)

### Phase 1: Approach (Side View)
- Dropship flies low over desert terrain
- Engines humming, slight vibration from age
- Player views side-scrolling landscape through cockpit window
- Distant planet surface rises in view

### Phase 2: Descent Initiation
- Alert sounds — hull stress warning
- Dropship tilts, begins atmospheric entry burn
- Rocket drives flare as deceleration commences
- Heat shield engages (visible orange glow on hull)

### Phase 3: Crash Course
- Atmospheric turbulence hits hard
- Left rocket drive sputters and fails
- Dropship spins slightly, losing altitude rapidly
- Dunes rush up to meet the ship

### Phase 4: Impact
- Massive crash — screen shakes violently
- Dust and debris fly everywhere
- Ship slides across dune surface, carving trench
- Final impact against sand dune — ship comes to rest

### Phase 5: Post-Crash
- Cockpit view — cracked display, flickering lights
- Smoke fills cabin from damaged systems
- Side view — player champion exits through habitation module
- Rear door opens — player steps onto alien planet surface
- Game transitions to 2.5D top-down RTS perspective

---

## Component Details

### Cockpit [1x1 Command Module]

**Starting Equipment:**
- Basic flight instruments (altimeter, speed gauge, heading)
- Limited navigation computer (pre-programmed jump coordinates)
- Communication array (short-range only)
- 2 pilot seats (player + optional AI co-pilot from champions)

**Functions:**
- Launch/landing control
- Planet selection for next jump
- Solar system map display [See Navigation](../../Planets/Navigation/README.md)
- Radar warning receiver

**Repair Priority:** HIGH — Must be functional for interplanetary travel

---

### Rocket Drives [1x1 Engine each × 2]

#### Right Drive (Functional but Low Fuel)
- **Type:** Standard chemical rocket drive
- **Status:** Operational, fuel tank at 0%
- **Thrust:** 50 kN per drive (100 kN total when both functional)
- **Fuel Type:** Chemical propellant (basic grade)

**Repair Requirements:**
- Refuel from planet fuel reserves OR
- Build fuel processing facility on planet [See Resources](../../resources/README.md)
- Minimum 1 functional drive required for planetary orbit escape
- Both drives recommended for efficient interplanetary travel

#### Left Drive (Damaged on Crash)
- **Type:** Standard chemical rocket drive
- **Status:** Partially operational (50% thrust)
- **Damage Source:** Debris impact during crash slide
- **Repair Needed:** Physical inspection, possible part replacement

**Repair Materials:**
- 100 Construction Material
- Blueprint: Drive Component Kit (from small dungeon or starting loot)

Diagnostics use ship power but do not add an energy-resource repair cost. Processed metals are part of crafting replacement components, not routine repair payment.

---

### Habitation Module [1x1 Habitation]

**Starting Capacity:** 6 personnel
- Player champion (1 slot)
- 10 soldiers in cryo-stasis pods (2 slots active, 8 in stasis)
- Remaining capacity: 3 slots (can recruit/train on planet)

**Cryo-Stasis System:**
- Keeps soldiers alive during transit and post-crash recovery
- Power-dependent — if energy core fails, stasis pods deactivate after 2 hours
- Soldiers wake automatically upon habitation module power restoration
- Recovery time: 10 minutes per soldier from stasis

**Functions:**
- Crew rest quarters (6 bunks)
- Cryo-stasis pod bank (10 pods)
- Mess area (small)
- Storage locker (personal gear)

**Repair Priority:** MEDIUM — Soldiers needed for base building and defense

---

### Drop Module [2x2 Drop Module/Bay]

**Features:**
- Rear deployment door (primary exit point)
- Landing struts (4 points, all intact)
- Ramp mechanism (functional but may need calibration)
- External weapon mounts (2x, currently empty — can mount turrets)

**Functions:**
- Unit deployment during planetary operations
- Vehicle launch/retrieval (if hangar module added later)
- Emergency exit from ship
- Loading/unloading cargo

**Repair Priority:** LOW — Functional but ramp calibration recommended

---

### Storage [1x1 Storage]

**Starting Contents (40% capacity, scattered due to crash):**
| Resource | Amount | Location |
|----------|--------|----------|
| Construction Material | 80 | Main storage (scattered ~20 around bay) |
| Minerals | 50 | Main storage |
| Fuel | 30 | Main storage |
| Energy Cells | 20 | Main storage |
| Survival (Food/Oxygen) | 40 | Main storage |
| Hull Repair Parts | 25 | Under debris in drop module |

**Scattered Items:**
- 4 construction material piles near drop module (20 total)
- 2 mineral deposits under collapsed shelving
- 1 fuel canister leaked onto floor (10 units, flammable)

**Repair Priority:** LOW — Contents accessible but disorganized

---

### Energy Core [1x1 Energy Core]

**Specifications:**
- **Type:** Compact fusion reactor (deuterium-based)
- **Output:** 50 energy units total capacity
- **Current Status:** 60% operational
- **Power Distribution:**
  - Life support: 10 energy/turn
  - Systems and instruments: 5 energy/turn
  - Cryo-stasis pods: 8 energy/turn
  - Lights and HVAC: 7 energy/turn
  - Weapons mounts: 0 (no weapons attached)
  - Reserve: 20 energy available for modules

**Functions:**
- Primary power source for all ship systems
- Backup battery (30-minute reserve if core shuts down)
- Power routing to external modules (when added)

**Repair Priority:** HIGH — Core efficiency affects all systems

**Upgrade Path:**
- Current: 50 energy capacity (starting)
- TIR 2 upgrade: 100 energy capacity (+100%)
- TIR 3 upgrade: 200 energy capacity (+300% total)
- TIR 4 upgrade: 500 energy capacity (+900% total)
- TIR 5 upgrade: 1,000 energy capacity (+1,900% total)

---

## Dropship Repair Checklist

### Immediate Repairs (Required to Leave Planet)

| # | Task | Materials Required | Time | Priority |
|---|------|-------------------|------|----------|
| 1 | Refuel right rocket drive | 50 fuel from planet reserves | 5 minutes | CRITICAL |
| 2 | Repair left rocket drive | 100 construction material | 15 minutes | HIGH |
| 3 | Recover scattered storage items | None (manual collection) | 10 minutes | MEDIUM |
| 4 | Restore energy core to 80%+ | 50 construction material | 10 minutes | HIGH |

### Secondary Repairs (Recommended Before Extended Travel)

| # | Task | Materials Required | Time | Priority |
|---|------|-------------------|------|----------|
| 5 | Calibrate ramp mechanism | 30 construction material | 5 minutes | LOW |
| 6 | Organize storage bay | None (manual sorting) | 15 minutes | MEDIUM |
| 7 | Mount weapons on drop module external slots | 2x turret mounts (scavenged or crafted) | 20 minutes | MEDIUM |
| 8 | Upgrade energy core output | See upgrade path above | Varies by tier | HIGH for expansion |

### Long-Term Expansion (Mothership Conversion)

Once dropship is flight-ready, expansion modules can be added:
- Additional hangar bays [See Ship Modules](../../ShipModules/README.md)
- Expanded storage [See Storage System](../../buildings/Reference/Storage-System.md)
- Advanced scanning arrays [See Navigation](../../Planets/Navigation/README.md)
- Weapon systems [See Weapons](../../weapons/Weapon-Types/README.md)
- Lab/research facilities [See Ship Modules](../../ShipModules/README.md)

---

## Dropship Combat Capability

### Starting Armament
- None (transport vessel, not warship)
- 2x external weapon mount points (empty)
- Champion personal weapons only

### Recommended Early Upgrades
1. Mount 2x Ballistic Turrets on external drop module slots [See Weapons](../../weapons/Weapon-Types/README.md)
   - Cost: 200 construction material, 150 minerals per turret
   - Damage: 15 per hit each
   - Range: 30m

2. Add basic energy shield [See Ship Modules](../../ShipModules/README.md)
   - Cost: 300 construction material, 200 minerals
   - Effect: +100 HP equivalent protection

3. Install basic radar [See Navigation](../../Planets/Navigation/README.md)
   - Cost: 150 construction material, 100 minerals
   - Range: Short-medium radial scan

---

## Dropship Stats Summary

| Attribute | Starting | After Full Repair | Max (TIR 5+) |
|-----------|----------|-------------------|--------------|
| Hull Integrity | 35% | 85% | 100% + upgrades |
| Fuel Capacity | 100 units | 100 units | 500+ (upgraded) |
| Energy Output | 50 units | 50 units | 1,000+ (upgraded) |
| Crew Capacity | 6 (+10 stasis) | 6 (+10 stasis) | 32+ (expanded) |
| Weapon Slots | 2 (empty) | 2 (mountable) | 8+ (expanded) |
| Storage | 40% full | ~70% organized | 10,000+ per type |
| Drive Functionality | 50% (1 damaged) | 100% | 150% (upgraded drives) |

---

## See Also

- [Ship module expansion system](../../ShipModules/README.md)
- [Storage grid placement](../../buildings/Reference/Storage-System.md)
- [Planet resource types and fuel generation](../../resources/README.md)
- [Solar system navigation](../../Planets/Navigation/README.md)
- [Weapon mounting on ship turrets](../../weapons/Weapon-Types/README.md)
- [Dropship progression path](../Progression/README.md)
