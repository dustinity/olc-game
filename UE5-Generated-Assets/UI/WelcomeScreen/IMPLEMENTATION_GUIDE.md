# Welcome Screen + Settings Menu UE5 Implementation Guide

This guide is for a coding agent implementing the `M01 Welcome / Login Screen` and related `M03 Settings / Options` menu in Unreal Engine 5 for **Our Last Chance**.

Source assets live here:

`/home/chbe/Development/Our Last Chance/UE5/Assets/UI/WelcomeScreen`

The Unreal project lives here:

`/home/chbe/Development/Our Last Chance/Unreal/OurLastChance`

Import the PNGs into the Unreal project as engine assets under:

`/Game/UI/WelcomeScreen`

Do not reference the raw PNG files directly from gameplay code.

## Visual Target

Match `Assets/Style/Stil-1/View/Mockups/01-Game-Welcome-Screen.png`.

Style rules:

- Dark realistic hard sci-fi interface.
- Gunmetal panels, angular military framing, orange active state, blue-grey inactive state.
- Text should be uppercase, condensed, bold, and high contrast.
- Suggested title font: Orbitron Bold or Rajdhani Bold.
- Suggested menu font: Rajdhani Bold, Roboto Condensed Bold, or a project-approved equivalent.
- Primary accent: `#E8852A`.
- Hover accent: `#F59E3F`.
- Inactive text: `#9CA3AF`.
- Main white: `#FFFFFF`.

## Provided Assets

Import these files:

| Source File | UE Texture Name | Use |
|---|---|---|
| `Background_3840x2160.png` | `T_WS_Background` | Full-screen background plate |
| `Button_Active.png` | `T_WS_Button_Active` | Hovered / selected / focused menu option |
| `Button_Inactive.png` | `T_WS_Button_Inactive` | Normal menu option |
| `Frame_Corner_TL.png` | `T_WS_Frame_Corner_TL` | Outer frame top-left |
| `Frame_Corner_TR.png` | `T_WS_Frame_Corner_TR` | Outer frame top-right |
| `Frame_Corner_BR.png` | `T_WS_Frame_Corner_BR` | Outer frame bottom-right |
| `Frame_Corner_BL.png` | `T_WS_Frame_Corner_BL` | Outer frame bottom-left |
| `Frame_Edge_Top.png` | `T_WS_Frame_Edge_Top` | Outer frame top edge |
| `Frame_Edge_Bottom.png` | `T_WS_Frame_Edge_Bottom` | Outer frame bottom edge |
| `Frame_Edge_Left.png` | `T_WS_Frame_Edge_Left` | Outer frame left edge |
| `Frame_Edge_Right.png` | `T_WS_Frame_Edge_Right` | Outer frame right edge |
| `Frame_Control_Panel.png` | `T_WS_Frame_ControlPanel` | Bottom-right decorative/control panel |

Recommended texture settings:

- Background: `UI` texture group or project equivalent, no mip blur if it softens too much.
- Button/frame sprites: `UserInterface2D`, alpha preserved, sRGB enabled.
- Compression: UI/Default UI compression, never normal-map compression.

## Recommended Widget Blueprints

Create these Widget Blueprints under `/Game/UI/WelcomeScreen`:

| Widget | Responsibility |
|---|---|
| `WBP_WS_Root` | Full welcome screen composition and menu state |
| `WBP_WS_MenuButton` | Reusable main menu button with normal/active images |
| `WBP_WS_Frame` | Outer screen frame assembled from frame parts |
| `WBP_SettingsMenu` | Settings overlay / full-screen submenu |
| `WBP_SettingsTabButton` | Reusable settings tab button |
| `WBP_SettingsRow_Slider` | Labeled slider row for volume / gamma / sensitivity |
| `WBP_SettingsRow_Combo` | Labeled combo row for resolution / quality / window mode |
| `WBP_SettingsRow_Checkbox` | Labeled checkbox row for toggles |

If the project already has base widgets such as `WBP_Button_Base` or `WBP_Panel_Angular`, derive from those instead of creating unrelated styles.

## Welcome Screen Layout

`WBP_WS_Root` hierarchy:

```text
CanvasPanel Root
  Image Background
  WBP_WS_Frame OuterFrame
  VerticalBox TitleBlock
    TextBlock OUR LAST
    TextBlock CHANCE
  VerticalBox MainMenu
    WBP_WS_MenuButton Continue
    WBP_WS_MenuButton NewCampaign
    WBP_WS_MenuButton LoadGame
    WBP_WS_MenuButton Settings
    WBP_WS_MenuButton Exit
  WBP_WS_Frame_ControlPanel BottomRightControl
  WBP_SettingsMenu SettingsOverlay
```

Baseline design resolution is `1920 x 1080`.

Suggested placement:

- Background: fill screen, scale to cover.
- Outer frame: anchored full screen, about `24 px` inset at 1080p.
- Title block: left side, around `X=150`, `Y=165`.
- Main menu: left side below title, around `X=150`, `Y=455`, button size about `470 x 64`.
- Bottom-right control panel: anchored bottom-right, around `40 px` margin.
- Settings overlay: hidden by default, fills screen or uses a centered angular panel.

The background already has negative space on the left. Keep the menu there.

## Menu Button Behavior

`WBP_WS_MenuButton` should expose:

- `ButtonText: Text`
- `bIsActive: bool`
- `OnClicked: Event Dispatcher`

Internal hierarchy:

```text
Button HitTarget
  Overlay
    Image StateImage
    TextBlock Label
```

State logic:

- Normal: `T_WS_Button_Inactive`, inactive text color `#9CA3AF`.
- Hovered / keyboard focus: `T_WS_Button_Active`, text color `#FFFFFF`.
- Pressed: active image, render scale `0.98`, optional darker tint.
- Disabled: inactive image, opacity `0.45`.

Use UMG button events:

- `OnHovered` -> set active visual.
- `OnUnhovered` -> clear active visual unless keyboard focused.
- `OnPressed` -> slight scale down.
- `OnReleased` -> restore scale.
- `OnClicked` -> broadcast.

Keyboard/gamepad support:

- First focus should go to `Continue` if a save exists, otherwise `NewCampaign`.
- Up/down cycles menu focus.
- Enter / Gamepad Face Button Bottom activates.
- Escape from welcome screen should open an exit confirmation, not instantly quit.

## Welcome Screen Actions

Implement action bindings in `WBP_WS_Root`:

| Button | Behavior |
|---|---|
| Continue | Load latest valid save. Disable when no save exists. |
| New Campaign | Open new campaign flow or load first playable map when no flow exists yet. |
| Load Game | Open save selection screen. Disable when no saves exist if save UI is not ready. |
| Settings | Show `WBP_SettingsMenu`, focus first settings tab. |
| Exit | Show confirmation dialog, then call quit game on confirm. |

Use a lightweight menu controller or subsystem for these actions if one already exists. Avoid putting long save/loading logic directly inside widget graph nodes.

## Settings Menu Structure

`WBP_SettingsMenu` should be a modal overlay that can be opened from welcome and later reused by the pause menu.

Recommended hierarchy:

```text
CanvasPanel SettingsRoot
  Border DimBackground
  Overlay SettingsPanel
    WBP_WS_Frame PanelFrame
    VerticalBox Content
      HorizontalBox Header
        TextBlock SETTINGS
        Button Close
      HorizontalBox Body
        VerticalBox Tabs
          WBP_SettingsTabButton Graphics
          WBP_SettingsTabButton Audio
          WBP_SettingsTabButton Controls
          WBP_SettingsTabButton Gameplay
          WBP_SettingsTabButton Accessibility
        WidgetSwitcher TabContent
          GraphicsPage
          AudioPage
          ControlsPage
          GameplayPage
          AccessibilityPage
      HorizontalBox Footer
        Button ResetDefaults
        Button Apply
        Button Back
```

Settings pages:

### Graphics

- Window mode: Fullscreen, Windowed Fullscreen, Windowed.
- Resolution.
- VSync.
- Frame rate limit.
- Overall quality.
- Texture quality.
- Shadow quality.
- Effects quality.
- Post-processing quality.
- Gamma / brightness.

Back these settings with `UGameUserSettings` where possible.

### Audio

- Master volume.
- Music volume.
- SFX volume.
- UI volume.
- Voice volume if planned.
- Mute when unfocused if desired.

Back these values with a project `USaveGame` settings object or audio settings subsystem. Apply to Sound Mix / Sound Classes / Audio Modulation according to the project audio setup.

### Controls

- Mouse sensitivity.
- Invert Y.
- Edge scroll toggle.
- Keybinding entry point.
- Gamepad sensitivity.

Use Enhanced Input if enabled in the project. If rebinding is not ready, create disabled rows and a clear placeholder action path.

### Gameplay

- Camera pan speed.
- Autosave interval.
- Show tutorials.
- Pause game when menu opens where applicable.

The welcome screen itself is not in active simulation, so do not show RTS time controls here.

### Accessibility

- UI scale.
- Subtitles.
- High contrast UI toggle.
- Reduce motion.
- Colorblind mode placeholder if not implemented yet.

At minimum, implement UI scale and reduce motion because they affect menu readability and animation.

## Settings Persistence

Use two layers:

1. Engine settings via `UGameUserSettings`:
   - Resolution.
   - Window mode.
   - VSync.
   - Frame limit if using engine support.
   - Scalability quality levels.

2. Project settings via a SaveGame object, for example `USaveGame_UISettings`:
   - Audio volumes.
   - UI scale.
   - Reduce motion.
   - Tutorials.
   - Input preferences not covered by Enhanced Input user settings.

