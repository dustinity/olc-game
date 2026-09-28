# Integration Status — Our Last Chance

> **Last Updated:** 2026-08-03 (WP-01 + WP-02 completion)

## UI Component Registry

| Component | File | Class | Status | Hotkey | Notes |
|-----------|------|-------|--------|--------|-------|
| Welcome Screen | `UI/OLCWelcomeScreenWidget.h` | `UOLCWelcomeScreenWidget` | DONE | — | Boot screen with Stil-1 hard sci-fi aesthetic |
| Test Switcher | `UI/OLCSharedWidgets.h` | `UOLCTestSwitcherWidget` | DONE | — | Dev-only screen selector |
| Main RTS HUD | `UI/OLCHUDWidgets.h` | `UOLCMainRTSHUDWidget` | **DONE** | F1 | Resource strip (7 resources), mission objectives, minimap with markers, simulation speed controls, biome/hazard badges |
| Construction Mode | `UI/OLCHUDWidgets.h` | `UOLCConstructionOverlayWidget` | **DONE** | F2 | 9 category tabs, build cards from subsystem, placement preview (green/red feedback), R rotate / Esc abort |
| Frame/Panel Shell | `UI/OLCSharedWidgets.h` | `UOLCFrameWidget` | DONE | — | Angular panel background |
| Button Widget | `UI/OLCSharedWidgets.h` | `UOLCButtonWidget` | DONE | — | Primary/secondary/danger/disabled variants |
| Icon Button | `UI/OLCSharedWidgets.h` | `UOLCIconButtonWidget` | DONE | — | Settings, close, pause, scan, filter, rotate |
| Tab Button | `UI/OLCSharedWidgets.h` | `UOLCTabButtonWidget` | DONE | — | Tab strip buttons |
| Resource Counter | `UI/OLCSharedWidgets.h` | `UOLCResourceCounterWidget` | DONE | — | Single resource with icon, value, capacity, delta, pressure |
| Resource Strip | `UI/OLCSharedWidgets.h` | `UOLCResourceStripWidget` | DONE | — | Adaptive strip of resource counters |
| Badge Widget | `UI/OLCSharedWidgets.h` | `UOLCBadgeWidget` | DONE | — | TIR, biome, hazard, faction, scan, status chips |
| Progress Bar | `UI/OLCSharedWidgets.h` | `UOLCProgressBarWidget` | DONE | — | Health, research, repair, construction progress |
| Detail Panel | `UI/OLCSharedWidgets.h` | `UOLCDetailPanelWidget` | DONE | — | Shared right-side entity/planet/system detail |
| Tooltip Widget | `UI/OLCSharedWidgets.h` | `UOLCTooltipWidget` | DONE | — | Hover/click details |
| Modal Overlay | `UI/OLCSharedWidgets.h` | `UOLCModalOverlayWidget` | DONE | — | Settings, confirmations, screen popups |

## Subsystem Data

| Data Source | Getter | Status | Fake Records |
|-------------|--------|--------|-------------|
| Resources | `GetResourceCounters()` | DONE | 7 (Energy, Fuel, ConstructionMaterial, Minerals, HullParts, Survival, DarkMatterCrystals) |
| Mission Objectives | `GetMissionObjectives()` | DONE | 4 objectives |
| Build Cards | `GetBuildCards()` / `GetBuildCardsForCategory()` | DONE | 21 cards across 9 categories (6 Tier 1 + Mine + 6 HighTier/Special + original 8) |
| Construction Categories | `GetConstructionCategories()` | DONE | 9 (Power through Special) |
| Planet Badges | `GetCurrentPlanetBadges()` | DONE | 4 badges (TIR, Biome, Hazard, Modifier) |
| Minimap Markers | `GetMinimapMarkers()` | DONE | 5 markers (Command Center, Minerals, Scout, Enemy, Objective) |
| Simulation Speed | `GetCurrentSimulationSpeed()` / `CycleSimulationSpeed()` | DONE | Paused → Normal → Fast → Paused |
| Quick Build Actions | `GetQuickBuildActions()` | DONE | 3 actions |
| Active Progresses | `GetActiveProgresses()` | DONE | 2 progresses |
| Reset Resources | `ResetResourcesToZero()` | DONE | All 7 resources → 0 on game start |
| Add Resource | `AddResource(EOLCResourceType, float)` | DONE | Bridge from building production to HUD resource counters |

