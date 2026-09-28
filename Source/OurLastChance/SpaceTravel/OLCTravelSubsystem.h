#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Containers/Ticker.h"
#include "Combat/OLCMinigameManager.h"
#include "SpaceTravel/OLCTravelConfigData.h"
#include "SpaceTravel/OLCSpaceEventData.h"
#include "SpaceTravel/OLCSpaceWeatherData.h"
#include "SpaceTravel/OLLandingSequence.h"
#include "OLCTravelSubsystem.generated.h"

/**
 * Interplanetary travel state machine phases (WP-122).
 *
 * Flow: Idle -> Depart -> Transit -> EventCheck -> Arrive -> Land -> Idle
 *
 * - Depart:     ship launch animation window; trip fuel + energy are deducted here
 *               (prepaid at depart, per WP-122 step 1 / WP-10 travel-cost semantics).
 * - Transit:    warp-route flight along the warp route.
 * - EventCheck: space-event roll window (event system lands here in a later step).
 * - Arrive:     destination reached; rests until BeginLanding() is called.
 * - Land:       UOLLandingSequence drives Approach -> SafetyAssessment -> Descend -> Touchdown;
 *               CompleteLanding() (automatic on finish, or manual) returns to Idle.
 */
UENUM(BlueprintType)
enum class EOLCTravelState : uint8
{
	Idle       UMETA(DisplayName = "Idle"),
	Depart     UMETA(DisplayName = "Depart"),
	Transit    UMETA(DisplayName = "Transit"),
	EventCheck UMETA(DisplayName = "Event Check"),
	Arrive     UMETA(DisplayName = "Arrive"),
	Land       UMETA(DisplayName = "Land")
};

/** Fired on every travel state transition (old, new). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOLCTravelStateChanged, EOLCTravelState, OldState, EOLCTravelState, NewState);

/**
 * Fired when a transit event resolves to a pirate encounter.
 * Payload: destination planet id of the interrupted trip.
 * The tactical combat system (WP-113) owns the combat itself; this delegate is the handoff point.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOLCPirateCombatHandoff, const FString&, DestinationPlanetId);

/**
 * Fired when a transit event resolves to a merchant — the trade hook for the UI
 * (transit trade screen with +20% markup is presented by a later step).
 * Payload: destination planet id of the trip.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOLCMerchantTradeOffered, const FString&, DestinationPlanetId);

/**
 * Fired when a space weather condition starts (bIsActive = true) or expires
 * (bIsActive = false) during Transit — the hook for HUD toasts/overlays.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOLCSpaceWeatherChanged, EOLCSpaceWeatherType, Weather, bool, bIsActive);

class UOLCUIDataSubsystem;

/**
 * GameInstanceSubsystem owning the interplanetary travel state machine (WP-122 step 2).
 *
 * Tick note: UGameInstanceSubsystem has no virtual Tick in UE 5.8 and nothing in this
 * project drives research-style subsystem ticks, so this class self-registers an
 * FTSTicker delegate in Initialize() and removes it in Deinitialize().
 */
