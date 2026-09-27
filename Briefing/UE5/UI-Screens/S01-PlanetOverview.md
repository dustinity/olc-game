# S01 Planet Overview — Agent Skill

## What to Create

### Assets

| Asset Name | Texture Reference | Path in Project | Source |
|-----------|------------------|-----------------|--------|
| Planet Surface Background | `T_Planet_Surface` | `/Game/UI/Planet/T_Planet_Surface` | Large planet surface texture for overview map |
| Building Marker Icon (per category) | `T_Marker_*` | `/Game/UI/Planet/T_Marker_Mine`, etc. | Small icons for each building type on the planet view |
| Threat Indicator | `T_Planet_Threat` | `/Game/UI/Planet/T_Planet_Threat` | Skull/warning icon, 48x48 PNG |
| Attraction Indicator | `T_Planet_Attraction` | `/Game/UI/Planet/T_Planet_Attraction` | Magnet/target icon, 48x48 PNG |
| Biome Overlay Textures (3-5) | `T_Biome_*` | `/Game/UI/Planet/T_Biome_Desert`, etc. | Semi-transparent color overlays per biome type |

**Toolset:** `asset-tools`, `import-export-toolset`

### Widget Blueprints (6 total)

| Blueprint Name | Class | Purpose |
|---------------|-------|---------|
| `WBP_PlanetOverviewRoot` | UserWidget | Root widget — planet overview screen |
| `WBP_BuildingMarker` | UserWidget | Small marker on the planet view showing a building's type and status |
| `WBP_PlanetStatsPanel` | UserWidget | Side panel with colony statistics: population, power, resources, threat level |
| `WBP_BiomeLegend` | UserWidget | Legend showing biome colors and their meanings |
| `WBP_BuildingInfoTooltip` | Modal — tooltip on hover over a building marker |
| `WBP_NavigationDots` | UserWidget — dots at bottom for navigating between planet views (if multiple planets) |

**Toolset:** `umg-toolset`, `blueprint-tools`

### Blueprint Classes (Gameplay)

| Name | Type | Purpose |
|------|------|---------|
| `BP_PlanetViewManager` | Actor | Manages the 2D/3D planet overview view: camera position, building markers, click detection |

**Toolset:** `blueprint-tools`, `actor-tools`

## MCP Workflow: Step by Step

### Phase 1 — Inspect & Prepare

1. **Inspect the project** for existing planet/view assets
   - Toolset: `editor-toolset`
2. **Create directory structure**: `/Game/UI/Planet/`
   - Toolset: `asset-tools` / `editor-toolset`

### Phase 2 — Import Textures (5+ textures)

Import all textures with SRGB = true, TF_Bilinear. Planet surface should be a large texture suitable for a map view. Biome overlays should use subtle color tints that don't obscure the underlying terrain.

**Toolset:** `asset-tools`, `import-export-toolset`

### Phase 3 — Create Building Marker System

1. **Create `WBP_BuildingMarker`**:
   - CanvasPanel child with: MarkerIcon (Image, building category icon), StatusDot (small circle in corner — green = operating normally, yellow = reduced output, red = no power/worker issue)
   - Variables: `BuildingID` (GameplayTag), `GridX`, `GridY`, `Status` (Enum: Normal, Warning, Critical)
   - Function `UpdateFromBuilding(buildingActor)` — reads building state from BP_BuildingActor
   - Event: `OnHovered()` → shows BuildingInfoTooltip with full stats
   - Event: `OnClicked()` → opens BuildingDetailPopup (from Colony Management S02) or centers camera on that building

2. **Spawn markers procedurally**: For each BP_BuildingActor in the colony, spawn a WBP_BuildingMarker WidgetComponent at the building's grid position on the planet view. Position is calculated from GridX/GridY × cell spacing.

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 4 — Create Planet Stats Panel

#### WBP_PlanetStatsPanel
- VerticalBox on the right side of screen (~25% width)
- Children sections (each a group of rows):
  - **Population**: Total / Housing capacity, with growth rate indicator
  - **Power**: Generation / Consumption with visual meter (green bar filling left to right)
  - **Resources**: Compact row of 6 resource icons with current amounts (mini version of Resource HUD)
  - **Threat Level**: Color-coded bar (green = low, yellow = medium, red = high) with numeric value
  - **Attraction Level**: Similar color-coded bar showing alien attraction/interest level
- Variable: reference to ColonyManager and PlanetViewManager, function `UpdateFromColony()` reads all current stats

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 5 — Create Remaining UI Widgets

