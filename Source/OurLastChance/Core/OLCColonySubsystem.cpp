#include "Core/OLCColonySubsystem.h"
#include "OurLastChance.h"

#include "Core/OLCColonyNetworkData.h"
#include "Core/OLCNavigationSubsystem.h"
#include "Core/OLCResearchSubsystem.h"
#include "Core/OLCUIDataSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UObject/UObjectGlobals.h"

#define LOCTEXT_NAMESPACE "OLCColonySubsystem"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

namespace OLCColonyInternal
{
	/** Every resource type in EOLCResourceType (stable order). */
	const TArray<EOLCResourceType>& AllResourceTypes()
	{
		static const TArray<EOLCResourceType> Types = {
			EOLCResourceType::Energy,
			EOLCResourceType::Fuel,
			EOLCResourceType::ConstructionMaterial,
			EOLCResourceType::Minerals,
			EOLCResourceType::HullParts,
			EOLCResourceType::Survival,
			EOLCResourceType::DarkMatterCrystals,
		};
		return Types;
	}

	const TCHAR* RoleName(EOLCColonyRole Role)
	{
		switch (Role)
		{
		case EOLCColonyRole::Mining:       return TEXT("Mining");
		case EOLCColonyRole::Fuel:         return TEXT("Fuel");
		case EOLCColonyRole::Military:     return TEXT("Military");
		case EOLCColonyRole::Research:     return TEXT("Research");
		case EOLCColonyRole::Agricultural: return TEXT("Agricultural");
		default:                           return TEXT("None");
		}
	}
}

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------

void UOLCColonySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Designer-tunable config (optional — built-in WP-124 defaults apply when the asset is absent).
	ColonyNetworkData = LoadObject<UOLCColonyNetworkData>(nullptr, TEXT("/Game/OurLastChance/Data/Colony/DA_ColonyNetwork"));

	if (UOLCResearchSubsystem* Research = GetResearch())
	{
		// Track quantum-gate unlock through the real research completion event.
		Research->OnTechCompleted.AddDynamic(this, &UOLCColonySubsystem::HandleTechCompleted);

		// Recheck techs that completed before this subsystem initialized (e.g. loaded save).
		if (UOLCTechData* QGTech = Research->FindTechByName(QuantumGateTechName))
		{
			bQuantumGateUnlocked = Research->IsTechCompleted(QGTech);
		}
	}

	UE_LOG(LogOLC, Log, TEXT("[OLC] Colony subsystem initialized (config: %s)"),
		ColonyNetworkData ? *ColonyNetworkData->GetName() : TEXT("built-in defaults"));
}

void UOLCColonySubsystem::Deinitialize()
{
	StopColonyTick();

	if (UOLCResearchSubsystem* Research = GetResearch())
	{
		Research->OnTechCompleted.RemoveDynamic(this, &UOLCColonySubsystem::HandleTechCompleted);
	}

	Super::Deinitialize();
}

// ---------------------------------------------------------------------------
// Tick — role transitions (later steps extend this with transfers)
// ---------------------------------------------------------------------------

void UOLCColonySubsystem::Tick(float DeltaTime)
{
	for (TPair<int32, FOLCColonyRecord>& Pair : Colonies)
	{
		FOLCColonyRecord& Record = Pair.Value;

		if (Record.PrimaryRoleTransitionRemaining > 0.0f && Record.PendingPrimaryRole != EOLCColonyRole::None)
		{
			Record.PrimaryRoleTransitionRemaining -= DeltaTime;
			if (Record.PrimaryRoleTransitionRemaining <= 0.0f)
			{
				Record.PrimaryRoleTransitionRemaining = 0.0f;
				Record.PrimaryRole = Record.PendingPrimaryRole;
				Record.PendingPrimaryRole = EOLCColonyRole::None;
				UE_LOG(LogOLC, Log, TEXT("[OLC] Colony %d primary role transition complete: %s"),
					Record.ClusterID, OLCColonyInternal::RoleName(Record.PrimaryRole));
			}
		}

		if (Record.SecondaryRoleTransitionRemaining > 0.0f && Record.PendingSecondaryRole != EOLCColonyRole::None)
		{
			Record.SecondaryRoleTransitionRemaining -= DeltaTime;
			if (Record.SecondaryRoleTransitionRemaining <= 0.0f)
			{
				Record.SecondaryRoleTransitionRemaining = 0.0f;
				Record.SecondaryRole = Record.PendingSecondaryRole;
				Record.PendingSecondaryRole = EOLCColonyRole::None;
				UE_LOG(LogOLC, Log, TEXT("[OLC] Colony %d secondary role transition complete: %s"),
					Record.ClusterID, OLCColonyInternal::RoleName(Record.SecondaryRole));
			}
		}
	}

	TickActiveTransfers(DeltaTime);
}

void UOLCColonySubsystem::PerformColonyTick()
{
	Tick(ColonyTickIntervalSeconds);
}

// ---------------------------------------------------------------------------
// Colony state machine
// ---------------------------------------------------------------------------

FOLCColonyRecord& UOLCColonySubsystem::GetOrCreateColony(int32 ClusterID)
{
	FOLCColonyRecord& Record = Colonies.FindOrAdd(ClusterID);
	Record.ClusterID = ClusterID;

	// Resolve the system name from the navigation subsystem's cluster layout.
	if (const UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (const UOLCNavigationSubsystem* Nav = GI->GetSubsystem<UOLCNavigationSubsystem>())
		{
			const TArray<FOLCGalaxyCluster> Clusters = Nav->GetOrGenerateGalaxyClusters();
			if (Clusters.IsValidIndex(ClusterID))
			{
				Record.SystemName = Clusters[ClusterID].ClusterName;
			}
		}
	}

	return Record;
}

void UOLCColonySubsystem::InitializeEmptyStorage(FOLCColonyRecord& Record) const
{
	Record.Storage.Empty();
	Record.StorageCapacity.Empty();
	for (const EOLCResourceType Type : OLCColonyInternal::AllResourceTypes())
	{
		Record.Storage.Add(Type, 0.0f);
		Record.StorageCapacity.Add(Type, DefaultStorageCapacityPerResource);
	}
}

