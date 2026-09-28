#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFramework/Actor.h"
#include "OLCUnitAnimationData.generated.h"

/**
 * Animation slots covered by the WP-128 animation system (17 total).
 * Infantry:  Idle / Walk / Run / Attack / Heal
 * Vehicle:   Move / Fire / Damage (Idle shared)
 * Aerial:    Hover / Fly / Crash (Attack shared)
 * Ship:      Takeoff / Cruise / Descent / Touchdown
 * Building:  Assemble / Collapse
 */
UENUM(BlueprintType)
enum class EOLCAnimSlot : uint8
{
	Idle      UMETA(DisplayName = "Idle"),
	Walk      UMETA(DisplayName = "Walk"),
	Run       UMETA(DisplayName = "Run"),
	Attack    UMETA(DisplayName = "Attack"),
	Heal      UMETA(DisplayName = "Heal"),
	Move      UMETA(DisplayName = "Move"),
	Fire      UMETA(DisplayName = "Fire"),
	Damage    UMETA(DisplayName = "Damage"),
	Hover     UMETA(DisplayName = "Hover"),
	Fly       UMETA(DisplayName = "Fly"),
	Crash     UMETA(DisplayName = "Crash"),
	Takeoff   UMETA(DisplayName = "Takeoff"),
	Cruise    UMETA(DisplayName = "Cruise"),
	Descent   UMETA(DisplayName = "Descent"),
	Touchdown UMETA(DisplayName = "Touchdown"),
	Assemble  UMETA(DisplayName = "Assemble"),
	Collapse  UMETA(DisplayName = "Collapse"),
};

/**
 * Single animation binding: one slot -> asset + playback parameters.
 */
USTRUCT(BlueprintType)
struct FOLCAnimationEntry
{
	GENERATED_BODY()

	/** Which animation slot this entry fills. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Animation")
	EOLCAnimSlot Slot = EOLCAnimSlot::Idle;

	/**
	 * Animation asset to play for this slot.
	 * Typed as UObject* with AllowAnyAsset so UAnimSequence (and, once the
	 * WP-128 animation pipeline lands, other visual assets) can be bound
	 * without re-typing the data class.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Animation", meta = (AllowAnyAsset = "true"))
	TObjectPtr<UObject> AnimationAsset = nullptr;

	/** Loop the animation (cycle) vs. play once. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Animation")
	bool bLoop = true;

	/** Playback rate multiplier (1.0 = native speed). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Animation", meta = (ClampMin = "0.0", ClampMax = "8.0"))
	float PlayRate = 1.0f;
};

/**
 * DataAsset holding the full animation set for one unit type (WP-128).
 * Pure data — consumed by the playback layer added in later WP-128 steps.
 */
UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCUnitAnimationSet : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Stable unit type identifier (matches UOLCUnitData::UnitId naming). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Animation")
	FString UnitTypeId;

	/** Optional actor class this set targets (informational; null = any). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Animation")
	TSubclassOf<AActor> TargetClass = nullptr;

	/** Animation bindings for this unit type. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Animation")
	TArray<FOLCAnimationEntry> Entries;

	/**
	 * Find the entry bound to the given slot.
	 * Null-safe: returns false and leaves OutEntry untouched when no entry
	 * exists for the slot; on success fills OutEntry with the binding.
	 */
	UFUNCTION(BlueprintPure, Category = "OLC|Animation")
	bool FindEntry(EOLCAnimSlot Slot, FOLCAnimationEntry& OutEntry) const;

	/**
	 * Whether a usable entry (existing + non-null asset) exists for the slot.
	 */
	UFUNCTION(BlueprintPure, Category = "OLC|Animation")
	bool HasEntry(EOLCAnimSlot Slot) const;
};
