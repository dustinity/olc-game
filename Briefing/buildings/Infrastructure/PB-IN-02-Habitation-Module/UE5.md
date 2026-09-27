# Habitation Module (PB-IN-02) - UE5
**Status:** IN_PROGRESS — C++ build card + infrastructure hooks implemented. DataAsset and Blueprint to be created in editor.
**Briefing:** [PB-IN-02-Habitation-Module.md](./PB-IN-02-Habitation-Module.md)

## Checklist
- [x] Build card added to UOLCUIDataSubsystem (Infrastructure tab, 2x2 grid, 300 CM + 150 minerals + 50 survival)
- [x] Infrastructure base class with unit housing bonus field (+8)
- [ ] DataAsset `DT_Building_HabitationModule` created in Content Browser
- [ ] Blueprint `BP_Building_HabitationModule` from AOLCInfrastructure with StaticMesh
- [ ] Build menu verified in Construction Mode (F2)
- [ ] Tested: placement on grid, rotation (R), abort (Esc)

## Briefing Stats
| Field | Value |
|-------|-------|
| Grid Size | 2x2 |
| Cost | 300 Construction Material, 150 Minerals, 50 Survival |
| Power | Requires 15 energy/turn |
| Unit Capacity | +8 base unit cap |
| TIR | 1 |

## How-To
Briefing/UE5/HOWTO/HOWTO_ADD_BUILDING.md
