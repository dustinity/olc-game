# S05 Tech Tree / Research — Agent Skill

## What to Create

### Assets

| Asset Name | Texture Reference | Path in Project | Source |
|-----------|------------------|-----------------|--------|
| Node Background (unlocked) | `T_Tech_Node_Unlocked` | `/Game/UI/TechTree/T_Tech_Node_Unlocked` | Rounded rectangle, dark with subtle border |
| Node Background (locked) | `T_Tech_Node_Locked` | `/Game/UI/TechTree/T_Tech_Node_Locked` | Grayed version of unlocked node |
| Node Background (researching) | `T_Tech_Node_Researching` | `/Game/UI/TechTree/T_Tech_Node_Researching` | Glowing border, accent color pulse |
| Node Background (completed) | `T_Tech_Node_Completed` | `/Game/UI/TechTree/T_Tech_Node_Completed` | Green-tinted border |
| Ring Path Line | `T_Tech_RingPath` | `/Game/UI/TechTree/T_Tech_RingPath` | Curved line texture for connecting rings |
| Cluster Icons (8) | `T_Cluster_*` | `/Game/UI/TechTree/T_Cluster_Weapons`, etc. | Category icons: Weapons, Armor, Drives, Energy, Vision, Storage, Buildings, Units |
| Prerequisite Arrow | `T_Tech_PrereqArrow` | `/Game/UI/TechTree/T_Tech_PrereqArrow` | Curved arrow for prerequisite lines |

**Toolset:** `asset-tools`, `import-export-toolset`

### Widget Blueprints (8 total)

| Blueprint Name | Class | Purpose |
|---------------|-------|---------|
| `WBP_TechTreeRoot` | UserWidget | Root widget — radial tech tree visualization |
| `WBP_TechNode` | UserWidget | Single research topic node with icon, name, status indicators |
| `WBP_RingContainer` | UserWidget | Container for one ring of the radial tree (Ring 0-3 + Outer) |
| `WBP_ClusterPanel` | UserWidget | Side panel showing cluster overview and progress |
| `WBP_ResearchDetailPanel` | Modal — shows topic details, prerequisites, cost, research progress |
| `WBP_ResearchQueue` | UserWidget | Shows current active research + queued topics |
| `WBP_RingNavigator` | UserWidget | Zoom/pan controls for navigating the radial tree |
| `WBP_TechTreeStatsBar` | UserWidget | Top bar showing total research speed, available scientists (if applicable) |

**Toolset:** `umg-toolset`, `blueprint-tools`

### Blueprint Classes (Gameplay)

| Name | Type | Purpose |
|------|------|---------|
| `BP_TechTreeManager` | GameMode/GameState extension | Central authority for all research state: completed topics, active research, queued topics |
| `BP_ResearchTopic` | DataAsset | Individual research topic definition: name, cluster, ring, prerequisites, cost, effect |

**Toolset:** `blueprint-tools`, `data-tools`

### Data Tables / Data Assets

| Name | Purpose |
|------|---------|
| `DG_TechTopics` | DataTable with all research topics across all clusters and rings |
| `DA_ResearchClusters` | DataAsset array defining the 8 clusters with their properties and ring assignments |

**Toolset:** `data-tools`, `blueprint-tools`

## MCP Workflow: Step by Step

### Phase 1 — Inspect & Prepare

1. **Inspect the project** for existing tech/research assets
   - Toolset: `editor-toolset`
2. **Create directory structure**: `/Game/UI/TechTree/` and `/Game/Gameplay/TechTree/`
   - Toolset: `asset-tools` / `editor-toolset`

### Phase 2 — Import Textures (7 textures)

Import all textures with SRGB = true, TF_Bilinear. Node backgrounds should be clean rounded rectangles suitable for scaling. Ring path line should tile smoothly along curves.

**Toolset:** `asset-tools`, `import-export-toolset`

### Phase 3 — Create Research Topic Data

