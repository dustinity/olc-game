# Armor Research Category

Comprehensive breakdown of all armor research topics with prerequisites, material costs, and cross-references. Covers hull plating, unit armor upgrades, energy shields, and alien shield technology — from basic dropship repair to void-infused armor and damage-immune barriers.

| Ring | Lab | Topics |
|------|-----|--------|
| Core | N/A | Basic Hull Plating (starting) |
| Ring 1 | Basic Forge | Reinforced Plating I, Hull Repair Basics, Basic Shield Generation |
| Ring 2 | Energy Lab | Titanium Composite Armor, Hull Reinforcement II, Energy Shield Generation II |
| Ring 3 | Dark Matter Lab | Energy-Infused Armor, Hull Reinforcement III, Advanced Shield Generation |
| Outer Ring | Void Lab | Void-Infused Armor, Reinforced Composite Hull, Alien Shield Generator |

---

## Core -- Starting (No Research Required)

### Basic Hull Plating

- **Time:** Auto-unlock at game start
- **Materials:** None
- **Effect:** Standard dropship hull, 35% integrity at crash landing [See Dropship](../../../Spaceship/Dropship/README.md)
- **Repair Cost:** 100 construction material restores 15% hull integrity
- **See Also:** [Dropship](../../../Spaceship/Dropship/README.md), [Upgrade System](../../../units/Upgrade-System/README.md)

---

## Ring 1 -- Basic Forge Research

### Reinforced Plating I (Unit Upgrade)

- **Time:** 3 minutes
- **Materials:** 150 construction material, 100 minerals
- **Prerequisites:** None (Ring 1 available at start)
- **Effect:** Unit armor upgrade tier 1->2 — +50% HP, +10 Defense [See Upgrade System](../../../units/Upgrade-System/README.md)
- **Applies To:** All infantry and light vehicle units
- **Forge Required:** Basic Forge (TIR 2)
- **See Also:** [Upgrade System](../../../units/Upgrade-System/README.md), [Units](../../../units/README.md)

### Hull Repair Basics

- **Time:** 2 minutes
- **Materials:** 100 construction material, 50 minerals
- **Prerequisites:** None (Ring 1 available at start)
- **Effect:** Restore dropship hull integrity to 85%+ from crash state [See Dropship](../../../Spaceship/Dropship/README.md)
- **Hull Restoration:** Restores from 35% to 85% in single application
- **Additional Repair Cost:** 50 construction material per 10% after initial repair
- **See Also:** [Dropship](../../../Spaceship/Dropship/README.md), [Ship Modules](../../../ShipModules/README.md)

### Basic Shield Generation

- **Time:** 5 minutes
- **Materials:** 300 construction material, 200 minerals
- **Prerequisites:** Hull Repair Basics complete + Power Distribution I ([Energy Research](../Energy-Research/README.md))
- **Effect:** +100 HP equivalent energy shield for dropship [See Ship Modules](../../../ShipModules/README.md)
- **Shield Recharge Time:** 30 seconds after depletion
- **Energy Cost:** 20 energy/turn while active
- **Max Shield Capacity:** 100 HP (absorbs all damage types equally)
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Dropship](../../../Spaceship/Dropship/README.md)

---

## Ring 2 -- Energy Lab Research

### Titanium Composite Armor (Unit Upgrade)

- **Time:** 10 minutes
- **Materials:** 800 construction material, 500 minerals, 200 energy
- **Prerequisites:** Reinforced Plating I complete + Processed Metals available [See Resources](../../../resources/README.md)
- **Effect:** Unit armor upgrade tier 2->3 — +100% HP (total +150% from base), +25 Defense [See Upgrade System](../../../units/Upgrade-System/README.md)
- **Applies To:** All units with Tier 1->2 upgrade. Heavy vehicles and mechs only at this tier.
- **Visual Change:** Full armor coverage with titanium sheen, blue-gray metallic finish
- **Forge Required:** Energy Lab (TIR 3)
- **See Also:** [Upgrade System](../../../units/Upgrade-System/README.md), [Resources](../../../resources/README.md)

### Hull Reinforcement II

