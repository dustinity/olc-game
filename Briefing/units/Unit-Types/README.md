# Unit Types

Soldiers, vehicles, and combat unit classifications for planetary operations.

---

## Unit Overview

Units are the military forces deployed on planet surfaces. They can be:
- **Trained** at Barracks or Factory buildings [See planet buildings](../../buildings/README.md)
- **Upgraded** through TIR-based improvement system [See upgrade system](../Upgrade-System/README.md)
- **Equipped** with swappable gear (weapons, armor, utility) [See equipment system](../Equipment-System/README.md)
- **Stored** in ship hangars when not in use [See ship modules](../../ShipModules/README.md)
- **Controlled** by champions or Command Center [See champions](../../factions/Champions/README.md)

Units marked with a faction name are exclusive to that faction at early TIR but may become available to all factions at higher TIR. Availability is noted per unit.

### Unit Categories

| Category | Examples | Training Location | Grid Size | Notes |
|----------|----------|-------------------|-----------|-------|
| Infantry | Soldiers, Medics, Scouts | Barracks | 0.5x0.5 each | Basic combat units |
| Light Vehicles | Scout cars, Jump bikes | Factory | 1x1 each | Fast, light armor |
| Heavy Vehicles | Tanks, Walkers | Factory | 2x2 each | Heavy armor, heavy weapons |
| Aerial Units | Fighters, Dropships | Airfield | 1x1-2x2 each | Flying, ignore terrain |
| Support Units | Engineers, Medics | Barracks/Factory | 0.5x0.5 each | Non-combat roles |

### Unit Capacity

Unit capacity is determined by:
- **Habitation Module** on planet: +8 base cap [See planet buildings](../../buildings/README.md)
- **Crew Quarters Small:** +4 per module [See ship modules](../../ShipModules/README.md)
- **Crew Quarters Large:** +16 per module [See ship modules](../../ShipModules/README.md)
- **Champion Endurance stat:** Each point of Endurance adds +2 unit capacity [See champions](../../factions/Champions/README.md)

---

## Infantry Units

### Basic Soldier
- **TIR:** 1 | **Faction:** All factions
- **Training Cost:** 50 construction material, 30 minerals, 10 survival
- **Training Time:** 30 seconds (Dark Realistic faction: -25%)
- **HP:** 100
- **Damage:** 15 (ballistic rifle)
- **Range:** 20m
- **Speed:** Medium
- **Notes:** Basic combat unit. Starting infantry available at game start. Can be equipped with TIR-based upgrades [See upgrade system](../Upgrade-System/README.md) and swappable gear [See equipment system](../Equipment-System/README.md)

### Heavy Gunner
- **TIR:** 2 | **Faction:** All factions (Dark Realistic specialty)
- **Training Cost:** 150 construction material, 100 minerals, 30 survival
- **Training Time:** 60 seconds
- **HP:** 150
- **Damage:** 25 (mounted turret weapon)
- **Range:** 40m
- **Speed:** Slow
- **Notes:** Stationary or slow-moving heavy weapons platform. Can mount on walls and gates [See planet buildings](../../buildings/README.md)

### Scout
- **TIR:** 1 | **Faction:** All factions
- **Training Cost:** 80 construction material, 50 minerals, 20 survival
- **Training Time:** 45 seconds
- **HP:** 75
- **Damage:** 10 (pistol)
- **Range:** 15m
- **Speed:** Fast
- **Notes:** Reconnaissance unit. Reveals map area while moving. Detection radius +50% vs normal units

### Mech Worker (Neon Punk Faction)
- **TIR:** 1 | **Faction:** Neon Punk exclusive
- **Training Cost:** 120 construction material, 80 minerals, 20 survival
- **Training Time:** 60 seconds
- **HP:** 180
- **Damage:** 12 (mining tool)
- **Range:** 5m
- **Speed:** Slow
- **Grid Size:** 1x1
- **Notes:** Heavy exoskeleton unit. Dual-purpose: mines resources at +50% output or constructs buildings at +25% speed. Neon Punk exclusive [See factions](../../factions/README.md)

### Neon Scout (Neon Punk Faction)
- **TIR:** 1 | **Faction:** Neon Punk exclusive
- **Training Cost:** 90 construction material, 60 minerals, 25 survival
- **Training Time:** 45 seconds
- **HP:** 60
- **Damage:** 8 (pistol)
- **Range:** 12m
- **Speed:** Very Fast
- **Notes:** Fast reconnaissance unit with brief cloak ability (3 seconds, 30-second cooldown). Neon Punk exclusive [See factions](../../factions/README.md)

