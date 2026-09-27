# Solar Array (PB-PG-01) - UE5
**Status:** IN_PROGRESS — C++ build card + power production tick implemented. DataAsset and Blueprint to be created in editor.
**Briefing:** [PB-PG-01-Solar-Array.md](./PB-PG-01-Solar-Array.md)

## Checklist
- [x] Build card added to UOLCUIDataSubsystem (Power tab, 2x1 grid, 80 CM + 60 minerals)
- [x] Power production tick in AOLCPowerGenerator wired to AddResource(Energy)
- [ ] DataAsset `DT_Building_SolarArray` created in Content Browser
- [ ] Blueprint `BP_Building_SolarArray` from AOLCPowerGenerator with StaticMesh
- [ ] Build menu verified in Construction Mode (F2)
- [ ] Tested: placement on grid, rotation (R), abort (Esc)
- [ ] Tested: production increments Energy counter on HUD

## Briefing Stats
| Field | Value |
|-------|-------|
| Grid Size | 2x1 (wheel-shaped rotation) |
| Cost | 80 Construction Material, 60 Minerals |
| Output | 5 energy/turn base |
| Biome Preference | Desert +30%, Dusty standard, Light Snow standard |
| Biome Penalty | Jungle -50%, Ice -30% |
| TIR | 1 |

## How-To
Briefing/UE5/HOWTO/HOWTO_ADD_BUILDING.md