#### WBP_BiomeLegend
- Small panel (bottom-left corner) showing biome color key
- Each entry: colored square + biome name (e.g., "Desert", "Tundra", "Jungle", "Volcanic", "Crystal Caves")
- Semi-transparent background, collapsible with a toggle button

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_BuildingInfoTooltip
- Lightweight tooltip that appears on hover over BuildingMarker
- Shows: BuildingName, Tier, Worker count, Production rate, Power draw
- Appears near the cursor position, follows mouse while hovering
- Disappears when mouse leaves the marker

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_NavigationDots
- Horizontal row of dots at bottom-center of screen
- One dot per planet (if multi-planet system)
- Active planet highlighted with accent color (#E8852A)
- Clicking a dot → switches to that planet's overview view

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_PlanetOverviewRoot
- Root: CanvasPanel
- Children (z-order back to front):
  1. PlanetSurface background (Image filling entire canvas)
  2. Biome overlays (semi-transparent colored images over biome regions)
  3. BuildingMarker instances (WidgetComponents positioned at building locations)
  4. PlanetStatsPanel on right side
  5. BiomeLegend at bottom-left
  6. NavigationDots at bottom-center
  7. BuildingInfoTooltip (follows cursor, shown on hover)

- Events:
  - `OnInitialize()` — scan for all BP_BuildingActor instances in the level, spawn markers for each
  - `OnCameraMoved(newX, newY)` — update marker visibility based on camera viewport (hide markers outside view frustum for performance)
  - `OnTick()` — update stats panel and marker status dots

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 6 — Wire Camera & Navigation

1. **Camera controls**: Planet overview uses a 2D top-down camera (or isometric if using 3D terrain). WASD or edge-pan to move, scroll wheel to zoom.
2. **Marker culling**: At high zoom levels, only show markers for buildings in the current viewport. Hide markers that are off-screen to reduce widget overhead.
3. **Click-to-focus**: Clicking a building marker smoothly pans the camera to center on that building's position.

**Toolset:** `blueprint-tools`

### Phase 7 — Compile & Verify

1. **Compile every Blueprint** after creation/modification
2. **Test in PIE**:
   - Open planet overview from Strategic loop start
   - Verify building markers appear at correct grid positions
   - Hover over a marker → tooltip shows building stats
   - Click a marker → camera centers on that building, detail popup opens
   - Check stats panel — verify population, power, resources match actual colony state
   - Zoom in/out → markers cull correctly (off-screen markers hidden)
   - Switch biomes legend → verify color key matches terrain overlay

## Key Patterns & Gotchas

### WidgetComponent for Markers
Building markers use UMG WidgetComponents attached to invisible scene actors at grid positions. This allows them to be positioned in world space and projected onto screen by the camera. At high zoom, many markers may be off-screen — culling them prevents unnecessary widget updates.

### Stats Panel is Read-Only
The Planet Overview stats panel displays information but doesn't allow direct editing. To change worker allocation, power distribution, or other colony parameters, the player must navigate to Colony Management (S02). This screen is purely informational and navigational.

### Threat vs Attraction
- **Threat Level**: Hostile alien presence/risk on this planet. Increases with nearby dungeon activity, hostile race proximity. Affects building safety (threatened buildings may be damaged).
- **Attraction Level**: How much other factions/civilizations are drawn to this planet's resources or strategic position. High attraction may trigger diplomatic events or invasion attempts.
- Both are tracked separately and have independent color scales.

### Multi-Planet Navigation
If the game supports multiple planets in a system, each planet gets its own overview view with separate building markers, stats, and biome data. NavigationDots at the bottom allow switching between planets. Each planet has its own ColonyManager instance.

## File Structure Summary

```
/Game/UI/Planet/
├── T_Planet_Surface.uasset
├── T_Marker_Mine.uasset (and other building marker icons)
├── T_Planet_Threat.uasset
├── T_Planet_Attraction.uasset
├── T_Biome_Desert.uasset (and other biome overlays)
├── WBP_PlanetOverviewRoot.uasset
├── WBP_BuildingMarker.uasset
├── WBP_PlanetStatsPanel.uasset
├── WBP_BiomeLegend.uasset
├── WBP_BuildingInfoTooltip.uasset
└── WBP_NavigationDots.uasset

/Game/Gameplay/Planet/
└── BP_PlanetViewManager.uasset
```

## Next Steps After This Skill

Planet Overview connects to:
- **Colony Management (S02)** — click building markers to open management panels
- **Construction Mode (S04)** — place new buildings visible on this overview
- **Dungeon Entry (S07/S09)** — threat level on this screen indicates dungeon danger
- **Galaxy Map (S12)** — navigation from planet view to galaxy-scale view
