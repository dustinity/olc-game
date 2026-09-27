# How to Add a Building — C++ DataAsset + Actor Approach

**Decision:** Use `UPrimaryDataAsset` (UOLCBuildingData) for building configuration and C++ subclasses for behavior. Create `.uasset` DataAssets in the UE5 editor for designer-editable values. Blueprint subclasses (`BP_Building_*`) inherit from C++ actor classes for rapid iteration.

## MCP-Assisted Editor Work

When using UE5 MCP for DataAssets, Blueprints, materials, or asset checks, read [../MCP/Index.md](../MCP/Index.md) and [../../../../agent-bob/Projects/agent-bob-techstack/ue5-mcp-toolsets/UE5-QUICK-REFERENCE.md]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/UE5-QUICK-REFERENCE.md) first. Load [../../../../agent-bob/Projects/agent-bob-techstack/ue5-dev-tools/mcp-helpers/UeMcp.ps1]([olc-agent-bob] agent-bob-techstack/ue5-dev-tools/mcp-helpers/UeMcp.ps1) for session handling and calls:

```powershell
. .\Tools\UeMcp.ps1
Get-UeMcpSession
Find-UeAssets -FolderPath "/Game/OurLastChance/Data/Buildings" -Name ""
```

Use `DataAssetTools` for DataAsset creation, `BlueprintTools` for Blueprint subclasses, and `AssetTools` for lookup/save. Do not start by calling `tools/list`, `list_toolsets`, or `__describe_toolset__` unless the checked-in MCP docs are missing the exact schema detail needed.

---

## Step 0: Base Classes (Already Created — WP-02)

### 0a. Building Data Asset — `UOLCBuildingData`

**File:** `Core/OLCBuildingData.h/cpp`

```cpp
// Core/OLCBuildingData.h — EXISTS
UCLASS()
class OURLASTCHANCE_API UOLCBuildingData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
    EOLCConstructionCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
    FVector2D GridSize; // in grid cells (e.g. 2x2)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
    TArray<FOLCResourceAmount> BuildCost;

    /** Negative value = produces power. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
    float PowerConsumption;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
    TArray<FOLCResourceAmount> OutputPerTick;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
    int32 TIRRequirement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
    FText Description;

    // Biome modifiers — per-biome production multipliers
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
    TArray<FOLCBiomeModifier> BiomeModifiers;

    // Helper: get multiplier for a given biome (default 1.0f)
    float GetBiomeMultiplier(EOLCBiomeType Biome) const;
};
```

**FOLCBiomeModifier struct:**
```cpp
USTRUCT(BlueprintType)
struct FOLCBiomeModifier
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
    EOLCBiomeType BiomeType; // Desert, Dusty, Rocky, Water, Swamp, Jungle, LightSnow, Ice

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Building")
    float Multiplier; // 1.0 = no change, 1.5 = +50%, 0.5 = -50%
};
```

**EOLCBiomeType enum:**
```cpp
UENUM(BlueprintType)
enum class EOLCBiomeType : uint8
{
    Desert, Dusty, Rocky, Water, Swamp, Jungle, LightSnow, Ice
};
```

### 0b. Building Base Actor — `AOLCBuildingBase`

**File:** `World/OLCBuildingBase.h/cpp`

```cpp
// World/OLCBuildingBase.h — EXISTS
UCLASS(Blueprintable)
class OURLASTCHANCE_API AOLCBuildingBase : public AActor
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

    const FOLCBuildingData& GetBuildingData() const;
    void SetBuildingData(const FOLCBuildingData& InData);
    void SnapToGrid();
    void RotateBuild();
    bool IsPowered() const;
    void SetPowered(bool bNewPowered);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building|Components")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building|Components")
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building|Components")
    TObjectPtr<UBoxComponent> Footprint;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Building|Data")
    FOLCBuildingData BuildingData;

    UPROPERTY(BlueprintReadWrite, Category = "Building|Power")
    bool bIsPowered;

    UPROPERTY(BlueprintReadWrite, Category = "Building|Rotation")
    int32 BuildRotationDegrees;

    UPROPERTY(BlueprintReadWrite, Category = "Building|Grid")
    float GridCellSize; // 180.0f (matches crash site tile size)
};
```

### 0c. Blueprintable Base Note

All C++ building classes are already `Blueprintable` via `UCLASS(Blueprintable)` on `AOLCBuildingBase`. Subclasses inherit this automatically. No separate `BOLCBuildingBase` wrapper is needed — create Blueprint subclasses (`BP_Building_Mine`, etc.) directly from the C++ actor classes in the UE5 Content Browser.

---

## Step 1: Design Document Already Exists

Each building has a briefing folder organized under its category:

```
Briefing/buildings/<Category>/PB-XX-NN-SanitizedName/
├── PB-XX-NN-SanitizedName.md    ← Game design specs (stats, costs, behavior)
└── UE5.md                       ← Integration tracker (update status!)
```

Categories: `PowerGeneration`, `Extraction`, `Infrastructure`, `Production`, `Storage`, `Defense`, `HighTier`, `Special`, `Support`

---

## Step 2: Create DataAsset (.uasset) in UE5 Editor

1. Open UE5 editor with the project loaded
2. Navigate to `/Game/OurLastChance/Data/buildings/` in Content Browser
3. Right-click → **Miscellaneous** → **Data Asset**
4. Select `UOLCBuildingData` as the class
5. Name it `DT_Building_<Name>` (e.g., `DT_Building_Mine`)
6. Fill in fields from the Briefing design doc:

| Field | Source | Example |
|-------|--------|---------|
| DisplayName | Briefing building name | "Mine" |
| Category | Building category folder | `Extraction` |
| GridSize | Grid-Placement.md | `2x2` → `(2.0, 2.0)` |
| BuildCost | Briefing stats | ConstructionMaterial: 75/75 |
| PowerConsumption | Briefing (negative = produces) | `-5.0` |
| OutputPerTick | Briefing production rate | Minerals: 10/9999/+2.5 |
| TIRRequirement | Building tier | `1` |
| Description | Briefing summary | "Extracts minerals..." |
| BiomeModifiers | Biome-Compatibility.md | Desert: 1.5x, etc. |

---

## Step 3: Create Blueprint Subclass in UE5 Editor

1. Right-click in Content Browser → **Blueprint Class**
2. Parent class: `AOLCResourceExtractor` (or appropriate category subclass)
3. Name it `BP_Building_<Name>` (e.g., `BP_Building_Mine`)
4. Set the `BuildingData` reference to your `DT_Building_*` DataAsset
5. Assign a placeholder `StaticMesh` (colored cube with category-appropriate color)
6. Save and compile

**Category subclass mapping:**

| Category | C++ Base Class | Blueprint Parent |
|----------|---------------|-----------------|
| Extraction | `AOLCResourceExtractor` | BP_Building_Mine, BP_Building_OilPump |
| PowerGeneration | `AOLCPowerGenerator` | BP_Building_SolarArray, BP_Building_WindTurbine |
| Infrastructure | `AOLCInfrastructure` | BP_Building_Barracks, BP_Building_HabModule |
| Production | `AOLCProductionFacility` | BP_Building_Factory, BP_Building_Refinery |
| Storage | `AOLCBuildingBase` (no subclass needed) | BP_Building_Locker |
| Defense | `AOLCDefenseStructure` | BP_Building_Turret |
| HighTier/Special/Support | `AOLCBuildingBase` or appropriate | Depends on behavior |

---

## Step 4: Add to Build Menu

Add your building's build card to `UOLCUIDataSubsystem::PopulateFakeBuildCards()`:

```cpp
// Core/OLCUIDataSubsystem.cpp
void UOLCUIDataSubsystem::PopulateFakeBuildCards()
{
    // ... existing cards ...

    // --- Extraction ---
    {
        FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
        card.BuildingName = LOCTEXT("Mine", "MINE");
        card.Category = EOLCConstructionCategory::Extraction;
        card.TIRRequirement = 1;
        card.GridSize = FVector2D(2.0f, 2.0f);
        card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 75.0f, 75.0f);
        card.Description = LOCTEXT("MineDesc", "Extracts minerals from the planet surface.");
        card.bAvailable = true;
    }
}
```

---

## Step 5: Add Unlock Condition

- **Available at start:** Add to default build list (as above)
- **Research unlock:** Link to TechData prerequisite in research tree (WP-09)
- **Loot blueprint:** Add to dungeon loot table (WP-11)

---

## Step 6: Test

1. Compile project (`Build → Build Solution` or `dotnet build`)
2. Start PIE in test mode
3. Press F2 for Construction Mode
4. Verify building appears in correct category tab
5. Place on grid, verify snap and rotation (R key)
6. Verify power drain and resource production visible on HUD
7. Update `UE5.md` status → DONE

---

## Building Categories Reference

| Category | Prefix | Examples | Base Class |
|----------|--------|----------|-----------|
| PowerGeneration | PB-PG-* | Solar Array, Wind Turbine, Coal Reactor | AOLCPowerGenerator |
| Extraction | PB-EX-* | Mine, Oil Pump, Harvester Post | AOLCResourceExtractor |
| Infrastructure | PB-IN-* | Camp Barracks, Hab Module, Wall, Gate | AOLCInfrastructure |
| Production | PB-PF-* | Factory, Forge, Refinery, Workbench | AOLCProductionFacility |
| Storage | PB-ST-* | Locker, Container, Haul Storage | AOLCBuildingBase |
| Defense | PB-DS-*, PB-SD-* | Turret Platform, Reinforced Wall | AOLCDefenseStructure |
| HighTier | PB-HP-* | Void Lab, Assembly Plant, Crystal Synthesizer | AOLCBuildingBase |
| Special | PB-SP-* | Dungeon Scanner, Resource Converter | AOLCBuildingBase |
| Support | PB-SB-* | Med Bay, Command Center, Airfield | AOLCInfrastructure |

---

## File Summary

| File | Location | Purpose |
|------|----------|---------|
| `DT_Building_*.uasset` | Content/Data/buildings/ | Building stats DataAsset (editor-created) |
| `BP_Building_*.uasset` | Content/buildings/ | Blueprint subclass (editor-created) |
| `OLCBuildingData.h/cpp` | Core/ | UPrimaryDataAsset type + FOLCBiomeModifier struct |
| `OLCBuildingBase.h/cpp` | World/ | Base actor with mesh, grid snap, power hooks |
| `OLCResourceExtractor.h/cpp` | World/ | Resource production tick behavior |
| `OLCPowerGenerator.h/cpp` | World/ | Power grid feed behavior |
| `OLCInfrastructure.h/cpp` | World/ | Capacity bonus system |
| `OLCProductionFacility.h/cpp` | World/ | Production queue system |
| `OLCDefenseStructure.h/cpp` | World/ | HP, attack range, damage, targeting |
| `OLCUIDataSubsystem.cpp` | Core/ | Build card registration in construction menu |
