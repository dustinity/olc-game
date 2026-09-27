# Architecture Decision — UE5 Implementation Strategy

**Date:** 2026-08-02  
**Status:** Accepted  
**Related:** [Readme.md](Readme.md), [HOWTO guides](HOWTO/), [Style Guide](Style/UI_THEME.md)

---

## Decision: Hybrid Slate + UMG Approach

### What We Do

| Layer | Technology | Rationale |
|-------|-----------|-----------|
| **Main HUD overlay** (resource strip, mission progress, minimap, simulation speed) | **Slate C++** (`RebuildWidget()`) | Frame-rate critical. Always visible. Must render every tick without UMG overhead. |
| **Construction Mode** (build cards, placement preview, category tabs) | **UMG Blueprints** | Layout-heavy. Needs visual designer for card grids, thumbnails, detail panels. |
| **Tech Tree** (node graph, research queue, unlock states) | **UMG Blueprints** | Complex layout with zoomable canvas, curved connections, hover interactions. |
| **Settings / Modals / Menus** (settings, confirmations, popups) | **UMG Blueprints** | Standard modal patterns well-served by UMG's built-in widgets. |
| **Dropship Repair / Mothership Builder** (module grid, hull schematic) | **UMG Blueprints** | Drag-and-drop, complex container layouts. |
| **Dungeon UI** (combat view, squad selection, results) | **UMG Blueprints** | Real-time combat overlay with health bars, hotbars, roster expansion. |
| **All game logic** (subsystems, actors, data flow, AI, production) | **C++ classes** | No debate here — all gameplay systems in C++. |

### Why Not Pure Slate?

- Construction Mode has 9 category tabs with build cards showing thumbnails, costs, grid sizes, TIR requirements. Doing this layout in pure Slate code is ~500+ lines of `SNew()` per screen change.
- Tech Tree needs a zoomable node graph with curved bezier connections. Writing that from scratch in Slate would take weeks.
- Settings menus have sliders, checkboxes, dropdowns — UMG provides these natively.

### Why Not Pure UMG?

- Main HUD resource strip must update every game tick (resource production rates change constantly). UMG has known overhead for per-frame updates on many widgets.
- Minimap with dozens of markers needs efficient rendering. Slate's `SCanvasPanel` + direct drawing is far more performant.
- Our existing codebase already has the HUD foundation in Slate — switching would mean rewriting all shared widget infrastructure.

### Why Not MCP for Blueprint Creation?

**Tested 2026-08-02:** The UE5 MCP server responds in ~0.3 seconds per call. However, creating one UMG Blueprint requires:
1. `CreateWidgetBlueprint` (0.3s)
2. `AddUIComponent` × N components (0.3s each)
3. `list_properties()` before every `set_properties()` call (0.3s each)
4. Blueprint compilation after each change (~5-30s in editor)

A single widget with 10 components = ~30 MCP calls + multiple compilations = **2+ hours**. That's the bottleneck, not server latency.

**Solution:** Write C++ source files directly (via Claude Code `Write` tool), compile once, and use Blueprints only for layout work done manually in the UE5 editor.

---

## Data Flow Pattern

```
C++ Data Structs (Core/OLC*.h)
    → UOLCUIDataSubsystem (GameInstanceSubsystem)
        → Slate widgets: RebuildWidget() reads from subsystem
        → UMG Blueprints: bind to properties, read from subsystem via BlueprintCallable functions
            → Rendered in viewport
```

### DataStructs (C++)

All game data lives in C++ structs and enums. No `.uasset` DataAssets needed for early development — use inline static factories or subsystem initialization.

```cpp
// Core/OLCResourceTypes.h — EXISTS
UENUM(BlueprintType) enum class EOLCResourceType : uint8 { ... };  // 7 types

USTRUCT(BlueprintType) struct FOLCResourceCounterViewData { ... };
USTRUCT(BlueprintType) struct FOLCBadgeViewData { ... };
USTRUCT(BlueprintType) struct FOLCBuildCardViewData { ... };
// etc.
```

### Subsystem (C++)

```cpp
// Core/OLCUIDataSubsystem.h/cpp — EXISTS
class UOLCUIDataSubsystem : public UGameInstanceSubsystem {
    // GetResourceCounters() → TArray<FOLCResourceCounterViewData>
    // GetBuildCards() → TArray<FOLCBuildCardViewData>
    // GetMissionObjectives() → TArray<FOLCMissionObjectiveViewData>
    // etc.
};
```

