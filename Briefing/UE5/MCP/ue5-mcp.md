---
name: ue5-mcp
description: Master routing index for all UE5 MCP toolsets — maps game dev tasks to the right tools and teaches agents how to discover live parameter schemas at runtime.
metadata:
  type: reference
---

# UE5 MCP Skill System — Master Index

## Purpose

This skill is your navigation map. When you're asked to build a feature, scene, or system in this UE5 game, use the routing table below to find which toolsets are relevant. Then read only those toolset summaries (not every detail) and query the live MCP server for exact parameter schemas when you start creating nodes.

**Key principle:** Skills = workflow knowledge (what to do, in what order). MCP JSON = runtime parameter discovery (exact field names, types, defaults). Never hardcode parameter details from a .md file — always check the live schema before calling tools.

## Routing Table: Game Tasks → Toolsets

### UI & Menus
| Task | Primary toolsets | Secondary toolsets |
|------|-----------------|-------------------|
| Create Widget Blueprints (menus, HUDs, panels) | `umg-toolset` | `blueprint-tools`, `asset-tools` |
| Style widgets (colors, transitions, states) | `material-tools`, `animation-tools` | `blueprint-tools` |
| Wire button actions / navigation | `blueprint-tools` | `editor-app-toolset` |
| Import UI textures with correct settings | `texture-tools` | `asset-tools` |
| Set startup widget / project config | `config-settings-toolset` | — |
| Persist settings / save data | `save-game-toolset`, `blueprint-tools` | — |
| Open levels / quit app from menus | `editor-app-toolset` | — |
| Manage input mode & cursor visibility | `editor-app-toolset` | — |

### Level & World Building
| Task | Primary toolsets | Secondary toolsets |
|------|-----------------|-------------------|
| Create levels, scenes, instances | `scene-tools` | `actor-tools`, `pcg-toolset` |
| Place actors / meshes procedurally | `pcg-toolset` | `actor-tools`, `transform-tools` |
| Modify actor properties (position, scale, rotation) | `actor-tools` | `blueprint-tools` |
| Tag actors for gameplay logic | `gameplay-tags` | `actor-tools` |

### Visual Effects & Materials
| Task | Primary toolsets | Secondary toolsets |
|------|-----------------|-------------------|
| Create materials (PBR, shaders) | `material-tools` | — |
| Niagara particle effects | `niagara-toolset` | `material-tools` |
| Sequencer cinematics / cutscenes | `sequencer-toolset`, `animation-tools` | `actor-tools` |

### Gameplay Systems
| Task | Primary toolsets | Secondary toolsets |
|------|-----------------|-------------------|
| Create Blueprint classes (game logic) | `blueprint-tools` | `editor-toolset` |
| Add events, functions, variables to Blueprints | `blueprint-tools` | — |
| Connect nodes and pins in graphs | `blueprint-tools` | — |
| Manage gameplay tags (categorization) | `gameplay-tags` | — |

### Asset Pipeline & Game Objects
| Task | Primary toolsets | Secondary toolsets |
|------|-----------------|-------------------|
| Import 3D models (FBX/OBJ) | `static-mesh-tools`, `skeletal-mesh-tools` | — |
| Create materials and material instances | `material-tools`, `material-instance-tools` | — |
| Generate LODs, collisions, Nanite | `static-mesh-tools` | — |
| Create physics assets for meshes | `physics-asset-toolset` | — |
| Import/export animations (Sequencer) | `sequencer-import-export-toolset`, `control-rig` | — |
| Create Blueprint classes with gameplay logic | `blueprint-tools` | `editor-toolset` |
| Manage data tables (unit/building definitions) | `data-table-tools`, `data-asset-tools` | — |
| Register gameplay tags for new actors | `gameplay-tags` | — |

### Asset Creation Workflow (Fast Path → Full Path)
| Task | Primary toolsets | Secondary toolsets |
|------|-----------------|-------------------|
| Placeholder: primitive shapes as temporary mesh | `primitive-tools`, `material-tools` | — |
| Upgrade placeholder to real 3D model later | `static-mesh-tools` or `skeletal-mesh-tools` | `blueprint-tools` |
| Create base Blueprint class + specific subclasses | `blueprint-tools` | `data-table-tools` |

