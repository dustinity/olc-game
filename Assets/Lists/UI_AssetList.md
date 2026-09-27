# Stil-1 UI Asset List

## Dark Realistic Hard Sci-Fi User Interface Assets

Generated from **Stil-1 Style Guide** for **Our Last Chance** game project.

This asset list catalogs all UI assets required for development, organized by screen/category and priority. Each asset includes specifications for AI image generation and Unreal Engine UMG implementation.

---

## ID Naming Convention

All UI assets use the following hierarchical ID system:

| Prefix | Category | Examples |
|--------|----------|----------|
| `UI-BG` | Backgrounds & Textures | UI-BG-01, UI-BG-02 |
| `UI-PNL` | Panels & Containers | UI-PNL-01 through UI-PNL-03 |
| `UI-BTN` | Buttons | UI-BTN-01 through UI-BTN-07 |
| `UI-ICN` | Icons (symbol-based) | UI-ICN-01 through UI-ICN-09 |
| `UI-SLD` | Sliders & Controls | UI-SLD-01 through UI-SLD-03 |
| `UI-TGB` | Tabs | UI-TGB-01 through UI-TGB-03 |
| `UI-CHK` | Checkboxes & Radios | UI-CHK-01, UI-CHK-02, UI-RDO-01, UI-RDO-02 |
| `UI-DIV` | Dividers | UI-DIV-01, UI-DIV-02 |
| `UI-PRG` | Progress Indicators | UI-PRG-01 through UI-PRG-03 |
| `HUD-` | Main HUD Elements | HUD-BAR-01, HUD-RSC-01, etc. |
| `BLD-` | Building Menu Assets | BLD-MNU-01, BLD-CAT-01, BLD-ICN-01, etc. |
| `UNT-` | Unit Panel Assets | UNT-PNL-01, UNT-HLP-01, etc. |
| `TRC-` | Tech Tree Assets | TRC-MNU-01, TRC-NOD-01, etc. |
| `RSM-` | Resource Management Assets | RSM-MNU-01, RSM-STO-01, etc. |
| `MAP-` | Map & Exploration Assets | MAP-MNU-01, MAP-DUN-01, etc. |
| `SSY-` | Solar System Assets | SSY-MNU-01, SSY-BGR-01, SSY-PLT-01, etc. |
| `PSM-` | Pause Menu Assets | PSM-MNU-01, PSM-RES-01, etc. |
| `SET-` | Settings Assets | SET-MNU-01, SET-TAB-01, etc. |
| `DLG-` | Dialog Assets | DLG-BX-01, DLG-BTN-01, etc. |
| `NTF-` | Notification Assets | NTF-POP-01 through NTF-POP-04, NTF-BNR-01, etc. |
| `TOO-` | Tooltip Assets | TOO-TIP-01 through TOO-TIP-03 |
| `LDR-` | Loading Screen Assets | LDR-SCR-01, LDR-PRG-01, etc. |
| `MNU-` | Main Menu Assets | MNU-Main-01, MNU-TTL-01 |
| `MLT-` | Multiplayer/Social Assets | MLT-CHT-01 through MLT-ALL-01 |
| `VCT-` / `DFT-` | End Game Assets | VCT-SCR-01, DFT-SCR-01, etc. |
| `FCM-` | Faction Selection Assets | FCM-SCT-01, FCM-CRD-01 through FCM-CRD-04 |
| `INV-` | Inventory Assets | INV-MNU-01, INV-SLT-01 through INV-QTY-01 |
| `MMI-` | Minimap Detail Assets | MMI-CUR-01 through MMI-BLS-01 |

**Format:** `{PREFIX}-{SEQ}` where `{PREFIX}` = category code, `{SEQ}` = sequential number.

---

## Style Reference

### Stil-1 UI Visual Language

