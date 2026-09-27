# UI Scenes & Game Loops Overview

> **Purpose:** Map every game loop to the UI scenes required, cross-referenced with approved mockups.
> This is the master reference for frontend planning — every scene here becomes a work package.

---

## Game Loops

The game has **three primary loops** that players cycle through at different frequencies:

| Loop | Frequency | Description |
|------|-----------|-------------|
| **Strategic Loop** | Minutes-hours | Colony management, resource production, research, diplomacy — the "4X" layer |
| **Tactical Loop** | Seconds-minutes | Dungeon exploration, unit deployment, real-time combat encounters |
| **Travel Loop** | Minutes | Dropship management, solar system navigation, galaxy hopping |

### Strategic Loop (Core)

```
Gather Resources → Build/Upgrade Structures → Research Tech → Expand Colonies → Repeat
```

The core loop from [Core Loop](../Core-Loop/README.md): explore planets, extract resources, build colonies, research technologies, and expand influence across the galaxy. This is a **hybrid idle-RTS** — buildings auto-produce and store resources passively; players check in periodically to collect, assign priorities, and make strategic decisions.

**Game Tick:** Tied to the in-game day-night cycle. Each tick = `x%` of one full day cycle (configurable globally). This means production rates scale with day length and can be tuned server-side without changing per-building values. The morning/midday/evening/night sub-loops from Core Loop are driven by this tick system.

### Tactical Loop (Dungeons)

As described in [Dungeons](../Dungeons/Dungeon-Overview/README.md): deploy units into dungeon zones, engage real-time combat, extract resources/artifacts, return to colony. **Not turn-based** — real-time with pause capability.

**Squad Composition:**
- **Squad size:** 16 units maximum per dungeon run.
- **Single Player:** Full freedom to select any combination of units from your roster, including your champion (main character selected at game start).
- **Co-op:** Both players contribute to the 16-unit squad. Each player's champion is automatically included when they accept a dungeon invite. Both players can suggest unit picks — collaborative squad building.

**Pause Mechanics:**
- **Single Player:** Click-to-pause directly (pause-anytime RTS style).
- **Co-op:** Either player requests a pause; the other receives a prompt to confirm. Combat freezes pending confirmation.

### Travel Loop (Space Navigation)

From [Space Travel](../Space-Travel/README.md): manage dropship modules, navigate between solar systems, traverse the galaxy map, handle ship repairs and upgrades between destinations.

### Loop Transitions — The Dropship Hub

The **crashed dropship** serves as the central navigation hub on every planet surface:

- **Click the dropship** → radial menu with action icons appears:
  - **Repair / Customize** → opens S13 (Dropship Repair View) and S14 (Ship Module Management)
  - **Navigate to Planet** → opens S11 (Solar System View) for intra-system travel
  - **Zoom Out** → scrolls out from solar system view to S12 (Galaxy Map) for inter-system travel

This is a *diegetic* navigation system — no abstract menus, the ship itself is your portal between loops. Breadcrumb hierarchy: Galaxy → Solar System → Planet → Dropship Hub.

---

## UI Scenes by Game Loop

### 1. Strategic Loop Scenes

| # | Scene | Purpose | Mockup Reference | Status |
|---|-------|---------|-----------------|--------|
| S01 | **Planet Overview** | View colony stats, resource production, population, building overview | `07-Planet-Overview-View.png` | Mockup exists |
| S02 | **Colony Management** | Manage buildings, assign workers, set production priorities | `08-Colony-Management-View.png` | Mockup exists |
| S03 | **RTS Base View** | Top-down isometric view of colony, unit placement, building overview | `03-RTS-Base-View.png` | Approved mockup |
| S04 | **Construction Mode** | Build new structures with placement preview (green glow for valid zones) | `04-RTS-Construction-Mode.png` | Approved mockup — love the green glow placement system |
| S05 | **Tech Tree / Research** | Anno 1800-style tree — independent branches converge at intersections to unlock higher TIR tiers (see Tech Tree spec below) | No mockup yet | Needs design — see Tech Tree spec below |
| S06 | **Resource HUD Bar** | Persistent resource display — adaptive placement & style per scene type (from [Resources/Types](../../resources/README.md)) | No mockup yet | Needs design — see Resource HUD spec below |