void UOLCColonySubsystem::SeedStorageFromGlobalCounters(FOLCColonyRecord& Record) const
{
	InitializeEmptyStorage(Record);

	const UOLCUIDataSubsystem* Data = GetUIData();
	if (!Data)
	{
		return;
	}

	for (const FOLCResourceCounterViewData& Counter : Data->GetResourceCounters())
	{
		Record.Storage.FindOrAdd(Counter.ResourceType) = Counter.Value;
		const float Capacity = Counter.Capacity > 0.0f ? Counter.Capacity : DefaultStorageCapacityPerResource;
		Record.StorageCapacity.FindOrAdd(Counter.ResourceType) = Capacity;
	}
}

bool UOLCColonySubsystem::IsCurrentSystem(int32 ClusterID) const
{
	if (const UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (const UOLCNavigationSubsystem* Nav = GI->GetSubsystem<UOLCNavigationSubsystem>())
		{
			return Nav->GetCurrentSystemID() == ClusterID;
		}
	}
	return false;
}

void UOLCColonySubsystem::MarkSystemVisited(int32 ClusterID)
{
	FOLCColonyRecord& Record = GetOrCreateColony(ClusterID);

	if (Record.State != EOLCColonyState::Unvisited)
	{
		// Visited/Owned/Abandoned are never downgraded by a visit.
		return;
	}

	Record.State = EOLCColonyState::Visited;
	OnColonyStateChanged.Broadcast(ClusterID, Record.State);
}

bool UOLCColonySubsystem::FoundColony(int32 ClusterID, EOLCBiomeType Biome)
{
	FOLCColonyRecord& Record = GetOrCreateColony(ClusterID);

	if (Record.State == EOLCColonyState::Owned)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] FoundColony: cluster %d already has a colony"), ClusterID);
		return false;
	}
	if (Record.State == EOLCColonyState::Abandoned)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] FoundColony: cluster %d is abandoned — use RecolonizeColony"), ClusterID);
		return false;
	}

	Record.State = EOLCColonyState::Owned;
	Record.Biome = Biome;

	const bool bIsHomeColony = IsCurrentSystem(ClusterID);

	if (bIsHomeColony)
	{
		// Home colony inherits the player's current global inventory and the
		// live building production rates (off-screen colonies have no building
		// registry yet — feed them via SetColonyProductionRates).
		SeedStorageFromGlobalCounters(Record);
		if (const UOLCUIDataSubsystem* Data = GetUIData())
		{
			for (const FOLCResourceAmount& Rate : Data->GetProductionRates())
			{
				Record.ProductionRates.FindOrAdd(Rate.ResourceType) = Rate.CurrentValue;
			}
		}
	}
	else
	{
		InitializeEmptyStorage(Record);
	}

	UE_LOG(LogOLC, Log, TEXT("[OLC] Colony founded on cluster %d (%s), biome: %d"),
		ClusterID, *Record.SystemName.ToString(), static_cast<int32>(Biome));

	OnColonyStateChanged.Broadcast(ClusterID, Record.State);
	return true;
}

bool UOLCColonySubsystem::AbandonColony(int32 ClusterID)
{
	FOLCColonyRecord* Record = Colonies.Find(ClusterID);
	if (!Record || Record->State != EOLCColonyState::Owned)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] AbandonColony: cluster %d has no owned colony"), ClusterID);
		return false;
	}

	Record->State = EOLCColonyState::Abandoned;

	// The colony is destroyed by hostile takeover: storage, production, roles,
	// and gates are all lost.
	Record->Storage.Empty();
	Record->StorageCapacity.Empty();
	Record->ProductionRates.Empty();
	Record->PrimaryRole = EOLCColonyRole::None;
	Record->SecondaryRole = EOLCColonyRole::None;
	Record->PendingPrimaryRole = EOLCColonyRole::None;
	Record->PendingSecondaryRole = EOLCColonyRole::None;
	Record->PrimaryRoleTransitionRemaining = 0.0f;
	Record->SecondaryRoleTransitionRemaining = 0.0f;
	Record->bHasGate = false;
	Record->bGateOperational = false;

	UE_LOG(LogOLC, Log, TEXT("[OLC] Colony on cluster %d abandoned (hostile takeover)"), ClusterID);

	OnColonyStateChanged.Broadcast(ClusterID, Record->State);
	return true;
}

bool UOLCColonySubsystem::RecolonizeColony(int32 ClusterID)
{
	FOLCColonyRecord* Record = Colonies.Find(ClusterID);
	if (!Record || Record->State != EOLCColonyState::Abandoned)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] RecolonizeColony: cluster %d is not abandoned"), ClusterID);
		return false;
	}

	UOLCUIDataSubsystem* Data = GetUIData();
	if (!Data)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] RecolonizeColony: no UI data subsystem available"));
		return false;
	}

	auto GlobalValue = [Data](EOLCResourceType Type) -> float
	{
		for (const FOLCResourceCounterViewData& Counter : Data->GetResourceCounters())
		{
			if (Counter.ResourceType == Type)
			{
				return Counter.Value;
			}
		}
		return 0.0f;
	};

	// Cost resolution: registered DataAsset wins; otherwise documented WP defaults.
	const float CMCost = ColonyNetworkData ? ColonyNetworkData->RecolonizeCostConstructionMaterial : RecolonizeCostConstructionMaterial;
	const float MineralCost = ColonyNetworkData ? ColonyNetworkData->RecolonizeCostMinerals : RecolonizeCostMinerals;

	// Phase 1: verify affordability of the whole cost (abort all if unaffordable).
	if (GlobalValue(EOLCResourceType::ConstructionMaterial) < CMCost ||
		GlobalValue(EOLCResourceType::Minerals) < MineralCost)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] RecolonizeColony: insufficient resources (%.0f CM + %.0f Minerals required)"),
			CMCost, MineralCost);
		return false;
	}

	// Phase 2: deduct (AddResource applies negative amounts linearly).
	Data->AddResource(EOLCResourceType::ConstructionMaterial, -CMCost);
	Data->AddResource(EOLCResourceType::Minerals, -MineralCost);

	Record->State = EOLCColonyState::Owned;
	InitializeEmptyStorage(*Record);

	UE_LOG(LogOLC, Log, TEXT("[OLC] Colony re-established on cluster %d (paid %.0f CM + %.0f Minerals)"),
		ClusterID, CMCost, MineralCost);

	OnColonyStateChanged.Broadcast(ClusterID, Record->State);
	return true;
}

