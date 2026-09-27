# S11/S12 Galaxy Map + Solar System View — Agent Skill

## What to Create

### Assets

| Asset Name | Texture Reference | Path in Project | Source |
|-----------|------------------|-----------------|--------|
| Star Field Background | `T_Galaxy_Starfield` | `/Game/UI/Galaxy/T_Galaxy_Starfield` | Deep space star field, seamless tile or large texture |
| System Node (visited) | `T_Node_System_Visited` | `/Game/UI/Galaxy/T_Node_System_Visited` | Glowing circle with system color, 48x48 PNG |
| System Node (unvisited) | `T_Node_System_Unvisited` | `/Game/UI/Galaxy/T_Node_System_Unvisited` | Dim/fog-of-war version, 48x48 PNG |
| Warp Route Line | `T_Galaxy_WarpRoute` | `/Game/UI/Galaxy/T_Galaxy_WarpRoute` | Dashed line texture for warp routes between systems |
| Planet Icon (types) | `T_PlanetIcon_*` | `/Game/UI/Galaxy/T_PlanetIcon_Terrestrial`, etc. | Small planet icons for different planet types within a system |
| Jump Point Icon | `T_JumpPoint_Icon` | `/Game/UI/Galaxy/T_JumpPoint_Icon` | Wormhole/jump gate icon, 32x32 PNG |

**Toolset:** `asset-tools`, `import-export-toolset`

### Widget Blueprints (8 total)

| Blueprint Name | Class | Purpose |
|---------------|-------|---------|
| `WBP_GalaxyMapRoot` | UserWidget | Root widget — galaxy-scale star map |
| `WBP_SystemNode` | UserWidget | Clickable node representing a solar system on the galaxy map |
| `WBP_WarpRouteLine` | UserWidget — visual line connecting two systems (warp route) |
| `WBP_SolarSystemRoot` | UserWidget | Root widget — zoomed-in solar system view |
| `WBP_OrbitalBody` | UserWidget — planet/station/jump point within a solar system |
| `WBP_SystemInfoPanel` | Modal — shows scan info for selected system or body |
| `WBP_FuelGauge` | UserWidget — displays remaining fuel/energy for travel |
| `WBP_NavigationConfirmDialog` | Modal — confirm warp jump with route preview and fuel cost |

**Toolset:** `umg-toolset`, `blueprint-tools`

### Blueprint Classes (Gameplay)

| Name | Type | Purpose |
|------|------|---------|
| `BP_GalaxyMapManager` | GameMode/GameState extension | Manages galaxy state: discovered systems, visited systems, warp routes, current position |
| `BP_SolarSystemManager` | Actor | Manages a single solar system's bodies, orbits, and navigation options |

**Toolset:** `blueprint-tools`

### Data Tables

| Name | Purpose |
|------|---------|
| `DG_GalaxySystems` | All discoverable star systems with coordinates, contents, discovery conditions |
| `DG_OrbitalBodies` | Planets, stations, and jump points within each system |

**Toolset:** `data-tools`, `blueprint-tools`

## MCP Workflow: Step by Step

### Phase 1 — Inspect & Prepare

1. **Inspect the project** for existing galaxy/map assets
   - Toolset: `editor-toolset`
2. **Create directory structure**: `/Game/UI/Galaxy/` and `/Game/Gameplay/Galaxy/`
   - Toolset: `asset-tools` / `editor-toolset`

### Phase 2 — Import Textures (6 textures)

Import all textures with SRGB = true, TF_Bilinear. Star field should be a large seamless texture or tileable pattern. System nodes should have distinct colors per system type (blue for gas giants, green for habitable, red for hostile).

**Toolset:** `asset-tools`, `import-export-toolset`

### Phase 3 — Create Galaxy Data

#### DG_GalaxySystems
Columns:
- `SystemID` (Name) — unique identifier, e.g., "system.alpha_centauri"
- `SystemName` (String)
- `GalaxyX`, `GalaxyY` (float) — position on galaxy map (can be polar or Cartesian)
- `bDiscovered` (bool) — whether player knows this system exists
- `bVisited` (bool) — whether player has entered this system
- `ScanLevel` (int 0-3: 0 = unknown, 1 = basic coords, 2 = bodies identified, 3 = full scan)
- `HostilePresence` (float 0.0-1.0) — danger level of this system
- `ResourcesAvailable` (Array of ResourceType) — resources extractable here
- `WarpCost_Fuel` (float) — fuel required to jump TO this system from nearest visited system
- `NearestVisitedSystemID` (Name) — reference for warp route calculation
- `OrbitalBodies` (Array of BodyID referencing DG_OrbitalBodies)

Populate with all systems in the galaxy map. Starting system is discovered and visited; others are discovered progressively through scanning or exploration.

#### DG_OrbitalBodies
Columns:
- `BodyID` (Name) — unique identifier
- `ParentSystemID` (Name) — which system this body belongs to
- `BodyType` (Enum: Planet, Station, JumpPoint)
- `PlanetClass` (Enum: Terrestrial, GasGiant, IceWorld, Volcanic, CrystalCave, Desert, Tundra, Jungle)
- `OrbitRadius` (float) — distance from system center in solar system view
- `OrbitSpeed` (float) — angular speed for animation
- `Name` (String)
- `ScanInfo` (String) — description text revealed at full scan level
- `Resources` (Array of ResourceType) — resources available on this body

