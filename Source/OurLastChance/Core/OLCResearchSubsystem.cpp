#include "OLCResearchSubsystem.h"
#include "OurLastChance.h"

#include "Core/OLCUIDataSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"

#define LOCTEXT_NAMESPACE "OLCResearchSubsystem"

// Mirrors UOLCTechData::GetRingTierIndex(), but usable without a Tech
// instance (needed to test an arbitrary ERingTier against ring-progression
// state in IsRingUnlocked/HasRequiredResearchBuilding).
static int32 RingIndexOf(ERingTier Ring)
{
	switch (Ring)
	{
		case ERingTier::Core:  return 0;
		case ERingTier::Ring1: return 1;
		case ERingTier::Ring2: return 2;
		case ERingTier::Ring3: return 3;
		case ERingTier::Outer: return 4;
		default:               return 0;
	}
}

void UOLCResearchSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogOLC, Log, TEXT("[OLC] Research subsystem initialized"));
}

void UOLCResearchSubsystem::Tick(float DeltaTime)
{
	if (!CurrentResearch) return;

	// Advance progress, scaled by faction/champion/building research-speed bonuses.
	CurrentProgressSeconds += DeltaTime * GetResearchSpeedMultiplier();

	if (CurrentProgressSeconds >= CurrentResearch->ResearchTimeSeconds)
	{
		// Research complete!
		UOLCTechData* Completed = CurrentResearch;
		CurrentResearch = nullptr;
		CurrentProgressSeconds = 0.0f;

		CompleteResearch(Completed);
	}
}

void UOLCResearchSubsystem::RegisterTech(UOLCTechData* Tech)
{
	if (!Tech || AllTechs.Contains(Tech)) return;

	AllTechs.Add(Tech);
	UE_LOG(LogOLC, Log, TEXT("[OLC] Registered tech: %s (ring=%d, category=%d)"),
		*Tech->DisplayName.ToString(),
		Tech->GetRingTierIndex(),
		static_cast<int32>(Tech->Category));
}

UOLCTechData* UOLCResearchSubsystem::FindTechByName(const FString& Name) const
{
	for (const TObjectPtr<UOLCTechData>& Tech : AllTechs)
	{
		if (!Tech) continue;
		FString DisplayName = Tech->DisplayName.ToString();
		if (DisplayName.Equals(Name, ESearchCase::IgnoreCase))
		{
			return Tech.Get();
		}
	}
	return nullptr;
}

float UOLCResearchSubsystem::GetResearchProgress() const
{
	if (!CurrentResearch) return 0.0f;
	float Progress = CurrentProgressSeconds / CurrentResearch->ResearchTimeSeconds;
	return FMath::Clamp(Progress, 0.0f, 1.0f);
}

float UOLCResearchSubsystem::GetTotalResearchTime() const
{
	if (!CurrentResearch) return 0.0f;
	return CurrentResearch->ResearchTimeSeconds;
}

TArray<UOLCTechData*> UOLCResearchSubsystem::GetAvailableTechs() const
{
	TArray<UOLCTechData*> Available;

	for (const TObjectPtr<UOLCTechData>& Tech : AllTechs)
	{
		if (!Tech) continue;
		if (CompletedTechs.Contains(Tech)) continue;
		if (Tech == CurrentResearch) continue;
		if (!IsRingUnlocked(Tech->RingTier)) continue;
		if (!HasRequiredResearchBuilding(Tech->RingTier)) continue;

		if (Tech->ArePrerequisitesMet(CompletedTechs))
		{
			Available.Add(Tech.Get());
		}
	}

	return Available;
}

bool UOLCResearchSubsystem::IsTechCompleted(UOLCTechData* Tech) const
{
	return CompletedTechs.Contains(Tech);
}

bool UOLCResearchSubsystem::StartResearch(UOLCTechData* Tech)
{
	if (!Tech) return false;
	if (CurrentResearch)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Cannot start research — already researching: %s"),
			*CurrentResearch->DisplayName.ToString());
		return false;
	}
	if (CompletedTechs.Contains(Tech))
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Tech already completed: %s"), *Tech->DisplayName.ToString());
		return false;
	}

	TArray<UOLCTechData*> Available = GetAvailableTechs();
	if (!Available.Contains(Tech))
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Tech not available for research: %s"), *Tech->DisplayName.ToString());
		return false;
	}

	// Deduct material costs.
	if (!Tech->MaterialCost.IsEmpty() && !DeductMaterialCosts(Tech->MaterialCost))
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Insufficient resources to start research: %s"), *Tech->DisplayName.ToString());
		return false;
	}

	CurrentResearch = Tech;
	CurrentProgressSeconds = 0.0f;

	UE_LOG(LogOLC, Display, TEXT("[OLC] Started researching: %s (%.0fs)"),
		*Tech->DisplayName.ToString(), Tech->ResearchTimeSeconds);

	OnResearchStarted.Broadcast(Tech);
	return true;
}

void UOLCResearchSubsystem::CancelResearch()
{
	if (!CurrentResearch) return;

	UE_LOG(LogOLC, Log, TEXT("[OLC] Cancelled research: %s (no refund)"),
		*CurrentResearch->DisplayName.ToString());

	CurrentResearch = nullptr;
	CurrentProgressSeconds = 0.0f;
}

void UOLCResearchSubsystem::RegisterBuildCardUnlock(UOLCTechData* Tech, const FString& BuildCardName)
{
	if (!Tech) return;
	BuildCardUnlocks.Add(BuildCardName, Tech);
	UE_LOG(LogOLC, Log, TEXT("[OLC] Registered build card unlock: '%s' → tech: %s"),
		*BuildCardName, *Tech->DisplayName.ToString());
}

bool UOLCResearchSubsystem::IsBuildCardUnlocked(const FString& BuildCardName) const
{
	return UnlockedBuildCards.Contains(BuildCardName);
}

bool UOLCResearchSubsystem::IsBuildCardNameReferenced(const FString& BuildCardName) const
{
	if (BuildCardUnlocks.Contains(BuildCardName)) return true;
	for (const TObjectPtr<UOLCTechData>& Tech : AllTechs)
	{
		if (Tech && !Tech->UnlocksBuildCardName.IsEmpty() && Tech->UnlocksBuildCardName.ToString().Equals(BuildCardName, ESearchCase::IgnoreCase))
		{
			return true;
		}
	}
	return false;
}

void UOLCResearchSubsystem::AutoCompleteCoreTechs()
{
	for (const TObjectPtr<UOLCTechData>& Tech : AllTechs)
	{
		if (!Tech) continue;
		if (Tech->bAutoUnlock || Tech->RingTier == ERingTier::Core)
		{
			// Skip if already completed or currently researching.
			if (CompletedTechs.Contains(Tech) || Tech == CurrentResearch) continue;

			CompletedTechs.Add(Tech);
			ApplyTechEffect(Tech);

			UE_LOG(LogOLC, Log, TEXT("[OLC] Auto-unlocked Core tech: %s"), *Tech->DisplayName.ToString());
		}
	}

	UE_LOG(LogOLC, Display, TEXT("[OLC] Core tech auto-unlock complete — %d total completed"), CompletedTechs.Num());
	OnTechCompleted.Broadcast(nullptr);
}

