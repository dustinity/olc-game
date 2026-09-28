# S04 Construction Mode — Agent Skill

## What to Create

### Assets

| Asset Name | Texture Reference | Path in Project | Source |
|-----------|------------------|-----------------|--------|
| Valid Zone Glow | `T_Construction_ValidGlow` | `/Game/UI/Construction/T_Construction_ValidGlow` | Procedural green glow texture |
| Invalid Zone Overlay | `T_Construction_InvalidOverlay` | `/Game/UI/Construction/T_Construction_InvalidOverlay` | Red tint overlay texture |
| Grid Lines | `T_Construction_GridLines` | `/Game/UI/Construction/T_Construction_GridLines` | Semi-transparent grid pattern |
| Building Preview Outline | `T_Construction_PreviewOutline` | `/Game/UI/Construction/T_Construction_PreviewOutline` | Dashed outline texture |
| Power Icon | `T_Construction_PowerIcon` | `/Game/UI/Construction/T_Construction_PowerIcon` | SVG icon, 64x64 PNG |
| Resource Cost Icons (6) | `T_Cost_*` | `/Game/UI/Construction/T_Cost_Material`, etc. | From resource HUD assets |

**Toolset:** `asset-tools`, `import-export-toolset`

### Widget Blueprints (8 total)

| Blueprint Name | Class | Purpose |
|---------------|-------|---------|
| `WBP_ConstructionInfoPanel` | UserWidget | Floating panel showing building name, tier, cost, power draw, size |
| `WBP_BuildingPreview` | UserWidget | Visual preview of selected building at current placement position |
| `WBP_GridOverlay` | UserWidget | Semi-transparent grid lines for placement guidance |
| `WBP_ConstructionToolbar` | UserWidget | Bottom toolbar with building selection tabs, rotation button, cancel |
| `WBP_BiomeIndicator` | UserWidget | Shows biome compatibility status (green check / red X) |
| `WBP_PowerMeter` | UserWidget | Horizontal bar showing current vs max power capacity |
| `WBP_ConfirmPlacementDialog` | Modal | Confirmation dialog before placing a building |
| `WBP_ConstructionRoot` | UserWidget | Root widget — entry point for construction mode UI |

**Toolset:** `umg-toolset`, `blueprint-tools`

### Blueprint Classes (Gameplay)

| Name | Type | Purpose |
|------|------|---------|
| `BP_BuildingPlacementActor` | Actor | Spawns at grid position, shows preview mesh + glow overlay while placing |
| `BP_ConstructionModeManager` | GameMode/GameState extension | Tracks current placement state, validates grid positions, manages building list |
| `BP_GridCell` | ActorComponent | Represents a single grid cell; tracks occupancy, biome type, elevation |

**Toolset:** `blueprint-tools`, `actor-tools`

### PCG / Procedural Data

| Name | Type | Purpose |
|------|------|---------|
| `PCG_BuildingPlacement` | PCG Graph | Validates placement: checks grid bounds, biome compatibility, power availability, adjacency rules |
| `DG_BuildingData` | DataTable | Per-building data: size (1x1 to 6x6), cost per resource, power draw, TIR tier, biome list, faction variants |

**Toolset:** `pcg-toolset`, `data-tools`

## MCP Workflow: Step by Step

### Phase 1 — Inspect & Prepare

1. **Inspect the project** for existing construction-related assets
   - Toolset: `editor-toolset`
2. **Create directory structure**: `/Game/UI/Construction/` and `/Game/Gameplay/Construction/`
   - Toolset: `asset-tools` / `editor-toolset`
3. **Verify PCG toolset availability** — confirm procedural placement tools are installed
   - Toolset: `pcg-toolset`

### Phase 2 — Import Textures (6 textures)

