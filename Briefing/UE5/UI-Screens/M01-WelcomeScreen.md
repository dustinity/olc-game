# M01 Welcome Screen + M03 Settings Menu — Agent Skill

## What to Create

### Assets (12 textures → import as PNG)

| Asset Name | Texture Reference | Path in Project | Source File |
|-----------|------------------|-----------------|-------------|
| Welcome Background | `T_WS_Background` | `/Game/UI/WelcomeScreen/T_WS_Background` | `01-Game-Welcome-Screen.png` (crop) |
| Button Normal | `T_WS_Button_Normal` | `/Game/UI/WelcomeScreen/T_WS_Button_Normal` | PNG asset |
| Button Active | `T_WS_Button_Active` | `/Game/UI/WelcomeScreen/T_WS_Button_Active` | PNG asset |
| Button Hovered | `T_WS_Button_Hovered` | `/Game/UI/WelcomeScreen/T_WS_Button_Hovered` | PNG asset |
| Button Pressed | `T_WS_Button_Pressed` | `/Game/UI/WelcomeScreen/T_WS_Button_Pressed` | PNG asset |
| Button Disabled | `T_WS_Button_Disabled` | `/Game/UI/WelcomeScreen/T_WS_Button_Disabled` | PNG asset |
| Frame Border | `T_WS_Frame` | `/Game/UI/WelcomeScreen/T_WS_Frame` | PNG asset |
| Settings Background | `T_WS_Settings_BG` | `/Game/UI/WelcomeScreen/T_WS_Settings_BG` | PNG asset |
| Tab Normal | `T_WS_Tab_Normal` | `/Game/UI/WelcomeScreen/T_WS_Tab_Normal` | PNG asset |
| Tab Active | `T_WS_Tab_Active` | `/Game/UI/WelcomeScreen/T_WS_Tab_Active` | PNG asset |
| Slider Handle | `T_WS_Slider_Handle` | `/Game/UI/WelcomeScreen/T_WS_Slider_Handle` | PNG asset |
| Checkbox | `T_WS_Checkbox` | `/Game/UI/WelcomeScreen/T_WS_Checkbox` | PNG asset |

**Toolset:** `texture-tools`, `asset-tools`

### Widget Blueprints (9 total)

| Blueprint Name | Class | Purpose | Parent of |
|---------------|-------|---------|-----------|
| `WBP_WS_MenuButton` | UserWidget | Reusable menu button with 4 states | — |
| `WBP_WS_Frame` | UserWidget | Decorative frame border for welcome screen | — |
| `WBP_WS_Root` | UserWidget | Main root widget — entry point of the game | MenuButton, Frame, TitleBlock, MainMenu, ControlPanel, SettingsOverlay |
| `WBP_SettingsMenu` | UserWidget | Full settings overlay panel | TabButton, SettingsRow widgets |
| `WBP_SettingsTabButton` | UserWidget | Tab button within settings (General, Audio, Video, Controls) | — |
| `WBP_SettingsRow_Slider` | UserWidget | Slider setting row (volume bars, brightness, etc.) | — |
| `WBP_SettingsRow_Combo` | UserWidget | Dropdown/combo setting row (resolution, quality preset) | — |
| `WBP_SettingsRow_Checkbox` | UserWidget | Toggle setting row (fullscreen, VSync, reduce motion) | — |

**Toolset:** `umg-toolset`, `blueprint-tools`

### Subsystems & Data

| Name | Type | Purpose |
|------|------|---------|
| `WS_SettingsSubsystem` | GameInstance Subsystem | Persists settings between sessions via SaveGame |
| `WS_UserSettings` | UGameUserSettings | Engine-level display/audio settings (resolution, fullscreen mode) |

**Toolset:** `blueprint-tools`, `config-settings-toolset`

## MCP Workflow: Step by Step

### Phase 1 — Inspect & Prepare

1. **Inspect the project** to understand existing structure
   - Toolsets: `asset-tools`, `config-settings-toolset`
   - Check current `/Game/UI/WelcomeScreen/` directory exists (create if needed)
   - Verify no conflicting assets or widgets already exist at those paths
   - Query current StartupWidget setting via `get_project_setting("Maps & Modes", "Startup Widget")`

