#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OLCEndgameEncounterData.generated.h"

/**
 * Data-driven configuration for the Center Galaxy final boss encounter (WP-125 Step 4).
 *
 * The constructor seeds sensible defaults so the class CDO doubles as the code-side
 * fallback: game systems read an authored instance when one is assigned and otherwise
 * fall back to GetDefault<UOLCEndgameEncounterData>() — it runs without any authored asset.
 */
UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCEndgameEncounterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UOLCEndgameEncounterData();

	/** Race ID of the boss this encounter targets (must match a registered UOLCBossRaceData). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Endgame")
	FString BossRaceId = TEXT("CenterGalaxyOverlord");

	/** Whether all TIR 5 (Outer Ring) research topics must be completed before this encounter can trigger. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Endgame")
	bool bRequiresAllTIR5Research = true;

	/** Preparation checklist items shown to the player during the final assault prep phase. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Endgame")
	TArray<FText> PrepChecklist;

	/** Human-readable description of what triggers this encounter. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Endgame")
	FText TriggerDescription;

	/** Minimum number of active Quantum Gates required for the supply line during the final assault. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Endgame")
	int32 MinActiveQuantumGates = 3;

	/** Seconds the preparation phase lasts (real-time). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Endgame")
	float PrepPhaseDurationSeconds = 60.0f;

	/** Minimum fleet HP percentage required to claim victory after defeating the boss. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Endgame")
	float MinFleetHPPercentForVictory = 10.0f;
};
