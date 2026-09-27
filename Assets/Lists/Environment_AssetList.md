# Planet Environment & Biome Asset List

Production inventory for planet surfaces, biome kits, landscape materials, traversal geometry, resource deposits, weather effects, dungeon entrances, and environmental gameplay feedback in **Our Last Chance**.

The first production target is **Stil-1: Dark Realistic Hard Sci-Fi**. This document supplements:

- [Planet Buildings](Building_AssetList.md)
- [Playable Units and Hostiles](Unit_AssetList.md)
- [User Interface](UI_AssetList.md)

---

## Gameplay and Presentation Rules

- Planet gameplay is real-time RTS; dungeon interiors are real-time hack-and-slay.
- Terrain must remain readable from the fixed 40° high three-quarter RTS camera.
- Buildable ground, blocked terrain, cover, shallow water, deep water, hazards, and traversal edges must be distinguishable without relying on UI text.
- Natural terrain is realistic and grounded. Industrial decals and constructed surfaces inherit the Stil-1 gunmetal/amber language.
- Large forms take priority over noisy micro-detail.
- Biome transitions must blend naturally; hard square tile seams are not allowed.
- Environment assets must preserve building footprint readability and unit silhouettes.
- Routine repair feedback uses Construction Material only. Environmental mineral colors identify deposits, not repair costs.
- No turn counters or turn-based environmental states.

---

## ID Naming Convention

| Prefix | Category |
|--------|----------|
| `ENV-SH` | Shared landscape shaders and material functions |
| `ENV-GP` | Shared gameplay surfaces and decals |
| `ENV-PR` | Shared natural props |
| `ENV-RS` | Resource deposits |
| `ENV-WT` | Water and shoreline assets |
| `ENV-WX` | Weather and ambient VFX |
| `ENV-DES` | Desert biome |
| `ENV-DST` | Dusty biome |
| `ENV-RKY` | Rocky biome |
| `ENV-WTR` | Water biome |
| `ENV-SWP` | Swamp biome |
| `ENV-JNG` | Jungle biome |
| `ENV-SNW` | Light Snow biome |
| `ENV-ICE` | Ice biome |
| `ENV-DUN` | Dungeon and abandoned-structure entrances |
| `ENV-BOS` | Hostile-area and boss arenas |
| `ENV-PCG` | Procedural generation graphs and rules |

**Format:** `ENV-{CATEGORY}-{SEQ}-{DESCRIPTOR}`

---

# Phase 0 — Rocky/Dusty Vertical Slice

This phase supplies everything required for the first playable planet: base placement, mining, vehicle traversal, surface combat, one abandoned structure, one small dungeon route, and dynamic dust weather.

## Shared Landscape Foundation

| Asset ID | Deliverable | Type | Priority |
|----------|-------------|------|:--------:|
| `ENV-SH-01` | Master Planet Landscape Material | UE5 material | P0 |
| `ENV-SH-02` | Distance-Based Macro/Micro Tiling Function | Material function | P0 |
| `ENV-SH-03` | Slope and Height Auto-Material Function | Material function | P0 |
| `ENV-SH-04` | Runtime Virtual Texture Integration | Material function | P0 |
| `ENV-SH-05` | Building/Foundation Terrain Blend | Material function | P0 |
| `ENV-SH-06` | Wetness, Snow and Dust Overlay Function | Material function | P0 |
| `ENV-SH-07` | Biome Transition Blend Function | Material function | P0 |
| `ENV-SH-08` | Unit/Vehicle Track Projection Function | Material function | P0 |

**Required Landscape Layers:**

- [ ] Exposed bedrock — `ENV-SH-01-Layer-Bedrock`
- [ ] Compacted dirt — `ENV-SH-01-Layer-Dirt`
- [ ] Loose gravel — `ENV-SH-01-Layer-Gravel`
- [ ] Fine dust — `ENV-SH-01-Layer-Dust`
- [ ] Scree and broken stone — `ENV-SH-01-Layer-Scree`
- [ ] Constructed roadbed — `ENV-SH-01-Layer-Roadbed`
- [ ] Foundation compression blend — `ENV-SH-01-Layer-Foundation`