- **Time:** 8 minutes
- **Materials:** 600 construction material, 400 minerals
- **Prerequisites:** Hull Repair Basics complete + Titanium Composite Armor research started
- **Effect:** Heat-resistant alloy hull for dropship [See Dropship](../../../Spaceship/Dropship/README.md)
- **Heat Resistance:** +200% resistance to thermal and incendiary damage
- **Hull Integrity Bonus:** +15% maximum hull capacity (85% -> 100%)
- **Material Required per Application:** 200 construction material, 100 minerals
- **See Also:** [Dropship](../../../Spaceship/Dropship/README.md), [Ship Modules](../../../ShipModules/README.md)

### Energy Shield Generation II

- **Time:** 12 minutes
- **Materials:** 500 construction material, 300 minerals, 200 energy
- **Prerequisites:** Basic Shield Generation complete + Power Distribution II ([Energy Research](../Energy-Research/README.md))
- **Effect:** Absorbs 200 damage/second shield for dropship [See Ship Modules](../../../ShipModules/README.md)
- **Shield Recharge Time:** 15 seconds after depletion (-50% over basic)
- **Energy Cost:** 30 energy/turn while active (+50% over basic, but double effectiveness)
- **Max Shield Capacity:** 200 HP (double basic shield)
- **Special Feature:** Toggleable efficiency mode — reduces to 100 HP capacity but only 15 energy/turn
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Dropship](../../../Spaceship/Dropship/README.md)

---

## Ring 3 -- Dark Matter Lab Research

### Energy-Infused Armor (Unit Upgrade)

- **Time:** 15 minutes
- **Materials:** 1,200 construction material, 800 minerals, 500 energy
- **Prerequisites:** Titanium Composite Armor complete + Dark Matter Crystals available [See Resources](../../../resources/README.md)
- **Effect:** Unit armor upgrade tier 3->4 — +200% HP (total +350% from base), +40 Defense, 10% melee damage reflection [See Upgrade System](../../../units/Upgrade-System/README.md)
- **Applies To:** Heavy vehicles, mechs, TIR 3+ infantry with full upgrade path
- **Visual Change:** Armor plates glow faintly blue at edges, energy channels visible along armor seams
- **Forge Required:** Dark Matter Lab (TIR 4)
- **See Also:** [Upgrade System](../../../units/Upgrade-System/README.md), [Resources](../../../resources/README.md)

### Hull Reinforcement III

- **Time:** 12 minutes
- **Materials:** 1,000 construction material, 700 minerals
- **Prerequisites:** Hull Reinforcement II complete + Titanium Composite Armor research complete
- **Effect:** Titanium alloy hull with maximum heat resistance for dropship [See Dropship](../../../Spaceship/Dropship/README.md)
- **Heat Resistance:** +400% total thermal protection (all biomes safe for extended operations)
- **Hull Integrity Bonus:** Maintains 100% integrity under all standard combat conditions
- **Material Required per Application:** 300 construction material, 200 minerals
- **See Also:** [Dropship](../../../Spaceship/Dropship/README.md), [Ship Modules](../../../ShipModules/README.md)

### Advanced Shield Generation

- **Time:** 18 minutes
- **Materials:** 800 construction material, 600 minerals, 400 energy
- **Prerequisites:** Energy Shield Generation II complete + Ion Weapon Integration ([Weapons Research](../Weapons-Research/README.md))
- **Effect:** Expanded shield coverage with toggleable efficiency mode for dropship [See Ship Modules](../../../ShipModules/README.md)
- **Shield Capacity:** 350 HP (+75% over Ring 2 shield)
- **Coverage:** Full 360 spherical protection (no blind spots)
- **Efficiency Mode:** 175 HP capacity at only 20 energy/turn (-33% over Ring 2 active cost)
- **Special Feature:** Damage reflection — 5% of melee damage reflected to attacker while shield is active
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Dropship](../../../Spaceship/Dropship/README.md)

---

## Outer Ring -- Void Lab Research

### Void-Infused Armor (Unit Upgrade)

