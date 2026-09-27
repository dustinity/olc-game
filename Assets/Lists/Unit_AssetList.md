# Unit Asset List

Production inventory for playable units, champions, hostile units, weapons, animation sets, VFX, and UI content in **Our Last Chance**.

The first production target is **Stil-1: Dark Realistic Hard Sci-Fi**. This document supplements [Building_AssetList.md](Building_AssetList.md), which covers planet buildings, and [UI_AssetList.md](UI_AssetList.md), which covers reusable interface components.

---

## Gameplay and Presentation Rules

- Planet combat and base control are real-time RTS.
- Dungeon combat is real-time hack-and-slay; it does not use turns or action points.
- Units must remain recognizable at the fixed 40° RTS camera angle.
- Large silhouettes and equipment profiles take priority over tiny surface detail.
- Concept assets use transparent backgrounds, centered composition, no terrain, no text, and no UI.
- Runtime unit meshes must support selection outlines, team-color masks, damage feedback, and status-effect sockets.
- Routine repair effects communicate **Construction Material only**.
- UI portraits, ability icons, and minimap markers are separate deliverables from the unit concept art.

---

## ID Naming Convention

| Prefix | Category |
|--------|----------|
| `CH-NP` | Neon Punk Champions |
| `CH-DR` | Dark Realistic Champions |
| `CH-CS` | Cartoon SciFi Champions |
| `CH-BR` | Bright Realistic Champions |
| `UN-INF` | Infantry Units |
| `UN-LV` | Light Vehicles |
| `UN-HV` | Heavy Vehicles |
| `UN-AIR` | Aerial Units |
| `UN-SP` | Support and Special Units |
| `EN-INS` | Insectoid Enemies |
| `EN-REP` | Reptilian Enemies |
| `EN-MOL` | Molluskoid Enemies |
| `EN-HUM` | Humanoid Enemies |
| `EN-CRY` | Crystalloid Enemies |
| `WPN` | Unit Weapons |
| `ANM` | Shared Animation Sets |
| `UVFX` | Unit and Combat VFX |
| `UUI` | Unit-Specific UI Content |

Asset suffixes use `Main`, `Turnaround`, `Equipment`, `Portrait`, `Icon`, `Anim-*`, `VFX-*`, and `Damage-*`.

---

# Phase 0 — Vertical Slice Units

These units are sufficient to prove base defense, squad control, vehicle combat, support behavior, and a small hack-and-slay dungeon.

## CH-DR-01: “Ironclad” — Major Rachel Torres

| Property | Value |
|----------|-------|
| Role | Player champion / frontline commander |
| Faction | Dark Realistic |
| Gameplay | RTS command unit and directly controlled dungeon character |
| Priority | P0 |

**Asset Requirements:**

- [x] Hero character concept, 40° isometric — `CH-DR-01-Main` → **[CH-DR-01-Main.png](Style/Stil-1/Units/CH-DR-01-Main.png)** (1024×1536 RGBA, validated transparent background)
- [ ] Front/side/back production turnaround — `CH-DR-01-Turnaround`
- [ ] Heavy command armor modular set — `CH-DR-01-Equipment-Armor`
- [ ] Ballistic rifle and command sidearm — `CH-DR-01-Equipment-Weapons`
- [ ] Helmeted and unhelmeted heads — `CH-DR-01-Head-Variants`
- [ ] RTS portrait — `CH-DR-01-Portrait`
- [ ] Full-body selection silhouette — `CH-DR-01-Icon-Silhouette`
- [ ] Champion ability icon set — `CH-DR-01-Icons-Abilities`
- [ ] RTS locomotion/combat animation set — `CH-DR-01-Anim-RTS`
- [ ] Hack-and-slay locomotion, dodge, combo, and ability set — `CH-DR-01-Anim-Dungeon`
- [ ] Hit, downed, revive, and death states — `CH-DR-01-Anim-Damage`

**Visual Notes:** Broad armored silhouette, practical powered joints, dark gunmetal plates, compact orange command lights, readable rank geometry without tiny insignia or text.