## World / Building System

| Component | File | Class | Status | Notes |
|-----------|------|-------|--------|-------|
| Building Data Asset | `Core/OLCBuildingData.h/cpp` | `UOLCBuildingData` | **DONE** | UPrimaryDataAsset with DisplayName, Category, GridSize, BuildCost, PowerConsumption, OutputPerTick, TIRRequirement, Description, BiomeModifiers |
| Biome Type Enum | `Core/OLCBuildingData.h` | `EOLCBiomeType` | **DONE** | Desert, Dusty, Rocky, Water, Swamp, Jungle, LightSnow, Ice |
| Biome Modifier Struct | `Core/OLCBuildingData.h` | `FOLCBiomeModifier` | **DONE** | EOLCBiomeType + multiplier float |
| Building Base Actor | `World/OLCBuildingBase.h/cpp` | `AOLCBuildingBase` | **DONE** | Blueprintable, mesh, footprint box, grid snap, rotation, power hooks |
| Resource Extractor | `World/OLCResourceExtractor.h/cpp` | `AOLCResourceExtractor` | **DONE** | Timed production tick → AddResource() → UOLCUIDataSubsystem resource counters |
| Power Generator | `World/OLCPowerGenerator.h/cpp` | `AOLCPowerGenerator` | **DONE** | Timed power production tick → AddResource(Energy) → subsystem counters; biome multiplier support |
| Infrastructure | `World/OLCInfrastructure.h/cpp` | `AOLCInfrastructure` | **DONE** | Unit housing bonus, storage capacity, movement blocking, ToggleGate() for gate open/close |
| Production Facility | `World/OLCProductionFacility.h/cpp` | `AOLCProductionFacility` | **DONE** | Production queue system with progress tracking |
| Defense Structure | `World/OLCDefenseStructure.h/cpp` | `AOLCDefenseStructure` | **DONE** | HP, attack range, damage per tick |
| Unit Data Asset | `Core/OLCUnitData.h/cpp` | `UOLCUnitData` | **DONE** | UPrimaryDataAsset with DisplayName, UnitCategory, MaxHP, MovementSpeed, AttackDamage, Range, BuildCost, TrainingTime, TIRRequirement, FactionExclusivity, Description, FuelConsumptionPerMinute |
| Unit Base Actor | `World/OLCUnitBase.h/cpp` | `AOLCUnitBase` | **DONE** | Blueprintable, mesh, health bar widget, selection highlight decal, HP/damage lifecycle |
| Ground Unit | `World/OLCGroundUnit.h/cpp` | `AOLCGroundUnit` | **DONE** | A* pathfinding via NavigationSystemV1 |
| Vehicle Unit | `World/OLCVehicleUnit.h/cpp` | `AOLCVehicleUnit` | **DONE** | Fuel consumption tracking with drain timer |
| Aerial Unit | `World/OLCAerialUnit.h/cpp` | `AOLCAerialUnit` | **DONE** | Altitude management with smooth transitions |

## Unit Training & Combat

| Component | File | Class | Status | Notes |
|-----------|------|-------|--------|-------|
| Training Queue Entry | `World/OLCInfrastructure.h` | `FOLCTrainingQueueEntry` | **DONE** | USTRUCT with UnitDataAsset, Progress, Duration, UnitName |
| Training System | `World/OLCInfrastructure.h/cpp` | `AOLCInfrastructure` | **DONE** | StartTraining(), Tick() progress advancement, CompleteTraining() spawns unit + calls AddUnit() |
| Combat AI Scan | `World/OLCUnitBase.h/cpp` | `AOLCUnitBase` | **DONE** | CheckForTarget() scans AOLCUnitBase actors within Range, attacks nearest enemy on cooldown |
| Combat Attack | `World/OLCUnitBase.cpp` | `AOLCUnitBase::Attack()` | **DONE** | Calls TakeDamage() on target, logs damage dealt |

## RTS Controls

