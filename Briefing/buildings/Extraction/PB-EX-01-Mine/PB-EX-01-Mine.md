# Mine (PB-EX-01)

## Specs
- **Grid Size:** 2x2
- **Cost:** 40 Construction Material, 30 Minerals
- **Power:** Requires 5 Energy/tick (consumes power)
- **Output:** +8 Minerals/tick (base rate)
- **TIR Requirement:** 1
- **Biome Modifiers:** Rocky +1.25x Minerals, Desert -0.25x Minerals
- [metal-ore-mine.png](PB-EX-01-metal-ore-mine.png) — Metal Ore Mine variant with massive drill

## Visual Design
Massive rotary drill entering the ground with heavy torque-braced support frame. Sealed bore collar with twin extraction/filter stacks. Clean VFX outlet points for mineral-specific smoke/light effects: black = coal, yellow = sulfur, green = uranium. Neutral hardware keeps VFX layers swappable for different mineral types. Gray/brown color scheme with cylinder base and conveyor belt top.

## Resource Extraction
The Mine connects to the nearest terrain resource tile within a 5-tile radius when placed. Output scales based on:

| Factor | Multiplier Range | Description |
|--------|-----------------|-------------|
| Base Rate | 8 Minerals/tick | Fixed output before modifiers |
| Resource Richness | 0.35x – 1.0x | Determined by terrain tile generation |
| Rocky Biome | 1.25x | +25% mineral yield |
| Desert Biome | 0.75x | -25% mineral yield |
| Scavenging Mode | 0.25x | No resource tile found within range |

**Production formula:** `Output = BaseRate × Richness × BiomeModifier`

### Example Outputs (Rocky Biome, Full Richness)
| Condition | Multiplier | Output/tick |
|-----------|-----------|-------------|
| Rocky + Full Richness | 1.25 × 1.0 | 10 Minerals |
| Rocky + Low Richness (0.35) | 1.25 × 0.35 | ~3 Minerals |
| Desert + Full Richness | 0.75 × 1.0 | 6 Minerals |
| No Tile Found (Scavenging) | 0.25 | 2 Minerals |

## Visual Markers
- **Resource Marker:** Thin glowing line or sphere instance on the extracted resource tile
- **Color Coding:** Cyan for Minerals, Orange for Fuel
- **Pulse Animation:** Marker pulses every production tick using MaterialInstanceDynamic
- **Floating Display:** Production rate shown as floating text above the building

## Variants
Stone Mine, Crystal Mine, Metal Ore Mine (drill entering ground), Coal Mine

## Implementation Notes
- Subclass of `AOLCResourceExtractor` base class
- Uses `UOLCBuildingData` DataAsset for stats lookup (`DA_Building_Mine`)
- Blueprint name: `BP_Mine`
- Requires power connection to produce; production stops when powered off
- Tutorial objective "Discover mineral deposit" completes when first Mine is placed near a resource tile

## See Also
- [Oil Pump (PB-EX-02)](../PB-EX-02-Oil-Pump/PB-EX-02-Oil-Pump.md) — Fuel extraction building
- [Harvester Post (PB-EX-03)](../PB-EX-03-Harvester-Post/PB-EX-03-Harvester-Post.md) — Biomass harvesting
- [Biome Compatibility](../../Biome-Compatibility.md) — Full biome modifier reference
- [Grid Placement Rules](../../Grid-Placement.md) — Building placement mechanics