---

## UN-INF-01: Basic Soldier

| Property | Value |
|----------|-------|
| TIR | 1 |
| Role | General-purpose rifle infantry |
| Priority | P0 |

**Asset Requirements:**

- [x] Standard rifle soldier concept — `UN-INF-01-Main` → **[UN-INF-01-Main.png](Style/Stil-1/Units/Infantry/UN-INF-01-Main.png)** (1024×1536 RGBA)
- [ ] Front/side/back turnaround — `UN-INF-01-Turnaround`
- [ ] Helmet and exposed-head variants — `UN-INF-01-Head-Variants`
- [ ] Ballistic rifle equipment asset — `UN-INF-01-Equipment-Rifle`
- [ ] Backpack and ammunition kit — `UN-INF-01-Equipment-FieldKit`
- [ ] Team-color material mask — `UN-INF-01-Material-TeamMask`
- [ ] Portrait and minimap silhouette — `UN-INF-01-UI`
- [ ] Shared rifle infantry animation set — `UN-INF-01-Anim`

**Visual Notes:** The baseline scale reference for all human units. Clearly lighter than a Shock Trooper and heavier than a Scout.

---

## UN-INF-02: Heavy Gunner

| Property | Value |
|----------|-------|
| TIR | 2 |
| Role | Suppression / anti-light-vehicle infantry |
| Priority | P0 |

**Asset Requirements:**

- [x] Heavy gunner concept — `UN-INF-02-Main` → **[UN-INF-02-Main.png](Style/Stil-1/Units/Infantry/UN-INF-02-Main.png)** (1024×1536 RGBA)
- [ ] Reinforced firing harness — `UN-INF-02-Equipment-Harness`
- [ ] Heavy rotary or dual-feed ballistic weapon — `UN-INF-02-Equipment-HeavyGun`
- [ ] Ammunition backpack and feed belt — `UN-INF-02-Equipment-AmmoFeed`
- [ ] Deployed firing stance reference — `UN-INF-02-Pose-Deployed`
- [ ] Portrait and minimap silhouette — `UN-INF-02-UI`
- [ ] Heavy-weapon animation set — `UN-INF-02-Anim`

---

## UN-INF-03: Scout

| Property | Value |
|----------|-------|
| TIR | 1 |
| Role | Reconnaissance / detection |
| Priority | P0 |

**Asset Requirements:**

- [x] Light scout concept — `UN-INF-03-Main` → **[UN-INF-03-Main.png](Style/Stil-1/Units/Infantry/UN-INF-03-Main.png)** (1024×1536 RGBA)
- [ ] Compact pistol or carbine — `UN-INF-03-Equipment-Weapon`
- [ ] Sensor visor and antenna pack — `UN-INF-03-Equipment-Sensors`
- [ ] Cloak-compatible material mask — `UN-INF-03-Material-CloakMask`
- [ ] Portrait and minimap silhouette — `UN-INF-03-UI`
- [ ] Fast locomotion and scanning animation set — `UN-INF-03-Anim`

---

## UN-INF-06: Field Medic

| Property | Value |
|----------|-------|
| TIR | 2 |
| Role | Healing / revive support |
| Priority | P0 |

**Asset Requirements:**

- [x] Armored field medic concept — `UN-INF-06-Main` → **[UN-INF-06-Main.png](Style/Stil-1/Units/Infantry/UN-INF-06-Main.png)** (1024×1536 RGBA)
- [ ] Medical backpack and injector tool — `UN-INF-06-Equipment-Medical`
- [ ] Compact defensive shotgun — `UN-INF-06-Equipment-Shotgun`
- [ ] Large readable medical light panels without text — `UN-INF-06-Material-Medical`
- [ ] Portrait and minimap silhouette — `UN-INF-06-UI`
- [ ] Heal, revive, drag, and defensive-fire animations — `UN-INF-06-Anim`

---

## UN-INF-07: Engineer