### 2. Tactical Loop Scenes

| # | Scene | Purpose | Mockup Reference | Status |
|---|-------|---------|-----------------|--------|
| S07 | **Dungeon Entry** | Select dungeon zone, view difficulty, preview rewards, deploy units | No mockup yet | Needs design |
| S08 | **Tactical Combat View** | Real-time combat in dungeon — unit control, health bars, ability hotbar | `05-Tactical-Combat-Dungeon-View.png` | Approved mockup — visual style reference, but NOT turn-based (real-time) |
| S09 | **Unit Selection / Squad** | Choose which units to deploy before entering a dungeon (from [Units](../../units/README.md)) | No mockup yet | Needs design |
| S10 | **Combat Results** | Show loot, XP gained, unit losses, artifacts found | No mockup yet | Needs design |

### 3. Travel Loop Scenes

| # | Scene | Purpose | Mockup Reference | Status |
|---|-------|---------|-----------------|--------|
| S11 | **Solar System View** | Navigate within a solar system — select planets, stations, jump points | `09-Solar-System-UI.png` | Approved mockup — really nice layout |
| S12 | **Galaxy Map** | High-level galaxy view with solar systems as nodes, warp routes | `10-Galaxy-System-UI.png` | Approved mockup — good but solar systems should be more spread out |
| S13 | **Dropship Repair View** | Repair and maintain ship modules (from [Dropship](../../Spaceship/Dropship/README.md)) | `06-Dropship-Repair-View.png` | Approved mockup — needs blueprint-style 2-column layout: available modules → ship hull |
| S14 | **Ship Module Management** | Attach/detach modules from [Ship Modules](../../ShipModules/README.md), manage module TIR upgrades | No mockup yet | Needs design (extend of S13) |

### 4. Meta / Onboarding Scenes

| # | Scene | Purpose | Mockup Reference | Status |
|---|-------|---------|-----------------|--------|
| M01 | **Welcome / Login Screen** | Game entry, account selection, last session summary | `01-Game-Welcome-Screen.png` | Approved mockup |
| M02 | **Main Menu (In-Game)** | Pause menu — resume, settings, galaxy map, colony view, exit | No mockup yet | Needs design |
| M03 | **Settings / Options** | Graphics, audio, controls, UI scaling | No mockup yet | Needs design |

---

## Approved Mockups Reference

Visual style guide: [Stil-1 UI Theme](../../../Assets/Stil-1/UI_THEME.md) | Faction-agnostic contract: [SHARED_UI.md](../../../Assets/Style/SHARED_UI.md)

| File | Scene | Notes |
|------|-------|-------|
| [`01-Game-Welcome-Screen.png`](../../../Assets/Stil-1/View/Mockups/01-Game-Welcome-Screen.png) | M01 Welcome | Approved — clean, atmospheric entry screen |
| [`03-RTS-Base-View.png`](../../../Assets/Stil-1/View/Mockups/03-RTS-Base-View.png) | S03 Colony Base View | Approved — isometric colony overview |
| [`04-RTS-Construction-Mode.png`](../../../Assets/Stil-1/View/Mockups/04-RTS-Construction-Mode.png) | S04 Construction Mode | **Favorite feature** — building preview with green glow highlighting valid placement zones |
| [`05-Tactical-Combat-Dungeon-View.png`](../../../Assets/Stil-1/View/Mockups/05-Tactical-Combat-Dungeon-View.png) | S08 Tactical Combat | Visual style reference only — dungeons should LOOK like this but combat is real-time, NOT turn-based |
| [`06-Dropship-Repair-View.png`](../../../Assets/Stil-1/View/Mockups/06-Dropship-Repair-View.png) | S13 Ship Repair | Good base — needs blueprint-style 2-column layout: left = available modules, right = ship hull with attachment points |
| [`09-Solar-System-UI.png`](../../../Assets/Stil-1/View/Mockups/09-Solar-System-UI.png) | S11 Solar System | **Really nice** — clean orbital layout |
| [`10-Galaxy-System-UI.png`](../../../Assets/Stil-1/View/Mockups/10-Galaxy-System-UI.png) | S12 Galaxy Map | Good concept — solar systems should be more spread out for better readability |

