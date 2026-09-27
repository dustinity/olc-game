# Unit Upgrade System

TIR-based armor, weapon, and vision upgrades for all unit types with material costs and forge requirements.

---

## Upgrade Overview

All units can be upgraded through the TIR-based upgrade system at Forge buildings [See Planet Buildings](../../buildings/README.md). Upgrades are permanent per-unit and persist across planets (units stored in hangars retain upgrades).

### Prerequisites

| Requirement | Tier 1→2 | Tier 2→3 | Tier 3→4 | Tier 4→5 |
|-------------|----------|----------|----------|----------|
| Building | Forge (TIR 2) | Energy Lab (TIR 3) | Dark Matter Lab (TIR 4) | Void Lab (TIR 5) |
| Unit Must Be | At building | At building | At building | At building |
| Research Required | Basic Armor I, Weapon I | Advanced Armor I, Weapon II | Elite Armor I, Weapon III | Master Armor I, Weapon V |

### Upgrade Slots Per Unit

Each unit type has a fixed number of upgrade slots:

| Unit Tier | Armor Slots | Weapon Slots | Vision/Utility Slots | Total Slots |
|-----------|-------------|--------------|---------------------|-------------|
| Infantry (TIR 1-2) | 1 | 1 | 0 | 2 |
| Infantry (TIR 3+) | 2 | 2 | 1 | 5 |
| Light Vehicles | 1 | 1 | 1 | 3 |
| Heavy Vehicles | 2 | 2 | 1 | 5 |
| Aerial Units | 1 | 2 | 1 | 4 |
| Support Units | 0-1 | 0-1 | 1 | 2-3 |

---

## Armor Upgrades

### Tier 1→2: Reinforced Plating

**Effect:** +50% HP, +10 Defense (damage reduction)

**Material Cost (per unit):**
| Resource | Amount | Source |
|----------|--------|--------|
| Construction Material | 100 | Mine/Scavenged |
| Minerals (Standard) | 80 | Mine |
| Energy | 50 | Power Grid |

**Research Required:** Basic Armor I (Forge research, 2 minutes)

**Applicable Units:** All infantry, light vehicles

**Visual Change:** Unit gains visible armor plating on front/shoulders. Color shifts from standard to reinforced gray.

---

### Tier 2→3: Titanium Composite Armor

**Effect:** +100% HP (total +150% from base), +25 Defense

**Material Cost (per unit):**
| Resource | Amount | Source |
|----------|--------|--------|
| Construction Material | 300 | Mine/Factory |
| Titanium Minerals | 150 | Rocky/Crystal deposits |
| Processed Metal | 100 | Refinery output |
| Energy | 200 | Power Grid |

**Research Required:** Advanced Armor I (Energy Lab research, 5 minutes)

**Applicable Units:** All units that have Tier 1→2 upgrade. Heavy vehicles and mechs only.

**Visual Change:** Full armor coverage with titanium sheen. Blue-gray metallic finish.

---

### Tier 3→4: Energy-Infused Armor

**Effect:** +200% HP (total +350% from base), +40 Defense, 10% damage reflection on melee

**Material Cost (per unit):**
| Resource | Amount | Source |
|----------|--------|--------|
| Construction Material | 800 | Factory/Assembly Plant |
| Dark Matter Crystals | 30 | Crystal Synthesizer or dungeon loot |
| Processed Titanium | 200 | Refinery output |
| Energy | 500 | Power Grid |

**Research Required:** Elite Armor I (Dark Matter Lab research, 10 minutes)

**Applicable Units:** Heavy vehicles, mechs, TIR 3+ infantry with full upgrade path

**Visual Change:** Armor plates glow faintly blue at edges. Energy channels visible along armor seams.

---

### Tier 4→5: Void-Infused Armor

**Effect:** +350% HP (total +600% from base), +60 Defense, 20% damage reflection, 15% attack chance reduction for enemies

**Material Cost (per unit):**
| Resource | Amount | Source |
|----------|--------|--------|
| Construction Material | 2,000 | Assembly Plant output |
| Dark Matter Crystals | 100 | Crystal Synthesizer or Void Lab |
| Reinforced Hull Alloy | 500 | Refinery (hull material processed) |
| Energy | 1,500 | Power Grid |

**Research Required:** Master Armor I (Void Lab research, 20 minutes)

**Applicable Units:** Heavy Mech, Walker, Tank, elite infantry squads

**Visual Change:** Armor shimmers with void energy. Purple-black hue with starlight edge highlights.

---

## Weapon Upgrades

### Tier 1→2: Enhanced Fire Control

**Effect:** +30% damage output

