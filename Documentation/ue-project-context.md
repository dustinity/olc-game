# UE Project Context — Our Last Chance

*Last updated: 2026-09-26 (re-drafted against `agentic-workflow-runner` HEAD after discovering the
first draft was made against a checkout 114 commits stale — see git history around this date).*
*Auto-drafted from `.uproject`, `Source/*/Build.cs`, `Source/*/*.Target.cs`, and a source scan of
`Source/OurLastChance/`. Fill in Team Context manually — not derivable from the repo.*

## Engine & Project Overview
**Engine version:** UE 5.8 (`EngineAssociation: "5.8"` in `OurLastChance.uproject`)
**Project name:** Our Last Chance
**Description:** Single-player sci-fi colony/RTS with dungeon-crawl, space-combat, and
interplanetary/galactic travel layers (planet colonization, research, ship modules, champion/squad
combat, tutorial-guided onboarding).
**Project type:** Game
**Genre / domain:** RTS/colony-sim + tactical dungeon combat, single-player
**Target platforms:** Windows (dev signing workaround in both `.Target.cs` files is Win64-only,
skipped on Linux/macOS/CI) — no other platform targets configured yet.

## Module Structure
**Primary game module:** `OurLastChance` (the only module — `Type: Runtime`, `LoadingPhase: Default`)

| Module | Type | Notes |
|--------|------|-------|
| OurLastChance | Runtime | Sole gameplay module; single-module project, no plugin/editor-tools module split yet |

**Source size:** 217 files (112 headers, 105 cpp), ~43.6k lines, across 11 feature folders:
`Core/`, `UI/`, `Player/`, `GameModes/`, `World/`, `Combat/`, `AI/`, `Data/`, plus three folders
added since the project's early state — `SpaceTravel/`, `Animation/`, `VFX/`.

**Key dependencies:**
- **Public:** Core, CoreUObject, Engine, Slate, SlateCore, UMG, EnhancedInput, CommonUI,
  ProceduralMeshComponent, ImageWrapper, NavigationSystem, **Niagara**
- **Private:** AssetRegistry, InputCore, Projects
- `bUseRTTI = false` — deliberate: engine's Linux build ships with `WITH_RTTI=0`; the module must
  stay in sync or linking fails on missing typeinfo symbols on Linux. Don't flip this without
  checking the Linux build.
- `EnhancedInput` and `CommonUI` are linked, but no `UInputAction`/`UInputMappingContext` C++
  classes exist in source — Enhanced Input config is likely Blueprint/DataAsset-only; confirm
  before assuming a C++ input-binding pattern exists to follow.
