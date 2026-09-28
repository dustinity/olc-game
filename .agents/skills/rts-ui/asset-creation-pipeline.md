# Asset Creation Pipeline — Agent Skill

## What to Create

### Purpose

This skill covers the workflow for creating **new gameplay assets from scratch** — units, buildings, and other placeable objects. It defines how to go from a concept (image, description, or placeholder) to a fully integrated game object with mesh, materials, Blueprint logic, data table entry, and placement readiness.

Three paths are supported:
- **Fast path**: Primitive shapes + basic materials → playable immediately, upgrade later
- **AI-assisted path**: Single front-view image → Hunyuan3D-2 generates textured mesh → Blender cleanup → UE5 import (see `UE5/3d/pipeline/`)
- **Full path**: Import hand-crafted FBX/OBJ 3D models → full visual fidelity from the start

The Blueprint class structure is identical for both — only the mesh component differs. Swapping a placeholder cube for a real building model requires changing one variable reference and recompiling.

### Assets Created Per Unit/Building

| Asset Type | Quantity | Purpose |
|-----------|----------|---------|
| StaticMesh or SkeletalMesh | 1–3 (with LODs) | Visual geometry — primitive box/sphere for fast path, imported FBX for full path |
| Texture(s) | 1–4 | Albedo/diffuse map, normal map, roughness/metallic if PBR |
| Material | 1 base + N material instances (per TIR tier) | PBR material with tier-based color/texture swaps |
| DataAsset | 1 | Per-unit/building definition: stats, costs, tags, mesh reference |
| Blueprint Class | 1 | Gameplay logic: movement, combat, construction, interactions |

### Data Tables Updated

| Table | What Gets Added |
|-------|----------------|
| `DG_Buildings` (planet buildings) | New building row with type, footprint, cost, production stats |
| `DG_Units` (combat units) | New unit row with role, base stats, TIR upgrade path, equipment slots |

**Toolset:** `data-tools`, `blueprint-tools`

## MCP Workflow: Step by Step

### Phase 1 — Inspect & Prepare

1. **Inspect the project** for existing unit/building assets to understand conventions
   - Toolset: `editor-toolset`
2. **Create directory structure**:
   - Buildings: `/Game/Assets/Buildings/<BuildingCategory>/<BuildingName>/`
   - Units: `/Game/Assets/Units/<UnitType>/<UnitName>/`
   - Blueprints: `/Game/Gameplay/Buildings/` or `/Game/Gameplay/Units/`
   - Data: `/Game/Data/` (shared tables)
3. **Check existing data table schemas** to match column structure exactly
   - Toolset: `data-tools`

### Phase 2 — Create Mesh (Fast Path or Full Path)

#### Fast Path: Primitive Shapes

1. **Add primitive shape to scene**:
   - Buildings: Use `PrimitiveTools` to add a Box component as placeholder mesh
   - Units: Use `PrimitiveTools` to add a Capsule or Sphere component
2. **Scale appropriately** for the game's unit of measurement (check existing actors)
3. **Apply basic material** — use `MaterialTools` to create a simple PBR material with albedo color matching the building/unit concept

#### AI-Assisted Path: Single Image → 3D Mesh

Use this when you have a front-facing concept image but no 3D model yet. The pipeline is at `UE5/3d/pipeline/`.

1. **Prepare the input image**: Front-facing view of the building/unit on a plain background. Industrial blocky shapes work best; organic shapes need more manual cleanup.
2. **Run the pipeline**:
   ```bash
   cd UE5/3d/pipeline
   python run_pipeline.py --image assets/barracks_front.png --name "Barracks"
   ```
   This runs three stages automatically:
   - **Stage 1** (Hunyuan3D-2): Generates a textured 3D mesh (.glb) from the single image. Requires GPU with 6–16 GB VRAM and a running `api_server.py`.
   - **Stage 2** (Blender CLI): Cleans up the raw mesh — decimates to target triangle count, auto-UV unwraps, generates LOD0/LOD1/LOD2 levels.
   - **Stage 3** (MCP → UE5): Imports the clean FBX into the project, creates a PBR material instance with TIR tier colors, adds VFX emitters (smoke/glow), and generates collision meshes.
3. **Inspect the output**: The mesh will be at `Content/<AssetName>/<AssetName>.fbx` in the UE5 project. Check for topology issues before proceeding to Blueprint creation.
4. **Continue with Phase 3+** as normal — materials, Blueprints, data tables.

