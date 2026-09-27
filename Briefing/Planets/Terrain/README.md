# Planet Terrain

Terrain generation strategy for planet surface RTS gameplay.

## Documents

| Document | Purpose |
|---|---|
| [Terrain Strategy](./Terrain-Strategy.md) | Source-of-truth approach for height maps, tile masks, biome rendering, resources, hazards, and gameplay rules. |
| [Asset Dependencies](./Asset-Dependencies.md) | Required images, masks, materials, plants, rocks, ruins, dungeon markers, and placeholder assets. |
| [UE5 Demo Plan](./UE5-Demo-Plan.md) | Implementation plan for an in-game test map with biome switching on keys 1-8. |

## Core Decision

Use a low-resolution procedural control map as gameplay truth, then render it through biome-specific materials and placement rules. Do not make final AI terrain images authoritative for building placement, resource spawning, combat modifiers, or dungeon discovery.

## See Also

- [Biomes](../Biomes/README.md)
- [Planet Types](../Planet-Types/README.md)
- [Planet Events](../Planet-Events/README.md)
- [Building Grid Placement](../../buildings/Grid-Placement.md)
- [Building Biome Compatibility](../../buildings/Biome-Compatibility.md)