void UOLCResearchSubsystem::RegisterStarterTechs()
{
	// -----------------------------------------------------------------------
	// CORE RING — auto-unlock at game start (no research needed)
	// -----------------------------------------------------------------------

	// --- Basic Wall & Gate Placement (Core, Buildings) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BasicWallPlacement"));
		Tech->DisplayName = FText::FromString(TEXT("Basic Wall & Gate Placement"));
		Tech->Category = EOLCTechCategory::Buildings;
		Tech->RingTier = ERingTier::Core;
		Tech->bAutoUnlock = true;
		Tech->ResearchTimeSeconds = 0.0f;
		Tech->EffectDescription = FText::FromString(TEXT("Unlocks wall and gate construction in building mode."));
		RegisterTech(Tech);
		RegisterBuildCardUnlock(Tech, TEXT("Wall Segment"));
	}

	// --- Basic Power Grid (Core, Energy) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BasicPowerGrid"));
		Tech->DisplayName = FText::FromString(TEXT("Basic Power Grid"));
		Tech->Category = EOLCTechCategory::Energy;
		Tech->RingTier = ERingTier::Core;
		Tech->bAutoUnlock = true;
		Tech->ResearchTimeSeconds = 0.0f;
		Tech->EffectDescription = FText::FromString(TEXT("Connect buildings to centralized power source."));
		RegisterTech(Tech);
	}

	// --- Basic Command Systems (Core, Buildings) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BasicCommand"));
		Tech->DisplayName = FText::FromString(TEXT("Basic Command Systems"));
		Tech->Category = EOLCTechCategory::Buildings;
		Tech->RingTier = ERingTier::Core;
		Tech->bAutoUnlock = true;
		Tech->ResearchTimeSeconds = 0.0f;
		Tech->EffectDescription = FText::FromString(TEXT("Enable command center and basic unit management."));
		RegisterTech(Tech);
	}

	// -----------------------------------------------------------------------
	// RING 1 — TIR 1-2 (prereqs: one or more Core techs)
	// -----------------------------------------------------------------------

	// --- Mine Placement (Ring 1, Buildings) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_MinePlacement"));
		Tech->DisplayName = FText::FromString(TEXT("Mine Placement"));
		Tech->Category = EOLCTechCategory::Buildings;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 180.0f; // 3 min per Briefing
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 100.0f, 100.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 80.0f, 80.0f);
		Tech->EffectDescription = FText::FromString(TEXT("Enable mine placement on discovered mineral deposits."));
		RegisterTech(Tech);
		RegisterBuildCardUnlock(Tech, TEXT("Mine"));
	}

	// --- Fusion Reactor Basics (Ring 1, Energy) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_FusionReactor"));
		Tech->DisplayName = FText::FromString(TEXT("Fusion Reactor Basics"));
		Tech->Category = EOLCTechCategory::Energy;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 240.0f; // 4 min
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 150.0f, 150.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 120.0f, 120.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Basic Power Grid")));
		Tech->EffectDescription = FText::FromString(TEXT("Unlock fusion reactors for high-output energy generation."));
		RegisterTech(Tech);
	}

	// --- Basic Defense Systems (Ring 1, Weapons) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BasicDefense"));
		Tech->DisplayName = FText::FromString(TEXT("Basic Defense Systems"));
		Tech->Category = EOLCTechCategory::Weapons;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 200.0f; // ~3.3 min
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 120.0f, 120.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 90.0f, 90.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Basic Wall & Gate Placement")));
		Tech->EffectDescription = FText::FromString(TEXT("Unlock turret and defense platform construction."));
		RegisterTech(Tech);
		RegisterBuildCardUnlock(Tech, TEXT("Turret"));
	}

	// --- Resource Processing (Ring 1, Buildings) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_ResourceProcessing"));
		Tech->DisplayName = FText::FromString(TEXT("Resource Processing"));
		Tech->Category = EOLCTechCategory::Buildings;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 200.0f; // ~3.3 min
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 130.0f, 130.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Basic Command Systems")));
		Tech->EffectDescription = FText::FromString(TEXT("Enable resource refineries and converters."));
		RegisterTech(Tech);
	}

	// -----------------------------------------------------------------------
	// RING 2 — TIR 2-3 (prereqs: one or more Ring 1 techs)
	// -----------------------------------------------------------------------

	// --- Advanced Weapon Systems (Ring 2, Weapons) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_AdvancedWeapons"));
		Tech->DisplayName = FText::FromString(TEXT("Advanced Weapon Systems"));
		Tech->Category = EOLCTechCategory::Weapons;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 300.0f; // 5 min
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 200.0f, 200.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 180.0f, 180.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Basic Defense Systems")));
		Tech->EffectDescription = FText::FromString(TEXT("Upgrade turrets with targeting arrays and multi-round capabilities."));
		RegisterTech(Tech);
	}

	// --- Mining Automation (Ring 2, Buildings) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_MiningAutomation"));
		Tech->DisplayName = FText::FromString(TEXT("Mining Automation"));
		Tech->Category = EOLCTechCategory::Buildings;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 300.0f; // 5 min
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 180.0f, 180.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Mine Placement")));
		Tech->EffectDescription = FText::FromString(TEXT("Automated mining drones increase extraction rates by 50%."));
		RegisterTech(Tech);
	}

	// --- Energy Shielding (Ring 2, Armor) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_EnergyShielding"));
		Tech->DisplayName = FText::FromString(TEXT("Energy Shielding"));
		Tech->Category = EOLCTechCategory::Armor;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 360.0f; // 6 min
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 220.0f, 220.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 150.0f, 150.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Fusion Reactor Basics")));
		Tech->EffectDescription = FText::FromString(TEXT("Deploy energy shields on buildings and units for damage mitigation."));
		RegisterTech(Tech);
	}

	// --- Orbital Scanning (Ring 2, Vision) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_OrbitalScanning"));
		Tech->DisplayName = FText::FromString(TEXT("Orbital Scanning"));
		Tech->Category = EOLCTechCategory::Vision;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 300.0f; // 5 min
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 160.0f, 160.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Basic Power Grid")));
		Tech->EffectDescription = FText::FromString(TEXT("Satellite scanning reveals hidden resources and enemy positions."));
		RegisterTech(Tech);
	}

	// --- Storage Expansion (Ring 2, Storage) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_StorageExpansion"));
		Tech->DisplayName = FText::FromString(TEXT("Storage Expansion"));
		Tech->Category = EOLCTechCategory::Storage;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 240.0f; // 4 min
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 140.0f, 140.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Basic Command Systems")));
		Tech->EffectDescription = FText::FromString(TEXT("Double all storage capacities for every resource type."));
		RegisterTech(Tech);
	}

	// -----------------------------------------------------------------------
	// RING 3 — TIR 3-4 (prereqs: one or more Ring 2 techs)
	// -----------------------------------------------------------------------

	// --- Dark Matter Drives (Ring 3, Drives) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_DarkMatterDrives"));
		Tech->DisplayName = FText::FromString(TEXT("Dark Matter Drives"));
		Tech->Category = EOLCTechCategory::Drives;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 420.0f; // 7 min
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 300.0f, 300.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 250.0f, 250.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::DarkMatterCrystals, 10.0f, 10.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Energy Shielding")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Fusion Reactor Basics")));
		Tech->EffectDescription = FText::FromString(TEXT("Enable faster-than-light travel between star systems."));
		RegisterTech(Tech);
	}

	// --- Quantum Computing (Ring 3, Vision) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_QuantumComputing"));
		Tech->DisplayName = FText::FromString(TEXT("Quantum Computing"));
		Tech->Category = EOLCTechCategory::Vision;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 480.0f; // 8 min
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 280.0f, 280.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Orbital Scanning")));
		Tech->EffectDescription = FText::FromString(TEXT("Predictive algorithms optimize production chains and research speed."));
		RegisterTech(Tech);
	}

	// --- Crystal Refining (Ring 3, Storage) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_CrystalRefining"));
		Tech->DisplayName = FText::FromString(TEXT("Crystal Refining"));
		Tech->Category = EOLCTechCategory::Storage;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 360.0f; // 6 min
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 240.0f, 240.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Mining Automation")));
		Tech->EffectDescription = FText::FromString(TEXT("Process dark matter crystals into usable energy cells."));
		RegisterTech(Tech);
	}

	// --- Advanced Combat AI (Ring 3, Weapons) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_AdvancedCombatAI"));
		Tech->DisplayName = FText::FromString(TEXT("Advanced Combat AI"));
		Tech->Category = EOLCTechCategory::Weapons;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 420.0f; // 7 min
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 260.0f, 260.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Advanced Weapon Systems")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Quantum Computing")));
		Tech->EffectDescription = FText::FromString(TEXT("AI-controlled units coordinate attacks and defend autonomously."));
		RegisterTech(Tech);
	}

	// -----------------------------------------------------------------------
	// OUTER RING — TIR 4-5 (prereqs: one or more Ring 3 techs)
	// -----------------------------------------------------------------------

	// --- Void Lab Research (Outer, Buildings) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_VoidLab"));
		Tech->DisplayName = FText::FromString(TEXT("Void Lab Research"));
		Tech->Category = EOLCTechCategory::Buildings;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 600.0f; // 10 min
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 500.0f, 500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 400.0f, 400.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::DarkMatterCrystals, 25.0f, 25.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Dark Matter Drives")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Crystal Refining")));
		Tech->EffectDescription = FText::FromString(TEXT("Research forbidden technologies from the void between galaxies."));
		RegisterTech(Tech);
	}

	// --- Advanced Assembly Plants (Outer, Buildings) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_AdvancedAssembly"));
		Tech->DisplayName = FText::FromString(TEXT("Advanced Assembly Plants"));
		Tech->Category = EOLCTechCategory::Buildings;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 540.0f; // 9 min
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 450.0f, 450.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Quantum Computing")));
		Tech->EffectDescription = FText::FromString(TEXT("Self-assembling factories produce units at triple speed."));
		RegisterTech(Tech);
	}

	// --- Dark Matter Crystals (Outer, Energy) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_DarkMatterEnergy"));
		Tech->DisplayName = FText::FromString(TEXT("Dark Matter Energy"));
		Tech->Category = EOLCTechCategory::Energy;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 600.0f; // 10 min
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 500.0f, 500.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Crystal Refining")));
		Tech->EffectDescription = FText::FromString(TEXT("Harness dark matter as a near-infinite energy source."));
		RegisterTech(Tech);
	}

	// --- Unit Production (Outer, Units) ---
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_UnitProduction"));
		Tech->DisplayName = FText::FromString(TEXT("Unit Production"));
		Tech->Category = EOLCTechCategory::Units;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 480.0f; // 8 min
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 350.0f, 350.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Advanced Combat AI")));
		Tech->EffectDescription = FText::FromString(TEXT("Unlock direct unit construction from orbital dropships."));
		RegisterTech(Tech);
	}

	// -----------------------------------------------------------------------
	// The 20 topics above were the original starter set (WP-09/WP-120 step 1-2).
	// Everything below fills out the full 89-topic tree from
	// Briefing/Tech-Tree/Research-Categories/*/README.md (WP-120 step 3) —
	// added directly in C++ rather than as 89 individually MCP-created
	// DataAssets, since UOLCTechData instances are already transient
	// NewObject()s constructed here, not persistent .uasset files; no MCP
	// round trip is needed for any of this. Registration order below is
	// Core -> Ring1 -> Ring2 -> Ring3 -> Outer, and within a ring, a
	// category that's a FindTechByName() prerequisite for another category
	// is registered first, so every lookup below resolves to an
	// already-registered tech (matching this function's existing
	// no-null-check convention).
	// -----------------------------------------------------------------------

	// ===== CORE — additional per-category starting topics =====
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_CompactFusionReactor"));
		Tech->DisplayName = FText::FromString(TEXT("Compact Fusion Reactor"));
		Tech->Category = EOLCTechCategory::Energy;
		Tech->RingTier = ERingTier::Core;
		Tech->bAutoUnlock = true;
		Tech->ResearchTimeSeconds = 0.0f;
		Tech->EffectDescription = FText::FromString(TEXT("Starting energy core — 50 energy capacity powering all dropship systems."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BasicBallisticRifle"));
		Tech->DisplayName = FText::FromString(TEXT("Basic Ballistic Rifle"));
		Tech->Category = EOLCTechCategory::Weapons;
		Tech->RingTier = ERingTier::Core;
		Tech->bAutoUnlock = true;
		Tech->ResearchTimeSeconds = 0.0f;
		Tech->EffectDescription = FText::FromString(TEXT("Standard infantry weapon, 8 damage per hit, 15m range, unlimited ammo."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BasicHullPlating"));
		Tech->DisplayName = FText::FromString(TEXT("Basic Hull Plating"));
		Tech->Category = EOLCTechCategory::Armor;
		Tech->RingTier = ERingTier::Core;
		Tech->bAutoUnlock = true;
		Tech->ResearchTimeSeconds = 0.0f;
		Tech->EffectDescription = FText::FromString(TEXT("Standard dropship hull, 35% integrity at crash landing."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BasicChemicalRocketDrive"));
		Tech->DisplayName = FText::FromString(TEXT("Basic Chemical Rocket Drive"));
		Tech->Category = EOLCTechCategory::Drives;
		Tech->RingTier = ERingTier::Core;
		Tech->bAutoUnlock = true;
		Tech->ResearchTimeSeconds = 0.0f;
		Tech->EffectDescription = FText::FromString(TEXT("Standard dropship drive, 50 kN thrust per engine. Right drive functional, left drive damaged at crash."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BasicDropshipStorageLocker"));
		Tech->DisplayName = FText::FromString(TEXT("Basic Dropship Storage Locker"));
		Tech->Category = EOLCTechCategory::Storage;
		Tech->RingTier = ERingTier::Core;
		Tech->bAutoUnlock = true;
		Tech->ResearchTimeSeconds = 0.0f;
		Tech->EffectDescription = FText::FromString(TEXT("200 resource capacity per type, ~40% full with scattered supplies at crash."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BasicSoldierTraining"));
		Tech->DisplayName = FText::FromString(TEXT("Basic Soldier Training"));
		Tech->Category = EOLCTechCategory::Units;
		Tech->RingTier = ERingTier::Core;
		Tech->bAutoUnlock = true;
		Tech->ResearchTimeSeconds = 0.0f;
		Tech->EffectDescription = FText::FromString(TEXT("10 cryo-stasis soldiers wake over 10 minutes; control up to 4 simultaneously at start."));
		RegisterTech(Tech);
	}

	// ===== RING 1 — Basic Forge (Energy first: Armor's shield gen needs Power Distribution I) =====
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_SolarPanelInstallation"));
		Tech->DisplayName = FText::FromString(TEXT("Solar Panel Installation"));
		Tech->Category = EOLCTechCategory::Energy;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 180.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 80.0f, 80.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 60.0f, 60.0f);
		Tech->EffectDescription = FText::FromString(TEXT("Solar array power generation on planet surface, 20 energy/turn in direct sunlight."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BatteryStorageI"));
		Tech->DisplayName = FText::FromString(TEXT("Battery Storage I"));
		Tech->Category = EOLCTechCategory::Energy;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 120.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 100.0f, 100.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 80.0f, 80.0f);
		Tech->EffectDescription = FText::FromString(TEXT("+50 energy capacity on dropship (total 100 from starting 50)."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_PowerDistributionI"));
		Tech->DisplayName = FText::FromString(TEXT("Power Distribution I"));
		Tech->Category = EOLCTechCategory::Energy;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 180.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 150.0f, 150.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 100.0f, 100.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Battery Storage I")));
		Tech->EffectDescription = FText::FromString(TEXT("Enable weapon power routing from the energy core to turrets and mounted weapons."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BallisticTurretMounting"));
		Tech->DisplayName = FText::FromString(TEXT("Ballistic Turret Mounting"));
		Tech->Category = EOLCTechCategory::Weapons;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 120.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 100.0f, 100.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 80.0f, 80.0f);
		Tech->EffectDescription = FText::FromString(TEXT("Enable turret installation on dropship external mounts and planet base walls."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_AmmoCraftingI"));
		Tech->DisplayName = FText::FromString(TEXT("Ammo Crafting I"));
		Tech->Category = EOLCTechCategory::Weapons;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 180.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 50.0f, 50.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 30.0f, 30.0f);
		Tech->EffectDescription = FText::FromString(TEXT("Unlock standard ammunition production at the forge bench."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_RocketBayBasic"));
		Tech->DisplayName = FText::FromString(TEXT("Rocket Bay Basic"));
		Tech->Category = EOLCTechCategory::Weapons;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 300.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 200.0f, 200.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 150.0f, 150.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Ballistic Turret Mounting")));
		Tech->EffectDescription = FText::FromString(TEXT("Enable small rocket bay (Standard HE, Incendiary, Armor-Piercing) for ship and base turrets."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_ReinforcedPlatingI"));
		Tech->DisplayName = FText::FromString(TEXT("Reinforced Plating I"));
		Tech->Category = EOLCTechCategory::Armor;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 180.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 150.0f, 150.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 100.0f, 100.0f);
		Tech->EffectDescription = FText::FromString(TEXT("Unit armor upgrade tier 1->2 — +50% HP, +10 Defense for infantry and light vehicles."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_HullRepairBasics"));
		Tech->DisplayName = FText::FromString(TEXT("Hull Repair Basics"));
		Tech->Category = EOLCTechCategory::Armor;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 120.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 100.0f, 100.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 50.0f, 50.0f);
		Tech->EffectDescription = FText::FromString(TEXT("Restore dropship hull integrity from 35% to 85%+ from crash state."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BasicShieldGeneration"));
		Tech->DisplayName = FText::FromString(TEXT("Basic Shield Generation"));
		Tech->Category = EOLCTechCategory::Armor;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 300.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 300.0f, 300.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 200.0f, 200.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Hull Repair Basics")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Power Distribution I")));
		Tech->EffectDescription = FText::FromString(TEXT("+100 HP equivalent energy shield for the dropship, 30s recharge after depletion."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_LeftDriveRepair"));
		Tech->DisplayName = FText::FromString(TEXT("Left Drive Repair"));
		Tech->Category = EOLCTechCategory::Drives;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 900.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 100.0f, 100.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 50.0f, 50.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 30.0f, 30.0f);
		Tech->EffectDescription = FText::FromString(TEXT("Restore left rocket drive to 90% operational capacity."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_FuelProcessingI"));
		Tech->DisplayName = FText::FromString(TEXT("Fuel Processing I"));
		Tech->Category = EOLCTechCategory::Drives;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 300.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 100.0f, 100.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 80.0f, 80.0f);
		Tech->EffectDescription = FText::FromString(TEXT("Enable oil pump and basic fuel refinement — chemical propellant and crude oil to rocket fuel."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_DriveCalibrationI"));
		Tech->DisplayName = FText::FromString(TEXT("Drive Calibration I"));
		Tech->Category = EOLCTechCategory::Drives;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 180.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 50.0f, 50.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 20.0f, 20.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Left Drive Repair")));
		Tech->EffectDescription = FText::FromString(TEXT("+10% drive efficiency for both rocket drives, reduces fuel use from 100 to 90 units/hop."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BasicRadarArray"));
		Tech->DisplayName = FText::FromString(TEXT("Basic Radar Array"));
		Tech->Category = EOLCTechCategory::Vision;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 300.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 200.0f, 200.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 150.0f, 150.0f);
		Tech->EffectDescription = FText::FromString(TEXT("Short-medium radial scan range — 200m on planet surface, 5km in space."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_VisualEnhancementI"));
		Tech->DisplayName = FText::FromString(TEXT("Visual Enhancement I"));
		Tech->Category = EOLCTechCategory::Vision;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 120.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 50.0f, 50.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 20.0f, 20.0f);
		Tech->EffectDescription = FText::FromString(TEXT("+20% unit detection radius for all ground units, +20% ambush detection."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_SignalProcessingI"));
		Tech->DisplayName = FText::FromString(TEXT("Signal Processing I"));
		Tech->Category = EOLCTechCategory::Vision;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 180.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 100.0f, 100.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 80.0f, 80.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Basic Radar Array")));
		Tech->EffectDescription = FText::FromString(TEXT("Radar clutter reduction, effective range +17%, +10% weapon lock-on speed."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_ContainerPlacement"));
		Tech->DisplayName = FText::FromString(TEXT("Container Placement"));
		Tech->Category = EOLCTechCategory::Storage;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 120.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 300.0f, 300.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 100.0f, 100.0f);
		Tech->EffectDescription = FText::FromString(TEXT("Enable container storage, 2,000 resources per type (10x starting locker)."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_StorageOrganizationI"));
		Tech->DisplayName = FText::FromString(TEXT("Storage Organization I"));
		Tech->Category = EOLCTechCategory::Storage;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 60.0f;
		Tech->Prerequisites.Add(FindTechByName(TEXT("Container Placement")));
		Tech->EffectDescription = FText::FromString(TEXT("Auto-sort buttons, category icons, and search for inventory management."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_ResourceLabeling"));
		Tech->DisplayName = FText::FromString(TEXT("Resource Labeling"));
		Tech->Category = EOLCTechCategory::Storage;
		Tech->RingTier = ERingTier::Ring1;
		Tech->bAutoUnlock = true;
		Tech->ResearchTimeSeconds = 0.0f;
		Tech->Prerequisites.Add(FindTechByName(TEXT("Storage Organization I")));
		Tech->EffectDescription = FText::FromString(TEXT("Automatic color-coded resource categorization and stack sorting across containers."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_UnitControlGroupsI"));
		Tech->DisplayName = FText::FromString(TEXT("Unit Control Groups I"));
		Tech->Category = EOLCTechCategory::Units;
		Tech->RingTier = ERingTier::Ring1;
		Tech->ResearchTimeSeconds = 180.0f;
		Tech->EffectDescription = FText::FromString(TEXT("Enable numbered unit group assignment (4 groups) for simultaneous control — requires Command Center."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BasicFormationCommands"));
		Tech->DisplayName = FText::FromString(TEXT("Basic Formation Commands"));
		Tech->Category = EOLCTechCategory::Units;
		Tech->RingTier = ERingTier::Ring1;
		Tech->bAutoUnlock = true;
		Tech->ResearchTimeSeconds = 0.0f;
		Tech->Prerequisites.Add(FindTechByName(TEXT("Unit Control Groups I")));
		Tech->EffectDescription = FText::FromString(TEXT("Squad formations — Line, Column, Diamond, V-Shape, Scatter — each with tactical bonuses."));
		RegisterTech(Tech);
	}

	// ===== RING 2 — Energy Lab (Buildings + Energy first: several others depend on them) =====
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_FactoryConstruction"));
		Tech->DisplayName = FText::FromString(TEXT("Factory Construction"));
		Tech->Category = EOLCTechCategory::Buildings;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 900.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 800.0f, 800.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 500.0f, 500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 200.0f, 200.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Basic Power Grid")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Mine Placement")));
		Tech->EffectDescription = FText::FromString(TEXT("Vehicle and module production on planet surface."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_ForgeUpgradePath"));
		Tech->DisplayName = FText::FromString(TEXT("Forge Upgrade Path"));
		Tech->Category = EOLCTechCategory::Buildings;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 600.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 600.0f, 600.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 400.0f, 400.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Factory Construction")));
		Tech->EffectDescription = FText::FromString(TEXT("Enable Energy Lab construction, upgrading Basic Forge to a TIR 3 research facility."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_AdvancedPowerGrid"));
		Tech->DisplayName = FText::FromString(TEXT("Advanced Power Grid"));
		Tech->Category = EOLCTechCategory::Buildings;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 480.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 500.0f, 500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 400.0f, 400.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Basic Power Grid")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Factory Construction")));
		Tech->EffectDescription = FText::FromString(TEXT("Automated power routing across all planet bases, grid range 500m, transmission loss down to 2%."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_WindTurbineOptimization"));
		Tech->DisplayName = FText::FromString(TEXT("Wind Turbine Optimization"));
		Tech->Category = EOLCTechCategory::Energy;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 480.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 400.0f, 400.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 300.0f, 300.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Solar Panel Installation")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Power Distribution I")));
		Tech->EffectDescription = FText::FromString(TEXT("+25% output in windy biomes for wind turbine generation."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BatteryStorageII"));
		Tech->DisplayName = FText::FromString(TEXT("Battery Storage II"));
		Tech->Category = EOLCTechCategory::Energy;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 300.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 300.0f, 300.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 200.0f, 200.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Battery Storage I")));
		Tech->EffectDescription = FText::FromString(TEXT("+200 energy capacity on dropship (total 300 from starting 50)."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_PowerDistributionII"));
		Tech->DisplayName = FText::FromString(TEXT("Power Distribution II"));
		Tech->Category = EOLCTechCategory::Energy;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 480.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 500.0f, 500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 400.0f, 400.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Power Distribution I")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Battery Storage II")));
		Tech->EffectDescription = FText::FromString(TEXT("Enable multiple simultaneous weapon systems, 60 energy/turn weapon allocation."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_PlasmaCannonMounting"));
		Tech->DisplayName = FText::FromString(TEXT("Plasma Cannon Mounting"));
		Tech->Category = EOLCTechCategory::Weapons;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 480.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 400.0f, 400.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 300.0f, 300.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Ballistic Turret Mounting")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Ammo Crafting I")));
		Tech->EffectDescription = FText::FromString(TEXT("Enable energy weapon deployment on ship and planet bases, 35m range."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_AmmoCraftingII"));
		Tech->DisplayName = FText::FromString(TEXT("Ammo Crafting II"));
		Tech->Category = EOLCTechCategory::Weapons;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 300.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 200.0f, 200.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 100.0f, 100.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Ammo Crafting I")));
		Tech->EffectDescription = FText::FromString(TEXT("Unlock armor-piercing and incendiary rounds for ballistic weapons."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_RocketBayAdvanced"));
		Tech->DisplayName = FText::FromString(TEXT("Rocket Bay Advanced"));
		Tech->Category = EOLCTechCategory::Weapons;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 600.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 600.0f, 600.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 400.0f, 400.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Rocket Bay Basic")));
		Tech->EffectDescription = FText::FromString(TEXT("Large rocket bay — Cluster, EMP, and Thermobaric warheads, +100% ammo capacity."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_TitaniumCompositeArmor"));
		Tech->DisplayName = FText::FromString(TEXT("Titanium Composite Armor"));
		Tech->Category = EOLCTechCategory::Armor;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 600.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 800.0f, 800.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 500.0f, 500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 200.0f, 200.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Reinforced Plating I")));
		Tech->EffectDescription = FText::FromString(TEXT("Unit armor upgrade tier 2->3 — +100% HP (total +150%), +25 Defense."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_HullReinforcementII"));
		Tech->DisplayName = FText::FromString(TEXT("Hull Reinforcement II"));
		Tech->Category = EOLCTechCategory::Armor;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 480.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 600.0f, 600.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 400.0f, 400.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Hull Repair Basics")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Titanium Composite Armor")));
		Tech->EffectDescription = FText::FromString(TEXT("Heat-resistant alloy hull, +200% thermal resistance, +15% max hull capacity."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_EnergyShieldGenerationII"));
		Tech->DisplayName = FText::FromString(TEXT("Energy Shield Generation II"));
		Tech->Category = EOLCTechCategory::Armor;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 720.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 500.0f, 500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 300.0f, 300.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 200.0f, 200.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Basic Shield Generation")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Power Distribution II")));
		Tech->EffectDescription = FText::FromString(TEXT("200 HP shield (double basic), 15s recharge, toggleable efficiency mode."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_AtomicReactorDrive"));
		Tech->DisplayName = FText::FromString(TEXT("Atomic Reactor Drive"));
		Tech->Category = EOLCTechCategory::Drives;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 900.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 800.0f, 800.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 600.0f, 600.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Left Drive Repair")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Fuel Processing I")));
		Tech->EffectDescription = FText::FromString(TEXT("-40% fuel consumption, +50% thrust, produces 30 energy/turn as byproduct."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_FuelProcessingII"));
		Tech->DisplayName = FText::FromString(TEXT("Fuel Processing II"));
		Tech->Category = EOLCTechCategory::Drives;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 480.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 400.0f, 400.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 300.0f, 300.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Fuel Processing I")));
		Tech->EffectDescription = FText::FromString(TEXT("Enable biofuel and frozen methane processing, up to 140% more efficient than crude oil."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_DriveCalibrationII"));
		Tech->DisplayName = FText::FromString(TEXT("Drive Calibration II"));
		Tech->Category = EOLCTechCategory::Drives;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 300.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 200.0f, 200.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 100.0f, 100.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Drive Calibration I")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Atomic Reactor Drive")));
		Tech->EffectDescription = FText::FromString(TEXT("+20% additional drive efficiency (total +30% with Calibration I)."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_DeepSonarArray"));
		Tech->DisplayName = FText::FromString(TEXT("Deep Sonar Array"));
		Tech->Category = EOLCTechCategory::Vision;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 600.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 500.0f, 500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 400.0f, 400.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Basic Radar Array")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Signal Processing I")));
		Tech->EffectDescription = FText::FromString(TEXT("Underground structure detection, reveals buried dungeons up to 50m deep."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_ThermalImaging"));
		Tech->DisplayName = FText::FromString(TEXT("Thermal Imaging"));
		Tech->Category = EOLCTechCategory::Vision;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 480.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 300.0f, 300.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 200.0f, 200.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Visual Enhancement I")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Signal Processing I")));
		Tech->EffectDescription = FText::FromString(TEXT("+100% detection range through terrain cover, sees through smoke/fog/foliage."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_LaserTargetingSystem"));
		Tech->DisplayName = FText::FromString(TEXT("Laser Targeting System"));
		Tech->Category = EOLCTechCategory::Vision;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 720.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 600.0f, 600.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 500.0f, 500.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Deep Sonar Array")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Thermal Imaging")));
		Tech->EffectDescription = FText::FromString(TEXT("Precise single-target weapon lock, -50% lock-on time, +25% hit probability."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_HaulStorageConstruction"));
		Tech->DisplayName = FText::FromString(TEXT("Haul Storage Construction"));
		Tech->Category = EOLCTechCategory::Storage;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 600.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 1500.0f, 1500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 500.0f, 500.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Container Placement")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Storage Organization I")));
		Tech->EffectDescription = FText::FromString(TEXT("10,000 resources per type capacity, 50% crash-protection recovery chance."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_StorageOrganizationII"));
		Tech->DisplayName = FText::FromString(TEXT("Storage Organization II"));
		Tech->Category = EOLCTechCategory::Storage;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 300.0f;
		Tech->Prerequisites.Add(FindTechByName(TEXT("Storage Organization I")));
		Tech->EffectDescription = FText::FromString(TEXT("Advanced filtering, multi-select, drag-and-drop bulk transfers, cross-storage search."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_ResourceConversionI"));
		Tech->DisplayName = FText::FromString(TEXT("Resource Conversion I"));
		Tech->Category = EOLCTechCategory::Storage;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 480.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 800.0f, 800.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 600.0f, 600.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Storage Organization II")));
		Tech->EffectDescription = FText::FromString(TEXT("Convert between resource types at 70% yield."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_HeavyVehicleProduction"));
		Tech->DisplayName = FText::FromString(TEXT("Heavy Vehicle Production"));
		Tech->Category = EOLCTechCategory::Units;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 900.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 800.0f, 800.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 600.0f, 600.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Fuel, 200.0f, 200.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Factory Construction")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Unit Control Groups I")));
		Tech->EffectDescription = FText::FromString(TEXT("Tank and walker training at the factory — Light Tank and Heavy Walker."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_UnitControlGroupsII"));
		Tech->DisplayName = FText::FromString(TEXT("Unit Control Groups II"));
		Tech->Category = EOLCTechCategory::Units;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 300.0f;
		Tech->Prerequisites.Add(FindTechByName(TEXT("Unit Control Groups I")));
		Tech->EffectDescription = FText::FromString(TEXT("+3 additional control group slots (total 7); scales further with champion Intelligence."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_AerialUnitDeployment"));
		Tech->DisplayName = FText::FromString(TEXT("Aerial Unit Deployment"));
		Tech->Category = EOLCTechCategory::Units;
		Tech->RingTier = ERingTier::Ring2;
		Tech->ResearchTimeSeconds = 600.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 400.0f, 400.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 300.0f, 300.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Heavy Vehicle Production")));
		Tech->EffectDescription = FText::FromString(TEXT("Scout Drone and Fighter Jet training at the airfield."));
		RegisterTech(Tech);
	}

	// ===== RING 3 — Dark Matter Lab (Vision first: Buildings' Orbital Strike Beacon needs it) =====
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_SatelliteDeploymentBay"));
		Tech->DisplayName = FText::FromString(TEXT("Satellite Deployment Bay"));
		Tech->Category = EOLCTechCategory::Vision;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 900.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 800.0f, 800.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 600.0f, 600.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Deep Sonar Array")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Laser Targeting System")));
		Tech->EffectDescription = FText::FromString(TEXT("Planet-wide monitoring with up to 3 satellites, ~99% total surface coverage."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_TacticalAIAssistant"));
		Tech->DisplayName = FText::FromString(TEXT("Tactical AI Assistant"));
		Tech->Category = EOLCTechCategory::Vision;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 720.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 600.0f, 600.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 400.0f, 400.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::DarkMatterCrystals, 200.0f, 200.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Thermal Imaging")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Laser Targeting System")));
		Tech->EffectDescription = FText::FromString(TEXT("Auto-target priority for highest threat, +150% detection range, +25% AI attack speed."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_QuantumRangeFinder"));
		Tech->DisplayName = FText::FromString(TEXT("Quantum Range Finder"));
		Tech->Category = EOLCTechCategory::Vision;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 600.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 500.0f, 500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 300.0f, 300.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Satellite Deployment Bay")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Tactical AI Assistant")));
		Tech->EffectDescription = FText::FromString(TEXT("Instant distance calculation and automatic firing solutions for all map features."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_VoidLabConstruction"));
		Tech->DisplayName = FText::FromString(TEXT("Void Lab Construction"));
		Tech->Category = EOLCTechCategory::Buildings;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 1200.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 2000.0f, 2000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 1500.0f, 1500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 500.0f, 500.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Forge Upgrade Path")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Factory Construction")));
		Tech->EffectDescription = FText::FromString(TEXT("TIR 5 research facility unlocking all Outer Ring technology, +100% research speed."));
		Tech->ResearchSpeedBonusPercent = 100.0f;
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_AssemblyPlant"));
		Tech->DisplayName = FText::FromString(TEXT("Assembly Plant"));
		Tech->Category = EOLCTechCategory::Buildings;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 1500.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 3000.0f, 3000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 2000.0f, 2000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 1000.0f, 1000.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Factory Construction")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Advanced Power Grid")));
		Tech->EffectDescription = FText::FromString(TEXT("3x factory production rate, +10% durability on all manufactured items."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_OrbitalStrikeBeacon"));
		Tech->DisplayName = FText::FromString(TEXT("Orbital Strike Beacon"));
		Tech->Category = EOLCTechCategory::Buildings;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 1200.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 2000.0f, 2000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 1500.0f, 1500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 800.0f, 800.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Assembly Plant")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Satellite Deployment Bay")));
		Tech->EffectDescription = FText::FromString(TEXT("Planet-wide bombardment — Artillery, Precision Laser, and EMP strike types."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_TorpedoLauncher"));
		Tech->DisplayName = FText::FromString(TEXT("Torpedo Launcher"));
		Tech->Category = EOLCTechCategory::Weapons;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 900.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 800.0f, 800.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 600.0f, 600.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 200.0f, 200.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Rocket Bay Advanced")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Plasma Cannon Mounting")));
		Tech->EffectDescription = FText::FromString(TEXT("Homing multi-target weapon, auto-acquires up to 3 targets, 80m range."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_PrecisionTargetingSystem"));
		Tech->DisplayName = FText::FromString(TEXT("Precision Targeting System"));
		Tech->Category = EOLCTechCategory::Weapons;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 720.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 600.0f, 600.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 400.0f, 400.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 200.0f, 200.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Ammo Crafting II")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Laser Targeting System")));
		Tech->EffectDescription = FText::FromString(TEXT("+100% damage, +40% range, +25% accuracy for all weapon systems."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_IonWeaponIntegration"));
		Tech->DisplayName = FText::FromString(TEXT("Ion Weapon Integration"));
		Tech->Category = EOLCTechCategory::Weapons;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 900.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 700.0f, 700.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 500.0f, 500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 300.0f, 300.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Plasma Cannon Mounting")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Precision Targeting System")));
		Tech->EffectDescription = FText::FromString(TEXT("EMP burst and ion storm for ship defense and planet base offense."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_EnergyInfusedArmor"));
		Tech->DisplayName = FText::FromString(TEXT("Energy-Infused Armor"));
		Tech->Category = EOLCTechCategory::Armor;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 900.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 1200.0f, 1200.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 800.0f, 800.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 500.0f, 500.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Titanium Composite Armor")));
		Tech->EffectDescription = FText::FromString(TEXT("Unit armor tier 3->4 — +200% HP (total +350%), +40 Defense, 10% melee reflection."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_HullReinforcementIII"));
		Tech->DisplayName = FText::FromString(TEXT("Hull Reinforcement III"));
		Tech->Category = EOLCTechCategory::Armor;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 720.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 1000.0f, 1000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 700.0f, 700.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Hull Reinforcement II")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Titanium Composite Armor")));
		Tech->EffectDescription = FText::FromString(TEXT("Titanium alloy hull, +400% thermal protection, maintains 100% integrity in combat."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_AdvancedShieldGeneration"));
		Tech->DisplayName = FText::FromString(TEXT("Advanced Shield Generation"));
		Tech->Category = EOLCTechCategory::Armor;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 1080.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 800.0f, 800.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 600.0f, 600.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 400.0f, 400.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Energy Shield Generation II")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Ion Weapon Integration")));
		Tech->EffectDescription = FText::FromString(TEXT("350 HP shield, full 360 coverage, 5% melee damage reflection."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_IonicReactorDrive"));
		Tech->DisplayName = FText::FromString(TEXT("Ionic Reactor Drive"));
		Tech->Category = EOLCTechCategory::Drives;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 1200.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 1200.0f, 1200.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 800.0f, 800.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Atomic Reactor Drive")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Fuel Processing II")));
		Tech->EffectDescription = FText::FromString(TEXT("-60% fuel consumption from base rate, +100% thrust over chemical drives."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_FuelProcessingIII"));
		Tech->DisplayName = FText::FromString(TEXT("Fuel Processing III"));
		Tech->Category = EOLCTechCategory::Drives;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 720.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 600.0f, 600.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 500.0f, 500.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Fuel Processing II")));
		Tech->EffectDescription = FText::FromString(TEXT("+25% dark matter fuel efficiency bonus to all processed fuels."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_DriveCalibrationIII"));
		Tech->DisplayName = FText::FromString(TEXT("Drive Calibration III"));
		Tech->Category = EOLCTechCategory::Drives;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 480.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 400.0f, 400.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 300.0f, 300.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Drive Calibration II")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Ionic Reactor Drive")));
		Tech->EffectDescription = FText::FromString(TEXT("+30% total drive efficiency (stacked total +60% with Calibration I+II)."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_GeothermalVentHarnessing"));
		Tech->DisplayName = FText::FromString(TEXT("Geothermal Vent Harnessing"));
		Tech->Category = EOLCTechCategory::Energy;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 900.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 800.0f, 800.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 600.0f, 600.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Solar Panel Installation")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Wind Turbine Optimization")));
		Tech->EffectDescription = FText::FromString(TEXT("100 energy/turn from geothermal vents (5x solar base output)."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BatteryStorageIII"));
		Tech->DisplayName = FText::FromString(TEXT("Battery Storage III"));
		Tech->Category = EOLCTechCategory::Energy;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 480.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 600.0f, 600.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 400.0f, 400.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Battery Storage II")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Power Distribution II")));
		Tech->EffectDescription = FText::FromString(TEXT("+500 energy capacity on dropship (total 800 from starting 50)."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_PowerDistributionIII"));
		Tech->DisplayName = FText::FromString(TEXT("Power Distribution III"));
		Tech->Category = EOLCTechCategory::Energy;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 720.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 800.0f, 800.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 600.0f, 600.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Power Distribution II")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Battery Storage III")));
		Tech->EffectDescription = FText::FromString(TEXT("Enable sustained Void Beam power draw, 150 energy/turn weapon allocation."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_ReinforcedVault"));
		Tech->DisplayName = FText::FromString(TEXT("Reinforced Vault"));
		Tech->Category = EOLCTechCategory::Storage;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 900.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 3000.0f, 3000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 2000.0f, 2000.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Haul Storage Construction")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Resource Conversion I")));
		Tech->EffectDescription = FText::FromString(TEXT("25,000 resources per type, 100% crash-protection content retention."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_ResourceConversionII"));
		Tech->DisplayName = FText::FromString(TEXT("Resource Conversion II"));
		Tech->Category = EOLCTechCategory::Storage;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 600.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 800.0f, 800.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 600.0f, 600.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Resource Conversion I")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Haul Storage Construction")));
		Tech->EffectDescription = FText::FromString(TEXT("75% conversion yield (up from 70%), unlocks dark matter crystal conversion."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_RemoteTransferNetwork"));
		Tech->DisplayName = FText::FromString(TEXT("Remote Transfer Network"));
		Tech->Category = EOLCTechCategory::Storage;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 720.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 1000.0f, 1000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 800.0f, 800.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Reinforced Vault")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Resource Conversion II")));
		Tech->EffectDescription = FText::FromString(TEXT("Instant quantum-gated resource sharing between linked bases, unlimited range."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_HeavyMechProduction"));
		Tech->DisplayName = FText::FromString(TEXT("Heavy Mech Production"));
		Tech->Category = EOLCTechCategory::Units;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 1500.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 2500.0f, 2500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 1500.0f, 1500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Fuel, 500.0f, 500.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Heavy Vehicle Production")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Assembly Plant")));
		Tech->EffectDescription = FText::FromString(TEXT("Heavy Combat Mech — dual plasma cannons, missile pod, energy shield generator."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_UnitControlGroupsIII"));
		Tech->DisplayName = FText::FromString(TEXT("Unit Control Groups III"));
		Tech->Category = EOLCTechCategory::Units;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 480.0f;
		Tech->Prerequisites.Add(FindTechByName(TEXT("Unit Control Groups II")));
		Tech->EffectDescription = FText::FromString(TEXT("+5 additional control group slots (total 12+); scales further with champion Intelligence."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_EliteSquadTraining"));
		Tech->DisplayName = FText::FromString(TEXT("Elite Squad Training"));
		Tech->Category = EOLCTechCategory::Units;
		Tech->RingTier = ERingTier::Ring3;
		Tech->ResearchTimeSeconds = 900.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 1000.0f, 1000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 800.0f, 800.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Survival, 300.0f, 300.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Heavy Mech Production")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Unit Control Groups III")));
		Tech->EffectDescription = FText::FromString(TEXT("Shock Trooper and Elite Sniper training — void-infused armor, precision weapons."));
		RegisterTech(Tech);
	}

	// ===== OUTER RING — Void Lab (Energy first: Buildings' Dyson Swarm needs Zero-Point) =====
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_CrystalSynthesizerIntegration"));
		Tech->DisplayName = FText::FromString(TEXT("Crystal Synthesizer Integration"));
		Tech->Category = EOLCTechCategory::Energy;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 1200.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 1500.0f, 1500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 800.0f, 800.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Geothermal Vent Harnessing")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Battery Storage III")));
		Tech->EffectDescription = FText::FromString(TEXT("Converts raw energy to dark matter crystals — 1 crystal per 50 energy input."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BatteryStorageV"));
		Tech->DisplayName = FText::FromString(TEXT("Battery Storage V"));
		Tech->Category = EOLCTechCategory::Energy;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 600.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 1000.0f, 1000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 800.0f, 800.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Battery Storage III")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Power Distribution III")));
		Tech->EffectDescription = FText::FromString(TEXT("+1,000 energy capacity on dropship (total 1,800 from starting 50)."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_ZeroPointEnergyExtraction"));
		Tech->DisplayName = FText::FromString(TEXT("Zero-Point Energy Extraction"));
		Tech->Category = EOLCTechCategory::Energy;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 1800.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 2000.0f, 2000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::DarkMatterCrystals, 1500.0f, 1500.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Crystal Synthesizer Integration")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Power Distribution III")));
		Tech->EffectDescription = FText::FromString(TEXT("Near-infinite energy from vacuum fluctuations, capped only by distribution capacity."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_QuantumScanner"));
		Tech->DisplayName = FText::FromString(TEXT("Quantum Scanner"));
		Tech->Category = EOLCTechCategory::Vision;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 1200.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 2000.0f, 2000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 1500.0f, 1500.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Satellite Deployment Bay")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Quantum Range Finder")));
		Tech->EffectDescription = FText::FromString(TEXT("Planet-wide instant map reveal — full resolution, no scan time required."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_DeepVoidSensors"));
		Tech->DisplayName = FText::FromString(TEXT("Deep Void Sensors"));
		Tech->Category = EOLCTechCategory::Vision;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 900.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 1000.0f, 1000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::DarkMatterCrystals, 800.0f, 800.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Quantum Scanner")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Satellite Deployment Bay")));
		Tech->EffectDescription = FText::FromString(TEXT("Unlimited-range deep space object detection across light-years."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_TemporalRadar"));
		Tech->DisplayName = FText::FromString(TEXT("Temporal Radar"));
		Tech->Category = EOLCTechCategory::Vision;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 1500.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 1500.0f, 1500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::DarkMatterCrystals, 1200.0f, 1200.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Deep Void Sensors")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Tactical AI Assistant")));
		Tech->EffectDescription = FText::FromString(TEXT("Predicts enemy movement 3 seconds ahead, +20% evasion when moving."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_VoidBeam"));
		Tech->DisplayName = FText::FromString(TEXT("Void Beam"));
		Tech->Category = EOLCTechCategory::Weapons;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 1200.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 1500.0f, 1500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 1000.0f, 1000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 500.0f, 500.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Torpedo Launcher")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Precision Targeting System")));
		Tech->EffectDescription = FText::FromString(TEXT("Extreme damage beam that ignores all armor types, 120m range."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_VoidCoreWeaponry"));
		Tech->DisplayName = FText::FromString(TEXT("Void Core Weaponry"));
		Tech->Category = EOLCTechCategory::Weapons;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 1500.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 2000.0f, 2000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::DarkMatterCrystals, 1500.0f, 1500.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Precision Targeting System")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Torpedo Launcher")));
		Tech->EffectDescription = FText::FromString(TEXT("Unit weapon upgrade — +150% damage, +60% range, ignores 30% of enemy armor."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_GravityWellEmitter"));
		Tech->DisplayName = FText::FromString(TEXT("Gravity Well Emitter"));
		Tech->Category = EOLCTechCategory::Weapons;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 1800.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 2500.0f, 2500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::DarkMatterCrystals, 2000.0f, 2000.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Void Beam")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Ion Weapon Integration")));
		Tech->EffectDescription = FText::FromString(TEXT("Ultimate area-control weapon — 50m implosion field, pulls and damages all units within."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_VoidInfusedArmor"));
		Tech->DisplayName = FText::FromString(TEXT("Void-Infused Armor"));
		Tech->Category = EOLCTechCategory::Armor;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 1500.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 3000.0f, 3000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::DarkMatterCrystals, 1500.0f, 1500.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Energy-Infused Armor")));
		Tech->EffectDescription = FText::FromString(TEXT("Unit armor tier 4->5 — +350% HP (total +600%), +60 Defense, 20% damage reflection."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_ReinforcedCompositeHull"));
		Tech->DisplayName = FText::FromString(TEXT("Reinforced Composite Hull"));
		Tech->Category = EOLCTechCategory::Armor;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 1200.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 3000.0f, 3000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 2000.0f, 2000.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Hull Reinforcement III")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Titanium Composite Armor")));
		Tech->EffectDescription = FText::FromString(TEXT("Maximum standard hull — +50% ballistic/energy/explosive resistance, slow regeneration."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_AlienShieldGenerator"));
		Tech->DisplayName = FText::FromString(TEXT("Alien Shield Generator"));
		Tech->Category = EOLCTechCategory::Armor;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 1800.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 5000.0f, 5000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::DarkMatterCrystals, 3000.0f, 3000.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Advanced Shield Generation")));
		Tech->EffectDescription = FText::FromString(TEXT("Immune to physical damage, 50% effectiveness against energy damage, no upkeep cost."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_EnergyStreamDrive"));
		Tech->DisplayName = FText::FromString(TEXT("Energy Stream Drive"));
		Tech->Category = EOLCTechCategory::Drives;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 1500.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 1800.0f, 1800.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 1200.0f, 1200.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Ionic Reactor Drive")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Drive Calibration III")));
		Tech->EffectDescription = FText::FromString(TEXT("-80% fuel consumption from base rate, +200% thrust over chemical drives."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_VoidWarpDrive"));
		Tech->DisplayName = FText::FromString(TEXT("Void Warp Drive"));
		Tech->Category = EOLCTechCategory::Drives;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 1800.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 2500.0f, 2500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::DarkMatterCrystals, 2000.0f, 2000.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Energy Stream Drive")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Fuel Processing III")));
		Tech->EffectDescription = FText::FromString(TEXT("Instant travel within a 5-system radius of previously scanned systems."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_AlienWarpIntegration"));
		Tech->DisplayName = FText::FromString(TEXT("Alien Warp Integration"));
		Tech->Category = EOLCTechCategory::Drives;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 2400.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 4000.0f, 4000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::DarkMatterCrystals, 3000.0f, 3000.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Void Warp Drive")));
		Tech->EffectDescription = FText::FromString(TEXT("Unlimited-range wormhole transit to any discovered system, near-free fuel cost."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_BlackHoleStorage"));
		Tech->DisplayName = FText::FromString(TEXT("Black Hole Storage"));
		Tech->Category = EOLCTechCategory::Storage;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 1800.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 5000.0f, 5000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::DarkMatterCrystals, 3000.0f, 3000.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Reinforced Vault")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Resource Conversion II")));
		Tech->EffectDescription = FText::FromString(TEXT("Effectively unlimited storage capacity while powered by 500+ energy output."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_CrossPlanetSyncNetwork"));
		Tech->DisplayName = FText::FromString(TEXT("Cross-Planet Sync Network"));
		Tech->Category = EOLCTechCategory::Storage;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 1200.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 2000.0f, 2000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 1500.0f, 1500.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Remote Transfer Network")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Black Hole Storage")));
		Tech->EffectDescription = FText::FromString(TEXT("All storage across every visited planet shares a single unified inventory."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_MatterCompressor"));
		Tech->DisplayName = FText::FromString(TEXT("Matter Compressor"));
		Tech->Category = EOLCTechCategory::Storage;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 900.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 1500.0f, 1500.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 1000.0f, 1000.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Cross-Planet Sync Network")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Black Hole Storage")));
		Tech->EffectDescription = FText::FromString(TEXT("Reduces bulk item storage volume to 10% of normal — a 10x effective capacity boost."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_AlienArtifactDecoder"));
		Tech->DisplayName = FText::FromString(TEXT("Alien Artifact Decoder"));
		Tech->Category = EOLCTechCategory::Buildings;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 1800.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 3000.0f, 3000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::DarkMatterCrystals, 2000.0f, 2000.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Void Lab Construction")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Orbital Strike Beacon")));
		Tech->EffectDescription = FText::FromString(TEXT("Decode center galaxy ruins for unique blueprints, 1 per 10 minutes of operation."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_QuantumGateNetwork"));
		Tech->DisplayName = FText::FromString(TEXT("Quantum Gate Network"));
		Tech->Category = EOLCTechCategory::Buildings;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 2400.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 8000.0f, 8000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Minerals, 6000.0f, 6000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::Energy, 4000.0f, 4000.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Orbital Strike Beacon")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Remote Transfer Network")));
		Tech->EffectDescription = FText::FromString(TEXT("Instant unit transport between linked gates, up to 10 units per transit cycle."));
		RegisterTech(Tech);
	}
	{
		UOLCTechData* Tech = NewObject<UOLCTechData>(this, TEXT("DT_Tech_DysonSwarmComponent"));
		Tech->DisplayName = FText::FromString(TEXT("Dyson Swarm Component"));
		Tech->Category = EOLCTechCategory::Buildings;
		Tech->RingTier = ERingTier::Outer;
		Tech->ResearchTimeSeconds = 3600.0f;
		Tech->MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 10000.0f, 10000.0f);
		Tech->MaterialCost.Emplace(EOLCResourceType::DarkMatterCrystals, 8000.0f, 8000.0f);
		Tech->Prerequisites.Add(FindTechByName(TEXT("Alien Artifact Decoder")));
		Tech->Prerequisites.Add(FindTechByName(TEXT("Zero-Point Energy Extraction")));
		Tech->EffectDescription = FText::FromString(TEXT("Permanent +50% research speed across all facilities (endgame capstone)."));
		Tech->ResearchSpeedBonusPercent = 50.0f;
		RegisterTech(Tech);
	}

	UE_LOG(LogOLC, Display, TEXT("[OLC] Techs registered: %d total"), AllTechs.Num());
}