## Gameplay Surface Feedback

| Asset ID | Asset | Requirements |
|----------|-------|--------------|
| `ENV-GP-01` | Buildable Ground Projection | Subtle green grid/edge state; toggleable; conforms to terrain |
| `ENV-GP-02` | Blocked Ground Projection | Red obstruction cells; readable through dust and snow |
| `ENV-GP-03` | Building Footprint Preview | Supports 1×1 through 6×6 footprints |
| `ENV-GP-04` | Unit Movement Destination | Short-lived directional ground marker |
| `ENV-GP-05` | Vehicle Path Preview | Wide path spline with turn-radius feedback |
| `ENV-GP-06` | Cover Indicator Decal | Natural half/full-cover edge visualization |
| `ENV-GP-07` | Shallow/Deep Water Boundary | World-space depth readability |
| `ENV-GP-08` | Hazard Boundary | Lava, toxic, unstable ice and radiation variants |
| `ENV-GP-09` | Selection Grounding Shadow | Soft controllable contact ellipse for units only |
| `ENV-GP-10` | Construction Zone Dust Mask | Prevents terrain clutter beneath structures |

## Rocky Biome Starter Kit

### Landscape Surfaces

- [ ] Gray-brown exposed bedrock material — `ENV-RKY-01-Surface-Bedrock`
- [ ] Fractured rock material — `ENV-RKY-02-Surface-Fractured`
- [ ] Gravel and scree material — `ENV-RKY-03-Surface-Scree`
- [ ] Mineral-stained stone material — `ENV-RKY-04-Surface-Mineral`
- [ ] Volcanic dark-rock material — `ENV-RKY-05-Surface-Volcanic`

### Modular Geometry

- [ ] Low cliff segment, straight — `ENV-RKY-10-Cliff-Low-Straight`
- [ ] Low cliff inner/outer corners — `ENV-RKY-11-Cliff-Low-Corners`
- [ ] High cliff segment, straight — `ENV-RKY-12-Cliff-High-Straight`
- [ ] High cliff inner/outer corners — `ENV-RKY-13-Cliff-High-Corners`
- [ ] Cliff end caps and transition ramps — `ENV-RKY-14-Cliff-Transitions`
- [ ] Natural traversal ramp — `ENV-RKY-15-Ramp-Natural`
- [ ] Narrow canyon blocker — `ENV-RKY-16-Canyon-Blocker`
- [ ] Elevated firing plateau — `ENV-RKY-17-Plateau-Cover`

### Scatter Props

- [ ] Small rock scatter set — `ENV-RKY-20-Scatter-Small`
- [ ] Medium boulder set — `ENV-RKY-21-Boulders-Medium`
- [ ] Large landmark boulder set — `ENV-RKY-22-Boulders-Large`
- [ ] Layered slab formations — `ENV-RKY-23-Formation-Slabs`
- [ ] Natural defensive rock wall — `ENV-RKY-24-Cover-RockWall`
- [ ] Scree pile clusters — `ENV-RKY-25-Scatter-Scree`
- [ ] Dry crevice and fissure decals — `ENV-RKY-26-Decal-Fissures`
- [ ] Geological survey marker prop — `ENV-RKY-27-Prop-SurveyMarker`

**Visual Notes:** Rocky formations provide natural cover and support the documented +15% wall-HP biome bonus. Cover silhouettes must not resemble player-built walls.

## Dusty Biome Starter Kit

### Landscape Surfaces

- [ ] Brown-tan compacted hardpan — `ENV-DST-01-Surface-Hardpan`
- [ ] Fine particulate dust — `ENV-DST-02-Surface-Dust`
- [ ] Wind-rippled dust sheet — `ENV-DST-03-Surface-Ripples`
- [ ] Exposed dry clay — `ENV-DST-04-Surface-Clay`
- [ ] Coal-stained ground — `ENV-DST-05-Surface-CoalDust`

### Modular Geometry and Props