EOLCColonyState UOLCColonySubsystem::GetColonyState(int32 ClusterID) const
{
	if (const FOLCColonyRecord* Record = Colonies.Find(ClusterID))
	{
		return Record->State;
	}
	return EOLCColonyState::Unvisited;
}

const FOLCColonyRecord* UOLCColonySubsystem::GetColony(int32 ClusterID) const
{
	return Colonies.Find(ClusterID);
}

TArray<FOLCColonyRecord> UOLCColonySubsystem::GetAllColonies() const
{
	TArray<FOLCColonyRecord> Result;
	Result.Reserve(Colonies.Num());
	for (const TPair<int32, FOLCColonyRecord>& Pair : Colonies)
	{
		Result.Add(Pair.Value);
	}
	return Result;
}

void UOLCColonySubsystem::SetActiveColony(int32 ClusterID)
{
	ActiveColonyID = ClusterID;
}

// ---------------------------------------------------------------------------
// Unlock gating
// ---------------------------------------------------------------------------

bool UOLCColonySubsystem::IsColonySystemUnlocked() const
{
	const UOLCResearchSubsystem* Research = GetResearch();
	if (!Research)
	{
		return false;
	}

	// Whole colony package unlocks at Outer ring (WP-124 description: "Void Lab
	// Outer Ring research completion").
	return Research->GetHighestCompletedRing() >= OuterRingIndex;
}

bool UOLCColonySubsystem::IsQuantumGateUnlocked() const
{
	if (bQuantumGateUnlocked)
	{
		return true;
	}

	// Live recheck covers techs completed through any path.
	const UOLCResearchSubsystem* Research = GetResearch();
	if (!Research)
	{
		return false;
	}

	if (UOLCTechData* QGTech = Research->FindTechByName(QuantumGateTechName))
	{
		return Research->IsTechCompleted(QGTech);
	}
	return false;
}

void UOLCColonySubsystem::HandleTechCompleted(UOLCTechData* CompletedTech)
{
	// Null-safe: CancelResearch broadcasts OnTechCompleted(nullptr).
	if (!CompletedTech)
	{
		return;
	}

	if (CompletedTech->DisplayName.ToString().Equals(QuantumGateTechName, ESearchCase::IgnoreCase))
	{
		bQuantumGateUnlocked = true;
		UE_LOG(LogOLC, Display, TEXT("[OLC] Quantum Gate Network research completed — quantum gates unlocked"));
	}
}

// ---------------------------------------------------------------------------
// Colony roles
// ---------------------------------------------------------------------------

const FOLCColonyRoleConfig* UOLCColonySubsystem::FindRoleConfig(EOLCColonyRole Role) const
{
	// Prefer the registered DataAsset; fall back to built-in WP-124 defaults.
	if (ColonyNetworkData && !ColonyNetworkData->RoleConfigs.IsEmpty())
	{
		for (const FOLCColonyRoleConfig& Config : ColonyNetworkData->RoleConfigs)
		{
			if (Config.Role == Role)
			{
				return &Config;
			}
		}
		return nullptr;
	}

	static const TArray<FOLCColonyRoleConfig> Defaults = UOLCColonyNetworkData::GetDefaultRoleConfigs();
	for (const FOLCColonyRoleConfig& Config : Defaults)
	{
		if (Config.Role == Role)
		{
			return &Config;
		}
	}
	return nullptr;
}

float UOLCColonySubsystem::GetRoleProductionBonusPercent(EOLCColonyRole Role, EOLCResourceType ResourceType) const
{
	const FOLCColonyRoleConfig* Config = FindRoleConfig(Role);
	if (!Config || Config->ProductionBonusPercent <= 0.0f)
	{
		return 0.0f;
	}
	return Config->AffectedResources.Contains(ResourceType) ? Config->ProductionBonusPercent : 0.0f;
}

bool UOLCColonySubsystem::AssignRole(int32 ClusterID, EOLCColonyRole Role, bool bSecondary)
{
	if (Role == EOLCColonyRole::None)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] AssignRole: cannot assign None"));
		return false;
	}

	FOLCColonyRecord* Record = Colonies.Find(ClusterID);
	if (!Record || Record->State != EOLCColonyState::Owned)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] AssignRole: cluster %d has no owned colony"), ClusterID);
		return false;
	}

	const FOLCColonyRoleConfig* Config = FindRoleConfig(Role);
	if (!Config)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] AssignRole: no config for role %d"), static_cast<int32>(Role));
		return false;
	}

	// Biome eligibility (empty list = any biome).
	if (!Config->EligibleBiomes.IsEmpty() && !Config->EligibleBiomes.Contains(Record->Biome))
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] AssignRole: role %s not eligible on biome %d (cluster %d)"),
			OLCColonyInternal::RoleName(Role), static_cast<int32>(Record->Biome), ClusterID);
		return false;
	}

	if (bSecondary)
	{
		if (Record->SecondaryRole == Role)
		{
			return true; // Already the active secondary role — nothing to do.
		}
		Record->PendingSecondaryRole = Role;
		Record->SecondaryRoleTransitionRemaining = RoleTransitionSeconds;
	}
	else
	{
		if (Record->PrimaryRole == Role)
		{
			return true; // Already the active primary role — nothing to do.
		}
		Record->PendingPrimaryRole = Role;
		Record->PrimaryRoleTransitionRemaining = RoleTransitionSeconds;
	}

	UE_LOG(LogOLC, Log, TEXT("[OLC] Colony %d %s role set to %s (takes effect in %.0fs)"),
		ClusterID, bSecondary ? TEXT("secondary") : TEXT("primary"),
		OLCColonyInternal::RoleName(Role), RoleTransitionSeconds);

	return true;
}

float UOLCColonySubsystem::GetEffectiveRoleMultiplier(int32 ClusterID, EOLCResourceType ResourceType) const
{
	const FOLCColonyRecord* Record = Colonies.Find(ClusterID);
	if (!Record)
	{
		return 1.0f;
	}

	// Only the currently active roles count — pending roles apply after their
	// transition timer expires (see Tick).
	float Multiplier = 1.0f;
	Multiplier += GetRoleProductionBonusPercent(Record->PrimaryRole, ResourceType) / 100.0f;
	Multiplier += GetRoleProductionBonusPercent(Record->SecondaryRole, ResourceType) / 200.0f; // half bonus
	return Multiplier;
}

