#include "SpaceTravel/OLCTravelSubsystem.h"
#include "OurLastChance.h"

#include "Core/OLCUIDataSubsystem.h"
#include "Logging/LogMacros.h"
#include "Containers/Ticker.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "VFX/OLCVFXSubsystem.h"

#define LOCTEXT_NAMESPACE "OLCTravelSubsystem"

namespace
{
	/** Current stored value of one resource type (0.0 if the counter is absent). */
	float GetResourceValue(const UOLCUIDataSubsystem* UI, EOLCResourceType ResourceType)
	{
		for (const FOLCResourceCounterViewData& Res : UI->GetResourceCounters())
		{
			if (Res.ResourceType == ResourceType)
			{
				return Res.Value;
			}
		}
		return 0.0f;
	}
}

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------
void UOLCTravelSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// UGameInstanceSubsystem has no virtual Tick in UE 5.8 (and nothing in this
	// project drives research-style subsystem ticks), so self-register a core
	// ticker delegate to advance the state machine clock every frame.
	TickHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(this, &UOLCTravelSubsystem::HandleTick), 0.0f);

	// Own a minigame manager instance: UOLCMinigameManager is a plain UObject with no
	// owner elsewhere in the tree, and in-transit events (e.g. meteor showers) drive it.
	MinigameManager = NewObject<UOLCMinigameManager>(this);

	UE_LOG(LogOLC, Log, TEXT("[OLC] Travel subsystem initialized (state machine ready)"));
}

void UOLCTravelSubsystem::Deinitialize()
{
	FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);

	Super::Deinitialize();

	CurrentState = EOLCTravelState::Idle;
	DestinationPlanetId.Empty();
	StateElapsedSeconds = 0.0f;
	TransitTotalSeconds = 0.0f;
	FuelSpentThisTrip = 0.0f;
	EnergySpentThisTrip = 0.0f;
	RouteDistance = 1.0f;
	CurrentEvent = EOLCSpaceEventType::None;
	HullDamageThisTrip = 0.0f;
	TripWeather = EOLCSpaceWeatherType::None;
	WeatherRemainingSeconds = 0.0f;

	UE_LOG(LogOLC, Log, TEXT("[OLC] Travel subsystem deinitialized"));
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------
bool UOLCTravelSubsystem::BeginTravel(const FString& InDestinationPlanetId, int32 DriveTier, float RouteDistanceUnits)
{
	if (CurrentState != EOLCTravelState::Idle)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] BeginTravel rejected — already in state %s"), *GetTravelStateName());
		return false;
	}

	if (InDestinationPlanetId.IsEmpty())
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] BeginTravel rejected — empty destination planet id"));
		return false;
	}

	// An Offline drive cannot travel (ship state is owned by the UI data subsystem).
	UOLCUIDataSubsystem* UI = GetGameInstance() ? GetGameInstance()->GetSubsystem<UOLCUIDataSubsystem>() : nullptr;
	if (UI && UI->GetDriveStatus() == EOLCModuleState::Offline)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] BeginTravel rejected — drive is Offline"));
		return false;
	}

	const UOLCTravelConfigData* Config = GetActiveTravelConfig();
	const int32 ClampedTier = FMath::Clamp(DriveTier, 1, 6);
	const FOLCDriveProfile* Profile = Config->FindDriveProfile(ClampedTier);
	if (!Profile)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] BeginTravel rejected — no drive profile for tier %d"), ClampedTier);
		return false;
	}

	const float Distance = (RouteDistanceUnits > 0.0f) ? RouteDistanceUnits : Config->DefaultRouteDistanceUnits;
	const float FuelCost = Profile->FuelCostPerDistanceUnit * Distance;
	const float EnergyCost = Profile->EnergyCostPerDistanceUnit * Distance;

	// Phase 1: affordability check — deduct nothing, move nowhere (same pattern as
	// UOLCNavigationSubsystem::TryPayFuelAndTravel).
	if (!UI || GetResourceValue(UI, EOLCResourceType::Fuel) < FuelCost ||
		GetResourceValue(UI, EOLCResourceType::Energy) < EnergyCost)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] BeginTravel rejected — insufficient resources (need %.1f fuel + %.1f energy for %s tier %d x %.2f units)"),
			FuelCost, EnergyCost, *Profile->DriveName, ClampedTier, Distance);
		return false;
	}

	// Phase 2: deduct the trip cost BEFORE Transit begins (WP-122 step 1: "Depart: fuel deducted").
	UI->AddResource(EOLCResourceType::Fuel, -FuelCost);
	UI->AddResource(EOLCResourceType::Energy, -EnergyCost);

	this->DestinationPlanetId = InDestinationPlanetId;
	ActiveDriveTier = ClampedTier;
	FuelSpentThisTrip = FuelCost;
	EnergySpentThisTrip = EnergyCost;
	RouteDistance = Distance;
	CurrentEvent = EOLCSpaceEventType::None;
	HullDamageThisTrip = 0.0f;
	TripWeather = EOLCSpaceWeatherType::None;
	WeatherRemainingSeconds = 0.0f;
	StateElapsedSeconds = 0.0f;
	TransitTotalSeconds = Config->BaseTransitSecondsPerUnit * Distance * Profile->TimeFactor;

	UE_LOG(LogOLC, Display, TEXT("[OLC] Travel started: dest=%s drive=%s (tier %d) dist=%.2f cost=%.1f fuel + %.1f energy transit=%.1fs"),
		*InDestinationPlanetId, *Profile->DriveName, ClampedTier, Distance, FuelCost, EnergyCost, TransitTotalSeconds);

	ChangeState(EOLCTravelState::Depart);
	return true;
}

