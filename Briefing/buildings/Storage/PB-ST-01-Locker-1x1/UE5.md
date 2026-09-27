# Locker (1x1) (PB-ST-01) - UE5
**Status:** IN_PROGRESS — C++ build card + storage bonus implemented. DataAsset and Blueprint to be created in editor.
**Briefing:** [PB-ST-01-Locker-1x1.md](./PB-ST-01-Locker-1x1.md)

## Checklist
- [x] Build card added to UOLCUIDataSubsystem (Storage tab, 1x1 grid, 50 CM)
- [x] AOLCInfrastructure with StorageCapacityBonus field (+200 per resource type)
- [ ] DataAsset `DT_Building_Locker` created in Content Browser
- [ ] Blueprint `BP_Building_Locker` from AOLCBuildingBase (no subclass behavior needed) with StaticMesh
- [ ] Build menu verified in Construction Mode (F2)
- [ ] Tested: placement on grid, rotation (R), abort (Esc)

## Briefing Stats
| Field | Value |
|-------|-------|
| Grid Size | 1x1 |
| Cost | 50 Construction Material |
| Capacity | +200 resources per type |
| TIR | 1 |

## How-To
Briefing/UE5/HOWTO/HOWTO_ADD_BUILDING.md
