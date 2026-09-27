# Planet Terrain Strategy

## Goal

Build a UE5-ready terrain system where every planet surface can be generated from a seed, a height map, and layered tile masks. The same terrain layout must be renderable as Desert, Dusty, Rocky, Water, Swamp, Jungle, Light Snow, or Ice so we can test gameplay readability and biome identity quickly.

## Key Principle

Gameplay truth lives in data. Visual terrain is generated from that data.

Do not use one large painted AI image as the only map definition. A beautiful bitmap is useful as a reference or optional surface detail, but building placement, resources, hazards, dungeons, patrol routes, and fog reveal need stable machine-readable layers.

## Map Stack

Each generated planet surface should contain these layers:

| Layer | Resolution | Purpose |
|---|---:|---|
| Height Map | 128-512 square | Elevation, slopes, cliffs, basin/water placement. |
| Tile Role Map | 128-512 square | Buildable, blocked, restricted, water, dungeon, resource, hostile, landing. |
| Biome Surface Map | 128-512 square | Local material variation inside the dominant biome. |
| Resource Map | 128-512 square | Resource node candidates and richness. Default is zero until generated or discovered. |
| Encounter Map | 128-512 square | Patrol, ambush, hostile area, event, boss approach zones. |
| Decoration Density Map | 128-512 square | Plants, rocks, wrecks, crystals, ruins, clutter density. |
| Reveal Map | Runtime | Fog of war and discovery state. |

Recommended MVP size: 128x128 for the first test map, 256x256 for normal planets, 512x512 for large late-game planets.

## Tile Roles

Use a small enum instead of raw color checks in gameplay code.

| Role | Debug Color | Meaning |
|---|---|---|
| Buildable | Green | Valid building placement after footprint validation. |
| Restricted | Yellow | No buildings; encounter, quest, patrol, or future event area. |
| Blocked | Red | Cliffs, rocks, trees, deep swamp, cracked ice, ruins, map edge. |
| Water | Blue | Water/swamp/liquid tiles; usable by water-specific buildings. |
| Resource | Cyan | Resource node or extractable deposit candidate. |
| Dungeon | Purple | Dungeon entrance, lost place, hostile structure, boss approach. |
| Landing | White | Valid dropship/base start candidate. |
| Road | Orange | Optional path preference for units/patrols. |
| Empty/Outside | Black | Outside playable area or no terrain. |

The debug color can exist as a bitmap for fast inspection, but importing should convert it into tile data.

## Generation Flow

1. Choose `PlanetSeed`, dominant biome, planet size, TIR, modifiers, and hostile-area type.
2. Generate height map using layered noise and biome profile parameters.
3. Derive water, cliff, slope, and basin masks from height.
4. Generate initial tile roles from slope, water, edge falloff, and biome rules.
5. Stamp required gameplay zones:
   - dropship landing zone
   - early build-safe area
   - hostile area
   - dungeon/lost place entrance
   - resource fields
   - restricted encounter areas
6. Validate:
   - at least one reachable landing zone
   - enough buildable cells for starter base
   - resources are reachable after discovery
   - dungeon entrance is reachable
   - hostile area is not too close to landing unless intentionally dangerous
7. Render terrain mesh/materials from height and biome.
8. Spawn biome props through PCG/Instanced Static Mesh placement from tile roles and decoration density.
9. Keep the reveal/discovery layer separate so resources and dungeons can start hidden.

## UE5 Data Model

The existing project already has `EOLCBiomeType` and `UOLCBiomeData`. Extend that pattern rather than creating a separate terrain-only biome enum.

Proposed new runtime/data types:

| Type | Kind | Purpose |
|---|---|---|
| `EOLCTerrainTileRole` | `UENUM` | Buildable, blocked, restricted, water, resource, dungeon, landing, road. |
| `FOLCTerrainTile` | `USTRUCT` | Height, role, biome type, resource type, richness, discovery state, movement/defense modifiers. |
| `UOLCPlanetTerrainProfile` | `UPrimaryDataAsset` | Per-biome generation weights, material refs, prop sets, hazard sets. |
| `AOLCPlanetTerrainActor` | Actor | Owns generated grid, mesh sections, debug overlay, and biome switching. |
| `UOLCPlanetTerrainGenerator` | UObject or subsystem | Deterministic generation and validation from seed/profile. |

## Biome Gameplay Hooks

| Biome | Terrain Rules | Gameplay Influence |
|---|---|---|
| Desert | Dunes, open flats, rocky ruin blockers. | Solar bonus, dust/sand events, sparse resources. |
| Dusty | Low water, dust basins, mining scars. | Wind bonus, radar penalty during storms. |
| Rocky | High cliffs, craters, boulders, vents. | Geothermal bonus, rock formations improve wall HP. |
| Water | Oceans, islands, coastlines, submerged structures. | Limited buildable land, water turbine priority. |
| Swamp | Shallow/deep water, soft ground, organic blockers. | Toxic spores, oil/biofuel/sulfur, movement risk. |
| Jungle | Dense vegetation, canopy blockers, temple ruins. | Movement penalty, cover bonus, food/biofuel. |
| Light Snow | Frozen flats, ridges, partial snow cover. | Wind bonus, frost events, methane candidates. |
| Ice | Ice sheets, cracks, crystal fields, polar blockers. | Crystal abundance, cracking risk, late-game gating. |

## Resource Philosophy

Planets can generate zero visible resources by default. Resources become useful when discovered by unit exploration, radar, deep sonar, satellite monitoring, or quantum scan.

Surface resources should be spawned as candidates at generation time, then revealed progressively:

| Scan/Discovery | Reveals |
|---|---|
| None | General biome color and safe landing hint. |
| Unit exploration | Nearby salvage, surface structures, small resource nodes. |
| Radar tower | Wider map reveal and dungeon silhouettes. |
| Deep sonar | Buried resources and underground dungeons. |
| Satellite | Planet-wide hostile movement and major sites. |
| Quantum scanner | Complete map disclosure. |

## Asset Strategy

For the first playable demo, generate terrain and simple placeholder props inside UE5. Do not block the terrain technology on final plant/rock art.

Use this order:

1. Procedural mesh/grid, flat colors, debug overlays.
2. Biome material instances with color/noise parameters.
3. Simple placeholder meshes for rocks, trees, crystals, ruins, wrecks, and resource nodes.
4. PCG/Instanced Static Mesh placement using masks.
5. Replace placeholders with final 2.5D/isometric-friendly assets after scale/readability is proven.

For the camera style, assets should be readable from a 45-degree RTS view. That means simple silhouettes, strong top shapes, and consistent base alignment matter more than ultra-detailed close-up geometry.
