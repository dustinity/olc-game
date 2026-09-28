// Copyright OLC Project. WP-128 Step 5 — append-only animation auto-attach subsystem.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/EngineTypes.h"
#include "OLCAnimationSubsystem.generated.h"

class UOLCUnitAnimationSet;
class AActor;

/**
 * Append-only integration layer for the WP-128 animation system.
 *
 * On Initialize and every 0.5 s this subsystem scans world actors, loads all
 * UOLCUnitAnimationSet assets from /Game/OurLastChance/Animation (AssetRegistry), and for
 * each unprocessed actor matching a set's TargetClass or UnitTypeId attaches a
 * UOLCAnimationPlayerComponent configured with that set. Building-category sets
 * (TargetClass under AOLCBuildingBase, "PB-" unit id, or Assemble/Collapse
 * entries) additionally attach a UOLCBuildingSequenceComponent.
 *
 * Processed actors are tracked in a TSet of weak pointers: attachment happens
 * exactly once per actor, and destroyed actors are pruned automatically on the
 * next scan. No existing C++ constructor or Blueprint is edited — this is the
 * only integration point (append-only compliance). With zero DataAssets present
 * the subsystem is a silent no-op.
 */
UCLASS()
class OURLASTCHANCE_API UOLCAnimationSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Number of actors that currently have animation components attached. */
	UFUNCTION(BlueprintPure, Category = "OLC|Animation")
	int32 GetProcessedActorCount() const { return ProcessedActors.Num(); }

	/** Force an immediate scan (the automatic scan runs every 0.5 s). */
	UFUNCTION(BlueprintCallable, Category = "OLC|Animation")
	void RescanNow();

protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

private:
	/** Scan cadence in seconds (brief: 0.5 s interval → matched actors attach within ~1 s). */
	static constexpr float ScanIntervalSeconds = 0.5f;

	FTimerHandle ScanTimerHandle;

	/** Animation sets currently present under /Game/OurLastChance/Animation (refreshed each scan). */
	TArray<TObjectPtr<UOLCUnitAnimationSet>> CachedSets;

	/** Actors already processed — exactly-once attachment; weak pointers prune on destruction. */
	TSet<TWeakObjectPtr<AActor>> ProcessedActors;

	void RescanWorld();
	void RefreshCachedSets();
	UOLCUnitAnimationSet* FindMatchingSet(const AActor& Actor) const;
	static bool IsBuildingCategorySet(const UOLCUnitAnimationSet* Set);
	void AttachComponentsFor(AActor* Actor, UOLCUnitAnimationSet* Set);
};
