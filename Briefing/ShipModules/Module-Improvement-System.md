# Module Improvement System

Every ship module can be improved through repeated cycles of upgrades, yielding escalating performance bonuses. The system applies uniformly across all module categories (drives, storage, protection, scanning, weapons, labs, and support).

---

## Improvement Cycles

Each module can be improved up to 10 times, with escalating bonuses:

| Improvement Level | Bonus Per Level | Cumulative Bonus | Notes |
|-------------------|-----------------|------------------|-------|
| 1-3 | +5% per level | +5%, +10%, +15% | Linear scaling |
| 4-6 | +7% per level | +22%, +29%, +36% | Accelerated |
| 7-9 | +10% per level | +46%, +56%, +66% | Major boosts |
| 10 (Max) | +15% | +81% | Peak performance |

The bonus applies to the module's primary stat:
- **Drives:** speed and range
- **Storage:** capacity per resource type
- **Protection:** HP and resistance values
- **Scanning:** range and accuracy
- **Weapons:** damage output
- **Labs:** research speed
- **Support:** crew capacity or slot count

---

## Improvement Materials

Each improvement requires four components:

1. **Construction Material** — Scales with the module's TIR (Tier Index Rating). Higher TIR modules consume more per cycle.
2. **Minerals** — Specific type depends on the module category (e.g., metals for hull, crystals for scanning, fuel compounds for drives).
3. **Energy** — Required for activation of the improvement process. Drawn from the ship's energy core.
4. **Blueprint** — Unique to each module improvement tier. Obtained through research labs, dungeon loot, or center galaxy rewards.

---

## Improvement Priority Guide

| Priority | Modules | Reason |
|----------|---------|--------|
| Critical | Storage, Hull, Drives | Survival and mobility |
| High | Weapons, Energy Core | Combat effectiveness |
| Medium | Scanning, Hangars | Exploration and flexibility |
| Low | Decorative modules | Aesthetics only |

### Recommended Early-Game Path

1. **Storage** (first improvement) — the starting Locker capacity of 200 per type is insufficient beyond the first few missions
2. **Hull** (second improvement) — enables atmospheric entry and dungeon landing without catastrophic damage
3. **Drives** (third improvement) — unlocks interplanetary travel range; the Rocket Drive's inefficiency compounds over distance

### Recommended Late-Game Path

1. **Weapons** — high-tier modules benefit most from the +81% peak bonus at level 10
2. **Scanning** — Quantum Scanner and Satellite Bay improvements reduce exploration time significantly
3. **Labs** — research speed improvements accelerate unlocking of alien-tier modules

---

## See Also

- [Ship Modules Index](./README.md)
- [Dropship initial state and repair](../Spaceship/Dropship/README.md)
- [Ship progression path](../Spaceship/Progression/README.md)
- [Tech tree research categories](../Tech-Tree/Research-Categories/README.md)