---

## Scenes Still Needing Mockups (Priority Order)

| Priority | Scene | Why It's Needed |
|----------|-------|----------------|
| **P0** | S05 Tech Tree (Anno-style convergence tree) | Bottom-up growth, branch intersections enforce breadth across categories |
| **P0** | S07 Dungeon Entry + S09 Squad Selection | Gateway to tactical loop — unit deployment is critical UX |
| **P1** | S06 Resource HUD Bar (3 variants) | Adaptive per scene group — strategic top bar, tactical bottom/side, travel compact corner |
| **P1** | S14 Ship Module Management | Extends the repair view with module attachment blueprint |
| **P2** | S10 Combat Results | Post-combat feedback — loot, XP, losses |
| **P2** | M02 Main Menu (In-Game) | Navigation hub between all loops |
| **P3** | M03 Settings | Standard but low-priority for gameplay planning |

---

## Resource HUD Bar — Adaptive Placement by Scene Type

A persistent resource bar is always visible but adapts its **position** and **visual style** to the active scene context. Six core resources displayed: Energy, Construction Material, Fuel, Minerals, Survival, Hull Parts. Each shows icon + current stock + production rate (±/tick). Clicking a resource expands a detail panel.

| Scene Group | Placement | Style Rationale |
|-------------|-----------|-----------------|
| **Strategic** (S01-S06: Colony views, construction, tech tree) | Top bar, full-width | Out of the way of isometric base view below; matches standard RTS convention |
| **Tactical** (S07-S10: Dungeon entry, combat, results) | Bottom bar or side panel | Keep top area free for unit health bars / ability hotbar. Compact icons only in combat — full detail on pause |
| **Travel** (S11-S14: Solar system, galaxy map, ship repair) | Top-left corner, compact | Minimal footprint; navigation maps need maximum screen real estate. Fuel and Energy highlighted (travel-relevant resources) |

---

## Tech Tree Visualization — Anno-Style Convergence Tree (S05)

**Reference:** Anno 1800 tech tree — organic, bottom-up growth with branch intersections.

### Structure

```
                    [TIR 5 - Void Lab] ← final convergence
                   /          |         \
              [Weapons V]  [Armor V]  [Drives Outer Ring]
                 /|            |           |\
                / |            |            |
      ┌────────┘  │    ────────┼────────────┤──  ← TIR 4 intersection
      |           │     (need Weapons III + Armor III)
   [Wpn III]  [Vision III]   [Armor III]
       |           |              |
       |           |              |
    ┌──┴──┐    ┌───┴────┐    ┌───┴──┐
 [Wpn II] [Vis II]  [Armor II] [Energy II]   ← TIR 3 intersection
     │       │          │          │
     │       │          │          │
     └──┬────┘          │          │
        │               │          │
    [Wpn I]         [Armor I]   [Drives Ring 1]    ← TIR 2, independent starts
        │               │          │
        └───────────────┴──────────┘
                    [TIR 1 - Forge]  ← single root
```

### Rules

- **Bottom-up growth:** Start at a single root node (Forge / TIR 1). Research unlocks nodes upward.
- **Independent branches early:** At TIR 2, multiple categories (Armor, Weapons, Vision, Drives) branch independently — player can pick any order.
- **Convergence at intersections:** Higher tiers require completing nodes in *multiple* branches. E.g., to unlock TIR 4 you need both Weapons III AND Armor III completed. The intersection node visually shows incoming lines from each prerequisite branch.
- **No single-path speedrun:** You can't go straight up one lane to the end. Breadth across categories is required for late-game tech.
- **Visual style:** Organic tree look — nodes are circular/ornamental, connections curve like branches. Key milestones (TIR gateways) get visual emphasis (larger node, glow effect).

