# Planet Terrain Asset Dependencies

## MVP Assets

These are enough to build and test the first in-game terrain demo.

| Asset | Type | Needed For | MVP Source |
|---|---|---|---|
| Height map generator | Code | Shared terrain shape. | Procedural noise. |
| Tile role debug texture | Runtime texture | Green/yellow/red/blue/purple/cyan/white overlay. | Generated from tile data. |
| Biome terrain materials | Material instances | Desert, Dusty, Rocky, Water, Swamp, Jungle, Light Snow, Ice. | Final-quality generated texture references first, then UE material instances. |
| Terrain mesh | Procedural mesh or grid mesh | Surface rendering. | Runtime generated mesh. |
| Blocker props | Static meshes | Rocks, cliffs, trees, deep swamp, ice cracks. | Final-quality 2.5D prop references first. |
| Resource node props | Static meshes | Ore, oil, crystal, biofuel, geothermal, salvage. | Final-quality 2.5D prop references first. |
| Dungeon/lost-place markers | Static meshes | Ruins, bunker entrances, alien structures. | Final-quality 2.5D prop references first. |
| Encounter markers | Debug actors/decals | Restricted yellow zones, patrol/ambush areas. | Runtime debug decals. |
| Landing zone marker | Decal/mesh | Dropship start and build-safe area. | Runtime debug decal. |

## Production Asset Families

Create production visuals after the MVP proves map scale and camera readability.

| Family | Examples | Biomes |
|---|---|---|
| Rocks and cliffs | Boulder clusters, cliff plates, crater rims, lava vents. | Rocky, Desert, Dusty, Ice, Light Snow |
| Vegetation | Trees, roots, bushes, vines, mushroom growth, swamp reeds. | Jungle, Swamp, Water coast |
| Water/swamp surfaces | Shallow water, deep water, mud, floating algae, foam edges. | Water, Swamp |
| Snow/ice features | Snow drifts, ice ridges, cracks, crystal clusters. | Light Snow, Ice |
| Ruins/lost places | Abandoned houses, military camps, bunkers, alien structures, vehicle graveyards. | All |
| Resource nodes | Ore veins, oil seeps, crystal nodes, geothermal vents, biofuel growths, salvage piles. | Biome-specific |
| Hazard markers | Lava bursts, spore clouds, dust storm emitters, frost cracks. | Biome-specific |

## AI Image Use

AI images are useful for:

- biome mood boards
- terrain material reference
- concepting plants, rocks, ruins, and dungeons
- final 2.5D prop sheets after the camera angle is locked

AI images should not be used as the authoritative gameplay map. They are hard to validate and hard to edit precisely. Gameplay should come from generated masks and DataAssets.

## 2.5D 45-Degree Asset Guidance

If the game keeps a 45-degree RTS camera, final props should be authored or generated with:

- consistent ground contact point
- readable top-down silhouette
- limited tiny surface detail
- strong biome color difference
- 2-4 LODs or simple Nanite-safe meshes where appropriate
- collision separate from visual mesh
- no gameplay-relevant detail hidden behind foliage

## Recommended Asset Order

1. Generate final-quality reference images and example props for one biome.
2. Use those assets as the visual target for the first terrain demo.
3. Test biome switching, buildability, resources, and discovery.
4. Lock grid scale and camera angle.
5. Generate or model the remaining biome prop variants.
6. Build biome prop palettes.
7. Add PCG placement rules.
8. Add final materials and effects.

This keeps the system maintainable. The terrain tech should not wait for final art.

## Desert Minimal Asset Pack

The first complete biome pass is Desert. Assets are stored under:

`UE5/Assets/Planets/Terrain/Desert`

Folder structure:

| Folder | Purpose |
|---|---|
| `Ground/` | Tiling terrain material references for procedural surface blending. |
| `Dunes/` | Dune-specific terrain material references and later dune props. |
| `Rocks/` | Blocker rocks, cliff chunks, sandstone obstacles. |
| `Plants/` | Dry shrubs, roots, dead bushes, non-blocking or light-blocking decoration. |
| `Resources/` | Mineral/ore nodes with quality variants. |
| `Dungeons/` | Lost-place entrances, buried bunkers, ruins. |
| `Hazards/` | Restricted/encounter tile visuals such as gullies and sinkholes. |
| `Landing/` | Dropship landing zone markers and prepared ground. |
| `StartLocation/` | Main crashed ship / starting base landmark. |

Current Desert ground references:

| Asset | Purpose |
|---|---|
| `Ground/DES-GRD-00_MixedGround.png` | Broad mixed desert reference. |
| `Ground/DES-GRD-01_FlatSand.png` | Buildable flat sand. |
| `Ground/DES-GRD-02_CrackedHardpan.png` | Dry rough terrain variation. |
| `Ground/DES-GRD-03_RockyDust.png` | Blocked-edge and slope blending. |
| `Ground/DES-GRD-04_WornPathLandingDust.png` | Roads, landing areas, heavy-use areas. |
| `Dunes/DES-DUN-01_DuneRipple.png` | Dune/open-sand visual variation. |
| `Dunes/DES-DUN-02_DuneBlocker_2x3.png` | Soft dune blocker or movement-slowing obstacle. |

