# S07/S09 Squad Selection + Dungeon Entry — Agent Skill

## What to Create

### Assets

| Asset Name | Texture Reference | Path in Project | Source |
|-----------|------------------|-----------------|--------|
| Unit Portrait Background | `T_Squad_PortraitBG` | `/Game/UI/Squad/T_Squad_PortraitBG` | Rounded rectangle, 128x128 PNG |
| Role Icons (5) | `T_Role_*` | `/Game/UI/Squad/T_Role_Scout`, etc. | Scout, Heavy Fire, Tank, Support, Specialist icons |
| Equipment Slot Icon | `T_EquipSlot_*` | `/Game/UI/Squad/T_EquipSlot_Weapon`, etc. | Weapon, Armor, Utility slot icons |
| Dungeon Type Icon (7) | `T_DungeonType_*` | `/Game/UI/Squad/T_DungeonType_Cave`, etc. | Icons for each dungeon type |
| Rarity Common/Uncommon/Rare/Epic/Legendary | `T_Rarity_*` | `/Game/UI/Squad/T_Rarity_Common`, etc. | Color-coded borders: gray, green, blue, purple, orange |

**Toolset:** `asset-tools`, `import-export-toolset`

### Widget Blueprints (9 total)

| Blueprint Name | Class | Purpose |
|---------------|-------|---------|
| `WBP_SquadRoot` | UserWidget | Root widget — squad selection and dungeon entry screen |
| `WBP_UnitCard` | UserWidget | Card showing a single unit: portrait, name, role, level, equipment slots |
| `WBP_RoleFilterBar` | UserWidget | Horizontal bar with role filter buttons (all/scout/heavy/tank/support/specialist) |
| `WBP_EquipmentSlot` | UserWidget | Single equipment slot on a unit card: icon, equipped item name, rarity border |
| `WBP_DungeonPreviewPanel` | UserWidget | Panel showing dungeon type, difficulty, size, expected rewards |
| `WBP_RewardPreviewModal` | Modal — shows loot table preview for selected dungeon |
| `WBP_SquadSummaryBar` | UserWidget | Bottom bar: squad count (X/16), role distribution summary, deploy button |
| `WBP_DeployConfirmationDialog` | Modal — confirmation before entering dungeon |
| `WBP_CoOpPickIndicator` | UserWidget — shows which units are "claimed" in co-op collaborative pick |

**Toolset:** `umg-toolset`, `blueprint-tools`

### Blueprint Classes (Gameplay)

| Name | Type | Purpose |
|------|------|---------|
| `BP_SquadManager` | GameMode/GameState extension | Manages squad composition: selected units, role distribution, equipment state |
| `BP_UnitRosterEntry` | DataAsset | Per-unit roster data: name, role, base stats, available equipment, unlock conditions |

**Toolset:** `blueprint-tools`, `data-tools`

### Data Tables

| Name | Purpose |
|------|---------|
| `DG_UnitRoster` | All available units with roles, stats, and unlock requirements |
| `DG_DungeonTypes` | 7 dungeon types with size ranges, enemy compositions, loot tables, squad recommendations |

**Toolset:** `data-tools`, `blueprint-tools`

## MCP Workflow: Step by Step

### Phase 1 — Inspect & Prepare

1. **Inspect the project** for existing squad/roster assets
   - Toolset: `editor-toolset`
2. **Create directory structure**: `/Game/UI/Squad/` and `/Game/Gameplay/Squad/`
   - Toolset: `asset-tools` / `editor-toolset`

### Phase 2 — Import Textures (8 textures)

Import all textures with SRGB = true, TF_Bilinear. Unit portrait backgrounds should be square. Role icons should be simple silhouettes at 64x64 minimum for clarity at small sizes.

**Toolset:** `asset-tools`, `import-export-toolset`

### Phase 3 — Create Data Tables

#### DG_UnitRoster
Columns: `UnitID` (Name), `UnitName` (String), `Role` (GameplayTag: role.scout, role.heavy, etc.), `BaseHP` (int), `BaseDamage` (float), `Speed` (float), `EquipmentSlots` (Array of EquipmentType enum), `UnlockTIRTier` (int), `FactionAffinity` (GameplayTag, which faction this unit type is native to), `Description` (String)

Populate with all 16+ available unit types across the 5 roles. Each role has multiple unit variants with different stat profiles.

#### DG_DungeonTypes
Columns: `DungeonTypeID` (Name), `DungeonTypeName` (String), `IconTexture` (Texture2D), `SizeCategory` (Enum: Tiny, Small, Medium, Large, Fortress), `RecommendedSquadSize` (int), `MinTIRTier` (int), `MaxTIRTier` (int), `EnemyComposition` (String description), `LootTableRef` (Name, references loot table data), `BossType` (GameplayTag), `DurationEstimate` (float, game minutes)

Populate with all 7 dungeon types from the design doc.

**Toolset:** `data-tools`, `blueprint-tools`

### Phase 4 — Create Unit Roster DataAsset