// ---------------------------------------------------------------------------
// State queries
// ---------------------------------------------------------------------------
FString UOLCTravelSubsystem::GetTravelStateName() const
{
	switch (CurrentState)
	{
	case EOLCTravelState::Idle:       return TEXT("IDLE");
	case EOLCTravelState::Depart:     return TEXT("DEPART");
	case EOLCTravelState::Transit:    return TEXT("TRANSIT");
	case EOLCTravelState::EventCheck: return TEXT("EVENT CHECK");
	case EOLCTravelState::Arrive:     return TEXT("ARRIVE");
	case EOLCTravelState::Land:       return TEXT("LAND");
	default:                          return TEXT("UNKNOWN");
	}
}

bool UOLCTravelSubsystem::IsInTransit() const
{
	return CurrentState == EOLCTravelState::Depart ||
	       CurrentState == EOLCTravelState::Transit ||
	       CurrentState == EOLCTravelState::EventCheck;
}

float UOLCTravelSubsystem::GetTransitProgress() const
{
	if (TransitTotalSeconds <= 0.0f)
	{
		return 1.0f;
	}
	return FMath::Clamp(StateElapsedSeconds / TransitTotalSeconds, 0.0f, 1.0f);
}

// ---------------------------------------------------------------------------
// Landing hooks (UOLLandingSequence drives the Land state, step 8)
// ---------------------------------------------------------------------------
bool UOLCTravelSubsystem::BeginLanding()
{
	if (CurrentState != EOLCTravelState::Arrive)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] BeginLanding rejected — not in Arrive (state %s)"), *GetTravelStateName());
		return false;
	}

	ChangeState(EOLCTravelState::Land);

	// Start the landing sequence for this trip. The destination biome is the active
	// biome of the UI data subsystem (the planet being landed on); coastal terrain
	// availability defaults to true until the planet-surface terrain system provides it.
	const UOLCUIDataSubsystem* UI = GetGameInstance() ? GetGameInstance()->GetSubsystem<UOLCUIDataSubsystem>() : nullptr;
	const EOLCBiomeType DestinationBiome = UI ? UI->GetActiveBiome() : EOLCBiomeType::Desert;

	LandingSequence = NewObject<UOLLandingSequence>(this);

	// WP-126 step 8: bind landing-VFX hooks (append-only; existing statements preserved verbatim).
	// Descend entry -> start looping crash smoke trail; Descend exit -> stop it.
	// Touchdown complete -> impact explosion + landing dust at the touchdown point.
	LandingSequence->OnLandingPhaseChanged.AddDynamic(this, &UOLCTravelSubsystem::HandleLandingPhaseChanged);
	LandingSequence->OnLandingComplete.AddDynamic(this, &UOLCTravelSubsystem::HandleLandingComplete);

	LandingSequence->StartLanding(DestinationPlanetId, DestinationBiome);
	return true;
}

