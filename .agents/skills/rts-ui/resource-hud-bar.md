# S06 Resource HUD Bar — Agent Skill

## What to Create

### Assets

| Asset Name | Texture Reference | Path in Project | Source |
|-----------|------------------|-----------------|--------|
| Resource Icon (6 types) | `T_Resource_*` | `/Game/UI/Resources/T_Resource_Material`, etc. | Icons for each of 6 resources |
| Production Arrow Up | `T_HUD_ArrowUp` | `/Game/UI/Resources/T_HUD_ArrowUp` | Green up arrow, 12x8 PNG |
| Production Arrow Down | `T_HUD_ArrowDown` | `/Game/UI/Resources/T_HUD_ArrowDown` | Red down arrow, 12x8 PNG |
| HUD Background (strategic) | `T_HUD_StrategicBG` | `/Game/UI/Resources/T_HUD_StrategicBG` | Semi-transparent bar background, full-width top |
| HUD Background (tactical compact) | `T_HUD_TacticalBG` | `/Game/UI/Resources/T_HUD_TacticalBG` | Compact dark panel, 200x40 PNG |
| HUD Background (travel minimal) | `T_HUD_TravelBG` | `/Game/UI/Resources/T_HUD_TravelBG` | Small corner panel, 160x32 PNG |

**Toolset:** `asset-tools`, `import-export-toolset`

### Widget Blueprints (5 total)

| Blueprint Name | Class | Purpose |
|---------------|-------|---------|
| `WBP_ResourceHUD_Strategic` | UserWidget | Full-width top bar variant for Strategic loop scenes |
| `WBP_ResourceHUD_Tactical` | Compact bottom/side variant for Tactical combat scenes |
| `WBP_ResourceHUD_Travel` | Small corner variant for Travel loop scenes |
| `WBP_ResourceItem` | UserWidget | Single resource row: icon, name, amount, production rate — reusable in all variants |
| `WBP_ResourceDetailPanel` | Modal — expanded view of one resource with source breakdown |

**Toolset:** `umg-toolset`, `blueprint-tools`

### Blueprint Classes (Gameplay)

| Name | Type | Purpose |
|------|------|---------|
| `BP_ResourcePool` | ActorComponent or GameState extension | Central resource tracking: current amounts, production rates, storage capacities for all 6 resources |

**Toolset:** `blueprint-tools`

## MCP Workflow: Step by Step

### Phase 1 — Inspect & Prepare

1. **Inspect the project** for existing HUD/resource assets
   - Toolset: `editor-toolset`
2. **Create directory structure**: `/Game/UI/Resources/`
   - Toolset: `asset-tools` / `editor-toolset`

### Phase 2 — Import Textures (6 textures)

Import all textures with SRGB = true, TF_Bilinear. Resource icons should be distinctive and readable at small sizes (32x32 minimum). HUD backgrounds should have slight transparency for overlay effect.

**Toolset:** `asset-tools`, `import-export-toolset`

### Phase 3 — Create Resource Pool System

1. **Create `BP_ResourcePool`**:
   - Variables: Struct `ResourceState` with fields: `ResourceType` (GameplayTag), `CurrentAmount` (int), `MaxCapacity` (int), `ProductionRate` (float, positive = producing, negative = consuming, 0 = balanced)
   - Array of ResourceState for all 6 resources
   - Functions:
     - `AddResource(resourceType, amount)` — adds to current, clamps to max capacity
     - `RemoveResource(resourceType, amount)` — subtracts, returns false if insufficient
     - `TickProduction(float deltaTime)` — called every game tick; updates CurrentAmount += ProductionRate * deltaTime
     - `GetResourceAmount(resourceType)` → int
     - `GetProductionRate(resourceType)` → float
   - This is the single source of truth for all resource data. All HUD variants read from this actor/component.

**Toolset:** `blueprint-tools`

### Phase 4 — Create Resource Item Widget (Reusable Component)

#### WBP_ResourceItem
- HorizontalBox with: ResourceIcon (Image, 32x32), ResourceName (TextBlock, small font), AmountText (TextBlock, e.g., "150"), ProductionArrow (Image — up arrow green if rate > 0, down arrow red if rate < 0, hidden if rate = 0), RateText (TextBlock, e.g., "+2/h" or "-1/h")
- Variables: `ResourceType` (GameplayTag), reference to ResourcePool
- Function `UpdateDisplay()`:
  - Reads CurrentAmount and ProductionRate from ResourcePool
  - Sets AmountText to formatted string
  - Sets ProductionArrow visibility and color based on rate sign
  - Sets RateText to formatted rate string
  - Color coding: amount < 10% of max → text turns red; amount 10-30% → yellow; above 30% → white
- On click → opens ResourceDetailPanel for this resource

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 5 — Create HUD Variants

