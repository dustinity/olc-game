You are implementing the UI foundation and first playable UI test pass for the Unreal Engine C++ project “Our Last Chance”.

Project root:
Project root:
D:\Development\Our-Last-Chance\UE5\ProjectFiles\OurLastChance for Claude Code

D:\Development\Our-Last-Chance\UE5\ProjectFiles\OurLastChance for Codex


Design/docs root:
D:\Development\Our-Last-Chance\UE5\Assets\UI

Read these first, in order:
1. D:\Development\Our-Last-Chance\UE5\Assets\UI\IMPLEMENTATION_MASTER_CHECKLIST.md
2. D:\Development\Our-Last-Chance\UE5\Assets\UI\Base.md
3. D:\Development\Our-Last-Chance\UE5\Assets\UI\Shared Assets\Shared UI Strategy.md
4. D:\Development\Our-Last-Chance\UE5\Assets\UI\Main HUD Elements\Briefing UI Audit.md
5. D:\Development\Our-Last-Chance\UE5\Assets\UI\ATLAS_SLICING_GUIDE.md

Goal:
Implement the base UI runtime and first testable UMG screen system, using fake/test data first. Do not implement full gameplay systems yet.

Hard rules:
- Use canonical resources only: Energy, Fuel, Construction Material, Minerals, Hull Parts, Survival.
- Dark Matter Crystals are special/late-game only.
- No turn counters, no action points, no end-turn UI.
- Do not use generated full-screen mockups as live UI backgrounds.
- Use sliced atlas assets or atlas regions for icons/components.
- Keep all labels in UMG text widgets.
- Every screen must be openable in Play mode.
- Mouse cursor must work for UI testing.
- Do not improvise new visual styles. Follow Stil-1 hard sci-fi UI.

Implementation order:
1. Run/verify:
   powershell -ExecutionPolicy Bypass -File D:\Development\Our-Last-Chance\UE5\Assets\UI\slice-atlases.ps1

2. Implement base C++ / Blueprint foundation from:
   D:\Development\Our-Last-Chance\UE5\Assets\UI\Base.md

   Required:
   - EOLCResourceType
   - FOLCResourceAmount
   - FOLCResourceCounterViewData
   - FOLCBadgeViewData
   - FOLCActionViewData
   - FOLCProgressViewData
   - FOLCMinimapMarkerViewData
   - UOLCWidgetBase
   - UOLCUIDataSubsystem with fake data
   - AOLCUITestGameMode
   - AOLCMenuPlayerController
   - AOLCGameplayPlayerController

3. Create UI test map / test mode:
   - On Play, show a developer UI test switcher.
   - Show mouse cursor.
   - Set input mode GameAndUI.
   - Hotkeys:
     F1 Main RTS HUD
     F2 Construction Mode
     F3 Colony Resource Network
     F4 Solar System
     F5 Galaxy Map
     F6 Tactical Dungeon
     F7 Research
     F8 Dropship Repair
     F9 Mothership Builder
     F10 Equipment
     Esc close active overlay / return to switcher

4. Build shared widgets first:
   - WBP_UI_Frame
   - WBP_UI_Button
   - WBP_UI_IconButton
   - WBP_UI_TabButton
   - WBP_UI_ResourceCounter
   - WBP_UI_ResourceStrip
   - WBP_UI_Badge
   - WBP_UI_ProgressBar
   - WBP_UI_DetailPanel
   - WBP_UI_Tooltip
   - WBP_UI_ModalOverlay
   - WBP_UI_TestSwitcher

5. Then implement the first two real screens:
   - Main HUD Elements/IMPLEMENTATION_GUIDE.md
   - Main RTS HUD on F1
   - Construction Mode on F2

Stop after F1/F2 are testable unless the checklist is fully stable.

Acceptance test:
- Project compiles.
- UI test map opens in Play mode.
- Mouse cursor is visible and buttons are clickable.
- F1 opens Main RTS HUD.
- F2 opens Construction Mode.
- Esc closes active overlay.
- Resource strip shows exactly:
  Energy, Fuel, Construction Material, Minerals, Hull Parts, Survival.
- Construction Mode has categorized build menu, build cards, placement preview, R Rotate, Esc Abort.
- No turn/action-point/end-turn UI appears.
- UI fits 1280x720 and 1920x1080 without major overlap.

If anything is blocked:
- Do not guess silently.
- Write the blocker and propose the smallest fix.
- Prefer fake data and placeholder widgets over waiting for real gameplay systems.