#### DG_TechTopics
Columns:
- `TopicID` (Name) — unique identifier, e.g., "weapons.ring1.ballistics"
- `Cluster` (GameplayTag) — e.g., "cluster.weapons"
- `Ring` (int 0-3 or Outer)
- `DisplayName` (String)
- `Description` (String)
- `IconTexture` (Texture2D reference)
- `Prerequisites` (Array of TopicID) — must complete these before researching
- `ResearchTime` (float, game hours)
- `CostResources` (struct: resource type + amount pairs)
- `EffectType` (Enum: UnlockBuilding, UnlockUnit, UnlockModule, StatBoost, NewAbility)
- `EffectTarget` (GameplayTag) — what the effect unlocks or modifies
- `EffectValue` (float) — magnitude of stat boost or quantity

Populate with all topics from the tech tree overview:
- **Core (Ring 0)**: Basic ballistics, basic armor plating, basic impulse drive, basic capacitor, basic sensors, basic cargo hold, basic structural frame, basic unit control
- **Ring 1**: Each cluster gets 3-4 topics advancing from core
- **Ring 2**: Advanced variants, branch intersections begin (e.g., Weapons Ring 2 requires Energy Ring 1)
- **Ring 3**: Elite-tier research, heavy prerequisite chains across clusters
- **Outer Ring**: Convergence topics requiring completion of multiple cluster branches

#### DA_ResearchClusters
Define the 8 clusters with:
- `ClusterName` (String) — "Weapons", "Armor", etc.
- `ClusterIcon` (Texture2D reference)
- `Color` (FLinearColor) — visual theme per cluster
- `RingTopics` (Array of TopicID arrays) — which topics belong to each ring

**Toolset:** `data-tools`, `blueprint-tools`

### Phase 4 — Create Research Topic DataAsset

1. **Create `BP_ResearchTopic` DataAsset**:
   - Properties mirror DG_TechTopics columns for runtime access
   - Each topic is a separate DataAsset instance, or all loaded from the DataTable
   - Function: `GetPrerequisites()` → array of completed/remaining prerequisite TopicIDs
   - Function: `CanBeginResearch()` → checks all prerequisites are complete and resources available

**Toolset:** `data-tools`, `blueprint-tools`

### Phase 5 — Create Tech Tree Manager

1. **Create `BP_TechTreeManager`**:
   - Variables:
     - `CompletedTopics` (Array of TopicID)
     - `ActiveResearch` (TopicID, only one at a time unless multiple research slots unlocked)
     - `ResearchQueue` (Array of TopicID)
     - `ResearchSpeedMultiplier` (float) — modified by Buildings (Lab buildings), Factions (intelligence bonuses), Champions (intelligence stat)
   - Functions:
     - `BeginResearch(TopicID)` — validates prerequisites, deducts cost, sets ActiveResearch, starts timer
     - `GetTopicProgress()` → float 0.0-1.0 based on elapsed time vs ResearchTime / ResearchSpeedMultiplier
     - `CompleteResearch(TopicID)` — adds to CompletedTopics, applies effect (unlock building/unit/module), triggers next queued research
     - `QueueTopic(TopicID)` — adds to ResearchQueue for when current research finishes
     - `DequeueTopic()` — removes from front of queue
     - `GetAvailableTopics()` → array of TopicIDs where all prerequisites are in CompletedTopics and resources are affordable
     - `IsTopicUnlocked(TopicID)` → true if topic is visible (prerequisites met or already completed)
   - Tick event: accumulate research progress on ActiveResearch, check for completion

2. **Effect application**: When a topic completes, its EffectType determines what happens:
   - UnlockBuilding → adds building to Construction Mode toolbar
   - UnlockUnit → adds unit type to Squad Selection roster
   - UnlockModule → adds module to Dropship Module Management
   - StatBoost → applies multiplier to relevant colony/building stat

**Toolset:** `blueprint-tools`

### Phase 6 — Create Radial Tree Layout System

1. **Calculate node positions** for the radial layout:
   - Center point = (0, 0) in widget space
   - Ring 0 (Core): single node at center
   - Ring 1: nodes arranged in a circle at radius R1 (e.g., 200 units from center)
   - Ring 2: nodes at radius R2 (~400 units), spaced evenly but grouped by cluster
   - Ring 3: nodes at radius R3 (~600 units)
   - Outer Ring: nodes at radius R4 (~800 units), positioned at branch intersection points