| Component | File | Class | Status | Notes |
|-----------|------|-------|--------|-------|
| Left-click Selection | `Player/OLCGameplayPlayerController.h/cpp` | `AOLCGameplayPlayerController` | **DONE** | DeprojectMousePositionToWorld + LineTraceMultiByObjectType → SetSelected() on hit unit |
| Right-click Movement | `Player/OLCGameplayPlayerController.cpp` | `AOLCGameplayPlayerController` | **DONE** | Raycast ground hit → SelectedUnit->MoveTo() via AOLCGroundUnit A* pathfinding |

## Work Package Status

| WP | Title | Status | Dependencies |
|----|-------|--------|-------------|
| WP-00 | UI Foundation (shared widgets, subsystem, enums/structs) | DONE | — |
| WP-01 | Main HUD (F1) + Construction Mode (F2) | **DONE** | WP-00 |
| WP-02 | Building Base Classes & DataAsset System | **DONE** | WP-01 |
| WP-03 | First Building: Mine (PB-EX-01) | **DONE** | WP-02 |
| WP-04 | Tier 1 Buildings: Starting Base Set (×6) | **DONE** | WP-03 |
| WP-05 | Unit Foundation (Base Classes & DataAsset System) | **DONE** | WP-04 |
| WP-06 | First Unit End-to-End: Basic Soldier | **DONE** | WP-05 |
| WP-07 | Biome Foundation: Enum, DataAsset & Desert Planet | **DONE** | WP-06 |
| WP-08 | Crash Landing Sequence & Tutorial Flow | **DONE** | WP-07 |
| WP-09 | Tech Tree System + S05 UI Screen | **DONE** | WP-08 |
| WP-10 | Space Travel: Solar System View S11 + Galaxy Map S12 | **DONE** | WP-09 |
| WP-11 | Dungeon System: Tactical Combat S08 & Squad Selection S09 | **DONE** | WP-06, WP-10 |
| WP-12 | Dropship Repair View S13 & Ship Module Management S14 | **DONE** | WP-08, WP-10 |
| WP-12.5 | Ship State Wiring (S13/S14 → HUD + Solar System) | **DONE** | WP-12 |

## Biome System

| Component | File | Class/Enum | Status | Notes |
|-----------|------|------------|--------|-------|
| Biome Type Enum | `Core/OLCBuildingData.h` | `EOLCBiomeType` | **DONE** | Desert, Dusty, Rocky, Water, Swamp, Jungle, LightSnow, Ice (8 values) |
| Biome Data Asset | `Core/OLCBiomeData.h/cpp` | `UOLCBiomeData` | **DONE** | UPrimaryDataAsset with EnergyMod, FuelMod, ConstructionMod, MineralsMod, HullMod, SurvivalMod, WallHPModifier, TerrainMaterial, GetResourceMultiplier() helper |
| Biome in Buildings | `World/OLCBuildingBase.h` | `AOLCBuildingBase` | **DONE** | CurrentBiome property (default Desert), GetBiomeMultiplier() helper reads BuildingData.BiomeModifiers[CurrentBiome] |
| Resource Extractor Biome | `World/OLCResourceExtractor.cpp` | `AOLCResourceExtractor` | **DONE** | Logs biome info, production uses BuildingData.GetBiomeMultiplier(CurrentBiome) |
| Power Generator Biome | `World/OLCPowerGenerator.cpp` | `AOLCPowerGenerator` | **DONE** | Uses GetBiomeMultiplier() instead of hardcoded Desert; logs biome + multiplier |

## Data Flow Verification

```
UOLCUIDataSubsystem (fake data source)
    → GetResourceCounters() → UOLCMainRTSHUDWidget::BuildResourceStrip()  ✓
    → GetMissionObjectives() → UOLCMainRTSHUDWidget::BuildMissionProgress()  ✓
    → GetCurrentPlanetBadges() → UOLCMainRTSHUDWidget::BuildBiomeHazardsStrip()  ✓
    → GetMinimapMarkers() → UOLCMainRTSHUDWidget::BuildMinimap()  ✓
    → GetCurrentSimulationSpeed() + CycleSimulationSpeed() → UOLCMainRTSHUDWidget::BuildTimeSpeedControl()  ✓
    → GetConstructionCategories() → UOLCConstructionOverlayWidget::BuildCategoryList()  ✓
    → GetBuildCards() → UOLCConstructionOverlayWidget::RebuildWidget() (card list)  ✓

UOLCBuildingData (UPrimaryDataAsset)
    → DT_Building_*.uasset (editor-created) → BP_Building_*.uasset (Blueprint subclass)
        → AOLCResourceExtractor::Tick() → UOLCUIDataSubsystem resource counters  [ready]
        → AOLCPowerGenerator::Tick() → power grid feed  [ready]
        → AOLCInfrastructure → unit housing / storage bonuses  [ready]
        → AOLCProductionFacility::Tick() → production queue  [ready]
        → AOLCDefenseStructure::Tick() → attack targeting  [ready]

AOLCResourceExtractor::Tick()
    → UOLCUIDataSubsystem::AddResource(ResourceType, Amount) → ResourceCounters[]  ✓ (WP-03)
```

