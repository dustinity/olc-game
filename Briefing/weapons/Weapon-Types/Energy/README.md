# Energy Weapons

Beam or projectile weapons that draw power directly from the ship's energy core. Infinite ammo (limited by energy reserves), effective against organic and unshielded targets.

---

## Energy Emitter

- **TIR:** 3
- **Damage:** 40/second (continuous beam)
- **Fire Mode:** Continuous until switched off
- **Energy Cost:** 20 energy/turn while firing
- **Range:** 20m
- **Special:** Overheats after 10 seconds continuous fire (60 second cooldown)
- **Notes:** Infinite ammo draws from ship energy core. Effective against swarms and light units

## Plasma Cannon

- **TIR:** 4
- **Damage:** 200 per shot (charge time: 3 seconds)
- **Fire Mode:** Single shot with charge delay
- **Energy Cost:** 50 energy per shot
- **Range:** 50m
- **Special:** Superheated plasma melts through armor (+100% damage vs armored)
- **Notes:** Heavy anti-armor weapon. Slow fire rate requires precise targeting

## Laser Array

- **TIR:** 4
- **Damage:** 60/second (continuous beam)
- **Fire Mode:** Continuous until switched off
- **Energy Cost:** 35 energy/turn while firing
- **Range:** 60m
- **Special:** No overheating, but builds heat gradually (10% damage increase per second, caps at 200%)
- **Notes:** Long-range precision weapon. Best used in short bursts for optimal efficiency

---

## Energy Weapon Heat Management

All energy weapons generate heat:
- **Heat buildup:** +10% per second of continuous fire
- **Cooling rate:** -5% per second when not firing
- **Overheat threshold:** 100% — weapon shuts down for cooldown period
- **Mitigation:** Improvement cycles reduce heat buildup by 5% per level [See ship modules](../../../ShipModules/README.md)

---

## See Also

- [Weapon Types Overview](../README.md)
- [Ballistic Weapons](../Ballistic/README.md)
- [TIR System](../../TIR-System/README.md)
- [Ship module improvements](../../../ShipModules/README.md)