| Property | Value |
|----------|-------|
| TIR | 2 |
| Role | Construction, repair, emergency defenses |
| Priority | P0 |

**Asset Requirements:**

- [x] Combat engineer concept — `UN-INF-07-Main` → **[UN-INF-07-Main.png](Style/Stil-1/Units/Infantry/UN-INF-07-Main.png)** (1024×1536 RGBA)
- [ ] Construction Material canister backpack — `UN-INF-07-Equipment-RepairPack`
- [ ] Powered repair tool — `UN-INF-07-Equipment-RepairTool`
- [ ] Emergency barricade deployment kit — `UN-INF-07-Equipment-Barricade`
- [ ] Portrait and minimap silhouette — `UN-INF-07-UI`
- [ ] Repair, build, weld, carry, and deploy animations — `UN-INF-07-Anim`

**Visual Notes:** Repair VFX and carried supplies must not imply titanium, minerals, or energy as routine repair currencies.

---

## UN-LV-01: Scout Car

| Property | Value |
|----------|-------|
| TIR | 1 |
| Grid Size | 1×1 |
| Role | Fast ground reconnaissance |
| Priority | P0 |

**Asset Requirements:**

- [x] Armored scout car concept — `UN-LV-01-Main` → **[UN-LV-01-Main.png](Style/Stil-1/Units/Vehicles/UN-LV-01-Main.png)** (1254×1254 RGBA)
- [ ] Front/side/rear vehicle turnaround — `UN-LV-01-Turnaround`
- [ ] Light turret hardpoint — `UN-LV-01-Mech-TurretMount`
- [ ] Sensor mast — `UN-LV-01-Prop-Sensors`
- [ ] Wheel, suspension, steering, and turret animation rig — `UN-LV-01-Rig`
- [ ] Damaged and destroyed states — `UN-LV-01-Damage`
- [ ] Portrait and minimap silhouette — `UN-LV-01-UI`

---

## UN-LV-03: Light APC

| Property | Value |
|----------|-------|
| TIR | 2 |
| Grid Size | 1×1 |
| Role | Protected transport for four infantry |
| Priority | P0 |

**Asset Requirements:**

- [x] Light APC concept — `UN-LV-03-Main` → **[UN-LV-03-Main.png](Style/Stil-1/Units/Vehicles/UN-LV-03-Main.png)** (1254×1254 RGBA, three axles/six wheels)
- [ ] Four-seat transport compartment layout — `UN-LV-03-Interior`
- [ ] Rear powered troop ramp — `UN-LV-03-Mech-Ramp`
- [ ] Defensive turret — `UN-LV-03-Mech-Turret`
- [ ] Wheel and suspension rig — `UN-LV-03-Rig`
- [ ] Load/unload animation references — `UN-LV-03-Anim-Transport`
- [ ] Damaged and destroyed states — `UN-LV-03-Damage`
- [ ] Portrait and minimap silhouette — `UN-LV-03-UI`

---

## UN-HV-01: Tank

| Property | Value |
|----------|-------|
| TIR | 2 |
| Grid Size | 2×2 |
| Role | Heavy direct-fire vehicle |
| Priority | P0 |

**Asset Requirements:**

- [x] Main battle tank concept — `UN-HV-01-Main` → **[UN-HV-01-Main.png](Style/Stil-1/Units/Vehicles/UN-HV-01-Main.png)** (1402×1122 RGBA)
- [ ] Front/side/rear vehicle turnaround — `UN-HV-01-Turnaround`
- [ ] Heavy single ballistic cannon — `UN-HV-01-Mech-MainCannon`
- [ ] Coaxial defensive weapon — `UN-HV-01-Mech-Coaxial`
- [ ] Track, suspension, turret, and recoil rig — `UN-HV-01-Rig`
- [ ] Armor damage stages — `UN-HV-01-Damage-State1, UN-HV-01-Damage-State2`
- [ ] Destroyed hull and detached turret state — `UN-HV-01-Damage-Destroyed`
- [ ] Portrait and minimap silhouette — `UN-HV-01-UI`

---