## Screen Hotkeys

| Key | Action | Controller |
|-----|--------|-----------|
| F1 | Open Main RTS HUD | Menu controller |
| F2 | Open Construction Mode | Menu controller |
| Esc | Close active screen / return to switcher | Menu controller |
| R | Rotate build placement (in construction mode) | Gameplay controller |
| Space | Cycle simulation speed | Subsystem |
| Mouse Wheel | Zoom camera (ortho width 1500–20000) | Menu controller |

## Files Created/Modified

### WP-01 (Main HUD + Construction Mode)

| File | Changes |
|------|---------|
| `Core/OLCUIDataSubsystem.cpp` | Added DarkMatterCrystals resource; added HighTier & Special categories (9 total); added 6 new build cards (3 HighTier + 3 Special); added `ResetResourcesToZero()` method |
| `Core/OLCUIDataSubsystem.h` | Added `ResetResourcesToZero()` declaration |
| `UI/OLCHUDWidgets.h` | Renamed `PopulateFakeBuildCards()` → `RefreshConstructionData()` |
| `UI/OLCHUDWidgets.cpp` | Refactored all inline data to subsystem reads; replaced minimap placeholder with SCanvasPanel markers; converted speed control to interactive buttons; added red/green placement preview states; updated category list to 9 categories; removed duplicate PopulateFakeBuildCards() |
| `Player/OLCGameplayPlayerController.cpp` | Delegated R/Esc to widget methods for rotation/cancel sync |
| `Player/OLCMenuPlayerController.h/cpp` | Added camera zoom with mouse wheel (OnZoomCamera); ActiveCamera reference; CurrentOrthoWidth tracking |
| `GameModes/OLCUITestGameMode.cpp` | Pass camera reference to menu controller; call ResetResourcesToZero() at end of BeginPlay |

### WP-02 (Building Base Classes & DataAsset System)

| File | Changes |
|------|---------|
| `Core/OLCBuildingData.h` | **NEW** — UOLCBuildingData (UPrimaryDataAsset), EOLCBiomeType enum, FOLCBiomeModifier struct, GetBiomeMultiplier() helper |
| `Core/OLCBuildingData.cpp` | **NEW** — Constructor implementation |
| `World/OLCBuildingBase.h/cpp` | **NEW** — AOLCBuildingBase actor: mesh, footprint box, grid snap, rotation, power hooks, Blueprintable |
| `World/OLCResourceExtractor.h/cpp` | **NEW** — Timed resource production tick → subsystem |
| `World/OLCPowerGenerator.h/cpp` | **NEW** — Power output tracking and feed |
| `World/OLCInfrastructure.h/cpp` | **NEW** — Unit housing bonus, storage capacity, movement blocking |
| `World/OLCProductionFacility.h/cpp` | **NEW** — Production queue with progress tracking |
| `World/OLCDefenseStructure.h/cpp` | **NEW** — HP, attack range, damage per tick |
| `OurLastChance/Briefing/UE5/HOWTO/HOWTO_ADD_BUILDING.md` | Updated with real file paths, UPrimaryDataAsset pattern, biome modifiers, category subclass mapping table |

### WP-03 (First Building: Mine PB-EX-01)