void UOLCTravelSubsystem::CompleteLanding()
{
	if (CurrentState != EOLCTravelState::Land)
	{
		return;
	}

	ChangeState(EOLCTravelState::Idle);
}

// ---------------------------------------------------------------------------
// Tick-driven state machine
// ---------------------------------------------------------------------------
bool UOLCTravelSubsystem::HandleTick(float DeltaTime)
{
	const UOLCTravelConfigData* Config = GetActiveTravelConfig();

	switch (CurrentState)
	{
	case EOLCTravelState::Depart:
		StateElapsedSeconds += DeltaTime;
		if (StateElapsedSeconds >= Config->DepartDurationSeconds)
		{
			ChangeState(EOLCTravelState::Transit);
			RollSpaceWeather(); // one weather condition (or none) per trip, rolled here
		}
		break;

	case EOLCTravelState::Transit:
		// Fuel + energy were prepaid at depart; the clock drives this phase, scaled by
		// the active space weather's speed modifier (1.0 when clear skies).
		UpdateActiveWeather(DeltaTime);
		StateElapsedSeconds += DeltaTime * GetWeatherSpeedFactor();
		if (StateElapsedSeconds >= TransitTotalSeconds)
		{
			ChangeState(EOLCTravelState::EventCheck);
			ResolveEventCheck(); // exactly one event (or none) per trip, resolved here
		}
		break;

	case EOLCTravelState::EventCheck:
		StateElapsedSeconds += DeltaTime;
		if (StateElapsedSeconds >= Config->EventCheckDurationSeconds)
		{
			ChangeState(EOLCTravelState::Arrive);
		}
		break;

	case EOLCTravelState::Land:
		// The landing sequence drives this phase (step 8). When it finishes —
		// touchdown or blocked at SafetyAssessment — the trip ends and we return to Idle.
		if (LandingSequence && LandingSequence->IsRunning())
		{
			LandingSequence->TickLanding(DeltaTime);
		}
		else if (LandingSequence && LandingSequence->HasFinished())
		{
			CompleteLanding(); // touchdown or blocked — result already broadcast by the sequence
		}
		break;

	default:
		// Idle and Arrive rest until explicitly advanced (BeginTravel / BeginLanding).
		break;
	}

	return true; // keep ticking
}

void UOLCTravelSubsystem::ChangeState(EOLCTravelState NewState)
{
	if (NewState == CurrentState)
	{
		return;
	}

	const EOLCTravelState OldState = CurrentState;
	const FString OldStateName = GetTravelStateName();
	CurrentState = NewState;
	StateElapsedSeconds = 0.0f;

	UE_LOG(LogOLC, Display, TEXT("[OLC] Travel state: %s -> %s (dest=%s)"),
		*OldStateName, *GetTravelStateName(), *DestinationPlanetId);

	OnTravelStateChanged.Broadcast(OldState, NewState);
}

const UOLCTravelConfigData* UOLCTravelSubsystem::GetActiveTravelConfig() const
{
	// Assigned authored instance wins; otherwise the class CDO carries the full
	// code-side default table (timing + six drive profiles), so travel works
	// without any authored DataAsset.
	return TravelConfigAsset ? TravelConfigAsset : GetDefault<UOLCTravelConfigData>();
}

const UOLCSpaceEventData* UOLCTravelSubsystem::GetActiveSpaceEventData() const
{
	// Same pattern as the travel config: authored instance wins, CDO fallback otherwise.
	return SpaceEventData ? SpaceEventData : GetDefault<UOLCSpaceEventData>();
}

// ---------------------------------------------------------------------------
// Space weather (WP-122 step 5)
// ---------------------------------------------------------------------------
const UOLCSpaceWeatherData* UOLCTravelSubsystem::GetActiveSpaceWeatherData() const
{
	// Same pattern as the travel config: authored instance wins, CDO fallback otherwise.
	return SpaceWeatherAsset ? SpaceWeatherAsset : GetDefault<UOLCSpaceWeatherData>();
}

