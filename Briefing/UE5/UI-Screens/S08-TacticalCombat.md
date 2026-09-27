# S08 Tactical Combat View — Agent Skill

## What to Create

### Assets

| Asset Name | Texture Reference | Path in Project | Source |
|-----------|------------------|-----------------|--------|
| Health Bar Background | `T_Combat_HPBar_BG` | `/Game/UI/Combat/T_Combat_HPBar_BG` | Dark bar, 120x8 PNG |
| Health Bar Fill (friendly) | `T_Combat_HPBar_Friendly` | `/Game/UI/Combat/T_Combat_HPBar_Friendly` | Green fill, matches BG width |
| Health Bar Fill (enemy) | `T_Combat_HPBar_Enemy` | `/Game/UI/Combat/T_Combat_HPBar_Enemy` | Red fill, matches BG width |
| Ability Icon Background | `T_Combat_AbilityBG` | `/Game/UI/Combat/T_Combat_AbilityBG` | Rounded square, 64x64 PNG |
| Formation Icons (4) | `T_Formation_*` | `/Game/UI/Combat/T_Formation_Line`, etc. | Line, Column, Diamond, V-Shape icons |
| Terrain Cover Icon | `T_Combat_Cover` | `/Game/UI/Combat/T_Combat_Cover` | Shield icon, 32x32 PNG |
| Pause Overlay | `T_Combat_PauseOverlay` | `/Game/UI/Combat/T_Combat_PauseOverlay` | Semi-transparent dark overlay |

**Toolset:** `asset-tools`, `import-export-toolset`

### Widget Blueprints (10 total)

| Blueprint Name | Class | Purpose |
|---------------|-------|---------|
| `WBP_CombatRoot` | UserWidget | Root widget for tactical combat screen |
| `WBP_UnitHealthBar` | UserWidget | Health bar overlay above a unit's 3D position (world-to-screen) |
| `WBP_AbilityButton` | UserWidget | Single ability button with icon, cooldown ring, hotkey label |
| `WBP_AbilityHotbar` | UserWidget | Horizontal row of AbilityButton instances for selected unit |
| `WBP_FormationSelector` | UserWidget | Popup showing 4 formation options with visual diagrams |
| `WBP_CombatHUD` | UserWidget | Top bar: timer, squad status summary, pause button |
| `WBP_TerrainInfoPanel` | Modal — shows terrain features, cover bonuses, elevation data |
| `WBP_CombatLog` | UserWidget | Scrolling text log of combat events (damage dealt, abilities used) |
| `WBP_SelectionRing` | UserWidget | Visual ring around selected unit(s) in 3D view |
| `WBP_PauseMenu` | Modal — pause overlay with resume, settings, quit options |

**Toolset:** `umg-toolset`, `blueprint-tools`

### Blueprint Classes (Gameplay)

| Name | Type | Purpose |
|------|------|---------|
| `BP_CombatManager` | GameMode/GameState extension | Central combat state: active units, turn timer, phase tracking, pause/resume |
| `BP_UnitActor` | Actor | Represents a single unit in combat: health, abilities, formation position, cover status |
| `BP_FormationController` | ActorComponent | Manages unit positioning within formations (Line, Column, Diamond, V-Shape) |

**Toolset:** `blueprint-tools`, `actor-tools`

### Data Tables

| Name | Purpose |
|------|---------|
| `DG_UnitAbilities` | Per-unit ability data: name, icon, cooldown, damage/effect, range, formation bonus |
| `DG_TerrainFeatures` | Terrain tile data: cover type (none/light/heavy), elevation bonus, movement cost |

**Toolset:** `data-tools`, `blueprint-tools`

## MCP Workflow: Step by Step

### Phase 1 — Inspect & Prepare

1. **Inspect the project** for existing combat-related assets
   - Toolset: `editor-toolset`
2. **Create directory structure**: `/Game/UI/Combat/` and `/Game/Gameplay/Combat/`
   - Toolset: `asset-tools` / `editor-toolset`

### Phase 2 — Import Textures (7 textures)

Import all textures with SRGB = true, TF_Bilinear. Health bar fills should match the background width exactly for seamless stretching. Ability icon backgrounds should be rounded squares.

**Toolset:** `asset-tools`, `import-export-toolset`

### Phase 3 — Create Data Tables

