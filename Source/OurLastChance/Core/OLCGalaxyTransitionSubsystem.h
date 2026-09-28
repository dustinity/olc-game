#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Containers/Ticker.h"
#include "OLCGalaxyTransitionSubsystem.generated.h"

// ---------------------------------------------------------------------------
// Galaxy transition phases (WP-125 Step 5)
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCGalaxyTransitionPhase : uint8
{
	None        UMETA(DisplayName = "None"),
	Shatter     UMETA(DisplayName = "Shatter"),
	Warp        UMETA(DisplayName = "Warp"),
	NewStarfield UMETA(DisplayName = "New Starfield")
};

/** Fired on every transition phase change (old, new). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOLCGalaxyTransitionPhaseChanged, EOLCGalaxyTransitionPhase, OldPhase, EOLCGalaxyTransitionPhase, NewPhase);

/** Fired once when the entire transition sequence completes. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOLCGalaxyTransitionComplete);

/** Configurable durations for each phase of the galaxy transition sequence. */
USTRUCT(BlueprintType)
struct FOLCGalaxyTransitionTiming
{
	GENERATED_BODY()

	/** Duration of the Shatter phase in seconds (galaxy breaking apart). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Transition")
	float ShatterDurationSeconds = 3.0f;

	/** Duration of the Warp phase in seconds (travel through void/rift). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Transition")
	float WarpDurationSeconds = 2.0f;

	/** Duration of the NewStarfield phase in seconds (new galaxy reveal). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Transition")
	float NewStarfieldDurationSeconds = 3.0f;
};

/**
 * GameInstanceSubsystem owning the alien galaxy transition cinematic state machine (WP-125 Step 5).
 *
 * Sequence: Shatter → Warp → NewStarfield → Complete.
 * Self-ticks via FTSTicker (same pattern as UOLCTravelSubsystem). A Blueprint or UI layer
 * binds to the delegates to drive visual effects (particle shatter, warp tunnel, new starfield reveal).
 */
UCLASS()
class OURLASTCHANCE_API UOLCGalaxyTransitionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// -----------------------------------------------------------------------
	// Entry point
	// -----------------------------------------------------------------------

	/**
	 * Start the galaxy transition sequence from Shatter.
	 * @return true if the transition started; false if already transitioning.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Transition")
	bool BeginTransition();

	// -----------------------------------------------------------------------
	// State queries
	// -----------------------------------------------------------------------

	/** Current transition phase (None when idle or after completion). */
	UFUNCTION(BlueprintPure, Category = "OLC|Transition")
	EOLCGalaxyTransitionPhase GetCurrentPhase() const { return CurrentPhase; }

	/** True while any phase of the transition is actively playing. */
	UFUNCTION(BlueprintPure, Category = "OLC|Transition")
	bool IsTransitioning() const { return CurrentPhase != EOLCGalaxyTransitionPhase::None; }

	// -----------------------------------------------------------------------
	// Configuration
	// -----------------------------------------------------------------------

	/** Phase timing configuration (durations in seconds). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Transition")
	FOLCGalaxyTransitionTiming Timing;

	// -----------------------------------------------------------------------
	// Events (for Blueprint binding)
	// -----------------------------------------------------------------------

	/** Fired on every phase transition with the old and new phase. */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Transition")
	FOnOLCGalaxyTransitionPhaseChanged OnTransitionPhaseChanged;

	/** Fired exactly once when the full sequence completes (after NewStarfield ends). */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Transition")
	FOnOLCGalaxyTransitionComplete OnTransitionComplete;

private:
	/** FTSTicker callback — advances the state machine clock. Returns true to keep ticking. */
	bool HandleTick(float DeltaTime);

	/** Advance to the next phase, broadcasting the delegate. */
	void AdvancePhase(EOLCGalaxyTransitionPhase NewPhase);

	EOLCGalaxyTransitionPhase CurrentPhase = EOLCGalaxyTransitionPhase::None;

	/** Seconds elapsed in the current phase (reset on every transition). */
	float PhaseElapsedSeconds = 0.0f;

	/** Handle of the registered core ticker delegate. */
	FTSTicker::FDelegateHandle TickHandle;
};