EOLCSpaceWeatherType UOLCTravelSubsystem::GetActiveWeather() const
{
	return (TripWeather != EOLCSpaceWeatherType::None && WeatherRemainingSeconds > 0.0f)
		? TripWeather
		: EOLCSpaceWeatherType::None;
}

float UOLCTravelSubsystem::GetSensorEffectiveness() const
{
	if (TripWeather == EOLCSpaceWeatherType::None || WeatherRemainingSeconds <= 0.0f)
	{
		return 1.0f;
	}

	const FOLCSpaceWeatherEffect* Effect = GetActiveSpaceWeatherData()->FindEffect(TripWeather);
	return Effect ? FMath::Clamp(1.0f - Effect->SensorPenalty, 0.0f, 1.0f) : 1.0f;
}

float UOLCTravelSubsystem::GetWeatherSpeedFactor() const
{
	if (TripWeather == EOLCSpaceWeatherType::None || WeatherRemainingSeconds <= 0.0f)
	{
		return 1.0f;
	}

	const FOLCSpaceWeatherEffect* Effect = GetActiveSpaceWeatherData()->FindEffect(TripWeather);
	return Effect ? FMath::Max(0.05f, Effect->SpeedMultiplier) : 1.0f;
}

void UOLCTravelSubsystem::RollSpaceWeather()
{
	const UOLCSpaceWeatherData* WeatherData = GetActiveSpaceWeatherData();
	if (!WeatherData || WeatherData->WeatherTable.Num() == 0)
	{
		return;
	}

	if (FMath::FRand() >= WeatherData->BaseWeatherChance)
	{
		TripWeather = EOLCSpaceWeatherType::None;
		UE_LOG(LogOLC, Display, TEXT("[OLC] Space weather: clear skies this trip"));
		return;
	}

	// Weighted pick over rows with Weight > 0 — exactly one weather condition.
	float TotalWeight = 0.0f;
	for (const FOLCSpaceWeatherEffect& Effect : WeatherData->WeatherTable)
	{
		if (Effect.Weight > 0.0f)
		{
			TotalWeight += Effect.Weight;
		}
	}

	if (TotalWeight <= 0.0f)
	{
		return;
	}

	float Roll = FMath::FRandRange(0.0f, TotalWeight);
	const FOLCSpaceWeatherEffect* Picked = nullptr;
	for (const FOLCSpaceWeatherEffect& Effect : WeatherData->WeatherTable)
	{
		if (Effect.Weight <= 0.0f)
		{
			continue;
		}
		Roll -= Effect.Weight;
		if (Roll < 0.0f)
		{
			Picked = &Effect;
			break;
		}
	}

	if (!Picked)
	{
		return;
	}

	TripWeather = Picked->WeatherType;
	WeatherRemainingSeconds = Picked->DurationSeconds;

	UE_LOG(LogOLC, Display, TEXT("[OLC] Space weather: %d (speed x%.2f, sensors -%.0f%%, hull %.1f%%/s, fuel +%.1f/s, %.0fs)"),
		static_cast<int32>(Picked->WeatherType), Picked->SpeedMultiplier, Picked->SensorPenalty * 100.0f,
		Picked->HullDamagePerSecond, Picked->ExtraFuelBurnPerSecond, Picked->DurationSeconds);

	OnSpaceWeatherChanged.Broadcast(TripWeather, true);
}