#### DG_UnitAbilities
Columns: `UnitID` (GameplayTag), `AbilitySlot` (int 0-7, hotbar position), `AbilityName` (String), `IconTexture` (Texture2D), `CooldownSeconds` (float), `DamageType` (Enum: Physical, Energy, Elemental), `DamageAmount` (float), `Range` (float, Unreal units), `EffectType` (Enum: Attack, Buff, Debuff, Heal, Utility), `EffectValue` (float), `FormationBonus` (String, e.g., "Line +10% damage")

Populate with abilities for all unit types across Scout, Heavy Fire, Tank, Support, Specialist roles.

#### DG_TerrainFeatures
Columns: `TerrainType` (GameplayTag), `CoverLevel` (Enum: None, Light, Heavy), `DefenseBonus` (float, 0%/20%/40%), `ElevationBonus` (float, attack bonus for high ground), `MovementCost` (float, multiplier for movement through this terrain)

**Toolset:** `data-tools`, `blueprint-tools`

### Phase 4 — Create Unit Actor

1. **Create `BP_UnitActor`**:
   - Components: StaticMesh (unit visual), SphereComponent (collision, radius based on unit size), SceneComponent (root)
   - Variables:
     - `UnitID` (GameplayTag)
     - `CurrentHP`, `MaxHP` (int)
     - `IsAlly` (bool) — true for player units, false for enemies
     - `CurrentFormationPosition` (int) — position within formation (1-4 for 4-unit formation)
     - `bHasCover` (bool), `CoverLevel` (Enum)
     - `AbilityCooldowns` (array of float, one per ability slot)
   - Events:
     - `TakeDamage(amount, damageType)` — reduces HP, plays hit VFX, updates health bar
     - `UseAbility(abilitySlot)` — checks cooldown, applies effect on target, starts cooldown timer
     - `ApplyCoverBonus()` → returns defense bonus from DG_TerrainFeatures based on current terrain
     - `IsAlive()` → returns CurrentHP > 0
   - **Health bar rendering**: Use a UMG widget component attached to the unit's SceneComponent. This makes the health bar follow the unit in 3D space (world-to-screen projection).

**Toolset:** `blueprint-tools`, `actor-tools`

### Phase 5 — Create Formation Controller

1. **Create `BP_FormationController` ActorComponent**:
   - Variables: `FormationType` (Enum: Line, Column, Diamond, V-Shape), `LeaderUnit` (reference to BP_UnitActor)
   - Functions:
     - `CalculatePositions(leaderLocation, facingDirection)` → array of 4 relative offset vectors
       - **Line**: units side by side perpendicular to facing direction
       - **Column**: units behind each other along facing direction
       - **Diamond**: leader at front, two flanking, one rear
       - **V-Shape**: leader + two forming a V, one rear center
     - `ApplyPositions()` — moves all formation units to calculated positions
     - `GetFormationBonus(abilityID)` → float bonus from DG_UnitAbilities.FormationBonus field

2. **Formation movement**: When the player issues a move command, the FormationController calculates new positions based on the destination point and current formation shape. Units move simultaneously toward their target offsets.

**Toolset:** `blueprint-tools`

### Phase 6 — Create Combat Manager

1. **Create `BP_CombatManager`**:
   - Variables:
     - `AllyUnits` (Array of BP_UnitActor), `EnemyUnits` (Array of BP_UnitActor)
     - `SelectedUnit` (reference to BP_UnitActor)
     - `CombatTimer` (float, seconds remaining in combat encounter)
     - `bIsPaused` (bool)
     - `CurrentPhase` (Enum: Deployment, PlayerTurn, EnemyTurn, CombatEnd)
   - Functions:
     - `BeginCombat(allyUnits, enemyUnits, terrainData)` — spawns all actors, initializes UI
     - `SelectUnit(unitActor)` — sets SelectedUnit, updates AbilityHotbar and SelectionRing
     - `IssueMoveCommand(targetLocation)` — sends selected unit (or formation) to target
     - `IssueAttackCommand(targetUnit)` — resolves attack: damage = base × formation bonus × terrain modifiers × random variance (±10%)
     - `TogglePause()` — sets bIsPaused, shows/hides PauseMenu overlay
     - `GetCoverDefenseBonus(unitActor)` → float from DG_TerrainFeatures
     - `EndCombat(victory bool)` — triggers Combat Results screen (S10)
   - Tick event: when not paused, process ability cooldowns, AI enemy actions (if enemy turn phase), check for combat end conditions

2. **Pause behavior**: When paused, all timers stop (combat timer, ability cooldowns, AI decision-making). The game is real-time with pause — not turn-based. Players issue commands freely during active time.

**Toolset:** `blueprint-tools`