2. **Cluster grouping**: Within each ring, group nodes by their cluster. Each cluster gets a扇-shaped sector of the ring. This makes it visually clear which cluster each topic belongs to.

3. **Prerequisite lines**: Draw curved Bézier paths between prerequisite topics and their dependents. Use a separate canvas layer for these lines so they appear behind the node widgets.

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 7 — Create UI Widgets (Bottom-Up)

#### WBP_TechNode
- CanvasPanel child with: NodeBackground (Image, changes based on status), ClusterIcon (small Image in corner), TopicName (TextBlock), ProgressRing (ProgressBar styled as circular ring for researching topics)
- Variables: `TopicID` (Name), `Status` (Enum: Locked, Available, Researching, Completed)
- Function `UpdateFromManager()` — reads state from TechTreeManager, updates visual appearance
  - Locked: gray node, no interaction
  - Available: normal node with subtle glow pulse, clickable
  - Researching: glowing border, progress ring fills over time
  - Completed: green-tinted background, checkmark overlay
- Event: `OnClicked()` — fires when user clicks an available node → opens ResearchDetailPanel

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_RingContainer
- CanvasPanel containing multiple TechNode instances arranged in a ring pattern
- Variable: `RingNumber` (int), `Nodes` (array of TopicIDs to display)
- Function `LayoutNodes()` — calculates angular positions and places each node at the correct radius
- Background: subtle ring-shaped decorative element using T_Tech_RingPath

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ClusterPanel
- Side panel (right side of screen) showing cluster overview
- For each of the 8 clusters: progress bar (topics completed / total in cluster), cluster icon, name
- Clicking a cluster name → zooms/centers the main tree view on that cluster's nodes
- Color-coded by completion percentage

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ResearchDetailPanel
- Modal overlay with: TopicName (large), Description, Cluster icon + color badge, Prerequisites list (checkmarks for completed, X for missing), Cost breakdown (resource icons + amounts), ResearchTime display, Effect description
- If available: "Research" button (deducts cost, begins research)
- If researching: progress bar with time remaining, "Queue Next" button for other topics
- If locked: grayed out with tooltip showing what prerequisite is missing
- Close button or Escape key dismisses

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_ResearchQueue
- Horizontal list of topic slots at the bottom of the screen
- First slot = currently researching (shows progress ring)
- Subsequent slots = queued topics (up to 3-5 based on unlocked research slots)
- Each slot shows topic icon + name; drag-and-drop reordering if supported
- Clicking a queued topic → opens detail panel for that topic

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_RingNavigator
- Small control panel (bottom-left or floating) with: Zoom in/out buttons, Reset view button, Ring filter checkboxes (show/hide specific rings), Cluster filter dropdown
- Smooth zoom animation when changing zoom level

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_TechTreeStatsBar
- Top bar showing: Total research speed multiplier (e.g., "1.5x"), Active research topic name + progress, Available topics count ("X ready to research")
- Updates every tick or when state changes

**Toolset:** `umg-toolset`, `blueprint-tools`

#### WBP_TechTreeRoot
- Root: CanvasPanel
- Children (z-order back to front):
  1. TechTreeStatsBar at top
  2. ClusterPanel on right side
  3. RingNavigator at bottom-left
  4. RingContainer instances (Ring 0 through Outer) — layered in center area
  5. PrerequisiteLines layer (CanvasPanel with line drawing widgets between nodes)
  6. ResearchQueue at bottom-center
  7. ResearchDetailPanel (hidden by default, modal overlay when open)

- Events:
  - `OnInitialize()` — load all topics from TechTreeManager, create and position node widgets
  - `OnTopicStateChanged(topicID)` — update affected node visuals
  - `OnZoomChanged(factor)` — scale the ring containers proportionally

**Toolset:** `umg-toolset`, `blueprint-tools`

### Phase 8 — Wire Research Speed Modifiers