**When to use this path:**
- You have concept art or front-view images but no 3D modeler available
- Industrial blocky buildings (solar panels, reactors, barracks) — AI handles right angles well
- Rapid prototyping: get a playable asset in ~5–10 minutes vs. hours of manual modeling

**Limitations:**
- Mesh quality is rough — expect visible artifacts that need Blender cleanup for production
- Textures are baked-in (not separate PBR maps). For hand-painted or high-quality PBR, use the Full Path instead
- Units with animations require skeletal meshes — this pipeline generates static meshes only. Add rigging in Blender after import if needed

#### Full Path: Import 3D Model

1. **Import mesh file** (FBX/OBJ/GLTF):
   - Toolset: `StaticMeshTools` for buildings and non-animated objects
   - Toolset: `SkeletalMeshTools` for units with animations (walking, attacking)
2. **Configure import settings**:
   - Enable Nanite if mesh has high polygon count
   - Generate LODs using `StaticMeshTools` → generate LODs (3 levels recommended: 0 = full detail, 1 = 50% triangles, 2 = 25%)
   - Generate collision meshes (box/capsule approximation)
   - Import normals and tangents
3. **Set up mesh materials** — assign material slots matching the PBR material created in Phase 3

### Phase 3 — Create Materials

1. **Create base PBR Material**:
   - Toolset: `MaterialTools`
   - Properties: BaseColor, Metallic, Roughness, Normal (from texture)
   - For buildings: add a `TierColorSwap` parameter (vector) so TIR upgrades change the building's visual color
   - For units: same tier-based color swap + optional emissive glow for active abilities

2. **Create Material Instances per TIR tier** (1–5):
   - Toolset: `MaterialInstanceTools`
   - Each instance overrides the `TierColorSwap` parameter with a distinct color
   - TIR 1 = gray/basic, TIR 2 = blue/energy, TIR 3 = purple/dark matter, TIR 4 = black/void (matching existing upgrade visual progression from briefing)

### Phase 4 — Create Blueprint Class

#### Building Blueprint (`BP_Building_Base`)

Create a **base class** that all buildings inherit from:

1. **Create `BP_Building_Base`**:
   - Root component: StaticMeshComponent (mesh reference is variable, swappable between placeholder and real model)
   - Variables:
     - `BuildingType` (Enum: ResourceExtraction, PowerGeneration, Production, Storage, Defense, Habitation)
     - `FootprintSize` (struct: Width int, Depth int) — grid footprint in building tiles
     - `PowerConsumption` (float) — negative values = power generator
     - `ProductionRate` (struct: Resource type → rate per tick)
     - `MaxWorkers` (int) — max workers that can be assigned
     - `CurrentTier` (int, default 1) — TIR tier level
     - `IsUnderConstruction` (bool)
   - Functions:
     - `GetEffectiveProductionRate()` → struct — applies worker efficiency and tier multipliers
     - `UpgradeTier(newTier)` — updates mesh material instance, adjusts production stats
     - `CanAffordConstruction(costStruct)` → bool

2. **Create specific building Blueprint** (e.g., `BP_Building_SolarPanel`):
   - Inherits from `BP_Building_Base`
   - Override: BuildingType = PowerGeneration, FootprintSize = 1x1, PowerConsumption = -50 (generates 50 power)
   - Set mesh reference to the imported/placeholder mesh

**Toolset:** `blueprint-tools`

#### Unit Blueprint (`BP_Unit_Base`)

Create a **base class** that all combat units inherit from:

1. **Create `BP_Unit_Base`**:
   - Root component: SkeletalMeshComponent (for animation support) or StaticMeshComponent for non-animated units
   - Variables:
     - `UnitRole` (GameplayTag: Scout, Soldier, Heavy, Engineer, Support)
     - `BaseHP`, `BaseDamage`, `BaseSpeed`, `BaseRange` — base stats before equipment/TIR bonuses
     - `CurrentTier` (int, default 1) — TIR upgrade level
     - `WeaponItemID` (Name), `ArmorItemID` (Name), `UtilityItemID` (Name) — equipped equipment slots
     - `CooldownTimers` (array of struct: AbilityName, CurrentCooldown, MaxCooldown)
   - Functions:
     - `GetEffectiveStats()` → struct — sums base stats + equipment bonuses + TIR multipliers
     - `ApplyDamage(amount)` → float — returns actual damage dealt after defense calculation
     - `StartCooldown(abilityName, duration)` — begins ability cooldown
     - `IsCooldownReady(abilityName)` → bool