### UI Binding

- **Slate widgets** read directly from the subsystem in `RebuildWidget()` or `ApplyViewData_Implementation()`
- **UMG Blueprints** use `BlueprintPure` functions on the subsystem, called from widget events (OnInitialize, OnTick)

---

## File Layout

```
Source/OurLastChance/
├── Core/          - Enums, structs, subsystems, base widget
│   ├── OLCResourceTypes.h/cpp     - All enums + data structs
│   ├── OLCWidgetBase.h/cpp        - Base widget class
│   └── OLCUIDataSubsystem.h/cpp   - Fake/test data provider
├── UI/            - Slate widgets (HUD screens, shared components)
│   ├── OLCHUDWidgets.h/cpp        - Main HUD + Construction overlay
│   └── OLCSharedWidgets.h/cpp     - Shared Slate components
├── Player/        - Player controllers, input routing
│   ├── OLCGameplayPlayerController.h/cpp  - RTS controls, construction mode
│   └── OLCMenuPlayerController.h/cpp      - Menu navigation
└── GameModes/     - Game modes
    ├── OLCMenuGameMode.h/cpp
    └── OLCUITestGameMode.h/cpp

Briefing/UE5/
├── Readme.md                    - Index of all UE5 docs
├── Architecture-Decision.md     - This file
├── HOWTO/                       - Implementation guides (rewritten for hybrid approach)
│   ├── HOWTO_ADD_UI_SCREEN.md   - Slate widget OR UMG Blueprint
│   ├── HOWTO_ADD_BUILDING.md    - C++ actor + DataAsset/DataStruct
│   ├── HOWTO_ADD_UNIT.md        - C++ pawn + DataAsset/DataStruct
│   ├── HOWTO_ADD_BIOME.md       - Enum + DataAsset/DataStruct + terrain
│   ├── HOWTO_ADD_RESOURCE.md    - Enum entry + subsystem fake data
│   └── HOWTO_ADD_DATAASSET.md   - When to use .uasset vs inline struct
├── Style/                       - Visual design reference
│   ├── UI_THEME.md              - Stil-1 color system, typography, panel styles
│   └── STYLE_GUIDE.md           - 3D asset style (buildings, units)
├── UI-Screens/                  - Screen-by-screen implementation specs
│   ├── S01-PlanetOverview.md    - Colony management screen
│   ├── S02-ConstructionMode.md  - Build mode overlay
│   └── ... (one per screen)
├── MCP/                         - MCP tool reference (for manual editor work)
│   └── Index.md                 - Available tools, when to use them
└── Assets/                      - Asset organization reference
    └── README.md                - Content browser structure, naming

UE5/ProjectFiles/OurLastChance/Knowledge/   ← Legacy location (keep for reference)
```

---

## Migration Notes

The `UE5/ProjectFiles/OurLastChance/Knowledge/` folder contains the original HOWTO guides written for a pure DataAsset + Blueprint workflow. They have been rewritten in `Briefing/UE5/HOWTO/` to reflect:

1. **Slate for HUD overlays** — not UMG
2. **C++ structs over .uasset DataAssets** for early development
3. **UMG Blueprints only where layout complexity justifies it** (construction, tech tree, settings)
4. **Direct C++ file editing** instead of MCP Blueprint creation

The legacy files are kept for reference but should not be used as the source of truth.

---

## Quick Reference: When to Use What

| Task | Technology | File Extension | Speed |
|------|-----------|----------------|-------|
| HUD overlay widgets | Slate C++ | `.h` / `.cpp` | Fast (Write tool → compile) |
| Construction mode UI | UMG Blueprint | `.uasset` | Medium (manual in editor) |
| Settings/modals | UMG Blueprint | `.uasset` | Medium (manual in editor) |
| Game logic / subsystems | C++ classes | `.h` / `.cpp` | Fast (Write tool → compile) |
| Building/Unit data | C++ struct + inline factory | `.h` | Fast (no .uasset needed yet) |
| Meshes / Textures | Import via editor or Write to disk | `.uasset` / `.png` | N/A |
| Niagara VFX | Manual in editor | `.uasset` | N/A |
