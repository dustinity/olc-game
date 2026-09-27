# Mine (PB-EX-01) - UE5
**Status:** COMPLETE — DataAsset and Blueprint definitions added to Python creation scripts. C++ infrastructure fully implemented.
**Briefing:** [PB-EX-01-Mine.md](./PB-EX-01-Mine.md)

## Checklist
- [x] Build card added to UOLCUIDataSubsystem (Extraction tab, 2x2 grid, 40 CM + 30 Minerals cost)
- [x] Production tick in AOLCResourceExtractor wired to AddResource() → HUD
- [x] DataAsset definition `DA_Building_Mine` added to create_building_dataassets.py (2x2, Rocky +1.25x, Desert -0.25x)
- [x] Blueprint definition `BP_Mine` added to create_building_blueprints.py (OLCResourceExtractor parent, cylinder mesh)
- [x] Resource tile finding: nearest tile within 5-tile radius stored on placement
- [x] Production scaling: BaseOutput × Richness × BiomeModifier
- [x] Scavenging mode: 0.25x when no resource tile found
- [x] Visual marker: InstancedStaticMeshComponent with MaterialInstanceDynamic pulse animation
- [x] Color-coded markers: Cyan for Minerals, Orange for Fuel
- [x] Tutorial objective "Discover mineral deposit" added to PopulateFakeMissionObjectives()
- [x] Briefing documentation updated with WP-105 specs

## How-To
Briefing/UE5/HOWTO/HOWTO_ADD_BUILDING.md

## Briefing Stats
| Field | Value |
|-------|-------|
| Grid Size | 2x2 |
| Cost | 40 Construction Material, 30 Minerals |
| Power | Requires 5 energy/tick (PowerConsumption = -5.0) |
| Output | +8 Minerals/tick (base) |
| Biome Modifiers | Rocky +1.25x, Desert -0.25x |
| TIR | 1 |
| Scavenging Mode | 0.25x when no tile found |

## See Also
- [WP-105 Implementation](../[olc-game] Workpackages/WP-105.md) — Full work package details
