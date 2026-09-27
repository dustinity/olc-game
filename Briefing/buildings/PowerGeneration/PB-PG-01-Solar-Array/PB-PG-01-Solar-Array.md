# Solar Array (PB-PG-01)

## Specs
- **Grid Size:** 2x1 (wheel-shaped rotation)
- **Cost:** 80 construction material, 60 minerals
- **Output:** Low energy (5/turn base)
- **Biome Preference:** Desert (+30%), Dusty (standard), Light Snow (standard)
- **Biome Penalty:** Jungle (-50% canopy cover), Ice (-30%)
- [solar-array.png](PB-PG-01-solar-array.png) — Neutral solar panel array
- [solar-array-desert.png](PB-PG-01-solar-array-desert.png) — Desert dust accumulation variant
- [solar-array-snow.png](PB-PG-01-solar-array-snow.png) — Light snow cover variant

## Asset Files
- **Grid Size:** 2x1 (wheel-shaped rotation)
- **Cost:** 80 construction material, 60 minerals
- **Output:** Low energy (5/turn base)
- **Biome Preference:** Desert (+30%), Dusty (standard), Light Snow (standard)
- **Biome Penalty:** Jungle (-50% canopy cover), Ice (-30%)
- [solar-array.png](PB-PG-01-solar-array.png) — Neutral solar panel array
- [solar-array-desert.png](PB-PG-01-solar-array-desert.png) — Desert dust accumulation variant
- [solar-array-snow.png](PB-PG-01-solar-array-snow.png) — Light snow cover variant
- **Asset Format:** All RGBA transparent PNGs with matching geometry for interchangeable rotation animation. Circular azimuth bearing, shared tilt axle, visible pistons, and single blue status light preserved across all variants.

## Visual Design
Desert variant shows dust accumulation on panel surfaces. Snow variant has light snow coverage without obscuring the rotating mechanism. Panels track sunlight angle via wheel-shaped mount — animation speed varies by biome efficiency.

## Notes
No fuel cost, passive generation. Dust storms reduce efficiency by 40%. All variants preserve identical geometry so rotation animation and footprint alignment stay interchangeable across biomes.

## Source
Extracted from `buildings/planet_buildings.md`
