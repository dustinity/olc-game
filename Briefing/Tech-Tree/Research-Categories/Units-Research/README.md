# Units Research Category

Unit production, control, formation, and elite training technologies that expand the player's ground and aerial combat capabilities — from basic infantry to heavy mechs and shock troopers.

| Ring | Lab | Topics |
|------|-----|--------|
| Core | N/A | Basic Soldier Training (starting) |
| Ring 1 | Basic Forge | Unit Control Groups I, Basic Formation Commands |
| Ring 2 | Energy Lab | Heavy Vehicle Production, Unit Control Groups II, Aerial Unit Deployment |
| Ring 3 | Dark Matter Lab | Heavy Mech Production, Unit Control Groups III, Elite Squad Training |

---

## Core -- Starting (No Research Required)

### Basic Soldier Training

- **Time:** Auto-unlock at game start
- **Materials:** None
- **Effect:** 10 cryo-stasis soldiers available from dropship [See Dropship](../../../Spaceship/Dropship/README.md)
- **Soldier Stats:** 50 HP, 8 damage output, 15m range, basic ballistic rifle
- **Training Time:** Soldiers wake from cryo-stasis over 10 minutes (1 soldier per minute)
- **Control Limit:** Can directly control up to 4 soldiers simultaneously at game start
- **See Also:** [Dropship](../../../Spaceship/Dropship/README.md), [Upgrade System](../../../units/Upgrade-System/README.md)

---

## Ring 1 -- Basic Forge Research

### Unit Control Groups I

- **Time:** 3 minutes
- **Materials:** Command Center building required (Basic Wall and Gate Placement + Basic Power Grid)
- **Prerequisites:** None (Ring 1 available at start, Command Center auto-unlocks with buildings)
- **Effect:** Enable numbered unit group assignment for simultaneous control [See Units](../../../units/Unit-Types/README.md)
- **Control Groups Available:** 4 groups (Group 1-4)
- **Assignment Method:** Select units → press number key to assign to group
- **Control Command:** Press number key to select entire group and issue unified commands
- **See Also:** [Unit Types](../../../units/Unit-Types/README.md), [Planet Buildings](../../../buildings/README.md)

### Basic Formation Commands

- **Time:** 3 minutes
- **Materials:** None (auto-unlock with Unit Control Groups I)
- **Prerequisites:** Unit Control Groups I complete
- **Effect:** Squad movement and combat formations for all controlled units [See Units](../../../units/Unit-Types/README.md)
- **Available Formations:** Line (front-line engagement), Column (rapid movement), Diamond (balanced defense/offense), V-Shape (flanking maneuver), Scatter (area coverage)
- **Formation Bonus:** Each formation provides tactical bonuses in combat (Line: +10% damage front-row, Column: +20% movement speed, etc.)
- **See Also:** [Unit Types](../../../units/Unit-Types/README.md), [Upgrade System](../../../units/Upgrade-System/README.md)

---

## Ring 2 -- Energy Lab Research

### Heavy Vehicle Production

- **Time:** 15 minutes
- **Materials:** 800 construction material, 600 minerals, 200 fuel
- **Prerequisites:** Factory Construction + Unit Control Groups I complete
- **Effect:** Tank and walker training at factory [See Units](../../../units/Unit-Types/README.md)
- **Available Vehicles:** Light Tank (ballistic cannon), Heavy Walker (plasma cannon + missile launcher)
- **Production Time:** 10 minutes per light tank, 15 minutes per heavy walker (reduced by Assembly Plant if available)
- **Size Required:** Factory building [See Planet Buildings](../../../buildings/README.md)
- **Worker Requirement:** 4 workers minimum for vehicle production line
- **Fuel Cost per Vehicle:** 200 fuel (loaded as starting fuel supply)
- **See Also:** [Unit Types](../../../units/Unit-Types/README.md), [Planet Buildings](../../../buildings/README.md)

### Unit Control Groups II

- **Time:** 5 minutes
- **Materials:** None (knowledge only, requires Intelligence stat ≥ 3 on active champion)
- **Prerequisites:** Unit Control Groups I complete + Champion Intelligence requirement
- **Effect:** +3 additional control group slots (total 7 groups: 1-7) [See Champions](../../../factions/Champions/README.md)
- **Intelligence Scaling:** Each point of Intelligence above 3 adds +1 additional group slot (max +3 at Int 6)
- **Control Groups Available:** 7+ groups depending on champion Intelligence stat
- **See Also:** [Champions](../../../factions/Champions/README.md), [Unit Types](../../../units/Unit-Types/README.md)

### Aerial Unit Deployment

- **Time:** 10 minutes
- **Materials:** Airfield building required, 400 construction material, 300 minerals
- **Prerequisites:** Heavy Vehicle Production + Satellite Deployment Bay (Vision category) started
- **Effect:** Scout drone and fighter jet training at airfield [See Planet Buildings](../../../buildings/README.md)
- **Available Aerial Units:** Scout Drone (reconnaissance, 5m range sensor), Fighter Jet (combat, plasma cannon + 4 missiles)
- **Production Time:** 5 minutes per scout drone, 12 minutes per fighter jet
- **Size Required:** Airfield building [See Planet Buildings](../../../buildings/README.md)
- **Worker Requirement:** 6 workers minimum for aerial unit production line
- **Fuel Cost per Unit:** Scout Drone — 50 fuel, Fighter Jet — 200 fuel (loaded as starting supply)
- **See Also:** [Planet Buildings](../../../buildings/README.md), [Unit Types](../../../units/Unit-Types/README.md)