### Procedural Generation & Systems
| Task | Primary toolsets | Secondary toolsets |
|------|-----------------|-------------------|
| Generate solar systems (planets, biomes, events) | `blueprint-tools`, `data-asset-tools` | — |
| Create dungeon layouts with rooms/enemies/loot | `blueprint-tools`, `data-table-tools` | — |
| Define biome modifiers and environmental hazards | `data-asset-tools` | — |
| Calculate travel costs between planets | `blueprint-tools` | — |
| Process loot drops and equipment rewards | `blueprint-tools`, `data-table-tools` | — |

### Save Game & Persistence
| Task | Primary toolsets | Secondary toolsets |
|------|-----------------|-------------------|
| Discover available save slots | `save-game-toolset` | — |
| Check if a save exists before loading | `save-game-toolset` | — |
| Get save metadata (timestamp, level, playtime) | `save-game-toolset` | — |
| Find latest save for "Continue" button | `save-game-toolset` | — |
| Create SaveGame Blueprint classes | `blueprint-tools` | — |

### Editor & Project Management
| Task | Primary toolsets | Secondary toolsets |
|------|-----------------|-------------------|
| Open project, inspect settings | `editor-app-toolset` | — |
| Compile Blueprints (verify no errors) | `blueprint-tools`, `umg-toolset` | — |
| Search / find actors or assets | `search-toolset`, `scene-tools` | — |
| Modify project config / ini files | `config-settings-toolset` | — |
| Manage save game slots & metadata | `save-game-toolset` | `blueprint-tools` |

## Available Skills

