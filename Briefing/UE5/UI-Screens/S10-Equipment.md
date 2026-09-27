# Unit Upgrade + Equipment System — Agent Skill

## What to Create

### Assets

| Asset Name | Texture Reference | Path in Project | Source |
|-----------|------------------|-----------------|--------|
| Equipment Icon (weapon types) | `T_Weapon_*` | `/Game/UI/Equipment/T_Weapon_Ballistic`, etc. | Icons for 5 weapon types |
| Equipment Icon (armor types) | `T_Armor_*` | `/Game/UI/Equipment/T_Armor_Heavy`, etc. | Icons for armor variants |
| Equipment Icon (utility types) | `T_Utility_*` | `/Game/UI/Equipment/T_Utility_Scanner`, etc. | Icons for utility items |
| Rarity Borders (5) | `T_RarityBorder_*` | `/Game/UI/Equipment/T_RarityBorder_Common`, etc. | Colored border frames: gray/green/blue/purple/orange |
| Upgrade Arrow | `T_Upgrade_Arrow` | `/Game/UI/Equipment/T_Upgrade_Arrow` | Rightward arrow, 24x16 PNG |

**Toolset:** `asset-tools`, `import-export-toolset`

### Widget Blueprints (7 total)

| Blueprint Name | Class | Purpose |
|---------------|-------|---------|
| `WBP_EquipmentRoot` | UserWidget | Root widget — unit upgrade and equipment screen |
| `WBP_UnitUpgradeCard` | UserWidget | Card showing a unit with its current upgrades and TIR progression |
| `WBP_EquipmentPanel` | Tab content — shows all equipment slots for selected unit |
| `WBP_EquipItemCard` | UserWidget — single equipment item: icon, name, rarity, stats |
| `WBP_TIRProgressionBar` | UserWidget — visual TIR tier progression (1-5) with unlock indicators |
| `WBP_EquipSwapDialog` | Modal — choose between existing equipped item and new loot drop |
| `WBP_UpgradeConfirmDialog` | Modal — confirm unit TIR upgrade with cost and requirements |

**Toolset:** `umg-toolset`, `blueprint-tools`

### Blueprint Classes (Gameplay)

| Name | Type | Purpose |
|------|------|---------|
| `BP_EquipmentManager` | GameMode/GameState extension | Manages equipment inventory, unit loadouts, TIR upgrades, loot distribution |
| `BP_EquipmentItem` | DataAsset | Per-item data: type, rarity, stats, faction compatibility, drop sources |

**Toolset:** `blueprint-tools`, `data-tools`

### Data Tables

| Name | Purpose |
|------|---------|
| `DG_EquipmentItems` | All equipment items across weapons, armor, and utility categories with stats and rarity |
| `DG_UnitUpgradePaths` | Per-unit TIR upgrade chains: requirements, costs, stat improvements per tier |

**Toolset:** `data-tools`, `blueprint-tools`

## MCP Workflow: Step by Step

### Phase 1 — Inspect & Prepare

1. **Inspect the project** for existing equipment/upgrade assets
   - Toolset: `editor-toolset`
2. **Create directory structure**: `/Game/UI/Equipment/` and `/Game/Gameplay/Equipment/`
   - Toolset: `asset-tools` / `editor-toolset`

### Phase 2 — Import Textures (8 textures)

Import all textures with SRGB = true, TF_Bilinear. Rarity borders should be frame-style (transparent center, colored edges) so they can overlay any equipment card background.

**Toolset:** `asset-tools`, `import-export-toolset`

### Phase 3 — Create Equipment Data

#### DG_EquipmentItems
Columns:
- `ItemID` (Name) — unique identifier, e.g., "weapon.ballistic.heavy_rifle.common"
- `ItemType` (Enum: Weapon, Armor, Utility)
- `SubType` (GameplayTag) — e.g., "subtype.ballistic", "subtype.heavy"
- `ItemName` (String)
- `IconTexture` (Texture2D)
- `Rarity` (Enum: Common, Uncommon, Rare, Epic, Legendary)
- `BaseStats` (struct: damage, defense, speed bonus, scan range, etc. — varies by type)
- `FactionCompatibility` (Array of GameplayTag) — which factions can use this item
- `DropSources` (Array of DungeonTypeID) — which dungeons this item can drop from
- `TIRTier` (int) — minimum colony TIR to equip

Populate with equipment items for all 5 weapon types, multiple armor variants, and utility slots. Each rarity tier has progressively better stats:
- Common: base stats, no special effects
- Uncommon: +10% main stat
- Rare: +25% main stat, possible minor effect
- Epic: +50% main stat, one special effect
- Legendary: +100% main stat, two special effects, unique name

#### DG_UnitUpgradePaths
Columns:
- `UnitID` (GameplayTag) — which unit this upgrade path applies to
- `CurrentTier` (int 1-5)
- `NextTier` (int)
- `CostResources` (struct: resource type + amount)
- `StatImprovements` (struct: HP change, damage change, speed change, etc.)
- `PrerequisiteTopic` (Name, tech tree topic ID — empty if no research prerequisite)