#### WBP_ResourceHUD_Strategic (full-width top bar)
- HorizontalBox filling the top of the screen (Anchor: top-center, width = 90% of canvas)
- Children: 6 ResourceItem instances arranged horizontally, evenly spaced
- Background: T_HUD_StrategicBG stretched behind all items with slight transparency (0.7)
- Each resource item shows full info: icon, name, amount, arrow, rate text
- Clicking any item → opens ResourceDetailPanel as a dropdown below that item

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ResourceHUD_Tactical (compact bottom/side)
- CanvasPanel child positioned at bottom-center or right side (depends on scene layout)
- Dimensions: ~200x40 units, much more compact than strategic variant
- Children: 6 ResourceItem instances arranged in a 2-row grid (3 per row) OR a single horizontal row with minimal info (icon + amount only, no name or rate)
- Background: T_HUD_TacticalBG
- Trade-off: less screen space available during combat, so names and rates are hidden. Only icon + current amount shown. Rate appears on hover as tooltip.

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ResourceHUD_Travel (small corner minimal)
- CanvasPanel child positioned at top-left corner
- Dimensions: ~160x32 units, smallest variant
- Children: 6 ResourceItem instances in a single horizontal row with icon + amount only (name and rate hidden entirely)
- Background: T_HUD_TravelBG
- On hover over any resource → expands to show full info temporarily (0.5s animation), collapses after 2 seconds of no interaction

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ResourceDetailPanel
- Modal or dropdown panel showing detailed breakdown for one resource
- Children: ResourceName (large), Current/Max display with progress bar, ProductionRate (large number with arrow), SourceBreakdown (list of buildings producing this resource with individual rates), ConsumptionList (list of buildings consuming this resource)
- Variable: `ResourceType` — function `UpdateFromPool()` reads all data from BP_ResourcePool and populates the breakdown lists by querying ColonyManager for active buildings

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 6 — Wire HUD to Scenes

1. **Strategic loop scenes** (Planet Overview S01, Colony Management S02, Construction Mode S04, Tech Tree S05):
   - Add WBP_ResourceHUD_Strategic as a child of the scene's root widget
   - Anchor to top-center so it stays visible during scrolling

2. **Tactical loop scenes** (Dungeon Entry S07, Tactical Combat S08, Combat Results S10):
   - Add WBP_ResourceHUD_Tactical as a child of the combat root widget
   - Position at bottom-center or right side depending on available screen space
   - Compact mode: icon + amount only

3. **Travel loop scenes** (Solar System S11, Galaxy Map S12, Dropship Repair S13, Ship Module Mgmt S14):
   - Add WBP_ResourceHUD_Travel as a child of the travel root widget
   - Anchor to top-left corner

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 7 — Compile & Verify

1. **Compile every Blueprint** after creation/modification
2. **Test in PIE**:
   - In each scene type, verify the correct HUD variant appears
   - Confirm resource amounts update when production changes (e.g., build a mine → material rate goes positive)
   - Test hover expansion on Travel variant
   - Click a resource in Strategic variant → detail panel shows source breakdown
   - Verify compact tactical mode doesn't obstruct combat UI elements

## Key Patterns & Gotchas

### Three Variants, One Data Source
All three HUD variants read from the same BP_ResourcePool actor. The only difference is layout and information density. This means:
- No synchronization issues between scenes
- Adding a new resource type requires changes in only one place (ResourcePool + ResourceItem)
- Each scene chooses which variant to include in its root widget

### Production Rate Calculation
ProductionRate is calculated every game tick by summing all contributing buildings' outputs. A mine produces Material at rate X per hour. A factory consumes Material and produces ProcessedGoods. The net rate for Material = (all mines) - (all factories). If negative, the colony is consuming faster than producing.

### Color Coding Thresholds
- **Amount color**: Red when < 10% of max capacity (critical), Yellow when 10-30%, White when > 30%
- **Arrow color**: Green for positive production rate, Red for negative
- These thresholds are hardcoded in the ResourceItem widget's UpdateDisplay() function

### Hover Expansion Timing (Travel Variant Only)
The Travel variant's hover expansion uses a timer: on hover, show full info immediately. Start a 2-second countdown. If mouse leaves before countdown ends, collapse back to minimal view. If countdown reaches zero, keep expanded until mouse leaves. This prevents constant flickering from accidental hovers.

### Performance Consideration
The Strategic variant has 6 ResourceItem widgets updating every tick (or every game hour). Each UpdateDisplay() call is lightweight (just reading integers and setting text), but if the HUD is visible in scenes with heavy other UI, consider throttling updates to once per second instead of every frame.

## File Structure Summary

```
/Game/UI/Resources/
├── T_Resource_Material.uasset (and 5 other resource icons)
├── T_HUD_ArrowUp.uasset
├── T_HUD_ArrowDown.uasset
├── T_HUD_StrategicBG.uasset
├── T_HUD_TacticalBG.uasset
└── T_HUD_TravelBG.uasset
├── WBP_ResourceHUD_Strategic.uasset
├── WBP_ResourceHUD_Tactical.uasset
├── WBP_ResourceHUD_Travel.uasset
├── WBP_ResourceItem.uasset
└── WBP_ResourceDetailPanel.uasset

/Game/Gameplay/Resources/
└── BP_ResourcePool.uasset (Actor or Component)
```

## Next Steps After This Skill

Resource HUD Bar is cross-cutting — it appears in ALL game loops. It connects to:
- **Colony Management (S02)** — production rates change when workers are allocated
- **Construction Mode (S04)** — building costs shown in resource icons during placement
- **All scene skills** — each includes the appropriate HUD variant in their root widget
