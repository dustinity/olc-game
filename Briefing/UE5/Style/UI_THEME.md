# Stil-1 UI Theme — Dark Realistic

## Dark Realistic Hard Sci-Fi User Interface Design

**Version:** 1.0  
**Last Updated:** 2026-07-07  
**Category:** Dark Military-Industrial Aesthetic  
**Faction:** [Dark Realistic](../../factions/README.md)  
**Related Documents:** [STYLE_GUIDE.md](./STYLE_GUIDE.md) (3D assets) | [SHARED_UI.md](../../../Assets/Style/SHARED_UI.md) (faction-agnostic contract)

---

# Table of Contents

1. [UI Design Philosophy](#ui-design-philosophy)
2. [Color System](#color-system)
3. [Typography](#typography)
4. [Panel & Container Styles](#panel--container-styles)
5. [Button System](#button-system)
6. [Icon System](#icon-system)
7. [Progress & Status Indicators](#progress--status-indicators)
8. [Screen-Specific Guidelines](#screen-specific-guidelines)
9. [AI Image Generation Guide for UI Assets](#ai-image-generation-guide-for-ui-assets)
10. [UI Asset Inventory Reference](#ui-asset-inventory-reference)
11. [Unreal Engine Implementation](#unreal-engine-implementation)

---

## UI Design Philosophy

### Core Principles

The Stil-1 UI complements the dark realistic hard sci-fi aesthetic without competing with gameplay elements. Every UI element must:

1. **Be functional over decorative** — Every pixel serves a purpose
2. **Match the building color palette** — Gunmetal, charcoal, dark steel
3. **Use consistent accent colors** — Orange/amber for primary actions, blue for interactive, green for positive, red for danger
4. **Maintain readability at all sizes** — RTS games require quick glances at information
5. **Feel like part of the game world** — UI elements look like actual in-game displays/hardware

### What The UI Feels Like

- Military-grade tactical display
- Functional command center interface
- Dark but not oppressive — informative and empowering
- Clean engineering aesthetic
- Not dystopian HUD — actively helpful and clear

## Gameplay Presentation Rules

- **Real-time game:** Base control is real-time RTS; dungeons are real-time hack-and-slay. Never use turn counters, end-turn buttons, action points, or turn-based travel estimates.
- **Time display:** Show elapsed mission time and the current simulation state (`PAUSED`, `1×`, `2×`).
- **Contextual resources:** Resource counters are modular. Show only resources that are currently discovered and available; collapse unavailable counters without leaving empty gaps.
- **Repairs:** A normal repair transaction has one cost row: **Construction Material**. Other resources belong to construction, crafting, refueling, upgrades, or full component replacement.
- **Dungeon progress:** Show the traversable room sequence in real time. An abandoned structure is unknown until explored; completing its route awards or reveals the blueprint.

---

## Color System

### Primary Background Colors

| Color Name | Hex | RGB | Usage |
|-----------|-----|-----|-------|
| Gunmetal Black | #1A1D21 | 26, 29, 33 | Main background panels |
| Dark Steel | #242830 | 36, 40, 48 | Secondary backgrounds |
| Charcoal Gray | #2E323A | 46, 50, 58 | Tertiary backgrounds, tooltips |
| Medium Gray | #3D424D | 61, 66, 77 | Panel borders, dividers |
| Light Steel | #5A6070 | 90, 96, 112 | Disabled text, inactive elements |

### Accent Colors

| Color Name | Hex | RGB | Usage |
|-----------|-----|-----|-------|
| Orange Primary | #E8852A | 232, 133, 42 | Primary buttons, active states, warnings |
| Orange Glow | #F59E3F | 245, 158, 63 | Hover states, highlights |
| Orange Dark | #C47020 | 196, 112, 32 | Pressed buttons, active borders |
| Blue Primary | #3B82F6 | 59, 130, 246 | Interactive elements, selections, info |
| Blue Glow | #60A5FA | 96, 165, 250 | Hover on interactive elements |
| Green Primary | #22C55E | 34, 197, 94 | Positive status, health full, success |
| Yellow Warning | #F59E0B | 245, 158, 11 | Low resources, caution states |
| Red Danger | #EF4444 | 239, 68, 68 | Damage alerts, destructive actions, enemy |
| Purple Elite | #A855F7 | 168, 85, 247 | Dark matter/elite tier indicators |

### Text Colors

| Color Name | Hex | RGB | Usage |
|-----------|-----|-----|-------|
| White Primary | #FFFFFF | 255, 255, 255 | Main text, headers |
| Light Gray | #E0E3E8 | 224, 227, 232 | Secondary text, labels |
| Medium Gray | #9CA3AF | 156, 163, 175 | Tertiary text, hints, placeholders |
| Dim Gray | #6B7280 | 107, 114, 128 | Disabled text, locked states |

### Color Usage Rules

```
┌──────────────────────────────────────────────────────────┐
│  COLOR USAGE REFERENCE                                   │
├──────────────┬───────────────────────────────────────────┤
│ Element      │ Color                                     │
├──────────────┼───────────────────────────────────────────┤
│ Background   │ Gunmetal Black (#1A1D21)                  │
│ Panel Border │ Medium Gray (#3D424D) with Orange glow    │
│ Primary Btn  │ Orange Primary (#E8852A)                  │
│ SecondaryBtn │ Blue outline (#3B82F6)                    │
│ Danger Btn   │ Red Danger (#EF4444)                      │
│ Active Tab   │ Orange top border + White text            │
│ Inactive Tab │ Dark Steel bg + Light Gray text           │
│ Health Bar   │ Green → Yellow → Red (by percentage)      │
│ Progress     │ Orange fill on Medium Gray track          │
│ Tooltip Bg   │ Charcoal Gray (#2E323A)                   │
│ Disabled     │ Dim Gray (#6B7280)                        │
└──────────────┴───────────────────────────────────────────┘
```

---

## Typography

### Font Recommendations

| Role | Font | Weight | Size | Color |
|------|------|--------|------|-------|
| **Header/Title** | Orbitron, Rajdhani Bold | 700 | 28-36px | White Primary |
| **Panel Title** | Roboto Medium, Inter SemiBold | 500-600 | 18-22px | White Primary |
| **Body Text** | Roboto Regular, Inter Regular | 400 | 14-16px | Light Gray |
| **Label/Caption** | Roboto Medium, Inter SemiBold | 500-600 | 11-12px (UPPERCASE) | Medium Gray |
| **Numbers/Stats** | Orbitron, Rajdhini Bold | 700 | 18-24px | Orange Primary / White |
| **Tooltip Text** | Roboto Regular | 400 | 12-13px | Light Gray |

### Typography Rules

- Use **uppercase** for labels and category headers
- Use **bold** for numerical values and important stats
- Keep body text at minimum 14px for readability at RTS distances
- Line height: 1.4-1.5 for body text, 1.2 for labels
- Text shadow or outline on all UI text for contrast against any background

### Font Pairing Example

```
┌─────────────────────────────────────────────┐
│  COMMAND CENTER          [×] [_]            │  ← Orbitron Bold 28px
├─────────────────────────────────────────────┤
│  PRODUCTION QUEUE                             │  ← Roboto Medium 16px
│                                             │
│  ████████████████░░░░░░  73%                │  ← Numbers: Orbitron Bold 18px
│  Tank Assembly - Batch 3                      │  ← Body: Roboto Regular 14px
│                                             │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  │
│  │ [Resume] │  │ [Cancel] │  │[Accelerate]│ │  ← Button text: Roboto Medium 14px
│  └──────────┘  └──────────┘  └──────────┘  │
└─────────────────────────────────────────────┘
```

---

## Panel & Container Styles

### Standard Panel Types

#### Type A: Primary Panel (Rounded Corners)

```
┌──────────────────────────────────┐
│ ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ │  Background: Gunmetal Black
│ ▓  [Content Area]               ▓ │  Border: 2px Medium Gray
│ ▓                               ▓ │  Border Glow: Orange (active)
│ ▓  ─────────────────────────── ▓ │  Corner Radius: 8px
└──────────────────────────────────┘
```

| Property | Value |
|----------|-------|
| Background | #1A1D21 with 95% opacity |
| Border | 2px solid #3D424D |
| Border Glow | Orange (#E8852A) at 30% opacity when active |
| Corner Radius | 8px |
| Inner Shadow | Yes, 4px downward, black at 20% |
| Padding | 16px standard |

#### Type B: Angular Panel (Military Style)

```
╱▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓╲
│ [Content Area]                   │  Background: Dark Steel
│ ────────────────────────────── │  Border: 2px Medium Gray
│                                │  Corner Style: Chamfered top-left
└────────────────────────────────┘  Border Glow: Orange (active)
```

| Property | Value |
|----------|-------|
| Background | #242830 with 95% opacity |
| Border | 2px solid #3D424D |
| Corner Style | Chamfered top-left corner (16px cut) |
| Usage | Military-themed panels, defense buildings |

#### Type C: Compact Panel (Tooltips/Info)

```
┌────────────────────────────┐
│ ▓  Small info text here   ▓ │  Background: Charcoal Gray #2E323A
└────────────────────────────┘  Border: 1px solid #3D424D
                                Corner Radius: 4px
```

| Property | Value |
|----------|-------|
| Background | #2E323A with 98% opacity |
| Border | 1px solid #3D424D |
| Corner Radius | 4px |
| Max Width | 300px |
| Padding | 8px |

### Panel Header Styles

```
┌──────────────────────────────────┐
│  PANEL TITLE          [×] [_]   │  ← Title: White, Left-aligned
├──────────────────────────────────┤  ← Divider: 1px Orange (#E8852A)
│                                  │
│  Content...                      │
└──────────────────────────────────┘
```

| Property | Value |
|----------|-------|
| Title Color | #FFFFFF, Bold |
| Divider Line | 1px solid #E8852A at 60% opacity |
| Close Button | Orange X icon, 32×32px hit area |
| Minimize Button | Orange underscore icon, 32×32px hit area |

---

## Button System

### Primary Button

```
┌──────────────────────┐
│   BUTTON LABEL       │  ← Background: #E8852A (Orange)
└──────────────────────┘  ← Text: #FFFFFF (White), Bold, 14px
                           ← Padding: 12px vertical, 24px horizontal
                           ← Corner Radius: 4px

[Hover]
┌──────────────────────┐
│   BUTTON LABEL       │  ← Background: #F59E3F (Brighter Orange)
└──────────────────────┘  ← Border: 1px solid #FFFFFF at 20%

[Pressed/Active]
┌──────────────────────┐
│   BUTTON LABEL       │  ← Background: #C47020 (Darker Orange)
└──────────────────────┘  ← Inner shadow for pressed effect

[Disabled]
┌──────────────────────┐
│   BUTTON LABEL       │  ← Background: #3D424D (Gray)
└──────────────────────┘  ← Text: #6B7280 (Dim Gray), 50% opacity
```

### Button Types Reference

| Type | Normal State | Hover State | Disabled State | Usage |
|------|-------------|-------------|----------------|-------|
| **Primary** | BG: #E8852A | BG: #F59E3F | BG: #3D424D | Main actions, confirm, build |
| **Secondary** | Border: #3B82F6 | Border: #60A5FA + BG: #3B82F6 at 10% | Border: #3D424D | Cancel, back, secondary actions |
| **Danger** | BG: #EF4444 | BG: #DC2626 | BG: #3D424D | Delete, destroy, discard |
| **Success** | BG: #22C55E | BG: #16A34A | BG: #3D424D | Accept, apply, save |
| **Icon Only** | BG: #2E323A | BG: #3D424D | BG: #242830 at 50% | Close, minimize, settings |

### Button Sizes

| Size | Width | Height | Font Size | Padding | Usage |
|------|-------|--------|-----------|---------|-------|
| **Large** | Auto (min 150px) | 56px | 18px | 16px vertical | Main CTA, full-width actions |
| **Standard** | Auto (min 100px) | 44px | 14px | 12px vertical | Dialog buttons, menu actions |
| **Small** | Auto (min 70px) | 32px | 12px | 8px vertical | Inline actions, quick commands |
| **Icon** | 44px | 44px | N/A | N/A | Close, minimize, toggle icons |

---

## Icon System

### Icon Design Principles

- **Simple and readable at small sizes** — Minimum 24×24px, preferably 32×32px
- **Monoline style** — Consistent stroke weight (2-3px)
- **Filled + outline mix** — Use filled for primary actions, outline for secondary
- **Orange accent for interactive icons** — Blue for info, green for positive

### Icon Grid Reference

```
┌──────┐
│      │  ← 32×32px icon grid
│  🡒   │     (example: chevron right)
│      │
└──────┘
```

| Icon Type | Size | Style | Color | Example |
|-----------|------|-------|-------|---------|
| **Navigation** | 32×32 | Outline | Light Gray → Orange on hover | Chevrons, arrows |
| **Actions** | 32×32 | Filled | White → Orange on hover | Play, pause, stop, save |
| **Status** | 24×24 | Filled | Green (good), Red (bad), Yellow (warn) | Health, shield, energy |
| **Building** | 128×128 | Front-view illustration | As per building style guide | Construction menu icons |
| **Unit** | 64×64 | Front/side view illustration | As per unit style | Unit selection panel |

### Icon States

| State | Normal | Hover | Active | Disabled |
|-------|--------|-------|--------|----------|
| **Color** | Light Gray #E0E3E8 | Orange #F59E3F | Orange Dark #C47020 | Dim Gray #6B7280 |
| **Opacity** | 100% | 100% | 100% | 40% |

---

## Progress & Status Indicators

### Health/Resource Bars

```
[Full Health - Green]
┌──────────────────────────────┐
│ ████████████████████████░░░░ │  #22C55E (Green)
└──────────────────────────────┘  73% / 100

[Low Health - Yellow]
┌──────────────────────────────┐
│ ████████░░░░░░░░░░░░░░░░░░░░ │  #F59E0B (Yellow)
└──────────────────────────────┘  28% / 100

[Critical Health - Red]
┌──────────────────────────────┐
│ ███░░░░░░░░░░░░░░░░░░░░░░░░░ │  #EF4444 (Red, pulsing)
└──────────────────────────────┘  8% / 100
```

| State | Fill Color | Border | Animation |
|-------|-----------|--------|-----------|
| **Full (>60%)** | #22C55E Green | None | Static |
| **Warning (30-60%)** | #F59E0B Yellow | 1px solid | Static |
| **Critical (<30%)** | #EF4444 Red | 1px solid | Pulse every 2s |

### Progress Indicators

| Type | Track Color | Fill Color | Usage |
|------|------------|-----------|-------|
| **Linear (construction)** | #3D424D | #E8852A Orange | Building construction, research progress |
| **Linear (production)** | #3D424D | #3B82F6 Blue | Unit production queue |
| **Circular** | #3D424D | #E8852A Orange | Cooldown timers, scan cycles |

### Circular Progress Reference

```
┌─────────────┐
│   ░░░░░░░   │  ← Track: #3D424D ring, 4px stroke
│   ░  ●  ░   │  ← Center: Icon or percentage number
│   ░░▓▓░░░   │  ← Fill: #E8852A arc, 4px stroke
└─────────────┘  ← Size: 64×64px standard
```

---

## Screen-Specific Guidelines

### A. Main HUD (Bottom Bar)

**Layout:** 1920×1080 viewport reference

```
┌─────────────────────────────────────────────────────────────────────┐
│                                                                     │
│                     Game Viewport Area                              │
│                                                                     │
│                                                                     │
├──────────┬────────────────────────────────────────────┬─────────────┤
│ Unit     │  Resources & Energy Bar                    │ Building    │
│ Info     │                                            │ Menu        │
│ Panel    │  [⛏️ 2,450] [💎 1,200] [⚡ 35/50] [📦 80%] │ (scrollable)│
│ (left)   │                                            │ (right)     │
│          │  ────────────────────────────────────────  │             │
│          │  [Health Bar] [Shield Bar]                 │ [+ Build]   │
├──────────┴────────────────────────────────────────────┴─────────────┤
│  [Minimap 300×300]              [LIVE 1× | 00:14:32]      [Pause ‖] │
└─────────────────────────────────────────────────────────────────────┘
```

| Element | Position | Size | Background | Notes |
|---------|----------|------|------------|-------|
| Resource Counters | Top of HUD bar | 120×40 each | Transparent | Orange icons, white numbers |
| Energy Bar | Center top | 300×40 | #1A1D21 at 80% | Green fill, orange border |
| Minimap | Bottom-left | 300×300 | #242830 with grid | Orange border glow |
| Unit Info Panel | Left side | 250×400 | #1A1D21 at 95% | Scrollable if content overflows |
| Building Menu | Right side | 250×600 | #1A1D21 at 95% | Category tabs + scrollable list |
| Mission Clock | Bottom-center | 180×40 | #1A1D21 at 80% | Elapsed time plus orange pause/1×/2× state |

### B. Building Construction Menu

**Layout:** Right-side panel or center popup

```
┌──────────────────────────────┐
│  BUILD STRUCTURE        [×]  │
├──────────────────────────────┤
│ [⚡ Power] [⛏️ Extract]      │  ← Category Tabs
│ [🏠 Infra] [📦 Storage]     │
│ [🏭 Production] [🛡️ Defense] │
│                              │
│ ─────────────────────────── │
│                              │
│ ┌──────┐ ┌──────┐           │  ← Building Icons (128×128)
│ │ ☀️   │ │ 🌀   │           │
│ │Solar │ │Wind  │           │
│ │Array │ │Turbine│          │
│ └──────┘ └──────┘           │
│                              │
│ ┌──────┐ ┌──────┐           │
│ │ ⚛️  │ │ 💧  │           │
│ │Reactor│ │Turbine│         │
│ └──────┘ └──────┘           │
│                              │
│ Cost: 80 ⛏️ 60 💎          │  ← Cost Display
│ Power draw: 5/s             │  ← Continuous Power Requirement
│                              │
│  [CONSTRUCT]  [CANCEL]      │  ← Action Buttons
└──────────────────────────────┘
```

### C. Tech Tree Screen

**Layout:** Full-screen with zoomable canvas

```
┌─────────────────────────────────────────────────────────┐
│  TECHNOLOGY TREE                              [×] [_]  │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌──────┐    ┌──────┐    ┌──────┐                     │
│  │TIR-1 │───▶│TIR-2 │───▶│TIR-3 │───▶│TIR-4 │───▶│TIR-5│
│  │Basic │    │Energy│    │Dark  │    │Void  │    │Elite│
│  │Forge │    │Lab   │    │Matter│    │Lab   │    │Tier│
│  └──────┘    └──────┘    └──────┘    └──────┘    └──────┘
│     │           │           │           │           │
│     ▼           ▼           ▼           ▼           ▼
│  [Unlock]   [Unlock]    [Locked]    [Locked]    [Locked]
│                                                         │
│ Selected: Energy Lab                                    │
│ Requires: TIR-1 Basic + 200 ⛏️ + 100 💎               │
│ Bonus: Research speed +25%                             │
│                                                         │
│                    [RESEARCH]        [CANCEL]           │
└─────────────────────────────────────────────────────────┘
```

### D. Solar System Overview

**Layout:** Full-screen cinematic with planet view

```
┌─────────────────────────────────────────────────────────┐
│  SOLAR SYSTEM MAP                            [×] [_]   │
├─────────────────────────────────────────────────────────┤
│                                                         │
│                    (Planet Sphere)                       │
│                   ╭────────────╮                        │
│                  ╱   🡢 Planet  ╲                       │
│                 │    Colony     │ ← Orange glow ring    │
│                  ╰─────────────╯   when selected        │
│                     ╱         ╲                          │
│                                                         │
│  ──── Distance: 2.4 AU ──── Travel Time: 02:14:36 ────  │
│                                                         │
│  Biome: Light Snow | Resources: Iron, Crystal           │
│  Facilities: Solar Array (-30% eff), Mine (active)      │
│                                                         │
│  [LANDING ZONE]    [EXPLORE DUNGEONS]   [SCAN FOR ENEMY]│
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### E. Settings Menu

**Layout:** Centered panel with tabbed interface

```
┌──────────────────────────────────────┐
│  SETTINGS                       [×]  │
├──────────────────────────────────────┤
│ [Graphics] [Audio] [Controls] [Game] │
├──────────────────────────────────────┤
│                                      │
│ Graphics Quality:                    │
│ ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐   │
│ │ Low │ │Med  │ │High │ │Ultra│ ← Preset buttons
│ └─────┘ └─────┘ └─────┘ └─────┘   │
│                                      │
│ Resolution:        [1920×1080 ▼]    │
│ FPS Limit:         [60 / 120 / ∞]  │
│ Upscaling:         [FSR Quality ▼] │
│                                      │
│ Master Volume:    ━━━━━━━━━━░░ 75% │
│ Music Volume:     ━━━━━━━━░░░░ 60% │
│ SFX Volume:       ━━━━━━━━━━░░ 80% │
│ Voice Volume:     ━━━━━━━━━░░░ 70% │
│                                      │
│              [APPLY]    [CANCEL]    │
└──────────────────────────────────────┘
```

---

## AI Image Generation Guide for UI Assets

### Master Prompt Template for UI Backgrounds/panels

Use this prompt when generating UI background panels and container images via AI:

```
Dark realistic hard sci-fi UI panel, flat 2D design, 
military tactical display aesthetic, gunmetal dark gray color scheme (#1A1D21), 
subtle grid line pattern overlay, clean geometric edges, 
orange accent border glow (#E8852A) on active edges, 
high resolution product mockup style, isolated on transparent background,
no text, no 3D perspective, flat orthographic view,
AAA game UI concept art, Unreal Engine UMG render quality.
```

### Prompt Template for UI Icons

```
Dark realistic hard sci-fi UI icon set, flat 2D design, 
minimalist line art style, orange accent color (#E8852A) on dark background (#1A1D21),
consistent stroke weight, military interface aesthetic,
isolated icons on transparent background, white/light gray primary lines,
clean geometric shapes, game UI asset quality, 32×32px grid aligned.
```

### Prompt Template for Building Icons (Front View)

```
Dark realistic hard sci-fi RTS building front-view icon, 
flat orthographic front elevation (0°), AAA game UI icon style,
centered composition on dark gunmetal background (#1A1D21),
orange accent border glow (#E8852A), no perspective distortion,
readable silhouette from small size (128×128px),
isolated building icon for RTS construction menu,
no terrain, no characters, no text, no UI elements.
```

### UI Asset Generation Specifications

| Asset Type | Resolution | Background | Notes |
|-----------|-----------|------------|-------|
| **Panel Containers** | 512×512 | Transparent | Include border glow as separate layer |
| **Button States** | 256×128 each state | Transparent | Generate normal, hover, pressed, disabled |
| **Icon Spritesheet** | 512×512 | Transparent | 32×32px icons on 48px grid |
| **Building Icons** | 256×256 each | Dark #1A1D21 | Front view, centered, 128×128px final |
| **Background Textures** | 2048×2048 | Seamless tile | Subtle grid/metal texture |
| **Planet/Solar System** | 1920×1080 | Space background | No UI elements baked in |

### Color Extraction for Engine

When generating UI assets with AI:
- Generate on **green screen (#00FF00)** for standard panels (allows clean alpha extraction)
- Use **magenta screen (#FF00FF)** if asset contains orange elements that need preservation
- Extract to **true RGBA PNG** after generation
- Verify no green spill on orange border pixels

---

## UI Asset Inventory Reference

### Complete Asset List by Category

| Category | Asset Count | Priority | Notes |
|----------|:-----------:|----------|-------|
| A. Global UI Elements | 32 | **P0** | Core buttons, panels, controls — generate first |
| B. Main HUD | 14 | **P0** | Always-visible gameplay elements — generate first |
| C. Building Construction Menu | 26 | **P1** | Secondary menu — generate after P0 |
| D. Unit Panel & Selection | 14 | **P1** | Secondary panel — generate after P0 |
| E. Tech Tree & Research Screen | 14 | **P2** | Full-screen modal — generate later |
| F. Resource Management Screen | 11 | **P2** | Full-screen modal — generate later |
| G. Map & Exploration Screen | 13 | **P2** | Includes map markers and overlays |
| H. Solar System Overview Screen | 14 | **P2** | Cinematic full-screen, planet assets separate |
| I. Pause/Strategy Menu Screen | 12 | **P2** | Overlay modal — generate later |
| J. Settings Menu Screen | 20 | **P3** | Low priority, standard controls |
| K. Dialog & Notification System | 12 | **P1** | Popups needed early for feedback |
| L. Loading & Transition Screens | 5 | **P2** | Loading screen, main menu background |
| M. Multiplayer & Social UI | 8 | **P3** | Network-dependent features |
| N. Victory/Defeat & End Game Screens | 6 | **P3** | End-game only |
| O. Faction Selection Screen | 6 | **P2** | Needed at game start |
| P. Inventory & Equipment Screen | 9 | **P2** | Mid-game feature |
| Q. Minimap Detail Elements | 4 | **P1** | HUD-adjacent, needed early |

### Priority Generation Order

```
Phase 1 (P0) — Core Gameplay UI: ~60 assets
├── Global UI Elements (32)
├── Main HUD (14)
└── Minimap Elements (4 + 2 from other categories)

Phase 2 (P1) — Interactive Menus: ~52 assets
├── Building Construction Menu (26)
├── Unit Panel & Selection (14)
├── Dialog & Notification System (12)

Phase 3 (P2) — Full-Screen Modals: ~72 assets
├── Tech Tree & Research Screen (14)
├── Resource Management Screen (11)
├── Map & Exploration Screen (13)
├── Solar System Overview Screen (14)
├── Pause/Strategy Menu Screen (12)
├── Faction Selection Screen (6)
└── Inventory & Equipment Screen (9 - partial)

Phase 4 (P3) — Secondary Features: ~45 assets
├── Settings Menu Screen (20)
├── Loading & Transition Screens (5)
├── Multiplayer & Social UI (8)
├── Victory/Defeat Screens (6)
└── Inventory remaining (6)
```

---

## Unreal Engine Implementation

### UMG Widget Structure

Recommended widget hierarchy for Stil-1 UI:

```
Widget Blueprints:
├── WBP_Panel_Base              ← Base panel with common styles
│   ├── WBP_Panel_Primary       ← Standard rounded panel
│   ├── WBP_Panel_Angular       ← Military chamfered panel
│   └── WBP_Panel_Compact       ← Tooltip/info panel
│
├── WBP_Button_Base             ← Base button with common styles
│   ├── WBP_Button_Primary      ← Orange primary action
│   ├── WBP_Button_Secondary    ← Blue outlined
│   ├── WBP_Button_Danger       ← Red destructive
│   └── WBP_Button_Icon         ← Icon-only button
│
├── WBP_HUD_Main                ← Main gameplay HUD
│   ├── WBP_ResourceBar         ← Resource counters
│   ├── WBP_EnergyBar           ← Energy display
│   ├── WBP_Minimap             ← Minimap container
│   ├── WBP_UnitInfoPanel       ← Left-side unit info
│   └── WBP_BuildingMenu        ← Right-side build menu
│
├── WBP_BuildingConstruction    ← Construction popup
├── WBP_TechTree                ← Tech tree screen
├── WBP_ResourceManagement      ← Resource management screen
├── WBP_MapExploration          ← Map/exploration screen
├── WBP_SolarSystem             ← Solar system overview
├── WBP_PauseMenu               ← Pause/strategy menu
├── WBP_Settings                ← Settings screen
│   └── WBP_SettingsTab_Graphics
│   ├── WBP_SettingsTab_Audio
│   ├── WBP_SettingsTab_Controls
│   ├── WBP_SettingsTab_Gameplay
│   └── WBP_SettingsTab_Accessibility
│
├── WBP_DialogBox               ← Standard dialog popup
├── WBP_NotificationPopup       ← Toast notifications
├── WBP_AchievementBanner       ← Achievement unlock banner
├── WBP_FactionSelect           ← Faction selection screen
├── WBP_Inventory               ← Inventory/equipment screen
├── WBP_VictoryScreen           ← Victory/end-game screen
└── WBP_DefeatScreen            ← Defeat/end-game screen
```

### Material Parameters for Glow Effects

```
Material: MAT_UI_PanelGlow
├── Parameter: GlowIntensity (0.0 - 1.0)
│   └── Controls orange border glow brightness
├── Parameter: GlowColor (Vector3)
│   └── Default: (0.91, 0.52, 0.16) = Orange #E8852A
├── Parameter: PulseSpeed (0.0 - 5.0)
│   └── Controls pulsing animation speed (0 = static)
└── Parameter: BorderThickness (Float)
    └── Default: 2.0px

Material: MAT_UI_ProgressBar
├── Parameter: FillPercentage (0.0 - 1.0)
│   └── Controls progress fill amount
├── Parameter: FillColor (Vector3)
│   └── Dynamic: Green → Yellow → Red by percentage
├── Parameter: GlowIntensity (0.0 - 1.0)
│   └── Orange glow on filled portion
└── Parameter: PulseOnCritical (Boolean)
    └── When true, pulses red fill every 2s
```

### Animation Guidelines

| Element | Animation | Duration | Easing | Trigger |
|---------|----------|----------|--------|---------|
| **Panel Open** | Slide up + fade in | 0.3s | Ease Out Quad | On show |
| **Panel Close** | Slide down + fade out | 0.25s | Ease In Quad | On hide |
| **Button Hover** | Brightness increase | 0.15s | Linear | Mouse enter |
| **Button Press** | Scale down (95%) | 0.08s | Ease Out | Mouse down |
| **Button Release** | Scale up (100%) | 0.12s | Ease Out Quad | Mouse up |
| **Progress Fill** | Smooth fill animation | 0.5s | Ease Out Cubic | Value change |
| **Critical Pulse** | Border glow pulse | 2.0s cycle | Sine Wave | HP < 30% |
| **Notification** | Slide in from top-right | 0.4s | Ease Out Quad | New notification |
| **Notification Hide** | Fade out | 0.3s | Linear | Auto-dismiss |

### UI Scaling Guidelines for RTS Camera

```
Design Resolution: 1920×1080 (Full HD)

Minimum Text Size:
├── Body text: 14px (never smaller, unreadable at distance)
├── Labels: 11px uppercase minimum
└── Numbers/Stats: 16px bold minimum

Minimum Interactive Element Size:
├── Buttons: 80×32px minimum (for mouse)
├── Icon buttons: 44×44px minimum
└── Checkbox/Radio: 24×24px hit area

HUD Safe Zone:
├── Bottom HUD bar: 100px height maximum
├── Side panels: Maximum 300px width each side
└── Minimap: Maximum 350×350px (leaves 1220×730 for game view)

UI Scale by Screen Size:
├── 1920×1080: 100% scale (baseline)
├── 2560×1440: 125% scale
├── 3840×2160: 150% scale
└── 1366×768: 90% scale (minimum readability)
```

---

## Quick Reference Card

### Stil-1 UI in One Glance

```
┌─────────────────────────────────────────────┐
│  STYLE: Dark Realistic Hard Sci-Fi UI       │
│  ENGINE: Unreal Engine UMG                  │
│  DESIGN: Military Tactical Display          │
├─────────────────────────────────────────────┤
│  COLORS:                                    │
│  BG: Gunmetal #1A1D21                       │
│  Accent: Orange #E8852A                     │
│  Interactive: Blue #3B82F6                  │
│  Positive: Green #22C55E                    │
│  Danger: Red #EF4444                        │
├─────────────────────────────────────────────┤
│  FONTS:                                     │
│  Headers: Orbitron Bold                     │
│  Body: Roboto Regular                       │
│  Numbers: Orbitron Bold                     │
├─────────────────────────────────────────────┤
│  PANELS:                                    │
│  Rounded (8px) for standard                 │
│  Chamfered for military                     │
│  Orange border glow when active             │
├─────────────────────────────────────────────┤
│  AVOID:                                     │
│  Cartoon colors, rounded playful shapes,    │
│  Light/bright backgrounds, decorative fluff │
└─────────────────────────────────────────────┘
```

---

## Appendix: Comparison with Other UI Styles

| Feature | Stil-1 (Dark) | Stil-4 (Bright) | Neon Punk |
|---------|---------------|-----------------|-----------|
| **Background** | Gunmetal #1A1D21 | White/Light Gray | Black with neon accents |
| **Primary Accent** | Orange #E8852A | Blue #3B82F6 | Cyan/Magenta neon glow |
| **Panel Style** | Military tactical | Clean modern corporate | Cyberpunk glitch effects |
| **Font Feel** | Industrial/functional | Friendly/approachable | Edgy/futuristic |
| **Border Treatment** | Orange glow lines | Subtle gray shadows | Neon tube outlines |
| **Animation Feel** | Mechanical/hardware | Smooth/fluid | Glitch/pulse effects |

---

## See Also

- [SHARED_UI.md](../../../Assets/Style/SHARED_UI.md) — Faction-agnostic UI contract (scenes, components, layout rules)
- [STYLE_MAP.md](../../../Assets/Style/STYLE_MAP.md) — Which faction uses which visual style
- [Stil-4/UI_THEME.md](UI_THEME.md) — Bright Realistic theme
- [Stil-6/UI_THEME.md](UI_THEME.md) — Neon Punk theme

---

*End of Stil-1 UI Theme v1.0*