**Material Cost (per unit):**
| Resource | Amount | Source |
|----------|--------|--------|
| Construction Material | 80 | Mine/Scavenged |
| Minerals (Standard) | 60 | Mine |
| Radar Components | 10 | Factory or scavenged |
| Energy | 40 | Power Grid |

**Research Required:** Weapon I (Forge research, 2 minutes)

**Applicable Units:** All units with weapons. Infantry and light vehicles only at this tier.

**Visual Change:** Weapon sights gain red targeting reticle. Muzzle flash becomes slightly brighter.

---

### Tier 2→3: Heavy Barrel / Energy Amplifier

**Effect:** +60% damage output (total from base), +20% range

**Material Cost (per unit):**
| Resource | Amount | Source |
|----------|--------|--------|
| Construction Material | 250 | Mine/Factory |
| Processed Metals | 120 | Refinery output |
| Minerals (Advanced) | 100 | Crystal/Uranium deposits |
| Energy | 150 | Power Grid |

**Research Required:** Weapon II (Energy Lab research, 5 minutes)

**Applicable Units:** All units with weapons that have Tier 1→2 upgrade. Tanks, walkers, heavy infantry.

**Visual Change:** Barrel/Emitter visibly larger or thicker. Energy weapons glow brighter. Muzzle flash increases in size.

---

### Tier 3→4: Precision Targeting System

**Effect:** +100% damage output (total from base), +40% range, +25% accuracy

**Material Cost (per unit):**
| Resource | Amount | Source |
|----------|--------|--------|
| Construction Material | 600 | Factory/Assembly Plant |
| Processed Titanium | 180 | Refinery output |
| Dark Matter Crystals | 20 | Crystal Synthesizer or dungeon loot |
| Energy | 400 | Power Grid |

**Research Required:** Weapon III (Dark Matter Lab research, 10 minutes)

**Applicable Units:** Heavy vehicles, mechs, sniper units, aerial combat units

**Visual Change:** Weapon mounts gain targeting sensors. Laser designator visible on weapon housing. Energy weapons pulse rhythmically.

---

### Tier 4→5: Void Core Weaponry

**Effect:** +150% damage output (total from base), +60% range, ignores 30% of enemy armor

**Material Cost (per unit):**
| Resource | Amount | Source |
|----------|--------|--------|
| Construction Material | 1,500 | Assembly Plant output |
| Dark Matter Crystals | 80 | Crystal Synthesizer or Void Lab |
| Processed Hull Alloy | 400 | Refinery (hull material processed) |
| Energy | 1,200 | Power Grid |

**Research Required:** Weapon V (Void Lab research, 20 minutes)

**Applicable Units:** Heavy Mech, Walker, Tank, Fighter Jet, all TIR 5 units

**Visual Change:** Weapons emit void energy. Purple-black glow with spatial distortion effect around barrel/muzzle.

---

## Vision and Utility Upgrades

### Tier 1→2: Enhanced Sensors

**Effect:** +50% detection range, +20% spotting speed for ambushes

**Material Cost (per unit):**
| Resource | Amount | Source |
|----------|--------|--------|
| Construction Material | 60 | Mine/Scavenged |
| Minerals (Crystal) | 40 | Crystal deposits |
| Radar Components | 5 | Factory or scavenged |
| Energy | 30 | Power Grid |

**Research Required:** Vision I (Forge research, 1 minute)

**Applicable Units:** Scout units, all vehicles, aerial units

**Visual Change:** Sensor array on unit gains visible antenna dish. HUD display shows enhanced range indicator.

---

### Tier 2→3: Thermal + Night Vision

**Effect:** +100% detection range, thermal signature visibility through smoke/terrain cover, -30% ambush vulnerability

**Material Cost (per unit):**
| Resource | Amount | Source |
|----------|--------|--------|
| Construction Material | 200 | Mine/Factory |
| Advanced Sensors | 15 | Factory production |
| Minerals (Crystal) | 80 | Crystal deposits |
| Energy | 100 | Power Grid |

**Research Required:** Vision II (Energy Lab research, 3 minutes)

**Applicable Units:** All units with Tier 1→2 vision upgrade. Infantry scouts, aerial drones, vehicles.

**Visual Change:** Sensor housing gains secondary lens array. Unit HUD shows thermal overlay when targeting.

---

### Tier 3→4: Tactical AI Assistant

**Effect:** +150% detection range, auto-target priority for highest threat, +25% attack speed (AI-controlled units receive targeting assistance)

**Material Cost (per unit):**
| Resource | Amount | Source |
|----------|--------|--------|
| Construction Material | 500 | Factory/Assembly Plant |
| Advanced Sensors | 30 | Factory production |
| Dark Matter Crystals | 15 | Crystal Synthesizer or dungeon loot |
| Energy | 250 | Power Grid |

