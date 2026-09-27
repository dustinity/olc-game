# Weapons Research Category

Comprehensive breakdown of all weapons research topics with prerequisites, material costs, and cross-references. Covers ballistic, energy, rocket, torpedo, ion, void, and alien weapon systems — from basic rifles to gravity well emitters.

| Ring | Lab | Topics |
|------|-----|--------|
| Core | N/A | Basic Ballistic Rifle (starting) |
| Ring 1 | Basic Forge | Ballistic Turret Mounting, Ammo Crafting I, Rocket Bay Basic |
| Ring 2 | Energy Lab | Plasma Cannon Mounting, Ammo Crafting II, Rocket Bay Advanced |
| Ring 3 | Dark Matter Lab | Torpedo Launcher, Precision Targeting System, Ion Weapon Integration |
| Outer Ring | Void Lab | Void Beam, Void Core Weaponry (Unit Upgrade Path), Gravity Well Emitter (Alien Tech) |

---

## Core -- Starting (No Research Required)

### Basic Ballistic Rifle

- **Time:** Auto-unlock at game start
- **Materials:** None
- **Effect:** Standard infantry weapon, 8 damage per hit, 15m range
- **Ammo:** Unlimited (standard rifle rounds)
- **See Also:** [Weapon Types](../../../weapons/Weapon-Types/README.md), [Units](../../../units/README.md)

---

## Ring 1 -- Basic Forge Research

### Ballistic Turret Mounting

- **Time:** 2 minutes
- **Materials:** 100 construction material, 80 minerals
- **Prerequisites:** None (Ring 1 available at start)
- **Effect:** Enable turret installation on dropship external mounts and planet base walls
- **Mount Points:** 2x external on dropship [See Dropship](../../../Spaceship/Dropship/README.md), unlimited on planet bases
- **Damage Output:** 15 per hit, 30m range
- **Energy Cost:** 5 energy/turn while active
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Weapon Types](../../../weapons/Weapon-Types/README.md)

### Ammo Crafting I

- **Time:** 3 minutes
- **Materials:** 50 minerals, 30 energy
- **Prerequisites:** None (Ring 1 available at start)
- **Effect:** Unlock standard ammunition production at forge bench
- **Crafting Output:** 100 rifle rounds per craft cycle
- **Material Cost per Cycle:** 20 minerals, 10 energy
- **See Also:** [Resources](../../../resources/README.md), [Planet Buildings](../../../buildings/README.md)

### Rocket Bay Basic

- **Time:** 5 minutes
- **Materials:** 200 construction material, 150 minerals
- **Prerequisites:** Ballistic Turret Mounting complete
- **Effect:** Enable small rocket bay for ship and planet base turrets
- **Rocket Types Available:** Standard HE, Incendiary, Armor-Piercing
- **Ammo Capacity per Rocket:** 3 shots before reload
- **Reload Time:** 10 seconds (planet base), 30 seconds (ship)
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Weapon Types](../../../weapons/Weapon-Types/README.md)

---

## Ring 2 -- Energy Lab Research

### Plasma Cannon Mounting

- **Time:** 8 minutes
- **Materials:** 400 construction material, 300 minerals
- **Prerequisites:** Ballistic Turret Mounting + Ammo Crafting I complete
- **Effect:** Enable energy weapon deployment on ship and planet bases
- **Damage Output:** 25 per hit (vs physical armor), 10 per hit (vs energy shields)
- **Range:** 35m (+5m over ballistic)
- **Energy Cost:** 15 energy/turn while active
- **Overheat Time:** 45 seconds continuous fire, then 15 second cool-down
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Weapon Types](../../../weapons/Weapon-Types/README.md)

### Ammo Crafting II -- Advanced Rounds

- **Time:** 5 minutes
- **Materials:** 200 processed metals, 100 energy
- **Prerequisites:** Ammo Crafting I complete
- **Effect:** Unlock armor-piercing and incendiary rounds for ballistic weapons
- **Armor-Piercing Output:** +30% damage vs armored targets, -10% vs unarmored
- **Incendiary Output:** +20% base damage + 5 damage/turn burn for 10 turns
- **Material Cost per Cycle:** 80 processed metals, 40 energy
- **See Also:** [Resources](../../../resources/README.md), [Upgrade System](../../../units/Upgrade-System/README.md)

### Rocket Bay Advanced

- **Time:** 10 minutes
- **Materials:** 600 construction material, 400 minerals
- **Prerequisites:** Rocket Bay Basic complete
- **Effect:** Enable large rocket bay with warhead variety for ship and bases
- **Additional Warheads:** Cluster (anti-infantry), EMP (anti-electronics), Thermobaric (area denial)
- **Ammo Capacity per Rocket:** 6 shots before reload (+100% over basic)
- **Reload Time:** 8 seconds (planet base), 20 seconds (ship) (-33% over basic)
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Weapon Types](../../../weapons/Weapon-Types/README.md)

---

## Ring 3 -- Dark Matter Lab Research

### Torpedo Launcher

- **Time:** 15 minutes
- **Materials:** 800 construction material, 600 minerals, 200 energy
- **Prerequisites:** Rocket Bay Advanced complete + Plasma Cannon Mounting
- **Effect:** Homing multi-target weapon system for ship and planet bases
- **Damage Output:** 40 per hit (single target), 15 per hit (splash damage)
- **Range:** 80m (+45m over advanced rockets)
- **Homing Capability:** Auto-acquires up to 3 targets within range
- **Ammo Capacity:** 4 torpedoes before reload
- **Reload Time:** 20 seconds (planet base), 45 seconds (ship)
- **Energy Cost:** 25 energy/turn while active
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Weapon Types](../../../weapons/Weapon-Types/README.md)