**Toolset:** `data-tools`, `blueprint-tools`

### Phase 4 — Create Galaxy Map Manager

1. **Create `BP_GalaxyMapManager`**:
   - Variables:
     - `CurrentSystemID` (Name) — system the player is currently in
     - `DiscoveredSystems` (Array of SystemID)
     - `VisitedSystems` (Array of SystemID)
     - `GalaxyZoomLevel` (float)
     - `CameraX`, `CameraY` (float) — current viewport center on galaxy map
   - Functions:
     - `DiscoverSystem(SystemID)` — adds to DiscoveredSystems, reveals system node on map
     - `TravelToSystem(targetSystemID)` — validates warp route exists (connected to visited system), checks fuel cost, transitions to Solar System View
     - `GetWarpRoute(currentSystem, targetSystem)` → array of SystemIDs (path through intermediate systems)
     - `CalculateFuelCost(route)` → total fuel for the entire warp path
     - `CanReachSystem(SystemID)` → bool — true if a valid warp route exists from any visited system and player has enough fuel
     - `ScanSystem(SystemID, scanStrength)` → increases ScanLevel, reveals more orbital body data
   - The galaxy is a graph of systems connected by potential warp routes. Players can only jump between directly connected systems or through intermediate systems (pathfinding).

**Toolset:** `blueprint-tools`

### Phase 5 — Create Galaxy Map UI Widgets (Bottom-Up)

#### WBP_SystemNode
- CanvasPanel child with: NodeBackground (Image, T_Node_System_Visited or Unvisited), SystemName (TextBlock below node), HostileIndicator (small icon in corner if hostile presence > 0.3)
- Variables: `SystemID` (Name), reference to GalaxyMapManager
- Function `UpdateFromGalaxy()`:
  - If not discovered: completely hidden (fog of war)
  - If discovered but not visited: show dim node with name
  - If visited: show bright glowing node
  - Hostile presence > threshold: red warning icon appears
