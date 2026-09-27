# Atlas Slicing Guide

This guide explains how to split generated UI atlases into individual PNG files for Unreal UMG.

## Rule

Keep the original atlas PNGs. Export sliced files into a sibling `Sliced` folder.

Example:

```text
Shared Assets/
  SHR-ICN-Atlas-ResourcesStatus-4x4.png
  Sliced/
    SHR-RSC-01_Energy.png
    ...
```

## Script

Run from the repository root:

```powershell
powershell -ExecutionPolicy Bypass -File UE5/Assets/UI/slice-atlases.ps1
```

The script uses equal-cell slicing based on the known grid size for each atlas. This is good enough for UMG placeholder implementation. If a generated sprite has too much padding, refine that single sprite later, but do not block implementation.

## Atlas List

| Atlas | Grid | Manifest |
|---|---:|---|
| `Shared Assets/SHR-ICN-Atlas-ResourcesStatus-4x4.png` | 4x4 | `Shared Assets/SHARED_ICON_MANIFEST.md` |
| `Shared Assets/SHR-UI-Atlas-Components-4x4.png` | 4x4 | `Shared Assets/SHARED_COMPONENT_MANIFEST.md` |
| `Shared Assets/SHR-ICN-Atlas-NotificationsSettings-4x4.png` | 4x4 | `Shared Assets/NOTIFICATION_SETTINGS_MANIFEST.md` |
| `Shared Assets/SHR-MAP-Atlas-Overlays-4x4.png` | 4x4 | `Shared Assets/MAP_OVERLAY_MANIFEST.md` |
| `Shared Assets/SHR-NAV-Atlas-ScreenIcons-4x3.png` | 4x3 | `Shared Assets/SCREEN_NAVIGATION_MANIFEST.md` |
| `Shared Assets/SHR-BDG-Atlas-BiomesHazards-4x4.png` | 4x4 | `Shared Assets/BIOME_HAZARD_MANIFEST.md` |
| `Shared Assets/SHR-UI-Atlas-FormControls-4x4.png` | 4x4 | `Shared Assets/FORM_CONTROL_MANIFEST.md` |
| `Main HUD Elements/Assets/BLD-UI-Atlas-Construction-4x4.png` | 4x4 | `Main HUD Elements/Assets/CONSTRUCTION_ASSET_MANIFEST.md` |
| `Galaxy System UI/Assets/GAL-PLN-Atlas-PlanetTypes-4x2.png` | 4x2 | `Galaxy System UI/Assets/PLANET_ASSET_MANIFEST.md` |
| `Galaxy System UI/Assets/GAL-MRK-Atlas-Navigation-4x4.png` | 4x4 | `Galaxy System UI/Assets/NAVIGATION_MARKER_MANIFEST.md` |
| `Solar System UI/Assets/SOL-PLN-Atlas-PlanetTypes-4x2.png` | 4x2 | `Solar System UI/Assets/PLANET_ASSET_MANIFEST.md` |
| `Solar System UI/Assets/SOL-MRK-Atlas-Navigation-4x4.png` | 4x4 | `Solar System UI/Assets/NAVIGATION_MARKER_MANIFEST.md` |
| `Colony Resource Network/Assets/COL-ICN-Atlas-RolesRoutes-4x3.png` | 4x3 | `Colony Resource Network/Assets/COLONY_ICON_MANIFEST.md` |
| `Research Tech Tree/Assets/RES-ICN-Atlas-TechCategories-4x3.png` | 4x3 | `Research Tech Tree/Assets/RESEARCH_ICON_MANIFEST.md` |
| `Tactical Combat Dungeon View/Assets/DNG-ICN-Atlas-Tactical-4x3.png` | 4x3 | `Tactical Combat Dungeon View/Assets/DUNGEON_ICON_MANIFEST.md` |
| `Dropship Repair View/Assets/SHP-ICN-Atlas-RepairStatus-4x4.png` | 4x4 | `Dropship Repair View/Assets/SHIP_REPAIR_STATUS_MANIFEST.md` |
| `Mothership Builder/Assets/MSB-MOD-Atlas-TopViewModules-4x3.png` | 4x3 | `Mothership Builder/Assets/MODULE_ASSET_MANIFEST.md` |
| `Mothership Builder/Assets/MSB-HUL-Atlas-HullShapes-3x2.png` | 3x2 | `Mothership Builder/Assets/HULL_SHAPE_MANIFEST.md` |
| `Mothership Builder/Assets/MSB-ICN-Atlas-ModuleStatus-4x4.png` | 4x4 | `Mothership Builder/Assets/MODULE_STATUS_MANIFEST.md` |
| `Inventory Equipment View/Assets/EQP-ICN-Atlas-UnitsUpgrades-4x3.png` | 4x3 | `Inventory Equipment View/Assets/EQUIPMENT_ICON_MANIFEST.md` |

## Unreal Import Settings

Recommended first-pass import settings:

- Texture Group: UI
- sRGB: enabled
- Compression: UserInterface2D or UI equivalent
- Mip Gen Settings: NoMipmaps for fixed-size UI icons
- Preserve alpha if the file has it

## Naming

Use manifest `Asset ID` plus descriptive name:

```text
SHR-RSC-01_Energy.png
BLD-CAT-01_PowerCategory.png
MSB-MOD-03_EnergyCore.png
```

The exact rendered text must come from UMG, not from bitmap assets.

