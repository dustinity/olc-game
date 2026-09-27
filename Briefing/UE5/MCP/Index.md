# MCP Tool Index

Overview of all available UE5 MCP toolsets, their purpose, and the tools they provide.

## Quickreference
[UE5-QUICK-REFERENCE]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/UE5-QUICK-REFERENCE.md)

## Toolsets

### Editor & Scene

| Toolset | Tools | Description |
|---------|-------|-------------|
| [ActorTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/actor-tools.md) | 17 | Get/set actor transforms, tags, labels, components, parent relationships. |
| [AssetTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/asset-tools.md) | 21 | Find, load, save, duplicate, move, delete assets; check edit state and metadata. |
| [BlueprintTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/blueprint-tools.md) | 53 | Create/compile Blueprints, manage graphs/functions/events/variables, add nodes/pins, retarget classes. |
| [CurveTableTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/curve-table-tools.md) | 9 | Create/import curve tables, add/remove/rename rows and keys. |
| [DataAssetTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/data-asset-tools.md) | 1 | Create data assets. |
| [DataTableTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/data-table-tools.md) | 10 | Create/import data tables, manage schema/rows/search structs. |
| [EditorAppToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/editor-app-toolset.md) | 21 | Control the Unreal Editor: PIE sessions, camera, viewport captures, actor/asset selection, CVar search, content browser navigation. |
| [LogsToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/logs-toolset.md) | 4 | Control UE5 log verbosity, retrieve entries, browse categories. |
| [MaterialTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/material-tools.md) | 22 | Create materials/functions/collections, manage expressions/parameters, connect/disconnect, recompile. |
| [MaterialInstanceTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/material-instance-tools.md) | 13 | Create material instances, set scalar/vector/texture/static-switch parameters. |
| [ObjectTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/object-tools.md) | 6 | Get class info, search subclasses, list/get/set/reset properties on UObjects. |
| [PrimitiveTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/primitive-tools.md) | 4 | Add primitive shapes: cube, sphere, cylinder, cone to the scene. |
| [ProgrammaticToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/programmatic-toolset.md) | 2 | Execute tool scripts and check execution environment. |
| [SceneTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/scene-tools.md) | 20 | Load levels, trace world, merge actors, manage level instances/folders/collision channels. |
| [SkeletalMeshTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/skeletal-mesh-tools.md) | 22 | Import meshes, get LOD/bone/material/morph-target info, assign physics assets, manage sockets. |
| [StaticMeshTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/static-mesh-tools.md) | 16 | Import meshes, get triangle/LOD/material info, generate/remove LODs and collisions, Nanite control. |
| [StringTableTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/string-table-tools.md) | 8 | Create/import string tables, manage keys and entries. |
| [TextureTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/texture-tools.md) | 3 | Import texture files with settings (sRGB, compression, mipmaps, filter), get dimensions.
| [ConfigSettingsToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/config-settings-toolset.md) | 8 | Manage project settings and engine ini files: startup widget, game mode, rendering presets, generic config queries.
| [SaveGameToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/save-game-toolset.md) | 5 | Discover save slots, check save existence, retrieve metadata (timestamp/level/playtime), find latest save. |

### Dataflow & Registry

| Toolset | Tools | Description |
|---------|-------|-------------|
| [DataflowAgentToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/dataflow-agent-toolset.md) | 22 | Create, modify, and manage dataflow graphs: nodes, pins, variables, templates, and asset generation. |
| [DataRegistryTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/data-registry-tools.md) | 7 | Query UE5 Data Registries at runtime: list registries, items, sources, schemas. |

### Gameplay & AI (GAS)

| Toolset | Tools | Description |
|---------|-------|-------------|
| [AbilitySystemInspector]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/ability-system-inspector-toolset.md) | 4 | Inspect GAS ability system: active effects, tags, attribute values, granted abilities. |
| [AttributeSetTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/attribute-set-toolset.md) | 2 | List attributes and find attribute set classes in the ability system. |
| [GameplayCueTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/gameplay-cue-toolset.md) | 8 | Create/find gameplay cue notifies, execute on actors, manage cue tags. |
| [BehaviorTreeTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/behavior-tree-tools.md) | 7 | Query behavior trees: root decorators, node depths, children, subtrees, blackboard data. |
| [ConversationTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/conversation-tools.md) | 7 | Navigate conversation trees: entry points, speakers, nodes, GUIDs, connections. |
| [GameFeaturesToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/game-features-toolset.md) | 7 | Activate/deactivate game feature plugins at runtime; check states and discover plugins. |
| [GameplayTagsToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/gameplay-tags-toolset.md) | 6 | Manage gameplay tags: add, remove, rename, list, query info, find asset referencers. |
| [StateTreeTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/state-tree-tools.md) | 9 | Query state tree editor data: root states, children, tasks, transitions, evaluators. |

