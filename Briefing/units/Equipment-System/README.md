# Equipment System — Modular Unit Gear, Slots, and Progression Layers

Swappable equipment system that keeps early units relevant through late game. Three layers of progression: faction-wide tech, pool gear, and unique loot.

---

## Equipment Overview

Units are composed of a **chassis** (the unit type itself) and **equipment slots** (swappable gear). The chassis determines base stats, slot count, and what categories of equipment the unit can carry. Equipment provides additive stat bonuses, special abilities, and role changes.

### Why Equipment Matters

- A TIR 1 soldier with a legendary dungeon-found weapon out-damages a base Heavy Mech
- Early units stay in the army because their gear scales, not because their chassis improves
- Combined arms is king — scouts reveal maps, medics heal, mechs tank. No single unit type dominates
- Gear auto-recalls on death — no inventory loss, risk is time not equipment

### Three Layers of Progression

| Layer | Scope | How You Get It | Example |
|-------|-------|----------------|---------|
| **Layer 1: Faction-Wide Tech** | All units of that type | Researched at labs (Forge, Energy Lab, Dark Matter Lab, Void Lab) | "Plasma Rifle Mk.I" researched → all newly trained soldiers issue with plasma rifles |
| **Layer 2: Pool Equipment** | Assignable to any compatible unit | Crafted at buildings or purchased in bulk from traders | Acquire "3x Medic Packs" → assign to a squad of soldiers, turning them into medics |
| **Layer 3: Unique Loot** | Single copy — one unit only | Dungeon boss drops, rare finds, Ancient Ruin Keeper rewards | "Void-Forged Rifle — +200% damage, purple glow" → slot onto your champion |

### How the Layers Interact

```
Soldier [TIR 1 chassis] — base HP 100, base damage 15

 Layer 1 (Faction-Wide Tech): "Plasma Rifle Mk.I" researched
   → Default weapon is now Plasma Rifle (+30% damage over ballistic)
   → ALL soldiers get this automatically on training

 Layer 2 (Pool Equipment): Player crafts 2x Reinforced Plating
   → Equips 2 soldiers with reinforced plating (+50% HP, +10 Defense)
   → New trainees auto-get plating if pool has spare

 Layer 3 (Unique Loot): Dungeon boss drops "Widow's Kiss" shotgun
   → Only ONE soldier can carry it. That soldier becomes special.
   → If that soldier dies, Widow's Kiss returns to equipment pool.
```

---

## Equipment Slots Per Unit Type

Each chassis has a fixed number of slots by category. Slots are always filled — empty slots use the chassis default (e.g., "standard-issue rifle").

### Infantry Units

| Slot Category | Count | What Fits | Default (Empty) |
|---------------|-------|-----------|-----------------|
| Weapon | 1 | Any handheld weapon | Ballistic Rifle (TIR 1) |
| Armor | 1 | Vests, plating, medic packs, shields | Standard Vest (+0 HP) |
| Utility | 0 — 1* | Goggles, comms, jump jets | None |

*\*Utility slot unlocked at TIR 3+ or via champion promotion.*

### Light Vehicles

| Slot Category | Count | What Fits | Default (Empty) |
|---------------|-------|-----------|-----------------|
| Weapon | 1 | Mounted turrets, light cannons | Machine Gun (TIR 1) |
| Armor | 1 | Hull plating, reactive armor | Standard Hull (+0 HP) |
| Utility | 1 | Sensors, comms, smoke launchers | Basic Radar |

### Heavy Vehicles

| Slot Category | Count | What Fits | Default (Empty) |
|---------------|-------|-----------|-----------------|
| Weapon — Main | 1 | Main cannons, heavy emitters | Basic Cannon (TIR 2) |
| Weapon — Secondary | 1 | Coaxial guns, missile pods | None |
| Armor | 2 | Hull plating + turret armor | Reinforced Hull (+0 HP each) |
| Utility | 1 | Targeting computers, AI assistants | Standard HUD |

### Aerial Units

| Slot Category | Count | What Fits | Default (Empty) |
|---------------|-------|-----------|-----------------|
| Weapon — Primary | 1 | Cannons, missile racks | Basic Cannon (TIR 1) |
| Weapon — Secondary | 1 | Bombs, torpedoes, secondary guns | None |
| Armor | 1 | Lightweight flight armor | Standard Skin (+0 HP) |
| Utility | 1 | Navigation, targeting, ECM | Flight Computer |

### Support Units