**Toolset:** `data-tools`, `blueprint-tools`

### Phase 4 — Create Equipment Item DataAsset

1. **Create `BP_EquipmentItem` DataAsset**:
   - Properties mirror DG_EquipmentItems columns
   - Function: `GetEffectiveStats(equippedUnitRole)` → returns stat values adjusted for unit role compatibility (e.g., a heavy weapon gives less bonus to a Scout unit)
   - Function: `CanEquip(unitID)` → checks faction compatibility and TIR tier requirement

**Toolset:** `data-tools`, `blueprint-tools`

### Phase 5 — Create Equipment Manager

1. **Create `BP_EquipmentManager`**:
   - Variables:
     - `Inventory` (Array of struct: ItemID, Quantity) — all equipment items owned
     - `UnitLoadouts` (struct array: UnitID, WeaponItemID, ArmorItemID, UtilityItemID) — equipped items per unit
     - `UnitTIRTiers` (array: UnitID, CurrentTier) — TIR upgrade level per unit
   - Functions:
     - `AddToInventory(ItemID, quantity)` — adds item to inventory
     - `EquipItem(UnitID, equipmentSlot, ItemID)` — moves from inventory to unit's loadout slot
     - `UnequipItem(UnitID, equipmentSlot)` — returns item from loadout back to inventory
     - `GetUnitEffectiveStats(UnitID)` → struct — sums base unit stats + all equipped item bonuses
     - `UpgradeUnitTIR(UnitID, newTier)` — checks cost and prerequisites, applies stat improvements
     - `CanAffordUpgrade(UnitID, tier)` → bool
     - `ProcessLootDrop(dungeonTypeID, rarityRoll)` → array of ItemIDs — determines loot based on dungeon type and rarity distribution
   - Loot drop rarity distribution (per item slot): Common 60%, Uncommon 25%, Rare 10%, Epic 4%, Legendary 1%

**Toolset:** `blueprint-tools`

### Phase 6 — Create UI Widgets (Bottom-Up)