Import all textures from the asset table above with:
- SRGB = true for UI textures
- Filter = TF_Bilinear
- GridLines: generate as a seamless tiling pattern, repeat mode = Repeat
- ValidGlow: use additive blend if supported, otherwise standard bilinear with green (#00FF88) tint

**Toolset:** `asset-tools`, `import-export-toolset`

### Phase 3 — Create Building Data Table

1. **Create DataTable `DG_BuildingData`** with columns:
   - `BuildingName` (String)
   - `BuildingID` (GameplayTag) — e.g., "building.resource.mine"
   - `SizeX`, `SizeY` (int) — grid footprint, 1-6
   - `CostMaterial`, `CostMetal`, `CostCrystal`, etc. (float) — per-resource costs
   - `PowerDraw` (int) — negative = generates power, positive = consumes
   - `TIRTier` (int) — TIR 1-5
   - `BiomeTags` (Array of GameplayTag) — compatible biomes
   - `FactionVariants` (Array of String) — faction-specific mesh overrides
   - `Description` (String)

2. **Populate with all buildings** from the building list:
   - Default tier: Mine, Oil Pump, Harvester Post, Solar Array, Wind Turbine, Barracks, Habitation Module, Radar Tower, Walls, Factory, Forge, Refinery, Workbench, Turrets, Med Bay, Command Center
   - Advanced tier (TIR 2-3): Coal Reactor, Water Turbine, Geothermal Vent, Reinforced Walls, Gates, Airfield
   - Elite tier (TIR 4-5): Void Lab, Assembly Plant, Crystal Synthesizer, Energy Shield Generator, Orbital Strike Beacon, Quantum Gate

**Toolset:** `data-tools`, `blueprint-tools`

### Phase 4 — Create Grid Cell Component

1. **Create `BP_GridCell` ActorComponent**:
   - Variables: `GridX` (int), `GridY` (int), `bOccupied` (bool), `BiomeType` (GameplayTag), `Elevation` (float)
   - Function: `CanPlaceBuilding(BuildingID)` — checks occupancy, biome compatibility via DataTable lookup
   - Function: `MarkOccupied()`, `MarkFree()` — toggle bOccupied

2. **Create a grid of BP_GridCell actors** on the planet surface level:
   - Use PCG to scatter GridCell instances in a regular pattern matching the colony map dimensions (e.g., 50x50 grid)
   - Each cell gets its biome tag from the terrain data

**Toolset:** `pcg-toolset`, `blueprint-tools`, `actor-tools`

### Phase 5 — Create Building Placement Actor (Preview System)

1. **Create `BP_BuildingPlacementActor`**:
   - Components: StaticMesh (building preview), MaterialInstanceDynamic (glow overlay), SphereComponent (collision, disabled during preview)
   - Variables: `CurrentBuildingID` (GameplayTag), `GridX`, `GridY`, `Rotation` (int 0/90/180/270)
   - Events:
     - `UpdatePreview(BuildingID)` — loads mesh from DataTable, sets glow color based on validity
     - `SetValidPlacement(bool)` — green glow (#00FF88) if valid, red tint (#FF4444) if invalid
     - `Rotate()` — rotates 90 degrees, revalidates placement
   - **Glow system**: Use a MaterialInstanceDynamic on the preview mesh. When valid, set emissive to green with pulsing intensity (sin wave, period ~1s). When invalid, static red tint at 50% opacity.

**Toolset:** `blueprint-tools`, `material-tools`

### Phase 6 — Create Construction Mode Manager

1. **Create `BP_ConstructionModeManager`**:
   - Variables: `bConstructionModeActive` (bool), `SelectedBuildingID` (GameplayTag), `PreviewActor` (reference to BP_BuildingPlacementActor), `MouseGridX`, `MouseGridY`
   - Events:
     - `EnterConstructionMode()` — spawns PreviewActor at camera raycast position, shows ConstructionRoot widget
     - `ExitConstructionMode()` — destroys PreviewActor if no building placed, hides UI, sets bConstructionModeActive = false
     - `SelectBuilding(BuildingID)` — loads building data from DataTable, updates PreviewActor mesh and info panel
     - `UpdateMousePosition(ScreenX, ScreenY)` — raycasts to grid, updates MouseGridX/Y, moves PreviewActor
     - `AttemptPlacement()` — validates via GridCell.CanPlaceBuilding(), if valid: spawns actual building actor, marks cells occupied, plays placement VFX, shows confirmation dialog briefly
     - `ConfirmPlacement(BuildingID)` — finalizes placement after user confirms in dialog

2. **PCG validation graph** (`PCG_BuildingPlacement`):
   - Input: BuildingID, GridX, GridY, Rotation
   - Checks: within map bounds, all cells free, biome compatible for ALL cells covered by footprint, power capacity sufficient, no overlapping with other buildings
   - Output: bValid (bool), InvalidCells (array of grid positions)

**Toolset:** `blueprint-tools`, `pcg-toolset`

### Phase 7 — Create UI Widgets (Bottom-Up)

#### WBP_GridOverlay
- Root: CanvasPanel, single Image child using `T_Construction_GridLines` stretched to fill
- Opacity: 0.3 by default, adjustable via variable
- No interaction events

**Toolset:** `umg-toolset`

#### WBP_BiomeIndicator
- HorizontalBox with Icon (Image) + TextBlock for biome name
- Green checkmark icon when compatible, red X when not
- Variable: `BiomeTag` (GameplayTag), function `SetBiomeStatus(bool bCompatible)`

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_PowerMeter
- HorizontalBox with PowerIcon + TextBlock (current/max) + ProgressBar for visual fill
- Variable: `CurrentPower` (int), `MaxPower` (int)
- Function `UpdateDisplay()` — sets ProgressBar value and text
- Color changes: green (>20% headroom), yellow (5-20%), red (<5%)

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ConstructionInfoPanel
- CanvasPanel child, positioned top-right of screen
- Children: BuildingName TextBlock (large), Tier badge, Size display (e.g., "3x3"), Resource cost rows (icon + amount for each resource), PowerDraw row with +/- icon, BiomeIndicator instance
- Variable: `BuildingID` — function `UpdateFromDataTable()` loads all data from DG_BuildingData

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ConfirmPlacementDialog
- Modal dialog (dark overlay + centered panel)
- Children: "Place [BuildingName]?" TextBlock, cost summary, Yes/No buttons
- Yes → calls ConstructionModeManager.ConfirmPlacement()
- No / Escape key → cancels placement, exits construction mode

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_BuildingPreview (UI overlay)
- Small preview panel in bottom-center showing a thumbnail/icon of the selected building
- Shows building name and size below the icon
- Variable: `BuildingID` — function loads icon from DataTable or uses default placeholder

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ConstructionToolbar
- HorizontalBox at bottom-center of screen
- Children: Building category tabs (Resource / Power / Infrastructure / Production / Defense / Support), Rotation button (R key icon), Cancel button (X or Escape)
- Each tab shows a grid of building icons; clicking one calls ConstructionModeManager.SelectBuilding()
- Active tab highlighted with accent color (#E8852A)

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ConstructionRoot
- Root: CanvasPanel
- Children (z-order back to front):
  1. GridOverlay instance (full screen, low opacity)
  2. ConstructionToolbar at bottom
  3. InfoPanel at top-right
  4. PowerMeter at top-left
  5. BuildingPreview at bottom-center
  6. BiomeIndicator near placement cursor
  7. ConfirmPlacementDialog (hidden by default, shown on placement attempt)

- Events:
  - `OnInitialize` — bind to ConstructionModeManager events, show widget
  - `OnDestroy` — hide widget, call ExitConstructionMode() if still in mode

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 8 — Wire Input & Camera Integration

1. **Input mapping**:
   - Left Click → AttemptPlacement (if building selected and valid)
   - Right Click / Escape → ExitConstructionMode
   - R key → Rotate preview 90 degrees
   - Tab key → Cycle building categories in toolbar
   - Mouse wheel → Zoom camera (standard UE5 camera behavior, not construction-specific)

2. **Camera integration**:
   - Construction mode uses a top-down/isometric camera angle
   - Raycast from camera through mouse position to find grid cell under cursor
   - PreviewActor follows raycast hit location on the grid plane
   - Use `LineTraceByChannel` with collision channel set to WorldStatic

3. **Building spawn on placement**:
   - When placement confirmed: read building data from DataTable, determine mesh based on faction variant, spawn BP_BuildingActor at GridX/GridY position
   - Apply rotation from PreviewActor state
   - Mark all covered grid cells as occupied
   - Deduct resources from colony resource pool
   - Add power draw to colony total

**Toolset:** `blueprint-tools`, `actor-tools`

### Phase 9 — Compile & Verify

1. **Compile every Blueprint** after creation/modification
2. **Test in PIE**:
   - Enter construction mode (test button or keybind)
   - Select different buildings from toolbar, verify preview mesh updates
   - Move cursor over valid grid cells → green glow; invalid cells → red tint
   - Try placing overlapping buildings → blocked with visual feedback
   - Rotate building preview → footprint updates correctly
   - Confirm placement → actual building spawns, grid cells marked occupied
   - Exit construction mode → preview disappears, normal gameplay resumes
3. **Verify PCG validation** — test edge cases: corner of map, partial overlap, biome mismatch, insufficient power

## Key Patterns & Gotchas

### Grid-Based Placement Validation
Every building has a footprint (SizeX x SizeY). Before placement, check ALL cells in the footprint rectangle — not just the center cell. Rotation swaps SizeX/SizeY. All cells must be free AND biome-compatible for the building to place.

### PCG vs Manual Grid Creation
For small colonies (< 100x100 grid), manually placing BP_GridCell actors is fine. For larger maps, use PCG to scatter them procedurally in a regular pattern. The PCG graph should generate cells at fixed intervals matching the desired grid spacing (e.g., every 200 Unreal units).

### Glow Material for Preview
Use a MaterialInstanceDynamic so you can change emissive color at runtime without creating separate materials. Green (#00FF88) for valid, red (#FF4444) for invalid. Add a pulsing effect using `sin(Time * 3.0)` in the material's emissive intensity input.

### DataTable-Driven Building Data
All building properties come from DG_BuildingData, not hardcoded. This means:
- New buildings can be added by adding rows to the DataTable (no Blueprint changes needed)
- Faction variants are handled via a string array — each faction has its own mesh path stored here
- TIR tier gating is checked at runtime against the colony's current TIR level

### Power System Integration
Power draw is negative for generators, positive for consumers. The colony's total power = sum of all generator outputs minus sum of all consumer draws. If total goes negative, buildings shut down in priority order (non-essential first). The PowerMeter widget reflects this in real-time.

### Rotation and Footprint Swapping
When rotating a building preview, swap SizeX and SizeY for footprint calculation. A 3x2 building rotated 90 degrees becomes a 2x3 footprint. All grid cell checks must use the rotated dimensions.

## File Structure Summary

```
/Game/UI/Construction/
├── T_Construction_ValidGlow.uasset
├── T_Construction_InvalidOverlay.uasset
├── T_Construction_GridLines.uasset
├── T_Construction_PreviewOutline.uasset
├── T_Construction_PowerIcon.uasset
├── WBP_ConstructionInfoPanel.uasset
├── WBP_BuildingPreview.uasset
├── WBP_GridOverlay.uasset
├── WBP_ConstructionToolbar.uasset
├── WBP_BiomeIndicator.uasset
├── WBP_PowerMeter.uasset
├── WBP_ConfirmPlacementDialog.uasset
└── WBP_ConstructionRoot.uasset

/Game/Gameplay/Construction/
├── BP_BuildingPlacementActor.uasset
├── BP_ConstructionModeManager.uasset
├── BP_GridCell.uasset
├── PCG_BuildingPlacement.uc (PCG graph)
└── DG_BuildingData.uasset (DataTable)

/Game/Data/
└── BuildingData.csv (source for DataTable)
```

## Next Steps After This Skill

Construction Mode feeds into:
- **Colony Management (S02)** — once buildings are placed, manage their workers and production
- **Tech Tree (S05)** — unlocks new building types as research progresses
- **Resource HUD Bar (S06)** — displays resource costs during placement
