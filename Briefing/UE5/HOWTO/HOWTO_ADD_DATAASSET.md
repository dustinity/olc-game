# How to Add a DataAsset — When and How

**Decision:** For early development (WP-01 through WP-04), use **C++ structs with inline factory functions**. Create real `UPrimaryDataAsset` classes later when designers need editable values without recompiling.

## MCP-Assisted Editor Work

Only use MCP for DataAsset `.uasset` work after the C++ `UPrimaryDataAsset` class exists and compiles. Before making MCP calls, read [../MCP/Index.md](../MCP/Index.md) and [../../../../agent-bob/Projects/agent-bob-techstack/ue5-mcp-toolsets/UE5-QUICK-REFERENCE.md]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/UE5-QUICK-REFERENCE.md), then load [../../../../agent-bob/Projects/agent-bob-techstack/ue5-dev-tools/mcp-helpers/UeMcp.ps1]([olc-agent-bob] agent-bob-techstack/ue5-dev-tools/mcp-helpers/UeMcp.ps1):

```powershell
. .\Tools\UeMcp.ps1
Get-UeMcpSession
Find-UeAssets -FolderPath "/Game/OurLastChance/Data" -Name ""
```

Use `DataAssetTools` to create DataAssets and `AssetTools` to verify, save, and rediscover them. Do not start by calling `tools/list`, `list_toolsets`, or `__describe_toolset__` unless the checked-in MCP docs are missing the exact schema detail needed.

---

## Phase 1: C++ Struct (Early Development)

### When to Use

- Prototype / early implementation
- Stats are stable and unlikely to change frequently
- You want fast iteration (Write file → compile → test)
- Avoiding MCP Blueprint creation overhead

### Pattern

```cpp
// Core/OLCYourDataType.h
USTRUCT(BlueprintType)
struct FOLCYourDataType
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float SomeValue = 0.0f;
};

// Factory function — stats come from Briefing docs
inline FOLCYourDataType GetDefaultData()
{
    FOLCYourDataType Data;
    Data.DisplayName = FText::FromString(TEXT("Name From Briefing"));
    Data.SomeValue = 42.0f; // per Briefing doc, not invented
    return Data;
}
```

### Usage

```cpp
// In UOLCUIDataSubsystem::Initialize() or PopulateFake*() functions
const auto& data = GetDefaultData();
// Use data.DisplayName, data.SomeValue, etc.
```

---

## Phase 2: PrimaryDataAsset (Later Development)

### When to Switch

- Designers need to edit values without recompiling C++
- You want runtime asset discovery (`GetAllAssetsOfClass`)
- Multiple instances with different overrides needed
- Project moves past prototype stage

### Pattern

```cpp
// Core/OLCYourDataAsset.h
UCLASS()
class UOLCYourDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    FText DisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float SomeValue = 0.0f;
};
```

### Create .uasset in UE5 Editor

1. Open Content Browser → `/Game/OurLastChance/Data/`
2. Right-click → Data Asset → select `OLCYourDataAsset`
3. Name: `DT_YourThing_Name`
4. Fill properties from Briefing docs

### Load at Runtime

```cpp
// By name (constructor/init)
ConstructorHelpers::FObjectFinder<UOLCYourDataAsset> Ref(
    TEXT("/Game/OurLastChance/Data/DT_YourThing"));

// Or discover all assets of a class
TArray<UOLCYourDataAsset*> All;
UGameplayStatics::GetAllAssetsOfClass(GetWorld(), UOLCYourDataAsset::StaticClass(), All);
```

---

## Naming Convention

| Type | Pattern | Example |
|------|---------|---------|
| C++ Struct | `FOLC[Type]Data` | `FOLCBuildingData`, `FOLCUnitData` |
| DataAsset class | `UOLC[Type]Data` | `UOLCBuildingData` (extends UPrimaryDataAsset) |
| DataAsset instance | `DT_[Cat]_[Name]` | `DT_Building_Mine`, `DT_Unit_Soldier` |
| Factory function | `Get[Name]Data()` | `GetMineData()`, `GetDesertBiomeData()` |

---

## Content Browser Structure (When Using DataAssets)

```
/Game/OurLastChance/Data/
├── Buildings/    - DT_Building_* assets
├── Units/        - DT_Unit_* assets
├── Biomes/       - DT_Biome_* assets
└── Tech/         - DT_Tech_* assets
```