float UOLCColonySubsystem::GetAggregateUnitTrainingSpeedBonusPercent() const
{
	float Total = 0.0f;
	const FOLCColonyRoleConfig* Config = FindRoleConfig(EOLCColonyRole::Military);
	if (!Config)
	{
		return 0.0f;
	}
	for (const TPair<int32, FOLCColonyRecord>& Pair : Colonies)
	{
		const FOLCColonyRecord& Record = Pair.Value;
		if (Record.State == EOLCColonyState::Owned && Record.PrimaryRole == EOLCColonyRole::Military)
		{
			Total += Config->UnitTrainingSpeedBonusPercent;
		}
	}
	return Total;
}

float UOLCColonySubsystem::GetAggregateDefenseStatBonusPercent() const
{
	float Total = 0.0f;
	const FOLCColonyRoleConfig* Config = FindRoleConfig(EOLCColonyRole::Military);
	if (!Config)
	{
		return 0.0f;
	}
	for (const TPair<int32, FOLCColonyRecord>& Pair : Colonies)
	{
		const FOLCColonyRecord& Record = Pair.Value;
		if (Record.State == EOLCColonyState::Owned && Record.PrimaryRole == EOLCColonyRole::Military)
		{
			Total += Config->DefenseStatBonusPercent;
		}
	}
	return Total;
}

float UOLCColonySubsystem::GetAggregateResearchSpeedBonusPercent() const
{
	float Total = 0.0f;
	const FOLCColonyRoleConfig* Config = FindRoleConfig(EOLCColonyRole::Research);
	if (!Config)
	{
		return 0.0f;
	}
	for (const TPair<int32, FOLCColonyRecord>& Pair : Colonies)
	{
		const FOLCColonyRecord& Record = Pair.Value;
		if (Record.State == EOLCColonyState::Owned && Record.PrimaryRole == EOLCColonyRole::Research)
		{
			Total += Config->ResearchSpeedBonusPercent;
		}
	}
	return Total;
}

float UOLCColonySubsystem::GetAggregateCrewCapacityBonusPercent() const
{
	float Total = 0.0f;
	const FOLCColonyRoleConfig* Config = FindRoleConfig(EOLCColonyRole::Agricultural);
	if (!Config)
	{
		return 0.0f;
	}
	for (const TPair<int32, FOLCColonyRecord>& Pair : Colonies)
	{
		const FOLCColonyRecord& Record = Pair.Value;
		if (Record.State == EOLCColonyState::Owned && Record.PrimaryRole == EOLCColonyRole::Agricultural)
		{
			Total += Config->CrewCapacityBonusPercent;
		}
	}
	return Total;
}

// ---------------------------------------------------------------------------
// Resource transfers (WP-124 Step 2) — Tanker / Hauler / Quantum Gate
// ---------------------------------------------------------------------------

const FOLCTransportMethodConfig* UOLCColonySubsystem::FindTransportConfig(EOLCTransportMethod Method) const
{
	// Prefer the registered DataAsset; fall back to built-in WP-124 defaults.
	if (ColonyNetworkData && !ColonyNetworkData->TransportConfigs.IsEmpty())
	{
		for (const FOLCTransportMethodConfig& Config : ColonyNetworkData->TransportConfigs)
		{
			if (Config.Method == Method)
			{
				return &Config;
			}
		}
		return nullptr;
	}

	static const TArray<FOLCTransportMethodConfig> Defaults = UOLCColonyNetworkData::GetDefaultTransportConfigs();
	for (const FOLCTransportMethodConfig& Config : Defaults)
	{
		if (Config.Method == Method)
		{
			return &Config;
		}
	}
	return nullptr;
}

bool UOLCColonySubsystem::ValidateTransfer(int32 SourceClusterID, int32 DestClusterID, EOLCResourceType ResourceType, float Amount, EOLCTransportMethod Method) const
{
	if (Amount <= 0.0f)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] StartTransfer: non-positive amount"));
		return false;
	}

	if (SourceClusterID == DestClusterID)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] StartTransfer: source and destination are the same cluster (%d)"), SourceClusterID);
		return false;
	}

	const FOLCColonyRecord* Source = Colonies.Find(SourceClusterID);
	if (!Source || Source->State != EOLCColonyState::Owned)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] StartTransfer: source cluster %d has no owned colony"), SourceClusterID);
		return false;
	}

	const FOLCColonyRecord* Dest = Colonies.Find(DestClusterID);
	if (!Dest || Dest->State != EOLCColonyState::Owned)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] StartTransfer: destination cluster %d has no owned colony"), DestClusterID);
		return false;
	}

	if (!IsColonySystemUnlocked())
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] StartTransfer: colony system not unlocked (Outer ring research required)"));
		return false;
	}

	const FOLCTransportMethodConfig* Config = FindTransportConfig(Method);
	if (!Config)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] StartTransfer: no config for transport method %d"), static_cast<int32>(Method));
		return false;
	}

	// Capacity: the method's configured per-trip capacity; quantum gate is
	// hard-capped at 50 units per call (WP-124 "50 units per tick").
	const float EffectiveCapacity = (Method == EOLCTransportMethod::QuantumGate)
		? FMath::Min(Config->CapacityPerTrip, QuantumGateHardCapPerCall)
		: Config->CapacityPerTrip;

	if (Amount > EffectiveCapacity)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] StartTransfer: amount %.1f exceeds %s capacity of %.1f"),
			Amount, *Config->DisplayName.ToString(), EffectiveCapacity);
		return false;
	}

	const float SourceValue = Source->Storage.Contains(ResourceType) ? Source->Storage[ResourceType] : 0.0f;
	if (Amount > SourceValue)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] StartTransfer: source cluster %d has only %.1f of resource %d (needs %.1f)"),
			SourceClusterID, SourceValue, static_cast<int32>(ResourceType), Amount);
		return false;
	}

	if (Method == EOLCTransportMethod::QuantumGate)
	{
		if (!IsQuantumGateUnlocked())
		{
			UE_LOG(LogOLC, Warning, TEXT("[OLC] StartTransfer: quantum gate network not researched"));
			return false;
		}
		if (!Source->bHasGate || !Source->bGateOperational)
		{
			UE_LOG(LogOLC, Warning, TEXT("[OLC] StartTransfer: source cluster %d has no operational quantum gate"), SourceClusterID);
			return false;
		}
		if (!Dest->bHasGate || !Dest->bGateOperational)
		{
			UE_LOG(LogOLC, Warning, TEXT("[OLC] StartTransfer: destination cluster %d has no operational quantum gate"), DestClusterID);
			return false;
		}
	}
	else
	{
		// Two-phase cost check: the trip's energy cost comes out of the source
		// colony's Energy storage. When moving Energy itself, payload and cost
		// draw from the same pool, so both must fit together.
		const float SourceEnergy = Source->Storage.Contains(EOLCResourceType::Energy) ? Source->Storage[EOLCResourceType::Energy] : 0.0f;
		const float RequiredEnergy = (ResourceType == EOLCResourceType::Energy)
			? Config->EnergyCostPerTrip + Amount
			: Config->EnergyCostPerTrip;

		if (SourceEnergy < RequiredEnergy)
		{
			UE_LOG(LogOLC, Warning, TEXT("[OLC] StartTransfer: source cluster %d has only %.1f Energy (needs %.1f for the trip)"),
				SourceClusterID, SourceEnergy, RequiredEnergy);
			return false;
		}
	}

	return true;
}