void UOLCTravelSubsystem::UpdateActiveWeather(float DeltaTime)
{
	if (TripWeather == EOLCSpaceWeatherType::None || WeatherRemainingSeconds <= 0.0f)
	{
		return;
	}

	const FOLCSpaceWeatherEffect* Effect = GetActiveSpaceWeatherData()->FindEffect(TripWeather);
	if (!Effect)
	{
		TripWeather = EOLCSpaceWeatherType::None;
		WeatherRemainingSeconds = 0.0f;
		return;
	}

	// Continuous effects while the weather window is open (no per-frame logging).
	if (Effect->HullDamagePerSecond > 0.0f)
	{
		HullDamageThisTrip = FMath::Min(100.0f, HullDamageThisTrip + Effect->HullDamagePerSecond * DeltaTime);
	}

	if (Effect->ExtraFuelBurnPerSecond > 0.0f)
	{
		UOLCUIDataSubsystem* UI = GetGameInstance() ? GetGameInstance()->GetSubsystem<UOLCUIDataSubsystem>() : nullptr;
		if (UI)
		{
			UI->AddResource(EOLCResourceType::Fuel, -Effect->ExtraFuelBurnPerSecond * DeltaTime);
		}
	}

	WeatherRemainingSeconds -= DeltaTime;
	if (WeatherRemainingSeconds <= 0.0f)
	{
		WeatherRemainingSeconds = 0.0f;
		UE_LOG(LogOLC, Display, TEXT("[OLC] Space weather %d cleared"), static_cast<int32>(TripWeather));
		OnSpaceWeatherChanged.Broadcast(TripWeather, false);
	}
}

// ---------------------------------------------------------------------------
// Space events (WP-122 step 4)
// ---------------------------------------------------------------------------
void UOLCTravelSubsystem::ResolveEventCheck()
{
	const UOLCSpaceEventData* Data = GetActiveSpaceEventData();
	if (!Data || Data->EventTable.Num() == 0)
	{
		CurrentEvent = EOLCSpaceEventType::None;
		return;
	}

	UOLCUIDataSubsystem* UI = GetGameInstance() ? GetGameInstance()->GetSubsystem<UOLCUIDataSubsystem>() : nullptr;

	// Distance conditioning: longer routes meet more things (multiplier capped).
	const UOLCTravelConfigData* TravelCfg = GetActiveTravelConfig();
	const float RefDistance = FMath::Max(0.001f, TravelCfg->DefaultRouteDistanceUnits);
	const float DistanceMultiplier = FMath::Clamp(RouteDistance / RefDistance, 1.0f, Data->MaxDistanceChanceMultiplier);

	if (FMath::FRand() >= Data->BaseEventChance * DistanceMultiplier)
	{
		CurrentEvent = EOLCSpaceEventType::None;
		UE_LOG(LogOLC, Display, TEXT("[OLC] EventCheck: no space event this trip"));
		return;
	}

	// Fuel conditioning: low fuel boosts the pirate weight.
	float FuelFraction = 1.0f;
	if (UI)
	{
		const float FuelCapacity = UI->GetMaxCapacity(EOLCResourceType::Fuel);
		if (FuelCapacity > 0.0f)
		{
			FuelFraction = GetResourceValue(UI, EOLCResourceType::Fuel) / FuelCapacity;
		}
	}

	// TIR conditioning: only events at or below colony TIR are eligible.
	const int32 ColonyTIR = UI ? UI->GetColonyTIR() : 1;

	// Weather conditioning: this trip's rolled weather biases specific event weights
	// for the whole trip (e.g. AsteroidStorm raises MeteorShower odds).
	const FOLCSpaceWeatherEffect* TripWeatherEffect = (TripWeather != EOLCSpaceWeatherType::None)
		? GetActiveSpaceWeatherData()->FindEffect(TripWeather)
		: nullptr;

	float TotalWeight = 0.0f;
	for (const FOLCSpaceEventDefinition& Def : Data->EventTable)
	{
		if (Def.Weight <= 0.0f || Def.MinTIR > ColonyTIR)
		{
			continue;
		}
		float Weight = Def.Weight;
		if (Def.EventType == EOLCSpaceEventType::PirateEncounter && FuelFraction < Data->LowFuelThreshold)
		{
			Weight *= Data->LowFuelPirateWeightMultiplier;
		}
		if (TripWeatherEffect)
		{
			for (const FOLCWeatherEventBias& Bias : TripWeatherEffect->EventWeightBias)
			{
				if (Bias.EventType == Def.EventType)
				{
					Weight *= Bias.WeightMultiplier;
				}
			}
		}
		TotalWeight += Weight;
	}

	if (TotalWeight <= 0.0f)
	{
		CurrentEvent = EOLCSpaceEventType::None;
		UE_LOG(LogOLC, Display, TEXT("[OLC] EventCheck: no TIR-eligible events (colony TIR %d)"), ColonyTIR);
		return;
	}

	// Weighted pick — exactly one event.
	float Roll = FMath::FRandRange(0.0f, TotalWeight);
	const FOLCSpaceEventDefinition* Picked = nullptr;
	for (const FOLCSpaceEventDefinition& Def : Data->EventTable)
	{
		if (Def.Weight <= 0.0f || Def.MinTIR > ColonyTIR)
		{
			continue;
		}
		float Weight = Def.Weight;
		if (Def.EventType == EOLCSpaceEventType::PirateEncounter && FuelFraction < Data->LowFuelThreshold)
		{
			Weight *= Data->LowFuelPirateWeightMultiplier;
		}
		if (TripWeatherEffect)
		{
			for (const FOLCWeatherEventBias& Bias : TripWeatherEffect->EventWeightBias)
			{
				if (Bias.EventType == Def.EventType)
				{
					Weight *= Bias.WeightMultiplier;
				}
			}
		}
		Roll -= Weight;
		if (Roll < 0.0f)
		{
			Picked = &Def;
			break;
		}
	}

	if (!Picked)
	{
		CurrentEvent = EOLCSpaceEventType::None;
		return;
	}

	CurrentEvent = Picked->EventType;
	UE_LOG(LogOLC, Display, TEXT("[OLC] EventCheck: space event rolled — %d (colony TIR %d, fuel %.0f%%)"),
		static_cast<int32>(Picked->EventType), ColonyTIR, FuelFraction * 100.0f);

	ResolveEventOutcome(*Picked);
}

