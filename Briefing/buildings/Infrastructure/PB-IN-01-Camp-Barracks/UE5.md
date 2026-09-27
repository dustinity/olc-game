# Camp Barracks (PB-IN-01) - UE5
**Status:** IN_PROGRESS — C++ build card + infrastructure hooks implemented. DataAsset and Blueprint to be created in editor.
**Briefing:** [PB-IN-01-Camp-Barracks.md](./PB-IN-01-Camp-Barracks.md)

## Checklist
- [x] Build card added to UOLCUIDataSubsystem (Infrastructure tab, 2x2 grid, 200 CM + 100 minerals)
- [x] Infrastructure base class with unit housing bonus field
- [ ] DataAsset `DT_Building_CampBarracks` created in Content Browser
- [ ] Blueprint `BP_Building_CampBarracks` from AOLCInfrastructure with StaticMesh
- [ ] Build menu verified in Construction Mode (F2)
- [ ] Tested: placement on grid, rotation (R), abort (Esc)

## Briefing Stats
| Field | Value |
|-------|-------|
| Grid Size | 2x2 |
| Cost | 200 Construction Material, 100 Minerals |
| Power | Requires 10 energy/turn |
| Unit Housing | Required for producing soldier units |
| TIR | 1 |

## How-To
Briefing/UE5/HOWTO/HOWTO_ADD_BUILDING.md