- [ ] Low eroded ridge set — `ENV-DST-10-Ridge-Low`
- [ ] Dust bank and drift set — `ENV-DST-11-Drift`
- [ ] Dry wash/channel set — `ENV-DST-12-DryWash`
- [ ] Wind-carved pillar landmarks — `ENV-DST-13-Pillars`
- [ ] Buried industrial debris — `ENV-DST-14-Debris-Industrial`
- [ ] Abandoned filter station prop — `ENV-DST-15-Prop-FilterStation`
- [ ] Dust accumulation mesh decals — `ENV-DST-16-Decal-Accumulation`
- [ ] Vehicle rut decals — `ENV-DST-17-Decal-Ruts`

## Shared Surface Damage and Tracks

- [ ] Infantry footprint decal set — `ENV-GP-20-Decal-Footprints`
- [ ] Wheeled vehicle track set — `ENV-GP-21-Decal-WheelTracks`
- [ ] Tracked vehicle tread set — `ENV-GP-22-Decal-TankTracks`
- [ ] Mech footprint set — `ENV-GP-23-Decal-MechTracks`
- [ ] Ballistic impact decals — `ENV-GP-24-Decal-Ballistic`
- [ ] Energy scorch decals — `ENV-GP-25-Decal-Energy`
- [ ] Explosion crater small/medium/large — `ENV-GP-26-Decal-Craters`
- [ ] Building destruction scorch mask — `ENV-GP-27-Decal-BuildingDestroyed`
- [ ] Temporary construction disturbance — `ENV-GP-28-Decal-Construction`

---

# Resource Deposit Kit

Deposits are world props and placement anchors. Their silhouettes must remain recognizable before a mine is constructed.

| Asset ID | Deposit | Primary Read | VFX Socket |
|----------|---------|--------------|------------|
| `ENV-RS-01` | Stone Outcrop | Gray broken slabs | Gray rock dust |
| `ENV-RS-02` | Metal Ore Vein | Silver metallic seams | Fine silver sparks/dust |
| `ENV-RS-03` | Coal Seam | Near-black fractured seam | Black dust |
| `ENV-RS-04` | Sulfur Deposit | Ochre/yellow crust | Yellow vapor |
| `ENV-RS-05` | Uranium Deposit | Dark stone with contained emissive inclusions | Controlled radioactive glow |
| `ENV-RS-06` | Crystal Cluster | Angular translucent formation | Resource-specific glow |
| `ENV-RS-07` | Titanium Outcrop | Silver-blue dense plates | Metallic glint |
| `ENV-RS-08` | Oil Seep | Dark viscous surface pocket | Thin vapor/bubble |
| `ENV-RS-09` | Gas Vent | Porous rock chimney | Distortion plume |
| `ENV-RS-10` | Geothermal Fissure | Hot fractured stone | Steam/heat distortion |
| `ENV-RS-11` | Frozen Methane Deposit | Pale dense ice pockets | Cold vapor |
| `ENV-RS-12` | Dark Matter Crystal | Near-black crystal with purple edge energy | Purple containment flicker |

Every deposit requires:

- [ ] Untouched state
- [ ] Active extraction state
- [ ] Depleted state
- [ ] Placement/socket transform for extraction buildings
- [ ] Minimap silhouette
- [ ] Collision and navigation classification

---

# Shared Water and Shoreline Kit

| Asset ID | Asset | Notes |
|----------|-------|-------|
| `ENV-WT-01` | Master Water Material | Depth, flow, contamination, ice and foam parameters |
| `ENV-WT-02` | Shallow Water Material Instance | Traversable where gameplay permits |
| `ENV-WT-03` | Deep Water Material Instance | Blocks normal ground units |
| `ENV-WT-04` | Straight Shoreline Segment | Terrain-blended wet edge |
| `ENV-WT-05` | Concave/Convex Shore Corners | No visible seam at 40° |
| `ENV-WT-06` | Rocky Shore Transition | Boulder and foam integration |
| `ENV-WT-07` | Muddy Shore Transition | Swamp/water integration |
| `ENV-WT-08` | Frozen Shore Transition | Light Snow/Ice integration |
| `ENV-WT-09` | Water Splash Niagara System | Unit-size parameters |
| `ENV-WT-10` | Vehicle Wake Niagara System | Speed and hull-width parameters |
| `ENV-WT-11` | Water Turbine Disturbance | Runtime attachment effect |
| `ENV-WT-12` | Underwater Visibility Function | For submerged turbine and deposits |