| Slot Category | Count | What Fits | Default (Empty) |
|---------------|-------|-----------|-----------------|
| Weapon | 0 — 1 | Sidearm or tool | Wrench / Pistol |
| Armor | 0 — 1 | Light vest or none | Standard Vest (+0 HP) |
| Utility | 1 | Medical kit, engineering tools, mine detector | Basic Tool Kit |

---

## Gear Rarity Tiers

Equipment items are classified by rarity. Common and Uncommon gear is pool equipment (multiple copies). Rare and above can be unique single-copy items.

| Rarity | Color Code | Stat Range | Pool or Unique? | Source |
|--------|-----------|------------|-----------------|--------|
| **Common** | White | +0% to +15% over default | Pool | Starting gear, basic crafting, Tiny/Small dungeons |
| **Uncommon** | Green | +15% to +30% over default | Pool | Medium dungeons, trader purchases |
| **Rare** | Blue | +30% to +60% over default | Pool or Unique (20% chance) | Large dungeons, Energy Lab crafting |
| **Epic** | Purple | +60% to +100% over default | Always Unique | Alien Structure dungeons, Dark Matter Lab crafting |
| **Legendary** | Orange | +100% to +200% over default | Always Unique | Inner Ring Citadel bosses, Ancient Ruin Keepers |
| **Alien** | Red-Gold | +150% to +300% over default | Always Unique | Center galaxy final boss, Alien Artifact Decoder |

### Rarity Mechanics

- **Pool items (Common–Rare):** Can be crafted in multiples. No name. Stats are fixed per tier.
- **Unique items (Rare+):** Single copy with a proper name. May have special effects beyond raw stats (e.g., "Widow's Kiss — kills apply 3-second fear aura to nearby enemies").
- **Faction-exclusive gear:** Some unique items can only be equipped by units of a specific faction. Marked with the faction icon in inventory.

---

## Equipment Categories

### Weapons

Weapons replace the default weapon in the Weapon slot. Damage values are additive bonuses over the chassis base damage.

| Category | Examples | Slot Type | Typical Users |
|----------|----------|-----------|---------------|
| **Ballistic Rifles** | Standard Rifle, Heavy Ballistic Rifle, Widow's Kiss (Legendary shotgun) | Infantry Weapon | All infantry |
| **Energy Weapons** | Plasma Pistol, Plasma Cannon Mk.II, Void Beam Emitter | Infantry / Vehicle Weapon | TIR 2+ units |
| **Heavy Cannons** | Main Battle Cannon, Rotary Cannon, Gravity Well Emitter | Heavy Vehicle Main | Tanks, Walkers, Mechs |
| **Missile Systems** | Rocket Pods, Homing Torpedoes, Void Missile Rack | Secondary / Aerial | All vehicles, aerial |
| **Specialist Weapons** | Sniper Rifle (80m range), Demolitions C4, Flamethrower | Infantry Weapon | Specialist roles |

### Armor

Armor replaces the default armor in the Armor slot. HP bonuses are percentage-based over chassis base HP.

| Category | Examples | Slot Type | Typical Users |
|----------|----------|-----------|---------------|
| **Standard Vests** | Reinforced Plating, Titanium Composite Vest | Infantry Armor | All infantry |
| **Medic Packs** | Field Medic Kit, Advanced Triage Pack (+healing ability) | Infantry Armor (role change) | Converts soldier to medic |
| **Hull Plating** | Reactive Armor, Energy-Infused Hull, Void-Infused Armor | Vehicle Armor | All vehicles |
| **Shield Generators** | Personal Shield (TIR 3), Alien Shield Generator (Legendary) | Any Armor slot | Champions, elite units |

### Utility

Utility items provide non-combat bonuses: detection range, movement speed, special abilities.

| Category | Examples | Slot Type | Effect |
|----------|----------|-----------|--------|
| **Sensors** | Thermal Goggles, Night Vision Array, Tactical AI Assistant | Any Utility slot | +detection range, ambush resistance |
| **Mobility** | Jump Jets (Cartoon SciFi exclusive), Sprint Boots | Infantry / Light Vehicle Utility | Movement speed bonus, terrain crossing |
| **Comms** | Enhanced Comms Array, Quantum Link Module | Any Utility slot | Control group slots, radar range |
| **Engineering** | Advanced Tool Kit, Emergency Barricade Deployer | Support Utility slot | Repair speed, field construction |

---

## Equipping Mechanics — Three Playstyles Supported

The equipment system supports three levels of player engagement. All three work simultaneously in the same game.

### Playstyle 1: Hands-Off (Research Only)

- Player researches tech at labs → Layer 1 faction-wide upgrades
- New trainees auto-issue with best researched default gear
- No manual equipping needed
- Army scales through research progression alone