void UOLCTravelSubsystem::ResolveEventOutcome(const FOLCSpaceEventDefinition& InDef)
{
	UOLCUIDataSubsystem* UI = GetGameInstance() ? GetGameInstance()->GetSubsystem<UOLCUIDataSubsystem>() : nullptr;

	switch (InDef.EventType)
	{
	case EOLCSpaceEventType::PirateEncounter:
		// Combat itself is owned by WP-113 (UOLCSpaceCombatManager); hand off only.
		OnPirateCombatHandoff.Broadcast(DestinationPlanetId);
		break;

	case EOLCSpaceEventType::DerelictShip:
		GrantEventReward(InDef, UI, TEXT("derelict salvage"));
		break;

	case EOLCSpaceEventType::Merchant:
		// Trade hook: the UI presents the transit trade screen (later step).
		OnMerchantTradeOffered.Broadcast(DestinationPlanetId);
		break;

	case EOLCSpaceEventType::Anomaly:
		if (FMath::FRand() < 0.5f)
		{
			GrantEventReward(InDef, UI, TEXT("anomaly mystery reward"));
		}
		else
		{
			UE_LOG(LogOLC, Warning, TEXT("[OLC] Anomaly destabilizes — hull hazard triggered"));
			ApplyEventHullDamage(InDef.HazardHullDamagePercent);
		}
		break;

	case EOLCSpaceEventType::MeteorShower:
		ApplyEventHullDamage(InDef.HazardHullDamagePercent);
		if (InDef.bHasMinigame && MinigameManager)
		{
			UE_LOG(LogOLC, Display, TEXT("[OLC] Meteor shower — AsteroidEvasion minigame launched"));
			MinigameManager->StartMinigame(InDef.MinigameType, InDef.MinigameDurationSeconds);
		}
		break;

	default:
		// None — nothing to resolve.
		break;
	}
}

void UOLCTravelSubsystem::GrantEventReward(const FOLCSpaceEventDefinition& InDef, UOLCUIDataSubsystem* UI, const TCHAR* Context)
{
	if (!UI || InDef.RewardPayload.Num() == 0)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] %s: no reward payload to grant"), Context);
		return;
	}

	for (const FOLCResourceAmount& Reward : InDef.RewardPayload)
	{
		UI->AddResource(Reward.ResourceType, Reward.CurrentValue);
	}

	UE_LOG(LogOLC, Display, TEXT("[OLC] %s: loot granted (%d resource lines)"), Context, InDef.RewardPayload.Num());
}

