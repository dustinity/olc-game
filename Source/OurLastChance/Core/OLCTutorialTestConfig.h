#pragma once

// WP-129 Step 3 — TEST-ONLY configuration for deterministic PIE verification of the
// tutorial world-highlight flow. Every field defaults to OFF: when the canonical
// asset is absent or all fields are at defaults, runtime behavior is unchanged.
//
// Why this exists: the in-game console and the UMG menu widgets are not reachable
// from the MCP toolsets available in coder sessions (the PIE viewport and its game
// UI are opaque to the Slate accessibility tree), so "trigger TransitionToGameplay
// via console or menu" cannot be driven by key/click automation. This asset is the
// equivalent dev hook: it is mutated between PIE runs via MCP
// (ObjectTools.set_properties) — no recompile or editor restart per run.
//
// Canonical instance: /Game/OurLastChance/Data/TestConfig/DA_OLCTutorialTestConfig
// IMPORTANT: reset all fields to defaults after verification runs so later PIE
// sessions (WP-129 step 4/5) start with the hooks off.

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OLCTutorialTestConfig.generated.h"

UCLASS()
class OURLASTCHANCE_API UOLCTutorialTestConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Fixed content path of the canonical test-config instance. */
	static const TCHAR* GetCanonicalPath() { return TEXT("/Game/OurLastChance/Data/TestConfig/DA_OLCTutorialTestConfig"); }

	/** Load the canonical test-config asset (null when it does not exist → all hooks off). */
	static UOLCTutorialTestConfig* Load()
	{
		return LoadObject<UOLCTutorialTestConfig>(nullptr, GetCanonicalPath());
	}

	/**
	 * If > 0, the menu player controller dismisses the welcome screen and calls
	 * OLCMenuGameMode::TransitionToGameplay() this many seconds after BeginPlay
	 * (skipping the welcome screen and crash sequence). 0 = off.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|TutorialTest")
	float AutoTransitionSeconds = 0.0f;

	/**
	 * If > 0, the tutorial subsystem auto-completes objectives 1..N (by enum value)
	 * at AutoCompleteBaseDelaySeconds + 0.75 s * (i - 1) after world creation. 0 = off.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|TutorialTest")
	int32 AutoCompleteObjectives = 0;

	/** Delay (seconds after world creation) of the first auto-completion. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|TutorialTest")
	float AutoCompleteBaseDelaySeconds = 1.0f;

	/** If true, the tutorial subsystem skips the tutorial SkipDelaySeconds after world creation (terminal-state check). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|TutorialTest")
	bool bSkipOnStart = false;

	/** Delay (seconds after world creation) of the auto-skip. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|TutorialTest")
	float SkipDelaySeconds = 1.0f;
};