### Circuit Breaker (Neon Punk Faction)
- **TIR:** 2 | **Faction:** Neon Punk exclusive at TIR 1, all factions from TIR 3
- **Training Cost:** 180 construction material, 120 minerals, 35 survival
- **Training Time:** 60 seconds
- **HP:** 130
- **Damage:** 30 (electric rifle)
- **Range:** 25m
- **Speed:** Medium
- **Notes:** Electric damage specialist. +50% damage vs Mechanical and Crystalloid enemies [See races](../../factions/Races/README.md). Neon Punk exclusive at TIR 1, available to all from TIR 3

### Blaster Ranger (Cartoon SciFi Faction)
- **TIR:** 1 | **Faction:** Cartoon SciFi exclusive at TIR 1, all factions from TIR 3
- **Training Cost:** 70 construction material, 50 minerals, 20 survival
- **Training Time:** 40 seconds
- **HP:** 90
- **Damage:** 18 (energy pistol)
- **Range:** 22m
- **Speed:** Fast
- **Notes:** Higher fire rate compensates for lower per-shot damage. Cartoon SciFi exclusive at TIR 1, available to all from TIR 3 [See factions](../../factions/README.md)

### Rocket Jockey (Cartoon SciFi Faction)
- **TIR:** 2 | **Faction:** Cartoon SciFi exclusive
- **Training Cost:** 250 construction material, 180 minerals, 60 fuel
- **Production Time:** 90 seconds (Factory required)
- **HP:** 160
- **Damage:** 45 (rocket launcher)
- **Range:** 50m
- **Speed:** Fast
- **Fuel Consumption:** 8 fuel/minute
- **Grid Size:** 1x1
- **Notes:** Mounted on light vehicle chassis. Area damage from rocket explosions. Cartoon SciFi exclusive [See factions](../../factions/README.md)

### Survival Scout (Bright Realistic Faction)
- **TIR:** 1 | **Faction:** Bright Realistic exclusive
- **Training Cost:** 75 construction material, 40 minerals, 30 survival
- **Training Time:** 50 seconds
- **HP:** 80
- **Damage:** 12 (hunting rifle)
- **Range:** 25m
- **Speed:** Fast
- **Notes:** -30% survival consumption while in field. Extended detection radius (+40%). Bright Realistic exclusive [See factions](../../factions/README.md)

### Shock Trooper (Dark Realistic Faction)
- **TIR:** 2
- **Training Cost:** 200 construction material, 120 minerals, 40 survival
- **Training Time:** 75 seconds
- **HP:** 200
- **Damage:** 35 (heavy ballistic rifle)
- **Range:** 30m
- **Speed:** Medium
- **Notes:** Heavily armored infantry. Dark Realistic faction exclusive at TIR 1, available to all factions from TIR 3

### Jump Trooper (Cartoon SciFi Faction)
- **TIR:** 2 | **Faction:** Cartoon SciFi exclusive at TIR 1, all factions from TIR 3
- **Training Cost:** 180 construction material, 100 minerals, 35 survival
- **Training Time:** 60 seconds
- **HP:** 120
- **Damage:** 20 (energy pistol)
- **Range:** 15m
- **Speed:** Very Fast (with jump jets)
- **Notes:** Rapid repositioning via personal jump jets. Can cross terrain obstacles instantly

### Field Medic
- **TIR:** 2 | **Faction:** All factions (Bright Realistic exclusive at TIR 1, all from TIR 2)
- **Training Cost:** 120 construction material, 80 minerals, 50 survival
- **Training Time:** 60 seconds
- **HP:** 90
- **Damage:** 8 (shotgun for self-defense)
- **Range:** 10m (healing), 15m (defense)
- **Speed:** Medium
- **Notes:** Heals nearby units for 5 HP/sec. Bright Realistic faction: healing output +50%

### Engineer
- **TIR:** 2
- **Training Cost:** 150 construction material, 100 minerals, 30 survival
- **Training Time:** 60 seconds
- **HP:** 100
- **Damage:** 10 (wrench throw)
- **Range:** 5m (repair), 10m (defense)
- **Speed:** Medium
- **Notes:** Repairs buildings at 20 HP/sec. Can construct emergency barricades. Architect champion synergy [See champions](../../factions/Champions/README.md)

---

## Light Vehicle Units

