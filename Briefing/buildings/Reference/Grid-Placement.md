# Building Grid Placement System

## Grid Rules
- All buildings occupy grid cells (1x1, 2x2, 4x4 sizes as specified per building)
- No connection lines or pipes required between buildings (independent placement)
- Buildings require power to operate — non-powered buildings produce nothing (storage remains functional)
- Each building adds to base attraction/threat level [See attraction system](../../Gameplay/Core-Loop/README.md)

## Grid Sizes
| Size | Cells | Examples |
|------|-------|----------|
| 1x1 | 1 cell | Mine, Oil Pump, Harvester Post, Radar Tower, Wall Segment, Gate |
| 2x1 | 2 cells | Solar Array (wheel-shaped), Wind Turbine (tall) |
| 2x2 | 4 cells | Camp Barracks, Habitation Module, Coal Reactor, Geothermal Vent |
| 3x3 | 9 cells | Forge, Refinery, Command Center, Crystal Synthesizer |
| 4x1 | 4 cells | Container (rotatable) |
| 4x4 | 16 cells | Factory, Haul Storage, Void Lab, Reinforced Vault |
| 6x6 | 36 cells | Assembly Plant, Airfield |

## Placement Rules
- Buildings snap to nearest grid cell
- Rotation (R key) rotates building within its grid footprint
- Overlapping buildings not allowed
- Some buildings require specific terrain (Water Turbine on water tile)

## See Also
- [Storage System](./Storage-System.md) — Grid-based storage mechanics
- [Ship Modules](../../ShipModules/) — Ship hull grid placement
- [Biome Compatibility](./Biome-Compatibility.md) — Terrain-specific building requirements