UCLASS()
class OURLASTCHANCE_API UOLCTravelSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// -----------------------------------------------------------------------
	// Entry point
	// -----------------------------------------------------------------------
	/**
	 * Start a trip from Idle — the depart hook the Solar System UI (WP-10) calls.
	 * Rejects when already traveling, when the drive is Offline, or when the ship
	 * cannot afford the trip cost. On success it deducts fuel + energy proportional
	 * to the drive-tier profile and route distance BEFORE Transit begins, then moves
	 * Idle -> Depart.
	 * @param InDestinationPlanetId  Planet id being traveled to (non-empty).
	 * @param DriveTier              Installed drive tier (1-6; clamped). Higher tier = faster + cheaper.
	 * @param RouteDistanceUnits     Route distance in units; <= 0 uses the config default.
	 * @return true if the trip started (state moved Idle -> Depart).
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Travel")
	bool BeginTravel(const FString& InDestinationPlanetId, int32 DriveTier, float RouteDistanceUnits = -1.0f);

	// -----------------------------------------------------------------------
	// State queries
	// -----------------------------------------------------------------------
	/** Current travel state. */
	UFUNCTION(BlueprintPure, Category = "OLC|Travel")
	EOLCTravelState GetTravelState() const { return CurrentState; }

	/** Display name of the current travel state (for HUD toasts). */
	UFUNCTION(BlueprintPure, Category = "OLC|Travel")
	FString GetTravelStateName() const;

	/** True while the ship is in Depart, Transit or EventCheck. */
	UFUNCTION(BlueprintPure, Category = "OLC|Travel")
	bool IsInTransit() const;

	/** Transit clock progress as a 0.0-1.0 fraction (1.0 before/after transit). */
	UFUNCTION(BlueprintPure, Category = "OLC|Travel")
	float GetTransitProgress() const;

	/** Destination planet id of the active/last trip. */
	UFUNCTION(BlueprintPure, Category = "OLC|Travel")
	FString GetDestinationPlanetId() const { return DestinationPlanetId; }

	/** Drive tier used by the active/last trip. */
	UFUNCTION(BlueprintPure, Category = "OLC|Travel")
	int32 GetActiveDriveTier() const { return ActiveDriveTier; }

	/** Fuel prepaid for the active/last trip at depart (resets on BeginTravel). */
	UFUNCTION(BlueprintPure, Category = "OLC|Travel")
	float GetFuelSpentThisTrip() const { return FuelSpentThisTrip; }

	/** Energy prepaid for the active/last trip at depart (resets on BeginTravel). */
	UFUNCTION(BlueprintPure, Category = "OLC|Travel")
	float GetEnergySpentThisTrip() const { return EnergySpentThisTrip; }

	// -----------------------------------------------------------------------
	// Space events (WP-122 step 4)
	// -----------------------------------------------------------------------
	/** Space event rolled during the last EventCheck phase (None = no event). */
	UFUNCTION(BlueprintPure, Category = "OLC|Travel")
	EOLCSpaceEventType GetLastEvent() const { return CurrentEvent; }

	/** Cumulative hull damage in percent from this trip's space events (0-100).
	    Consumed by the landing-type evaluation (hull + weather, later step). */
	UFUNCTION(BlueprintPure, Category = "OLC|Travel")
	float GetHullDamageThisTrip() const { return HullDamageThisTrip; }

	/** The minigame manager instance owned by this subsystem (drives in-transit minigames). */
	UFUNCTION(BlueprintPure, Category = "OLC|Travel")
	UOLCMinigameManager* GetMinigameManager() const { return MinigameManager; }

	// -----------------------------------------------------------------------
	// Space weather (WP-122 step 5)
	// -----------------------------------------------------------------------
	/** Weather currently active during Transit (None = clear skies or window expired). */
	UFUNCTION(BlueprintPure, Category = "OLC|Travel")
	EOLCSpaceWeatherType GetActiveWeather() const;

	/** Sensor/radar effectiveness while weather is active: 1.0 - active penalty (0-1). */
	UFUNCTION(BlueprintPure, Category = "OLC|Travel")
	float GetSensorEffectiveness() const;

	/** Seconds of the active weather's duration window remaining (0 when inactive). */
	UFUNCTION(BlueprintPure, Category = "OLC|Travel")
	float GetWeatherTimeRemaining() const { return FMath::Max(0.0f, WeatherRemainingSeconds); }

	// -----------------------------------------------------------------------
	// Landing hooks (UOLLandingSequence drives the Land state, step 8)
	// -----------------------------------------------------------------------
	/** Arrive -> Land and start the landing sequence (Approach -> SafetyAssessment -> Descend -> Touchdown).
	    Returns false unless currently in Arrive. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Travel")
	bool BeginLanding();

	/** Land -> Idle. No-op unless currently in Land. Also called automatically when the
	    landing sequence finishes (touchdown or blocked). */
	UFUNCTION(BlueprintCallable, Category = "OLC|Travel")
	void CompleteLanding();

	/** The landing sequence instance for the active/last trip (created in BeginLanding). */
	UFUNCTION(BlueprintPure, Category = "OLC|Travel")
	UOLLandingSequence* GetLandingSequence() const { return LandingSequence; }

	// -----------------------------------------------------------------------
	// Configuration (WP-122 step 3)
	// -----------------------------------------------------------------------
	/**
	 * Authored travel config DataAsset. When set, all timing and drive-tier profiles
	 * come from it; when null the class CDO fallback table is used, so the subsystem
	 * runs without any authored asset.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Travel")
	TObjectPtr<UOLCTravelConfigData> TravelConfigAsset;

	/**
	 * Authored space event DataAsset. When set, the EventCheck roll uses its table;
	 * when null the class CDO fallback table is used (five default events).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Travel")
	TObjectPtr<UOLCSpaceEventData> SpaceEventData;

	/**
	 * Authored space weather DataAsset. When set, the Transit roll uses its table;
	 * when null the class CDO fallback table is used (five default effects).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Travel")
	TObjectPtr<UOLCSpaceWeatherData> SpaceWeatherAsset;

	// -----------------------------------------------------------------------
	// Events (for Blueprint binding)
	// -----------------------------------------------------------------------
	/** Fired on every state transition. */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Travel")
	FOnOLCTravelStateChanged OnTravelStateChanged;

	/** Fired when a transit event resolves to a pirate encounter (WP-113 handles combat). */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Travel")
	FOnOLCPirateCombatHandoff OnPirateCombatHandoff;

	/** Fired when a transit event resolves to a merchant (trade hook for the UI). */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Travel")
	FOnOLCMerchantTradeOffered OnMerchantTradeOffered;

	/** Fired when space weather starts or expires during Transit (HUD toast/overlay hook). */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Travel")
	FOnOLCSpaceWeatherChanged OnSpaceWeatherChanged;