### Niagara VFX

| Toolset | Tools | Description |
|---------|-------|-------------|
| [Niagara Assets]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/niagara-assets.md) | 3 | Find Niagara scripts, get asset discovery info and digests. |
| [Niagara Blueprint]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/niagara-blueprint.md) | 2 | Construct Niagara BP wrappers from components or systems. |
| [Niagara Component]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/niagara-component.md) | 4 | Manage Niagara component variables and system assignment at runtime. |
| [Niagara Info]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/niagara-info.md) | 1 | Enum type information for Niagara scripting. |
| [Niagara System Editor]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/niagara-system.md) | 46 | Full Niagara system editor: emitters, modules, renderers, parameters, user variables, compile state. |

### PCG & Physics

| Toolset | Tools | Description |
|---------|-------|-------------|
| [PCG]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/pcg-toolset.md) | 30 | Procedural Content Generation: add/remove/reposition nodes, connect pins, create graphs, execute instances. |
| [PCG Spatial]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/pcg-spatial-toolset.md) | 1 | Run a PCG instant graph for procedural placement. |
| [PhysicsAssetToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/physics-asset-toolset.md) | 17 | Create physics assets from meshes, manage bodies and constraints (sphere/capsule/box). |

### Plugin & World

| Toolset | Tools | Description |
|---------|-------|-------------|
| [PluginManager]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/plugin-toolset.md) | 17 | Manage plugins: create, validate names, add/remove dependencies, enable/disable, list/discover. |
| [WorldConditions]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/world-condition-tools.md) | 2 | Get descriptions for world conditions and queries in UE5. |

### UI & Search

| Toolset | Tools | Description |
|---------|-------|-------------|
| [SemanticSearch]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/semantic-search-toolset.md) | 2 | Semantic search across project assets: general search and find similar to a target. |
| [SlateInspector]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/slate-inspector-toolset.md) | 14 | Inspect and interact with Slate UI elements: click, hover, type, drag, screenshot, observe. |
| [UMGToolSet]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/umg-toolset.md) | 23 | Create/compile UMG widget Blueprints, manage widgets/named slots/events/bindings. |

### Animation (Sequencer & Control Rig)

| Toolset | Tools | Description |
|---------|-------|-------------|
| [ControlRig]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/control-rig-tools.md) | 44 | Create control rigs: graphs, bones, nulls, controls, pins, nodes, variables, transforms. |
| [Sequencer]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/sequencer-tools.md) | 140 | Full Level Sequence management: tracks, sections, bindings, folders, playback, marked frames, selection. |
| [Sequencer ControlRig]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/sequencer-control-rig-tools.md) | 72 | Key and animate control rig data in Sequencer: snap/tween/blending, bake, FBX import/export, transforms. |
| [Sequencer CustomBindings]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/sequencer-custom-binding-tools.md) | 8 | Convert actors to spawnable/possessable/custom bindings; manage actor templates. |
| [Sequencer Conditions]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/sequencer-condition-tools.md) | 9 | Set/clear conditions on sections, tracks, and track rows in Sequencer. |
| [Sequencer ImportExport]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/sequencer-import-export-tools.md) | 6 | Import/export FBX animation sequences; link/unlink anim sequences to levels. |
| [Sequencer Keyframing]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/sequencer-keyframing-tools.md) | 22 | Curve editor: open/close, select channels/keys, add/remove keys by type, bake keys. |
| [Sequencer Outliner]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/sequencer-outliner-tools.md) | 18 | Sequencer outliner tree: expand/collapse nodes, mute/solo/deactivate/lock/pin state management. |

---

## Quick Reference by Task

### Editor Control & Navigation
- **Start/stop PIE, control camera, capture screenshots** -> [EditorAppToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/editor-app-toolset.md)
- **Set startup widget, game mode, project & engine ini settings** -> [ConfigSettingsToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/config-settings-toolset.md)
- **Control log verbosity and entries** -> [LogsToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/logs-toolset.md)

### Asset Management
- **Find, load, save, duplicate, move, delete assets** -> [AssetTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/asset-tools.md)
- **Create/import curve tables** -> [CurveTableTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/curve-table-tools.md)
- **Create/import data tables with schema** -> [DataTableTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/data-table-tools.md)
- **Create data assets** -> [DataAssetTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/data-asset-tools.md)

### Blueprints & Code
- **Create/compile Blueprints, manage graphs/functions/events/variables/nodes/pins** -> [BlueprintTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/blueprint-tools.md)
- **Get/set UObject properties and search subclasses** -> [ObjectTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/object-tools.md)
- **Execute tool scripts programmatically** -> [ProgrammaticToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/programmatic-toolset.md)

### Materials & Textures
- **Create materials, manage expressions/parameters, recompile** -> [MaterialTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/material-tools.md)
- **Set parameters on material instances** -> [MaterialInstanceTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/material-instance-tools.md)
- **Import textures with settings (sRGB, compression, mipmaps, filter)** -> [TextureTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/texture-tools.md)