## UN-AIR-01: Scout Drone

| Property | Value |
|----------|-------|
| TIR | 1 |
| Role | Aerial reconnaissance |
| Priority | P0 |

**Asset Requirements:**

- [x] Compact military scout drone — `UN-AIR-01-Main` → **[UN-AIR-01-Main.png](Style/Stil-1/Units/Aircraft/UN-AIR-01-Main.png)** (1448×1086 RGBA)
- [ ] Folding rotor/thruster assembly — `UN-AIR-01-Mech-Flight`
- [ ] Optical and thermal sensor cluster — `UN-AIR-01-Prop-Sensors`
- [ ] Small defensive payload mount — `UN-AIR-01-Mech-Payload`
- [ ] Hover, bank, scan, deploy, return, and crash animations — `UN-AIR-01-Anim`
- [ ] Damaged and destroyed states — `UN-AIR-01-Damage`
- [ ] Portrait and minimap silhouette — `UN-AIR-01-UI`

---

# Phase 1 — Remaining Playable Roster

## Infantry

| Asset ID | Unit | TIR | Gameplay Signature | Required Distinctive Assets |
|----------|------|:---:|--------------------|-----------------------------|
| `UN-INF-04` | [Shock Trooper](Style/Stil-1/Units/Infantry/UN-INF-04-Main.png) | 2 | Dark Realistic heavy assault | Main concept complete; powered armor, heavy rifle, charge/braced-fire animation pending |
| `UN-INF-05` | [Jump Trooper](Style/Stil-1/Units/Infantry/UN-INF-05-Main.png) | 2 | Rapid vertical repositioning | Main concept complete; airborne pose set and takeoff/landing VFX sockets pending |
| `UN-SP-01` | [Demolitions Expert](Style/Stil-1/Units/Infantry/UN-SP-01-Main.png) | 2 | Remote explosives | Main concept complete; C4 kit and plant/detonate animations pending |
| `UN-SP-02` | [Sniper](Style/Stil-1/Units/Infantry/UN-SP-02-Main.png) | 2 | Extreme-range precision | Main concept complete; cloak sheet and prone/aim/fire animations pending |
| `UN-SP-03` | [Engineer Sapper](Style/Stil-1/Units/Infantry/UN-SP-03-Main.png) | 2 | Mines and traps | Main concept complete; tripwire props and deploy/disarm animations pending |

Every infantry entry requires `Main`, `Turnaround`, `Equipment`, `UI`, `Material-TeamMask`, and `Anim` assets.

## Ground Vehicles

| Asset ID | Unit | TIR | Grid | Gameplay Signature |
|----------|------|:---:|:----:|--------------------|
| `UN-LV-02` | [Jump Bike](Style/Stil-1/Units/Vehicles/UN-LV-02-Main.png) | 1 | 1×1 | Fastest ground unit; jump/boost capability |
| `UN-HV-02` | [Walker](Style/Stil-1/Units/Vehicles/UN-HV-02-Main.png) | 3 | 2×2 | Two-legged all-terrain assault mech |
| `UN-HV-03` | [Heavy Transport](Style/Stil-1/Units/Vehicles/UN-HV-03-Main.png) | 2 | 2×2 | Resources and unit logistics |
| `UN-SP-04` | [Heavy Mech](Style/Stil-1/Units/Vehicles/UN-SP-04-Main.png) | 4 | 2×2 | Endgame cannon and rocket platform |

Every vehicle entry requires `Main`, `Turnaround`, `Rig`, `Hardpoints`, `Damage-State1`, `Damage-State2`, `Damage-Destroyed`, and `UI`.

## Aerial Units

| Asset ID | Unit | TIR | Gameplay Signature |
|----------|------|:---:|--------------------|
| `UN-AIR-02` | [Fighter Jet](Style/Stil-1/Units/Aircraft/UN-AIR-02-Main.png) | 3 | Air superiority and ground attack |
| `UN-AIR-03` | [Tactical Dropship](Style/Stil-1/Units/Aircraft/UN-AIR-03-Main.png) | 2 | Eight-unit rapid transport |

