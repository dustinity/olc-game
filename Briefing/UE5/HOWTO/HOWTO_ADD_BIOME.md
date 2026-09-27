# How to Add a Biome — DataAsset Pattern

**Decision:** Use **UPrimaryDataAsset (`UOLCBiomeData`)** for biome data. This matches the building system pattern (`UOLCBuildingData` → `AOLCBuildingBase`). The `EBiomeType` enum is already defined in OLCBuildingData.h (from WP-02).

## MCP-Assisted Editor Work

When using UE5 MCP for biome DataAssets, materials, or asset checks, read [../MCP/Index.md](../MCP/Index.md) and [../../../../agent-bob/Projects/agent-bob-techstack/ue5-mcp-toolsets/UE5-QUICK-REFERENCE.md]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/UE5-QUICK-REFERENCE.md) first. Load [../../../../agent-bob/Projects/agent-bob-techstack/ue5-dev-tools/mcp-helpers/UeMcp.ps1]([olc-agent-bob] agent-bob-techstack/ue5-dev-tools/mcp-helpers/UeMcp.ps1) for session handling and calls:

```powershell
. .\Tools\UeMcp.ps1
Get-UeMcpSession
Find-UeAssets -FolderPath "/Game/OurLastChance/Data/Biomes" -Name ""
```

Use `DataAssetTools` for DataAsset creation, `MaterialTools` or `MaterialInstanceTools` for terrain materials, and `AssetTools` for lookup/save. Do not start by calling `tools/list`, `list_toolsets`, or `__describe_toolset__` unless the checked-in MCP docs are missing the exact schema detail needed.

---

## Step 0: Base Types Already Exist

The following files are already created and compiled. Do not recreate them.

| File | Class/Enum | Purpose |
|------|------------|---------|
| `Core/OLCBuildingData.h` | `EBiomeType` enum | Desert, Dusty, Rocky, Water, Swamp, Jungle, LightSnow, Ice |
| `Core/OLCBiomeData.h/cpp` | `UOLCBiomeData` (UPrimaryDataAsset) | Biome DataAsset type with resource multipliers |
| `World/OLCBuildingBase.h` | `AOLCBuildingBase` | Has `CurrentBiome` member + `GetBiomeMultiplier()` helper |

### EBiomeType Enum

```cpp
enum class EOLCBiomeType : uint8
{
    Desert, Dusty, Rocky, Water, Swamp, Jungle, LightSnow, Ice
};
```

### UOLCBiomeData Fields

```cpp
FText              DisplayName;
EOLCBiomeType      BiomeType;
float              EnergyMod;        // 1.0 = no change, 1.5 = +50%, 0.5 = -50%
float              FuelMod;
float              ConstructionMod;
float              MineralsMod;
float              HullMod;
float              SurvivalMod;
float              WallHPModifier;
UMaterialInterface* TerrainMaterial; // Set in editor

// Helper: GetResourceMultiplier(EOLCResourceType) returns the correct modifier.
```

### Building Integration

Each building has a `CurrentBiome` property (defaults to Desert). The `GetBiomeMultiplier()` helper reads `BuildingData.BiomeModifiers` and looks up the multiplier for the current biome. Production ticks in OLCResourceExtractor and OLCPowerGenerator use this value.

---

## Step 1: Design Document Already Exists

Each biome has a briefing folder:
```
Briefing/Planets/Biomes/Desert/Desert.md    ← Game design (source of truth)
```

---

## Step 2: Create DataAsset in Editor

In the UE5 Content Browser, create a new **Data Asset** of type `UOLCBiomeData`:

1. Right-click in `/OurLastChance/Data/Biomes/` → `Data Asset` → Select `UOLCBiomeData`
2. Name it `DT_Biome_<Name>` (e.g., `DT_Biome_Desert`)
3. Fill fields from the Briefing design doc:

Example — Desert biome (from Resource Biome Modifier Table):
- DisplayName: "Desert"
- BiomeType: Desert
- EnergyMod: 1.5f   // ⭐⭐⭐⭐⭐ High solar efficiency
- FuelMod: 1.0f     // ⭐⭐⭐ Average
- ConstructionMod: 1.25f  // ⭐⭐⭐⭐ Above average
- MineralsMod: 0.75f      // ⭐⭐ Below average
- HullMod: 0.75f          // ⭐⭐ Below average
- SurvivalMod: 0.75f      // ⭐⭐ Below average
- WallHPModifier: 1.0f    // No wall HP change
- TerrainMaterial: Set to M_Terrain_Desert (editor task)

---

## Step 3: Wire Biome to Buildings

The biome is wired into building production via `AOLCBuildingBase::GetBiomeMultiplier()`. Buildings read the current planet biome from their `CurrentBiome` property and apply the appropriate multiplier from `BuildingData.BiomeModifiers`.

To set a building's biome in editor or code:
```cpp
MyBuilding->CurrentBiome = EOLCBiomeType::Desert;
float multiplier = MyBuilding->GetBiomeMultiplier(); // Returns value from BuildingData.GetBiomeMultiplier(CurrentBiome)
```

---

## Step 4: Create Terrain Material (Editor Task)

In UE5 Content Browser, create a placeholder terrain material:

1. Right-click in `/OurLastChance/Materials/` → `Material` → name `M_Terrain_Desert`
2. Set base color to sandy tan (#D2B48C or similar)
3. Add basic roughness/metallic for desert look
4. Assign to `DT_Biome_Desert.TerrainMaterial` property in editor

Repeat for other biomes as needed (Dusty, Rocky, Water, Swamp, Jungle, LightSnow, Ice).

---

## Step 5: Test

1. Compile project
2. Create DT_Biome_Desert DataAsset with values from Briefing
3. In test map, set building CurrentBiome to Desert
4. Verify Solar Array produces 1.5x energy (EnergyMod = 1.5f)
5. Verify terrain material displays correctly
6. Update `INTEGRATION_STATUS.md` → DONE

---

## File Summary

| File | Location | Purpose |
|------|----------|---------|
| Biome briefing doc | Briefing/Planets/Biomes/ID/ | Game design (source of truth) |
| `OLCBuildingData.h` | Core/ | EBiomeType enum (shared with building system) |
| `OLCBiomeData.h/cpp` | Core/ | UOLCBiomeData DataAsset type + GetResourceMultiplier() |
| `OLCBuildingBase.h` | World/ | CurrentBiome property + GetBiomeMultiplier() helper |
| Biome DataAsset | Content/Data/Biomes/DT_Biome_*.uasset | Editor-created per biome from Briefing specs |
| Terrain Material | Content/Materials/M_Terrain_*.uasset | Editor-created placeholder per biome |

---

## Data Flow

```
UOLCBiomeData (UPrimaryDataAsset)
    → DT_Biome_*.uasset (editor-created)
        → Building.CurrentBiome = enum value from game state/subsystem
        → Building.GetBiomeMultiplier() → reads BuildingData.BiomeModifiers[CurrentBiome]
        → OLCResourceExtractor::Tick() / OLCPowerGenerator::Tick() → applies multiplier to OutputPerTick
```