void UOLCTravelSubsystem::ApplyEventHullDamage(float InPercent)
{
	if (InPercent <= 0.0f)
	{
		return;
	}

	HullDamageThisTrip = FMath::Min(100.0f, HullDamageThisTrip + InPercent);
	UE_LOG(LogOLC, Warning, TEXT("[OLC] Event hull damage: +%.1f%% (total %.1f%% this trip)"),
		InPercent, HullDamageThisTrip);
}

// ---------------------------------------------------------------------------
// Landing VFX hooks (WP-126 step 8; append-only)
// ---------------------------------------------------------------------------

void UOLCTravelSubsystem::HandleLandingPhaseChanged(EOLCLandingPhase OldPhase, EOLCLandingPhase NewPhase)
{
	if (NewPhase == EOLCLandingPhase::Descend && OldPhase != EOLCLandingPhase::Descend)
	{
		StartCrashSmokeTrail();
	}
	else if (OldPhase == EOLCLandingPhase::Descend && NewPhase != EOLCLandingPhase::Descend)
	{
		StopCrashSmokeTrail();
	}
}

void UOLCTravelSubsystem::HandleLandingComplete(const FString& InDestinationPlanetId, float InLandingRiskLevel)
{
	PlayCrashImpactVFXAtTouchdown();
}

FVector UOLCTravelSubsystem::GetTouchdownLocation() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return FVector::ZeroVector;
	}

	// The player pawn's location is the best proxy for the touchdown point.
	const APlayerController* PC = World->GetFirstPlayerController();
	if (PC && PC->GetPawn())
	{
		return PC->GetPawn()->GetActorLocation();
	}

	// Fall back to the level's PlayerStart, then to the world origin.
	for (TActorIterator<APlayerStart> It(World); It; ++It)
	{
		if (const APlayerStart* Start = *It)
		{
			return Start->GetActorLocation();
		}
	}

	return FVector::ZeroVector;
}

void UOLCTravelSubsystem::StartCrashSmokeTrail()
{
	UWorld* World = GetWorld();
	if (!World || CrashSmokeTrailActor.IsValid())
	{
		return;
	}

	const UGameInstance* GI = World->GetGameInstance();
	if (GI)
	{
		if (UOLCVFXSubsystem* VFX = GI->GetSubsystem<UOLCVFXSubsystem>())
		{
			const FVector Touchdown = GetTouchdownLocation();
			CrashSmokeTrailActor = VFX->PlayCrashSmoke(Touchdown);

			// Diagnostic: make the Descend-entry hook observable in PIE logs (WP-126 step-8 verification).
			UE_LOG(LogOLC, Log, TEXT("[OLC] Landing VFX: smoke trail %s at %s"),
				CrashSmokeTrailActor.IsValid() ? TEXT("started") : TEXT("FAILED to start"),
				*Touchdown.ToString());
		}
	}
}

void UOLCTravelSubsystem::StopCrashSmokeTrail()
{
	if (AActor* Smoke = CrashSmokeTrailActor.Get())
	{
		Smoke->Destroy();
		CrashSmokeTrailActor = nullptr;

		// Diagnostic: make the Descend-exit hook observable in PIE logs (WP-126 step-8 verification).
		UE_LOG(LogOLC, Log, TEXT("[OLC] Landing VFX: smoke trail stopped"));
	}
}

void UOLCTravelSubsystem::PlayCrashImpactVFXAtTouchdown()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const UGameInstance* GI = World->GetGameInstance();
	if (GI)
	{
		if (UOLCVFXSubsystem* VFX = GI->GetSubsystem<UOLCVFXSubsystem>())
		{
			const FVector Touchdown = GetTouchdownLocation();
			VFX->PlayCrashImpact(Touchdown);
			VFX->PlayCrashDust(Touchdown);

			// Diagnostic: make the touchdown hook observable in PIE logs (WP-126 step-8 verification).
			UE_LOG(LogOLC, Log, TEXT("[OLC] Landing VFX: impact + dust spawned at %s"), *Touchdown.ToString());
		}
	}
}

#undef LOCTEXT_NAMESPACE
