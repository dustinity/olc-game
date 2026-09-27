# UE5 Implementation — Index

**Project:** Our Last Chance  
**Engine:** Unreal Engine 5 (C++ project)  
**UI Strategy:** Hybrid Slate C++ + UMG Blueprints  
**Last Updated:** 2026-08-02  

---

## Quick Start

1. Read [Architecture-Decision.md](Architecture-Decision.md) — the hybrid approach rationale
2. Follow [HOWTO guides](HOWTO/) for entity-specific implementation patterns
3. Use [UI screen specs](UI-Screens/) for screen-by-screen design
4. Reference [Style docs](Style/) for visual consistency

---

## Architecture Decision

| Layer | Technology | See Also |
|-------|-----------|----------|
| Main HUD overlay (F1) | **Slate C++** | [HOWTO_ADD_UI_SCREEN.md](HOWTO/HOWTO_ADD_UI_SCREEN.md#part-a-slate-widget-c) |
| Construction Mode (F2) | **UMG Blueprint** | [HOWTO_ADD_UI_SCREEN.md](HOWTO/HOWTO_ADD_UI_SCREEN.md#part-b-umg-blueprint-manual-in-editor) |
| Tech Tree, Settings, Modals | **UMG Blueprint** | [S05-TechTree.md](UI-Screens/S05-TechTree.md) |
| All game logic | **C++ classes** | [SOURCE_LAYOUT.md](Architecture/SOURCE_LAYOUT.md) |

Full rationale: [Architecture-Decision.md](Architecture-Decision.md)

---

## HOWTO Guides

All guides rewritten for the hybrid Slate/UMG approach. Stats come from Briefing design docs — never invented values.

| Guide | Purpose | Key Change from Legacy |
|-------|---------|----------------------|
| [HOWTO_ADD_UI_SCREEN.md](HOWTO/HOWTO_ADD_UI_SCREEN.md) | Add HUD overlay or modal screen | Decision tree: Slate vs UMG |
| [HOWTO_ADD_BUILDING.md](HOWTO/HOWTO_ADD_BUILDING.md) | Building data + actor pipeline | C++ struct factory instead of `.uasset` for early dev |
| [HOWTO_ADD_UNIT.md](HOWTO/HOWTO_ADD_UNIT.md) | Unit data + pawn pipeline | C++ struct + APawn subclass |
| [HOWTO_ADD_BIOME.md](HOWTO/HOWTO_ADD_BIOME.md) | Biome enum + terrain modifiers | Inline factory function |
| [HOWTO_ADD_RESOURCE.md](HOWTO/HOWTO_ADD_RESOURCE.md) | Add resource to economy | Same pattern, no changes needed |
| [HOWTO_ADD_DATAASSET.md](HOWTO/HOWTO_ADD_DATAASSET.md) | When to use .uasset vs inline struct | Two-phase approach (structs first, DataAssets later) |

---

## UI Screen Specifications

Screen-by-screen implementation specs adapted from the original RTS UI skills. These define assets, widget hierarchy, and data requirements.

### Menu & Setup
| Screen | File | Hotkey |
|--------|------|--------|
| M01 Welcome Screen + Settings | [M01-WelcomeScreen.md](UI-Screens/M01-WelcomeScreen.md) | Startup |

### Strategic Loop (Colony Management)
| Screen | File | Hotkey |
|--------|------|--------|
| S01 Planet Overview / Colony Mgmt | [S01-PlanetOverview.md](UI-Screens/S01-PlanetOverview.md) | — |
| S02 Colony Resource Network | [S02-ColonyManagement.md](UI-Screens/S02-ColonyManagement.md) | F3 |
| S04 Construction Mode | [S04-ConstructionMode.md](UI-Screens/S04-ConstructionMode.md) | F2 |
| S05 Tech Tree | [S05-TechTree.md](UI-Screens/S05-TechTree.md) | F7 |

### Tactical Loop (Dungeons & Combat)
| Screen | File | Hotkey |
|--------|------|--------|
| S06 Resource HUD Bar | [S06-ResourceHUDBar.md](UI-Screens/S06-ResourceHUDBar.md) | Cross-cutting |
| S07 Dungeon Generation | [S07-DungeonGeneration.md](UI-Screens/S07-DungeonGeneration.md) | — |
| S08 Tactical Combat | [S08-TacticalCombat.md](UI-Screens/S08-TacticalCombat.md) | F6 |
| S09 Squad Selection | [S09-SquadSelection.md](UI-Screens/S09-SquadSelection.md) | — |
| S10 Equipment | [S10-Equipment.md](UI-Screens/S10-Equipment.md) | F10 |

### Travel Loop (Space)
| Screen | File | Hotkey |
|--------|------|--------|
| S11 Solar System | [S11-SolarSystem.md](UI-Screens/S11-SolarSystem.md) | F4 |
| S12 Galaxy Map | [S12-GalaxyMap.md](UI-Screens/S12-GalaxyMap.md) | F5 |
| S13 Dropship Repair | [S13-DropshipRepair.md](UI-Screens/S13-DropshipRepair.md) | F8 |

### Foundation
| Screen | File | Purpose |
|--------|------|---------|
| S00 UI Component Library | [S00-UIComponentLibrary.md](UI-Screens/S00-UIComponentLibrary.md) | Shared components (buttons, cards, progress bars) |

---

## Style & Visual Design

| Document | Purpose |
|----------|---------|
| [UI_THEME.md](Style/UI_THEME.md) | Stil-1 color system, typography, panel styles, button states |
| [STYLE_GUIDE.md](Style/STYLE_GUIDE.md) | 3D asset style (buildings, units) — camera, materials, lighting |
| [SHARED_UI_STRATEGY.md](Style/SHARED_UI_STRATEGY.md) | Visual contract for all screens — colors, rules, mockup policy |
| [ATLAS_SLICING_GUIDE.md](Style/ATLAS_SLICING_GUIDE.md) | How to split generated UI atlases into individual PNGs |

---

## Asset Organization

| Document | Purpose |
|----------|---------|
| [SHARED_ICON_MANIFEST.md](Assets/SHARED_ICON_MANIFEST.md) | Resource status icons (4×4 atlas) |
| [SHARED_COMPONENT_MANIFEST.md](Assets/SHARED_COMPONENT_MANIFEST.md) | UI component sprites (4×4 atlas) |
| [NOTIFICATION_SETTINGS_MANIFEST.md](Assets/NOTIFICATION_SETTINGS_MANIFEST.md) | Toast/notification icons |
| [MAP_OVERLAY_MANIFEST.md](Assets/MAP_OVERLAY_MANIFEST.md) | Map overlay elements |
| [SCREEN_NAVIGATION_MANIFEST.md](Assets/SCREEN_NAVIGATION_MANIFEST.md) | Screen navigation icons |
| [BIOME_HAZARD_MANIFEST.md](Assets/BIOME_HAZARD_MANIFEST.md) | Biome/hazard badges |
| [FORM_CONTROL_MANIFEST.md](Assets/FORM_CONTROL_MANIFEST.md) | Sliders, checkboxes, dropdowns |

---

## MCP Tool Reference

For manual editor work only (not for automated Blueprint creation — see Architecture Decision).

| Document | Purpose |
|----------|---------|
| [MCP/Index.md](MCP/Index.md) | All 51 toolsets, ~825 tools, organized by category |

**Key UI-related toolsets:**
- `UMGToolSet.UMGToolSet` — Create/compile UMG widget Blueprints (23 tools)
- `SlateInspectorToolset.SlateInspectorToolset` — Inspect and interact with Slate UI elements (14 tools)
- `BlueprintTools` — Create/compile Blueprints, manage graphs/functions/events (53 tools)
- `AssetTools` — Find, load, save, duplicate, move, delete assets (21 tools)

---

## Legacy Reference

These files are kept from the original location but should **not** be used as source of truth:

| Path | Status |
|------|--------|
| `UE5/ProjectFiles/OurLastChance/Knowledge/HOWTO_ADD_*.md` | ⚠️ Outdated — pure Blueprint/DataAsset approach |
| `UE5/ProjectFiles/OurLastChance/Knowledge/ARCHITECTURE_OVERVIEW.md` | ℹ️ Reference only — architecture has evolved |
| `UE5/ProjectFiles/OurLastChance/Knowledge/SOURCE_LAYOUT_RULES.md` | ℹ️ Reference only — naming conventions still apply |
| `UE5/ProjectFiles/OurLastChance/Knowledge/INTEGRATION_STATUS.md` | ℹ️ Reference only — updated per WP completion |

---

## Implementation Workflow

```
1. Read Briefing design doc for entity (`Briefing/<Category>/<ID>/<ID>.md`)
2. Read HOWTO guide for entity type (Briefing/UE5/HOWTO/HOWTO_ADD_*.md)
3. Implement C++ structs + actors OR UMG Blueprints per screen spec
4. Update entity's UE5.md tracker with integration status
5. Update INTEGRATION_STATUS.md table row
6. Test in Play mode → Mark WP Done
```

---

## Workpackage Mapping

| WP | Title | Primary HOWTO |
|----|-------|---------------|
| WP-01 | Finish Main HUD (F1) + Construction Mode (F2) | [HOWTO_ADD_UI_SCREEN.md](HOWTO/HOWTO_ADD_UI_SCREEN.md) |
| WP-02 | Building base classes & DataAsset system | [HOWTO_ADD_BUILDING.md](HOWTO/HOWTO_ADD_BUILDING.md) |
| WP-03 | First building end-to-end (Mine PB-EX-01) | [HOWTO_ADD_BUILDING.md](HOWTO/HOWTO_ADD_BUILDING.md) |
| WP-04 | Tier 1 buildings (6 starting base structures) | [HOWTO_ADD_BUILDING.md](HOWTO/HOWTO_ADD_BUILDING.md) |
| WP-05 | Unit base classes & DataAsset system | [HOWTO_ADD_UNIT.md](HOWTO/HOWTO_ADD_UNIT.md) |
| WP-06 | First unit end-to-end (Basic Soldier) | [HOWTO_ADD_UNIT.md](HOWTO/HOWTO_ADD_UNIT.md) |
| WP-07 | Biome foundation + Desert planet | [HOWTO_ADD_BIOME.md](HOWTO/HOWTO_ADD_BIOME.md) |
| WP-08 | Crash landing sequence & tutorial flow | — (gameplay logic) |
| WP-09 | Tech tree system + S05 UI screen | [S05-TechTree.md](UI-Screens/S05-TechTree.md) |
| WP-10 | Space travel — Solar System + Galaxy Map | [S11-SolarSystem.md](UI-Screens/S11-SolarSystem.md) |
| WP-11 | Dungeon system — tactical combat & squad selection | [S08-TacticalCombat.md](UI-Screens/S08-TacticalCombat.md) |
| WP-12 | Dropship repair view & ship module management | [S13-DropshipRepair.md](UI-Screens/S13-DropshipRepair.md) |