---

# Weather and Ambient VFX

## Rocky/Dusty Vertical Slice

- [ ] Light windborne dust — `ENV-WX-01-Dust-Light`
- [ ] Full dust storm — `ENV-WX-02-Dust-Storm`
- [ ] Dust gust ground sheet — `ENV-WX-03-Dust-Gust`
- [ ] Heat shimmer volume — `ENV-WX-04-Heat-Shimmer`
- [ ] Loose gravel wind movement — `ENV-WX-05-Gravel-Wind`
- [ ] Distant particulate atmosphere — `ENV-WX-06-Atmosphere-Dust`
- [ ] Storm visibility gameplay mask — `ENV-WX-07-Visibility-Dust`
- [ ] Lightning/dry electrical storm variant — `ENV-WX-08-Dry-Lightning`

## Shared Weather Framework

- [ ] Biome weather controller — `ENV-WX-20-Controller`
- [ ] Wind direction and strength material parameter collection — `ENV-WX-21-MPC-Wind`
- [ ] Surface accumulation controller — `ENV-WX-22-Accumulation`
- [ ] Unit/building exposure interface — `ENV-WX-23-Exposure`
- [ ] Ambient color and fog profile manager — `ENV-WX-24-Atmosphere`

---

# Dungeon and Abandoned-Structure Entrances

These are exterior world assets. Interior modular kits are tracked separately from planet terrain.

| Asset ID | Entrance | Size | Discovery Read |
|----------|----------|------|----------------|
| `ENV-DUN-01` | Abandoned House/Shelter | Small | Collapsed doorway and weak signal |
| `ENV-DUN-02` | Military Camp Gate | Medium | Searchlights, barriers, patrol route |
| `ENV-DUN-03` | Hospital Complex Entrance | Medium | Clean structural remnants and emergency lights |
| `ENV-DUN-04` | Lost Bunker Blast Door | Large | Reinforced embedded door and power lock |
| `ENV-DUN-05` | Alien Structure Threshold | Variable | Non-human silhouette and contained energy |
| `ENV-DUN-06` | Vehicle Graveyard Access | Medium | Wreck corridor and salvage beacon |
| `ENV-DUN-07` | Oil Rig Ruin Access | Medium | Leaking industrial platform and lower hatch |
| `ENV-DUN-08` | Natural Cave Entrance | Small/Large | Rock-integrated opening and depth fog |
| `ENV-DUN-09` | Crashed Satellite Site | Small | Debris trail and recoverable module |
| `ENV-DUN-10` | Mining Colony Access Shaft | Medium | Lift frame, dust filters and sealed shaft |

Every entrance requires:

- [ ] Undiscovered silhouette state
- [ ] Discovered/interactive state
- [ ] Cleared state
- [ ] Locked/breaching state where applicable
- [ ] Navigation and interaction sockets
- [ ] Map/minimap marker anchor

---

# Full Biome Production Kits

## Desert — `ENV-DES`

**Surface Layers:** golden sand, compacted sand, sandstone, salt crust, glass-rich patches.

**Required Assets:**

- Dune crest, slope and basin meshes
- Wind-ripple decals
- Sandstone cliffs and arches
- Dry scrub and mineral crust props
- Quicksand hazard surface
- Sand accumulation around buildings
- Buried ruin debris
- Dust-devil and full sandstorm VFX
- Sun-baked ruin hostile-area kit
- Sand Worm Titan arena anchors

## Dusty — `ENV-DST`

**Surface Layers:** hardpan, fine dust, clay, coal stain, eroded stone.

**Required Assets:**

