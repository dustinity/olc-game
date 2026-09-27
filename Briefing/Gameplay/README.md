# Gameplay Systems — Master Index

All gameplay systems for *Our Last Chance*. The game is a 2.5D top-down RTS with side-view cinematic sequences, structured around three interlocking loops: **Strategic** (colony management), **Tactical** (dungeon combat), and **Travel** (space navigation).

---

## Subsystems

| Subsystem | Description |
|-----------|-------------|
| [Core Loop](./Core-Loop/README.md) | Complete progression flow from crash landing through galaxy conquest — five phases covering survival, exploration, base building, interplanetary expansion, and endgame. |
| [Dungeons](Dungeons/Dungeon-Overview/README.md) | Tactical dungeon exploration, loot tables, boss fights, and blueprint rewards. |
| [Space Travel](./Space-Travel/README.md) | Solar system navigation, wormhole transit, space events, ship-to-ship combat, in-transit minigames, and multi-planet logistics. |
| [UI Screens](./UI-Screens/README.md) | Every UI scene mapped to game loops — strategic, tactical, travel, and meta/onboarding views with mockup references and component inventory. |

---

## Three Primary Game Loops

| Loop | Frequency | Description | Key Scenes |
|------|-----------|-------------|------------|
| **Strategic** | Minutes-hours | Colony management, resource production, research, expansion — the "4X" layer | Planet Overview, Colony Management, RTS Base View, Construction Mode, Tech Tree, Resource HUD |
| **Tactical** | Seconds-minutes | Dungeon exploration, unit deployment, real-time combat encounters | Dungeon Entry, Tactical Combat View, Squad Selection, Combat Results |
| **Travel** | Minutes | Dropship management, solar system navigation, galaxy hopping | Solar System View, Galaxy Map, Dropship Repair, Ship Module Management |

The **crashed dropship** serves as the central diegetic hub between all three loops — click it to access repair, navigation, and zoom-out actions. No abstract menus; the ship itself is your portal.

---

## Game Tick System

Production and events are tied to an in-game day-night cycle. Each tick equals a configurable percentage of one full day cycle, meaning production rates scale with day length and can be tuned server-side without changing per-building values. The morning/midday/evening/night sub-loops documented in the Core Loop are driven by this tick system.

---

## Cross-System References

| Topic | Primary Briefing Location |
|-------|--------------------------|
| Planet types & biomes | [Planets](../Planets/README.md) |
| Navigation & scanning | [Planets/Navigation](../Planets/Navigation/README.md) |
| Buildings (planet surface) | [Buildings](../buildings/README.md) |
| Ship modules | [ShipModules](../ShipModules/README.md) |
| Dropship layout | [Spaceship/Dropship](../Spaceship/Dropship/README.md) |
| Ship progression | [Spaceship/Progression](../Spaceship/Progression/README.md) |
| Tech tree structure | [Tech-Tree/Overview](../Tech-Tree/Overview/README.md) |
| Research categories | [Tech-Tree/Research-Categories](../Tech-Tree/Research-Categories/README.md) |
| Resources | [Resources](../resources/README.md) |
| Units | [Units](../units/README.md) |
| Weapons | [Weapons](../weapons/README.md) |
