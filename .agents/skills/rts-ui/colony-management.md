# S02 Colony Management — Agent Skill

## What to Create

### Assets

| Asset Name | Texture Reference | Path in Project | Source |
|-----------|------------------|-----------------|--------|
| Worker Icon | `T_Colony_Worker` | `/Game/UI/Colony/T_Colony_Worker` | Person/pickaxe icon, 48x48 PNG |
| Building Slot Icon (per building type) | `T_Building_*` | `/Game/UI/Colony/T_Building_Mine`, etc. | Small icons for each building category |
| Production Arrow | `T_Colony_ProductionArrow` | `/Game/UI/Colony/T_Colony_ProductionArrow` | Rightward arrow, 32x16 PNG |
| Priority Bar Background | `T_Colony_PriorityBG` | `/Game/UI/Colony/T_Colony_PriorityBG` | Dark bar background, 200x8 PNG |
| Tab Active / Normal | `T_Colony_Tab_*` | `/Game/UI/Colony/T_Colony_Tab_Active`, etc. | From UI component library |

**Toolset:** `asset-tools`, `import-export-toolset`

### Widget Blueprints (10 total)

| Blueprint Name | Class | Purpose |
|---------------|-------|---------|
| `WBP_ColonyRoot` | UserWidget | Root widget for colony management screen |
| `WBP_BuildingSlotCard` | UserWidget | Card showing a single building: icon, name, worker count slider, production output |
| `WBP_WorkerAllocationPanel` | Tab content — shows all buildings with worker sliders |
| `WBP_ProductionPriorityPanel` | Tab content — priority list for resource processing order |
| `WBP_StorageOverviewPanel` | Tab content — storage capacity vs current usage per resource |
| `WBP_BuildingUpgradePanel` | Modal/panel — shows upgrade path for selected building, TIR requirements |
| `WBP_TabBar_Colony` | UserWidget | Reusable tab bar (Workers / Production / Storage) |
| `WBP_ResourceRow` | UserWidget | Single resource row: icon, name, current/max, production rate (+/-), expandable detail |
| `WBP_BuildingDetailPopup` | Modal — detailed view of selected building with all stats |
| `WBP_ColonyStatsBar` | UserWidget | Top bar showing total population, total power, threat level |

**Toolset:** `umg-toolset`, `blueprint-tools`

### Blueprint Classes (Gameplay)

| Name | Type | Purpose |
|------|------|---------|
| `BP_ColonyManager` | GameMode/GameState extension | Central authority for all colony state: worker allocation, production rates, storage levels |
| `BP_BuildingActor` | Actor | Represents a placed building; has worker slots, production output, upgrade level |
| `BP_WorkerPool` | ActorComponent | Tracks available workers, allocated workers per building type |

**Toolset:** `blueprint-tools`, `actor-tools`

### Data Tables

| Name | Purpose |
|------|---------|
| `DG_BuildingProduction` | Per-building production data: output resource, base rate (units/hour), worker efficiency multiplier |
| `DG_BuildingUpgrades` | Upgrade paths per building: TIR requirement, cost, new stats, prerequisite upgrades |

**Toolset:** `data-tools`, `blueprint-tools`

## MCP Workflow: Step by Step

### Phase 1 — Inspect & Prepare

1. **Inspect the project** for existing colony/management assets
   - Toolset: `editor-toolset`
2. **Create directory structure**: `/Game/UI/Colony/` and `/Game/Gameplay/Colony/`
   - Toolset: `asset-tools` / `editor-toolset`

### Phase 2 — Import Textures (5 textures)

Import all textures from the asset table with SRGB = true, TF_Bilinear filter. Worker icon should be a simple silhouette suitable for small UI sizes.

**Toolset:** `asset-tools`, `import-export-toolset`

### Phase 3 — Create Data Tables

