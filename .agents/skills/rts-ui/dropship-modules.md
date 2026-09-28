# S13/S14 Dropship Repair + Ship Module Management — Agent Skill

## What to Create

### Assets

| Asset Name | Texture Reference | Path in Project | Source |
|-----------|------------------|-----------------|--------|
| Hull Integrity Bar BG | `T_Ship_HullBG` | `/Game/UI/Ship/T_Ship_HullBG` | Dark bar, 300x16 PNG |
| Module Slot Background | `T_Ship_ModuleSlot` | `/Game/UI/Ship/T_Ship_ModuleSlot` | Rounded rectangle outline, 120x80 PNG |
| Module Icon (35+ types) | `T_Module_*` | `/Game/UI/Ship/T_Module_ImpulseDrive`, etc. | Icons for each module type |
| Attachment Point Indicator | `T_Ship_AttachPoint` | `/Game/UI/Ship/T_Ship_AttachPoint` | Glowing circle for available attachment points |
| Hull Damage Overlay | `T_Ship_HullDamage` | `/Game/UI/Ship/T_Ship_HullDamage` | Crack/scorch texture overlay |

**Toolset:** `asset-tools`, `import-export-toolset`

### Widget Blueprints (8 total)

| Blueprint Name | Class | Purpose |
|---------------|-------|---------|
| `WBP_ShipRoot` | UserWidget | Root widget — dropship module management screen |
| `WBP_HullDisplay` | UserWidget | Visual hull integrity display with damage overlay and integrity bar |
| `WBP_ModuleSlot` | UserWidget | Single attachment point: shows installed module or empty slot outline |
| `WBP_ModuleCatalog` | Scrollable list of available modules to install/remove |
| `WBP_ModuleDetailPanel` | Modal — detailed stats for selected module, upgrade path, TIR requirements |
| `WBP_RepairStatusPanel` | Panel showing current repair progress and estimated completion time |
| `WBP_ShipStatsBar` | UserWidget — top bar with hull integrity, fuel/energy, key ship stats |
| `WBP_ModuleUpgradeDialog` | Modal — confirm module TIR upgrade with cost breakdown |

**Toolset:** `umg-toolset`, `blueprint-tools`

### Blueprint Classes (Gameplay)

| Name | Type | Purpose |
|------|------|---------|
| `BP_ShipManager` | GameMode/GameState extension | Manages dropship state: hull integrity, installed modules, fuel/energy levels, repair progress |
| `BP_ModuleDefinition` | DataAsset | Per-module data: type, stats, TIR tiers, attachment point requirements, upgrade paths |

**Toolset:** `blueprint-tools`, `data-tools`

### Data Tables

| Name | Purpose |
|------|---------|
| `DG_ShipModules` | All 35+ ship modules with stats, costs, TIR progression, and attachment point types |

**Toolset:** `data-tools`, `blueprint-tools`

## MCP Workflow: Step by Step

### Phase 1 — Inspect & Prepare

1. **Inspect the project** for existing ship/module assets
   - Toolset: `editor-toolset`
2. **Create directory structure**: `/Game/UI/Ship/` and `/Game/Gameplay/Ship/`
   - Toolset: `asset-tools` / `editor-toolset`

### Phase 2 — Import Textures (5 textures)

Import all textures with SRGB = true, TF_Bilinear. Hull integrity bar should stretch seamlessly. Module icons need to be distinctive at small sizes (48x48 minimum).

**Toolset:** `asset-tools`, `import-export-toolset`

### Phase 3 — Create Ship Module Data

#### DG_ShipModules
Columns:
- `ModuleID` (Name) — unique identifier, e.g., "drive.impulse.mark1"
- `ModuleName` (String)
- `ModuleType` (Enum: Drive, Storage, Protection, Scanning, Weapon, Lab, Utility)
- `IconTexture` (Texture2D)
- `AttachmentPoints` (Array of String — which hull slots this module can occupy)
- `TIRTiers` (struct array: tier number, stat values at that tier, upgrade cost)
- `BaseStats` (struct: speed bonus, cargo capacity, defense bonus, scan range, damage, research speed, etc. — varies by type)
- `Description` (String)

Populate with all 35+ modules organized by type:
- **Drives**: Impulse Drive M1-M3, Plasma Drive M1-M2, Warp Core M1 (Elite)
- **Storage**: Cargo Hold L1-L3, Cryo Storage M1-M2, Fuel Tank L1-L2
- **Protection**: Hull Plating M1-M3, Shield Generator M1-M2, Emergency Bulkheads (Elite)
- **Scanning**: Long-range Sensors M1-M2, Life Form Detector M1, Quantum Scanner (Elite)
- **Weapons**: Point Defense M1-M2, Missile Launcher M1-M2, Plasma Cannon (Elite)
- **Labs**: Research Lab M1-M2, Bio Lab M1, Crystal Analysis Bay (Elite)
- **Utility**: Habitat Module M1-M2, Hydroponics M1, Air Recycling M1

