# Tech Tree — Master Index

Radial research system with topic clustering, prerequisite chains, and eight research categories spanning five progression tiers from crash landing to endgame alien technology.

---

## System Overview

The technology tree is organized as concentric rings radiating outward from a central core. Each ring represents a TIR (Threat Index Rating) progression tier and contains clustered research topics across eight categories. Research unlocks are gated by building requirements, dungeon completions, planet discoveries, and boss defeats.

### Radial Ring Design

```
                ┌─────────────────────────┐
                │   OUTER RING (TIR 4-5)  │
                │  Void Lab Research      │
                │  • Void Weapons         │
                │  • Void Drives          │
                │  • Alien Integration    │
                └───────────┬─────────────┘
                            │
                ┌───────────▼─────────────┐
                │  RING 3 (TIR 3-4)       │
                │  Dark Matter Lab        │
                │  • Energy Advanced      │
                │  • Armor Elite          │
                │  • Weapons III          │
                └───────────┬─────────────┘
                            │
                ┌───────────▼─────────────┐
                │  RING 2 (TIR 2-3)       │
                │  Energy Lab             │
                │  • Weapons II           │
                │  • Armor Advanced       │
                │  • Vision II            │
                └───────────┬─────────────┘
                            │
                ┌───────────▼─────────────┐
                │  RING 1 (TIR 1-2)       │
                │  Basic Forge            │
                │  • Weapons I            │
                │  • Armor Basic          │
                │  • Vision I             │
                └───────────┬─────────────┘
                            │
                ┌───────────▼─────────────┐
                │    CORE (Starting)      │
                │  Dropship Systems       │
                │  • Basic Combat         │
                │  • Navigation           │
                │  • Resource Basics      │
                └─────────────────────────┘
```

### Ring Progression Requirements

| Ring | TIR Range | Research Building | Unlock Requirement |
|------|-----------|-------------------|--------------------|
| Core | Starting | Dropship systems | Available at game start |
| Ring 1 | TIR 1-2 | Basic Forge (TIR 2) | Complete 2 small dungeons OR forge basic research |
| Ring 2 | TIR 2-3 | Energy Lab (TIR 3) | Complete Ring 1 topics + build Energy Lab |
| Ring 3 | TIR 3-4 | Dark Matter Lab (TIR 4) | Complete Ring 2 topics + build Dark Matter Lab |
| Outer | TIR 4-5 | Void Lab (TIR 5) | Complete Ring 3 topics + build Void Lab |

---

## Documentation Structure

### Overview

Full radial system documentation — ring structure, cluster categories, prerequisite chains, and research speed modifiers.

- **[Overview/README.md](./Overview/README.md)** — Complete tech tree overview with all rings, clusters, progression chains, and bonuses

### Research Categories

Detailed breakdown of all 89 research topics organized by category with prerequisites, material costs, timing, and cross-references.

| Category | Subfolder | Total Topics | Rings Covered |
|----------|-----------|-------------|---------------|
| Weapons | [Weapons-Research](./Research-Categories/Weapons-Research/README.md) | 15 | Core through Outer |
| Armor | [Armor-Research](./Research-Categories/Armor-Research/README.md) | 12 | Core through Outer |
| Drives | [Drives-Research](./Research-Categories/Drives-Research/README.md) | 10 | Core through Outer |
| Energy | [Energy-Research](./Research-Categories/Energy-Research/README.md) | 12 | Core through Outer |
| Vision | [Vision-Research](./Research-Categories/Vision-Research/README.md) | 9 | Ring 1 through Outer |
| Storage | [Storage-Research](./Research-Categories/Storage-Research/README.md) | 9 | Core through Outer |
| Buildings | [Buildings-Research](./Research-Categories/Buildings-Research/README.md) | 12 | Core through Outer |
| Units | [Units-Research](./Research-Categories/Units-Research/README.md) | 10 | Core through Outer |

**Total: 89 research topics across 5 rings and 8 categories.**

---

## Cross-References

- [Buildings](../buildings/README.md) — Planet buildings, ship modules, storage systems
- [Weapons](../weapons/Weapon-Types/README.md) — Weapon types and TIR system
- [Units](../units/README.md) — Unit roster and upgrade system
- [Spaceship](../Spaceship/Dropship/README.md) — Dropship systems and progression
- [Resources](../resources/README.md) — Resource types and economy
- [Planets](../Planets/Navigation/README.md) — Navigation, planet types, biomes
- [Factions](../factions/Champions/README.md) — Champions, factions, races
- [Gameplay](../Gameplay/Dungeons/Dungeon-Overview/README.md) — Dungeons, core loop, space travel