Every aircraft entry requires `Main`, `Turnaround`, `Rig`, `LandingGear`, `Hardpoints`, `Damage`, `UI`, and flight/combat animation references.

---

# Phase 2 — Champion Roster

## Neon Punk

| Asset ID | Champion | Role |
|----------|----------|------|
| `CH-NP-01` | [“Voltage” — Kai Nakamura](Style/Stil-1/Units/Champions/CH-NP-01-Main.png) | Energy damage |
| `CH-NP-02` | [“Glitch” — Alex Chen](Style/Stil-1/Units/Champions/CH-NP-02-Main.png) | Electronic warfare |
| `CH-NP-03` | [“Wrench” — Sam Okafor](Style/Stil-1/Units/Champions/CH-NP-03-Main.png) | Engineering and extraction |

## Dark Realistic

| Asset ID | Champion | Role |
|----------|----------|------|
| `CH-DR-01` | [“Ironclad” — Major Rachel Torres](Style/Stil-1/Units/CH-DR-01-Main.png) | Frontline commander |
| `CH-DR-02` | [“Sniper” — Viktor Petrov](Style/Stil-1/Units/Champions/CH-DR-02-Main.png) | Precision combat |
| `CH-DR-03` | [“Demolitions” — Corporal Jake Morrison](Style/Stil-1/Units/Champions/CH-DR-03-Main.png) | Explosives |

## Cartoon SciFi

| Asset ID | Champion | Role |
|----------|----------|------|
| `CH-CS-01` | [“Blaze” — Rico Delgado](Style/Stil-1/Units/Champions/CH-CS-01-Main.png) | Aggressive mobility |
| `CH-CS-02` | [“Zoom” — Pip Tanaka](Style/Stil-1/Units/Champions/CH-CS-02-Main.png) | Speed specialist |
| `CH-CS-03` | [“Boomstick” — Daisy Mayhem](Style/Stil-1/Units/Champions/CH-CS-03-Main.png) | Heavy weapons |

## Bright Realistic

| Asset ID | Champion | Role |
|----------|----------|------|
| `CH-BR-01` | [“Medic” — Dr. Amara Osei](Style/Stil-1/Units/Champions/CH-BR-01-Main.png) | Healing and survival |
| `CH-BR-02` | [“Gardener” — Elias Greenfield](Style/Stil-1/Units/Champions/CH-BR-02-Main.png) | Sustainability |
| `CH-BR-03` | [“Architect” — Morgan Lee](Style/Stil-1/Units/Champions/CH-BR-03-Main.png) | Construction and logistics |

Each champion requires:

- [x] 40° isometric hero concept — `Main` (12/12 champions complete and linked above)
- [ ] Front/side/back turnaround — `Turnaround`
- [ ] Modular armor and equipment — `Equipment`
- [ ] Helmeted/unhelmeted heads — `Head-Variants`
- [ ] RTS and dialogue portraits — `Portrait-RTS, Portrait-Dialogue`
- [ ] Ability icon set — `Icons-Abilities`
- [ ] RTS animation set — `Anim-RTS`
- [ ] Dungeon hack-and-slay animation set — `Anim-Dungeon`
- [ ] Downed, revive, and death set — `Anim-Damage`

---

# Phase 3 — Hostile Units and Bosses

The briefing defines 30 hostile archetypes. Production should share one skeleton per family where anatomy permits.

## Insectoid Family

| Asset ID | Enemy | Combat Read |
|----------|-------|-------------|
| `EN-INS-01` | [Chitin Bugs](Style/Stil-1/Units/Enemies/EN-INS-01-Main.png) | Small swarm melee; main concept complete |
| `EN-INS-02` | Spore Queen | Large slow spawner |
| `EN-INS-03` | Burrower | Underground ambush |
| `EN-INS-04` | Wing Stinger | Fast aerial attacker |
| `EN-INS-05` | Armored Mantis | Heavy charging elite |
| `EN-INS-06` | Crystal Hive | Reflective boss |

