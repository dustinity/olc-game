#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Combat/OLCCombatTypes.h"
#include "Combat/OLCMinigameData.h"
#include "OLCMinigameManager.generated.h"

UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCMinigameManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	void StartMinigame(EOLCMinigameType Type, float DurationSeconds = 20.0f);

	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	void RegisterInput(const FString& InputToken);

	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	bool CompleteMinigame();

	/**
	 * Data-driven start (WP-122 step 6): configures any minigame type from a
	 * UOLCMinigameData instance instead of the legacy hardcoded switch. Looks up the
	 * row for InType in InData's table and loads its InputSequence; the existing
	 * RegisterInput/CompleteMinigame flow then works unchanged on that sequence.
	 * No-op when InData is null, has no row for InType, or the row's sequence is empty.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	void StartMinigameFromData(const UOLCMinigameData* InData, EOLCMinigameType InType);

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	float GetProgress() const { return Progress; }

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	bool IsActive() const { return bActive; }

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Combat")
	EOLCMinigameType CurrentType = EOLCMinigameType::WireRepair;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Combat")
	int32 Score = 0;

private:
	UPROPERTY()
	TArray<FString> RequiredSequence;

	int32 SequenceIndex = 0;
	float Progress = 0.0f;
	bool bActive = false;
};