- **Niagara is now a real C++ dependency** (added since the project's early state) — `VFX/OLCVFXSubsystem`,
  `VFX/OLCVFXData`, `Animation/OLCBuildingSequenceComponent`, and `World/OLCBuildingBase` all
  reference `UNiagaraComponent`/`UNiagaraSystem` directly. `ue-niagara-effects` now applies to
  real code, not just MCP toolset capability.

**Build targets:** `OurLastChanceTarget` (Game) and `OurLastChanceEditorTarget` (Editor) only — no
Server/Client split (single-player; confirmed no `GetLifetimeReplicatedProps`/`DOREPLIFETIME`
anywhere in `Source/`). Both targets carry an identical Win64-only post-build step that auto-signs
the dev DLL with a local cert to satisfy Windows Smart App Control; it always exits 0 so it
silently no-ops without the cert (including CI).

## Plugin Dependencies
**Engine plugins enabled** (from `.uproject`):
- `CommonUI` — UI framework (see UI module notes below)
- `ModelingToolsEditorMode` — editor-only (`TargetAllowList: ["Editor"]`)
- `ModelContextProtocol` + `MCPClientToolset` + `AllToolsets` — Epic's official MCP plugin; this
  is the live-editor automation surface `.claude/SKILL.md` and `UE5/MCP/` document
- `ProceduralVegetationEditor` — editor-only PCG/vegetation tooling

**Not enabled / not in use:** GameplayAbilities (GAS), StateTree, Mass Entity — still zero
references anywhere in `Source/` as of this re-scan. `ue-gameplay-abilities`, `ue-state-trees`,
`ue-mass-entity` remain forward-looking only, not "how this project already does it." (Niagara
has moved out of this list — see Module Structure above.)

**Custom plugins:** none under `Plugins/` — everything lives in the single `OurLastChance` module.

## Coding Conventions
**Naming prefixes:** Standard UE (`A`/`U`/`F`/`E`/`I`) **plus a mandatory `OLC` project prefix**
on every project-owned class, documented in `Source/OurLastChance/SOURCE_LAYOUT.md`. Two legacy
exceptions remain unfixed as of this re-scan: `UI/WBP_FactionSelect.h` (file/include misnamed,
but the class inside is already correctly `UOLCFactionSelectWidget` — a safe rename) and
`World/BP_TerrainDemo.h` (the native class itself is `ABP_TerrainDemo`, and a real Blueprint asset,
`Content/Terrain/Actors/BP_TerrainDemo.uasset`, has that class as its parent — fixing this one
needs a live editor/MCP session to reparent the Blueprint, not just a file rename). See WP-137.
**Header style:** `#pragma once` universally (112/112 headers checked — zero traditional guards).
**Log categories in use:** still none custom-declared — all logging goes through the built-in
`LogTemp` category. No `DEFINE_LOG_CATEGORY` calls exist in `Source/`. See WP-138.
**Assertion style:** `check()` only, still just 2 call sites across 43.6k lines. No `ensure`/
`checkf`/`verify` usage found.
**Header organization:** flat per-folder, not Public/Private split. See `SOURCE_LAYOUT.md` for the
authoritative rule set: keep gameplay code out of the module root, prefix cross-folder includes
with their folder (`Core/OLCResourceTypes.h`), keep `.generated.h` last in every reflected header.
**Additional rules (from source, not just SOURCE_LAYOUT.md):**
- `TObjectPtr<T>` is the default for UPROPERTY object references (159 uses, up from 131 at last
  scan); raw pointers are the exception, not the rule.
- Gameplay-facing `UFUNCTION`/BlueprintCallable categories consistently use the `"OLC|<System>"`
  pattern (`"OLC|Equipment"`, `"OLC|Combat"`) — follow this for new subsystem functions.
- Data-heavy subsystems favor transient `NewObject()`-constructed definitions over persistent
  DataAssets when no MCP/editor round-trip is needed (explicit doc-comment pattern in
  `OLCRaceSubsystem.h`/`OLCResearchSubsystem.h`) — deliberate, not an oversight.

## Subsystems in Use
**Gameplay framework:**
- GameModes: `AOLCMenuGameMode`, `AOLCGameplayDemoGameMode`, `AOLCTerrainDemoGameMode`,
  `AOLCUITestGameMode` — all extend `AGameModeBase` (no `AGameMode`/`AGameState`).
- PlayerControllers: `AOLCMenuPlayerController`, `AOLCGameplayPlayerController`.
- Still no custom Pawn/Character class — `World/OLCUnitBase.h` and subclasses (`OLCGroundUnit`,
  `OLCAerialUnit`, `OLCVehicleUnit`) remain the closest equivalent, as RTS-style controllable
  units rather than possessed Pawns/Characters.

**Subsystems — 12 total now (up from 5 at the first scan), 11 `UGameInstanceSubsystem` + the
project's first `UWorldSubsystem`:**

| Class | Type | Responsibility |
|-------|------|-----------------|
| `UOLCUIDataSubsystem` | UGameInstanceSubsystem | UI-facing data aggregation — see god-object note below (WP-136) |
| `UOLCEquipmentSubsystem` | UGameInstanceSubsystem | Faction equipment pools, loot/assignment |
| `UOLCRaceSubsystem` | UGameInstanceSubsystem | 33 race definitions, biome spawn weighting, boss encounters, faction bonuses |
| `UOLCResearchSubsystem` | UGameInstanceSubsystem | Single-slot research queue, tech completion delegates |
| `UOLCCombatLogSubsystem` | UGameInstanceSubsystem | Combat log entries for UI |
| `UOLCGalaxyTransitionSubsystem` | UGameInstanceSubsystem | *(new)* Galaxy-map/solar-system transition state |
| `UOLCTutorialSubsystem` | UGameInstanceSubsystem | *(new)* Tutorial objective tracking, PIE-safe test-hook scheduling |
| `UOLCNavigationSubsystem` | UGameInstanceSubsystem | *(new)* Navigation state (planetary/galactic) |
| `UOLCColonySubsystem` | UGameInstanceSubsystem | *(new)* Colony-network / multi-planet management (1337-line cpp) |
| `UOLCTravelSubsystem` | UGameInstanceSubsystem | *(new)* Interplanetary/space travel (750-line cpp) |
| `UOLCVFXSubsystem` | UGameInstanceSubsystem | *(new)* Niagara VFX spawning/tracking |
| `UOLCAnimationSubsystem` | **UWorldSubsystem** | *(new)* The project's only non-`GameInstanceSubsystem` — animation state scoped per-world, correctly chosen since animation instances don't persist across level transitions the way game-instance-scoped data does |

**Custom systems (not GAS-based):**
- Combat: `OLCCombatStateComponent`, `OLCDamageCalculator`, `OLCTargetingSystem`,
  `OLCWeaponMountComponent`, `OLCSpaceCombatManager`, `OLCMinigameManager` — hand-rolled, not
  built on `AbilitySystemComponent`/`GameplayEffect`.
- AI: `OLCCombatAIController`, `OLCFormationManager`, `OLCNavMeshPathfinder`, `OLCThreatWeights`,
  `OLCControlGroupManager` — custom AI/formation logic, not Behavior Tree/Blackboard-driven and
  not StateTree-based.
- World/economy: `OLCPlanetTerrainGenerator`, `OLCResourceExtractor`, `OLCPowerGenerator`,
  `OLCProductionFacility`, `OLCBuildingBase`, `OLCInfrastructure`, `OLCDefenseStructure`,
  `OLCCrashSitePrototypeActor`, `OLCObjectiveHighlightComponent`.
- VFX/Animation *(new folders)*: `OLCVFXSubsystem`/`OLCVFXData` (Niagara), `OLCAnimationSubsystem`,
  `OLCAnimationPlayerComponent`, `OLCBuildingSequenceComponent`, `OLCAnimationSmokeTestHook`.
- SpaceTravel *(new folder)*: `OLCTravelSubsystem`, `OLCTravelConfigData`, `OLCSpaceEventData`,
  `OLCSpaceWeatherData`, `OLLandingSequence` (note: this one class is missing the `C` in `OLC` —
  worth checking whether that's a typo alongside WP-137's other two naming exceptions).

**`OLCUIDataSubsystem` god-object note:** grown further since the first scan — now 533-line header,
74 `UFUNCTION`s, 1954-line cpp, spanning at least 16 domain comment-blocks including two added
since (Squad deployment / WP-119, Alien Shield Generator / WP-125). See WP-136.

**GAS usage:** still none. No `UGameplayAbility`, `UAttributeSet`, `UGameplayEffect`, or
`UAbilitySystemComponent` reference exists anywhere in `Source/`.

## Build Configuration
**Build targets:** Game + Editor only (no Server/Client/dedicated-server targets).
**Custom macros / build flags:** `bUseRTTI = false` (see Module Structure above).
**Third-party libraries:** none integrated via `Build.cs` beyond engine modules.
**Platform-specific notes:** Win64-only post-build signing step in both `.Target.cs` files, guarded
by `Target.Platform == UnrealTargetPlatform.Win64` and always exiting 0 — safe no-op on
Linux/macOS/CI.
**Engine modifications:** None — stock engine, launcher-association `5.8`.

## Team Context
*Not yet established from repo scan — fill in if/when a team process is defined (source control is
Git; branch `agentic-workflow-runner` also carries an active automated LLM-coding-benchmark
pipeline under `benchmark/`, evaluating multiple models against this project's own work packages —
worth knowing before assuming all commits on this branch are hand-authored gameplay work).*

---

## Notes for whoever updates this next

**This document was already re-drafted once (2026-09-26) after the first draft turned out to be
based on a checkout 114 commits behind `origin/agentic-workflow-runner`.** Before trusting any
snapshot like this one, confirm `git status` shows the working branch is not both ahead and behind
its remote counterpart — a diverged branch means the "current" scan might not be current at all.

Re-run the scan described in
`.claude/skills/unreal-engine-skills/ue-project-context/SKILL.md` (not hand-patch line-by-line)
whenever a system listed above as "not in use" gets adopted, or periodically to catch drift like
the folder/subsystem growth found this pass. Quick re-verify commands:

```
git fetch origin && git status                                   # confirm not stale/diverged first
grep -rli "GameplayAbilit\|AttributeSet\|StateTree\|MassEntity" Source/OurLastChance/
find Source/OurLastChance -name "*.h" -o -name "*.cpp" | wc -l    # compare against 217
```