bool UOLCColonySubsystem::CanStartTransfer(int32 SourceClusterID, int32 DestClusterID, EOLCResourceType ResourceType, float Amount, EOLCTransportMethod Method) const
{
	return ValidateTransfer(SourceClusterID, DestClusterID, ResourceType, Amount, Method);
}

bool UOLCColonySubsystem::StartTransfer(int32 SourceClusterID, int32 DestClusterID, EOLCResourceType ResourceType, float Amount, EOLCTransportMethod Method)
{
	if (!ValidateTransfer(SourceClusterID, DestClusterID, ResourceType, Amount, Method))
	{
		return false;
	}

	const FOLCTransportMethodConfig* Config = FindTransportConfig(Method);
	FOLCColonyRecord& Source = *Colonies.Find(SourceClusterID);
	FOLCColonyRecord& Dest = *Colonies.Find(DestClusterID);

	if (Method == EOLCTransportMethod::QuantumGate)
	{
		// Instant: no energy cost, no active-transfer entry.
		Source.Storage[ResourceType] -= Amount;
		CreditToColonyStorage(Dest, ResourceType, Amount);

		FOLCActiveTransfer Completed;
		Completed.SourceClusterID = SourceClusterID;
		Completed.DestClusterID = DestClusterID;
		Completed.ResourceType = ResourceType;
		Completed.Amount = Amount;
		Completed.Method = Method;
		Completed.TimeRemaining = 0.0f;

		UE_LOG(LogOLC, Log, TEXT("[OLC] Quantum gate transfer: %.1f of resource %d, cluster %d -> %d (instant)"),
			Amount, static_cast<int32>(ResourceType), SourceClusterID, DestClusterID);

		OnTransferCompleted.Broadcast(Completed);
		return true;
	}

	// Tanker/Hauler: deduct payload + trip energy cost from the source colony
	// now; the destination is credited when Tick() completes the transfer.
	Source.Storage[ResourceType] -= Amount;
	Source.Storage[EOLCResourceType::Energy] -= Config->EnergyCostPerTrip;

	FOLCActiveTransfer Transfer;
	Transfer.SourceClusterID = SourceClusterID;
	Transfer.DestClusterID = DestClusterID;
	Transfer.ResourceType = ResourceType;
	Transfer.Amount = Amount;
	Transfer.Method = Method;
	Transfer.TimeRemaining = Config->TransitSeconds;
	ActiveTransfers.Add(Transfer);

	UE_LOG(LogOLC, Log, TEXT("[OLC] %s transfer started: %.1f of resource %d, cluster %d -> %d (%.0fs transit, %.0f Energy)"),
		*Config->DisplayName.ToString(), Amount, static_cast<int32>(ResourceType), SourceClusterID, DestClusterID,
		Config->TransitSeconds, Config->EnergyCostPerTrip);

	return true;
}

TArray<FOLCActiveTransfer> UOLCColonySubsystem::GetActiveTransfers() const
{
	return ActiveTransfers;
}

void UOLCColonySubsystem::CreditToColonyStorage(FOLCColonyRecord& Record, EOLCResourceType ResourceType, float Amount) const
{
	float& Value = Record.Storage.FindOrAdd(ResourceType);
	const float Capacity = Record.StorageCapacity.Contains(ResourceType) ? Record.StorageCapacity[ResourceType] : DefaultStorageCapacityPerResource;

	const float Requested = Value + Amount;
	if (Requested > Capacity)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Colony %d storage full for resource %d — %.1f of incoming %.1f dropped"),
			Record.ClusterID, static_cast<int32>(ResourceType), Requested - Capacity, Amount);
		Value = Capacity;
	}
	else
	{
		Value = Requested;
	}
}

void UOLCColonySubsystem::TickActiveTransfers(float DeltaTime)
{
	for (int32 i = ActiveTransfers.Num() - 1; i >= 0; --i)
	{
		FOLCActiveTransfer& Transfer = ActiveTransfers[i];
		Transfer.TimeRemaining -= DeltaTime;
		if (Transfer.TimeRemaining > 0.0f)
		{
			continue;
		}
		Transfer.TimeRemaining = 0.0f;

		FOLCColonyRecord* Dest = Colonies.Find(Transfer.DestClusterID);
		if (Dest && Dest->State == EOLCColonyState::Owned)
		{
			CreditToColonyStorage(*Dest, Transfer.ResourceType, Transfer.Amount);
		}
		else
		{
			UE_LOG(LogOLC, Warning, TEXT("[OLC] Transfer of %.1f of resource %d to cluster %d lost — destination is no longer an owned colony"),
				Transfer.Amount, static_cast<int32>(Transfer.ResourceType), Transfer.DestClusterID);
		}

		OnTransferCompleted.Broadcast(Transfer);
		ActiveTransfers.RemoveAt(i);
	}
}