bool UOLCResearchSubsystem::IsRingUnlocked(ERingTier Ring) const
{
	// Core is always unlocked (it auto-completes at game start via
	// AutoCompleteCoreTechs()). Ring N+1 opens once Ring N has produced at
	// least one completed tech — see HighestCompletedRing in CompleteResearch().
	return RingIndexOf(Ring) <= HighestCompletedRing + 1;
}

void UOLCResearchSubsystem::SetActiveFactionAndChampion(UOLCFactionData* Faction, UOLCChampionData* Champion)
{
	ActiveFaction = Faction;
	ActiveChampion = Champion;
	UE_LOG(LogOLC, Display, TEXT("[OLC] Research bonuses active — faction: %s, champion: %s"),
		Faction ? *Faction->FactionId : TEXT("none"),
		Champion ? *Champion->DisplayName.ToString() : TEXT("none"));
}

float UOLCResearchSubsystem::GetResearchSpeedMultiplier() const
{
	float BonusPercent = 0.0f;

	// Per-tech completion bonuses (WP-120 Step 4 effects — the structured
	// subset of "modify faction bonuses" that has a concrete percentage in
	// its EffectDescription; see ResearchSpeedBonusPercent doc comment).
	for (const TObjectPtr<UOLCTechData>& Tech : CompletedTechs)
	{
		if (Tech) BonusPercent += Tech->ResearchSpeedBonusPercent;
	}

	// Faction base bonus — per Workpackages/WP-120.md Step 7. FactionId values
	// match the faction DataAssets authored for WBP_FactionSelect.
	if (ActiveFaction)
	{
		if (ActiveFaction->FactionId == TEXT("BrightRealistic")) BonusPercent += 15.0f;
		else if (ActiveFaction->FactionId == TEXT("NeonPunk")) BonusPercent += 10.0f; // energy-focused faction; applied as a flat bonus rather than per-category
		else if (ActiveFaction->FactionId == TEXT("DarkRealistic")) BonusPercent += 10.0f; // weapons/armor-focused faction
		else if (ActiveFaction->FactionId == TEXT("CartoonSciFi")) BonusPercent += 10.0f; // drives/engineering-focused faction
	}

	// Champion bonus — this codebase has no "Intelligence" stat (see
	// Workpackages/WP-120.md architecture note for other such substitutions);
	// EngineeringRating (1-5) is the closest existing analog. +2%/point, capped
	// at +20% per the brief (the 1-5 scale never actually reaches that cap).
	if (ActiveChampion)
	{
		BonusPercent += FMath::Clamp(static_cast<float>(ActiveChampion->EngineeringRating) * 2.0f, 0.0f, 20.0f);
	}

	// Research building proximity bonus. "Proximity" in the brief implies
	// distance to the active research building; this project has no location
	// tracking for the current research site, so presence (any matching
	// building placed) is used instead — a disclosed simplification.
	if (const UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (const UOLCUIDataSubsystem* UIData = GI->GetSubsystem<UOLCUIDataSubsystem>())
		{
			if (UIData->HasBuildingNamed(TEXT("Void Lab"))) BonusPercent += 20.0f;
			else if (UIData->HasBuildingNamed(TEXT("Energy Lab"))) BonusPercent += 10.0f;
			else if (UIData->HasBuildingNamed(TEXT("Forge"))) BonusPercent += 5.0f;
		}
	}

	return 1.0f + BonusPercent / 100.0f;
}