### Why This Fits

Maps naturally onto your ring-based research categories: each TIR tier is a "canopy layer" of the tree. The intersection mechanic enforces that players develop their colony holistically — not just maxing weapons while ignoring armor or drives.

---

## Scene → Briefing Document Mapping

Each UI scene draws data from specific briefing documents:

| Scene | Data Source(s) |
|-------|---------------|
| S01 Planet Overview | [Planet-Types](../../Planets/Planet-Types/README.md), [Planet-Events](../../Planets/Planet-Events/README.md) |
| S02 Colony Management | [Planet-Buildings](../../buildings/README.md), [Storage-System](../../buildings/Reference/Storage-System.md) |
| S03 RTS Base View | [Planet-Buildings](../../buildings/README.md), [Units](../../units/README.md) |
| S04 Construction Mode | [Planet-Buildings](../../buildings/README.md) — building costs, placement rules |
| S05 Tech Tree | [Tech-Tree/Overview](../../Tech-Tree/Overview/README.md), [Research-Categories](../../Tech-Tree/Research-Categories/README.md) |
| S06 Resource Overview | [Resources/Types](../../resources/README.md) |
| S07 Dungeon Entry | [Dungeons](../Dungeons/Dungeon-Overview/README.md) — zone difficulty, rewards |
| S08 Tactical Combat | [Dungeons](../Dungeons/Dungeon-Overview/README.md), [Units](../../units/README.md), [Weapon-Types](../../weapons/Weapon-Types/README.md) |
| S09 Squad Selection | [Units](../../units/README.md), [Upgrade-System](../../units/Upgrade-System/README.md) |
| S11 Solar System View | [Space-Travel](../Space-Travel/README.md), [Navigation](../../Planets/Navigation/README.md) |
| S12 Galaxy Map | [Space-Travel](../Space-Travel/README.md) — system connections, warp routes |
| S13 Dropship Repair | [Dropship](../../Spaceship/Dropship/README.md) — hull integrity, module status |
| S14 Ship Module Mgmt | [ShipModules](../../ShipModules/README.md), [Progression](../../Spaceship/Progression/README.md) |

---

## UI Component Inventory (from Style Guide)

Per the [SHARED_UI.md](../../../Assets/Style/SHARED_UI.md) contract, these components are needed across all factions and scenes:

| Component | Used In |
|-----------|---------|
| **Buttons** (Primary, Secondary, Ghost, Icon) | Every scene |
| **Cards/Panels** | Colony overview, resource cards, module cards |
| **Progress Bars** | Building construction, research progress, ship repair, combat health |
| **Tables/Data Grids** | Resource lists, unit rosters, tech tree nodes |
| **Tabs** | Multi-panel views (colony tabs: Buildings / Workers / Storage) |
| **Modals/Dialogs** | Construction confirmations, squad selection, settings |
| **Tooltips** | Building details, module stats, resource info |
| **Notifications/Toasts** | Resource alerts, combat events, research completion |
| **Sliders** | Settings (volume, graphics), production allocation |
| **Breadcrumbs** | Galaxy → Solar System → Planet navigation hierarchy |

---

## See Also

- [Core Loop](../Core-Loop/README.md) — Primary gameplay loop details
- [Dungeons](../Dungeons/Dungeon-Overview/README.md) — Tactical combat system
- [Space Travel](../Space-Travel/README.md) — Navigation and dropship mechanics
- [SHARED_UI.md](../../../Assets/Style/SHARED_UI.md) — Faction-agnostic UI contract (scenes, components, layout rules)
- [Stil-1/UI_THEME.md](../../../Assets/Stil-1/UI_THEME.md) — Dark Realistic theme specification
- [STYLE_MAP.md](../../../Assets/Style/STYLE_MAP.md) — Which faction uses which visual style
- [Mockups Directory](../../../Assets/Stil-1/View/Mockups) — Visual reference images (Stil-1)
