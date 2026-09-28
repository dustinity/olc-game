#pragma once

#include "CoreMinimal.h"
#include "Core/OLCRaceData.h"
#include "OLCBossRaceData.generated.h"

// ---------------------------------------------------------------------------
// Boss tier — matches WP-118's None/Standard/Large/Fortress/CentralGalaxy
// classification. UOLCBossRaceData itself is only ever Standard or above;
// "None" describes an ordinary (non-boss) UOLCRaceData instance.
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCBossTier : uint8
{
	Standard      UMETA(DisplayName = "Standard Boss"),
	Large         UMETA(DisplayName = "Large Boss"),
	Fortress      UMETA(DisplayName = "Fortress Boss"),
	CentralGalaxy UMETA(DisplayName = "Central Galaxy Boss"),
};

// ---------------------------------------------------------------------------
// One phase of a boss's 4-phase encounter (intro -> enrage -> desperate -> final).
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCBossPhaseData
{
	GENERATED_BODY()

	/** Display label, e.g. "Intro", "Enrage", "Desperate", "Final". */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Boss|Phase")
	FText PhaseName;

	/** This phase begins once boss HP falls to or below this percent (100 = intro). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Boss|Phase")
	float HPThresholdPercent = 100.0f;

	/** Outgoing damage multiplier while in this phase. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Boss|Phase")
	float DamageMultiplier = 1.0f;

	/** Whether this phase summons additional adds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Boss|Phase")
	bool bSummonsAdds = false;

	/** Short flavor/mechanic description for this phase. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Boss|Phase")
	FText PhaseDescription;

	FOLCBossPhaseData() {}

	FOLCBossPhaseData(const FText& InName, float InThreshold, float InDamageMult, bool InSummons, const FText& InDescription)
		: PhaseName(InName), HPThresholdPercent(InThreshold), DamageMultiplier(InDamageMult), bSummonsAdds(InSummons), PhaseDescription(InDescription)
	{}
};

/**
 * Boss-tier race — extends UOLCRaceData with a 4-phase encounter, a
 * boss-specific mechanic description, and its own weakness (distinct from
 * the base race's Weakness field, which may describe the trash-tier
 * variant of the same family instead).
 */
UCLASS()
class OURLASTCHANCE_API UOLCBossRaceData : public UOLCRaceData
{
	GENERATED_BODY()

public:
	UOLCBossRaceData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Boss")
	EOLCBossTier BossTier = EOLCBossTier::Standard;

	/** Always 4 entries: intro, enrage (70% HP), desperate (40% HP), final (15% HP). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Boss")
	TArray<FOLCBossPhaseData> Phases;

	/** e.g. "Spawns Drone waves", "Controls mechanical adds; must destroy AI Core first". */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Boss")
	FText SpecialMechanicDescription;

	/** If not enraged (see final phase), boss auto-defeats the player after this many seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Boss")
	float FinalPhaseEnrageTimerSeconds = 60.0f;

	/** Returns the phase whose HP threshold the given percent falls at or below (defaults to the first phase). */
	UFUNCTION(BlueprintCallable, Category = "OLC|Boss")
	FOLCBossPhaseData GetPhaseForHPPercent(float HPPercent) const;
};