bool UOLCResearchSubsystem::HasRequiredResearchBuilding(ERingTier Ring) const
{
	const UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	const UOLCUIDataSubsystem* UIData = GI ? GI->GetSubsystem<UOLCUIDataSubsystem>() : nullptr;
	if (!UIData) return true; // No UI data subsystem (e.g. unit tests) — don't block on a requirement we can't check.

	// Per Workpackages/WP-120.md Step 8. This project's ERingTier has 5 values
	// (Core/Ring1/Ring2/Ring3/Outer), not the brief's 6 (Ring1-5 + Alien), so
	// Ring3+ collapses onto Energy Lab and Outer (the top tier) onto Void Lab
	// — Dark Matter Converter has no distinct tier left to gate on its own.
	switch (Ring)
	{
		case ERingTier::Core:
		case ERingTier::Ring1:
		case ERingTier::Ring2:
			return UIData->HasBuildingNamed(TEXT("Forge"));
		case ERingTier::Ring3:
			return UIData->HasBuildingNamed(TEXT("Energy Lab"));
		case ERingTier::Outer:
			return UIData->HasBuildingNamed(TEXT("Void Lab"));
		default:
			return true;
	}
}

bool UOLCResearchSubsystem::DeductMaterialCosts(const TArray<FOLCResourceAmount>& Costs)
{
	if (!GetWorld()) return false;

	UGameInstance* GI = GetWorld()->GetGameInstance();
	if (!GI) return false;

	UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>();
	if (!Data) return false;

	for (const auto& Cost : Costs)
	{
		if (Cost.CurrentValue <= 0.0f) continue;

		bool bHasEnough = false;
		for (const auto& Res : Data->GetResourceCounters())
		{
			if (Res.ResourceType == Cost.ResourceType && Res.Value >= Cost.CurrentValue)
			{
				bHasEnough = true;
				break;
			}
		}

		if (!bHasEnough) return false; // Can't afford — abort all.
	}

	// Deduct from subsystem resources.
	for (const auto& Cost : Costs)
	{
		if (Cost.CurrentValue <= 0.0f) continue;
		Data->AddResource(Cost.ResourceType, -Cost.CurrentValue);
	}

	return true;
}