#### DG_BuildingProduction
Columns: `BuildingID` (GameplayTag), `OutputResource` (GameplayTag), `BaseRate` (float, units per game hour), `WorkerEfficiency` (float, multiplier per worker), `MaxWorkers` (int), `RequiresPower` (bool), `PowerConsumption` (int)

Populate with production data for all extractive and productive buildings:
- Mine → outputs raw material, base rate varies by resource type and biome
- Oil Pump → crude oil, moderate rate
- Harvester Post → biomass, high rate but low value
- Factory → converts raw materials to processed goods, needs input resources
- Forge → metal ingots from ore
- Refinery → refined fuel from crude oil

#### DG_BuildingUpgrades
Columns: `BuildingID` (GameplayTag), `UpgradeTier` (int TIR 2-5), `CostResources` (struct or separate detail rows), `NewOutputRate` (float), `NewMaxWorkers` (int), `PrerequisiteUpgrade` (GameplayTag, empty for first upgrade)

**Toolset:** `data-tools`, `blueprint-tools`

### Phase 4 — Create Building Actor

1. **Create `BP_BuildingActor`** (extends Actor):
   - Components: StaticMesh (building visual), BP_GridCell (grid position data), SceneComponent (root)
   - Variables:
     - `BuildingID` (GameplayTag)
     - `CurrentTier` (int, starts at 1)
     - `AllocatedWorkers` (int)
     - `MaxWorkers` (int) — from DG_BuildingProduction
     - `bIsProducing` (bool)
     - `ProductionRate` (float) — calculated from base rate * worker efficiency
   - Events:
     - `OnInitialize()` — loads data from DG_BuildingProduction, sets initial stats
     - `SetWorkerCount(int)` — validates against MaxWorkers, updates ProductionRate
     - `TickProduction(float DeltaTime)` — called every game hour; produces output resources based on rate
     - `UpgradeToTier(newTier)` — checks DG_BuildingUpgrades for requirements, applies new stats
     - `GetProductionOutput()` — returns resource type and amount produced this tick

2. **Integrate with placed buildings**: When a building is placed via Construction Mode (S04), spawn BP_BuildingActor at the grid position instead of a generic mesh actor.

**Toolset:** `blueprint-tools`, `actor-tools`

### Phase 5 — Create Colony Manager

1. **Create `BP_ColonyManager`**:
   - Variables:
     - `AvailableWorkers` (int) — total population minus workers already allocated
     - `TotalPopulation` (int) — from Habitation Module capacity
     - `Buildings` (Array of BP_BuildingActor references)
     - `ResourceStorage` (struct array: resource type, current amount, max capacity)
     - `ProductionQueue` (array of pending production orders)
   - Functions:
     - `AllocateWorkers(BuildingID, workerCount)` — validates available workers, updates building and colony totals
     - `DeallocateWorkers(BuildingID, workerCount)` — returns workers to pool
     - `GetTotalPowerGeneration()` — sum of all power-generating buildings
     - `GetTotalPowerConsumption()` — sum of all power-consuming buildings
     - `AddResource(ResourceType, amount)` — adds to storage with overflow check against capacity
     - `RemoveResource(ResourceType, amount)` — subtracts from storage, returns false if insufficient
     - `GetBuildingByID(BuildingID)` → BP_BuildingActor reference
   - Tick event: call `TickProduction()` on all buildings every game hour

**Toolset:** `blueprint-tools`

### Phase 6 — Create Worker Pool Component

1. **Create `BP_WorkerPool` ActorComponent**:
   - Variables: `TotalPopulation` (int), `HousingCapacity` (int, from Habitation Modules), `AllocatedWorkers` (int)
   - Functions:
     - `GetAvailableWorkers()` → TotalPopulation - AllocatedWorkers
     - `TryAllocate(count)` → returns true if enough available, decrements counter
     - `Release(count)` → increments counter, clamps to TotalPopulation
   - Housing capacity is determined by total Habitation Module count × capacity per module

**Toolset:** `blueprint-tools`