### Scout Car
- **TIR:** 1 | **Faction:** All factions
- **Training Cost:** 200 construction material, 150 minerals, 50 fuel
- **Production Time:** 90 seconds (Factory required) [See planet buildings](../../buildings/README.md)
- **HP:** 200
- **Damage:** 20 (mounted machine gun)
- **Range:** 25m
- **Speed:** Very Fast
- **Fuel Consumption:** 5 fuel/minute
- **Notes:** Fast reconnaissance vehicle. Can mount light turret [See weapon types](../../weapons/Weapon-Types/README.md)

### Jump Bike
- **TIR:** 1 | **Faction:** All factions (Cartoon SciFi specialty)
- **Training Cost:** 150 construction material, 100 minerals, 40 fuel
- **Production Time:** 60 seconds
- **HP:** 120
- **Damage:** 15 (mounted pistol)
- **Range:** 20m
- **Speed:** Extremely Fast
- **Fuel Consumption:** 3 fuel/minute
- **Notes:** Fastest ground unit. Fragile but excellent for hit-and-run tactics

### Light APC (Armored Personnel Carrier)
- **TIR:** 2 | **Faction:** All factions
- **Training Cost:** 400 construction material, 300 minerals, 100 fuel
- **Production Time:** 120 seconds
- **HP:** 500
- **Damage:** 30 (mounted turret)
- **Range:** 30m
- **Speed:** Fast
- **Fuel Consumption:** 10 fuel/minute
- **Notes:** Can carry 4 infantry units inside. Protected transport to battlefield

---

## Heavy Vehicle Units

### Tank
- **TIR:** 2 | **Faction:** All factions (Dark Realistic specialty — +20% HP from faction bonus)
- **Training Cost:** 800 construction material, 600 minerals, 200 fuel
- **Production Time:** 180 seconds (Factory required)
- **HP:** 1,000
- **Damage:** 60 (main cannon)
- **Range:** 60m
- **Speed:** Slow
- **Fuel Consumption:** 15 fuel/minute
- **Notes:** Heavy armor and firepower. Can destroy buildings in 3 shots. Requires trained crew

### Walker (Two-Legged Assault Mech)
- **TIR:** 3 | **Faction:** All factions
- **Training Cost:** 1,200 construction material, 800 minerals, 300 fuel
- **Production Time:** 240 seconds
- **HP:** 1,500
- **Damage:** 80 (heavy cannon)
- **Range:** 50m
- **Speed:** Medium
- **Fuel Consumption:** 20 fuel/minute
- **Notes:** Can cross all terrain types including water. High profile makes it easy target for anti-air

### Heavy Transport
- **TIR:** 2 | **Faction:** All factions (Bright Realistic specialty)
- **Training Cost:** 600 construction material, 400 minerals, 150 fuel
- **Production Time:** 150 seconds
- **HP:** 800
- **Damage:** 15 (defensive turret)
- **Range:** 20m
- **Speed:** Medium
- **Fuel Consumption:** 12 fuel/minute
- **Notes:** Can carry resources or units. Essential for logistics between base and frontlines

---

## Aerial Units [See planet buildings](../../buildings/README.md)

### Scout Drone
- **TIR:** 1 | **Faction:** All factions
- **Training Cost:** 300 construction material, 200 minerals, 50 fuel
- **Production Time:** 60 seconds (Airfield required)
- **HP:** 50
- **Damage:** 5 (small explosive)
- **Range:** N/A (flying)
- **Speed:** Very Fast
- **Fuel Consumption:** 2 fuel/minute
- **Notes:** Flying reconnaissance. Immune to ground terrain effects and traps. Reveals large map area

### Fighter Jet
- **TIR:** 3 | **Faction:** All factions (Cartoon SciFi specialty — +30% fire rate)
- **Training Cost:** 1,500 construction material, 1,000 minerals, 400 fuel
- **Production Time:** 300 seconds (Airfield required)
- **HP:** 400
- **Damage:** 100 (cannon + missiles)
- **Range:** 200m (flying)
- **Speed:** Extremely Fast
- **Fuel Consumption:** 30 fuel/minute
- **Notes:** Air superiority unit. Can dogfight enemy aircraft. Ground attack mode for heavy damage

### Dropship (Combat)
- **TIR:** 2 | **Faction:** All factions
- **Training Cost:** 1,000 construction material, 600 minerals, 300 fuel
- **Production Time:** 240 seconds (Airfield required)
- **HP:** 800
- **Damage:** 40 (defensive turrets)
- **Range:** N/A (flying)
- **Speed:** Fast
- **Fuel Consumption:** 20 fuel/minute
- **Notes:** Transport up to 8 units. Essential for rapid deployment across planet [See ship modules](../../ShipModules/README.md)

