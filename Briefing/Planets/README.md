# Planets

Planet classification, navigation, events, and biome systems for the solar system map and surface gameplay.

---

## Overview

Each solar system contains one central star with 5–10 orbiting planets. Planets are classified by their dominant biome type, environmental modifiers, TIR (Technology/Threat Intensity Rating) ring position, and hostile area content. The galaxy comprises 3,000+ solar systems arranged in concentric rings from the outer rim to the center, with difficulty scaling inward.

---

## Navigation & Travel

- **[Navigation](./Navigation/README.md)** — Solar system map, TIR progression, scanning mechanics, landing procedures, interplanetary travel, fuel-based movement, wormhole transit, and colony markers.

## Planet Types & Biomes

- **[Planet Types](./Planet-Types/README.md)** — Biome classification, secondary modifiers, hostile areas, boss mechanics, and TIR-based planet progression. Detailed biome docs are in the Biomes subfolders below.
- **[Terrain](./Terrain/README.md)** — Procedural planet surface strategy, terrain asset dependencies, and UE5 in-game biome demo plan.

### Biome Detail Folders

Each biome has its own folder with a design doc and UE5 implementation notes:

| Biome | Design Doc | UE5 Notes |
|-------|-----------|-----------|
| Desert | [Biomes/Desert/Desert.md](./Biomes/Desert/Desert.md) | [UE5.md](./Biomes/Desert/UE5.md) |
| Dusty | [Biomes/Dusty/Dusty.md](./Biomes/Dusty/Dusty.md) | [UE5.md](./Biomes/Dusty/UE5.md) |
| Rocky | [Biomes/Rocky/Rocky.md](./Biomes/Rocky/Rocky.md) | [UE5.md](./Biomes/Rocky/UE5.md) |
| Water | [Biomes/Water/Water.md](./Biomes/Water/Water.md) | [UE5.md](./Biomes/Water/UE5.md) |
| Swamp | [Biomes/Swamp/Swamp.md](./Biomes/Swamp/Swamp.md) | [UE5.md](./Biomes/Swamp/UE5.md) |
| Jungle | [Biomes/Jungle/Jungle.md](./Biomes/Jungle/Jungle.md) | [UE5.md](./Biomes/Jungle/UE5.md) |
| Light Snow | [Biomes/Light Snow/Light Snow.md](./Biomes/Light%20Snow/Light%20Snow.md) | [UE5.md](./Biomes/Light%20Snow/UE5.md) |
| Ice | [Biomes/Ice/Ice.md](./Biomes/Ice/Ice.md) | [UE5.md](./Biomes/Ice/UE5.md) |

## Planet Events & Dungeons

- **[Planet Events](./Planet-Events/README.md)** — Dungeon types, loot tables, abandoned structures, camp encounters, environmental and wildlife events, and dungeon discovery mechanics.

---

## See Also

- [Resource Types](../resources/README.md) — Resource generation by biome
- [Ship Modules](../ShipModules/README.md) — Ship scanning modules
- [Tech Tree Research](../Tech-Tree/Research-Categories/README.md) — Radar and scanning research
- [Gameplay Core Loop](../Gameplay/Core-Loop/README.md) — Colony lifecycle and abandoned planets
- [Factions Races](../factions/Races/README.md) — Race biome preferences