- Event: `OnClicked()` — if discovered → opens SystemInfoPanel; if player is IN this system → transitions to Solar System View (S11)
- Event: `OnHovered()` — shows tooltip with basic info (name, hostile level, resources)

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_WarpRouteLine
- CanvasPanel child with a Line or stretched Image between two SystemNode positions
- Visual: dashed line in accent color (#E8852A), opacity based on whether the route is currently selected for travel
- If route is selected (player hovering target system): bright solid line
- If route exists but not selected: dim dashed line

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_FuelGauge
- HorizontalBox with FuelIcon (Image), FuelBar (ProgressBar), FuelText (TextBlock, e.g., "75/100")
- Variable: reference to ShipManager or GalaxyMapManager, function `UpdateFromShip()` reads current fuel level
- Color coding: >40% = green, 20-40% = yellow, <20% = red pulsing
- Appears on both Galaxy Map and Solar System views

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_SystemInfoPanel
- Modal overlay showing scan data for a system
- Children: SystemName (large), ScanLevel indicator, Hostile presence bar, Resources list (icons + names), Orbital bodies summary (list of planets/stations with types), Warp cost to reach (if not current system), "Travel" button (if reachable and has fuel)
- Variable: `SystemID` — function `UpdateFromGalaxy()` reads all data from DG_GalaxySystems

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_NavigationConfirmDialog
- Modal confirming warp jump
- Shows: Destination system name, route preview (list of intermediate systems), Total fuel cost, Estimated travel time, Warning if destination has high hostile presence
- Yes → executes travel, transitions to Solar System View
- No / Escape → cancels

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_GalaxyMapRoot
- Root: CanvasPanel
- Children (z-order back to front):
  1. StarField background (Image filling entire canvas, parallax-scrolls with camera movement)
  2. WarpRouteLine instances between connected systems
  3. SystemNode instances at their galaxy coordinates
  4. FuelGauge at top-right corner
  5. SystemInfoPanel modal (hidden by default)
  6. NavigationConfirmDialog modal (hidden by default)

- Camera controls: Pan with WASD or mouse drag, zoom with scroll wheel
- On zoom in close enough to a system → auto-transitions to Solar System View
- Hovering over an unvisited but discovered system → highlights warp route from nearest visited system

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 6 — Create Solar System UI Widgets (Bottom-Up)

#### WBP_OrbitalBody
- CanvasPanel child positioned at orbital position (calculated from orbit radius and angle)
- Children: BodyIcon (Image, planet type icon), BodyName (TextBlock below), OrbitRing (thin circle showing orbital path — decorative)
- Variables: `BodyID` (Name), `OrbitRadius`, `OrbitSpeed`, `CurrentAngle` (float, updated on tick for animation)
- Function `UpdatePosition(float deltaTime)`:
  - CurrentAngle += OrbitSpeed * deltaTime
  - X = centerX + orbitRadius * cos(CurrentAngle)
  - Y = centerY + orbitRadius * sin(CurrentAngle)
  - Updates widget position each frame for smooth orbital animation
- Event: `OnClicked()` — opens SystemInfoPanel with detailed body scan data
- Event: `OnHovered()` — shows tooltip with body type and resources

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_SolarSystemRoot
- Root: CanvasPanel
- Children (z-order back to front):
  1. StarField background (different from galaxy view — closer, more detailed)
  2. SystemCenter (decorative star or central body at center of screen)
  3. OrbitalBody instances arranged in orbit around center
  4. FuelGauge at top-right
  5. Navigation button (top-left) → returns to Galaxy Map view
  6. SystemInfoPanel modal (hidden by default)

- Camera: Zoomable view of the solar system, pan to follow specific bodies
- Orbital animation: All bodies orbit at their defined speeds, creating a living dynamic view
- Clicking the central star → shows system overview info
- Clicking a planet → shows planet scan data and available resources

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 7 — Wire Travel Flow

1. **Galaxy Map → Solar System**: Player clicks a visited system node → transition animation (zoom in) → Solar System View loads with that system's orbital bodies.
2. **Solar System → Galaxy Map**: Navigation button in top-left returns to galaxy map at the current system's position.
3. **Warp travel**: On Galaxy Map, player hovers over a reachable system → warp route highlights → clicks node → confirms in dialog → fuel deducted → transition animation → Solar System View of destination.

**Toolset:** `blueprint-tools`

### Phase 8 — Compile & Verify

1. **Compile every Blueprint** after creation/modification
2. **Test in PIE**:
   - Open galaxy map from Travel loop
   - Verify discovered systems show as nodes, undiscovered are hidden
   - Pan and zoom galaxy view smoothly
   - Hover over a system → tooltip shows info
   - Click a visited system → solar system view loads with orbital bodies
   - Orbital bodies animate correctly (orbit around center)
   - Click an orbiting body → scan info panel opens
   - Initiate warp jump to another system → confirm dialog shows route and fuel cost
   - After travel → arrive at destination system, new bodies visible

## Key Patterns & Gotchas

### Fog of War Progression
Systems start completely hidden (not even as dim nodes). They become discoverable through: scanning from nearby systems, completing certain tech tree topics, or random exploration events. Once discovered, they appear as dim nodes. Visiting a system brightens the node and reveals orbital body data up to the scan level.

### Orbital Animation is Visual Only
The orbital body animation in Solar System View is cosmetic — it doesn't affect gameplay mechanics like range or line-of-sight. Bodies are always clickable regardless of their current orbital position. The animation provides visual feedback that the system is "alive" and helps distinguish different bodies by their relative motion speeds.

### Fuel Cost is Path-Based
Warp travel cost depends on the full path, not just direct distance. If System C requires jumping through System B first, the total fuel cost = cost(B→C) + any intermediate costs. The player must have enough fuel for the entire route, not just the final leg.

### Scan Levels Unlock Information Gradually
- **Scan 0**: System doesn't exist in player's knowledge
- **Scan 1**: System discovered, coordinates visible on map, basic name known
- **Scan 2**: Orbital body types identified (planet/gas giant/etc.), but not names or resources
- **Scan 3**: Full data — all body names, resource locations, hostile presence details

Scanning requires a Scanning module installed on the ship and consumes fuel/energy proportional to scan depth.

### Hostile Presence Affects Travel
High hostile presence in a system increases the risk of enemy encounters during travel. This is represented by a warning icon on the system node and a red tint on the warp route line. Systems with hostile presence > 0.7 may require combat readiness (specific module loadout) to enter safely.

## File Structure Summary

```
/Game/UI/Galaxy/
├── T_Galaxy_Starfield.uasset
├── T_Node_System_Visited.uasset
├── T_Node_System_Unvisited.uasset
├── T_Galaxy_WarpRoute.uasset
├── T_PlanetIcon_Terrestrial.uasset (and other planet type icons)
├── T_JumpPoint_Icon.uasset
├── WBP_GalaxyMapRoot.uasset
├── WBP_SystemNode.uasset
├── WBP_WarpRouteLine.uasset
├── WBP_SolarSystemRoot.uasset
├── WBP_OrbitalBody.uasset
├── WBP_SystemInfoPanel.uasset
├── WBP_FuelGauge.uasset
└── WBP_NavigationConfirmDialog.uasset

/Game/Gameplay/Galaxy/
├── BP_GalaxyMapManager.uasset
└── BP_SolarSystemManager.uasset

/Game/Data/
├── DG_GalaxySystems.uasset (DataTable)
└── DG_OrbitalBodies.uasset (DataTable)
```

## Next Steps After This Skill

Galaxy Map + Solar System connects to:
- **Dropship Modules (S13/S14)** — ship stats determine travel range and capabilities
- **Planet Overview (S01)** — entering a system with planets leads to planet overview
- **Resource HUD Bar (S06)** — fuel is one of the tracked resources
- **Tech Tree (S05)** — scanning modules unlocked through research