// ---------------------------------------------------------------------------
// Quantum Gate Network (WP-124 Step 3) — build/repair, max-5 cap, links
// ---------------------------------------------------------------------------

bool UOLCColonySubsystem::BuildQuantumGate(int32 ClusterID)
{
	FOLCColonyRecord* Record = Colonies.Find(ClusterID);
	if (!Record || Record->State != EOLCColonyState::Owned)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] BuildQuantumGate: cluster %d has no owned colony"), ClusterID);
		return false;
	}

	if (Record->bHasGate)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] BuildQuantumGate: cluster %d already has a quantum gate"), ClusterID);
		return false;
	}

	if (!IsColonySystemUnlocked())
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] BuildQuantumGate: colony system not unlocked (Outer ring research required)"));
		return false;
	}

	if (!IsQuantumGateUnlocked())
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] BuildQuantumGate: quantum gate network not researched"));
		return false;
	}

	if (CountActiveGates() >= MaxQuantumGates)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] BuildQuantumGate: maximum of %d quantum gates already built"), MaxQuantumGates);
		return false;
	}

	// Phase 1: verify the full build cost is available in THIS colony's storage.
	const float HaveCM = Record->Storage.Contains(EOLCResourceType::ConstructionMaterial) ? Record->Storage[EOLCResourceType::ConstructionMaterial] : 0.0f;
	const float HaveMinerals = Record->Storage.Contains(EOLCResourceType::Minerals) ? Record->Storage[EOLCResourceType::Minerals] : 0.0f;
	const float HaveEnergy = Record->Storage.Contains(EOLCResourceType::Energy) ? Record->Storage[EOLCResourceType::Energy] : 0.0f;
	const float HaveDMC = Record->Storage.Contains(EOLCResourceType::DarkMatterCrystals) ? Record->Storage[EOLCResourceType::DarkMatterCrystals] : 0.0f;

	if (HaveCM < QuantumGateBuildCostConstructionMaterial ||
		HaveMinerals < QuantumGateBuildCostMinerals ||
		HaveEnergy < QuantumGateBuildCostEnergy ||
		HaveDMC < QuantumGateBuildCostDarkMatterCrystals)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] BuildQuantumGate: cluster %d cannot afford the gate (has %.0f CM / %.0f Minerals / %.0f Energy / %.0f DMC; needs %.0f / %.0f / %.0f / %.0f)"),
			ClusterID, HaveCM, HaveMinerals, HaveEnergy, HaveDMC,
			QuantumGateBuildCostConstructionMaterial, QuantumGateBuildCostMinerals, QuantumGateBuildCostEnergy, QuantumGateBuildCostDarkMatterCrystals);
		return false;
	}

	// Phase 2: deduct the full cost from this colony's storage.
	Record->Storage[EOLCResourceType::ConstructionMaterial] -= QuantumGateBuildCostConstructionMaterial;
	Record->Storage[EOLCResourceType::Minerals] -= QuantumGateBuildCostMinerals;
	Record->Storage[EOLCResourceType::Energy] -= QuantumGateBuildCostEnergy;
	Record->Storage[EOLCResourceType::DarkMatterCrystals] -= QuantumGateBuildCostDarkMatterCrystals;

	Record->bHasGate = true;
	Record->bGateOperational = true;

	UE_LOG(LogOLC, Log, TEXT("[OLC] Quantum gate built on cluster %d (%d/%d active)"),
		ClusterID, CountActiveGates(), MaxQuantumGates);

	OnGateNetworkChanged.Broadcast();
	return true;
}

bool UOLCColonySubsystem::DestroyGate(int32 ClusterID)
{
	FOLCColonyRecord* Record = Colonies.Find(ClusterID);
	if (!Record || !Record->bHasGate)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] DestroyGate: cluster %d has no quantum gate"), ClusterID);
		return false;
	}

	Record->bHasGate = false;
	Record->bGateOperational = false;

	UE_LOG(LogOLC, Log, TEXT("[OLC] Quantum gate on cluster %d destroyed (build slot freed)"), ClusterID);

	OnGateNetworkChanged.Broadcast();
	return true;
}

bool UOLCColonySubsystem::SetGateOperational(int32 ClusterID, bool bOperational)
{
	FOLCColonyRecord* Record = Colonies.Find(ClusterID);
	if (!Record || !Record->bHasGate)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] SetGateOperational: cluster %d has no quantum gate"), ClusterID);
		return false;
	}

	if (Record->bGateOperational == bOperational)
	{
		return true; // Already in the requested state — nothing to do.
	}

	Record->bGateOperational = bOperational;

	UE_LOG(LogOLC, Log, TEXT("[OLC] Quantum gate on cluster %d is now %s"),
		ClusterID, bOperational ? TEXT("operational") : TEXT("non-operational (broken)"));

	OnGateNetworkChanged.Broadcast();
	return true;
}

bool UOLCColonySubsystem::RepairGate(int32 ClusterID)
{
	FOLCColonyRecord* Record = Colonies.Find(ClusterID);
	if (!Record || !Record->bHasGate)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] RepairGate: cluster %d has no quantum gate"), ClusterID);
		return false;
	}

	if (Record->bGateOperational)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] RepairGate: cluster %d gate is already operational"), ClusterID);
		return false;
	}

	// Phase 1: verify the repair cost (half build — CM + Minerals only) in colony storage.
	const float HaveCM = Record->Storage.Contains(EOLCResourceType::ConstructionMaterial) ? Record->Storage[EOLCResourceType::ConstructionMaterial] : 0.0f;
	const float HaveMinerals = Record->Storage.Contains(EOLCResourceType::Minerals) ? Record->Storage[EOLCResourceType::Minerals] : 0.0f;

	if (HaveCM < QuantumGateRepairCostConstructionMaterial || HaveMinerals < QuantumGateRepairCostMinerals)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] RepairGate: cluster %d cannot afford the repair (has %.0f CM / %.0f Minerals; needs %.0f / %.0f)"),
			ClusterID, HaveCM, HaveMinerals, QuantumGateRepairCostConstructionMaterial, QuantumGateRepairCostMinerals);
		return false;
	}

	// Phase 2: deduct the repair cost.
	Record->Storage[EOLCResourceType::ConstructionMaterial] -= QuantumGateRepairCostConstructionMaterial;
	Record->Storage[EOLCResourceType::Minerals] -= QuantumGateRepairCostMinerals;

	Record->bGateOperational = true;

	UE_LOG(LogOLC, Log, TEXT("[OLC] Quantum gate on cluster %d repaired (paid %.0f CM + %.0f Minerals)"),
		ClusterID, QuantumGateRepairCostConstructionMaterial, QuantumGateRepairCostMinerals);

	OnGateNetworkChanged.Broadcast();
	return true;
}

