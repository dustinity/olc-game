# UI Component Library — Agent Skill

## What to Create

### Purpose

This skill defines the **shared UI component library** — reusable, cross-cutting widget definitions that every other RTS UI skill references. Instead of each skill inventing its own buttons, cards, progress bars, and dialogs, they all use these standardized components with consistent styling, behavior, and animation patterns.

Think of this as the design system foundation: once built, no other skill needs to create a button or card from scratch — they instantiate these components and configure them.

### Assets

| Asset Name | Texture Reference | Path in Project | Source |
|-----------|------------------|-----------------|--------|
| Button Backgrounds (3 states) | `T_Button_BG_*` | `/Game/UI/Components/T_Button_BG_Normal`, etc. | Normal, hover, pressed — dark panel with accent border (#E8852A) |
| Tab Indicator | `T_Tab_Indicator` | `/Game/UI/Components/T_Tab_Indicator` | Bottom underline, 32px wide, accent color |
| Progress Bar Fill | `T_ProgressFill` | `/Game/UI/Components/T_ProgressFill` | Gradient fill: green (good), yellow (warning), red (critical) |
| Scrollbar Track + Thumb | `T_ScrollTrack`, `T_ScrollThumb` | `/Game/UI/Components/T_Scroll*` | Dark track, semi-transparent thumb with accent hover |
| Card Background | `T_Card_BG` | `/Game/UI/Components/T_Card_BG` | Rounded rectangle, dark panel (#1A1A2E), subtle border |
| Modal Overlay | `T_ModalOverlay` | `/Game/UI/Components/T_ModalOverlay` | Semi-transparent black (60% opacity) for dimming background |
| Tooltip Background | `T_Tooltip_BG` | `/Game/UI/Components/T_Tooltip_BG` | Dark panel (#2A2A3E), small border radius, white text |
| Toast Notification BG | `T_Toast_BG` | `/Game/UI/Components/T_Toast_BG` | Success = green tint, Error = red tint, Info = blue tint |

**Toolset:** `asset-tools`, `import-export-toolset`

### Widget Blueprints (12 total)

| Blueprint Name | Type | Purpose |
|---------------|------|---------|
| `WBP_Button_Primary` | UserWidget | Accent-colored button with hover/pressed states, text label |
| `WBP_Button_Secondary` | UserWidget | Outlined button, less prominent than Primary |
| `WBP_Button_Destructive` | UserWidget | Red-tinted button for delete/cancel actions |
| `WBP_Card` | UserWidget | Container with dark background, border, padding — wraps content |
| `WBP_ProgressBar` | UserWidget | Horizontal bar: track + fill + label (percentage or value) |
| `WBP_ScrollPanel` | UserWidget | Scrollable container with styled scrollbar |
| `WBP_TabButton` | UserWidget | Tab item: text + bottom indicator, active/inactive states |
| `WBP_TabStrip` | UserWidget | Horizontal row of TabButtons — manages which tab is active |
| `WBP_ModalDialog` | UserWidget | Base modal: overlay + content area + close button |
| `WBP_Tooltip` | UserWidget | Floating tooltip that appears on hover over any widget |
| `WBP_ToastNotification` | UserWidget | Brief notification appearing at screen edge, auto-dismisses after 3s |
| `WBP_Slider` | UserWidget | Horizontal slider: track + fill + draggable handle + value label |

**Toolset:** `umg-toolset`, `blueprint-tools`

### Blueprint Classes (Gameplay)

| Name | Type | Purpose |
|------|------|---------|
| `BP_UIComponentLibrary` | Static Utility Class | Central registry of all UI components, theme colors, animation timings |
| `BP_ToastManager` | GameInstance subclass | Manages toast notifications across the entire game — queues, displays, dismisses |

**Toolset:** `blueprint-tools`, `data-tools`

## MCP Workflow: Step by Step

### Phase 1 — Inspect & Prepare

1. **Inspect the project** for existing UI components
   - Toolset: `editor-toolset`
2. **Create directory structure**: `/Game/UI/Components/` and `/Game/Gameplay/UI/`
   - Toolset: `asset-tools` / `editor-toolset`

### Phase 2 — Import Textures (9 textures)

Import all component textures with SRGB = true, TF_Bilinear. Button backgrounds should be frame-style (transparent center, colored border) so they can overlay any button text/icon without clipping. Progress bar fill uses a gradient texture that stretches horizontally.

**Toolset:** `asset-tools`, `import-export-toolset`

### Phase 3 — Create UI Component Library Static Class

1. **Create `BP_UIComponentLibrary`**:
   - Constants (theme colors):
     - `AccentColor` = #E8852A (warm orange)
     - `BackgroundColor` = #0D0D1A (near-black, slight blue tint)
     - `PanelColor` = #1A1A2E (dark panel)
     - `TextColor` = #FFFFFF (white)
     - `TextSecondaryColor` = #B0B0C0 (muted gray-white)
     - `SuccessColor` = #4CAF50 (green)
     - `WarningColor` = #FFC107 (yellow/amber)
     - `ErrorColor` = #F44336 (red)
     - `InfoColor` = #2196F3 (blue)
   - Constants (animation timings):
     - `HoverAnimDuration` = 0.15s
     - `PressAnimDuration` = 0.08s
     - `ToastDismissDelay` = 3.0s
     - `ModalFadeInDuration` = 0.2s
   - Functions:
     - `GetThemeColor(ThemeColorType)` → FLinearColor — returns the named theme color
     - `CreateButton(ButtonType, LabelText)` → WBP_Button_Primary/Secondary/Destructive — factory function to create a configured button widget
     - `CreateCard()` → WBP_Card — creates an empty card container with default padding and background

**Toolset:** `blueprint-tools`

### Phase 4 — Create Toast Manager

1. **Create `BP_ToastManager`** (GameInstance subclass):
   - Variables:
     - `ToastQueue` (Array of struct: MessageText, ToastType [Success/Warning/Error/Info], Duration)
     - `ActiveToasts` (Array of WBP_ToastNotification references) — currently displayed toasts
   - Functions:
     - `ShowToast(MessageText, ToastType)` → adds to queue, triggers display if < 3 active toasts
     - `DismissToast(ToastRef)` → removes toast from screen and array
     - `ClearAllToasts()` → dismisses all active toasts immediately
   - On GameInstance initialization: creates a hidden root widget that serves as the toast container (positioned at top-right of screen, stacked vertically)

**Toolset:** `blueprint-tools`

### Phase 5 — Create UI Widgets (Bottom-Up)

#### WBP_Button_Primary / Secondary / Destructive
All three share the same base structure but differ in styling:

**Common children**: CanvasPanel root → Border (Image with button background texture) → HorizontalBox → TextBlock (label, centered) + optional Icon Image (left of text)

**Primary**: Background = accent color (#E8852A), text = white. Hover: slightly brighter accent. Pressed: darker accent.
**Secondary**: Background = transparent, border = accent outline (#E8852A, 2px). Text = accent color. Hover: filled background with lighter accent.
**Destructive**: Background = error color (#F44336), text = white. Hover: brighter red. Pressed: darker red.

**Variables**: `ButtonText` (String), `OnClicked` (delegate/event) — other widgets bind to this event when they use the button.
**Function**: `SetEnabled(bool)` — disables button by graying out text and background, preventing clicks.

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_Card
- CanvasPanel root → Border (Image with T_Card_BG) → Content slot (UserWidget component or nested children container)
- Default padding: 16px on all sides
- Variables: none — purely a container. Other widgets place their content inside the card's child slot.
- Function: `SetBorderHighlight(bool)` — adds an accent-colored (#E8852A) border glow when true (used for selected/active cards)

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ProgressBar
- HorizontalBox with 3 children:
  - **Track**: Image with dark background (#2A2A3E), fixed height = 16px, stretches to fill available width
  - **Fill**: Image with gradient texture (T_ProgressFill), stretched horizontally — its Width Percentage is set dynamically via Blueprint
  - **Label**: TextBlock showing percentage or value text, right-aligned

**Variables**: `CurrentValue` (float), `MaxValue` (float)
**Function**: `UpdateDisplay()`:
- Calculates fill percentage = CurrentValue / MaxValue
- Sets Fill image Width Percentage to that ratio
- Sets Label text to formatted string: "X%" or "X/Y" depending on display mode
- Color logic: if ratio < 0.3 → red, 0.3–0.7 → yellow, > 0.7 → green

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ScrollPanel
- CanvasPanel root → ScrollBar (vertical, right side) + Viewport (left side, takes remaining space)
- The Viewport contains a UserWidget component that holds the actual scrollable content — any other widget can be placed here as the child.
- ScrollBar uses T_ScrollTrack and T_ScrollThumb textures

**Variables**: `ScrollContent` (UserWidget reference) — set when configuring what scrolls inside
**Function**: `AddChildToViewport(widget)` — places a widget inside the Viewport for scrolling
**Function**: `ScrollToTop()` / `ScrollToBottom()` — animates scroll position

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_TabButton
- VerticalBox with 2 children:
  - **Label**: TextBlock showing tab name, centered
  - **Indicator**: Image (T_Tab_Indicator) — visible when active, hidden when inactive. Accent color (#E8852A).

**Variables**: `TabName` (String), `IsActive` (bool)
**Function**: `SetActive(bool)` — shows/hides indicator, changes text color (active = white, inactive = muted gray)
**Event**: `OnTabClicked()` — delegate that TabStrip listens to for switching tabs

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_TabStrip
- HorizontalBox containing multiple TabButton children
- Manages which tab is active: when one TabButton fires OnTabClicked, it calls SetActive(false) on all other tabs and SetActive(true) on itself
- **Variables**: `ActiveTabIndex` (int), `OnTabChanged` (delegate/event) — notifies parent widget when a different tab is selected
- **Function**: `AddTab(tabName)` → TabButton reference — creates a new TabButton, adds to the strip
- **Function**: `SelectTab(index)` — programmatically selects a tab by index

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ModalDialog
- CanvasPanel root with layered children (back to front):
  1. **Overlay**: Image (T_ModalOverlay) — fills entire screen, 60% opacity black
  2. **Content Card**: WBP_Card instance centered on screen, sized to fit content
  3. **Close Button**: WBP_Button_Secondary or X icon in top-right corner of Content Card

**Variables**: `TitleText` (String), `OnClosed` (delegate/event) — fires when close button is clicked or Escape is pressed
**Function**: `Show(contentWidget)` — places contentWidget inside the Content Card's child slot, makes modal visible with fade-in animation (0.2s)
**Function**: `Hide()` — sets visibility to false, fires OnClosed event

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_Tooltip
- CanvasPanel root → Border (Image with T_Tooltip_BG) → TextBlock (tooltip text, wrapped if long)
- Initially invisible. Appears when the parent widget's mouse enters its bounds.
- **Variables**: `TooltipText` (String), `ParentWidget` (weak reference to the widget this tooltip belongs to)
- **Function**: `Show()` — sets visibility to true, positions itself near the cursor or below/above the parent widget depending on screen space
- **Function**: `Hide()` — sets visibility to false

**Integration pattern**: Any widget that needs a tooltip creates a Tooltip instance, sets its ParentWidget and TooltipText, then binds the parent's OnMouseEnter event to Show() and OnMouseLeave to Hide().

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ToastNotification
- CanvasPanel root → Border (Image with T_Toast_BG) → HorizontalBox → Icon Image (left, type-dependent) + TextBlock (message text, centered vertically)
- Positioned at top-right of screen by the ToastManager. Stacks downward if multiple toasts are active simultaneously (max 3 visible).
- **Variables**: `MessageType` (Enum: Success/Warning/Error/Info), `MessageText` (String)
- **Function**: `Show()` — makes toast visible, starts a timer for ToastDismissDelay (3s), then calls Hide()
- **Function**: `Hide()` — sets visibility to false, notifies ToastManager to remove from ActiveToasts array and shift remaining toasts up

**Toast icon by type**: Success = checkmark, Warning = exclamation mark, Error = X symbol, Info = lowercase "i" in circle. All icons use the accent color (#E8852A) except Error which uses red (#F44336).

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_Slider
- HorizontalBox with 3 children:
  - **Label**: TextBlock showing slider name (left side)
  - **Track**: Image with dark background, fixed height = 8px, stretches to fill available width
  - **Fill**: Image with accent color (#E8852A), stretched horizontally from left edge — Width Percentage set dynamically
  - **Handle**: Image (small circle, 16x16) positioned at the end of Fill — draggable via mouse
  - **Value Label**: TextBlock showing current value (right side)

**Variables**: `CurrentValue` (float), `MinValue` (float = 0), `MaxValue` (float = 100), `OnValueChanged` (delegate/event)
**Function**: `UpdateDisplay()`:
- Calculates fill percentage = (CurrentValue - MinValue) / (MaxValue - MinValue)
- Sets Fill Width Percentage and Handle position accordingly
- Updates Value Label text to formatted string

**Drag behavior**: When player clicks/draggs the handle, UpdateDisplay is called continuously during drag. On release, fires OnValueChanged with the new CurrentValue.

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 6 — Wire ToastManager to GameInstance

1. **Set BP_ToastManager as the project's GameInstance class**:
   - In Project Settings → Maps & Modes → Default Game Instance, set to BP_ToastManager
   - This ensures ShowToast() is available globally from any Blueprint at any time
2. **Add a global shortcut**: Create a Blueprint function library with `ShowGlobalToast(MessageText, ToastType)` that calls the GameInstance's ShowToast — this lets any widget show a toast without needing a direct reference to the GameInstance

**Toolset:** `blueprint-tools`, `editor-toolset`

### Phase 7 — Establish Component Usage Conventions

Document how other skills should use these components:

- **Buttons**: Always use WBP_Button_Primary for primary actions, WBP_Button_Secondary for secondary/optional actions, WBP_Button_Destructive for destructive actions. Never create custom buttons.
- **Cards**: Use WBP_Card as the container for any grouped content (unit info panels, equipment slots, building details). Set border highlight when selected.
- **Progress bars**: Always use WBP_ProgressBar with automatic color coding. For non-percentage progress (e.g., "3/5"), set display mode to value-based.
- **Tabs**: Use WBP_TabStrip for any screen with multiple content categories. Each tab button should have a clear, short label.
- **Modals**: Always use WBP_ModalDialog — never create custom overlay dialogs. Content goes inside the card container.
- **Tooltips**: Add tooltips to any interactive element whose purpose isn't immediately obvious from its icon or label.
- **Toasts**: Use for transient feedback: "Equipment equipped", "Upgrade complete", "Insufficient resources". Never use for critical information that requires user action (use modals for those).
- **Scroll panels**: Use when content exceeds ~500px vertical height. Always scroll to top on initial display.
- **Sliders**: Use for numeric ranges with clear min/max (volume, speed multipliers, priority levels 1-10).

**Toolset:** `blueprint-tools`

### Phase 8 — Compile & Verify

1. **Compile every Blueprint** after creation/modification
2. **Test each component in isolation**:
   - Create a simple test widget that instantiates all components and displays them
   - Test button hover/pressed states visually
   - Drag a slider and verify fill updates smoothly
   - Show toasts via ToastManager — verify 3-second auto-dismiss, stacking behavior (max 3), color coding by type
   - Open a modal dialog with sample content — verify fade-in animation, close on Escape key
   - Add tooltips to buttons — verify hover appearance and positioning
   - Create a tab strip with 3 tabs — verify switching updates active indicator
   - Scroll a panel with long content — verify scrollbar styling and smooth scrolling

## Key Patterns & Gotchas

### Components Are Never Modified Directly
Other skills should **instantiate** these components and configure them via their public variables/functions. They should never modify the component Blueprints themselves (e.g., don't add new children to WBP_Button_Primary for a specific skill's needs). If a variant is needed, create a new widget that wraps or extends the base component.

### ToastManager Must Be Accessible Globally
The BP_ToastManager as GameInstance + global function library pattern ensures any Blueprint in the game can show a toast without needing to find and hold a reference to the GameInstance. This is critical because UI widgets often need to show feedback but don't always have direct access to the GameInstance.

### Tooltip Positioning Needs Screen Space Awareness
The WBP_Tooltip's Show() function must check if there's enough screen space below the parent widget. If not, it positions above instead. If neither fits (widget is at extreme top or bottom), position horizontally adjacent with a slight offset. This prevents tooltips from being clipped off-screen.

### Progress Bar Color Coding is Automatic
The WBP_ProgressBar.UpdateDisplay() function automatically sets fill color based on the percentage ratio (< 0.3 = red, 0.3–0.7 = yellow, > 0.7 = green). Other skills should NOT manually set progress bar colors — let the component handle it. This ensures consistency across all screens.

### Modal Escape Key Must Be Bound at Root Level
The WBP_ModalDialog handles Escape key internally, but this only works if the modal widget has keyboard focus. The Show() function must call SetKeyboardFocus() on itself to ensure Escape dismisses the modal reliably. Without this, players can get stuck in modals they can't close.

### ScrollPanel Content Must Have Fixed Height
For scrolling to work correctly, the child widget placed inside WBP_ScrollPanel's Viewport should have a fixed or minimum height that exceeds the viewport's visible area. If the content is shorter than the viewport, there's nothing to scroll — which is fine, but don't expect scrollbars to appear for short content.

## File Structure Summary

```
/Game/UI/Components/
├── T_Button_BG_Normal.uasset (Hover, Pressed)
├── T_Tab_Indicator.uasset
├── T_ProgressFill.uasset
├── T_ScrollTrack.uasset (ScrollThumb)
├── T_Card_BG.uasset
├── T_ModalOverlay.uasset
├── T_Tooltip_BG.uasset
└── T_Toast_BG.uasset

/Game/UI/Components/WBP_*.uasset (12 widget blueprints listed above)

/Game/Gameplay/UI/
├── BP_UIComponentLibrary.uasset (Static Utility Class)
└── BP_ToastManager.uasset (GameInstance subclass)
```

## Next Steps After This Skill

The UI Component Library is the **foundation** — all other RTS UI skills should reference and use these components rather than creating their own. It connects to every other skill:

- **Welcome Screen, Settings, Colony Management, Construction Mode, Tech Tree, Tactical Combat, Squad Selection, Resource HUD, Planet Overview, Dropship Modules, Galaxy Map, Unit Upgrade, Combat Results** — all use these shared components for buttons, cards, progress bars, tabs, modals, tooltips, toasts, and sliders.

This skill should be built **first** (or alongside the first UI skill) so that subsequent skills can immediately leverage the component library.