1. **Create `BP_UnitRosterEntry` DataAsset**:
   - Properties mirror DG_UnitRoster columns
   - Function: `GetAvailableEquipment()` → array of equipment items this unit can equip (from faction + role compatibility)
   - Function: `CanDeploy(currentTIR)` → checks if current colony TIR meets UnlockTIRTier

**Toolset:** `data-tools`, `blueprint-tools`

### Phase 5 — Create Squad Manager

1. **Create `BP_SquadManager`**:
   - Variables:
     - `RosterUnits` (Array of BP_UnitRosterEntry references) — all unlocked units
     - `SelectedSquad` (Array of UnitID, max 16)
     - `RoleCounts` (struct: scoutCount, heavyFireCount, tankCount, supportCount, specialistCount)
   - Functions:
     - `UnlockUnit(UnitID)` — adds to RosterUnits when TIR requirement met
     - `AddToSquad(UnitID)` — validates squad size < 16 and role balance rules
     - `RemoveFromSquad(UnitID)` — removes from SelectedSquad
     - `ValidateRoleBalance()` → returns true if squad has at least one unit from Tank and Support roles (mandatory for dungeon survival)
     - `GetRecommendedDungeons(DungeonTypeID)` → filters dungeons by recommended squad size and TIR range
     - `EquipUnit(UnitID, equipmentSlot, equipmentItem)` — assigns equipment to a unit slot
     - `GetSquadSummary()` → returns role distribution counts for display

2. **Role balance rules**: A valid dungeon squad must include at least 1 Tank (for damage absorption) and 1 Support (for healing/buffs). No more than 4 units of the same role recommended (soft cap, not hard block).

**Toolset:** `blueprint-tools`

### Phase 6 — Create UI Widgets (Bottom-Up)

