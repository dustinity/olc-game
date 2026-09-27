# Final Terrain Image Generation Brief

## Goal

Generate final-quality biome and prop imagery for the planet terrain demo instead of waiting on placeholder art.

Gameplay still comes from generated tile data. These images define the final visual target, material direction, and 2.5D/45-degree prop silhouettes that UE5 assets should follow.

## Recommended Output Sizes

| Asset Type | Size | Format | Notes |
|---|---:|---|---|
| Biome key art / mood reference | 3840x2160 | PNG/WebP | Wide 45-degree RTS camera reference. |
| Terrain material reference tile | 2048x2048 | PNG/WebP | Seamless-looking ground surface reference, not gameplay data. |
| Prop sheet | 2048x2048 | PNG | Multiple isolated objects on flat chroma-key background for extraction. |
| Large landmark / dungeon entrance | 2048x2048 | PNG | One object per image, 45-degree view. |
| Resource node sheet | 2048x2048 | PNG | 6-10 readable nodes per sheet. |
| Hazard/event sheet | 2048x2048 | PNG | Visual markers and encounter props. |
| UI/minimap legend reference | 1024x1024 | PNG | Optional, later. |

For direct UE5 import, prefer prop sheets with generous spacing and a flat removable chroma-key background. For concept/reference only, use full scenic backgrounds.

## Core Style Rules

- 45-degree RTS/isometric camera readability.
- Detailed stylized sci-fi survival art, not photoreal noise.
- Strong silhouettes from gameplay camera distance.
- Ground contact point visible and consistent.
- No text, labels, logos, watermarks, UI, humans, or active units.
- Props should look usable as game assets after cleanup/modeling.
- Avoid tiny fragile detail that disappears at RTS scale.
- Keep biome identity strong without making buildable areas unreadable.

## Biome Image List

Generate these first, one image per biome:

| ID | Biome | Image | Purpose |
|---|---|---|---|
| BIO-REF-01 | Desert | Desert planet terrain reference | Dunes, dry flats, stone blockers, salvage hints. |
| BIO-REF-02 | Dusty | Dusty industrial wasteland reference | Brown-gray dust, mining scars, storm-worn metal. |
| BIO-REF-03 | Rocky | Rocky crater planet reference | Cliffs, boulders, basalt shelves, vents. |
| BIO-REF-04 | Water | Ocean/island planet reference | Islands, shallows, reefs, wreckage, wet stone. |
| BIO-REF-05 | Swamp | Toxic swamp planet reference | Mud, reeds, dead trees, algae, pools. |
| BIO-REF-06 | Jungle | Dense alien jungle reference | Canopy blockers, roots, vines, ruin hints. |
| BIO-REF-07 | Light Snow | Frozen frontier reference | Snow flats, exposed rock, frost ridges. |
| BIO-REF-08 | Ice | Deep ice planet reference | Blue-white ice sheets, cracks, crystal fields. |
| BIO-REF-09A | Volcanic Inactive | Dark volcanic planet reference | Cold basalt, ash plains, inactive cone, fumaroles. |
| BIO-REF-09B | Volcanic Active | Active volcanic planet reference | Bright magma, dark biome contrast, active cone, lava hazards. |
| BIO-REF-10 | Mech-Terraformed | Mechanized terrain reference | Artificial grid-cut ground, reinforced paths, machine-built blockers. |
| BIO-REF-11 | Sulfuric Alien | Acid-yellow alien planet reference | Sulfur flats, acidic pools, strange growths, corrosive hazards. |
| BIO-REF-12 | Magnetic Mystic | Purple magnetic anomaly reference | Floating dark rocks, violet fog, anti-gravity blockers, strange energy fields. |
| BIO-REF-13 | Polluted Iridescent | Bright polluted high-TIR reference | Shiny toxic slicks, neon chemical pools, refinery waste, contaminated blockers. |
| BIO-REF-14 | Civilized Marble Garden | Friendly bright normal-world reference | Bright grass, clean stone, marble blocks, ordered peaceful terrain. |

### Biome Reference Prompt Template