2. **Create directory structure** under `/Game/UI/WelcomeScreen/`
   - Toolset: `asset-tools` (create folder)

### Phase 2 — Import Textures (12 PNGs)

For each texture in the asset table above:

1. Read the source image file from disk
2. Call `import_file_with_settings` from `texture-tools` to create a Texture2D at the target path with:
   - `srgb = true` — all UI textures are sRGB (not normal/roughness maps)
   - `compression = "TC_Diffuse"` — standard sRGB compression for color/UI textures
   - `mipmaps = false` — disable mipmaps for pixel-perfect crispness on UI elements
   - `filter = "TF_Bilinear"` — smooth filtering for high-res UI textures
   - `max_size = original resolution` (or downsample if > 2048)
3. Verify import succeeded before proceeding to the next texture

**Toolset:** `texture-tools`

### Phase 3 — Create Reusable Widgets (Bottom-Up)

Create these first, before any container widgets that reference them:

#### WBP_WS_MenuButton

- **Root component:** CanvasPanel
- **Child components:**
  - BackgroundImage (Image) — binds to `T_WS_Button_Normal` by default
  - TextBlock (label) — centered, font size ~24, color #E8852A accent
- **Variables:**
  - `ButtonText` (String) — the label text
  - `ButtonState` (Enum: Normal, Hovered, Pressed, Disabled)
  - `OnClicked` (Delegate/Event) — fired when button is clicked
  - `ActiveImage` (Texture2D reference) — for hovered state image
- **Events:**
  - `OnHovered` → set ButtonState = Hovered, swap BackgroundImage to `T_WS_Button_Hovered`, text color to #FFFFFF
  - `OnPressed` → set ButtonState = Pressed, scale to 0.98, duration 0.08s ease-out
  - `OnReleased` / `OnUnhovered` → restore from Hovered/Normal state
  - `SetDisabled` (Function) → set opacity to 0.45, ButtonState = Disabled
- **Transitions:** Use Storyboard or simple SetVisibility with delay for smooth state changes (~0.15s hover)

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_WS_Frame

- Decorative border widget using `T_WS_Frame` texture as a stretched Image
- Root: CanvasPanel, single child Image filling the panel with margin insets for corner/edge stretching
- No interactive events — purely visual

**Toolset:** `umg-toolset`

#### Settings Row Widgets (Slider, Combo, Checkbox)

Each follows the same pattern:
- **Root:** HorizontalBox or CanvasPanel
- **Left side:** TextBlock for setting label
- **Right side:** The control widget (Slider / ComboBox / CheckBox)
- **Variable:** `SettingValue` — bound to the settings subsystem
- **Event:** `OnValueChanged` — fires when user changes the setting, writes to SaveGame

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 4 — Create WBP_SettingsMenu

- **Root component:** CanvasPanel
- **Background:** Image using `T_WS_Settings_BG` (dark overlay)
- **Content container:** VerticalBox or CanvasPanel child containing:
  - Tab bar at top with `WBP_SettingsTabButton` instances for each tab: General, Audio, Video, Controls
  - Tab content area — use a WidgetSwitcher pattern: one panel per tab, only the active tab is visible
    - **General tab:** Checkbox rows (Fullscreen, VSync, Reduce Motion)
    - **Audio tab:** Slider rows (Master Volume, Music Volume, SFX Volume)
    - **Video tab:** Combo row (Resolution), Slider (Brightness), Combo (Quality Preset)
    - **Controls tab:** Slider rows (Mouse Sensitivity), Combo rows (keybind display — future expansion)
- **Close button** in top-right corner → triggers fade-out animation, returns to main menu

**Tab switching logic:**
- When a `WBP_SettingsTabButton` is clicked, it fires an event on the parent SettingsMenu
- The SettingsMenu hides all tab content panels and shows only the selected one
- Active tab button gets `T_WS_Tab_Active`, inactive get `T_WS_Tab_Normal`

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 5 — Create WBP_WS_Root (The Main Widget)

This is the root widget that the game launches. Build it last since all other widgets reference it.

- **Root component:** CanvasPanel
- **Child components (in z-order, back to front):**

