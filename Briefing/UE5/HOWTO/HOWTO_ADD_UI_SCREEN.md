# How to Add a UI Screen — Hybrid Slate / UMG Approach

**Decision tree:** Use **Slate C++** for always-visible HUD overlays. Use **UMG Blueprints** for modal screens, menus, and complex layouts.

## MCP-Assisted Editor Work

For UMG work through UE5 MCP, read [../MCP/Index.md](../MCP/Index.md) and [../../../../agent-bob/Projects/agent-bob-techstack/ue5-mcp-toolsets/UE5-QUICK-REFERENCE.md]([olc-agent-bob] agent-bob-techstack/ue5-mcp-toolsets/UE5-QUICK-REFERENCE.md) first. Load [../../../../agent-bob/Projects/agent-bob-techstack/ue5-dev-tools/mcp-helpers/UeMcp.ps1]([olc-agent-bob] agent-bob-techstack/ue5-dev-tools/mcp-helpers/UeMcp.ps1) for session handling and calls:

```powershell
. .\Tools\UeMcp.ps1
Get-UeMcpSession
Invoke-UeMcpTool -ToolsetName "UMGToolSet.UMGToolSet" -ToolName "ListWidgetBlueprints" -Arguments @{ folderPath = "/Game/OurLastChance/UI" }
```

Use `UMGToolSet` for Widget Blueprints, `ObjectTools` for widget properties after `list_properties`, and `AssetTools` for lookup/save. Do not start by calling `tools/list`, `list_toolsets`, or `__describe_toolset__` unless the checked-in MCP docs are missing the exact schema detail needed.

---

## When to Use Slate (C++)

| Screen | Why Slate? |
|--------|-----------|
| Main RTS HUD (F1) | Always visible, updates every tick, frame-rate critical |
| Resource strip | Many counters updating per second |
| Minimap with markers | Needs efficient per-frame rendering |
| Simulation speed control | Must respond instantly to input |

## When to Use UMG (Blueprint)

| Screen | Why UMG? |
|--------|---------|
| Construction Mode (F2) | Card grids, thumbnails, detail panels — layout-heavy |
| Tech Tree (S05) | Zoomable node graph, curved connections |
| Settings / Modals | Standard patterns well-served by UMG widgets |
| Dropship Repair / Mothership Builder | Drag-and-drop module placement |
| Dungeon UI (Tactical Combat) | Health bars, hotbars, roster expansion |

---

## Part A: Slate Widget (C++)

### Step 1: Create Header

```cpp
// UI/OLCYourScreen.h
#pragma once
#include "CoreMinimal.h"
#include "Core/OLCWidgetBase.h"
#include "Core/OLCResourceTypes.h"
#include "OLCYourScreen.generated.h"

UCLASS()
class OURLASTCHANCE_API UOLCYourScreen : public UOLCWidgetBase
{
    GENERATED_BODY()

public:
    UOLCYourScreen(const FObjectInitializer& ObjectInitializer);

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;

private:
    TSharedRef<SWidget> BuildMainPanel();
    TSharedRef<SWidget> BuildDetailSection();
};
```

### Step 2: Implement Slate Layout

```cpp
// UI/OLCYourScreen.cpp
#include "OLCYourScreen.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SOverlay.h"
#include "Player/OLCMenuPlayerController.h"

#define LOCTEXT_NAMESPACE "OLCYourScreen"

UOLCYourScreen::UOLCYourScreen(const FObjectInitializer& Obj)
{
}

TSharedRef<SWidget> UOLCYourScreen::RebuildWidget()
{
    return SNew(SOverlay)
        + SOverlay::Slot()
        [
            BuildMainPanel()
        ];
}

TSharedRef<SWidget> UOLCYourScreen::BuildMainPanel()
{
    // Read data from subsystem — never hardcode
    if (UGameInstance* GI = GetWorld()->GetGameInstance())
    {
        if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
        {
            const auto& Resources = Data->GetResourceCounters();
            // Build widgets from Resources...
        }
    }

    return SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .FillHeight(1.0f)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("YourScreen_Title", "YOUR SCREEN"))
            .ColorAndOpacity(OLCStyleColors::PrimaryOrange)
        ];
}

#undef LOCTEXT_NAMESPACE
```