- Starter kit listed above
- Abandoned mining-colony exterior
- Filtration towers and broken duct props
- Particulate storm shelters
- Coal seam landmarks
- Dust Storm Elemental arena

## Rocky — `ENV-RKY`

**Surface Layers:** bedrock, fractured stone, gravel, scree, volcanic stone.

**Required Assets:**

- Starter kit listed above
- Geothermal fissure clusters
- Lava channel and cooled-lava transitions
- Volcanic vent props
- Titanium-rich outcrop variants
- Crater fortress exterior
- Magma Golem Lord arena

## Water — `ENV-WTR`

**Surface Layers:** coastal rock, wet sand, seabed sediment, reef stone, platform foundation.

**Required Assets:**

- Deep ocean and coastal water instances
- Beach/rock shoreline modules
- Shallow-water traversal zones
- Reef and seabed prop sets
- Floating debris and buoy markers
- Buildable coastal platform kit
- Underwater oil/crystal deposits
- Sunken facility exterior
- Abyssal Leviathan arena/tether anchors

## Swamp — `ENV-SWP`

**Surface Layers:** saturated mud, peat, contaminated water, root mat, sulfur crust.

**Required Assets:**

- Mud bank and shallow-channel modules
- Dead trees, roots and hanging growth
- Reed and fungal scatter sets
- Contaminated water material instance
- Oil/biofuel seep variants
- Toxic spore vent props and VFX
- Movement-slow mud volumes
- Overgrown alien research station
- Spore Queen Mother arena

## Jungle — `ENV-JNG`

**Surface Layers:** dark soil, leaf litter, exposed roots, wet rock, shallow water.

**Required Assets:**

- Canopy tree family with PCG variants
- Understory plants and broad-leaf clusters
- Root blockers and traversal arches
- Fallen logs and natural cover
- Vine and moss decals
- Canopy shadow/solar-efficiency mask
- Destructible vegetation subset
- Ancient temple exterior
- Jungle Predator Alpha arena and camouflage anchors

## Light Snow — `ENV-SNW`

**Surface Layers:** thin snow, exposed frozen soil, compacted snow, frost rock, slush.

**Required Assets:**

- Snow accumulation material function instances
- Light-snow rock and cliff variants
- Frozen scrub and dead-tree props
- Footprint and vehicle compression decals
- Heated foundation melt mask
- Light snowfall and frost-event VFX
- Frozen military-outpost exterior
- Frost Warden arena

## Ice — `ENV-ICE`

**Surface Layers:** opaque ice sheet, translucent deep ice, wind-scoured ice, snow crust, crystal ice.

**Required Assets:**

- Thick/thin ice material instances
- Crack growth decals and Niagara effect
- Breakable thin-ice gameplay mesh
- Ice cliffs, pressure ridges and crevasses
- Frozen methane vents
- Crystal and dark-matter deposit variants
- Anchored foundation sockets
- Blizzard and polar fog VFX
- Polar alien monolith exterior
- Ice Colossus arena and split-phase anchors

---

# Biome Transition Assets

| Asset ID | Transition |
|----------|------------|
| `ENV-SH-30` | Rocky ↔ Dusty |
| `ENV-SH-31` | Desert ↔ Dusty |
| `ENV-SH-32` | Rocky ↔ Light Snow |
| `ENV-SH-33` | Light Snow ↔ Ice |
| `ENV-SH-34` | Jungle ↔ Swamp |
| `ENV-SH-35` | Swamp ↔ Water |
| `ENV-SH-36` | Rocky ↔ Water |
| `ENV-SH-37` | Jungle ↔ Rocky |

Each transition requires blended landscape materials, matching scatter rules, shoreline/cliff connectors where relevant, and PCG exclusion masks.

---

# Procedural Generation and PCG Rules