1. **Background** (Image) — fills entire canvas, uses `T_WS_Background` texture
2. **Frame** (add instance of `WBP_WS_Frame`) — decorative border overlay
3. **TitleBlock** (VerticalBox):
   - "OUR LAST" TextBlock — large font, top position, color #FFFFFF or accent
   - "CHANCE" TextBlock — same style, below "OUR LAST"
4. **MainMenu** (VerticalBox, centered on screen):
   - 5 `WBP_WS_MenuButton` instances: PLAY, SETTINGS, LOAD GAME, QUIT (and any future buttons)
   - Each button wired to its action delegate
5. **ControlPanel** (CanvasPanel child, bottom of screen):
   - Version text / copyright info
   - Optional: social links, website URL
6. **SettingsOverlay** (add instance of `WBP_SettingsMenu`):
   - Initially hidden (SetVisibility = Hidden)
   - When Settings button is clicked → fade in over 0.25s
   - When close button is pressed → fade out over 0.25s, return to main menu

- **Events:**
  - `OnInitialize` — load saved settings from SaveGame + UGameUserSettings, apply to UI controls. Use `check_save_exists` and `get_latest_save_slot` from `save-game-toolset` to determine if "Load Game" button should be enabled.
  - `PlayClicked` delegate → call `openLevel` from `editor-app-toolset` with target level path
  - `SettingsClicked` delegate → make SettingsOverlay visible with fade animation
  - `QuitClicked` delegate → call `quitApplication` from `editor-app-toolset` (or show confirmation in PIE)

**Toolset:** `umg-toolset`, `blueprint-tools`, `save-game-toolset`

### Phase 6 — Wire Actions & Navigation

For each menu button's action:

1. **Play button** → calls a function that loads the first game level (e.g., dropship hub or galaxy map). Use `openLevel` from `editor-app-toolset` with the target level path (no `.umap` extension, use Content Browser path like `/Game/Levels/GalaxyMap`).
2. **Settings button** → triggers SettingsOverlay visibility change with animation.
3. **Load Game button** → future expansion; for now, can be disabled or show a "Coming Soon" toast. Use `check_save_exists` from `save-game-toolset` to determine if the button should be enabled.
4. **Quit button** → calls `quitApplication` from `editor-app-toolset`, or shows a confirmation dialog.

For settings persistence:

1. Create a SaveGame object class to hold project-specific settings (volume levels, quality preset, keybinds) using `blueprint-tools`.
2. On any setting change event → write the new value to the SaveGame object and call `SaveToSlot`.
3. On widget initialization → call `LoadFromSlot` to restore saved values, then apply them to UI controls.
4. For engine-level settings (resolution, fullscreen mode) → use `UGameUserSettings` ApplySettings methods via Blueprint nodes.

For save game discovery (Load Game button):
1. Use `list_save_slots("*")` from `save-game-toolset` to discover available saves
2. Use `get_latest_save_slot("*")` to populate the "Continue" button with the most recent save name
3. Use `get_save_metadata(slot, 0)` to display timestamp and level info

**Toolset:** `blueprint-tools`, `editor-app-toolset`, `save-game-toolset`

### Phase 7 — Create Subsystems

1. **WS_SettingsSubsystem** (GameInstance Subsystem):
   - Variables: `MasterVolume`, `MusicVolume`, `SFXVolume`, `Brightness`, `ResolutionIndex`, `QualityPreset`, `bFullscreen`, `bVSync`, `bReduceMotion`, `MouseSensitivity`
   - Functions: `SaveSettings()`, `LoadSettings()`, `GetSetting(String)` — generic getter for UI binding
   - On `GameInitialized` event → auto-load saved settings

2. **Apply Reduce Motion:** When `bReduceMotion` is true, set all animation durations to 0s (disable transitions). Check this on widget initialization and apply globally.

**Toolset:** `blueprint-tools`, `config-settings-toolset`

### Phase 8 — Add Launch Path

1. Set the startup widget using `set_startup_widget("/Game/UI/WelcomeScreen/WBP_WS_Root")` from `config-settings-toolset`
   - This handles the `_C` suffix automatically and updates Project Settings → Maps & Modes
2. Verify via `get_project_setting("Maps & Modes", "Startup Widget")` that it was applied correctly