void UOLCResearchSubsystem::ApplyTechEffect(UOLCTechData* Tech)
{
	// Unlock associated build cards.
	for (const auto& Pair : BuildCardUnlocks)
	{
		if (Pair.Value == Tech && !UnlockedBuildCards.Contains(Pair.Key))
		{
			UnlockedBuildCards.Add(Pair.Key);
			UE_LOG(LogOLC, Display, TEXT("[OLC] Build card unlocked: '%s' (by tech: %s)"),
				*Pair.Key, *Tech->DisplayName.ToString());
		}
	}

	// Future effects: unlock unit types, building categories, etc.
	if (!Tech->UnlocksBuildCardName.IsEmpty())
	{
		FString CardName = Tech->UnlocksBuildCardName.ToString();
		if (!UnlockedBuildCards.Contains(CardName))
		{
			UnlockedBuildCards.Add(CardName);
			UE_LOG(LogOLC, Display, TEXT("[OLC] Build card unlocked via EffectDescription: '%s'"), *CardName);
		}
	}
}

void UOLCResearchSubsystem::CompleteResearch(UOLCTechData* Tech)
{
	if (!Tech) return;

	// Mark as completed.
	if (!CompletedTechs.Contains(Tech))
	{
		CompletedTechs.Add(Tech);
	}

	// Apply effects (unlock build cards, etc.).
	ApplyTechEffect(Tech);

	// Advance ring progression (WP-120 Step 6) — Ring N+1 opens once any
	// Ring N tech has completed.
	const int32 CompletedRingIndex = RingIndexOf(Tech->RingTier);
	if (CompletedRingIndex > HighestCompletedRing)
	{
		HighestCompletedRing = CompletedRingIndex;
		UE_LOG(LogOLC, Display, TEXT("[OLC] Ring progression advanced: HighestCompletedRing=%d"), HighestCompletedRing);
	}

	// Live-refresh construction mode so newly-unlocked/ring-gated build cards
	// reflect the new state immediately, not just on next PopulateFakeBuildCards().
	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UOLCUIDataSubsystem* UIData = GI->GetSubsystem<UOLCUIDataSubsystem>())
		{
			UIData->RefreshBuildCardAvailability();
		}
	}

	UE_LOG(LogOLC, Display, TEXT("[OLC] Research complete: %s"), *Tech->DisplayName.ToString());

	// Fire completion event.
	OnTechCompleted.Broadcast(Tech);
}

#undef LOCTEXT_NAMESPACE
