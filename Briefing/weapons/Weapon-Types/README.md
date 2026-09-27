# Weapon Types

All weapons in the game fall into one of five damage categories, each with distinct mechanics, ammo types, and tactical roles. Weapons can be mounted on:
- **Mothership turrets** [See ship modules](../../ShipModules/README.md)
- **Planet base turrets** [See planet buildings](../../buildings/README.md)
- **Unit equipment** [See units](../../units/Unit-Types/README.md)

---

## Subfolders

- **[Ballistic](./Ballistic/README.md)** — Conventional projectile weapons using kinetic force
- **[Energy](./Energy/README.md)** — Beam and projectile weapons drawing from energy core
- **[Rocket/Torpedo](./Rocket-Torpedo/README.md)** — Guided and unguided high-yield explosives
- **[Ion](./Ion/README.md)** — Electronic disruption weapons vs mechanical targets
- **[Void](./Void/README.md)** — Spatial manipulation weapons that bypass all armor

---

## Category Comparison

| Type | Best Against | Ammo Source | Energy Cost | Damage Type | Range |
|------|-------------|-------------|-------------|-------------|-------|
| Ballistic | Light vehicles, structures | Metal ore (processed) | Low (5/turn) | Physical | Medium |
| Energy | Organic swarms, unarmored | Ship energy core | Medium (20/turn) | Thermal | Short-Medium |
| Rocket/Torpedo | Heavy armor, buildings | Crafted pods | Medium (10/turn) | Explosive | Long |
| Ion | Mechanical enemies, drones | Ship energy core | Low (15/turn) | Electronic | Short |
| Void | Everything (ignores armor) | Dark matter crystals | High (40/turn) | Spatial | Medium |

---

## Turret Configuration System

### Mount Types

| Mount | Grid Size | Weapon Slots | Power Required | Notes |
|-------|-----------|--------------|----------------|-------|
| Light Turret | 1x1 | 1 (ballistic or energy) | 5-20 energy/turn | Basic mount, any turret platform |
| Heavy Turret | 2x2 | 1 (rocket or ion) | 10-40 energy/turn | Reinforced mount, higher damage |
| Dual Mount | 2x2 | 2 (same type only) | 10-30 energy/turn | Two light weapons simultaneously |
| Quad Mount | 4x4 | 4 (any mix) | 20-80 energy/turn | Full turret platform [See planet buildings](../../buildings/README.md) |

### Ship-to-Ground Weapon Sync

Ship weapons enable planetary Orbital Strike Beacon functionality:
- **Rocket bays installed on ship** → Beacon fires missile strikes
- **Energy cores/emitters on ship** → Beacon fires beam strikes
- **Ion emitters on ship** → Beacon fires EMP strikes
- **Void weapons on ship** → Beacon fires void pulses (TIR 5+)

### Turret Targeting Modes

| Mode | Behavior | Energy Cost | Best Use |
|------|----------|-------------|----------|
| Auto | Targets nearest enemy in range | Base | Passive defense |
| Focus | Attacks single target until destroyed | Base + 5 energy | Boss fights, heavy units |
| Spread | Hits all enemies in area | Base + 15 energy | Swarm control |
| Pulse | Fires periodic wide burst | Base + 10 energy | Area denial |

---

## Weapon TIR Progression [See TIR system](../TIR-System/README.md)

Each weapon type follows TIR progression with improvement cycles:

| TIR | Unlock Method | Damage Multiplier | Range Multiplier |
|-----|---------------|-------------------|------------------|
| 1 | Starting | 80% | 80% |
| 2 | Dungeon blueprint or forge research | 90% | 90% |
| 3 | Energy Lab research | 100% | 100% |
| 4 | Dark Matter Lab research | 115% | 110% |
| 5 | Void Lab research | 130% | 120% |
| Alien | Center galaxy reward | 150% | 150% |

Improvement cycles add +5% to +81% cumulative bonus per improvement level [See ship modules](../../ShipModules/README.md)

---

## See Also

- [TIR system and improvement cycles](../TIR-System/README.md)
- [Ship weapon modules](../../ShipModules/README.md)
- [Planet turret configurations](../../buildings/README.md)
- [Unit weapon equipment](../../units/Unit-Types/README.md)
- [Weapon resource requirements](../../resources/README.md)
