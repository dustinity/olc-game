// WP-128 Step 9 — test-only hook for the animation smoke test.
//
// Exposes UOLCBuildingSequenceComponent's protected TickComponent so the
// automation test can drive the sequence clock directly. Manually pumping
// UWorld::Tick does not reach component ticks in a standalone dummy world;
// TickComponent(LEVELTICK_All, nullptr) is the engine AITestSuite pattern.

#pragma once

#include "CoreMinimal.h"
#include "Animation/OLCBuildingSequenceComponent.h"
// UE 5.8: the generated header must precede the class declaration — it defines
// CURRENT_FILE_ID and the per-line PROLOG/BODY macros UCLASS()/GENERATED_BODY() expand to.
#include "OLCAnimationSmokeTestHook.generated.h"

UCLASS()
class OURLASTCHANCE_API UOLCAnimSmokeTestBuildingSeq : public UOLCBuildingSequenceComponent
{
	GENERATED_BODY()

public:
	/** Advance the sequence clock by DeltaSeconds, as the owning actor's tick would. */
	void TickForTest(float DeltaSeconds)
	{
		TickComponent(DeltaSeconds, LEVELTICK_All, nullptr);
	}
};
