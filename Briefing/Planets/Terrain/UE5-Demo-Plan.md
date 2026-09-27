# UE5 Planet Terrain Demo Plan

## Target

Create an in-game UE5 test map that displays one shared generated planet surface and lets the player switch the active biome with keys `1` through `8`.

The purpose is to prove that the same height/tile map can support different biome visuals and gameplay modifiers.

## Confirmed MVP Decisions

| Topic | Decision |
|---|---|
| UE5 project | `UE5/ProjectFiles/OurLastChance` |
| First map size | `128x128` |
| Renderer | Surface mesh and debug tile grid, switchable at runtime. |
| Initial gameplay integration | Visual/debug biome switching first; full gameplay hooks after the demo is visible. |
| Camera | 45-degree RTS-style view, movable with arrow keys. |
| Assets | Production-ready data structure with generated/placeholder visuals first; final 2.5D assets after scale/readability are proven. |

## Demo Controls

| Key | Biome |
|---:|---|
| 1 | Desert |
| 2 | Dusty |
| 3 | Rocky |
| 4 | Water |
| 5 | Swamp |
| 6 | Jungle |
| 7 | Light Snow |
| 8 | Ice |

Optional debug controls:

| Key | Action |
|---|---|
| G | Toggle buildability/debug grid. |
| T | Regenerate same biome with next seed. |
| F | Toggle fog/reveal layer preview. |
| Arrow Keys | Pan camera. |

## Implementation Steps

### 1. Define The Strategy

Document and approve the terrain stack:

- height map
- tile role map
- biome surface map
- resource map
- encounter map
- decoration density map
- reveal map

Done in [Terrain Strategy](./Terrain-Strategy.md).

### 2. Build Dependencies

Create the asset list and decide which assets are placeholders versus final production assets.

Done in [Asset Dependencies](./Asset-Dependencies.md).

### 3. Add Dependencies To UE5

In `UE5/ProjectFiles/OurLastChance`, add or prepare:

| Dependency | Purpose |
|---|---|
| `ProceduralMeshComponent` module | Runtime terrain mesh generation. |
| `EnhancedInput` usage | Biome switching and debug keybinds. |
| Material instances | One terrain material instance per biome. |
| Optional PCG plugin | Later prop placement from masks. |

If avoiding `ProceduralMeshComponent` at first, use Instanced Static Mesh tiles for a simpler MVP. The tile approach is slower visually but easier to debug.

### 4. Build UE5 Planet Tech

Add runtime terrain data and generation:

| File/Class | Purpose |
|---|---|
| `Core/OLCTerrainTypes.h` | Tile role enum, tile struct, terrain generation settings. |
| `Core/OLCPlanetTerrainProfile.h/cpp` | DataAsset for biome-specific terrain generation values and asset refs. |
| `World/OLCPlanetTerrainActor.h/cpp` | Actor that owns generated map, renders mesh/tiles, swaps biome, and draws debug overlay. |
| `World/OLCPlanetTerrainGenerator.h/cpp` | Deterministic generation and validation. |

MVP behavior:

- Generate one `128x128` map from seed.
- Produce height values.
- Mark buildable, blocked, water, resource, restricted, dungeon, and landing tiles.
- Render terrain with current biome material.
- Spawn placeholder blockers/resources/dungeon markers.
- Switch biome without changing the underlying map.

### 5. Build UE5 Planet Biomes

Use the existing `EOLCBiomeType` and `UOLCBiomeData` direction.

Each biome should eventually have:

| Asset | Example |
|---|---|
| Biome data | `DA_Biome_Rocky` |
| Terrain profile | `DA_TerrainProfile_Rocky` |
| Terrain material instance | `MI_Terrain_Rocky` |
| Prop palette | `DA_PropPalette_Rocky` |
| Hazard palette | `DA_HazardPalette_Rocky` |

Biome switching in the demo should update:

- terrain material
- prop palette
- blocked-tile visual set
- resource node visual set
- movement/defense/resource modifiers
- hazard/event flavor

The underlying tile roles stay stable so we can compare readability across biomes.

### 6. Connect Gameplay Effects

Terrain must influence:

| System | Terrain/Biome Input |
|---|---|
| Building placement | Tile role, slope, footprint, biome-specific building compatibility. |
| Building production | Existing building biome modifiers plus local resource availability. |
| Unit movement | Tile role, water/deep swamp, vegetation, snow/ice, gravity modifier. |
| Unit defense | Jungle vegetation cover, rocky wall/cover bonuses, ruins. |
| Resource extraction | Hidden resource candidates revealed by exploration/scanning. |
| Dungeon discovery | Dungeon tiles hidden until reveal conditions are met. |
| Encounters | Yellow restricted zones and hostile-area masks. |

### 7. Build Demo Planet

Create a test map:

`/Game/Maps/TerrainDemo`

Place:

- `AOLCPlanetTerrainActor`
- RTS camera/player controller
- light
- optional debug HUD

When the game starts:

1. Generate the shared terrain map.
2. Apply Desert by default.
3. Show tile-role debug overlay.
4. Allow keys `1..8` to switch biome.
5. Keep resource/dungeon/restricted markers visible in debug mode.

## MVP Visual Result

The first version does not need final plants and rocks. It should prove:

- buildable green spaces remain readable in every biome
- blocked red spaces become biome-specific blockers
- yellow restricted encounter zones remain visible
- blue water/swamp/liquid tiles make sense
- purple dungeon/lost-place zone is obvious
- cyan resource zones can be hidden/revealed later
- switching `1..8` changes the planet feel instantly

## Later Production Pass

After the terrain demo is working:

1. Capture screenshots from the 45-degree RTS camera.
2. Use those screenshots to judge scale and silhouette.
3. Generate/model final biome props.
4. Replace placeholder palettes.
5. Add PCG density rules.
6. Add minimap rendering from the same tile data.
7. Add resource/dungeon reveal progression.

## Why This Order

Final images, plants, rocks, and dungeon props should come after the terrain technology. Otherwise we risk making beautiful assets that do not fit the grid, camera, collision, or gameplay readability.



# Steps To Tech Demo
Left To Finish Terrain Tech Demo
with D:/Development/Our-Last-Chance/UE5/Assets/Planets/Terrain/Desert

Import generated PNG assets into UE5:
- Desert ground textures
- Desert props
- Dusty minimal set
- biome reference images optional, as editor-only references

Convert prop PNGs into usable UE assets:
either temporary billboard/plane sprites for fast review
or use them as references for Static Mesh/3D asset generation later
remove/chroma-key green background if using them directly

Create terrain material system:
- base procedural terrain material
- material slots/layers for ground variants
- Desert material instance
- Dusty material instance
- support mask/blend input from terrain tile data

Extend procedural terrain actor:
- assign ground material based on active biome
- support per-tile/region material blending
- spawn props from biome asset palette
- support multi-tile blocker footprints: 1x1, 1x3, 2x2, 3x2, 3x3, L-shape

Add biome asset palette data:
- DesertTerrainProfile
- DustyTerrainProfile
- blocker prop list
- resource prop list
- dungeon prop list
- hazard prop list
- landing/start-location prop list
- footprint metadata per prop

Add start location:
- place crashed welcome ship as Desert starting landmark
- reserve safe build area around it
- mark landing/start tiles
- prevent blockers/resources spawning too close

Improve terrain generation validation:
- guarantee reachable start area
- guarantee enough buildable tiles
- place blocker clusters using shape footprints
- place poor/good/rare resource nodes
- place dungeon/lost-place at reachable distance
- place restricted/hazard zones away from start

Build UE5 review map:
/Game/Maps/TerrainDemo
- terrain actor placed
- RTS camera pawn
- 45-degree camera
- movable camera with arrow keys
- debug HUD or on-screen biome label

Add runtime controls:
1 Desert
2 Dusty
G surface/debug tile toggle
T regenerate seed
optional F reveal/fog preview

Review modes:
- surface/material mode
- debug tile color mode
- prop placement mode
- resource/dungeon visibility mode

Verify in UE5:
- project compiles
- map opens
- terrain appears
- camera moves
- biome switch works
- debug grid works
- props spawn correctly
- large blockers align to tile footprints
- start ship appears in safe start area

After first review:
- adjust scale of tiles/props
- tune density
- tune buildable/blocked ratio
- decide if PNG billboard assets are enough for demo or need real meshes
- continue generating full Dusty set or move to Rocky/Water next