#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OLCChampionAnimationData.generated.h"

/**
 * Animation binding for one champion ability (WP-128 Step 6).
 * AbilityId matches FOLCChampionAbility::AbilityId in UOLCChampionData.
 */
USTRUCT(BlueprintType)
struct FOLCChampionAbilityAnim
{
	GENERATED_BODY()

	/** Stable ability identifier (e.g. "CloakField", "RapidBuild"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion")
	FString AbilityId;

	/**
	 * Animation asset to play on ability activation.
	 * Typed as UObject* with AllowAnyAsset so UAnimSequence (and, once the
	 * WP-128 animation pipeline lands, other visual assets) can be bound
	 * without re-typing the data class.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion", meta = (AllowAnyAsset = "true"))
	TObjectPtr<UObject> AnimationAsset = nullptr;

	/** Loop the animation vs. play once (one-shot ability casts typically false). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion")
	bool bLoop = false;

	/** Playback rate multiplier (1.0 = native speed). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion", meta = (ClampMin = "0.0", ClampMax = "8.0"))
	float PlayRate = 1.0f;
};

/**
 * DataAsset mapping a champion's ability IDs to their activation animations (WP-128).
 * Pure data — consumed by the playback layer added in later WP-128 steps.
 */
UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCChampionAnimationData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Champion identifier (matches UOLCChampionData::ChampionId, e.g. "CH-NP-01"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion")
	FString ChampionId;

	/** One entry per ability of this champion. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Champion")
	TArray<FOLCChampionAbilityAnim> AbilityAnims;

	/**
	 * Find the anim binding for an ability ID (case-insensitive).
	 * Null-safe: returns false and leaves OutAnim untouched when the id is
	 * empty or no entry exists; on success fills OutAnim with the binding.
	 */
	UFUNCTION(BlueprintPure, Category = "OLC|Champion")
	bool FindAbilityAnim(const FString& InAbilityId, FOLCChampionAbilityAnim& OutAnim) const;
};