#### WBP_EquipItemCard
- CanvasPanel child with: ItemIcon (Image), ItemName (TextBlock below), RarityBorder (border image matching item rarity color), StatRows (vertical list of stat name + value pairs), FactionBadge (small icon if faction-specific)
- Variables: `ItemID` (Name), reference to EquipmentManager
- Function `UpdateFromInventory()`:
  - Loads item data from DG_EquipmentItems
  - Sets icon, name, rarity border color
  - Displays effective stats for the currently selected unit role
  - Rarity colors: Common = gray (#9E9E9E), Uncommon = green (#4CAF50), Rare = blue (#2196F3), Epic = purple (#9C27B0), Legendary = orange (#FF9800) with glow effect

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_UnitUpgradeCard
- CanvasPanel child showing a single unit's upgrade status
- Children: UnitPortrait (Image), UnitName (TextBlock), TIRProgressionBar below portrait, CurrentEquipment row (3 small icons for weapon/armor/utility), UpgradeButton (visible if upgrade available)
- Variables: `UnitID` (GameplayTag), reference to EquipmentManager
- Function `UpdateFromLoadout()`:
  - Loads unit name, portrait from roster data
  - Shows current TIR tier with progression bar (filled circles for tiers 1-current, empty for future)
  - Shows equipped equipment icons in a row
  - If upgrade available: UpgradeButton is green and clickable; if not: grayed out with "Requires [prerequisite]" tooltip

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_TIRProgressionBar
- HorizontalBox with 5 circle indicators (one per TIR tier)
- Filled circles = tiers reached, empty circles = future tiers
- Current tier highlighted with accent color (#E8852A) pulse animation
- Hovering over a tier shows: requirements met/not met, cost to reach next tier
- Variable: `CurrentTier` (int), function `UpdateDisplay()` fills appropriate number of circles

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_EquipmentPanel
- Tab content area showing equipment management for the selected unit
- Layout: Unit card on left, equipment slots in center, inventory on right
  - **Left**: UnitUpgradeCard (portrait, name, TIR bar)
  - **Center**: 3 EquipmentSlot displays (Weapon / Armor / Utility) — each shows equipped item with Unequip button
  - **Right**: Scrollable list of EquipItemCards from inventory, filtered by compatibility with this unit's role
- Drag-and-drop: Player can drag an item from the inventory panel onto a slot to equip it
- If drag-and-drop isn't supported: clicking an inventory item highlights it, then clicking a slot equips it

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_EquipmentSwapDialog
- Modal triggered when loot drops that matches an already-equipped slot
- Shows: New item card (left), Currently equipped item card (right), "Keep Both" button (equip new, move old to inventory), "Discard Old" button (equip new, discard old), "Reject New" button (keep old, add new to inventory)
- Legendary drops always force the dialog (can't auto-equip legendary over existing gear)

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_UpgradeConfirmDialog
- Modal confirming TIR upgrade
- Shows: Unit name, current tier → new tier, cost breakdown (resource icons + amounts), Stat improvements preview (before/after comparison), Tech tree prerequisite status (green check or red X)
- Yes → applies upgrade, deducts resources, updates unit stats
- No / Escape → cancels

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_EquipmentRoot
- Root: CanvasPanel
- Layout:
  - Top: Unit selector (dropdown or horizontal list of unit cards — click to switch which unit's equipment is shown)
  - Middle: EquipmentPanel (left: unit card, center: slots, right: inventory)
  - Bottom: TIRProgressionBar showing overall colony TIR and per-unit TIR levels

- Events:
  - `OnInitialize()` — load all unit loadouts from EquipmentManager, populate inventory display
  - `OnUnitSelected(UnitID)` — update EquipmentPanel to show that unit's equipment
  - `OnItemEquipped(UnitID, slot, ItemID)` — refresh affected displays

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 7 — Wire Loot Drop Integration

1. **Post-combat loot**: After Combat Results (S10), the EquipmentManager processes loot drops based on dungeon type and rarity distribution. Each dropped item is added to inventory.
2. **Swap dialog**: If a drop fills an empty slot or matches an existing equipped item's slot type, the EquipSwapDialog appears immediately after combat results.
3. **Permanent upgrades**: TIR upgrades are permanent stat improvements that persist across all future combats and dungeon runs.

**Toolset:** `blueprint-tools`

### Phase 8 — Compile & Verify

1. **Compile every Blueprint** after creation/modification
2. **Test in PIE**:
   - Open equipment screen (accessible from squad selection or post-combat)
   - Select different units → verify loadout display updates
   - Equip an item from inventory → slot shows new item, unit stats update
   - Unequip an item → returns to inventory
   - Swap equipped item with better loot → confirm dialog appears with correct options
   - Upgrade a unit's TIR tier → verify cost deduction and stat improvements apply
   - Verify rarity colors and glow effects on legendary items

## Key Patterns & Gotchas

### Rarity Determines Drop Chance, Not Power Alone
While higher rarity items are strictly better, the drop chance system means Legendary items are genuinely rare (1% per slot). Players will go through many dungeon runs before finding legendaries. The equipment screen should make non-legendary items feel worthwhile — Uncommon and Rare items provide meaningful stat improvements over Common gear.

### Faction Compatibility is Hard-Locked
An item can only be equipped by units from compatible factions. This is checked at equip time — the inventory panel filters items to show only those compatible with the currently selected unit's faction. Incompatible items don't appear in the filtered list at all.

### TIR Upgrades are Per-Unit, Not Global
Each individual unit has its own TIR tier (1-5), separate from the colony's overall TIR. However, the colony's TIR acts as a gate — you can't upgrade a unit beyond the colony's current TIR. So if the colony is at TIR 3, no unit can be upgraded past TIR 3 until the colony reaches TIR 4 via tech tree or building upgrades.

### Stat Bonuses Stack Additively
Equipment stat bonuses add to the unit's base stats. A Scout with base damage 10 who equips a +5 damage weapon has effective damage 15. If they also equip a +2 damage utility item, total is 17. No multiplicative stacking — purely additive between equipment and base stats.

### Loot Drop Timing
Loot drops are determined at the end of combat (in the Combat Results screen flow). Items are added to inventory immediately. The EquipSwapDialog appears as part of that post-combat flow, before returning to the squad selection screen where players can manage their new gear.

## File Structure Summary

```
/Game/UI/Equipment/
├── T_Weapon_Ballistic.uasset (and other weapon type icons)
├── T_Armor_Heavy.uasset (and other armor icons)
├── T_Utility_Scanner.uasset (and other utility icons)
├── T_RarityBorder_Common.uasset (Uncommon, Rare, Epic, Legendary)
├── T_Upgrade_Arrow.uasset
├── WBP_EquipmentRoot.uasset
├── WBP_UnitUpgradeCard.uasset
├── WBP_EquipmentPanel.uasset
├── WBP_EquipItemCard.uasset
├── WBP_TIRProgressionBar.uasset
├── WBP_EquipSwapDialog.uasset
└── WBP_UpgradeConfirmDialog.uasset

/Game/Gameplay/Equipment/
├── BP_EquipmentManager.uasset
└── BP_EquipmentItem.uasset (DataAsset)

/Game/Data/
├── DG_EquipmentItems.uasset (DataTable)
└── DG_UnitUpgradePaths.uasset (DataTable)
```

## Next Steps After This Skill

Unit Upgrade + Equipment connects to:
- **Squad Selection (S07/S09)** — equipped units are selected for dungeon deployment
- **Tactical Combat (S08)** — unit stats from equipment affect combat performance
- **Combat Results (S10)** — loot drops awarded here, equipment swap triggered
- **Tech Tree (S05)** — unlocks new equipment types through research
