#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Combat/OLCCombatTypes.h"
#include "OLCCombatPhaseData.generated.h"

USTRUCT(BlueprintType)
struct FOLCCombatPhaseSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	EOLCCombatPhase Phase = EOLCCombatPhase::Engage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	float DurationSeconds = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	FText Description;
};

UCLASS()
class OURLASTCHANCE_API UOLCCombatPhaseData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	TArray<FOLCCombatPhaseSettings> PhaseSettings;
};