bool UOLCColonySubsystem::AreGatesLinked(int32 ClusterA, int32 ClusterB) const
{
	if (ClusterA == ClusterB)
	{
		return false;
	}

	const FOLCColonyRecord* A = Colonies.Find(ClusterA);
	const FOLCColonyRecord* B = Colonies.Find(ClusterB);
	return A && B && A->bHasGate && A->bGateOperational && B->bHasGate && B->bGateOperational;
}

TArray<TPair<int32, int32>> UOLCColonySubsystem::GetGateNetworkLinks() const
{
	TArray<int32> OperationalClusters;
	for (const TPair<int32, FOLCColonyRecord>& Pair : Colonies)
	{
		if (Pair.Value.bHasGate && Pair.Value.bGateOperational)
		{
			OperationalClusters.Add(Pair.Key);
		}
	}
	OperationalClusters.Sort();

	TArray<TPair<int32, int32>> Links;
	for (int32 i = 0; i < OperationalClusters.Num(); ++i)
	{
		for (int32 j = i + 1; j < OperationalClusters.Num(); ++j)
		{
			Links.Emplace(OperationalClusters[i], OperationalClusters[j]);
		}
	}
	return Links;
}

int32 UOLCColonySubsystem::CountActiveGates() const
{
	int32 Count = 0;
	for (const TPair<int32, FOLCColonyRecord>& Pair : Colonies)
	{
		if (Pair.Value.bHasGate)
		{
			++Count;
		}
	}
	return Count;
}

// ---------------------------------------------------------------------------
// Production dashboard (WP-124 Step 5)
// ---------------------------------------------------------------------------

float UOLCColonySubsystem::GetBestRoleMultiplierForBiome(EOLCBiomeType Biome, EOLCResourceType ResourceType) const
{
	float Best = 1.0f; // No-role baseline.
	static const TArray<EOLCColonyRole> AllRoles = {
		EOLCColonyRole::Mining,
		EOLCColonyRole::Fuel,
		EOLCColonyRole::Military,
		EOLCColonyRole::Research,
		EOLCColonyRole::Agricultural,
	};

	for (const EOLCColonyRole Role : AllRoles)
	{
		const FOLCColonyRoleConfig* Config = FindRoleConfig(Role);
		if (!Config)
		{
			continue;
		}
		if (!Config->EligibleBiomes.IsEmpty() && !Config->EligibleBiomes.Contains(Biome))
		{
			continue;
		}
		Best = FMath::Max(Best, 1.0f + GetRoleProductionBonusPercent(Role, ResourceType) / 100.0f);
	}

	return Best;
}

FOLCProductionDashboardViewData UOLCColonySubsystem::BuildProductionDashboard() const
{
	FOLCProductionDashboardViewData Dashboard;

	TMap<EOLCResourceType, float> TotalsByType;
	TMap<EOLCResourceType, float> CapacitiesByType;
	float StorageSum = 0.0f;
	float CapacitySum = 0.0f;
	float ActualTotal = 0.0f;
	float PotentialTotal = 0.0f;

	// Per-colony rows (owned colonies only) + aggregates.
	for (const TPair<int32, FOLCColonyRecord>& Pair : Colonies)
	{
		const FOLCColonyRecord& Record = Pair.Value;
		if (Record.State != EOLCColonyState::Owned)
		{
			continue;
		}

		FOLCColonyMetricsViewData Row;
		Row.ClusterID = Record.ClusterID;
		Row.SystemName = Record.SystemName;
		Row.State = Record.State;
		Row.PrimaryRole = Record.PrimaryRole;

		for (const EOLCResourceType Type : OLCColonyInternal::AllResourceTypes())
		{
			const float Value = Record.Storage.Contains(Type) ? Record.Storage[Type] : 0.0f;
			const float Capacity = Record.StorageCapacity.Contains(Type) ? Record.StorageCapacity[Type] : DefaultStorageCapacityPerResource;

			FOLCResourceAmount Amount;
			Amount.ResourceType = Type;
			Amount.CurrentValue = Value;
			Amount.Capacity = Capacity;
			Row.Storage.Add(Amount);

			StorageSum += Value;
			CapacitySum += Capacity;
			CapacitiesByType.FindOrAdd(Type) += Capacity;
		}

		for (const TPair<EOLCResourceType, float>& Rate : Record.ProductionRates)
		{
			const float BaseRate = Rate.Value;
			const float EffectiveRate = BaseRate * GetEffectiveRoleMultiplier(Record.ClusterID, Rate.Key);

			Row.ProductionRatesPerSecond.Add(Rate.Key, EffectiveRate);
			TotalsByType.FindOrAdd(Rate.Key) += EffectiveRate;

			ActualTotal += EffectiveRate;
			PotentialTotal += BaseRate * GetBestRoleMultiplierForBiome(Record.Biome, Rate.Key);
		}

		for (const FOLCActiveTransfer& Transfer : ActiveTransfers)
		{
			const float ImpliedRate = Transfer.TimeRemaining > 0.0f ? Transfer.Amount / Transfer.TimeRemaining : 0.0f;
			if (Transfer.DestClusterID == Record.ClusterID)
			{
				Row.TransferInAmount += Transfer.Amount;
				Row.TransferInRatePerSecond += ImpliedRate;
			}
			if (Transfer.SourceClusterID == Record.ClusterID)
			{
				Row.TransferOutAmount += Transfer.Amount;
				Row.TransferOutRatePerSecond += ImpliedRate;
			}
		}

		Dashboard.PerColony.Add(Row);
	}

	for (const EOLCResourceType Type : OLCColonyInternal::AllResourceTypes())
	{
		FOLCResourceAmount Entry;
		Entry.ResourceType = Type;
		Entry.CurrentValue = TotalsByType.Contains(Type) ? TotalsByType[Type] : 0.0f;
		Entry.Capacity = CapacitiesByType.Contains(Type) ? CapacitiesByType[Type] : 0.0f;
		Dashboard.TotalPerTick.Add(Entry);
	}

	// Network status: operational gates vs broken links (exist but non-operational).
	for (const TPair<int32, FOLCColonyRecord>& Pair : Colonies)
	{
		if (!Pair.Value.bHasGate)
		{
			continue;
		}
		if (Pair.Value.bGateOperational)
		{
			++Dashboard.ActiveGateCount;
		}
		else
		{
			++Dashboard.BrokenLinkCount;
		}
	}

	Dashboard.CapacityUtilization = CapacitySum > 0.0f ? FMath::Clamp(StorageSum / CapacitySum, 0.0f, 1.0f) : 0.0f;

	// Efficiency: actual production vs potential at best eligible role per resource
	// (documented interpretation of "percentage of potential production being utilized").
	Dashboard.EfficiencyScore = PotentialTotal > 0.0f
		? FMath::Clamp(100.0f * ActualTotal / PotentialTotal, 0.0f, 100.0f)
		: 100.0f;

	// Alerts (owned colonies).
	for (const TPair<int32, FOLCColonyRecord>& Pair : Colonies)
	{
		const FOLCColonyRecord& Record = Pair.Value;
		if (Record.State != EOLCColonyState::Owned)
		{
			continue;
		}

		// Low stock: any resource below the threshold percent of capacity (zero counts).
		for (const TPair<EOLCResourceType, float>& Stored : Record.Storage)
		{
			const float Capacity = Record.StorageCapacity.Contains(Stored.Key) ? Record.StorageCapacity[Stored.Key] : DefaultStorageCapacityPerResource;
			if (Capacity > 0.0f && Stored.Value < LowStockThresholdPercent / 100.0f * Capacity)
			{
				Dashboard.Alerts.Add(FText::FromString(FString::Printf(
					TEXT("LOW STOCK: %s — resource %d at %.1f%% of capacity"),
					*Record.SystemName.ToString(), static_cast<int32>(Stored.Key), 100.0f * Stored.Value / Capacity)));
			}
		}

		// Broken quantum gate link (per affected colony).
		if (Record.bHasGate && !Record.bGateOperational)
		{
			Dashboard.Alerts.Add(FText::FromString(FString::Printf(
				TEXT("BROKEN GATE LINK: %s — quantum gate non-operational"), *Record.SystemName.ToString())));
		}

		// Role mismatch: active primary role not eligible for the colony biome.
		if (Record.PrimaryRole != EOLCColonyRole::None)
		{
			const FOLCColonyRoleConfig* Config = FindRoleConfig(Record.PrimaryRole);
			if (Config && !Config->EligibleBiomes.IsEmpty() && !Config->EligibleBiomes.Contains(Record.Biome))
			{
				Dashboard.Alerts.Add(FText::FromString(FString::Printf(
					TEXT("ROLE MISMATCH: %s — %s is not eligible on biome %d"),
					*Record.SystemName.ToString(), OLCColonyInternal::RoleName(Record.PrimaryRole), static_cast<int32>(Record.Biome))));
			}
		}
	}

	return Dashboard;
}