### Meshes & Geometry
- **Get/set actor transforms, tags, components** -> [ActorTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/actor-tools.md)
- **Manage skeletal meshes: bones, LODs, physics assets, sockets** -> [SkeletalMeshTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/skeletal-mesh-tools.md)
- **Manage static meshes: LODs, collisions, Nanite** -> [StaticMeshTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/static-mesh-tools.md)
- **Add primitive shapes to scene** -> [PrimitiveTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/primitive-tools.md)

### Scene & Levels
- **Load levels, merge actors, manage folders/collision** -> [SceneTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/scene-tools.md)

### Dataflow & Runtime Data
- **Build and edit dataflow graphs with nodes/pins/variables** -> [DataflowAgentToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/dataflow-agent-toolset.md)
- **Query UE5 Data Registries at runtime** -> [DataRegistryTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/data-registry-tools.md)

### Gameplay Ability System (GAS)
- **Inspect active effects, tags, attributes, abilities** -> [AbilitySystemInspector]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/ability-system-inspector-toolset.md)
- **List attributes and find attribute set classes** -> [AttributeSetTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/attribute-set-toolset.md)
- **Create/find gameplay cues, execute on actors** -> [GameplayCueTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/gameplay-cue-toolset.md)

### AI & Behavior
- **Query behavior trees: decorators, nodes, blackboard data** -> [BehaviorTreeTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/behavior-tree-tools.md)
- **Navigate conversation trees and dialogue graphs** -> [ConversationTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/conversation-tools.md)
- **Manage gameplay tags and find asset referencers** -> [GameplayTagsToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/gameplay-tags-toolset.md)
- **Query state tree editor data (states, tasks, transitions)** -> [StateTreeTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/state-tree-tools.md)

### Game Features & Plugins
- **Activate/deactivate game feature plugins at runtime** -> [GameFeaturesToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/game-features-toolset.md)
- **Create/enable/disable plugins and manage dependencies** -> [PluginManager]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/plugin-toolset.md)

### VFX (Niagara)
- **Full Niagara system editor: emitters, modules, parameters** -> [Niagara System Editor]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/niagara-system.md)
- **Manage Niagara component variables at runtime** -> [Niagara Component]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/niagara-component.md)
- **Find and discover Niagara script assets** -> [Niagara Assets]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/niagara-assets.md)

### Procedural Content (PCG) & Physics
- **Build PCG graphs: nodes, pins, subgraphs, execute instances** -> [PCG]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/pcg-toolset.md)
- **Run instant procedural placement graph** -> [PCG Spatial]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/pcg-spatial-toolset.md)
- **Create physics assets with bodies and constraints** -> [PhysicsAssetToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/physics-asset-toolset.md)

### UI (UMG & Slate)
- **Create UMG widgets, manage slots/events/bindings** -> [UMGToolSet]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/umg-toolset.md)
- **Inspect and interact with Slate UI elements** -> [SlateInspector]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/slate-inspector-toolset.md)

### Save Game & Persistence
- **Discover save slots, check existence, get metadata** -> [SaveGameToolset]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/save-game-toolset.md)

### Search
- **Semantic search across project assets** -> [SemanticSearch]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/semantic-search-toolset.md)

### Animation & Sequencer
- **Create control rigs: graphs, bones, controls, transforms** -> [ControlRig]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/control-rig-tools.md)
- **Full Level Sequence management (tracks, sections, bindings, playback)** -> [Sequencer]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/sequencer-tools.md)
- **Animate control rig data in Sequencer with keyframing** -> [Sequencer ControlRig]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/sequencer-control-rig-tools.md)
- **Manage custom actor bindings in Sequencer** -> [Sequencer CustomBindings]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/sequencer-custom-binding-tools.md)
- **Set conditions on sequencer tracks and sections** -> [Sequencer Conditions]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/sequencer-condition-tools.md)
- **Import/export FBX animation sequences** -> [Sequencer ImportExport]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/sequencer-import-export-tools.md)
- **Curve editor: select channels, add/remove keys, bake** -> [Sequencer Keyframing]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/sequencer-keyframing-tools.md)
- **Outliner tree: expand/collapse, mute/solo/deactivate nodes** -> [Sequencer Outliner]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/sequencer-outliner-tools.md)

### Localization
- **Create/import string tables and manage translation entries** -> [StringTableTools]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/toolsets/string-table-tools.md)

---

## Summary

- **Total toolsets:** 51
- **Total tools:** ~825
- **Generator script:** `UE5/MCP/fetch_toolset.ps1` — run with no args to regenerate all, or `-ToolsetPrefix "Namespace.Prefix"` for a single toolset
- **Progress tracking:** [PROGRESS.md]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/PROGRESS.md)