### Step 3: Register Hotkey in Player Controller

```cpp
// In OLCGameplayPlayerController::SetupInputComponent()
InputComponent->BindKey(EKeys::F1, IE_Pressed, this, &AOLCGameplayPlayerController::OpenMainRTSHUD);
```

### Step 4: Open Widget from Controller

```cpp
void AOLCGameplayPlayerController::OpenYourScreen()
{
    if (UWorld* World = GetWorld())
    {
        auto* Widget = CreateWidget<UOLCYourScreen>(World, UOLCYourScreen::StaticClass());
        if (Widget)
        {
            Widget->AddToViewport();
            ActiveHUDWidget = Widget;
        }
    }
}
```

---

## Part B: UMG Blueprint (Manual in Editor)

### Step 1: Create Blueprint in UE5 Editor

1. Open UE5 editor with the project loaded
2. Navigate to `/Game/OurLastChance/UI/` in Content Browser
3. Right-click → Widget Blueprint → Name: `WBP_YourScreen`
4. Parent class: `UserWidget` (or your custom base if created)

### Step 2: Design Layout Visually

1. Open the widget blueprint editor
2. Set Canvas Panel as root
3. Drag in UMG widgets from the Palette:
   - **TextBlock** for labels
   - **Image** for icons/backgrounds
   - **HorizontalBox / VerticalBox** for layout
   - **Button** for interactive elements
4. Use anchors to position elements (top-left, center, etc.)

### Step 3: Wire Data from Subsystem

1. In the Blueprint's Event Graph, add an `On Initialize` event
2. Add a `Get Game Instance` node → cast to `UOLCUIDataSubsystem`
3. Call `BlueprintPure` functions like `GetResourceCounters()`, `GetBuildCards()`
4. Store results in widget variables
5. In `On Tick` or `Update` events, refresh the UI from stored data

### Step 4: Open from Player Controller

```cpp
void AOLCGameplayPlayerController::OpenUMGScreen(UClass* WidgetClass)
{
    if (UWorld* World = GetWorld() && WidgetClass)
    {
        auto* Widget = CreateWidget<UUserWidget>(World, WidgetClass);
        if (Widget)
        {
            Widget->AddToViewport();
        }
    }
}
```

Call from controller:
```cpp
OpenUMGScreen(WBP_YourScreen::StaticClass());
```

---

## Style Colors

Always use `OLCStyleColors::*` namespace from `UI/OLCSharedWidgets.h`:

| Color | Usage |
|-------|-------|
| `PrimaryOrange` | Primary actions, highlights |
| `TacticalBlue` | Secondary actions, info |
| `ValidGreen` | Success, valid state |
| `DangerRed` | Errors, destructive |
| `WarningYellow` | Warnings, approaching limits |
| `GunmetalBlack` | Backgrounds |
| `DarkSteel` | Panel backgrounds |
| `CharcoalGray` | Secondary panels |
| `BorderGray` | Borders, dividers |
| `TextWhite` | Primary text |
| `TextDim` | Secondary text |

---

## Testing

1. Compile project (C++ changes) or save Blueprint (UMG changes)
2. Start PIE in test mode
3. Press assigned hotkey to open screen
4. Verify layout at 1920×1080, 2560×1440, 3840×2160
5. Press Esc to close / return to switcher

---

## Common Slate Widgets Reference

| Widget | Purpose |
|--------|---------|
| `SOverlay` | Layered positioning (z-order) |
| `SVerticalBox` / `SHorizontalBox` | Flexbox-like layout |
| `STextBlock` | Text labels |
| `SButton` | Clickable buttons |
| `SImage` | Icons, backgrounds |
| `SScaleBox` | Scaled content (aspect ratio) |
| `SCanvasPanel` | Absolute positioning |
| `SBorder` | Styled containers (frames, panels) |
| `SScrollBox` | Scrollable content |
| `SUniformGridPanel` | Grid layout (build cards, icons) |
