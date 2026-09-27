# Gate (PB-IN-05) - UE5
**Status:** IN_PROGRESS — C++ build card + toggleable collision implemented. DataAsset and Blueprint to be created in editor.
**Briefing:** [PB-IN-05-Gate.md](./PB-IN-05-Gate.md)

## Checklist
- [x] Build card added to UOLCUIDataSubsystem (Infrastructure tab, 1x1 grid, 80 CM + 50 minerals)
- [x] AOLCInfrastructure with ToggleGate() method — bBlocksMovement=true, bGateOpen toggle
- [ ] DataAsset `DT_Building_Gate` created in Content Browser
- [ ] Blueprint `BP_Building_Gate` from AOLCInfrastructure with StaticMesh
- [ ] Build menu verified in Construction Mode (F2)
- [ ] Tested: placement on grid, ToggleGate() opens/closes collision

## Briefing Stats
| Field | Value |
|-------|-------|
| Grid Size | 1x1 (gate opening) |
| Cost | 80 Construction Material, 50 Minerals |
| Power | Requires 5 energy/turn (powered to function) |
| Effect | Controlled entry point for wall perimeters |
| TIR | 1 |

## How-To
Briefing/UE5/HOWTO/HOWTO_ADD_BUILDING.md
