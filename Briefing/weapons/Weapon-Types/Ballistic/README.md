# Ballistic Weapons

Conventional projectile weapons using kinetic force to penetrate targets. Reliable, ammo-hungry, effective against unshielded targets.

---

## 2-Cannon Ballistic Turret

- **TIR:** 1
- **Damage:** 15 per hit
- **Fire Rate:** 1 shot/sec
- **Ammo:** 5 processed metal per shot
- **Energy Cost:** 5 energy/turn (idle maintenance)
- **Range:** 30m
- **Special:** -25% effectiveness vs armored targets
- **Notes:** Starting ship weapon. Basic defense against light threats

## 4-Cannon Ballistic Turret

- **TIR:** 2
- **Damage:** 25 per hit (each cannon)
- **Fire Rate:** 1 shot/sec per cannon (4 total)
- **Ammo:** 8 processed metal per shot per cannon
- **Energy Cost:** 10 energy/turn (idle maintenance)
- **Range:** 40m
- **Special:** Can switch between standard and armor-piercing rounds (crafted separately)
- **Notes:** Heavy ballistic option. Effective against buildings and light vehicles

## Rotary Cannon

- **TIR:** 3
- **Damage:** 20 per hit
- **Fire Rate:** 8 shots/sec
- **Ammo:** 3 processed metal per shot
- **Energy Cost:** 15 energy/turn (idle maintenance)
- **Range:** 25m
- **Special:** Overheat after 5 seconds continuous fire (30 second cooldown)
- **Notes:** Gatling-style devastation against swarms. Ammo-hungry — requires constant supply

---

## Ammunition Types [See resources/types.md](../../../resources/README.md)

| Round Type | TIR | Damage Multiplier | Special Effect | Cost |
|------------|-----|-------------------|----------------|------|
| Standard Metal | 1 | 100% | None | Base |
| Armor-Piercing | 2 | 150% vs armor, 75% vs organic | Penetrates hull plating | +50% mineral cost |
| Incendiary | 2 | 80% base, 40%/sec for 10s | Sets targets on fire | +30% mineral cost |
| Hollow Point | 1 | 200% vs organic, 50% vs armor | Expansion on impact | Base cost |

---

## See Also

- [Weapon Types Overview](../README.md)
- [Energy Weapons](../Energy/README.md)
- [TIR System](../../TIR-System/README.md)
- [Weapon resource requirements](../../../resources/README.md)