### Phase 7 — Create UI Widgets (Bottom-Up)

#### WBP_UnitHealthBar
- HorizontalBox: HPBarBackground (Image, stretched), HPBarFill (Image, width = CurrentHP/MaxHP * 100%), HPText (TextBlock, "120/150")
- Variables: `CurrentHP`, `MaxHP`, `IsAlly` (bool)
- Function `UpdateDisplay()` — sets fill width and text color (green for ally, red for enemy)
- Color coding: green (>60% HP), yellow (30-60%), red (<30%)
- **World-to-screen**: This widget is placed as a WidgetComponent on BP_UnitActor's SceneComponent so it follows the unit in 3D space.

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_AbilityButton
- CanvasPanel child with: AbilityBG (Image, rounded square), AbilityIcon (Image centered), CooldownOverlay (Image, dark circle that grows from center during cooldown), HotkeyLabel (TextBlock, bottom-right corner, e.g., "1", "2", "3")
- Variables: `AbilitySlot` (int), `IsReady` (bool), `CooldownRemaining` (float)
- Function `UpdateFromAbility(abilityData)` — sets icon, hotkey label from DG_UnitAbilities
- Function `SetCooldown(float remaining, float total)` — animates CooldownOverlay fill percentage
- Event: `OnClicked()` — fires when ability is ready and clicked → calls CombatManager.UseAbility() on selected unit

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_AbilityHotbar
- HorizontalBox with 8 AbilityButton instances (slots 0-7)
- Variable: reference to CombatManager, SelectedUnit reference
- Function `UpdateForUnit(unitActor)` — loads abilities from DG_UnitAbilities for this unit's type, populates all 8 slots
- Function `UpdateCooldowns()` — called every tick to update cooldown overlays on active buttons

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_FormationSelector
- CanvasPanel child (popup) with: Title TextBlock ("Formation"), 4 FormationButton instances arranged in a 2x2 grid, each showing formation icon + name + brief description
- Each button calls CombatManager.SetFormation(formType) when clicked
- Closes on selection or Escape key

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_CombatHUD
- HorizontalBox at top of screen
- Children: CombatTimer (TextBlock, countdown), SquadStatus (small row of unit portrait icons with mini health bars), PauseButton (Image button)
- Variable: reference to CombatManager, function `UpdateFromCombat()` reads current state

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_CombatLog
- VerticalBox at bottom-right corner (or side panel)
- Scrolling list of TextBlock entries for combat events
- Entries: "[12:34] Scout fired at Enemy Drone — 25 damage"
- Color coding: green for ally actions, red for enemy actions, yellow for environmental effects
- Auto-scrolls to newest entry; expandable to full history

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_SelectionRing
- This is a visual effect widget (not interactive) that appears as a glowing ring around the selected unit in 3D space
- Implemented as a WidgetComponent on the selected unit's SceneComponent
- Visual: circular ring with accent color (#E8852A), pulsing animation, semi-transparent fill

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_TerrainInfoPanel
- Modal overlay showing current terrain features under cursor/selection
- Displays: Terrain type name, Cover level (None/Light/Heavy with icon), Defense bonus percentage, Elevation bonus, Movement cost multiplier
- Variable: reference to CombatManager, function `UpdateAtLocation(location)` queries DG_TerrainFeatures for the terrain at that world location

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_PauseMenu
- Full-screen dark overlay (T_Combat_PauseOverlay) with centered panel
- Children: "PAUSED" TextBlock (large), Resume button, Settings button (opens M03 Settings Menu), Quit to Main Menu button
- Resume button or Escape key → calls CombatManager.TogglePause()

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_CombatRoot
- Root: CanvasPanel
- Children (z-order back to front):
  1. CombatHUD at top
  2. AbilityHotbar at bottom-center
  3. FormationSelector popup (hidden by default, shown on formation button click)
  4. TerrainInfoPanel (hidden by default, shown when inspecting terrain)
  5. CombatLog at bottom-right (collapsible)
  6. PauseMenu overlay (hidden by default, shown when paused)

- Events:
  - `OnInitialize()` — bind to CombatManager events, create health bar widgets for all units
  - `OnUnitSelected(unitActor)` — update AbilityHotbar, show SelectionRing
  - `OnCombatEnded(victory)` — transition to Combat Results screen (S10)

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 8 — Wire Input & Camera

1. **Input mapping**:
   - Left Click on unit → Select that unit
   - Right Click on ground → Move selected unit (or formation) to location
   - Number keys 1-7 → Use ability in that hotbar slot
   - F key → Open FormationSelector popup
   - P / Escape → Toggle pause
   - Tab → Cycle through ally units (quick selection)

2. **Camera**: Tactical combat uses a top-down/isometric camera with pan at screen edges and zoom via mouse wheel. Camera controls are standard RTS-style: WASD or edge-pan to move, scroll wheel to zoom.

3. **Click detection**: Use `LineTraceByChannel` from camera through mouse position. First hit on a BP_UnitActor → select that unit. First hit on ground → issue move command to selected unit/formation.

**Toolset:** `blueprint-tools`, `actor-tools`

### Phase 9 — Compile & Verify

1. **Compile every Blueprint** after creation/modification
2. **Test in PIE**:
   - Enter combat from Dungeon Entry screen (S07)
   - Select units, verify health bars appear above units in 3D space
   - Use abilities via hotbar or number keys → verify damage calculation and cooldowns
   - Change formations → verify unit positions update correctly
   - Pause/unpause → verify all timers stop and resume
   - Test cover bonuses: place unit behind terrain feature, verify defense bonus applies to incoming damage
   - Complete combat → transition to Combat Results screen (S10)
3. **Verify real-time with pause**: Confirm that time stops when paused and resumes correctly — not turn-based

## Key Patterns & Gotchas

### Real-Time With Pause, Not Turn-Based
Combat is real-time with a pause button. Units move and abilities fire continuously. When paused, everything freezes — movement, cooldowns, AI decisions. This is fundamentally different from turn-based tactics. Players issue commands during active time, then pause to plan the next batch.

### World-to-Screen Health Bars
Health bars use UMG WidgetComponents attached to unit SceneComponents. UE5 automatically handles world-to-screen projection. The widget always faces the camera (billboard) and scales with distance. Set the widget component's `WorldSize` property based on unit size for consistent screen appearance at all zoom levels.

### Formation Bonus Stacking
Formation bonuses are multiplicative with other modifiers: `FinalDamage = BaseDamage × FormationBonus × CoverDefense(against attacker) × RandomVariance`. A Line formation giving +10% damage means multiply by 1.10, not add 10 to a percentage pool.

### Terrain Query at Runtime
Terrain features are queried via world location, not unit reference. When a unit moves to a new tile, the CombatManager checks DG_TerrainFeatures for that terrain type and updates the unit's cover/elevation status. This means units can move in and out of cover dynamically during combat.

### Ability Cooldowns Are Per-Unit
Each unit has its own independent cooldown array. Using an ability on Unit A does not affect Unit B's cooldowns. The AbilityHotbar only shows abilities for the currently selected unit. If no unit is selected, the hotbar is hidden or shows a "select a unit" prompt.

### Combat End Conditions
Combat ends when: all enemy units are defeated (victory), or all ally units are defeated (defeat). There's also a timeout if the combat timer reaches zero without either side being eliminated — result depends on mission objectives (e.g., "survive for 10 minutes" vs "eliminate all enemies").

## File Structure Summary

```
/Game/UI/Combat/
├── T_Combat_HPBar_BG.uasset
├── T_Combat_HPBar_Friendly.uasset
├── T_Combat_HPBar_Enemy.uasset
├── T_Combat_AbilityBG.uasset
├── T_Formation_Line.uasset (and 3 other formation icons)
├── T_Combat_Cover.uasset
├── T_Combat_PauseOverlay.uasset
├── WBP_CombatRoot.uasset
├── WBP_UnitHealthBar.uasset
├── WBP_AbilityButton.uasset
├── WBP_AbilityHotbar.uasset
├── WBP_FormationSelector.uasset
├── WBP_CombatHUD.uasset
├── WBP_TerrainInfoPanel.uasset
├── WBP_CombatLog.uasset
├── WBP_SelectionRing.uasset
└── WBP_PauseMenu.uasset

/Game/Gameplay/Combat/
├── BP_CombatManager.uasset
├── BP_UnitActor.uasset
└── BP_FormationController.uasset

/Game/Data/
├── DG_UnitAbilities.uasset (DataTable)
└── DG_TerrainFeatures.uasset (DataTable)
```

## Next Steps After This Skill

Tactical Combat connects to:
- **Squad Selection (S07/S09)** — choose units before entering combat
- **Combat Results (S10)** — loot distribution and progression after combat
- **Unit Upgrade System** — equipment and TIR upgrades between combats
- **Dungeon Exploration** — combat occurs within dungeon rooms