## Reptilian Family

| Asset ID | Enemy | Combat Read |
|----------|-------|-------------|
| `EN-REP-01` | Terradon | Charging brute |
| `EN-REP-02` | Scaleback | Defensive shell tank |
| `EN-REP-03` | Viper Pack | Fast venom swarm |
| `EN-REP-04` | Frost Drake | Freeze-breath elite |
| `EN-REP-05` | Magma Lizard | Persistent fire zones |
| `EN-REP-06` | Hydra Colony | Multi-target boss |

## Molluskoid Family

| Asset ID | Enemy |
|----------|-------|
| `EN-MOL-01` | Shell Snail |
| `EN-MOL-02` | Tentacle Crawler |
| `EN-MOL-03` | Acid Spitter |
| `EN-MOL-04` | Glow Squid |
| `EN-MOL-05` | Iron Clam |
| `EN-MOL-06` | Void Octopus |

## Humanoid Family

| Asset ID | Enemy |
|----------|-------|
| `EN-HUM-01` | Scavenger |
| `EN-HUM-02` | Punk Raider |
| `EN-HUM-03` | Military Outpost Soldier |
| `EN-HUM-04` | Hostile Medic |
| `EN-HUM-05` | Hostile Engineer |
| `EN-HUM-06` | Elite Commander |

## Crystalloid Family

| Asset ID | Enemy |
|----------|-------|
| `EN-CRY-01` | Crystal Shard |
| `EN-CRY-02` | Prism Guardian |
| `EN-CRY-03` | Obsidian Golem |
| `EN-CRY-04` | Radioactive Core |
| `EN-CRY-05` | Resonance Crystal |
| `EN-CRY-06` | Dark Matter Shaper |

Each enemy requires:

- [ ] Main gameplay concept and scale comparison
- [ ] Production turnaround
- [ ] Family-compatible skeletal rig
- [ ] Idle, movement, primary attack, special attack, hit, stagger, death animations
- [ ] Elite/boss material variant where applicable
- [ ] Telegraph, impact, status, spawn, and death VFX sockets
- [ ] Health-bar silhouette, minimap marker, and bestiary portrait

---

# Weapon Asset Families

| Asset ID | Weapon |
|----------|--------|
| `WPN-BAL-01` | Dual ballistic cannon |
| `WPN-BAL-02` | Quad ballistic cannon |
| `WPN-BAL-03` | Rotary cannon |
| `WPN-ENG-01` | Energy emitter |
| `WPN-ENG-02` | Plasma cannon |
| `WPN-ENG-03` | Laser array |
| `WPN-RKT-01` | Small rocket bay |
| `WPN-RKT-02` | Large rocket bay |
| `WPN-RKT-03` | Torpedo launcher |
| `WPN-ION-01` | Ion emitter |
| `WPN-ION-02` | Ion storm generator |
| `WPN-VOI-01` | Void beam |
| `WPN-VOI-02` | Gravity well emitter |

Each family requires a world model, unit/vehicle mounting variant, inventory icon, projectile or beam, muzzle effect, impact effect, audio event reference, and TIR material progression.

---

# Shared Animation Sets

| Asset ID | Animation Set | Required Clips |
|----------|---------------|----------------|
| `ANM-INF-01` | Rifle Infantry | Idle, walk, run, aim, fire, reload, melee, hit, downed, revive, death |
| `ANM-INF-02` | Heavy Weapon Infantry | Carry, brace, deploy, spin-up, fire, reload, undeploy |
| `ANM-INF-03` | Support Infantry | Repair, build, heal, revive, interact, carry |
| `ANM-CH-01` | Dungeon Champion | Eight-direction locomotion, sprint, dodge, light/heavy combos, abilities, interact |
| `ANM-VEH-01` | Wheeled Vehicle | Steering, suspension, wheel rotation, braking, recoil |
| `ANM-VEH-02` | Tracked Vehicle | Track movement, suspension, turret aim, recoil |
| `ANM-MECH-01` | Walker/Mech | Locomotion, turn-in-place, aim, fire, stagger, collapse |
| `ANM-AIR-01` | Aircraft | Takeoff, land, hover, bank, attack, crash |
| `ANM-EN-*` | Enemy Family Sets | Family locomotion, attacks, telegraphs, stagger, death |

