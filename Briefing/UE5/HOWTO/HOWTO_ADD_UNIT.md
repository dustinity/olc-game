# How to Add a Unit — DataAsset + Actor Pattern

**Decision:** Use **UPrimaryDataAsset (`UOLCUnitData`)** for unit data. Create `AActor` subclasses in C++. Use Blueprints only for visual customization (meshes, animations). This matches the building system pattern (`UOLCBuildingData` → `AOLCBuildingBase`).

## MCP-Assisted Editor Work

When using UE5 MCP for unit DataAssets, Blueprint subclasses, or asset checks, read [../MCP/Index.md](../MCP/Index.md) and [../../../../agent-bob/Projects/agent-bob-techstack/ue5-mcp-toolsets/UE5-QUICK-REFERENCE.md]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/UE5-QUICK-REFERENCE.md) first. Load [../../../../agent-bob/Projects/agent-bob-techstack/ue5-dev-tools/mcp-helpers/UeMcp.ps1]([olc-agent-bob] agent-bob-techstack/ue5-dev-tools/mcp-helpers/UeMcp.ps1) for session handling and calls:

```powershell
. .\Tools\UeMcp.ps1
Get-UeMcpSession
Find-UeAssets -FolderPath "/Game/OurLastChance/Data/Units" -Name ""
```

Use `DataAssetTools` for DataAsset creation, `BlueprintTools` for Blueprint subclasses, and `AssetTools` for lookup/save. Do not start by calling `tools/list`, `list_toolsets`, or `__describe_toolset__` unless the checked-in MCP docs are missing the exact schema detail needed.

---

## Step 0: Base Classes Already Exist

The following files are already created and compiled. Do not recreate them.

| File | Class | Purpose |
|------|-------|---------|
| `Core/OLCUnitData.h/cpp` | `UOLCUnitData` (UPrimaryDataAsset) | Unit stats DataAsset type |
| `World/OLCUnitBase.h/cpp` | `AOLCUnitBase` (AActor, Blueprintable) | Base actor with mesh, health bar, selection highlight |
| `World/OLCGroundUnit.h/cpp` | `AOLCGroundUnit` | Ground movement + A* pathfinding |
| `World/OLCVehicleUnit.h/cpp` | `AOLCVehicleUnit` | Fuel consumption tracking |
| `World/OLCAerialUnit.h/cpp` | `AOLCAerialUnit` | Flight physics, altitude management |

### UOLCUnitData Fields

```cpp
FText              DisplayName;
EOLCUnitCategory   UnitCategory;       // Infantry, LightVehicle, HeavyVehicle, Aerial, Support
float              MaxHP;
float              MovementSpeed;      // world units per second
float              AttackDamage;
float              Range;              // world units
TArray<FOLCResourceAmount> BuildCost;
float              TrainingTime;       // seconds
FText              FactionExclusivity; // empty = all factions
int32              TIRRequirement;
FText              Description;
float              FuelConsumptionPerMinute; // 0.0f for non-vehicle units
```

### EOLCUnitCategory Enum

```cpp
enum class EOLCUnitCategory : uint8
{
    Infantry,
    LightVehicle,
    HeavyVehicle,
    Aerial,
    Support
};
```

---

## Step 1: Design Document Already Exists

Each unit has a briefing doc. For the Basic Soldier:

```
Briefing/units/Unit-Types/README.md    ← Unit catalog with stats
```

---

## Step 2: Create DataAsset in Editor

In the UE5 Content Browser, create a new **Data Asset** of type `UOLCUnitData`:

1. Right-click in `/OurLastChance/Data/units/` → `Data Asset` → Select `UOLCUnitData`
2. Name it `DT_Unit_<Name>` (e.g., `DT_Unit_Soldier`)
3. Fill fields from the Briefing design doc:

Example — Basic Soldier:
- DisplayName: "Basic Soldier"
- UnitCategory: Infantry
- MaxHP: 100.0f
- MovementSpeed: 250.0f
- AttackDamage: 15.0f
- Range: 20.0f
- BuildCost: [ConstructionMaterial=50, Minerals=30, Survival=10]
- TrainingTime: 30.0f
- TIRRequirement: 1