**Toolset:** `data-tools`, `blueprint-tools`

### Phase 4 — Create Ship Manager

1. **Create `BP_ShipManager`**:
   - Variables:
     - `HullIntegrity` (float, 0.0-1.0)
     - `FuelEnergy` (float, 0.0-1.0)
     - `InstalledModules` (Array of struct: ModuleID, CurrentTier, SlotIndex)
     - `AvailableSlots` (int — total attachment points on hull)
     - `RepairRate` (float, integrity per game hour during repair)
     - `bIsInRepairMode` (bool)
   - Functions:
     - `InstallModule(ModuleID, slotIndex)` — validates slot compatibility, deducts cost, adds to InstalledModules
     - `RemoveModule(slotIndex)` — removes from InstalledModules, returns module to catalog
     - `UpgradeModule(ModuleID, newTier)` — checks TIR requirement and cost, applies new stats
     - `TickRepair(float deltaTime)` — when in repair mode, increases HullIntegrity by RepairRate * deltaTime
     - `GetEffectiveStat(statName)` → float — sums stat contributions from all installed modules at their current tiers
     - `CanAffordUpgrade(ModuleID, tier)` → bool — checks resource costs against colony pool

2. **Module stat aggregation**: Each module contributes to ship-wide stats. The effective ship speed = base speed + sum of all Drive module speed bonuses. Effective scan range = sum of Scanning module ranges. Stats stack additively across modules of the same type.

**Toolset:** `blueprint-tools`

### Phase 5 — Create UI Widgets (Bottom-Up)

#### WBP_ModuleSlot
- CanvasPanel child with: SlotBackground (Image, T_Ship_ModuleSlot), InstalledModuleIcon (Image centered, visible if module installed), ModuleName (TextBlock below icon), TierBadge (TextBlock, e.g., "TIR 2"), RemoveButton (small X button in corner)
- Variables: `SlotIndex` (int), `InstalledModuleID` (Name, empty if none), `CurrentTier` (int)
- Function `UpdateFromShip(shipManager)` — reads installed module data and updates display
  - If slot is empty: show dashed outline with "Empty" text
  - If occupied: show module icon, name, tier badge
- Event: `OnClicked()` — if empty → opens ModuleCatalog filtered for compatible modules; if occupied → opens ModuleDetailPanel
- Event: `OnRemoveClicked()` — removes module from this slot (with confirmation)

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_HullDisplay
- CanvasPanel child showing visual representation of the ship's hull
- Children: HullShape (Image or custom geometry), HullDamageOverlay (Image, opacity = 1.0 - HullIntegrity), HullIntegrityBar (ProgressBar at bottom of hull shape), IntegrityText (TextBlock, e.g., "75%")
- Variable: reference to ShipManager, function `UpdateFromShip()`:
  - Sets ProgressBar value to HullIntegrity
  - Sets DamageOverlay opacity inversely proportional to integrity
  - Color coding: >60% = green, 30-60% = yellow, <30% = red pulsing
- When integrity is low, the hull shape shows visible crack textures (increasing overlay opacity)

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ModuleCatalog
- Scrollable VerticalBox listing all available modules
- Each entry: ModuleIcon + Name + Type badge + "Install" button (if slot available and resources affordable)
- Filter buttons at top: All / Drives / Storage / Protection / Scanning / Weapons / Labs / Utility
- Clicking a module → opens ModuleDetailPanel for that module
- Install button disabled if no compatible slots available or resources insufficient

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ModuleDetailPanel
- Modal overlay with: ModuleName (large), Type badge, Icon, Current Tier display, Stats breakdown (each stat as a row with icon + value), TIR Upgrade path (vertical chain showing M1 → M2 → M3 with costs and requirements), Install button (if module not yet installed), Upgrade button (if module installed but higher tier available)
- If upgrade requires colony TIR check: show "Requires Colony TIR X" warning if not met

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_RepairStatusPanel
- Panel showing repair progress when in repair mode
- Children: RepairTitle TextBlock ("Repairing Hull"), Progress bar with percentage, Estimated time remaining (calculated from current integrity gap and repair rate), Cancel button
- Variable: reference to ShipManager, function `UpdateFromShip()` reads repair state

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ShipStatsBar
- Top bar with key ship stats displayed horizontally
- Children: HullIntegrity (icon + percentage), Fuel/Energy (icon + percentage), Speed (from drive modules), Scan Range (from scanning modules), Cargo Capacity (from storage modules)
- Variable: reference to ShipManager, function `UpdateFromShip()` reads effective stats by summing installed module contributions

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ModuleUpgradeDialog
- Modal confirming TIR upgrade for a specific module
- Shows: Module name, current tier → new tier, cost breakdown (resource icons + amounts), Colony TIR requirement check
- Yes/No buttons — Yes applies the upgrade and deducts resources

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ShipRoot
- Root: CanvasPanel
- Layout split into two columns (Blueprint-style 2-column layout from design doc):
  - **Left column (40% width)**: HullDisplay at top, ModuleSlots grid below (arranged to match hull attachment point layout)
  - **Right column (60% width)**: ShipStatsBar at top, ModuleCatalog scroll area in middle, RepairStatusPanel at bottom (when repairing)