> Dark realistic hard sci-fi military tactical display aesthetic. Gunmetal dark gray color scheme (#1A1D21) with orange accent glow (#E8852A). Functional engineering design — every element serves a purpose. Clean geometric edges, no decorative flourishes.

**Color Palette:**
- **Backgrounds:** Gunmetal Black #1A1D21, Dark Steel #242830, Charcoal Gray #2E323A
- **Borders/Dividers:** Medium Gray #3D424D
- **Primary Accent (Orange):** #E8852A — buttons, active states, warnings
- **Secondary Accent (Blue):** #3B82F6 — interactive elements, selections
- **Positive (Green):** #22C55E — health full, success
- **Warning (Yellow):** #F59E0B — low resources, caution
- **Danger (Red):** #EF4444 — damage alerts, destructive actions
- **Elite (Purple):** #A855F7 — dark matter/elite tier indicators

**Typography:**
- Headers: Orbitron Bold / Rajdhani Bold
- Body: Roboto Regular / Inter Regular
- Labels: Roboto Medium uppercase
- Numbers: Orbitron Bold

### Gameplay UI Rules

- **Real-time only:** Planet/base gameplay is an RTS and dungeon gameplay is real-time hack-and-slay. UI must never show turns, action points, or end-turn controls.
- **Simulation state:** Time widgets show elapsed mission time plus pause and simulation-speed state (`PAUSED`, `1×`, `2×`).
- **Dynamic resource strip:** Only instantiate counters for resources currently discovered and available to the player. Do not reserve empty slots for locked or unavailable resources.
- **Repair economy:** Routine repairs consume **Construction Material only**. Minerals, titanium, energy, and other specialist resources may be used for crafting, upgrades, refueling, or component replacement, but never appear as routine repair costs.
- **Dungeon route:** Dungeon UI presents a real-time room route and exploration state. Abandoned structures remain unexplored/unknown until cleared and yield their blueprint at completion.

---

## Table of Contents

1. [Phase 0 — Core Gameplay UI (P0)](#phase-0--core-gameplay-ui-p0)
2. [Phase 1 — Interactive Menus (P1)](#phase-1--interactive-menus-p1)
3. [Phase 2 — Full-Screen Modals (P2)](#phase-2--full-screen-modals-p2)
4. [Phase 3 — Secondary Features (P3)](#phase-3--secondary-features-p3)
5. [AI Image Generation Prompts](#ai-image-generation-prompts)

---

## Phase 0 — Core Gameplay UI (P0)

**Priority:** Generate first — needed for basic gameplay functionality  
**Asset Count:** ~60 assets

### A. Global UI Elements (Buttons, Panels, Controls)

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `UI-BG-01` | Main Background Panel | 512×512 | Dark gunmetal panel with subtle grid line pattern overlay, transparent background | `dark gunmetal panel #1A1D21, subtle grid lines, military tactical display` |
| `UI-BG-02` | Modal Overlay | 1920×1080 | Semi-transparent dark overlay (#1A1D21 at 70% opacity) for popup screens | `semi-transparent dark overlay #1A1D21, full-screen modal background` |
| `UI-PNL-01` | Standard Panel (rounded) | 400×300 | Main container panel, rounded corners 8px, orange border glow on active edges | `rounded panel 8px corner radius, dark gunmetal #1A1D21, orange border glow #E8852A` |
| `UI-PNL-02` | Standard Panel (sharp) | 400×300 | Angular military-style panel with chamfered top-left corner | `angular military panel, chamfered top-left corner, dark steel #242830` |
| `UI-PNL-03` | Small Info Panel | 200×150 | Compact tooltip/info panel, 4px corner radius | `compact info panel, small rounded rectangle, charcoal gray #2E323A` |
| `UI-BTN-01` | Primary Button (normal) | 200×50 | Orange glow border, white bold text, 4px corner radius | `primary button, orange background #E8852A, white text, rounded corners` |
| `UI-BTN-02` | Primary Button (hover) | 200×50 | Brighter orange glow (#F59E3F), highlighted state | `primary button hover state, bright orange #F59E3F, enhanced glow` |
| `UI-BTN-03` | Primary Button (pressed) | 200×50 | Darker orange (#C47020), inset shadow effect | `primary button pressed state, dark orange #C47020, inner shadow` |
| `UI-BTN-04` | Secondary Button (normal) | 200×50 | Blue outline border (#3B82F6), transparent fill | `secondary button, blue outline border #3B82F6, transparent center` |
| `UI-BTN-05` | Secondary Button (hover) | 200×50 | Brighter blue glow with subtle blue background tint | `secondary button hover, bright blue glow #60A5FA, light blue tint` |
| `UI-BTN-06` | Danger Button (normal) | 200×50 | Red accent (#EF4444) for destructive actions | `danger button, red background #EF4444, white text` |
| `UI-BTN-07` | Icon Button (small) | 50×50 | Circular/dark panel button for close/minimize functions | `icon-only button, dark circular panel, minimal design` |
| `UI-ICN-01` | Close (X) Icon | 32×32 | White X symbol on dark background square | `close icon, white X symbol, minimalist line art, 32x32 grid` |
| `UI-ICN-02` | Minimize (_) Icon | 32×32 | White underscore/horizontal line symbol | `minimize icon, white horizontal line, minimalist, 32x32 grid` |
| `UI-ICN-03` | Maximize (□) Icon | 32×32 | White square outline symbol | `maximize icon, white square outline, minimalist, 32x32 grid` |
| `UI-ICN-04` | Chevron Right (>) | 32×32 | Navigation arrow pointing right | `chevron right icon, white arrow >, minimalist line art, 32x32` |
| `UI-ICN-05` | Chevron Left (<) | 32×32 | Navigation arrow pointing left | `chevron left icon, white arrow <, minimalist line art, 32x32` |
| `UI-ICN-06` | Chevron Down (∨) | 32×32 | Dropdown indicator arrow pointing down | `chevron down icon, white arrow ∨, dropdown indicator, 32x32` |
| `UI-ICN-07` | Checkmark (✓) | 32×32 | Confirmation/green status checkmark symbol | `checkmark icon, green ✓ symbol, confirmation, 32x32 grid` |
| `UI-ICN-08` | Warning (!) | 32×32 | Orange triangle with exclamation mark | `warning icon, orange triangle ! symbol, caution indicator, 32x32` |
| `UI-ICN-09` | Info (i) | 32×32 | Blue circle with lowercase i letter | `info icon, blue circle i symbol, information indicator, 32x32` |
| `UI-SLD-01` | Slider Track (horizontal) | 200×8 | Dark gray track with orange fill portion | `horizontal slider track, dark gray #3D424D background, orange fill #E8852A` |
| `UI-SLD-02` | Slider Handle | 24×24 | Orange glowing circular knob for dragging | `slider handle, orange glowing circle #E8852A, 24x24px knob` |
| `UI-SLD-03` | Slider Track (vertical) | 8×200 | Vertical variant for volume/resource bars | `vertical slider track, dark gray #3D424D, orange fill #E8852A` |
| `UI-TGB-01` | Tab (selected) | 150×40 | Orange top border highlight, white text | `selected tab, orange top border #E8852A, white text on dark bg` |
| `UI-TGB-02` | Tab (unselected) | 150×40 | Dark panel with gray text, no highlight | `unselected tab, dark steel background #242830, gray text` |
| `UI-TGB-03` | Tab (hover) | 150×40 | Subtle orange edge glow on hover state | `tab hover state, subtle orange edge glow, dark background` |
| `UI-CHK-01` | Checkbox (unchecked) | 24×24 | Empty square with gray border | `unchecked checkbox, empty square outline, gray border #3D424D` |
| `UI-CHK-02` | Checkbox (checked) | 24×24 | Filled square with white checkmark | `checked checkbox, filled square with white checkmark ✓` |
| `UI-RDO-01` | Radio Button (unchecked) | 24×24 | Empty circle with gray border | `unchecked radio button, empty circle outline, gray border` |
| `UI-RDO-02` | Radio Button (checked) | 24×24 | Filled circle with inner dot | `checked radio button, filled circle with center dot` |
| `UI-DIV-01` | Horizontal Divider | 500×2 | Subtle orange line separator between sections | `horizontal divider line, subtle orange #E8852A at 60% opacity, 2px height` |
| `UI-DIV-02` | Vertical Divider | 2×200 | Thin vertical separator line | `vertical divider line, subtle gray #3D424D, 2px width` |
| `UI-PRG-01` | Progress Bar (empty) | 200×16 | Dark track with border outline | `empty progress bar, dark track #3D424D with border, 200x16px` |
| `UI-PRG-02` | Progress Bar (fill) | 200×16 | Orange gradient fill on track | `filled progress bar, orange gradient fill #E8852A, glowing edge` |
| `UI-PRG-03` | Circular Progress | 64×64 | Ring progress indicator with arc fill | `circular progress ring, dark track with orange arc fill #E8852A, 64x64px` |

### B. Main HUD Elements

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `HUD-BAR-01` | Bottom Resource Bar | 1920×80 | Horizontal bar spanning screen bottom with resource counter slots | `horizontal HUD bar, dark gunmetal #1A1D21, resource counter slots, orange accents` |
| `HUD-RSC-01` | Construction Material Icon + Counter | 120×40 | Hammer/building icon with number display area | `resource counter slot, construction material icon, hammer symbol, dark panel` |
| `HUD-RSC-02` | Minerals Icon + Counter | 120×40 | Crystal/mineral icon with number display | `resource counter slot, mineral crystal icon, dark panel #1A1D21` |
| `HUD-RSC-03` | Energy Icon + Counter | 120×40 | Lightning bolt icon with energy value | `resource counter slot, energy lightning bolt icon, orange glow accent` |
| `HUD-RSC-04` | Survival Supplies Icon + Counter | 120×40 | Shield/box icon for survival supplies count | `resource counter slot, survival shield icon, dark panel military style` |
| `HUD-RSC-05` | Dark Matter Crystals Icon | 120×40 | Purple crystal icon for elite-tier resources | `resource counter slot, purple dark matter crystal icon #A855F7` |
| `HUD-BAR-02` | Energy Bar Display | 300×40 | Green/orange energy output vs consumption bar | `energy bar display, green fill #22C55E on dark track, orange border, 300x40px` |
| `HUD-MAP-01` | Minimap Container | 300×300 | Dark panel with subtle grid overlay for minimap area | `minimap container, square dark panel #242830, subtle grid pattern, 300x300px` |
| `HUD-MAP-02` | Minimap Border Glow | 300×300 | Orange edge highlight frame for minimap border | `minimap orange border glow frame, #E8852A glowing edge, square 300x300px` |
| `HUD-TIM-01` | Real-Time Clock & Simulation Speed | 180×40 | Elapsed mission time with paused/1×/2× simulation state; never a turn counter | `real-time mission clock and simulation speed display, dark panel, orange live status, white elapsed time, 180x40px` |
| `HUD-PNL-01` | Unit Info Panel (left side) | 250×400 | Left-side vertical panel for selected unit health/stats/abilities | `vertical unit info panel, dark gunmetal #1A1D21, health bar slot, ability icon slots` |
| `HUD-PNL-02` | Building Info Panel (right side) | 250×400 | Right-side vertical panel for building production queue/status | `vertical building info panel, dark steel #242830, production queue display` |
| `HUD-HLP-01` | Help/Info Button | 50×50 | Question mark button for tooltip/help system | `help button icon, question mark symbol, orange glow border, 50x50px` |
| `HUD-PSE-01` | Pause Button Icon | 50×50 | Two vertical bars (pause symbol) on dark panel | `pause button, two vertical bars symbol, dark circular panel, orange accent` |
| `HUD-QBL-01` | Bottom Quick-Build Tray | 900×96 | Centered bottom tray whose populated building slots are supplied dynamically | `bottom quick-build tray, modular slot rail, dark gunmetal, orange active edge, 900x96px` |
| `HUD-QBL-02` | Quick-Build Slot (normal) | 80×80 | Empty reusable building shortcut slot; icon and hotkey are runtime layers | `quick-build building slot, dark steel, reinforced corners, 80x80px` |
| `HUD-QBL-03` | Quick-Build Slot (selected) | 80×80 | Selected shortcut slot with orange readiness border | `quick-build building slot selected, orange border glow, dark steel, 80x80px` |

### C. Minimap Detail Elements

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `MMI-CUR-01` | Minimap Cursor/Reticle | 24×24 | Crosshair cursor for minimap positioning | `minimap crosshair reticle, orange + symbol #E8852A, 24x24px` |
| `MMI-ZOM-01` | Zoom Level Indicator | 80×30 | Current zoom percentage display badge | `zoom level indicator badge, dark panel with white percentage text, 80x30px` |
| `MMi-RAD-01` | Radar Sweep Frame | 64×64 | Single frame of radar sweep arc effect reference | `radar sweep arc segment, orange glow #E8852A on dark background, 64x64px` |
| `MMI-BLS-01` | Selection Box Outline | N/A | Rectangular selection outline style reference (engine-drawn) | `rectangular selection box outline, dashed orange line #E8852A` |

---

## Phase 1 — Interactive Menus (P1)

**Priority:** Generate second — needed for player interaction with game systems  
**Asset Count:** ~52 assets

### D. Building Construction Menu

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `BLD-MNU-01` | Building Menu Container | 400×600 | Main panel with building category tabs and scrollable list area | `building menu container, vertical panel with category tab slots, dark gunmetal #1A1D21` |
| `BLD-CAT-01` | Power Generation Category Header | 350×40 | "Power" tab with lightning bolt icon | `category header tab, power generation label, orange top border when active, 350x40px` |
| `BLD-CAT-02` | Resource Extraction Category Header | 350×40 | "Extraction" tab with drill/mining icon | `category header tab, resource extraction label, dark steel background #242830` |
| `BLD-CAT-03` | Infrastructure Category Header | 350×40 | "Infrastructure" tab with building/house icon | `category header tab, infrastructure label, military-style panel design` |
| `BLD-CAT-04` | Storage Category Header | 350×40 | "Storage" tab with container/crate icon | `category header tab, storage label, dark panel with orange accent line` |
| `BLD-CAT-05` | Production Facilities Category Header | 350×40 | "Production" tab with factory gear icon | `category header tab, production facilities label, industrial design language` |
| `BLD-CAT-06` | Defense Structures Category Header | 350×40 | "Defense" tab with shield icon | `category header tab, defense structures label, armored panel aesthetic` |
| `BLD-CAT-07` | Support Buildings Category Header | 350×40 | "Support" tab with medical/cross icon | `category header tab, support buildings label, clean functional design` |
| `BLD-CRD-01` | Building Card (normal) | 150×160 | Reusable building thumbnail card matching the categorized construction grid | `building card normal, dark gunmetal thumbnail frame, clipped corners, label rail, 150x160px` |
| `BLD-CRD-02` | Building Card (selected) | 150×160 | Selected building card with blue tactical border | `building card selected, blue tactical border glow, dark thumbnail viewport, 150x160px` |
| `BLD-CRD-03` | Building Card (unavailable) | 150×160 | Locked or unaffordable building card with dim overlay | `building card unavailable, dim gunmetal frame, lock status area, 150x160px` |
| `BLD-INF-01` | Building Cost Tooltip Panel | 300×100 | Small panel showing construction cost breakdown | `cost tooltip panel, compact dark panel #2E323A, resource icon slots, orange text for costs` |
| `BLD-INF-02` | Building Power Requirement Indicator | 200×40 | Energy consumption display bar with +/- values | `power requirement indicator, energy bar style, negative value in red/orange, 200x40px` |
| `BLD-PLC-02` | Valid Placement Highlight | 128×128 | Green glow square for valid build position on grid | `valid placement highlight, green glow border #22C55E, transparent center, 128x128px` |
| `BLD-PLC-03` | Invalid Placement Highlight | 128×128 | Red glow square for invalid build position on grid | `invalid placement highlight, red glow border #EF4444, transparent center, 128x128px` |
| `BLD-ACT-01` | Build Action Button | 200×60 | Orange primary BUILD control used during placement | `build action button, orange industrial control, hammer icon, BUILD label, 200x60px` |
| `BLD-ACT-02` | Rotate Action Button | 220×60 | Dark secondary ROTATE control with rotation symbol | `rotate action button, dark gunmetal control, rotate icon, ROTATE label, 220x60px` |
| `BLD-ACT-03` | Cancel Placement Button | 140×50 | Secondary CANCEL control for leaving placement mode | `cancel placement button, dark outlined tactical control, CANCEL label, 140x50px` |

### E. Unit Panel & Selection

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `UNT-PNL-01` | Unit Info Panel Container | 300×500 | Main unit information panel with health/shield/ability sections | `unit info panel container, vertical dark panel #1A1D21, health bar area, ability icon grid` |
| `UNT-HLP-01` | Health Bar (full) | 250×20 | Green health bar with border outline | `health bar full state, green fill #22C55E, dark track background, 250x20px` |
| `UNT-HLP-02` | Health Bar (low - yellow) | 250×20 | Yellow health bar for low HP warning state | `health bar low state, yellow fill #F59E0B, warning indicator, 250x20px` |
| `UNT-HLP-03` | Health Bar (critical - red) | 250×20 | Red pulsing health bar for critical HP danger state | `health bar critical state, red fill #EF4444, pulsing glow effect, 250x20px` |
| `UNT-SHD-01` | Shield Bar (full) | 250×20 | Blue shield bar with border outline | `shield bar full state, blue fill #3B82F6, dark track background, 250x20px` |
| `UNT-MOR-01` | Morale/Energy Bar | 250×20 | Orange morale indicator bar | `morale energy bar, orange fill #E8852A, dark track, 250x20px military style` |
| `UNT-ABN-01` | Ability Icon Slot (normal) | 64×64 | Dark square slot for unit ability icon | `ability icon slot, dark square panel #2E323A with gray border, 64x64px` |
| `UNT-ABN-02` | Ability Icon Slot (active/ready) | 64×64 | Blue glow border indicating ready-to-use ability | `ability icon slot active state, blue glow border #3B82F6, dark center, 64x64px` |
| `UNT-ABN-03` | Ability Icon Slot (cooldown) | 64×64 | Grayed out with countdown overlay for cooling ability | `ability icon slot cooldown state, grayed out #3D424D, dark overlay, 64x64px` |
| `UNT-ABN-04` | Ability Icon Slot (unlocked) | 64×64 | Orange glow border for newly unlocked ability | `ability icon slot new unlock state, orange glow border #E8852A, 64x64px` |
| `UNT-SPD-01` | Speed Indicator Icon | 32×32 | Boot/propeller icon for movement speed stat | `speed indicator icon, boot/propeller symbol, minimalist line art, 32x32px` |
| `UNT-RNG-01` | Range Indicator Icon | 32×32 | Crosshair/target icon for attack range stat | `range indicator icon, crosshair target symbol, military UI style, 32x32px` |
| `UNT-DEF-01` | Defense Value Icon | 32×32 | Shield icon for armor value stat | `defense shield icon, armored shield symbol, minimalist, 32x32px` |
| `UNT-DMG-01` | Damage Value Icon | 32×32 | Sword/explosion icon for attack power stat | `damage icon, sword explosion symbol, aggressive design, 32x32px` |

### F. Dialog & Notification System

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `DLG-BX-01` | Standard Dialog Box | 500×300 | Main dialog/popup container with title bar and content area | `dialog box container, dark panel #1A1D21, orange top divider line, close button slot, 500x300px` |
| `DLG-BTN-01` | Dialog Confirm Button | 200×45 | Green/orange confirm/accept button for dialog actions | `dialog confirm button, orange background #E8852A, white text, rounded corners, 200x45px` |
| `DLG-BTN-02` | Dialog Cancel Button | 200×45 | Gray cancel/dismiss button for dialog actions | `dialog cancel button, gray outline border #3D424D, dark fill, 200x45px` |
| `NTF-POP-01` | Notification Popup (info) | 400×80 | Blue-accented info notification toast | `notification popup info state, blue left accent bar #3B82F6, dark panel #2E323A, 400x80px` |
| `NTF-POP-02` | Notification Popup (success) | 400×80 | Green-accented success notification toast | `notification popup success state, green left accent bar #22C55E, dark panel, 400x80px` |
| `NTF-POP-03` | Notification Popup (warning) | 400×80 | Orange-accented warning notification toast | `notification popup warning state, orange left accent bar #E8852A, dark panel, 400x80px` |
| `NTF-POP-04` | Notification Popup (error) | 400×80 | Red-accented error notification toast | `notification popup error state, red left accent bar #EF4444, dark panel, 400x80px` |
| `NTF-BNR-01` | Achievement Banner | 600×150 | Large banner for achievement/unlock notifications | `achievement unlock banner, orange glow border #E8852A, dark background, trophy icon slot, 600x150px` |
| `NTF-BNR-02` | Research Complete Banner | 600×150 | Special blue banner for research completion | `research complete banner, blue glow accent #3B82F6, dark panel, science icon slot, 600x150px` |
| `NTF-BNR-03` | Alert/Invasion Banner | 600×150 | Red pulsing banner for enemy threats/alerts | `alert invasion banner, red pulsing glow #EF4444, dark background, warning icon slot, 600x150px` |
| `TOO-TIP-01` | Tooltip Container | 250×80 | Small hover tooltip panel with arrow pointer | `tooltip container, compact dark panel #2E323A, subtle border, small text area, 250x80px` |

---

## Phase 2 — Full-Screen Modals (P2)

**Priority:** Generate third — needed for deeper game systems  
**Asset Count:** ~72 assets

### G. Tech Tree & Research Screen

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `TRC-MNU-01` | Tech Tree Container Panel | 1200×800 | Main tech tree browsing panel with zoomable canvas area | `tech tree container, large dark panel #1A1D21, node connection grid pattern, 1200x800px` |
| `TRC-NOD-01` | Research Node (available) | 200×150 | Clickable research node with orange border glow | `research node available state, dark panel #242830, orange border glow #E8852A, 200x150px` |
| `TRC-NOD-02` | Research Node (locked) | 200×150 | Grayed out node with lock icon overlay | `research node locked state, gray panel #3D424D, lock icon overlay, dimmed appearance` |
| `TRC-NOD-03` | Research Node (in progress) | 200×150 | Blue glow with circular progress indicator overlay | `research node in-progress state, blue glow border #3B82F6, progress ring overlay, 200x150px` |
| `TRC-NOD-04` | Research Node (completed) | 200×150 | Green border with checkmark indicator | `research node completed state, green border #22C55E, checkmark overlay, 200x150px` |
| `TRC-LVL-01` | TIR Level Indicator 1 | 100×40 | "TIR 1" label with basic gray styling | `TIR level indicator tier 1, basic gray styling, dark panel, 100x40px` |
| `TRC-LVL-02` | TIR Level Indicator 2 | 100×40 | "TIR 2" label with orange accent | `TIR level indicator tier 2, orange accent #E8852A, dark panel, 100x40px` |
| `TRC-LVL-03` | TIR Level Indicator 3 | 100×40 | "TIR 3" label with blue glow | `TIR level indicator tier 3, blue glow #3B82F6, dark panel, 100x40px` |
| `TRC-LVL-04` | TIR Level Indicator 4 | 100×40 | "TIR 4" label with purple glow | `TIR level indicator tier 4, purple glow #A855F7, dark panel, 100x40px` |
| `TRC-LVL-05` | TIR Level Indicator 5 | 100×40 | "TIR 5" label with elite black/void styling | `TIR level indicator tier 5 elite, purple void glow #A855F7, dark panel, 100x40px` |
| `TRC-REQ-01` | Research Requirement Display | 250×60 | Shows prerequisite research nodes and dependencies | `research requirement display, dark panel with prerequisite icon slots, orange connector lines, 250x60px` |
| `TRC-CST-01` | Research Cost Display | 250×60 | Shows resource cost breakdown for research | `research cost display, dark panel with resource icon slots and number areas, 250x60px` |

### H. Resource Management Screen

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `RSM-MNU-01` | Resource Management Container | 800×600 | Main resource overview panel with storage grid area | `resource management container, dark panel #1A1D21, storage grid layout slots, 800x600px` |
| `RSM-STO-01` | Storage Facility Slot (empty) | 200×150 | Empty storage slot with border outline | `storage slot empty state, dark panel #2E323A with gray border, 200x150px` |
| `RSM-STO-02` | Storage Facility Slot (full) | 200×150 | Filled storage slot with resource icon and quantity | `storage slot full state, resource icon area, quantity number display, dark panel, 200x150px` |
| `RSM-STO-03` | Storage Facility Slot (warning) | 200×150 | Orange border for near-capacity storage warning | `storage slot warning state, orange border glow #E8852A, dark panel, 200x150px` |
| `RSM-CNV-01` | Resource Converter Panel | 400×300 | Input/output resource selection area for converter building | `resource converter panel, two-slot layout with conversion arrow between them, dark panels, 400x300px` |
| `RSm-INP-01` | Converter Input Slot | 100×100 | Drag-and-drop input area for resource conversion | `converter input slot, square drop zone, orange dashed border #E8852A, dark center, 100x100px` |
| `RSm-OUT-01` | Converter Output Slot | 100×100 | Drag-and-drop output area for conversion result | `converter output slot, square drop zone, green dashed border #22C55E, dark center, 100x100px` |
| `RSm-CNV-02` | Conversion Arrow/Indicator | 64×64 | Orange arrow showing resource conversion direction | `conversion directional arrow, orange #E8852A, minimalist design, 64x64px` |
| `RSM-TRD-01` | Trade Route Panel | 500×200 | Shows active trade routes between locations | `trade route panel, dark panel with route line slots and location markers, 500x200px` |
| `RSm-TRD-02` | Trade Route Icon (active) | 64×64 | Truck/ship icon for active trade route status | `active trade route icon, transport vehicle symbol, green accent #22C55E, 64x64px` |
| `RSm-TRD-03` | Trade Route Icon (blocked) | 64×64 | Red X over trade route icon for blocked routes | `blocked trade route icon, red X overlay on transport symbol, 64x64px` |

### I. Map & Exploration Screen

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `MAP-MNU-01` | Full Map Container Panel | 1200×800 | Main map browsing panel with terrain display area | `map container panel, dark frame #1A1D21, terrain display area, coordinate grid overlay, 1200x800px` |
| `MAP-DUN-01` | Dungeon Marker (undiscovered) | 48×48 | Gray question mark marker for undiscovered dungeons | `dungeon marker undiscovered, gray question mark ? symbol on dark circle, 48x48px` |
| `MAP-DUN-02` | Dungeon Marker (discovered) | 48×48 | Orange exclamation marker for discovered dungeons | `dungeon marker discovered, orange exclamation ! symbol on dark circle, 48x48px` |
| `MAP-DUN-03` | Dungeon Marker (completed) | 48×48 | Green checkmark marker for completed dungeons | `dungeon marker completed, green checkmark ✓ symbol on dark circle, 48x48px` |
| `MAP-RSC-01` | Resource Node Marker | 32×32 | Small crystal/mineral icon marker on map | `resource node marker, small crystal mineral icon, orange accent #E8852A, 32x32px` |
| `MAP-BSE-01` | Base Location Marker | 48×48 | Orange triangle base/location icon on map | `base location marker, orange triangle ▲ symbol, military map style, 48x48px` |
| `MAP-ENE-01` | Enemy Unit Marker (detected) | 32×32 | Red diamond enemy indicator on map | `enemy unit marker detected, red diamond ◆ symbol, hostile indicator, 32x32px` |
| `MAP-ENE-02` | Friendly Unit Marker | 32×32 | Blue square friendly force icon on map | `friendly unit marker, blue square ■ symbol, allied force indicator, 32x32px` |
| `MAP-SNC-01` | Scan Cooldown Indicator | 64×64 | Circular radar sweep animation reference for scanner | `scan cooldown circular indicator, radar sweep arc, orange glow #E8852A, dark center, 64x64px` |
| `MAP-ZNE-01` | Biome Zone Label Background | 150×30 | Dark label background for biome name text display | `biome zone label background, dark rounded rectangle #2E323A, subtle border, 150x30px` |
| `MAP-LAY-01` | Map Layer Toggle (terrain) | 48×48 | Terrain/map icon for layer toggle button | `map layer toggle terrain, topographic map icon, dark panel background, 48x48px` |
| `MAP-LAY-02` | Map Layer Toggle (resources) | 48×48 | Resource/crystal overlay icon for layer toggle | `map layer toggle resources, crystal mineral cluster icon, orange accent, 48x48px` |
| `MAP-LAY-03` | Map Layer Toggle (units) | 48×48 | Unit/soldier overlay icon for layer toggle | `map layer toggle units, soldier figure icon, blue accent #3B82F6, 48x48px` |

### J. Solar System Overview Screen

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `SSY-MNU-01` | Solar System Container Panel | 1920×1080 | Full-screen space background panel frame with UI chrome | `solar system container, full-screen space UI frame, dark gunmetal #1A1D21 chrome borders, 1920x1080px` |
| `SSY-BGR-01` | Deep Space Background | 1920×1080 | Starfield with subtle nebula effect reference image | `deep space starfield background, subtle nebula effect, dark cosmic scene, 1920x1080px cinematic` |
| `SSY-PLT-01` | Planet Sphere (default) | 400×400 | Main colony planet sphere reference for AI generation | `planet sphere render, rocky colony world surface, atmospheric rim glow, 400x400px isolated` |
| `SSY-PLT-02` | Planet Sphere (selected/highlighted) | 400×400 | Orange glow ring around planet for selected state | `planet sphere with orange selection glow ring #E8852A, atmospheric highlight, 400x400px` |
| `SSY-PLT-03` | Planet Sphere (locked/unavailable) | 400×400 | Gray/dimmed planet with lock icon overlay | `planet sphere locked state, grayed out dimmed appearance, lock icon overlay, 400x400px` |
| `SSY-PRC-01` | Planet Progress Ring | 450×450 | Circular progress indicator around planet for travel/colonization | `planet progress ring, circular arc progress indicator, orange glow #E8852A, 450x450px` |
| `SSy-NOD-01` | Solar System Node (planet) | 200×200 | Clickable planet node in system overview view | `solar system clickable planet node, dark circular panel with planet icon area, orange border glow, 200x200px` |
| `SSy-INF-01` | Planet Info Panel | 300×200 | Shows planet name, biome type, available resources list | `planet info display panel, dark panel #1A1D21 with text line slots and resource icon row, 300x200px` |
| `SSy-LNC-01` | Landing Zone Marker | 64×64 | Orange circle target marker for landing zone selection | `landing zone marker, orange circular target #E8852A, crosshair center, 64x64px` |
| `SSY-DST-01` | Distance Indicator | 150×40 | Shows travel time/distance between planetary bodies | `distance indicator panel, dark narrow panel with number display area and unit label slot, 150x40px` |
| `SSy-DUN-01` | Dungeon Proximity Marker | 48×48 | Small dungeon icon near planet showing nearby dungeons | `dungeon proximity marker, small question mark icon on dark circle, orange accent, 48x48px` |

### K. Pause/Strategy Menu Screen

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `PSM-MNU-01` | Pause Menu Container | 800×600 | Centered overlay panel when game is paused | `pause menu container, centered dark panel #1A1D21 with semi-transparent backdrop, action button slots, 800x600px` |
| `PSM-RES-01` | Resume Game Button | 300×60 | Large green/orange resume/continue button | `resume game button, large orange gradient #E8852A to #F59E3F, white bold text, 300x60px` |
| `PSM-BSE-01` | Base Overview Button | 300×50 | Opens base management/screen overview panel | `base overview button, dark panel with blue accent border #3B82F6, 300x50px` |
| `PSM-PRD-01` | Production Queue Panel | 400×300 | Shows active production orders and queue items | `production queue panel, dark panel with item slot rows, orange header divider, 400x300px` |
| `PSm-PRD-02` | Production Item Slot (queued) | 200×60 | Unit/building currently in production queue | `production queue item slot, dark panel #2E323A with icon area and progress bar, 200x60px` |
| `PSm-PRD-03` | Production Item Slot (building) | 200×60 | Currently constructing building display | `production building slot, dark panel with construction progress indicator, orange fill bar, 200x60px` |
| `PSm-PRD-04` | Cancel Production Button | 100×40 | Red X button for canceling a queue item | `cancel production button, red X icon #EF4444 on dark panel, 100x40px compact` |
| `PSM-ACC-01` | Accelerate Build Button | 120×40 | Speed-up button with resource cost display | `accelerate build button, orange accent #E8852A with cost number area, 120x40px` |
| `PSM-OVR-01` | Production Overview Panel | 500×400 | Full production status overview with all facilities | `production overview panel, large dark panel grid layout, facility status indicators, 500x400px` |
| `PSM-STP-01` | Stop All Production Button | 200×50 | Red stop button for halting all production | `stop all production button, red background #EF4444, white text, warning style, 200x50px` |

### L. Loading & Transition Screens

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `LDR-SCR-01` | Loading Screen Background | 1920×1080 | Dark industrial scene with progress bar area at bottom | `loading screen background, dark industrial sci-fi scene, gunmetal tones #1A1D21, cinematic composition, 1920x1080px` |
| `LDR-PRG-01` | Loading Progress Bar | 600×30 | Wide orange progress bar for loading screen | `loading progress bar, wide dark track #3D424D with orange fill #E8852A, glowing edge, 600x30px` |
| `LDR-TIP-01` | Tip/Hint Text Area | 500×60 | Background panel for loading screen tips and hints | `loading tip text area background, dark narrow panel #2E323A with subtle border, 500x60px` |
| `MNU-Main-01` | Main Menu Background | 1920×1080 | Colony scene reference for main menu title placement | `main menu background, colony sci-fi scene at dusk, dark industrial aesthetic, cinematic lighting, 1920x1080px` |

### M. Faction Selection Screen

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `FCM-SCT-01` | Faction Selection Container | 1200×800 | Main faction picker panel with card grid area | `faction selection container, dark panel #1A1D21 with card grid layout slots, title bar area, 1200x800px` |
| `FCM-CRD-01` | Dark Realistic Faction Card | 400×500 | Dark military faction card front reference image | `faction card design, dark military aesthetic, gunmetal steel #1A1D21, orange accent details, 400x500px portrait` |
| `FCM-CRD-02` | Bright Realistic Faction Card | 400×500 | Clean white faction card front reference image | `faction card design, clean bright aesthetic, white painted steel #E0E3E8, blue accents, 400x500px portrait` |
| `FCM-CRD-03` | Neon Punk Faction Card | 400×500 | Neon-accented faction card front reference image | `faction card design, neon punk aesthetic, black background with cyan/magenta neon glow accents, 400x500px portrait` |
| `FCM-CRD-04` | Cartoon SciFi Faction Card | 400×500 | Bright colorful faction card front reference image | `faction card design, cartoon sci-fi aesthetic, bright saturated colors, playful shapes, 400x500px portrait` |
| `FCM-SCT-02` | Selected Faction Highlight | 400×500 | Orange glow border frame for selected faction card | `selected faction highlight frame, orange glowing border #E8852A, dark center, 400x500px` |

### N. Inventory & Equipment Screen

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `INV-MNU-01` | Inventory Container Panel | 600×500 | Main inventory grid panel with equipment slots area | `inventory container panel, dark panel #1A1D21 with grid slot layout, category tabs at top, 600x500px` |
| `INV-SLT-01` | Inventory Slot (empty) | 64×64 | Dark empty slot with subtle border outline | `inventory slot empty state, dark square #2E323A with gray border #3D424D, 64x64px grid` |
| `INV-SLT-02` | Inventory Slot (filled) | 64×64 | Filled slot with item icon area and subtle glow | `inventory slot filled state, dark square with item icon area, subtle orange inner glow, 64x64px` |
| `INV-SLT-03` | Inventory Slot (selected) | 64×64 | Orange glow border on currently selected inventory slot | `inventory slot selected state, bright orange border glow #E8852A, dark center, 64x64px` |
| `INV-SLT-04` | Inventory Slot (dragging) | 64×64 | Semi-transparent dragging state reference | `inventory slot drag state, semi-transparent overlay effect, ghost item appearance` |
| `INV-QTY-01` | Quantity Counter Overlay | 48×24 | Number overlay badge for bottom-right of inventory slot | `quantity counter badge, small dark rounded rectangle #1A1D21 with white number text, 48x24px` |

### O. Dialog & Notification Extended

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `TOO-TIP-02` | Tooltip Container (extended) | 250×80 | Small hover tooltip panel with directional arrow pointer | `tooltip container extended, dark charcoal panel #2E323A, subtle border, directional arrow pointer, 250x80px` |

---

## Phase 3 — Secondary Features (P3)

**Priority:** Generate last — needed for optional/late-game features  
**Asset Count:** ~45 assets

### P. Settings Menu Screen

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `SET-MNU-01` | Settings Container Panel | 800×700 | Main settings window with tabbed interface area | `settings container panel, dark panel #1A1D21 with horizontal tab bar slots and content area, 800x700px` |
| `SET-TAB-01` | Settings Tab - Graphics | 150×40 | Graphics settings tab label | `settings tab graphics, dark panel with orange top border when selected, 150x40px` |
| `SET-TAB-02` | Settings Tab - Audio | 150×40 | Audio settings tab label | `settings tab audio, dark panel with speaker icon area, 150x40px` |
| `SET-TAB-03` | Settings Tab - Controls | 150×40 | Keybinds/controls settings tab | `settings tab controls, dark panel with keyboard icon area, 150x40px` |
| `SET-TAB-04` | Settings Tab - Gameplay | 150×40 | Gameplay options settings tab | `settings tab gameplay, dark panel with gamepad icon area, 150x40px` |
| `SET-TAB-05` | Settings Tab - Accessibility | 150×40 | Accessibility options settings tab | `settings tab accessibility, dark panel with accessibility icon area, 150x40px` |
| `SET-TAB-06` | Settings Tab - About | 150×40 | Game info/credits/about tab | `settings tab about, dark panel with info icon area, 150x40px` |
| `SET-GRP-01` | Graphics Quality Preset (Low) | 120×40 | Low settings preset button | `graphics quality preset low button, dark panel with "LOW" label, gray accent, 120x40px` |
| `SET-GRP-02` | Graphics Quality Preset (Medium) | 120×40 | Medium settings preset button | `graphics quality preset medium button, dark panel with "MEDIUM" label, blue accent, 120x40px` |
| `SET-GRP-03` | Graphics Quality Preset (High) | 120×40 | High settings preset button | `graphics quality preset high button, dark panel with "HIGH" label, orange accent, 120x40px` |
| `SET-GRP-04` | Graphics Quality Preset (Ultra) | 120×40 | Ultra settings preset button | `graphics quality preset ultra button, dark panel with "ULTRA" label, purple glow #A855F7, 120x40px` |
| `SET-VOL-02` | Master Volume Slider | 200×30 | Full master volume control slider | `master volume slider, horizontal track dark #3D424D with orange fill #E8852A and handle, 200x30px` |
| `SET-VOL-03` | Music Volume Slider | 200×30 | Music track volume control slider | `music volume slider, horizontal dark track with blue fill #3B82F6 and handle, 200x30px` |
| `SET-VOL-04` | SFX Volume Slider | 200×30 | Sound effects volume control slider | `SFX volume slider, horizontal dark track with green fill #22C55E and handle, 200x30px` |
| `SET-VOL-05` | Voice Volume Slider | 200×30 | Dialogue/voice volume control slider | `voice volume slider, horizontal dark track with orange fill #E8852A and handle, 200x30px` |
| `SET-RES-01` | Resolution Selector Dropdown | 200×40 | Screen resolution dropdown menu control | `resolution selector dropdown, dark panel with text display area and chevron arrow, 200x40px` |
| `SET-UPS-01` | Upscaling Method Selector | 200×40 | FSR/DLSS/TSR upscaling method dropdown | `upscaling method selector, dark panel dropdown with method label area, 200x40px` |
| `SET-FPS-01` | FPS Limit Selector | 150×40 | Frame rate limit selector control | `FPS limit selector, dark panel with number options display, 150x40px` |
| `SET-LNG-01` | Language Selector Dropdown | 200×40 | Game language selection dropdown menu | `language selector dropdown, dark panel with language code area and chevron, 200x40px` |

### Q. Multiplayer & Social UI

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `MLT-CHT-01` | Chat Box Container | 400×200 | Main chat window panel with message list and input area | `chat box container, dark panel #1A1D21 with message scroll area and text input slot at bottom, 400x200px` |
| `MLT-CHT-02` | Chat Message Bubble (friendly) | 350×40 | Light gray message bubble for friendly/ally chat messages | `chat message bubble friendly, light charcoal panel #3D424D with rounded corners, text area, 350x40px` |
| `MLT-CHT-03` | Chat Message Bubble (enemy/allied) | 350×40 | Orange-tinted message bubble for enemy or other faction chat | `chat message bubble other faction, orange-tinted panel #2E323A with orange left accent, 350x40px` |
| `MLT-CHT-04` | Chat Input Field | 350×35 | Text input field for typing chat messages | `chat input text field, dark narrow panel #2E323A with cursor indicator line, 350x35px` |
| `MLT-PRT-01` | Party Member Slot (online) | 200×60 | Online party member display with avatar and status | `party member slot online, dark panel with circular avatar area, green status dot, name text line, 200x60px` |
| `MLT-PRT-02` | Party Member Slot (offline) | 200×60 | Grayed out offline party member display | `party member slot offline, grayed out panel #3D424D with dimmed avatar area, 200x60px` |
| `MLT-RQT-01` | Trade Request Panel | 350×200 | Incoming trade offer notification panel | `trade request panel, dark panel with offer detail slots and accept/cancel button areas, 350x200px` |
| `MLt-ALL-01` | Alliance Status Indicator | 150×40 | Alliance relationship status display badge | `alliance status indicator, narrow dark panel with relationship icon area and text line, 150x40px` |

### R. Victory/Defeat & End Game Screens

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `VCT-SCR-01` | Victory Screen Background | 1920×1080 | Orange/gold themed victory scene reference image | `victory screen background, orange and gold themed cinematic scene, triumphant atmosphere, 1920x1080px` |
| `DFT-SCR-01` | Defeat Screen Background | 1920×1080 | Dark red themed defeat/destruction scene reference image | `defeat screen background, dark red and charcoal themed scene, destruction atmosphere, 1920x1080px` |
| `VCT-STT-01` | Victory Statistics Panel | 500×300 | Stats summary panel for victory screen display | `victory statistics panel, dark panel #1A1D21 with orange accent border, stat line slots, 500x300px` |
| `DFT-STT-01` | Defeat Statistics Panel | 500×300 | Stats summary panel for defeat screen display | `defeat statistics panel, dark panel #1A1D21 with red accent border, stat line slots, 500x300px` |
| `VCT-BTN-01` | Continue/Next Mission Button | 250×50 | Orange/gold continue button for post-victory flow | `continue next mission button, orange-gold gradient #E8852A to #F59E3F, white bold text, 250x50px` |
| `DFT-BTN-01` | Retry Button | 250×50 | Red retry button for post-defeat restart | `retry button, red background #EF4444 with white text, warning style, 250x50px` |

### S. Loading Screen Extended

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
Reuse `LDR-SCR-01` as the loading-screen background; it is intentionally not exported twice.

### T. Dropship Component & Repair View

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `SHP-PNL-01` | Dropship Component Overview Panel | 520×420 | Side-view component inspection panel with runtime attachment zones | `dropship component overview panel, dark tactical inspection frame, modular attachment zones, 520x420px` |
| `SHP-SIL-01` | Dropship Silhouette Viewport | 420×220 | Transparent framed viewport for the rendered dropship side view | `dropship side-view viewport frame, dark gunmetal, engineering grid, transparent center, 420x220px` |
| `SHP-CMP-01` | Component Hitbox (normal) | 96×64 | Neutral component selection overlay placed over the ship render | `ship component hitbox normal, subtle steel outline, transparent center, 96x64px` |
| `SHP-CMP-02` | Component Hitbox (selected) | 96×64 | Blue selected component overlay | `ship component hitbox selected, blue tactical outline, transparent center, 96x64px` |
| `SHP-CMP-03` | Component Hitbox (damaged) | 96×64 | Red damaged component overlay with warning corners | `ship component hitbox damaged, red warning outline, transparent center, 96x64px` |
| `SHP-HP-01` | Per-Component Health Bar | 140×14 | Health bar bound independently to each ship component | `per-component health bar, compact segmented tactical bar, 140x14px` |
| `SHP-RPR-01` | Repair Cost Row — Construction Material | 300×48 | The only routine repair-cost row; construction material icon and runtime value | `repair cost row, construction material hammer icon, one value slot only, dark panel, 300x48px` |
| `SHP-RPR-02` | Repair Action Button | 180×48 | Executes the selected component repair in real time | `repair action button, orange industrial control, reinforced corners, 180x48px` |

### U. Real-Time Dungeon Route View

| Asset ID | Name | Size | Description | AI Prompt Keyword |
|----------|------|------|-------------|-------------------|
| `DUN-PNL-01` | Dungeon Room Route Panel | 900×160 | Horizontal real-time route from entry through rooms to blueprint objective | `dungeon room route panel, horizontal tactical route, dark gunmetal, 900x160px` |
| `DUN-ROM-01` | Dungeon Room Node (unknown) | 96×72 | Unexplored room node with concealed contents | `unknown dungeon room node, dim steel outline, concealed interior, 96x72px` |
| `DUN-ROM-02` | Dungeon Room Node (current) | 96×72 | Current hack-and-slay combat room with orange live border | `current dungeon combat room node, orange live border glow, 96x72px` |
| `DUN-ROM-03` | Dungeon Room Node (cleared) | 96×72 | Cleared room node with green status edge | `cleared dungeon room node, green confirmation edge, 96x72px` |
| `DUN-LNK-01` | Dungeon Room Route Connector | 80×8 | Directional connector between consecutive real-time rooms | `dungeon route connector, narrow directional tactical line, 80x8px` |
| `DUN-BLU-01` | Blueprint Objective Node | 120×88 | Final abandoned-structure objective; blueprint remains hidden until cleared | `blueprint objective dungeon node, locked technical schematic slot, orange objective corners, 120x88px` |
| `DUN-OBJ-01` | Blueprint Recovery Panel | 360×120 | Completion panel revealing the recovered blueprint after the route is cleared | `blueprint recovered panel, technical schematic display slot, orange and blue accents, 360x120px` |

---

## UI Asset Summary by Priority

| Phase | Category Range | Asset Count | Description |
|-------|---------------|:-----------:|-------------|
| **P0** | A–C, T–U | 72 | Global elements, RTS HUD, minimap, dropship inspection, dungeon route |
| **P1** | D–F | 43 | Building menu, unit panel, dialogs/notifications |
| **P2** | G–O | 74 | Tech tree, resources, map, solar system, pause, loading, faction and inventory |
| **P3** | P–S | 33 | Settings, multiplayer, end-game and shared loading references |
| **TOTAL** | All | **222** | Complete Stil-1 production UI asset list |

---

## AI Image Generation Prompts

### Master Prompt Template for UI Panels/Controls

Use this prompt as the base for all UI panel and control asset generation:

```
Dark realistic hard sci-fi UI element, flat 2D design, military tactical display aesthetic,
gunmetal dark gray color scheme (#1A1D21), subtle grid line pattern overlay on backgrounds,
clean geometric edges with precise corners, orange accent border glow (#E8852A) on active elements,
high resolution product mockup style, isolated on transparent background (green screen #00FF00 for extraction),
no text, no 3D perspective, flat orthographic view, AAA game UI concept art,
Unreal Engine UMG render quality, consistent 2-3px stroke weight.
```

### Prompt Template for UI Icons

```
Dark realistic hard sci-fi UI icon, flat 2D minimalist line art style,
orange accent color (#E8852A) and white/light gray primary lines on dark background (#1A1D21),
consistent monoline stroke weight 2-3px, military interface aesthetic,
isolated on transparent background (green screen #00FF00 for extraction),
clean geometric shapes, game UI asset quality, grid-aligned composition.
```

### Prompt Template for Building Icons (Front View)

```
Dark realistic hard sci-fi RTS building front-view icon, flat orthographic front elevation (0°),
AAA game UI icon style, centered composition on dark gunmetal background (#1A1D21),
orange accent border glow (#E8852A), no perspective distortion,
readable silhouette from small size (128x128px final),
isolated building icon for RTS construction menu,
no terrain, no characters, no text, no UI elements.
```

### Prompt Template for Backgrounds/Screens

```
Dark realistic hard sci-fi game screen background, cinematic composition,
industrial military aesthetic with gunmetal tones (#1A1D21),
subtle atmospheric lighting, high resolution concept art quality,
isolated scene reference without UI overlays, AAA game production quality.
```

### Generation Specifications

| Asset Type | Resolution | Background Key | Extraction |
|-----------|-----------|----------------|------------|
| **Panel Containers** | 512×512 | Green screen (#00FF00) | Extract to RGBA PNG |
| **Button States** (each state) | 256×128 | Green screen (#00FF00) | Extract to RGBA PNG |
| **Icon Spritesheets** | 512×512 | Green screen (#00FF00) | Extract, crop to 32×32px grid |
| **Building Icons (front view)** | 256×256 each | Dark #1A1D21 background | Direct save, no extraction needed |
| **Background Textures** | 2048×2048 | Seamless tile pattern | Direct save |
| **Planet/Solar System** | 1920×1080 or 400×400 | Transparent or space bg | Validate alpha edges |

### Color Extraction Validation Rules

- Generate panels on **green screen (#00FF00)** for clean alpha extraction
- Use **magenta screen (#FF00FF)** if asset contains orange elements that must be preserved
- After extraction, verify no green spill on orange border pixels
- Check thin geometry edges (icon lines, borders) for matte holes
- Target output: true RGBA PNG with no visible chroma artifacts

---

## See Also

- [Stil-1 Style Guide](./Style/Stil-1/STYLE_GUIDE.md) — Building asset style reference
- [Stil-1 UI Theme](../Style/Stil-1/UI_THEME.md) — Complete UI design specifications (colors, typography, panels, buttons)
- [Planet Buildings Asset List](./Building_AssetList.md) — Building asset inventory
- [Unit Asset List](./Unit_AssetList.md) — Portrait, ability, order, and minimap content inventory
- [Environment Asset List](./Environment_AssetList.md) — Biome, terrain, resource and weather inventory

---

*End of Stil-1 UI Asset List v1.0*