1. **Building bonuses**: Lab-type buildings (Void Lab, etc.) provide research speed multipliers. TechTreeManager reads active lab buildings on tick and updates ResearchSpeedMultiplier.
2. **Faction bonuses**: Each playable faction has a base intelligence modifier stored in faction data. Applied as a flat multiplier to all research.
3. **Champion bonuses**: Champions with high Intelligence stats provide additional bonus when assigned to research tasks (if champion assignment system exists).

**Toolset:** `blueprint-tools`

### Phase 9 — Compile & Verify

1. **Compile every Blueprint** after creation/modification
2. **Test in PIE**:
   - Open tech tree screen from Strategic loop
   - Verify core nodes are available, ring 1+ nodes are locked with visual feedback
   - Click an available node → detail panel opens with correct prerequisites and costs
   - Begin research → node changes to "researching" state, progress ring fills over time
   - Complete a topic → effect applies (unlock building/unit), next queued topic begins automatically
   - Verify prerequisite chains: try researching a Ring 2 topic without Ring 1 prereqs → blocked
   - Test cluster filtering and zoom navigation
3. **Verify branch intersections**: Confirm that Outer Ring topics require completion of multiple cluster branches, not just one

## Key Patterns & Gotchas

### Radial Layout Math
Node positions use polar coordinates converted to Cartesian: `X = centerX + radius * cos(angle)`, `Y = centerY + radius * sin(angle)`. Each ring has a different radius. Within each ring, nodes are spaced evenly but grouped by cluster sector angle.

### Single Active Research (Initially)
By default, only one topic can be researched at a time. Additional research slots unlock as progression topics in the tree. The ResearchQueue holds pending topics; when active research completes, the next queued topic begins automatically.

### Prerequisite Validation is Transitive
If Topic C requires Topic B, and Topic B requires Topic A, then completing A alone doesn't unlock B (B still needs resources), but completing both A and B unlocks C. The `CanBeginResearch()` check must verify ALL prerequisites recursively, not just direct ones.

### Convergence Mechanics
Outer Ring topics are the convergence points — they require completion of topics from at least 3 different clusters. This enforces breadth across categories rather than letting players specialize in one cluster and ignore others. Visually, these nodes sit at the intersections of cluster sectors.

### Research Speed is Multiplicative
Base speed (1.0x) × faction bonus × lab building bonuses × champion bonuses = final multiplier. A 100-hour topic with 2.5x speed takes 40 game hours. Stack bonuses multiplicatively, not additively, to prevent runaway speeds.

### DataTable vs DataAssets
Use DG_TechTopics (DataTable) for the master list of all topics — it's easier to edit and iterate on. Use BP_ResearchTopic (DataAsset) only if you need runtime object references in Blueprints. For most cases, look up topics by TopicID string from the DataTable at runtime.

## File Structure Summary

```
/Game/UI/TechTree/
├── T_Tech_Node_Unlocked.uasset
├── T_Tech_Node_Locked.uasset
├── T_Tech_Node_Researching.uasset
├── T_Tech_Node_Completed.uasset
├── T_Tech_RingPath.uasset
├── T_Cluster_Weapons.uasset (and 7 other cluster icons)
├── T_Tech_PrereqArrow.uasset
├── WBP_TechTreeRoot.uasset
├── WBP_TechNode.uasset
├── WBP_RingContainer.uasset
├── WBP_ClusterPanel.uasset
├── WBP_ResearchDetailPanel.uasset
├── WBP_ResearchQueue.uasset
├── WBP_RingNavigator.uasset
└── WBP_TechTreeStatsBar.uasset

/Game/Gameplay/TechTree/
├── BP_TechTreeManager.uasset
└── BP_ResearchTopic.uasset (DataAsset base class)

/Game/Data/
├── DG_TechTopics.uasset (DataTable)
└── DA_ResearchClusters.uasset (DataAssets)
```

## Next Steps After This Skill

Tech Tree connects to:
- **Construction Mode (S04)** — unlocks new building types as research completes
- **Colony Management (S02)** — lab buildings boost research speed
- **Squad Selection (S07/S09)** — unlocks new unit types through research
- **Ship Module Management (S13/S14)** — unlocks new ship modules