- **Time:** 25 minutes
- **Materials:** 3,000 construction material, 1,500 dark matter crystals
- **Prerequisites:** Energy-Infused Armor complete + Dark Matter Crystals from Crystal Synthesizer [See Resources](../../../resources/README.md)
- **Effect:** Unit armor upgrade tier 4->5 — +350% HP (total +600% from base), +60 Defense, 20% damage reflection, 15% attack chance reduction for enemies [See Upgrade System](../../../units/Upgrade-System/README.md)
- **Applies To:** Heavy Mech, Walker, Tank, elite infantry squads
- **Visual Change:** Armor shimmers with void energy, purple-black hue with starlight edge highlights
- **Forge Required:** Void Lab (TIR 5)
- **See Also:** [Upgrade System](../../../units/Upgrade-System/README.md), [Resources](../../../resources/README.md)

### Reinforced Composite Hull

- **Time:** 20 minutes
- **Materials:** 3,000 construction material, 2,000 minerals
- **Prerequisites:** Hull Reinforcement III complete + Titanium Composite Armor research complete
- **Effect:** Maximum standard hull tier that resists all known damage types for dropship [See Dropship](../../../Spaceship/Dropship/README.md)
- **Damage Resistance:** +50% resistance to ballistic, energy, and explosive damage
- **Heat Resistance:** +600% total thermal protection (survives in any biome indefinitely)
- **Hull Integrity Bonus:** 100% integrity with slow natural regeneration (1%/minute when not in combat)
- **See Also:** [Dropship](../../../Spaceship/Dropship/README.md), [Ship Modules](../../../ShipModules/README.md)

### Alien Shield Generator

- **Time:** 30 minutes
- **Materials:** 5,000 construction material, 3,000 dark matter crystals
- **Prerequisites:** Advanced Shield Generation complete + Center Galaxy reward (Alien tech)
- **Effect:** Immune to physical/projectile damage for dropship [See Ship Modules](../../../ShipModules/README.md)
- **Protection Type:** Energy barrier absorbs all non-physical damage at 50% effectiveness
- **Physical Damage:** 0% taken from ballistic, rocket, and melee attacks
- **Energy Damage:** 50% taken from plasma, ion, and void weapons
- **Special Feature:** Passive field — no energy cost to maintain, draws power from alien warp core
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Dropship](../../../Spaceship/Dropship/README.md)

---

## See Also

### Related Research Categories

- [Weapons Research](../Weapons-Research/README.md) — Ion Weapon Integration (Ring 3) is a prerequisite for Advanced Shield Generation; weapon damage types determine shield and armor effectiveness
- [Drives Research](../Drives-Research/README.md) — Hull integrity directly affects dropship survivability during propulsion events and crash landings
- [Energy Research](../Energy-Research/README.md) — Power Distribution I and II gate shield generation; Battery Storage sustains active shield operations
- [Vision Research](../Vision-Research/README.md) — Detection capabilities enable proactive defensive positioning and shield activation timing
- [Buildings Research](../Buildings-Research/README.md) — Forge upgrade path (Basic Forge -> Energy Lab -> Dark Matter Lab -> Void Lab) enables armor research tiers; base walls provide static defense
- [Storage Research](../Storage-Research/README.md) — Material logistics for construction material, minerals, and dark matter crystals required by armor upgrades
- [Units Research](../Units-Research/README.md) — Unit upgrade paths consume armor research tiers; Shock Troopers use void-infused armor

### Cross-References

- [Dropship Overview](../../../Spaceship/Dropship/README.md) — Ship hull integrity, crash landing state, and repair mechanics
- [Ship Modules](../../../ShipModules/README.md) — Mountable shield generators and hull reinforcement systems
- [Planet Buildings](../../../buildings/README.md) — Forge, Energy Lab, Dark Matter Lab, Void Lab research facilities
- [Upgrade System](../../../units/Upgrade-System/README.md) — Unit armor tiers, visual changes, and defense stat scaling
- [Resources](../../../resources/README.md) — Construction material, minerals, processed metals, dark matter crystals, and energy costs
- [Tech Tree Overview](../../Overview/README.md) — Ring progression, unlock conditions, and critical paths