```text
Use case: stylized-concept
Asset type: UE5 RTS terrain biome reference, 3840x2160
Primary request: final-quality 45-degree RTS camera view of a {BIOME} planet surface for a sci-fi survival RTS game
Scene/backdrop: a wide readable terrain area with buildable open ground, blocked natural obstacles, restricted encounter zones implied by dangerous terrain, resource-node candidates, and one distant lost-place/dungeon landmark
Subject: planet surface only, no characters, no vehicles in motion, no UI
Style/medium: detailed stylized game concept art, production-ready UE5 visual target, semi-realistic materials with clean readable silhouettes
Composition/framing: 45-degree isometric RTS perspective, camera high enough to judge tile readability, open buildable space in the center, blockers around edges and clusters
Lighting/mood: clear readable daylight with dramatic but not dark atmosphere
Color palette: strong {BIOME} identity while preserving green/yellow/red/blue/cyan/purple debug-overlay readability later
Materials/textures: detailed ground material, biome-specific rocks/plants/ice/water/mud, subtle sci-fi salvage and ruin elements
Constraints: no text, no labels, no logos, no watermarks, no UI, no characters, no blurry fog hiding terrain, no extreme close-up, no pure top-down view
```

## Prop Sheet List

Generate these after the eight biome references:

| ID | Sheet | Count | Size | Purpose |
|---|---|---:|---:|---|
| PRP-DES-01 | Desert rocks, dune blockers, dry shrubs | 8-12 | 2048x2048 | Blocked red tiles. |
| PRP-DUS-01 | Dusty scrap, mining debris, eroded boulders | 8-12 | 2048x2048 | Blockers and salvage flavor. |
| PRP-ROC-01 | Rocky cliffs, basalt chunks, crater rims | 8-12 | 2048x2048 | Rocky blockers. |
| PRP-WAT-01 | Reefs, wet rocks, island edge props | 8-12 | 2048x2048 | Water biome blockers/coast detail. |
| PRP-SWP-01 | Swamp reeds, dead trunks, toxic growths | 8-12 | 2048x2048 | Swamp blockers. |
| PRP-JNG-01 | Jungle trees, root knots, vine clumps | 8-12 | 2048x2048 | Jungle blockers. |
| PRP-SNO-01 | Snow rocks, frost shrubs, snowdrifts | 8-12 | 2048x2048 | Light snow blockers. |
| PRP-ICE-01 | Ice crystals, ridges, cracked chunks | 8-12 | 2048x2048 | Ice blockers. |
| RSC-ALL-01 | Minerals, fuel, crystal, salvage, biofuel, geothermal nodes | 8-10 | 2048x2048 | Resource tiles. |
| DNG-ALL-01 | Lost-place entrances, bunkers, alien doors, ruined structures | 6-8 | 2048x2048 | Dungeon/lost-place tiles. |
| HAZ-ALL-01 | Hazard/event props: spores, cracks, storm rods, lava vents | 8-10 | 2048x2048 | Restricted/yellow tiles. |
| LND-ALL-01 | Landing zone plates, beacon wreckage, flattened terrain rings | 6-8 | 2048x2048 | White landing area. |

### Prop Sheet Prompt Template

```text
Use case: stylized-concept
Asset type: UE5 2.5D RTS prop sheet, 2048x2048
Primary request: final detailed game prop sheet of {PROP_FAMILY} for a sci-fi survival RTS planet terrain system
Scene/backdrop: perfectly flat solid #00ff00 chroma-key background for background removal
Subject: {COUNT} separate biome props, each fully visible and isolated with generous spacing
Style/medium: detailed stylized 3D game asset concept art, UE5-ready visual target
Composition/framing: each prop shown from the same 45-degree RTS/isometric camera angle, consistent scale family, clear ground contact, no overlap
Lighting/mood: neutral studio lighting, readable forms, no cast shadows
Color palette: {BIOME_OR_ROLE} colors, strong silhouettes, no #00ff00 in the objects
Materials/textures: high-detail surfaces, chipped sci-fi survival wear where appropriate, clean shape language
Constraints: no text, no labels, no logos, no watermark, no characters, no UI, no background scenery, no floor plane, no shadows, no overlapping objects
```

## First Batch Prompts

### BIO-REF-01 Desert

