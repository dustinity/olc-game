#include "Core/OLCGalaxyTransitionSubsystem.h"
#include "OurLastChance.h"

#include "Logging/LogMacros.h"

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------

void UOLCGalaxyTransitionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Self-register a core ticker delegate to advance the state machine clock every frame.
	TickHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(this, &UOLCGalaxyTransitionSubsystem::HandleTick), 0.0f);

	UE_LOG(LogOLC, Log, TEXT("[OLC] Galaxy transition subsystem initialized"));
}

void UOLCGalaxyTransitionSubsystem::Deinitialize()
{
	FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);

	Super::Deinitialize();

	CurrentPhase = EOLCGalaxyTransitionPhase::None;
	PhaseElapsedSeconds = 0.0f;
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------

bool UOLCGalaxyTransitionSubsystem::BeginTransition()
{
	// Reject if already transitioning.
	if (CurrentPhase != EOLCGalaxyTransitionPhase::None)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] GalaxyTransition: BeginTransition rejected — already in phase %d"),
			static_cast<int32>(CurrentPhase));
		return false;
	}

	// Start from Shatter.
	PhaseElapsedSeconds = 0.0f;
	AdvancePhase(EOLCGalaxyTransitionPhase::Shatter);

	UE_LOG(LogOLC, Display, TEXT("[OLC] Galaxy transition started: Shatter (%.1fs) -> Warp (%.1fs) -> NewStarfield (%.1fs)"),
		Timing.ShatterDurationSeconds, Timing.WarpDurationSeconds, Timing.NewStarfieldDurationSeconds);

	return true;
}

// ---------------------------------------------------------------------------
// Tick — advance the state machine
// ---------------------------------------------------------------------------

bool UOLCGalaxyTransitionSubsystem::HandleTick(float DeltaTime)
{
	// Only tick when a phase is active.
	if (CurrentPhase == EOLCGalaxyTransitionPhase::None)
	{
		return true; // Keep ticking so BeginTransition can be called later.
	}

	PhaseElapsedSeconds += DeltaTime;

	switch (CurrentPhase)
	{
		case EOLCGalaxyTransitionPhase::Shatter:
			if (PhaseElapsedSeconds >= Timing.ShatterDurationSeconds)
			{
				AdvancePhase(EOLCGalaxyTransitionPhase::Warp);
			}
			break;

		case EOLCGalaxyTransitionPhase::Warp:
			if (PhaseElapsedSeconds >= Timing.WarpDurationSeconds)
			{
				AdvancePhase(EOLCGalaxyTransitionPhase::NewStarfield);
			}
			break;

		case EOLCGalaxyTransitionPhase::NewStarfield:
			if (PhaseElapsedSeconds >= Timing.NewStarfieldDurationSeconds)
			{
				// Sequence complete — return to idle.
				const EOLCGalaxyTransitionPhase Finished = CurrentPhase;
				CurrentPhase = EOLCGalaxyTransitionPhase::None;
				PhaseElapsedSeconds = 0.0f;

				UE_LOG(LogOLC, Display, TEXT("[OLC] Galaxy transition complete (finished %d phase)"), static_cast<int32>(Finished));
				OnTransitionComplete.Broadcast();
			}
			break;

		default:
			break;
	}

	return true;
}

// ---------------------------------------------------------------------------
// Phase advancement
// ---------------------------------------------------------------------------

void UOLCGalaxyTransitionSubsystem::AdvancePhase(EOLCGalaxyTransitionPhase NewPhase)
{
	const EOLCGalaxyTransitionPhase OldPhase = CurrentPhase;
	CurrentPhase = NewPhase;
	PhaseElapsedSeconds = 0.0f;

	UE_LOG(LogOLC, Display, TEXT("[OLC] Galaxy transition: %d -> %d"),
		static_cast<int32>(OldPhase), static_cast<int32>(NewPhase));

	OnTransitionPhaseChanged.Broadcast(OldPhase, NewPhase);
}