---

## Ring 3 -- Dark Matter Lab Research

### Heavy Mech Production

- **Time:** 25 minutes
- **Materials:** 2,500 construction material, 1,500 minerals, 500 fuel
- **Prerequisites:** Heavy Vehicle Production + Assembly Plant complete
- **Effect:** Ultimate ground combat unit production [See Units](../../../units/Unit-Types/README.md)
- **Available Mech:** Heavy Combat Mech (dual plasma cannons + missile pod + energy shield generator)
- **Production Time:** 20 minutes per heavy mech (reduced to ~7 minutes with Assembly Plant 3x multiplier)
- **Size Required:** Assembly Plant building [See Planet Buildings](../../../buildings/README.md)
- **Worker Requirement:** 12 workers minimum for mech production line
- **Fuel Cost per Mech:** 500 fuel (loaded as starting fuel supply)
- **Mech Stats:** 500 HP, 60 damage output per cannon (120 total), 80m range, energy shield absorbs 100 damage/second
- **See Also:** [Unit Types](../../../units/Unit-Types/README.md), [Planet Buildings](../../../buildings/README.md)

### Unit Control Groups III

- **Time:** 8 minutes
- **Materials:** None (knowledge only, requires Intelligence stat ≥ 6 on active champion)
- **Prerequisites:** Unit Control Groups II complete + Champion Intelligence requirement
- **Effect:** +5 additional control group slots (total 12+ groups: 1-12+) [See Champions](../../../factions/Champions/README.md)
- **Intelligence Scaling:** Each point of Intelligence above 6 adds +1 additional group slot (max +5 at Int 11, effectively unlimited at Int 10+)
- **Control Groups Available:** 12+ groups depending on champion Intelligence stat
- **See Also:** [Champions](../../../factions/Champions/README.md), [Unit Types](../../../units/Unit-Types/README.md)

### Elite Squad Training

- **Time:** 15 minutes
- **Materials:** 1,000 construction material, 800 minerals, 300 survival
- **Prerequisites:** Heavy Mech Production + Unit Control Groups III complete
- **Effect:** Shock Trooper and elite variant training for infantry units [See Upgrade System](../../../units/Upgrade-System/README.md)
- **Available Elite Units:** Shock Trooper (void-infused armor + void beam weapon), Elite Sniper (precision targeting + thermal imaging)
- **Training Time:** 8 minutes per shock trooper, 10 minutes per elite sniper
- **Size Required:** Barracks building with elite training module [See Planet Buildings](../../../buildings/README.md)
- **Worker Requirement:** 6 workers minimum for elite squad training
- **Survival Cost per Unit:** 30 survival (food/oxygen supplies for elite unit sustenance)
- **Elite Unit Stats:** Shock Trooper — 200 HP, 40 damage output, void-infused armor (+350% total HP), void beam weapon. Elite Sniper — 80 HP, 100 damage per shot (ignores light armor), 150m range
- **See Also:** [Unit Types](../../../units/Unit-Types/README.md), [Planet Buildings](../../../buildings/README.md)

---

## See Also

### Related Research Categories

- [Weapons Research](../Weapons-Research/README.md) — Void Core Weaponry upgrades apply to unit weapon slots; Shock Troopers use void beam weapons
- [Armor Research](../Armor-Research/README.md) — Hull and shield upgrades protect units; energy shields on mechs tie into shield generation research
- [Drives Research](../Drives-Research/README.md) — Propulsion systems determine unit mobility; fuel processing affects vehicle and aerial unit costs
- [Energy Research](../Energy-Research/README.md) — Power Distribution gates mech energy shields and plasma weapon operation on vehicles
- [Vision Research](../Vision-Research/README.md) — Satellite Deployment Bay (Ring 3) is a prerequisite for Aerial Unit Deployment; Tactical AI Assistant enhances unit targeting
- [Buildings Research](../Buildings-Research/README.md) — Factories, Assembly Plants, Airfields, and Barracks are required for unit production across all rings
- [Storage Research](../Storage-Research/README.md) — Resource logistics for construction material, minerals, fuel, and survival supplies consumed by unit training

### Cross-References

- [Unit Types](../../../units/Unit-Types/README.md) — Infantry, vehicles, aerial units, mechs, and elite variants
- [Upgrade System](../../../units/Upgrade-System/README.md) — Unit upgrade slots, visual changes, and stat scaling
- [Equipment System](../../../units/Equipment-System/README.md) — Weapons, armor, and gear that units carry
- [Champions](../../../factions/Champions/README.md) — Intelligence stat gating for control group expansions
- [Planet Buildings](../../../buildings/README.md) — Factories, Assembly Plants, Airfields, Barracks, and Command Centers
- [Dropship](../../../Spaceship/Dropship/README.md) — Starting soldier deployment via cryo-stasis pods
- [Resources](../../../resources/README.md) — Construction material, minerals, fuel, and survival costs for unit production
- [Tech Tree Overview](../../Overview/README.md) — Ring progression, unlock conditions, and critical paths across all categories