Recommended flow:

```text
Open Settings
  Read current engine/project settings into a temporary view model.

Change Row
  Update temporary view model only.
  Enable Apply button.

Apply
  Push values to UGameUserSettings and project settings subsystem.
  Save settings.
  Disable Apply button.

Back / Close with pending changes
  Ask: Apply, Discard, Cancel.
```

Do not save every slider tick unless the project already uses immediate settings. For audio sliders, live preview is fine, but keep final persistence on Apply or Back confirmation.

## Suggested C++ / Blueprint Split

Blueprint:

- Widget layout.
- Menu animations.
- Visual state changes.
- Simple event dispatchers.

C++ or subsystem:

- Save discovery and latest-save selection.
- Map loading.
- Settings read/write.
- UGameUserSettings integration.
- Exit confirmation execution.

Possible classes:

```text
UOLCMenuSubsystem
  HasAnySaveGame()
  GetLatestSaveSlot()
  ContinueLatestSave()
  StartNewCampaign()
  OpenLoadGame()
  RequestQuit()

UOLCSettingsSubsystem
  LoadProjectSettings()
  ApplyProjectSettings()
  SaveProjectSettings()
  Get/Set audio and UI preferences
```

If the project does not yet have these subsystems, create minimal versions rather than embedding all behavior into `WBP_WS_Root`.

## Animation

Keep the menu responsive and mechanical:

- Screen fade-in: `0.25s`.
- Button hover brighten: `0.15s`.
- Button press scale: `0.08s`.
- Settings open: fade dim background and slide panel up over `0.25s`.
- Settings close: reverse over `0.20s`.

Honor `Reduce Motion`:

- Replace slides/scales with simple fades.
- Disable looping glows or strong pulses.

## Unreal MCP Workflow For The Coding Agent

Use Unreal MCP/editor automation as the source of truth for asset and widget state.

Recommended sequence:

1. Inspect project:
   - Confirm `OurLastChance.uproject`.
   - Confirm enabled UI plugins: UMG, Slate, CommonUI if used, Enhanced Input if used.
   - Query existing `/Game/UI` assets before creating new duplicates.

2. Import textures:
   - Import the PNGs from `UE5/Assets/UI/WelcomeScreen`.
   - Destination: `/Game/UI/WelcomeScreen/Textures`.
   - Rename imported textures to the `T_WS_*` names listed above.
   - Set button/frame textures to UI compression with alpha.

3. Create widgets:
   - Create `/Game/UI/WelcomeScreen/Widgets/WBP_WS_Root`.
   - Create reusable child widgets first: button, frame, settings rows.
   - Use existing base widgets if found.

4. Wire widget references:
   - Assign all texture brushes.
   - Set anchors and offsets at `1920 x 1080`.
   - Mark interactive buttons as focusable.
   - Bind menu actions to widget events or subsystem calls.

5. Create settings data path:
   - Query for existing game instance, player controller, save-game, settings, or menu subsystem classes.
   - Reuse existing classes if present.
   - Otherwise create minimal `MenuSubsystem` and `SettingsSubsystem` in the project module.

6. Add launch path:
   - Set the initial map or menu level to show `WBP_WS_Root`.
   - On begin play, set input mode UI Only and show mouse cursor.
   - On leaving the menu, restore appropriate game input mode.

7. Verify in editor:
   - Open `WBP_WS_Root` in designer and confirm no missing brushes.
   - Run PIE.
   - Check mouse hover, keyboard focus, gamepad navigation if available.
   - Open settings, change values, apply, close, reopen, verify persistence.
   - Check 16:9 at 1920x1080, 2560x1440, and 3840x2160.

8. Report back:
   - List created/modified assets.
   - List any settings intentionally stubbed.
   - Include screenshots if the MCP/editor tool can capture them.

## Acceptance Checklist

- Welcome screen background fills the viewport without stretching.
- Title and menu match the mockup layout.
- Continue is disabled or hidden when no valid save exists.
- Active/hovered menu button uses the amber active texture.
- Inactive menu buttons use the dark inactive texture.
- Settings opens without changing map or destroying the welcome screen.
- Settings can be closed with Back, Escape, and close button.
- Settings values load from saved state.
- Apply persists values.
- Unsaved settings prompt before closing.
- UI is usable with mouse and keyboard.
- No hardcoded absolute file paths remain inside Unreal assets or code.
- Widgets and textures are organized under `/Game/UI/WelcomeScreen`.

## Notes For Future Polish

- Add a subtle animated dust/fog overlay in UMG or Niagara only after the static screen is working.
- Add low-volume mechanical UI sounds for hover, click, open, close.
- Add localized text keys instead of raw English labels before shipping.
- Consider CommonUI if the project needs robust gamepad-first menu navigation across platforms.