**Research Required:** Vision III (Dark Matter Lab research, 8 minutes)

**Applicable Units:** Heavy vehicles, mechs, aerial units, scout drones

**Visual Change:** Unit gains small AI processor module. HUD displays threat priority indicators and auto-target reticle.

---

## Upgrade Queue System

### Forge Processing

Upgrades are processed at Forge buildings [See Planet Buildings](../../buildings/README.md) in a queue system:

| Forge Tier | Max Queue Size | Research Speed Bonus |
|------------|----------------|---------------------|
| Basic Forge (TIR 2) | 1 unit at a time | Base speed |
| Energy Lab (TIR 3) | 2 units simultaneously | +25% research speed |
| Dark Matter Lab (TIR 4) | 3 units simultaneously | +50% research speed |
| Void Lab (TIR 5) | 5 units simultaneously | +100% research speed |

### Upgrade Duration by Tier

| Upgrade Type | Forge (TIR 2) | Energy Lab (TIR 3) | Dark Matter Lab (TIR 4) | Void Lab (TIR 5) |
|--------------|---------------|-------------------|------------------------|-----------------|
| Armor 1→2 | 2 minutes | 1.5 minutes | 1 minute | 30 seconds |
| Armor 2→3 | 5 minutes | 3.5 minutes | 2.5 minutes | 1.5 minutes |
| Armor 3→4 | 10 minutes | 7 minutes | 5 minutes | 3 minutes |
| Armor 4→5 | 20 minutes | 14 minutes | 10 minutes | 6 minutes |
| Weapon 1→2 | 2 minutes | 1.5 minutes | 1 minute | 30 seconds |
| Weapon 2→3 | 5 minutes | 3.5 minutes | 2.5 minutes | 1.5 minutes |
| Weapon 3→4 | 10 minutes | 7 minutes | 5 minutes | 3 minutes |
| Weapon 4→5 | 20 minutes | 14 minutes | 10 minutes | 6 minutes |
| Vision 1→2 | 1 minute | 45 seconds | 30 seconds | 15 seconds |
| Vision 2→3 | 3 minutes | 2 minutes | 1.5 minutes | 1 minute |
| Vision 3→4 | 8 minutes | 5.5 minutes | 4 minutes | 2.5 minutes |

### Parallel Upgrade Bonus

When multiple identical units are queued for the same upgrade:
- **2-4 units:** Research time -10% (researched once, applied to all)
- **5-9 units:** Research time -25% (batch production efficiency)
- **10+ units:** Research time -40% (factory line efficiency)

---

## Faction Upgrade Bonuses [See Factions](../../factions/README.md)

| Faction | Armor Bonus | Weapon Bonus | Vision Bonus | Special |
|---------|-------------|--------------|--------------|---------|
| Neon Punk | +15% material efficiency on upgrades | +10% damage per upgrade level | None | Quantum Mine: auto-refines 10% of upgrade materials |
| Dark Realistic | +25% HP from armor upgrades | +20% range from weapon upgrades | +15% detection range | Fortress doctrine: all defensive upgrades 50% cheaper |
| Cartoon SciFi | None | +25% damage per upgrade level | +20% attack speed with upgrades | Wild modifications: 10% chance for bonus effect on upgrade |
| Bright Realistic | +20% HP from armor upgrades | +10% durability (upgrades last longer) | +15% vision duration | Civilian efficiency: all upgrade costs -20% |

---

## Upgrade Decay and Replacement

### Permanent vs Temporary Upgrades

All upgrades are **permanent** once applied. However, certain game events can cause temporary degradation:

| Event | Effect | Recovery |
|-------|--------|----------|
| Planetary bombardment | -25% armor effectiveness for 10 minutes | Repairs over 5 minutes after event ends |
| Ion storm exposure | Weapon systems disabled for 30 seconds | Auto-repairs in 10 seconds |
| Extreme heat exposure | +50% weapon overheating rate | Cool down over 2 minutes out of combat |
| Radiation exposure | Vision sensors -50% effectiveness | Med Bay heals effect over 60 seconds |

### Upgrade Scrap Value

If a unit is destroyed, 30% of upgrade material cost is refunded to base storage. Energy cost is not refunded.

---

## See Also

- [Unit types and stats](../Unit-Types/README.md)
- [Forge building requirements](../../buildings/README.md)
- [TIR system and improvement cycles](../../weapons/TIR-System/README.md)
- [Faction bonuses](../../factions/README.md)
- [Champion ability scaling](../../factions/Champions/README.md)