2. **Create specific unit Blueprint** (e.g., `BP_Unit_Scout`):
   - Inherits from `BP_Unit_Base`
   - Override: UnitRole = Scout, BaseHP = 80, BaseDamage = 5, BaseSpeed = 4.0, BaseRange = 150
   - Set mesh reference to the imported/placeholder mesh

**Toolset:** `blueprint-tools`

### Phase 5 — Create Data Asset

#### Building DataAsset (`BP_BuildingDefinition`)

1. **Create `BP_BuildingDefinition` DataAsset**:
   - Properties: BuildingName, BuildingType, FootprintSize, PowerConsumption, ProductionRate, MaxWorkers, ConstructionCost (resource struct), Description, AssociatedDataTableRow (reference to DG_Buildings row)
   - Function: `GetConstructionTime()` → float — derived from cost and available construction capacity

**Toolset:** `data-asset-tools`, `blueprint-tools`

#### Unit DataAsset (`BP_UnitDefinition`)

1. **Create `BP_UnitDefinition` DataAsset**:
   - Properties: UnitName, UnitRole, BaseHP, BaseDamage, BaseSpeed, BaseRange, TIRUpgradeCosts (array of cost structs for tiers 2-5), RequiredTechTopics (array of tech tree topic IDs), Description
   - Function: `GetStatsAtTier(tier)` → struct — returns base stats multiplied by tier progression formula

**Toolset:** `data-asset-tools`, `blueprint-tools`

### Phase 6 — Update Data Tables

#### DG_Buildings DataTable

1. **Add new row to DG_Buildings**:
   - Toolset: `DataTableTools`
   - Columns match existing schema (check current columns first)
   - Typical columns: BuildingID, BuildingName, BuildingType, FootprintWidth, FootprintDepth, PowerConsumption, ProductionRate_Energy, ProductionRate_Materials, MaxWorkers, ConstructionCost_Energy, ConstructionCost_Materials, Description, AssociatedDataAsset

2. **Verify row integrity** — ensure all required columns have values, no null references to non-existent assets

#### DG_Units DataTable

1. **Add new row to DG_Units**:
   - Toolset: `DataTableTools`
   - Columns match existing schema
   - Typical columns: UnitID, UnitName, UnitRole, BaseHP, BaseDamage, BaseSpeed, BaseRange, TIRCost_Tier2_Energy, TIRCost_Tier2_Materials, ..., RequiredTechTopics, AssociatedDataAsset

**Toolset:** `data-tools`

### Phase 7 — Register Gameplay Tags

1. **Add gameplay tags** for the new unit/building:
   - Toolset: `GameplayTagsToolset`
   - For units: `unit.role.scout`, `unit.type.<name>`, `faction.<faction>`
   - For buildings: `building.type.<name>`, `building.category.production`, etc.
2. **Verify tag hierarchy** — ensure parent tags exist before adding child tags

### Phase 8 — Wire Into Existing Systems

#### Building Placement (Construction Mode)

1. **Verify the new building appears in Construction Mode**:
   - The PCG placement system reads from DG_Buildings DataTable
   - New row should automatically appear in the construction menu UI
   - Test grid validation: footprint size, biome compatibility, power connection requirements

#### Squad Selection (Squad Building)

1. **Verify the new unit appears in Squad Selection**:
   - The squad building UI reads from DG_Units DataTable
   - New unit should appear in the roster with correct role icon and base stats display
   - Test TIR upgrade path: verify cost requirements and stat improvements display correctly

#### Combat Integration

1. **Verify combat readiness**:
   - Unit's BP_Unit_Base functions are called by Tactical Combat system
   - Equipment slots integrate with Unit Upgrade + Equipment skill
   - Cooldown timers work with ability hotbar in tactical combat UI

**Toolset:** `blueprint-tools`

### Phase 9 — Compile & Verify

1. **Compile every Blueprint** after creation/modification
2. **Test in PIE**:
   - For buildings: Enter construction mode, place the new building on a valid grid tile, verify it appears correctly and produces/consumes resources as expected
   - For units: Open squad selection, add unit to squad, enter tactical combat, verify movement, health display, and ability usage work correctly
3. **Verify TIR upgrades**: Upgrade the building/unit through all 5 tiers — check that mesh material colors change at each tier (gray → blue → purple → black/void)

## Key Patterns & Gotchas