#### WBP_EquipmentSlot
- CanvasPanel child with: SlotIcon (Image), EquippedItemIcon (smaller Image overlay on top-right), EquippedItemName (TextBlock below slot), RarityBorder (border color based on item rarity tier)
- Variables: `EquipmentSlotType` (Enum: Weapon, Armor, Utility), `EquippedItemID` (Name, empty if none)
- Function `UpdateFromEquipment(itemData)` — sets icons, name, and rarity border color
  - Common = gray (#9E9E9E), Uncommon = green (#4CAF50), Rare = blue (#2196F3), Epic = purple (#9C27B0), Legendary = orange (#FF9800)
- Event: `OnClicked()` — opens equipment swap dialog for this slot

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_UnitCard
- CanvasPanel child with: PortraitBackground (Image, 128x128), UnitPortrait (Image centered on portrait bg), UnitName (TextBlock below portrait), RoleIcon (small Image in top-right corner), Level/TIR badge (TextBlock), EquipmentSlots (3 EquipmentSlot instances arranged horizontally at bottom)
- Variables: `UnitID` (Name), reference to SquadManager
- Function `UpdateFromRoster(rosterEntry)` — loads name, role, portrait from DG_UnitRoster
- Function `UpdateEquipment()` — shows currently equipped items in each slot
- Event: `OnClicked()` — adds/removes unit from SelectedSquad (toggle behavior)
  - If squad full (16/16), show toast notification "Squad at capacity"
- Visual state: selected units get a green border highlight (#00FF88)

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_RoleFilterBar
- HorizontalBox with filter buttons: All, Scout, Heavy Fire, Tank, Support, Specialist
- Each button shows RoleIcon + count badge (e.g., "Scout 4" means 4 scouts available)
- Active filter highlighted with accent color (#E8852A)
- Event: `OnFilterChanged(role)` — filters UnitCard visibility in the parent squad list

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_DungeonPreviewPanel
- CanvasPanel child showing selected dungeon's details
- Children: DungeonIcon (large Image), DungeonName (TextBlock, large), Size badge (e.g., "Large"), TIR requirement badge, Recommended squad size, Enemy composition summary text, Duration estimate
- Bottom section: Reward preview button → opens RewardPreviewModal
- Variable: `DungeonTypeID` (Name), function `UpdateFromTable(dungeonData)` loads all display data

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_RewardPreviewModal
- Modal overlay with dungeon name at top, loot table below as a list
- Each loot entry: item icon, item name, rarity border, drop chance percentage
- Grouped by rarity tier section headers (Common 60%, Uncommon 25%, Rare 10%, Epic 4%, Legendary 1%)
- Note: "Actual rewards determined after combat completion" disclaimer at bottom

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_SquadSummaryBar
- HorizontalBox at bottom of screen
- Children: SquadCount TextBlock ("8/16"), RoleDistribution (small row of role icons with counts), DeployButton (large button, enabled only when squad is valid per ValidateRoleBalance())
- DeployButton color: green when valid, gray/disabled when invalid
- Event: `OnDeployClicked()` — opens DeployConfirmationDialog

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_DeployConfirmationDialog
- Modal overlay with: "Deploy to [DungeonName]?" TextBlock, squad composition summary (list of selected units with roles), Warning text if role balance is marginal (e.g., "Only 1 Tank — consider adding a second"), Yes/No buttons
- Yes → closes dungeon entry screen, loads tactical combat scene (S08)
- No / Escape → dismisses dialog

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_CoOpPickIndicator
- Small badge overlay on UnitCard showing co-op pick status
- Shows player number/color if claimed by a specific player in collaborative mode
- Visual: colored circle with player number (1 = blue, 2 = red, etc.)

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_SquadRoot
- Root: CanvasPanel
- Layout split into two vertical panels:
  - **Left panel (60% width)**: RoleFilterBar at top, scrollable list of UnitCard instances below
  - **Right panel (40% width)**: DungeonPreviewPanel at top, SquadSummaryBar at bottom
- Children (z-order):
  1. Left panel: RoleFilterBar + UnitCard scroll area
  2. Right panel: DungeonPreviewPanel + SquadSummaryBar
  3. RewardPreviewModal (hidden by default)
  4. DeployConfirmationDialog (hidden by default)

- Events:
  - `OnInitialize()` — load roster from SquadManager, create all UnitCards, populate role filter counts
  - `OnUnitToggled(UnitID)` — add/remove from SelectedSquad, update SquadSummaryBar
  - `OnDungeonSelected(DungeonTypeID)` — update DungeonPreviewPanel content

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 7 — Wire Dungeon Entry Flow

1. **Selection flow**: Player browses unit roster → filters by role → clicks units to add/remove from squad → reviews squad summary → selects dungeon type → previews rewards → confirms deployment
2. **Transition to combat**: On deploy confirmation, hide SquadRoot widget, load the tactical combat level (S08). Pass squad composition data to CombatManager so it knows which ally units to spawn.
3. **Post-combat return**: After combat results (S10), return to this screen with updated roster (new units unlocked from dungeon rewards).

**Toolset:** `blueprint-tools`

### Phase 8 — Compile & Verify

1. **Compile every Blueprint** after creation/modification
2. **Test in PIE**:
   - Open squad selection screen
   - Filter by role → verify unit cards show only matching units
   - Click units to add/remove from squad → verify count updates and selected border appears
   - Try deploying with invalid role balance (no Tank) → Deploy button stays disabled
   - Select a dungeon → preview panel shows correct type, size, TIR requirements
   - Click reward preview → modal shows loot table with drop chances
   - Confirm deployment → transition to combat scene
3. **Verify co-op indicator** (if co-op mode is implemented): Multiple players can claim different units simultaneously

## Key Patterns & Gotchas

### Mandatory Role Balance
Every dungeon squad MUST have at least 1 Tank and 1 Support. This is a hard validation — the Deploy button stays disabled until both roles are represented. The rationale: without a Tank, the squad takes too much damage; without Support, there's no healing/buffs for sustained encounters.

### Squad Size Scaling with Dungeon Size
Tiny dungeons work with 2-4 units, Fortresses need 12-16. The dungeon recommendation in DG_DungeonTypes tells players the optimal size, but smaller squads can attempt larger dungeons at a difficulty penalty (enemies get +20% stats per missing recommended unit).

### Equipment Persistence Across Dungeons
Equipment equipped on units persists between dungeon runs. When a player returns from combat with new loot, they come back to this screen where they can equip the new items before the next deployment.

### Unlock Progression
Units unlock as the colony's TIR tier increases through tech tree research. A unit with UnlockTIRTier = 3 won't appear in the roster until the player has reached TIR 3 (via building upgrades or specific tech tree topics). Locked units show a grayed-out card with "Requires TIR 3" text.

### Co-op Collaborative Pick
In co-op mode, each player gets a color indicator. When Player 1 clicks a unit, it shows their color badge — that unit is now claimed and invisible to other players. This prevents both players from selecting the same unit for their respective squads.

## File Structure Summary

```
/Game/UI/Squad/
├── T_Squad_PortraitBG.uasset
├── T_Role_Scout.uasset (and 4 other role icons)
├── T_EquipSlot_Weapon.uasset (Armor, Utility)
├── T_DungeonType_Cave.uasset (and 6 other dungeon type icons)
├── T_Rarity_Common.uasset (Uncommon, Rare, Epic, Legendary)
├── WBP_SquadRoot.uasset
├── WBP_UnitCard.uasset
├── WBP_RoleFilterBar.uasset
├── WBP_EquipmentSlot.uasset
├── WBP_DungeonPreviewPanel.uasset
├── WBP_RewardPreviewModal.uasset
├── WBP_SquadSummaryBar.uasset
├── WBP_DeployConfirmationDialog.uasset
└── WBP_CoOpPickIndicator.uasset

/Game/Gameplay/Squad/
├── BP_SquadManager.uasset
└── BP_UnitRosterEntry.uasset (DataAsset)

/Game/Data/
├── DG_UnitRoster.uasset (DataTable)
└── DG_DungeonTypes.uasset (DataTable)
```

## Next Steps After This Skill

Squad Selection connects to:
- **Tactical Combat (S08)** — squad composition determines ally units in combat
- **Combat Results (S10)** — loot earned here is equipped back on this screen
- **Unit Upgrade System** — equipment and TIR upgrades managed between deployments
- **Dungeon types** — each dungeon type has unique enemy compositions and rewards
