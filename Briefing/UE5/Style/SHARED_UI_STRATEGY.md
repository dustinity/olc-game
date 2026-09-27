# Shared UI Strategy

Shared UI assets define one visual language for every screen in **Our Last Chance**. Screen-specific folders may add layout assets, icons, and preview art, but they must not introduce a different UI style.

## Visual Contract

- Style: dark realistic hard sci-fi, military tactical display, functional engineering panels.
- Base colors:
  - Gunmetal black: `#1A1D21`
  - Dark steel: `#242830`
  - Charcoal gray: `#2E323A`
  - Border gray: `#3D424D`
  - Primary orange: `#E8852A`
  - Hover orange: `#F59E3F`
  - Tactical blue: `#3B82F6`
  - Valid green: `#22C55E`
  - Danger red: `#EF4444`
- Typography:
  - Headers: Orbitron Bold or Rajdhani Bold.
  - Labels: uppercase condensed bold.
  - Body text: Roboto or Inter.
  - Numbers: Orbitron/Rajdhani bold.
- Corners: angular/chamfered where possible; keep rounded corners subtle, 4-8 px max.
- Animation: short mechanical fades, glows, and snaps. No playful bounce.

## Shared Runtime Components

Create these as reusable C++/UMG components before building individual screens:

| Component | Asset IDs | Purpose |
|---|---|---|
| `WBP_UI_Frame` | `UI-PNL-*`, frame parts from welcome screen | Reusable outer frame / panel shell |
| `WBP_UI_Button` | `UI-BTN-01` to `UI-BTN-07` | Primary, secondary, danger, icon buttons |
| `WBP_UI_TabButton` | `UI-TGB-01` to `UI-TGB-03` | Settings, minimap, build menu, research categories |
| `WBP_UI_ResourceCounter` | `HUD-RSC-01` to `HUD-RSC-05` | Dynamic resource strip items |
| `WBP_UI_ProgressBar` | `UI-PRG-01` to `UI-PRG-03` | Health, repair, research, construction |
| `WBP_UI_Tooltip` | `TOO-TIP-*` | Hover details for buildings, resources, tech |
| `WBP_UI_ModalOverlay` | `UI-BG-02`, `DLG-*` | Settings, confirmations, detail panels |
| `WBP_UI_IconButton` | `UI-ICN-*`, `HUD-PSE-01`, `HUD-HLP-01` | Small action buttons |

## Global Rules

- Never show turn counters, action points, or end-turn controls.
- Simulation state may show `PAUSED`, `1x`, `2x`.
- Resource counters are dynamic. Only show resources discovered/available to the player.
- Core resources are:
  - Energy
  - Fuel
  - Construction Material
  - Minerals
  - Hull Parts
  - Survival
- Use `Hull Parts` for HUD labels. Use `Hull Materials` only in longer tooltips or documentation text.
- Routine dropship repair costs use Construction Material only.
- Minerals, Hull Parts, Fuel, Energy, Survival, Dark Matter Crystals, and blueprints can appear for crafting, upgrading, travel, survival, and strategic planning, but not as generic repair tax.
- TIR appears as a compact difficulty/tech chip on planet, dungeon, research, building, unit, weapon, and travel detail panels.
- Resource counters must support capacity pressure states: normal, approaching full, full, and overflow.
- Scan actions must show Energy cost, scan tier, scan time, and what information will be revealed.
- Biome, planet modifier, and hazard badges use the shared badge style instead of custom one-off panels.
- Faction modifiers may appear as small badges/effects, but faction themes do not change the overall Stil-1 UI language.
- Red triangles are reserved for enemy hotspots/threat markers.
- Green glow means valid build placement.
- Red glow means invalid build placement or danger.
- Orange means primary action, selected state, or live objective.
- Blue means tactical selection, scan, map, or secondary navigation.

## Mockup Policy

Do not generate a new mockup for every screen yet. First lock the shared system from the best existing mockups:

1. `01-Game-Welcome-Screen.png` for frame, menu tone, and visual density.
2. `03-RTS-Base-View.png` for main RTS HUD placement.
3. `04-RTS-Construction-Mode.png` for construction category/menu/placement behavior.
4. `08-Colony-Resource-Network.png` for top development/version/header treatment and economy overview idea.
5. `09-Solar-System-UI.png` and `10-Galaxy-System-UI.png` for map-detail panels and route planning patterns only.

Generate new mockups after the shared HUD grammar is accepted. Otherwise each generated screen will drift into a different game.