private:
	/** FTSTicker callback — advances the state machine clock. Returns true to keep ticking. */
	bool HandleTick(float DeltaTime);

	/** Move to NewState, log it, and broadcast OnTravelStateChanged. */
	void ChangeState(EOLCTravelState NewState);

	/** Active config: the assigned DataAsset, or its class CDO as code-side fallback. */
	const UOLCTravelConfigData* GetActiveTravelConfig() const;

	/** Active event table: the assigned DataAsset, or its class CDO as code-side fallback. */
	const UOLCSpaceEventData* GetActiveSpaceEventData() const;

	/** Roll (or not) one space event for this trip and resolve its outcome. Called once per EventCheck entry. */
	void ResolveEventCheck();

	/** Apply the resolved outcome of one event definition. */
	void ResolveEventOutcome(const FOLCSpaceEventDefinition& InDef);

	/** Grant an event's reward payload into the UI resource counters. */
	void GrantEventReward(const FOLCSpaceEventDefinition& InDef, UOLCUIDataSubsystem* UI, const TCHAR* Context);

	/** Add hazard hull damage (percent) to this trip's accumulated total (capped at 100). */
	void ApplyEventHullDamage(float InPercent);

	// -----------------------------------------------------------------------
	// Space weather (WP-122 step 5)
	// -----------------------------------------------------------------------
	/** Active weather table: the assigned DataAsset, or its class CDO as code-side fallback. */
	const UOLCSpaceWeatherData* GetActiveSpaceWeatherData() const;

	/** Roll (or not) this trip's space weather. Called once when Transit begins. */
	void RollSpaceWeather();

	/** Tick the active weather: continuous hull/fuel effects, expiry, and broadcast. */
	void UpdateActiveWeather(float DeltaTime);

	/** Transit clock speed factor of the active weather (1.0 when inactive). */
	float GetWeatherSpeedFactor() const;

	EOLCTravelState CurrentState = EOLCTravelState::Idle;

	FString DestinationPlanetId;

	int32 ActiveDriveTier = 1;

	/** Seconds elapsed in the current state (reset on every transition). */
	float StateElapsedSeconds = 0.0f;

	/** Total duration of the current trip's Transit phase (set in BeginTravel). */
	float TransitTotalSeconds = 0.0f;

	/** Fuel prepaid for the active/last trip at depart (drive-tier profile x distance). */
	float FuelSpentThisTrip = 0.0f;

	/** Energy prepaid for the active/last trip at depart (drive-tier profile x distance). */
	float EnergySpentThisTrip = 0.0f;

	/** Route distance in units of the active/last trip (set in BeginTravel). */
	float RouteDistance = 1.0f;

	/** Space event rolled during the last EventCheck phase (None = no event). */
	EOLCSpaceEventType CurrentEvent = EOLCSpaceEventType::None;

	/** Cumulative hull damage in percent from this trip's space events (0-100). */
	float HullDamageThisTrip = 0.0f;

	/** Space weather rolled for this trip (None = clear skies); biases the EventCheck roll. */
	EOLCSpaceWeatherType TripWeather = EOLCSpaceWeatherType::None;

	/** Seconds remaining in the active weather's modifier window (0 = inactive/expired). */
	float WeatherRemainingSeconds = 0.0f;

	/** Minigame manager owned by this subsystem (created in Initialize). */
	UPROPERTY()
	TObjectPtr<UOLCMinigameManager> MinigameManager;

	/** Landing sequence owned by this subsystem (created in BeginLanding, step 8). */
	UPROPERTY()
	TObjectPtr<UOLLandingSequence> LandingSequence;

	/** Handle of the registered core ticker delegate. */
	FTSTicker::FDelegateHandle TickHandle;

	// -----------------------------------------------------------------------
	// Landing VFX hooks (WP-126 step 8; append-only)
	// -----------------------------------------------------------------------
	/** OnLandingPhaseChanged handler: start the smoke trail on Descend entry, stop it on Descend exit. */
	UFUNCTION()
	void HandleLandingPhaseChanged(EOLCLandingPhase OldPhase, EOLCLandingPhase NewPhase);

	/** OnLandingComplete handler: spawn impact explosion + landing dust at the touchdown point. */
	UFUNCTION()
	void HandleLandingComplete(const FString& InDestinationPlanetId, float InLandingRiskLevel);

	/** Resolve the touchdown point in the current world: player pawn, else PlayerStart, else origin. */
	FVector GetTouchdownLocation() const;

	/** Start the looping crash smoke trail at the touchdown point (Descend entry). No-op if already running. */
	void StartCrashSmokeTrail();

	/** Stop and destroy the crash smoke trail actor (Descend exit). No-op if not running. */
	void StopCrashSmokeTrail();

	/** Spawn impact explosion + landing dust at the touchdown point (OnLandingComplete). */
	void PlayCrashImpactVFXAtTouchdown();

	/** Looping crash smoke trail actor active during the landing Descend phase (null when not descending). */
	UPROPERTY()
	TWeakObjectPtr<AActor> CrashSmokeTrailActor;
};