bool UOLCColonySubsystem::SetColonyProductionRates(int32 ClusterID, const TArray<FOLCResourceAmount>& Rates)
{
	FOLCColonyRecord* Record = Colonies.Find(ClusterID);
	if (!Record || Record->State != EOLCColonyState::Owned)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] SetColonyProductionRates: cluster %d has no owned colony"), ClusterID);
		return false;
	}

	Record->ProductionRates.Empty();
	for (const FOLCResourceAmount& Rate : Rates)
	{
		if (Rate.CurrentValue < 0.0f)
		{
			UE_LOG(LogOLC, Warning, TEXT("[OLC] SetColonyProductionRates: skipping negative rate for resource %d"), static_cast<int32>(Rate.ResourceType));
			continue;
		}
		Record->ProductionRates.FindOrAdd(Rate.ResourceType) = Rate.CurrentValue;
	}

	UE_LOG(LogOLC, Log, TEXT("[OLC] Colony %d production rates updated (%d resources)"), ClusterID, Record->ProductionRates.Num());
	return true;
}

// ---------------------------------------------------------------------------
// Config
// ---------------------------------------------------------------------------

void UOLCColonySubsystem::SetColonyNetworkData(UOLCColonyNetworkData* InData)
{
	ColonyNetworkData = InData;
	UE_LOG(LogOLC, Log, TEXT("[OLC] Colony network data %s"), InData ? *InData->GetName() : TEXT("cleared (built-in defaults)"));
}

// ---------------------------------------------------------------------------
// Tick driver (world timer — mirrors UOLCUIDataSubsystem production tick)
// ---------------------------------------------------------------------------

void UOLCColonySubsystem::StartColonyTick()
{
	if (ColonyTickTimer.IsValid())
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Colony tick already running"));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] StartColonyTick: no world available yet"));
		return;
	}

	World->GetTimerManager().SetTimer(
		ColonyTickTimer,
		this,
		&UOLCColonySubsystem::PerformColonyTick,
		ColonyTickIntervalSeconds,
		true // bLoop = true
	);

	UE_LOG(LogOLC, Log, TEXT("[OLC] Colony tick started (interval: %.2fs)"), ColonyTickIntervalSeconds);
}

void UOLCColonySubsystem::StopColonyTick()
{
	if (!ColonyTickTimer.IsValid())
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ColonyTickTimer);
	}
	ColonyTickTimer.Invalidate(); // Allow StartColonyTick() to run again.
	UE_LOG(LogOLC, Log, TEXT("[OLC] Colony tick stopped"));
}

// ---------------------------------------------------------------------------
// Subsystem fetch helpers (null-safe for standalone game instances)
// ---------------------------------------------------------------------------

UOLCUIDataSubsystem* UOLCColonySubsystem::GetUIData() const
{
	if (const UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		return GI->GetSubsystem<UOLCUIDataSubsystem>();
	}
	return nullptr;
}

UOLCResearchSubsystem* UOLCColonySubsystem::GetResearch() const
{
	if (const UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		return GI->GetSubsystem<UOLCResearchSubsystem>();
	}
	return nullptr;
}

#undef LOCTEXT_NAMESPACE
