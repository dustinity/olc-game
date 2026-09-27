# Wall Segment (PB-IN-04) - UE5
**Status:** IN_PROGRESS — C++ build card + collision blocking implemented. DataAsset and Blueprint to be created in editor.
**Briefing:** [PB-IN-04-Wall-Segment.md](./PB-IN-04-Wall-Segment.md)

## Checklist
- [x] Build card added to UOLCUIDataSubsystem (Infrastructure tab, 1x1 grid, 30 CM + 20 minerals)
- [x] AOLCInfrastructure with bBlocksMovement = true, collision enabled on footprint
- [ ] DataAsset `DT_Building_WallSegment` created in Content Browser
- [ ] Blueprint `BP_Building_WallSegment` from AOLCInfrastructure with StaticMesh
- [ ] Build menu verified in Construction Mode (F2)
- [ ] Tested: placement on grid, rotation (R), abort (Esc)

## Briefing Stats
| Field | Value |
|-------|-------|
| Grid Size | 1x1 (single wall segment) |
| Cost | 30 Construction Material, 20 Minerals |
| Power | No power required (passive structure) |
| Effect | Blocks enemy movement |
| TIR | 1 |

## How-To
Briefing/UE5/HOWTO/HOWTO_ADD_BUILDING.md