### Precision Targeting System

- **Time:** 12 minutes
- **Materials:** 600 construction material, 400 minerals, 200 energy
- **Prerequisites:** Ammo Crafting II complete + Laser Targeting System (Vision category)
- **Effect:** +100% damage output, +40% range, +25% accuracy for all weapon systems [See Upgrade System](../../../units/Upgrade-System/README.md)
- **Applies To:** All Ring 3+ weapons including Torpedo Launcher and Plasma Cannon
- **Visual Change:** Weapon mounts gain targeting sensors with laser designator
- **See Also:** [Upgrade System](../../../units/Upgrade-System/README.md), [Vision Advanced Cluster](../Vision-Research/README.md#ring-2-energy-lab-research)

### Ion Weapon Integration

- **Time:** 15 minutes
- **Materials:** 700 construction material, 500 minerals, 300 energy
- **Prerequisites:** Plasma Cannon Mounting + Precision Targeting System complete
- **Effect:** EMP and ion storm capabilities for ship defense and planet base offense
- **EMP Burst:** Disables all electronic systems in 20m radius for 10 seconds
- **Ion Storm:** Continuous field that degrades enemy shields at rate of 5 HP/second within 40m radius
- **Energy Cost:** 30 energy/turn while active (ion storm mode)
- **Cooldown:** 60 seconds between EMP bursts
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Weapon Types](../../../weapons/Weapon-Types/README.md)

---

## Outer Ring -- Void Lab Research

### Void Beam

- **Time:** 20 minutes
- **Materials:** 1,500 construction material, 1,000 minerals, 500 energy
- **Prerequisites:** Torpedo Launcher + Precision Targeting System complete
- **Effect:** Extreme damage weapon that ignores all armor types [See Weapon Types](../../../weapons/Weapon-Types/README.md)
- **Damage Output:** 80 per hit (ignores armor), 40 vs shields
- **Range:** 120m (+50m over torpedoes)
- **Beam Type:** Continuous energy beam, damage scales with hold time
- **Max Hold Time:** 8 seconds before 30 second cool-down
- **Energy Cost:** 50 energy/turn while firing
- **Visual Effect:** Purple-black spatial distortion beam [See Upgrade System](../../../units/Upgrade-System/README.md)
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Weapon Types](../../../weapons/Weapon-Types/README.md)

### Void Core Weaponry (Unit Upgrade Path)

- **Time:** 25 minutes
- **Materials:** 2,000 construction material, 1,500 dark matter crystals
- **Prerequisites:** Precision Targeting System + Torpedo Launcher complete
- **Effect:** +150% damage output, +60% range, ignores 30% of enemy armor [See Upgrade System](../../../units/Upgrade-System/README.md)
- **Applies To:** All TIR 5 units with weapon upgrade slots
- **Visual Change:** Weapons emit void energy with purple-black glow and spatial distortion effect
- **See Also:** [Upgrade System](../../../units/Upgrade-System/README.md), [Weapon Types](../../../weapons/Weapon-Types/README.md)

### Gravity Well Emitter (Alien Tech)

- **Time:** 30 minutes
- **Materials:** 2,500 construction material, 2,000 dark matter crystals
- **Prerequisites:** Void Beam + Ion Weapon Integration complete + Center Galaxy reward
- **Effect:** Ultimate area control weapon creating implosion damage field [See Weapon Types](../../../weapons/Weapon-Types/README.md)
- **Gravity Well Radius:** 50m from emitter center
- **Damage Output:** 60 per second to all units within gravity well (both friendly and enemy)
- **Pull Effect:** Draws all units toward center at 3m/second
- **Duration:** 15 seconds before 90 second cool-down
- **Energy Cost:** 40 energy/turn while active
- **See Also:** [Ship Modules](../../../ShipModules/README.md), [Weapon Types](../../../weapons/Weapon-Types/README.md)

---

## See Also

### Related Research Categories

- [Armor Research](../Armor-Research/README.md) — Hull and shield upgrades that protect weapon platforms; Ion Weapon Integration is a prerequisite for Advanced Shield Generation
- [Drives Research](../Drives-Research/README.md) — Propulsion systems determine weapon deployment range and positioning options
- [Energy Research](../Energy-Research/README.md) — Power Distribution gates all energy weapons; Battery Storage sustains prolonged combat operations
- [Vision Research](../Vision-Research/README.md) — Laser Targeting System (Ring 2) is a prerequisite for Precision Targeting System; Temporal Radar provides predictive firing solutions
- [Buildings Research](../Buildings-Research/README.md) — Factories and Assembly Plants produce weapon systems; Orbital Strike Beacon delivers planet-wide bombardment
- [Storage Research](../Storage-Research/README.md) — Resource logistics for ammunition, materials, and energy reserves required by weapons
- [Units Research](../Units-Research/README.md) — Void Core Weaponry upgrades apply to unit weapon slots; Shock Troopers use void beam weapons

### Cross-References

- [Weapon Types](../../../weapons/Weapon-Types/README.md) — Damage types, armor interactions, and weapon classifications
- [Ship Modules](../../../ShipModules/README.md) — Mountable weapon systems for the dropship
- [Planet Buildings](../../../buildings/README.md) — Base turrets, factories, and production facilities
- [Upgrade System](../../../units/Upgrade-System/README.md) — Unit weapon upgrade slots and visual changes
- [Resources](../../../resources/README.md) — Materials, minerals, processed metals, dark matter crystals, and energy costs
- [Tech Tree Overview](../../Overview/README.md) — Ring progression, unlock conditions, and critical paths
