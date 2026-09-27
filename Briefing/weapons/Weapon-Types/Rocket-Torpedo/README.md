# Rocket/Torpedo Weapons

Guided or unguided projectile weapons with high explosive yield. Slow fire rate but devastating damage. Effective against heavy armor, buildings, and clustered enemies.

---

## Small Rocket Bay

- **TIR:** 2
- **Damage:** 150 per rocket (area: 5m radius)
- **Fire Rate:** 1 volley/sec (6 rockets per reload)
- **Ammo:** 1 rocket pod (crafted at factory) [See resources/types.md](../../../resources/README.md)
- **Energy Cost:** 10 energy/turn (guidance system)
- **Range:** 80m
- **Special:** Area damage — splash damage decreases with distance from impact center
- **Notes:** Versatile anti-vehicle and anti-structure weapon

## Large Rocket Bay

- **TIR:** 3
- **Damage:** 300 per rocket (area: 8m radius)
- **Fire Rate:** 1 volley/5sec (4 rockets per reload)
- **Ammo:** 1 large rocket pod (crafted at factory)
- **Energy Cost:** 20 energy/turn (guidance system)
- **Range:** 120m
- **Special:** Can carry specialized warheads (see below)
- **Notes:** Heavy bombardment weapon. Requires Factory to produce ammo

## Torpedo Launcher

- **TIR:** 4
- **Damage:** 500 per torpedo (area: 10m radius)
- **Fire Rate:** 1 torpedo/10sec (self-guided)
- **Ammo:** 1 torpedo (crafted at Assembly Plant) [See planet buildings](../../../buildings/README.md)
- **Energy Cost:** 30 energy/turn (homing system)
- **Range:** Unlimited (self-guided within 200m)
- **Special:** Locks onto up to 4 targets automatically, prioritizes by threat level
- **Notes:** Ultimate conventional weapon. Rare ammo type

---

## Rocket Warhead Types [See resources/types.md](../../../resources/README.md)

| Warhead | TIR | Damage Multiplier | Special Effect | Unlock |
|---------|-----|-------------------|----------------|--------|
| High Explosive | 2 | 100% | Standard area damage | Starting |
| Nuclear | 3 | 300% (single target), 100% (area) | Radiation persists for 60 seconds | Dungeon blueprint |
| EMP | 3 | 50% physical, 200% electronic | Disables electronics for 10 seconds | Forge research |
| Heating | 4 | 150% + melts armor -50% | Reduces target armor permanently for 30s | Energy Lab research |
| Penetrator | 4 | 400% vs hull, 50% vs units | Pierces through everything | Dungeon blueprint |

---

## See Also

- [Weapon Types Overview](../README.md)
- [TIR System](../../TIR-System/README.md)
- [Planet turret configurations](../../../buildings/README.md)
- [Weapon resource requirements](../../../resources/README.md)