| File | Changes |
|------|---------|
| `Core/OLCUIDataSubsystem.cpp` | Replaced Mineral Drill with Mine (PB-EX-01): 1x1 grid, 100 CM + 50 minerals cost per Briefing specs |
| `Core/OLCUIDataSubsystem.h` | Added `AddResource(EOLCResourceType, float)` BlueprintCallable method — bridge from building production to HUD |
| `Core/OLCUIDataSubsystem.cpp` | Implemented `AddResource()` — increments ResourceCounters[] by Amount, clamped to capacity |
| `World/OLCResourceExtractor.cpp` | Rewrote Tick() to use AddResource() instead of broken copy-based approach; proper production accumulator |
| `OurLastChance/Briefing/buildings/Extraction/PB-EX-01-Mine/UE5.md` | Updated status: C++ build card + production tick done; DataAsset + Blueprint pending editor creation |

### WP-04 (Tier 1 Buildings: Starting Base Set — ×6)

| File | Changes |
|------|---------|
| `Core/OLCUIDataSubsystem.cpp` | Added 6 Tier 1 buildings: Solar Array (2x1, 80 CM + 60 minerals, -5 power), Camp Barracks (2x2, 200 CM + 100 minerals), Habitation Module (2x2, 300 CM + 150 minerals + 50 survival), Wall Segment (1x1, 30 CM + 20 minerals), Gate (1x1, 80 CM + 50 minerals), Locker (1x1, 50 CM) |
| `World/OLCPowerGenerator.h/cpp` | Added power production timer with biome multiplier support; Tick() feeds energy via AddResource(Energy) |
| `World/OLCInfrastructure.h/cpp` | Added ToggleGate() for gate open/close state; footprint collision toggleable based on bBlocksMovement and bGateOpen |
| 6× `OurLastChance/Briefing/buildings/*/UE5.md` | Updated status to IN_PROGRESS with C++ done / editor pending checklists |

### WP-08 (Crash Landing Sequence & Tutorial Flow)

| File | Changes |
|------|---------|
| `GameModes/OLCMenuGameMode.h` | **MODIFIED** — Added EOLCGameState enum, StartCrashSequence(), TransitionToGameplay(), SetupSequenceCamera(), SetupRTSCamera(), crash phase tracking |
| `GameModes/OLCMenuGameMode.cpp` | **MODIFIED** — Full crash sequence implementation: 5-phase animation timer (4+3+2+3+2s), side-view camera setup, RTS camera transition with smooth blend, tutorial objective initialization, dropship starting resources from Briefing specs |
| `UI/OLCWelcomeScreenWidget.h` | **MODIFIED** — Added FOnNewCampaignClicked delegate + SetOnNewCampaignClicked() binding method |
| `UI/OLCWelcomeScreenWidget.cpp` | **MODIFIED** — HandleNewCampaignClicked() now broadcasts OnNewCampaign delegate to start crash sequence |
| `Player/OLCMenuPlayerController.h` | **MODIFIED** — Added WelcomeScreenInstance (typed), MenuGameMode reference, ShowWelcomeScreen(), OnNewCampaignClicked(), TransitionToGameplayDirect() |
| `Player/OLCMenuPlayerController.cpp` | **MODIFIED** — BeginPlay() now shows welcome screen; New Campaign delegate wired to StartCrashSequence(); full crash sequence flow with camera transition and HUD display |
| `Core/OLCUIDataSubsystem.h` | **MODIFIED** — Added SetMissionObjectives(), CompleteObjective(), ActivateObjective(), AdvanceObjectiveProgress() for tutorial management |
| `Core/OLCUIDataSubsystem.cpp` | **MODIFIED** — Implemented all 4 tutorial objective methods; TransitionToGameplay sets 5 tutorial objectives from Briefing Phase 1 and initial resources (80 CM, 50 Minerals, 30 Fuel, 40 Survival, 25 Hull Parts) |

### WP-09 (Tech Tree System + S05 UI Screen)