**Toolset:** `config-settings-toolset`

### Phase 9 — Compile & Verify

1. **Compile every Blueprint** after creation/modification using the compile tool from `blueprint-tools`.
2. Fix any compilation errors before proceeding to the next step.
3. **Test in PIE:**
   - Verify welcome screen loads on startup
   - Click each menu button and confirm correct behavior
   - Open Settings, switch tabs, adjust sliders/combo/checkboxes
   - Close Settings and return to main menu
   - Test Reduce Motion toggle (if accessible) disables animations
4. **Verify asset references** — no missing texture warnings in the output log

## Key Patterns & Gotchas

### Always Compile After Changes
Every Blueprint modification requires a compile step. Errors block all downstream work. If compilation fails, read the error message, fix the issue, and recompile before proceeding.

### WidgetSwitcher for Tabs
The settings menu uses a show/hide pattern (WidgetSwitcher) rather than destroying/recreating tab content. This preserves slider values when switching tabs. Each tab panel is a child of a container; only the active one has visibility = Visible.

### Settings Persistence Dual-Path
- **Engine settings** (resolution, fullscreen): `UGameUserSettings` — these are applied immediately and persist across sessions via UE5's built-in system.
- **Project settings** (volumes, quality, keybinds): Custom SaveGame object — you handle load/save explicitly in the SettingsSubsystem.

### Reduce Motion Handling
Check `UUserSettings::bReduceMotion` on widget init. If true, skip all animation nodes and set opacity/visibility directly. This is an accessibility requirement, not optional.

### Button State Management
The MenuButton uses a state enum (Normal/Hovered/Pressed/Disabled) rather than separate boolean flags. This prevents invalid states (e.g., both Hovered and Disabled). The visual swap happens by changing the Image's Brush.Image property to the corresponding texture.

### Animation Timings (from design spec)
- Fade-in (welcome screen / settings overlay): 0.25s
- Hover transition: 0.15s
- Press feedback: 0.08s
- Settings open/close: 0.25s

### Color Palette
- Accent: `#E8852A` (warm orange)
- Normal text/button: `#9CA3AF` (gray)
- Hovered text/button: `#FFFFFF` (white)
- Disabled opacity: 0.45
- Background: dark realistic sci-fi (from T_WS_Background texture)

### Input Mode & Cursor Management
- Main menu should use **GameAndUI** input mode (`setInputMode("GameAndUI")`) — cursor visible + both UI and game input active
- Settings overlay keeps GameAndUI mode; close button returns to same mode
- Use `showMouseCursor(true)` when entering menus, `showMouseCursor(false)` when returning to gameplay
- Never leave the game in UIOnly mode during PIE testing — you won't be able to stop the session

## File Structure Summary

```
/Game/UI/WelcomeScreen/
├── T_WS_Background.uasset
├── T_WS_Button_Normal.uasset
├── T_WS_Button_Active.uasset
├── T_WS_Button_Hovered.uasset
├── T_WS_Button_Pressed.uasset
├── T_WS_Button_Disabled.uasset
├── T_WS_Frame.uasset
├── T_WS_Settings_BG.uasset
├── T_WS_Tab_Normal.uasset
├── T_WS_Tab_Active.uasset
├── T_WS_Slider_Handle.uasset
├── T_WS_Checkbox.uasset
├── WBP_WS_MenuButton.uasset
├── WBP_WS_Frame.uasset
├── WBP_SettingsMenu.uasset
├── WBP_SettingsTabButton.uasset
├── WBP_SettingsRow_Slider.uasset
├── WBP_SettingsRow_Combo.uasset
├── WBP_SettingsRow_Checkbox.uasset
└── WBP_WS_Root.uasset

/Game/Subsystems/
└── WS_SettingsSubsystem.uasset
```

## Next Steps After This Skill

Once the Welcome Screen + Settings Menu are complete, proceed to:
- **M02 Dropship Hub** — central navigation between all game modes
- **Galaxy Map (Travel loop)** — star system navigation
- **Squad Selection (Tactical prep)** — unit and ability selection before combat
- **Construction Mode (Strategic)** — PCG-based building placement in colony view

Each of these has its own skill file following the same structure.