```text
Use case: stylized-concept
Asset type: UE5 RTS terrain biome reference, 3840x2160
Primary request: final-quality 45-degree RTS camera view of a desert planet surface for a sci-fi survival RTS game
Scene/backdrop: a wide readable terrain area with dry buildable flats in the center, dune ridges, sandstone boulder blockers, restricted encounter gullies, cyan mineral-node candidates, and one distant half-buried bunker entrance
Subject: planet surface only, no characters, no vehicles in motion, no UI
Style/medium: detailed stylized game concept art, production-ready UE5 visual target, semi-realistic materials with clean readable silhouettes
Composition/framing: 45-degree isometric RTS perspective, camera high enough to judge tile readability, open buildable space in the center, blockers around edges and clusters
Lighting/mood: bright harsh daylight, readable shadows, survival frontier mood
Color palette: ochre sand, pale stone, rusted metal accents, small cyan resource glints, no dominant purple
Materials/textures: wind-shaped sand, cracked hardpan, eroded rock, dust-stained salvage, bunker concrete
Constraints: no text, no labels, no logos, no watermarks, no UI, no characters, no blurry fog hiding terrain, no extreme close-up, no pure top-down view
```

### PRP-DES-01 Desert Blocker Prop Sheet

```text
Use case: stylized-concept
Asset type: UE5 2.5D RTS prop sheet, 2048x2048
Primary request: final detailed game prop sheet of desert rocks, dune blockers, dry shrubs, and broken sandstone slabs for a sci-fi survival RTS planet terrain system
Scene/backdrop: perfectly flat solid #00ff00 chroma-key background for background removal
Subject: 10 separate desert blocker props, each fully visible and isolated with generous spacing
Style/medium: detailed stylized 3D game asset concept art, UE5-ready visual target
Composition/framing: each prop shown from the same 45-degree RTS/isometric camera angle, consistent scale family, clear ground contact, no overlap
Lighting/mood: neutral studio lighting, readable forms, no cast shadows
Color palette: ochre, tan, dusty red stone, dead dark shrub accents, no #00ff00 in the objects
Materials/textures: eroded sandstone, cracked dry wood, wind-polished rock, small embedded metal fragments
Constraints: no text, no labels, no logos, no watermark, no characters, no UI, no background scenery, no floor plane, no shadows, no overlapping objects
```

### BIO-REF-09A Volcanic Inactive

```text
Use case: stylized-concept
Asset type: UE5 RTS terrain biome reference, 3840x2160
Primary request: final-quality 45-degree RTS camera view of an inactive volcanic planet surface for a sci-fi survival RTS game
Scene/backdrop: wide readable terrain with central dark ash buildable flats, black basalt rock blockers, old cooled lava rivers, gray fumarole vents releasing thin steam, sulfur-stained restricted zones, cyan mineral-node candidates, and one dormant volcanic cone in the mid distance
Subject: planet surface only, no characters, no vehicles in motion, no UI
Style/medium: detailed stylized game concept art, production-ready UE5 visual target, semi-realistic materials with clean readable silhouettes
Composition/framing: 45-degree isometric RTS perspective, camera high enough to judge tile readability, open buildable ash plain in the center, blockers and vents around edges and clusters
Lighting/mood: cool overcast daylight, high contrast between pale steam and dark basalt, ominous but readable
Color palette: charcoal black, graphite gray, ash white, sulfur yellow accents, faint dull orange cracks, cyan resource glints
Materials/textures: cooled lava crust, basalt columns, ash dust, sulfur deposits, cracked volcanic glass, fumarole steam
Constraints: no text, no labels, no logos, no watermarks, no UI, no characters, no blurry fog hiding terrain, no extreme close-up, no pure top-down view
```

### BIO-REF-09B Volcanic Active

```text
Use case: stylized-concept
Asset type: UE5 RTS terrain biome reference, 3840x2160
Primary request: final-quality 45-degree RTS camera view of an active volcanic planet surface for a sci-fi survival RTS game
Scene/backdrop: wide readable terrain with central dark basalt buildable shelves, glowing orange magma rivers, red-hot lava cracks, black cliff blockers, active fumaroles releasing white steam, yellow restricted heat zones, cyan rare mineral-node candidates, and one active volcano cone with visible lava glow
Subject: planet surface only, no characters, no vehicles in motion, no UI
Style/medium: detailed stylized game concept art, production-ready UE5 visual target, semi-realistic materials with clean readable silhouettes
Composition/framing: 45-degree isometric RTS perspective, camera high enough to judge tile readability, open buildable basalt shelf in the center away from lava, blockers and volcanic hazards around edges and clusters
Lighting/mood: dramatic high-contrast lighting, bright magma against dark terrain, readable tactical terrain despite heat haze
Color palette: black basalt, dark ash gray, bright orange magma, red ember cracks, white fumarole steam, cyan resource glints
Materials/textures: molten lava, cooled volcanic crust, basalt columns, ash, glowing cracks, sulfur deposits, steam vents
Constraints: no text, no labels, no logos, no watermarks, no UI, no characters, no heavy smoke hiding terrain, no extreme close-up, no pure top-down view
```