### Fast Path → Full Path is a One-Line Change
The Blueprint class uses a variable reference for the mesh component. Swapping from placeholder to real model:
1. Import the real FBX mesh into the same asset path
2. Open the Blueprint, find the Mesh variable
3. Drag the new mesh onto the variable in the Details panel
4. Recompile — that's it

This means you can build and test gameplay with primitive shapes immediately, then upgrade visuals later without touching any logic code.

### TIR Tier Visual Progression is Standardized
All buildings and units follow the same color progression across TIR tiers:
- **TIR 1**: Gray/basic (default material)
- **TIR 2**: Blue/energy glow
- **TIR 3**: Purple/dark matter
- **TIR 4**: Black/void with spatial distortion
- **TIR 5**: Shimmering iridescent (final tier, unique visual)

This is controlled via the `TierColorSwap` vector parameter on the material. Each TIR tier has a pre-configured Material Instance that overrides this value.

### Footprint Grid Must Match Building Size
Buildings have a Width × Depth footprint in building tiles. A 2×2 building occupies 4 grid cells and must be validated against:
- Obstacles (other buildings, terrain features)
- Biome compatibility (some buildings can't be placed on certain biomes)
- Power line routing space (adjacent tiles for connection lines)

### Units Need Role Balance in Squad Selection
Squad selection enforces mandatory role balance — every squad must have at least one Tank and one Support. When adding a new unit, assign it to an existing role category. If creating a brand new role, you'll also need to update the squad validation logic in BP_EquipmentManager (from the Unit Upgrade + Equipment skill).

### Data Table Schema Must Match Exactly
When adding rows to DG_Buildings or DG_Units, every column must have a value — even if it's empty/default. A missing column value will cause the DataTable row to fail loading at runtime, and the unit/building won't appear in any UI. Always check the existing schema first (Phase 1) before adding new rows.

### Skeletal vs Static Mesh Decision
- **SkeletalMesh**: Required for units with animations (walking, aiming, attacking). Uses `SkeletalMeshTools` for import. Supports ControlRig for animation blending.
- **StaticMesh**: Sufficient for buildings and non-animated units. Uses `StaticMeshTools`. Simpler, faster to render, supports Nanite natively.

If you're unsure whether a unit needs skeletal animation, start with StaticMesh (fast path). Adding skeletal support later requires converting the mesh and reassigning bone references in the Blueprint — doable but not trivial.

## File Structure Summary

```
/Game/Assets/Buildings/<Category>/<Name>/
├── SM_<BuildingName>.uasset (StaticMesh) or SKM_<BuildingName>.uasset (SkeletalMesh)
├── T_<BuildingName>_Albedo.uasset (Texture2D)
├── T_<BuildingName>_Normal.uasset (Texture2D)
├── M_BuildingBase.uasset (Material)
└── MI_BuildingBase_TIR1-5.uasset (5 MaterialInstances)

/Game/Assets/Units/<Type>/<Name>/
├── SKM_<UnitName>.uasset (SkeletalMesh — with LODs if full path)
├── T_<UnitName>_Albedo.uasset (Texture2D)
├── T_<UnitName>_Normal.uasset (Texture2D)
├── M_UnitBase.uasset (Material)
└── MI_UnitBase_TIR1-5.uasset (5 MaterialInstances)

/Game/Gameplay/Buildings/
├── BP_Building_Base.uasset (Blueprint Class — base for all buildings)
└── BP_Building_<Name>.uasset (Blueprint Class — specific building, inherits from Base)

/Game/Gameplay/Units/
├── BP_Unit_Base.uasset (Blueprint Class — base for all units)
└── BP_Unit_<Name>.uasset (Blueprint Class — specific unit, inherits from Base)

/Game/Data/
├── DG_Buildings.uasset (DataTable — new row added)
└── DG_Units.uasset (DataTable — new row added)
```

## Next Steps After This Skill

Asset creation connects to every gameplay skill:
- **Construction Mode (S04)** — newly created buildings appear in the placement menu
- **Colony Management (S02)** — new buildings show up with their production stats and worker slots
- **Tactical Combat (S08)** — newly created units appear in squad selection and fight with correct stats
- **Unit Upgrade + Equipment** — new units get TIR upgrades and equipment loadouts
- **Tech Tree (S05)** — new buildings/units can be gated behind research topics

This skill is the bridge between concept art and playable game content. Without it, you have UI screens but nothing to put in them.