---

## Support and Special Units

### Demolitions Expert
- **TIR:** 2 | **Faction:** All factions (Dark Realistic specialty — +30% explosive damage)
- **Training Cost:** 200 construction material, 150 minerals, 40 survival
- **Training Time:** 90 seconds
- **HP:** 120
- **Damage:** 50 (C4 explosive)
- **Range:** 10m (placement), 20m (detonation)
- **Speed:** Medium
- **Notes:** Places remote explosives. Demolitions champion: damage +30% [See champions](../../factions/Champions/README.md)

### Sniper
- **TIR:** 2 | **Faction:** All factions (Dark Realistic specialty — +20% weapon range)
- **Training Cost:** 180 construction material, 200 minerals, 30 survival
- **Training Time:** 90 seconds
- **HP:** 80
- **Damage:** 40 (precision rifle)
- **Range:** 80m
- **Speed:** Slow (prefers stationary position)
- **Notes:** Long-range elimination. Smoke Screen ability: -30% enemy accuracy in smoke [See champions](../../factions/Champions/README.md)

### Engineer Sapper
- **TIR:** 2 | **Faction:** All factions (Bright Realistic specialty)
- **Training Cost:** 150 construction material, 180 minerals, 30 survival
- **Training Time:** 75 seconds
- **HP:** 100
- **Damage:** 25 (mines and traps)
- **Range:** 15m (mine placement)
- **Speed:** Medium
- **Notes:** Deploys landmines and tripwires. Can repair walls at 30 HP/sec

### Heavy Mech (TIR 4)
- **TIR:** 4 | **Faction:** All factions
- **Training Cost:** 2,500 construction material, 1,500 minerals, 500 fuel
- **Production Time:** 480 seconds (Factory or Assembly Plant)
- **HP:** 3,000
- **Damage:** 150 (heavy cannon + rocket pods)
- **Range:** 70m
- **Speed:** Slow
- **Fuel Consumption:** 40 fuel/minute
- **Notes:** Ultimate ground combat unit. Requires trained crew of 2. Can breach reinforced walls

---

## Unit Upgrade Integration [See upgrade system](../Upgrade-System/README.md)

All units can receive upgrades through the TIR-based upgrade system:

| Upgrade Type | Effect | Requirement |
|--------------|--------|-------------|
| Armor Upgrade (TIR 1→2) | +50% HP, +10 defense | Forge research + materials |
| Armor Upgrade (TIR 2→3) | +100% HP, +25 defense | Energy Lab + titanium |
| Weapon Upgrade (TIR 1→2) | +30% damage | Forge research + minerals |
| Weapon Upgrade (TIR 2→3) | +60% damage | Energy Lab + processed metals |
| Vision Upgrade (TIR 1→2) | +50% detection range | Radar components |
| Vision Upgrade (TIR 2→3) | +100% detection, thermal | Advanced sensors |

### Unit Group Management

- **Command Center** unlocks unit control groups [See planet buildings](../../buildings/README.md)
- **Champion Intelligence stat:** Each point adds +1 control group slot [See champions](../../factions/Champions/README.md)
- **Control group hotkeys:** Assign units to numbered groups for rapid deployment

---

## Unit Biome Modifiers

Units operate in all biomes but receive environmental modifiers:

| Biome | Movement Modifier | Combat Modifier | Special Effect |
|-------|------------------|-----------------|----------------|
| Desert | -10% | None | Overheating after 5 min combat (-20% damage) |
| Jungle | -15% | +20% cover defense | Dense vegetation slows targeting |
| Ice | -20% | -10% accuracy | Slippery terrain, braking distance longer |
| Swamp | -25% | None | Toxic spore events every 30 min |
| Rocky | +10% cover | +10% defense | Natural terrain provides cover |
| Water (amphibious only) | -50% (non-amphibious) | None | Non-amphibious units cannot enter |

---

## See Also

- [Unit upgrade system](../Upgrade-System/README.md)
- [Equipment system — swappable gear per unit type](../Equipment-System/README.md)
- [Champion abilities and unit coordination](../../factions/Champions/README.md)
- [Faction unit rosters and exclusive units](../../factions/README.md)
- [Building training facilities](../../buildings/README.md)
- [Ship hangar storage](../../ShipModules/README.md)
- [Race families and counter tactics](../../factions/Races/README.md)
