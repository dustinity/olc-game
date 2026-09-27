# How to Add a Resource Type

Add a new resource to the game's economy (e.g., Dark Matter Crystals, Biofuel).

---

## Prerequisites

- `EOLCResourceType` enum exists in `Core/OLCResourceTypes.h`
- `UOLCUIDataSubsystem` is initialized with fake data

---

## Step 1: Add to Enum

```cpp
// Core/OLCResourceTypes.h
UENUM(BlueprintType)
enum class EOLCResourceType : uint8
{
    Energy               UMETA(DisplayName = "Energy"),
    Fuel                 UMETA(DisplayName = "Fuel"),
    ConstructionMaterial UMETA(DisplayName = "Construction Material"),
    Minerals             UMETA(DisplayName = "Minerals"),
    HullParts            UMETA(DisplayName = "Hull Parts"),
    Survival             UMETA(DisplayName = "Survival"),
    DarkMatterCrystals   UMETA(DisplayName = "Dark Matter Crystals"),
    YourNewResource      UMETA(DisplayName = "Your Display Name"),  // <-- add here
};
```

## Step 2: Add Display Name Mapping

Update `FOLCResourceCounterViewData` constructor in `Core/OLCResourceTypes.h`:

```cpp
case EOLCResourceType::YourNewResource:
    DisplayName = FText::FromString(TEXT("YOUR LABEL"));
    break;
```

## Step 3: Add to Subsystem Fake Data

In `Core/OLCUIDataSubsystem.cpp`, add a fake resource entry in `PopulateFakeResources()`:

```cpp
ResourceCounters.Emplace(EOLCResourceType::YourNewResource, 50.0f, 200.0f, 2.0f);
```

## Step 4: Add to UI

The resource strip (`UOLCMainRTSHUDWidget::BuildResourceStrip()`) auto-picks up all resources from the subsystem. No widget changes needed for standard resources.

For **special** resources (Dark Matter Crystals — late game only):
- Add a `bVisible` flag check in the resource strip builder
- Gate visibility behind TIR level or research unlock

## Step 5: Test

1. Compile project
2. Play test map (F1 for Main HUD)
3. Verify new resource appears in top resource strip
4. Verify label, value, capacity, and delta display correctly