### Phase 7 — Create UI Widgets (Bottom-Up)

#### WBP_ResourceRow
- HorizontalBox with: ResourceIcon (Image), ResourceName (TextBlock), CurrentAmount (TextBlock, e.g., "150/200"), ProductionRate (TextBlock with +/- color coding), ExpandArrow (Image, rotates on expand)
- Variable: `ResourceType` (GameplayTag), `CurrentAmount`, `MaxCapacity`, `ProductionRate`
- Function `UpdateDisplay()` — refreshes all text and colors
- On click → expands to show detail panel (storage breakdown by source building)

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_BuildingSlotCard
- CanvasPanel child with: BuildingIcon (Image), BuildingName (TextBlock), WorkerSlider (custom slider from 0 to MaxWorkers), ProductionOutput (TextBlock showing resource + rate), UpgradeButton (visible if upgrades available)
- Variables: `BuildingID` (GameplayTag), reference to parent ColonyManager
- Events:
  - `OnInitialize()` — loads building data, sets slider max
  - `OnWorkerSliderChanged(float)` — calls ColonyManager.AllocateWorkers() or DeallocateWorkers()
  - `OnUpgradeClicked()` — opens BuildingUpgradePanel for this building

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ColonyStatsBar
- HorizontalBox at top of screen
- Children: Population counter (icon + text), Power meter (generation/consumption), Threat level indicator (color-coded bar)
- Variable: reference to ColonyManager, function `UpdateFromColony()` reads current totals

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_TabBar_Colony
- HorizontalBox with tab buttons for Workers / Production / Storage
- Active tab highlighted with accent color (#E8852A)
- Event: `OnTabSelected(tabIndex)` — fires when user clicks a tab, parent widget handles showing/hiding panels

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_WorkerAllocationPanel (tab content)
- Scrollable list of BuildingSlotCard instances
- One card per building in the colony
- Each card shows current worker count and allows adjustment via slider
- Total allocated workers displayed at top: "Workers: 45/60"

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ProductionPriorityPanel (tab content)
- VerticalBox with priority-ranked resource rows
- Each row has a drag handle (for reordering), resource icon, current stockpile, and target threshold slider
- Higher priority resources get production allocation first when capacity is limited
- Variable: `PriorityList` (array of resource types in order)

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_StorageOverviewPanel (tab content)
- Grid layout showing all 6 resources with: icon, current/max bar, percentage fill, source breakdown
- Each resource row expandable to show which buildings contribute to that resource
- Color coding: green (>50% full), yellow (20-50%), red (<20%)

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_BuildingUpgradePanel
- Modal overlay with building info at top, upgrade path below as a vertical chain of nodes
- Each node shows: tier number, cost (resource icons + amounts), new stats, prerequisite check (grayed out if not met)
- Clickable "Upgrade" button on the current available tier
- Close button or Escape key dismisses

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_BuildingDetailPopup
- Modal with full building stats: name, tier, size, workers, production rate, power draw, biome compatibility
- Shows upgrade path preview (next available upgrade)
- "Demolish" button at bottom (with confirmation dialog) — returns workers to pool, removes building from colony

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ColonyRoot
- Root: CanvasPanel
- Children (z-order back to front):
  1. ColonyStatsBar at top
  2. TabBar below stats bar
  3. Tab content area (WorkerAllocation / ProductionPriority / StorageOverview) — WidgetSwitcher pattern, only active tab visible
  4. BuildingDetailPopup (hidden by default)
  5. BuildingUpgradePanel (hidden by default)

- Events:
  - `OnInitialize()` — bind to ColonyManager events, populate worker allocation cards
  - `OnTabChanged(index)` — show/hide tab panels via WidgetSwitcher
  - `OnBuildingSelected(buildingID)` — open detail popup for that building

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 8 — Wire Game Time Integration

1. **Game hour tick**: ColonyManager.TickProduction() runs every game hour (configurable via project settings). Each building produces its output based on worker count and efficiency.
2. **Resource distribution**: When production occurs, resources are added to the colony's storage pool. The ProductionPriority panel determines allocation order when storage is near capacity.
3. **Worker population growth**: Population increases over time based on Habitation Module count and food/comfort resource availability. ColonyManager tracks this with a slow tick (every 10 game hours).

**Toolset:** `blueprint-tools`

### Phase 9 — Compile & Verify

1. **Compile every Blueprint** after creation/modification
2. **Test in PIE**:
   - Open colony management screen from Strategic loop
   - Verify stats bar shows correct population, power, threat values
   - Adjust worker sliders on multiple buildings → verify allocation updates and available workers decrease
   - Switch between Workers / Production / Storage tabs → panels swap correctly
   - Click a building card → detail popup opens with full stats
   - Trigger an upgrade (if TIR tier allows) → verify cost deduction and stat changes
   - Demolish a building → workers return to pool, building removed from list
3. **Verify production tick**: Fast-forward game time, confirm resources increase at expected rates

## Key Patterns & Gotchas

### Worker Allocation is Real-Time
Unlike turn-based games, worker allocation in the idle-RTS loop happens continuously. When you move a slider from 5 to 8 workers, those 3 workers immediately start producing at the new rate on the next game hour tick. There's no "confirm" step — changes are instant.

### Production Rate Formula
`ActualRate = BaseRate × (AllocatedWorkers / MaxWorkers) × WorkerEfficiencyMultiplier`

If AllocatedWorkers is 0, production stops entirely. A building with workers but insufficient power produces at 50% rate (warning state). At full power and workers, it runs at 100%.

### Storage Overflow Protection
When a resource reaches max capacity, its producing buildings automatically reduce output to prevent waste. The ProductionPriority panel lets you set target thresholds — if a high-priority resource is near its threshold, production shifts toward it even if lower-priority resources are overflowing.

### Upgrade Prerequisite Chains
Upgrades form chains within each building type. You must complete Tier 2 before Tier 3, and some upgrades require specific other buildings at certain tiers. DG_BuildingUpgrades.PrerequisiteUpgrade handles this — if the prerequisite isn't met, the upgrade node is grayed out with a tooltip explaining what's needed.

### Population vs Housing
TotalPopulation cannot exceed total HousingCapacity (sum of all Habitation Module capacities). If population grows beyond housing, there's a penalty: worker efficiency drops by 25% per person over capacity. This creates natural pressure to expand housing alongside production.

## File Structure Summary

```
/Game/UI/Colony/
├── T_Colony_Worker.uasset
├── T_Building_Mine.uasset (and other building icons)
├── T_Colony_ProductionArrow.uasset
├── T_Colony_PriorityBG.uasset
├── T_Colony_Tab_Active.uasset
├── T_Colony_Tab_Normal.uasset
├── WBP_ColonyRoot.uasset
├── WBP_BuildingSlotCard.uasset
├── WBP_WorkerAllocationPanel.uasset
├── WBP_ProductionPriorityPanel.uasset
├── WBP_StorageOverviewPanel.uasset
├── WBP_BuildingUpgradePanel.uasset
├── WBP_TabBar_Colony.uasset
├── WBP_ResourceRow.uasset
├── WBP_BuildingDetailPopup.uasset
└── WBP_ColonyStatsBar.uasset

/Game/Gameplay/Colony/
├── BP_ColonyManager.uasset
├── BP_BuildingActor.uasset
└── BP_WorkerPool.uasset

/Game/Data/
├── DG_BuildingProduction.uasset (DataTable)
└── DG_BuildingUpgrades.uasset (DataTable)
```

## Next Steps After This Skill

Colony Management connects to:
- **Construction Mode (S04)** — place new buildings to expand production capacity
- **Tech Tree (S05)** — unlock building upgrades and new types through research
- **Resource HUD Bar (S06)** — cross-cutting display of resource levels during all loops