**UI:** Tech tree screen. Queue research, wait, new units are better. Done.

### Playstyle 2: Group Management (Pool Equipment)

- Player acquires pool equipment (crafted or purchased): "5x Medic Packs", "3x Reinforced Plating"
- Select all soldiers → click armor icon → best available gear auto-assigned from pool
- New trainees auto-equip if pool has spare items
- Mid-level management: optimize squads without touching individual units

**UI:** Equipment pool panel. Drag gear category onto unit group. Auto-distributes.

### Playstyle 3: Micro Management (Individual Loadouts)

- Player selects individual units, opens equipment screen
- Drag-and-drop specific gear into specific slots
- Hand-craft hero builds for champion units
- Unique loot items assigned to specific named units

**UI:** Unit detail panel with slot grid. Drag from inventory to slot. Preview stat changes before confirming.

### Auto-Equip Rules

When a new unit is trained and the pool has compatible gear:

1. **Layer 1 defaults** are applied first (researched faction-wide tech)
2. **Pool items** fill remaining slots, highest rarity first
3. If pool has insufficient items, slot stays at Layer 1 default
4. Pool items freed when a unit is disassembled or swaps to different gear

---

## Equipment and Death

- **Gear auto-recalls on death.** When a unit is destroyed, all equipped gear returns to the equipment pool automatically. No materials are lost.
- **Unique named items** return to the player's inventory as standalone items, ready to re-equip on another unit.
- **The risk of combat is time and momentum**, not inventory loss. This encourages aggressive play and dungeon exploration.
- **Champions are an exception.** Champion death is permanent (the named unit is gone forever). Their gear auto-recalls but the champion's veterancy, name, and story do not persist. [See champions](../../factions/Champions/README.md)

---

## Faction-Exclusive Equipment Categories

Each faction has access to unique equipment categories that other factions cannot use. These reinforce asymmetric playstyles. [See factions](../../factions/README.md)

| Faction | Exclusive Weapon Category | Exclusive Armor Category | Exclusive Utility Category |
|---------|--------------------------|------------------------|---------------------------|
| **Neon Punk** | Circuit Weapons (electric damage, +vs mechanical), Cyber Implants (passive stat boosts) | Cloaking Mesh (stealth armor variant) | Quantum Refiners (+10% material efficiency while equipped) |
| **Dark Realistic** | Heavy Ballistic Arsenal (high single-target damage), Fortress Turrets (mountable on walls) | Fortress Plating (+40% HP, -15% movement speed trade-off) | Tactical Comms (morale aura for nearby units) |
| **Cartoon SciFi** | Chaos Weapons (random bonus effects: fire, explosion, freeze), Overcharge Rifles (+fire rate) | Reinforced Jump Armor (jump jet + armor hybrid) | Chaos Mods (10% chance for random effect on hit) |
| **Bright Realistic** | Precision Tools (non-lethal crowd control options) | Civilian Efficiency Vest (-20% all resource costs while equipped) | Survival Kits (+50% survival resource efficiency, field rations) |

---

## Relationship to TIR Upgrade System

The equipment system and the TIR upgrade system [See upgrade system](../Upgrade-System/README.md) coexist:

- **TIR upgrades** are permanent chassis improvements (researched at labs, applied per-unit). They increase base HP, defense, and damage. These are the "foundation" stats.
- **Equipment** is swappable gear layered on top. It provides additive bonuses over the upgraded chassis.
- A fully upgraded TIR 3 soldier with a Legendary weapon can out-damage a base TIR 4 Heavy Mech — different roles, complementary strengths.

```
Example: TIR 2 Soldier after Forge upgrade + equipment

Chassis (TIR 2, upgraded at Forge):
  Base HP: 100 → 150 (+50% from Armor 1→2 upgrade)
  Base Defense: 0 → 10

Equipment Layer:
  Weapon slot: "Widow's Kiss" Legendary shotgun → +200% damage = 45 total damage
  Armor slot: Medic Pack → role change to medic, heals 5 HP/sec
  Utility slot: Thermal Goggles → +50% detection range

Result: A healing scout with devastating close-range firepower.
No TIR 3+ chassis required.
```

---

## See Also

- [Unit types and chassis stats](../Unit-Types/README.md)
- [TIR upgrade system (permanent chassis improvements)](../Upgrade-System/README.md)
- [Dungeon loot tables (equipment drop sources)](../../Gameplay/Dungeons/Dungeon-Overview/README.md)
- [Faction exclusive gear categories](../../factions/README.md)
- [Champion equipment slots and loadouts](../../factions/Champions/README.md)
- [Resource costs for crafting pool equipment](../../resources/README.md)