---

# Combat VFX

| Asset ID | Effect |
|----------|--------|
| `UVFX-SEL-01` | Friendly unit selection ring |
| `UVFX-SEL-02` | Enemy target ring |
| `UVFX-MOV-01` | Move-order destination marker |
| `UVFX-ATK-01` | Attack-order marker |
| `UVFX-DMG-01` | Ballistic impact |
| `UVFX-DMG-02` | Armor ricochet |
| `UVFX-DMG-03` | Shield impact |
| `UVFX-DMG-04` | Critical hit |
| `UVFX-STS-01` | Heal status |
| `UVFX-STS-02` | Poison status |
| `UVFX-STS-03` | Burning status |
| `UVFX-STS-04` | Frozen status |
| `UVFX-STS-05` | Stunned/EMP status |
| `UVFX-RPR-01` | Construction Material repair effect |
| `UVFX-DTH-01` | Infantry death/dissolve cleanup |
| `UVFX-DTH-02` | Vehicle destruction and persistent wreck |

---

# Unit UI Content

These assets populate the reusable panels already defined in `UI_AssetList.md`.

| Asset ID | Content |
|----------|---------|
| `UUI-POR-*` | Unit and champion portraits |
| `UUI-SIL-*` | Selection and minimap silhouettes |
| `UUI-ABL-*` | Active and passive ability icons |
| `UUI-STS-*` | Buff, debuff, veterancy, and status icons |
| `UUI-ORD-01` | Move order |
| `UUI-ORD-02` | Attack order |
| `UUI-ORD-03` | Stop order |
| `UUI-ORD-04` | Hold position |
| `UUI-ORD-05` | Patrol |
| `UUI-ORD-06` | Guard/follow |
| `UUI-ORD-07` | Enter transport |
| `UUI-ORD-08` | Exit transport |
| `UUI-ORD-09` | Repair |
| `UUI-ORD-10` | Heal/revive |

---

# Production Order

1. `CH-DR-01` Ironclad
2. `UN-INF-01` Basic Soldier
3. `UN-INF-03` Scout
4. `UN-INF-07` Engineer
5. `UN-INF-02` Heavy Gunner
6. `UN-INF-06` Field Medic
7. `UN-LV-01` Scout Car
8. `UN-LV-03` Light APC
9. `UN-HV-01` Tank
10. `UN-AIR-01` Scout Drone
11. One small enemy family slice: `EN-INS-01`, `EN-INS-02`, `EN-INS-05`
12. One dungeon boss: `EN-INS-06`

This sequence supports a playable RTS base-defense encounter and a complete room-based hack-and-slay dungeon before expanding the full roster.

---

# Asset Count Summary

| Category | Gameplay Types |
|----------|:--------------:|
| Champions | 12 |
| Infantry and special infantry | 10 |
| Ground vehicles and mechs | 7 |
| Aerial units | 3 |
| Hostile archetypes | 30 |
| Weapon families | 13 |
| Shared animation families | 9+ |
| Core combat/status VFX | 16 |

The same unit can require several deliverables—concept, turnaround, equipment, portrait, rig, animations, VFX hooks, and damage states—so gameplay-type count is not the final file count.

---

## See Also

- [Unit gameplay definitions](../Briefing/units/units.md)
- [Unit upgrade system](../Briefing/units/Upgrade_System.md)
- [Champion definitions](../Briefing/factions/champions.md)
- [Hostile race definitions](../Briefing/factions/races.md)
- [Weapon definitions](../Briefing/weapons/weapon_types.md)
- [Dungeon gameplay](../Briefing/Gameplay/Dungeons.md)
- [Stil-1 visual guide](Style/Stil-1/STYLE_GUIDE.md)