- Children (z-order):
  1. Left column: HullDisplay + ModuleSlots grid
  2. Right column: ShipStatsBar + ModuleCatalog + RepairStatusPanel
  3. ModuleDetailPanel modal (hidden by default)
  4. ModuleUpgradeDialog modal (hidden by default)

- Events:
  - `OnInitialize()` — load ship state from ShipManager, create module slot widgets for each attachment point
  - `OnModuleInstalled(slotIndex)` — update that slot's display
  - `OnHullDamaged(amount)` — trigger visual damage effect on HullDisplay

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 6 — Wire Repair System

1. **Repair mode activation**: Player clicks "Start Repair" button (available when not in active travel/combat). Ship enters repair mode, hull integrity increases over time at RepairRate.
2. **Repair cost**: Repairing costs resources proportional to the amount repaired. Cost = (integrity restored) × base cost per point × resource type multiplier.
3. **Interruptible**: Player can cancel repair at any time. Uncompleted repair progress is lost (no partial credit).

**Toolset:** `blueprint-tools`

### Phase 7 — Compile & Verify

1. **Compile every Blueprint** after creation/modification
2. **Test in PIE**:
   - Open dropship module screen from Travel loop
   - Verify hull display shows current integrity with correct color coding
   - Click an empty slot → module catalog opens, filtered by compatible types
   - Install a module → slot updates, ship stats bar reflects new stats
   - Click an installed module → detail panel shows stats and upgrade path
   - Upgrade a module to higher TIR tier → verify cost deduction and stat increase
   - Start repair → hull integrity increases over time, progress bar fills
   - Remove a module → slot becomes empty, ship stats decrease accordingly

## Key Patterns & Gotchas

### Two-Column Blueprint Layout
The design doc specifies a 2-column layout: left side shows the physical ship with attachment points, right side shows the module catalog and details. This mirrors real blueprint/technical documentation — visual reference on one side, component list on the other.

### Module Type Compatibility
Each module has an AttachmentPoints array specifying which hull slots it can occupy. A Drive module can only go in drive slots, a Weapon module only in weapon hardpoints, etc. The ModuleCatalog filters available modules based on the selected empty slot's type.

### TIR Upgrade Chains Within Modules
Each module has its own internal TIR progression (M1 → M2 → M3). Upgrading a module tier is separate from the colony's overall TIR — but the colony must have reached a minimum TIR to unlock higher module tiers. For example, Impulse Drive M3 requires Colony TIR 4 even if you have the resources.

### Stat Aggregation is Additive
Ship-wide stats are the sum of all contributing modules. If you install two Drive modules (Impulse M1: +5 speed, Plasma M1: +8 speed), total drive bonus = +13. There's no diminishing return or cap on module stacking — this encourages players to optimize their module loadout for specific travel needs.

### Repair Costs Resources
Repairing isn't free. The amount of hull integrity restored is multiplied by a base cost factor, and the player must have enough resources in their colony pool. This creates a strategic decision: repair now (costly but safe) or risk traveling with damaged hull (cheaper but risk of further damage during travel).

## File Structure Summary

```
/Game/UI/Ship/
├── T_Ship_HullBG.uasset
├── T_Ship_ModuleSlot.uasset
├── T_Module_ImpulseDrive.uasset (and 34+ other module icons)
├── T_Ship_AttachPoint.uasset
├── T_Ship_HullDamage.uasset
├── WBP_ShipRoot.uasset
├── WBP_HullDisplay.uasset
├── WBP_ModuleSlot.uasset
├── WBP_ModuleCatalog.uasset
├── WBP_ModuleDetailPanel.uasset
├── WBP_RepairStatusPanel.uasset
├── WBP_ShipStatsBar.uasset
└── WBP_ModuleUpgradeDialog.uasset

/Game/Gameplay/Ship/
├── BP_ShipManager.uasset
└── BP_ModuleDefinition.uasset (DataAsset)

/Game/Data/
└── DG_ShipModules.uasset (DataTable)
```

## Next Steps After This Skill

Dropship Modules connects to:
- **Galaxy Map (S12)** — ship stats determine travel range and speed between systems
- **Solar System View (S11)** — hull integrity affects navigation safety within a system
- **Tech Tree (S05)** — unlocks new module types through research
- **Resource HUD Bar (S06)** — repair costs and module upgrades consume resources