The `.claude/skills/unreal-engine-skills/` directory contains 27 general UE5
engine-knowledge skills (C++ foundations, GAS, AI/navigation, state trees,
Mass Entity, networking, rendering, etc.), vendored from
[quodsoler/unreal-engine-skills](https://github.com/quodsoler/unreal-engine-skills)
(MIT). These cover engine conventions and common mistakes — reach for them
when writing UE5 C++ or designing a gameplay system, as distinct from the
MCP-driving gotchas in this file or the feature specs below. Each has its own
auto-triggering `description`; see that directory's README for the full list.

**This project's actual conventions are captured in [../../.agents/ue-project-context.md](../../.agents/ue-project-context.md)**
(auto-drafted from `.uproject`/`Build.cs`/`Target.cs`/source scan, per the
`ue-project-context` skill's own workflow). Read it before applying any of
the 27 skills' generic advice — it tells you which systems (GAS, StateTree,
Mass Entity, custom log categories, etc.) actually exist in this project's
source versus which are unused/aspirational, so the skills aren't applied
against imagined code.

The `.claude/skills/rts-ui/` directory contains 16 workflow skills organized by game feature:

### UI Scenes (display layer)
| Skill | Scene Ref | What It Builds |
|-------|-----------|---------------|
| ui-component-library.md | Foundation | Shared design system — buttons, cards, progress bars, tabs, modals, tooltips, toasts, sliders |
| welcome-screen.md | M01 + M03 | Welcome screen, settings menu, first-time player flow |
| planet-overview.md | S01 | Colony stats dashboard with building markers |
| colony-management.md | S02 | Worker allocation, production priorities, storage |
| construction-mode.md | S04 | PCG grid-based building placement system |
| tech-tree.md | S05 | Radial ring research visualization UI |
| resource-hud-bar.md | S06 | Cross-cutting resource display (3 adaptive variants) |
| squad-selection.md | S07/S09 | Squad building, role assignment, dungeon entry |
| tactical-combat.md | S08 | Real-time combat with pause, formations, cover |
| combat-results.md | S10 | Loot distribution and progression after combat |
| dropship-modules.md | S13/S14 | Ship module management UI |
| galaxy-map-solar-system.md | S11/S12 | Galaxy star map + solar system orbital view UI |

### Gameplay Systems (backend logic)
| Skill | What It Builds |
|-------|---------------|
| asset-creation-pipeline.md | Creating new units/buildings from scratch (fast path → full path) |
| unit-upgrade-equipment.md | Unit TIR upgrades, equipment loadouts, loot rarity system |
| solar-system-generation.md | Procedural solar systems — planets, biomes, events, travel routes |
| dungeon-generation.md | Dungeon layouts, enemy composition, loot tables, boss phases |

### Master Index (this file)
Maps game dev tasks to MCP toolsets and teaches agents how to discover live parameter schemas at runtime.

## MCP Discovery Guide

When you need exact parameter details for any tool, follow this pattern:

1. **Find the toolset** — Use the routing table above to identify which toolset(s) apply.
2. **Read the toolset summary** — Open `.claude/skills/ue5-mcp/toolsets/<toolset-name>.md` (or `UE5/MCP/toolsets/`) for a high-level overview of what tools are available and their purpose.
3. **Query the live schema** — For each specific tool you'll call, read its JSON schema from `.claude/skills/ue5-mcp/toolsets/<toolset-name>-<ToolName>/` or `UE5/MCP/toolsets/<toolset>/<tool-name>.json`. This gives you exact parameter names, types, required fields, and defaults.
4. **Build the call** — Use the schema to construct your tool invocation with correct parameter names and types.

### Example: Creating a Widget Blueprint

```
1. Routing table → UI & Menus → "Create Widget Blueprints" → umg-toolset
2. Read umg-toolset.md → see 23 tools, find Create_widget_blueprint
3. Read the JSON schema for that tool → get exact parameters (refPath, widgetName, parentClass, etc.)
4. Call the tool with correct parameters
```

### Example: Adding an Event to a Blueprint

```
1. Routing table → Gameplay Systems → "Add events" → blueprint-tools
2. Read blueprint-tools.md → find Add_event tool
3. Read add_event.json schema → get blueprint.refPath, event_name, position.x/y
4. Call the tool with correct parameters
```

## Workflow Patterns

### Always Compile After Changes
After creating or modifying any Blueprint (widget, game logic, subsystem), always compile it to catch errors before proceeding. A failed compile blocks all downstream work.

### Bottom-Up Widget Creation
For UI widgets: create reusable components first (buttons, sliders, tabs), then assemble them into larger containers and root widgets last. This mirrors how you'd build in the editor.

### Always Verify in PIE
After building a widget or scene, verify it works in Play In Editor before moving to the next component. Catch layout issues early.

### Reduce Motion Respect
Always check for `UUserSettings::bReduceMotion` and disable animations/transitions when enabled. This is a UE5 accessibility requirement.

## Toolset Quick Reference

| Toolset | File | Tools | Purpose |
|---------|------|-------|---------|
| blueprint-tools | `UE5/MCP/toolsets/blueprint-tools.md` | ~53 | Create/modify Blueprints, graphs, events, variables, nodes, pins |
| umg-toolset | `UE5/MCP/toolsets/umg-toolset.md` | ~23 | UMG Widget Blueprint creation and modification |
| static-mesh-tools | `UE5/MCP/toolsets/static-mesh-tools.md` | ~16 | Import meshes, LODs, collisions, Nanite control |
| skeletal-mesh-tools | `UE5/MCP/toolsets/skeletal-mesh-tools.md` | ~22 | Import skeletal meshes, bones, LODs, morph targets, sockets |
| material-tools | `UE5/MCP/toolsets/material-tools.md` | ~22 | Material creation, expressions, connections, recompilation |
| material-instance-tools | `UE5/MCP/toolsets/material-instance-tools.md` | ~13 | Create material instances, set parameters |
| physics-asset-toolset | `UE5/MCP/toolsets/physics-asset-toolset.md` | 17 | Physics assets from meshes (bodies, constraints) |
| primitive-tools | `UE5/MCP/toolsets/primitive-tools.md` | 4 | Add cube/sphere/cylinder/cone primitives as placeholders |
| data-table-tools | `UE5/MCP/toolsets/data-table-tools.md` | ~10 | Create/import DataTables with schema and rows |
| data-asset-tools | `UE5/MCP/toolsets/data-asset-tools.md` | 1 | Create DataAsset definitions |
| gameplay-tags | `UE5/MCP/toolsets/gameplay-tags-toolset.md` | 6 | Tag management for actor categorization |
| pcg-toolset | `UE5/MCP/toolsets/pcg-toolset.md` | ~30 | Procedural content generation nodes and graphs |
| asset-tools | See Index.md | — | Asset import, directory management |

## When to Read More

- **First time building in a domain** → read the full toolset summary for that domain
- **Building something you've done before** → skim the routing table, go straight to JSON schemas
- **Stuck on an error** → check the toolset summary for common patterns and gotchas
- **Need to discover a new capability** → search the Index.md for relevant keywords