| File | Changes |
|------|---------|
| `Core/OLCTechData.h/cpp` | **NEW** — UOLCTechData DataAsset type with DisplayName, Category (EOLCTechCategory enum), RingTier (ERingTier enum), Prerequisites array, MaterialCost, ResearchTimeSeconds, EffectDescription, UnlocksBuildCardName, bAutoUnlock flag; helper methods: ArePrerequisitesMet(), GetCostSummary(), GetRingTierIndex() |
| `Core/OLCResearchSubsystem.h/cpp` | **NEW** — UOLCResearchSubsystem (GameInstanceSubsystem): single-slot research queue with Tick() progress advancement, RegisterTech(), StartResearch(), CancelResearch(), AutoCompleteCoreTechs(), RegisterBuildCardUnlock(), RegisterStarterTechs(); FOnTechComplete + FOnResearchStarted delegates; build card unlock tracking via TMap/TSet |
| `UI/OLCHUDWidgets.h/cpp` | **MODIFIED** — Added BuildResearchProgress() slot to main HUD (between mission objectives and biome badges); research progress bar with tech name, percentage, remaining time display; "NO ACTIVE RESEARCH" idle state |
| `UI/OLCTechTreeWidget.h/cpp` | **NEW** — S05 Tech Tree Slate widget: bottom-up ring layout (Core→Outer), clickable node circles color-coded by state (locked=gray, available=orange, researching=blue, completed=green), intersection nodes highlighted larger, edge connections between prerequisites, right-side detail panel with tech info + RESEARCH button |
| `Player/OLCMenuPlayerController.h/cpp` | **MODIFIED** — Added UOLCTechTreeWidget include; OpenResearch() now creates UOLCTechTreeWidget (S05) instead of placeholder |
| `GameModes/OLCMenuGameMode.cpp` | **MODIFIED** — TransitionToGameplay() calls RegisterStarterTechs() + AutoCompleteCoreTechs() on the research subsystem; registers 3 starter techs: BasicWallPlacement (Core, auto-unlock → unlocks Wall Segment build card), MinePlacement (Ring 1, 100 CM + 80 Minerals → unlocks Mine build card), BasicPowerGrid (Ring 1, 150 CM + 100 Minerals) |

### WP-10 (Space Travel: Solar System View S11 + Galaxy Map S12)

| File | Changes |
|------|---------|
| `UI/OLCSolarSystemWidget.h/cpp` | **NEW** — S11 Solar System View Slate widget: orbital layout with 6 sample planets orbiting sun at varying radii (180–500), clickable planet nodes color-coded by state (green=current, blue=scanned, gray=unscanned), station markers, fuel/energy display in top bar, right-side detail panel with TIR rating, biome info, resources, scan button (10 energy cost, 10s prototype timer), navigate button (deducts fuel); InitializeSolarSystemData() creates planets: Aethel, Vornax, Kaelis, Theron, Nyx, Helios with increasing TIR (1→3) and fuel costs (100→500) |
| `UI/OLCGalaxyMapWidget.h/cpp` | **NEW** — S12 Galaxy Map Slate widget: 9 solar system clusters in spiral pattern around center galaxy objective, warp routes between systems with fuel costs (500 base scaling), clickable system nodes color-coded (green=visited, orange=reachable, gray=locked), right-side detail panel with TIR, distance from center, connected systems list; InitializeGalaxyData() creates Home System, Kepler Reach, Vega Frontier, Orion Belt, Cygnus Deep, Lyra Outpost, Draco Core, Andromeda Gate, Galactic Center (objective) |
| `UI/OLCDropshipRadialMenu.h/cpp` | **NEW** — Circular radial menu appearing at click position with 3 actions: Navigate (S11), Galaxy Map (S12), Repair (DropshipRepair screen); semi-transparent background overlay closes menu on outside click; buttons positioned at equal angular intervals around center point (radius 120px) |
| `Player/OLCMenuPlayerController.h` | **MODIFIED** — Added UScaleBox TransitionOverlay, TransitionProgress, TransitionDuration (1.5s), bIsTransitioning, FTimeline TransitionTimeline, StartScreenTransition(), OnTransitionTick() for S11→S12 zoom-out transition animation |
| `Player/OLCMenuPlayerController.cpp` | **MODIFIED** — Added includes for SolarSystem/GalaxyMap widgets, SScaleBox, Timeline; S11 case creates UOLCSolarSystemWidget; S12 case creates UOLCGalaxyMapWidget with conditional StartScreenTransition() when coming from S11; transition animation: black overlay zooms from 0.01→1.0 scale while fading opacity over 1.5s ease-in-out timeline |
| `Workpackages/WP-10.md` | **NEW** — Work package document with 6 steps: Solar System View, Galaxy Map, fuel consumption wiring, planet scanning, dropship radial menu, transition animation |