### BIO-REF-10 Mech-Terraformed

```text
Use case: stylized-concept
Asset type: UE5 RTS terrain biome reference, 3840x2160
Primary request: final-quality 45-degree RTS camera view of a mech-optimized terraformed planet surface for a sci-fi survival RTS game
Scene/backdrop: wide readable terrain where autonomous mechs have reshaped the ground into strange artificial build platforms, reinforced hex and square foundation plates, trench-like service lanes, compacted dark soil, machine-cut ramps, metal retaining walls, blocked zones made from heavy mech debris and armored pylons, cyan resource-node candidates, and one dormant mech factory entrance built into the terrain
Subject: planet surface only, no characters, no active mechs, no vehicles in motion, no UI
Style/medium: detailed stylized game concept art, production-ready UE5 visual target, semi-realistic materials with clean readable silhouettes
Composition/framing: 45-degree isometric RTS perspective, camera high enough to judge tile readability, large central buildable engineered ground, blockers and machine structures around edges and clusters
Lighting/mood: cold industrial daylight, utilitarian, slightly uncanny terraforming pattern
Color palette: dark compacted earth, gunmetal gray, worn yellow safety paint accents, oxidized steel, cyan resource glints
Materials/textures: reinforced concrete, armored steel plates, machine-carved trenches, compacted regolith, hydraulic scars, cable conduits, dust-stained metal
Constraints: no text, no labels, no logos, no watermarks, no UI, no characters, no active robots, no blurry fog hiding terrain, no extreme close-up, no pure top-down view
```

### BIO-REF-11 Sulfuric Alien

```text
Use case: stylized-concept
Asset type: UE5 RTS terrain biome reference, 3840x2160
Primary request: final-quality 45-degree RTS camera view of a weird sulfuric alien planet surface for a sci-fi survival RTS game
Scene/backdrop: wide readable terrain with central buildable hardened sulfur flats, acid-yellow mineral crust, green-yellow acidic pools, strange alien fungal towers and crystalline growth blockers, white fumaroles, restricted corrosive gas zones, cyan rare mineral-node candidates, and one bizarre alien ruin entrance partly melted into the ground
Subject: planet surface only, no characters, no vehicles in motion, no UI
Style/medium: detailed stylized game concept art, production-ready UE5 visual target, semi-realistic materials with clean readable silhouettes, alien and uncomfortable but tactically readable
Composition/framing: 45-degree isometric RTS perspective, camera high enough to judge tile readability, open buildable sulfur crust in the center, blockers and acid hazards around edges and clusters
Lighting/mood: harsh acidic daylight, eerie alien atmosphere, readable contrast without heavy fog
Color palette: sulfur yellow, acid chartreuse, dirty lime green, pale bone white, dark volcanic brown, cyan resource glints
Materials/textures: sulfur crystals, acidic pools, mineral crust, wet corrosive mud, alien fungus, brittle glassy deposits, steam vents
Constraints: no text, no labels, no logos, no watermarks, no UI, no characters, no blurry gas hiding terrain, no extreme close-up, no pure top-down view
```

### BIO-REF-12 Magnetic Mystic

```text
Use case: stylized-concept
Asset type: UE5 RTS terrain biome reference, 3840x2160
Primary request: final-quality 45-degree RTS camera view of a purple magnetic anomaly planet surface for a sci-fi survival RTS game
Scene/backdrop: wide readable terrain with central dark violet buildable flats, floating magnetic black rock formations as blockers, fractured obsidian ground, hovering stone arches, purple-blue energy veins, low mystic fog that does not hide gameplay, restricted anti-gravity anomaly zones, cyan rare mineral-node candidates, and one ancient levitating alien monolith entrance
Subject: planet surface only, no characters, no vehicles in motion, no UI
Style/medium: detailed stylized game concept art, production-ready UE5 visual target, semi-realistic materials with clean readable silhouettes, mystical alien sci-fi
Composition/framing: 45-degree isometric RTS perspective, camera high enough to judge tile readability, open buildable ground in the center, floating rock blockers around edges and clusters, fog kept low and translucent
Lighting/mood: eerie twilight, glowing purple energy, mysterious and dangerous but tactically readable
Color palette: deep purple, violet fog, black obsidian, blue-magenta energy glow, pale cyan resource glints
Materials/textures: dark magnetic stone, obsidian shards, levitating rock, glowing mineral veins, mist, ancient alien alloy
Constraints: no text, no labels, no logos, no watermarks, no UI, no characters, no dense fog hiding terrain, no extreme close-up, no pure top-down view
```