---

## Step 3: Create Blueprint Subclass

1. In Content Browser, right-click → `Blueprint Class` → Select `AOLCGroundUnit` (for Infantry) or appropriate subclass
2. Name it `BP_Unit_<Name>` (e.g., `BP_Unit_Soldier`)
3. In the Details panel:
   - Set `UnitData` property → reference your `DT_Unit_<Name>` DataAsset
   - Replace placeholder mesh with unit-specific skeletal mesh
   - Configure health bar widget space and decal size

### Subclass Selection Guide

| Unit Type | Base Class | Notes |
|-----------|------------|-------|
| Infantry | `AOLCGroundUnit` | A* pathfinding on navmesh |
| LightVehicle | `AOLCVehicleUnit` | Fuel consumption tracking |
| HeavyVehicle | `AOLCVehicleUnit` | Fuel consumption, larger collision |
| Aerial | `AOLCAerialUnit` | Flight physics, altitude management |
| Support | `AOLCGroundUnit` | Ground-based support units |

---

## Step 4: Add Spawn Source

- **Barracks:** add to production list with training queue (FTimerHandle-based timer)
- **Spawn point:** add to available units
- **Loot:** add to dungeon encounter tables

Wire spawning through `UOLCUIDataSubsystem::AddUnit()` which tracks current/max unit capacity.

---

## Step 5: RTS Selection & Movement

Wire selection in `OLCGameplayPlayerController`:

```cpp
// Left-click unit → highlight ring + health bar visible
// Store selected unit reference in player controller
void AOLCGameplayPlayerController::SelectUnit(AActor* Target)
{
    SelectedUnit = Cast<AOLCUnitBase>(Target);
    if (SelectedUnit)
    {
        SelectedUnit->SetSelected(true);
        // Show health bar widget, update entity panel
    }
}

// Right-click → move to location
void AOLCGameplayPlayerController::MoveSelectedUnit(FVector Destination)
{
    if (SelectedUnit)
    {
        SelectedUnit->GetMovementComponent()->MoveToLocation(Destination);
    }
}
```

---

## Step 6: Test

1. Compile project
2. Play test map
3. Spawn unit in Play mode
4. Verify movement, stats match DataAsset
5. Verify selection highlight and HUD display
6. Update `INTEGRATION_STATUS.md` → DONE

---

## File Summary

| File | Location | Purpose |
|------|----------|---------|
| Unit briefing doc | Briefing/units/Unit-Types/README.md | Game design (source of truth) |
| `OLCUnitData.h/cpp` | Core/ | UOLCUnitData DataAsset type + EOLCUnitCategory enum |
| `OLCUnitBase.h/cpp` | World/ | AOLCUnitBase actor — mesh, health bar, selection highlight |
| `OLCGroundUnit.h/cpp` | World/ | AOLCGroundUnit — ground movement + A* pathfinding |
| `OLCVehicleUnit.h/cpp` | World/ | AOLCVehicleUnit — fuel consumption tracking |
| `OLCAerialUnit.h/cpp` | World/ | AOLCAerialUnit — flight physics + altitude management |
| Unit DataAsset | Content/Data/units/DT_Unit_*.uasset | Editor-created per unit from Briefing specs |
| Unit Blueprint | Content/buildings/BP_Unit_*.uasset | Editor-created subclass with mesh + DataAsset ref |

---

## Data Flow

```
UOLCUnitData (UPrimaryDataAsset)
    → DT_Unit_*.uasset (editor-created)
        → BP_Unit_*.uasset (Blueprint subclass of AOLCGroundUnit/AOLCVehicleUnit/AOLCAerialUnit)
            → AOLCUnitBase::BeginPlay() → CurrentHP = UnitData->MaxHP
            → AOLCUnitBase::TakeDamage() → SetCurrentHP() → Destroy() at 0 HP
            → AOLCGameplayPlayerController::SelectUnit() → SetSelected(true)

UOLCUIDataSubsystem::AddUnit() / RemoveUnit()
    → CurrentUnitCount ↔ MaxUnitCapacity tracking
```
