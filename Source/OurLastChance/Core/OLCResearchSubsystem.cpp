#include "OLCResearchSubsystem.h"

#include "Core/OLCUIDataSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"

#define LOCTEXT_NAMESPACE "OLCResearchSubsystem"

void UOLCResearchSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("[OLC] Research subsystem initialized"));
}

void UOLCResearchSubsystem::Tick(float DeltaTime)
{
	if (!CurrentResearch) return;

	// Advance progress.
	CurrentProgressSeconds += DeltaTime;

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
	UE_LOG(LogTemp, Log, TEXT("[OLC] Registered tech: %s (ring=%d, category=%d)"),
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
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Cannot start research — already researching: %s"),
			*CurrentResearch->DisplayName.ToString());
		return false;
	}
	if (CompletedTechs.Contains(Tech))
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Tech already completed: %s"), *Tech->DisplayName.ToString());
		return false;
	}

	TArray<UOLCTechData*> Available = GetAvailableTechs();
	if (!Available.Contains(Tech))
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Tech not available for research: %s"), *Tech->DisplayName.ToString());
		return false;
	}

	// Deduct material costs.
	if (!Tech->MaterialCost.IsEmpty() && !DeductMaterialCosts(Tech->MaterialCost))
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Insufficient resources to start research: %s"), *Tech->DisplayName.ToString());
		return false;
	}

	CurrentResearch = Tech;
	CurrentProgressSeconds = 0.0f;

	UE_LOG(LogTemp, Display, TEXT("[OLC] Started researching: %s (%.0fs)"),
		*Tech->DisplayName.ToString(), Tech->ResearchTimeSeconds);

	OnResearchStarted.Broadcast(Tech);
	return true;
}

void UOLCResearchSubsystem::CancelResearch()
{
	if (!CurrentResearch) return;

	UE_LOG(LogTemp, Log, TEXT("[OLC] Cancelled research: %s (no refund)"),
		*CurrentResearch->DisplayName.ToString());

	CurrentResearch = nullptr;
	CurrentProgressSeconds = 0.0f;
}

void UOLCResearchSubsystem::RegisterBuildCardUnlock(UOLCTechData* Tech, const FString& BuildCardName)
{
	if (!Tech) return;
	BuildCardUnlocks.Add(BuildCardName, Tech);
	UE_LOG(LogTemp, Log, TEXT("[OLC] Registered build card unlock: '%s' → tech: %s"),
		*BuildCardName, *Tech->DisplayName.ToString());
}

bool UOLCResearchSubsystem::IsBuildCardUnlocked(const FString& BuildCardName) const
{
	return UnlockedBuildCards.Contains(BuildCardName);
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

			UE_LOG(LogTemp, Log, TEXT("[OLC] Auto-unlocked Core tech: %s"), *Tech->DisplayName.ToString());
		}
	}

	UE_LOG(LogTemp, Display, TEXT("[OLC] Core tech auto-unlock complete — %d total completed"), CompletedTechs.Num());
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

	UE_LOG(LogTemp, Display, TEXT("[OLC] Techs registered: %d total"), AllTechs.Num());
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
			UE_LOG(LogTemp, Display, TEXT("[OLC] Build card unlocked: '%s' (by tech: %s)"),
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
			UE_LOG(LogTemp, Display, TEXT("[OLC] Build card unlocked via EffectDescription: '%s'"), *CardName);
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

	UE_LOG(LogTemp, Display, TEXT("[OLC] Research complete: %s"), *Tech->DisplayName.ToString());

	// Fire completion event.
	OnTechCompleted.Broadcast(Tech);
}

#undef LOCTEXT_NAMESPACE