Current Desert prop references:

| Asset | Tile Role / Use |
|---|---|
| `Rocks/DES-ROK-01_SandstoneBlocker.png` | Blocked tile. |
| `Rocks/DES-ROK-02_LowSandstoneBlocker.png` | Smaller blocked tile or partial obstruction. |
| `Rocks/DES-ROK-03_Ridge_1x3.png` | Long ridge blocker for path shaping. |
| `Rocks/DES-ROK-04_BoulderCluster_2x2.png` | Compact multi-tile boulder cluster. |
| `Rocks/DES-ROK-05_CliffShelf_3x2.png` | Wide cliff shelf / mesa edge. |
| `Rocks/DES-ROK-06_Mesa_3x3.png` | Large landmark blocker. |
| `Rocks/DES-ROK-07_CanyonWall_LShape.png` | Irregular canyon turn blocker. |
| `Plants/DES-PLT-01_DryShrubRootCluster.png` | Decoration or light blocker. |
| `Plants/DES-PLT-02_DryGrassTufts.png` | Small non-blocking decoration. |
| `Plants/DES-PLT-03_AlienSucculent.png` | Desert alien plant decoration or light blocker. |
| `Resources/DES-RSC-02_SubtleMineralRock.png` | Poor mineral node. |
| `Resources/DES-RSC-03_GoodMineralVein.png` | Good mineral vein. |
| `Resources/DES-RSC-01_CyanCrystalNode.png` | Rare/high-value node or debug-friendly resource marker. |
| `Resources/DES-RSC-04_FuelSeepSalvage.png` | Fuel node or polluted salvage resource. |
| `Resources/DES-RSC-05_SalvagePile.png` | Construction material / hull parts salvage node. |
| `Dungeons/DES-DNG-01_BuriedBunkerEntrance.png` | Dungeon/lost-place tile. |
| `Dungeons/DES-DNG-02_RuinedRadarStation.png` | Lost-place / salvage landmark. |
| `Hazards/DES-HAZ-01_RestrictedGully.png` | Restricted/encounter tile. |
| `Hazards/DES-HAZ-02_DustGeyserVent.png` | Restricted hazard / vent field. |
| `Landing/DES-LND-01_DropshipLandingPad.png` | Landing tile area. |
| `Landing/DES-LND-02_EmergencyBeaconPad.png` | Alternate landing marker. |
| `StartLocation/DES-STR-01_CrashedWelcomeShip_6x4.png` | Main starting location, based on the welcome screen broken ship. |

Resource readability decision:

- Poor nodes should be mostly natural rock with subtle ore/mineral hints.
- Good nodes should show a visible mineral vein and a few small crystals.
- Large glowing crystal clusters are reserved for rare/high-value nodes or debug readability, not normal resources.

Blocker footprint decision:

- Blockers should not be only `1x1` props.
- The generator should choose visual variants by footprint and shape so blocked areas look natural.
- Each blocker asset should eventually have metadata: biome, tile role, footprint, allowed rotations, collision type, visual weight, and placement tags.

Recommended Desert blocker footprints:

| Footprint | Use |
|---|---|
| `1x1` | Small rock, shrub, debris, minor obstruction. |
| `1x2` / `2x1` | Short ridge, low wall, tight path shaping. |
| `1x3` / `3x1` | Long ridge, canyon lip, route blocker. |
| `2x2` | Boulder cluster, compact rock island. |
| `3x2` / `2x3` | Cliff shelf, mesa edge, larger blocked shape. |
| `3x3` | Large mesa, landmark rock island, strong path blocker. |
| Irregular / L-shape | Natural canyon turns and less grid-obvious blocked zones. |

## Dusty Minimal Asset Pack

The second biome pass is Dusty, based on `BIO-REF-02_Dusty.png`.

Assets are stored under:

`UE5/Assets/Planets/Terrain/Dusty`

Folder structure:

| Folder | Purpose |
|---|---|
| `Ground/` | Dusty industrial terrain material references. |
| `Rocks/` | Natural rocky blockers. |
| `Scrap/` | Industrial debris blockers and salvage obstacles. |
| `Resources/` | Ore, fuel, salvage, and mine-related resource nodes. |
| `Dungeons/` | Mining bunkers and lost industrial facilities. |
| `Hazards/` | Dust vents, unstable pits, radiation/waste zones. |
| `Landing/` | Dusty landing markers. |
| `StartLocation/` | Crash or base-start landmarks if this biome starts a run. |

Current Dusty assets:

| Asset | Tile Role / Use |
|---|---|
| `Ground/DUS-GRD-01_FlatDust.png` | Buildable flat dusty ground. |
| `Ground/DUS-GRD-02_MiningScars.png` | Rough mining-scar/path material. |
| `Scrap/DUS-SCP-01_IndustrialScrapBlocker_2x2.png` | Industrial blocked tile cluster. |
| `Resources/DUS-RSC-01_DustyOreVein.png` | Grounded ore resource node. |
| `Dungeons/DUS-DNG-01_MiningBunkerEntrance.png` | Dungeon/lost-place mining entrance. |