### BIO-REF-13 Polluted Iridescent

```text
Use case: stylized-concept
Asset type: UE5 RTS terrain biome reference, 3840x2160
Primary request: final-quality 45-degree RTS camera view of a bright shiny polluted high-TIR planet surface for a sci-fi survival RTS game
Scene/backdrop: wide readable terrain with central buildable contaminated concrete and mineral flats, rainbow oil-slick ground, neon chemical runoff channels, glossy toxic pools, corroded industrial blockers, waste-crystal growths, restricted radiation and pollution zones, cyan rare resource-node candidates, and one abandoned refinery-lab entrance leaking colored vapor
Subject: planet surface only, no characters, no vehicles in motion, no UI
Style/medium: detailed stylized game concept art, production-ready UE5 visual target, semi-realistic materials with clean readable silhouettes, beautiful toxic sci-fi pollution
Composition/framing: 45-degree isometric RTS perspective, camera high enough to judge tile readability, open buildable polluted flat in the center, blockers and toxic pools around edges and clusters
Lighting/mood: bright harsh daylight with glossy reflections, eerie synthetic beauty, dangerous but readable
Color palette: iridescent rainbow sheen, toxic magenta, cyan, lime green, bright yellow, corroded gray metal, black wet sludge
Materials/textures: reflective chemical slicks, wet concrete, corroded metal, plastic debris, crystallized waste, glowing liquid, stained mineral crust
Constraints: no text, no labels, no logos, no watermarks, no UI, no characters, no dense vapor hiding terrain, no extreme close-up, no pure top-down view
```

### BIO-REF-14 Civilized Marble Garden

```text
Use case: stylized-concept
Asset type: UE5 RTS terrain biome reference, 3840x2160
Primary request: final-quality 45-degree RTS camera view of a polite friendly terraformed normal world for a sci-fi survival RTS game
Scene/backdrop: wide readable terrain with central bright green buildable grassland, clean pale stone paths, white marble rectangular blocks and quarry-cut stone as gentle blockers, small ordered terraces, calm shallow water details, cyan mineral-node candidates kept subtle, and one elegant ancient marble ruin entrance
Subject: planet surface only, no characters, no vehicles in motion, no UI
Style/medium: detailed stylized game concept art, production-ready UE5 visual target, semi-realistic materials with clean readable silhouettes, calm civilized garden-world mood
Composition/framing: 45-degree isometric RTS perspective, camera high enough to judge tile readability, open buildable grass in the center, marble and stone blockers around edges and clusters
Lighting/mood: bright clear daylight, friendly, clean, optimistic, peaceful
Color palette: fresh bright grass green, white marble, pale limestone, soft gray stone, small cyan resource glints, restrained warm sunlight
Materials/textures: healthy grass, polished marble blocks, carved stone slabs, clean gravel, shallow clear water, lightly weathered ruins
Constraints: no text, no labels, no logos, no watermarks, no UI, no characters, no pollution, no toxic neon colors, no dark horror mood, no blurry fog hiding terrain, no extreme close-up, no pure top-down view
```

## Generation Order

1. Generate `BIO-REF-01` through `BIO-REF-08`.
2. Pick the best visual language per biome.
3. Generate blocker prop sheets per biome.
4. Generate shared resource, dungeon, hazard, and landing sheets.
5. Extract/cut prop sheets into individual PNGs or use them as modeling/reference sheets.
6. Import into UE5 as concept/reference first, then convert to Static Mesh/Nanite/PCG-ready assets.

## Notes

Direct final image generation is useful now for art direction and final prop targets. The generated images should still not become the only source of gameplay truth. UE5 must keep tile roles, resources, encounters, fog reveal, and build validation in data so maps remain testable and maintainable.