- [ ] Biome surface distribution graph — `ENV-PCG-01-Surfaces`
- [ ] Rock and cliff placement graph — `ENV-PCG-02-Rocks`
- [ ] Vegetation placement graph — `ENV-PCG-03-Vegetation`
- [ ] Resource deposit distribution graph — `ENV-PCG-04-Resources`
- [ ] Dungeon/structure placement graph — `ENV-PCG-05-Structures`
- [ ] Navigable corridor preservation graph — `ENV-PCG-06-Navigation`
- [ ] Building-clearance exclusion graph — `ENV-PCG-07-Buildable`
- [ ] Landmark and vista graph — `ENV-PCG-08-Landmarks`
- [ ] Weather-zone assignment graph — `ENV-PCG-09-Weather`
- [ ] Hostile-area perimeter graph — `ENV-PCG-10-HostileAreas`

**Generation Constraints:**

- Preserve connected buildable regions large enough for at least one 6×6 structure.
- Never spawn resource deposits beneath cliffs, dungeon entrances, water boundaries, or initial dropship clearance.
- Maintain readable lanes for infantry, vehicles, and large 2×2 units.
- Avoid uniform prop spacing and visible procedural grids.
- Limit high-frequency clutter around combat and construction zones.

---

# UE5 Technical Requirements

## Landscape Materials

- Use material instances per biome from one shared master.
- Support Runtime Virtual Textures for building, road, decal, and mesh blending.
- Use world-aligned macro variation to suppress visible tiling.
- Target no more than four dominant painted layers per landscape component.
- Expose wetness, dust, snow, contamination, scorch and temperature parameters.
- Support Virtual Shadow Maps and Lumen.

## Geometry

- Nanite enabled for cliffs, boulders and dense static landmark geometry.
- Simple authored collision for navigation-critical rocks and cliffs.
- Separate visual mesh from gameplay blocker volume.
- Pivot and connector rules for modular cliffs and shorelines.
- HLOD clusters for distant environment groups.

## Texture Targets

| Asset Type | Target |
|------------|--------|
| Landscape surface set | 2048–4096 px tileable |
| Hero cliff/landmark | 2048–4096 px |
| Medium prop | 1024–2048 px |
| Small scatter | 512–1024 px |
| Decal atlas | 2048 px |
| VFX flipbook | 1024–2048 px atlas |

## Collision and Navigation

Every environmental asset must declare:

- Walkable, slow, blocked, hazardous, shallow-water, deep-water, or destructible.
- Infantry, light-vehicle, heavy-vehicle and aerial traversal compatibility.
- Cover height and direction where relevant.
- PCG exclusion radius.
- Building placement exclusion or foundation compatibility.

---

# Production Order

1. Shared master landscape material and gameplay projections
2. Rocky and Dusty surface layers
3. Rocky cliffs, ramps, blockers and cover
4. Shared resource-deposit kit
5. Tracks, craters and surface damage
6. Dust weather framework
7. Water edge and shoreline starter kit
8. Abandoned shelter and lost-bunker entrances
9. Desert and Light Snow kits
10. Water and Swamp kits
11. Jungle and Ice kits
12. Hostile-area/boss arena extensions

---

# Asset Count Summary

| Category | Estimated Assets/Systems |
|----------|:------------------------:|
| Shared landscape/material systems | 8+ |
| Gameplay projections and decals | 20+ |
| Rocky/Dusty vertical-slice surfaces and props | 35+ |
| Resource deposits | 12 families × 3 states |
| Water/shoreline systems | 12+ |
| Weather and atmosphere systems | 13+ |
| Dungeon/structure entrances | 10 families × multiple states |
| Full biome kits | 8 |
| Biome transitions | 8 |
| PCG graphs | 10 |

The final runtime file count will be higher because material instances, LOD/HLOD data, collision meshes, Niagara systems, PCG graphs, and state variants are separate deliverables.

---

## See Also

- [Planet types and biome modifiers](../Briefing/planet/planet_types.md)
- [Planet events and dungeon entrances](../Briefing/planet/planet_events.md)
- [Resource deposit definitions](../Briefing/resources/types.md)
- [Stil-1 visual guide](Style/Stil-1/STYLE_GUIDE.md)
- [Planet building compatibility](../Briefing/buildings/planet_buildings.md)
