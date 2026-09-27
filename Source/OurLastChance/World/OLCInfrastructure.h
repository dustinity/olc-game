#pragma once

#include "CoreMinimal.h"
#include "World/OLCBuildingBase.h"
#include "Core/OLCUnitData.h"
#include "OLCInfrastructure.generated.h"

class UGameInstance;
class UOLCUIDataSubsystem;

// ---------------------------------------------------------------------------
// Training queue entry — tracks a single unit being trained at a building
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCTrainingQueueEntry
{
	GENERATED_BODY()

	/** Unit data asset to spawn when training completes. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Training")
	TObjectPtr<UOLCUnitData> UnitDataAsset;

	/** Training progress (0.0 to 1.0). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Training")
	float Progress = 0.0f;

	/** Total training duration in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Training")
	float Duration = 30.0f;

	/** Name of the unit being trained (for logging/UI). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Training")
	FText UnitName;
};

/**
 * Infrastructure buildings (barracks, hab modules, walls, gates).
 * Provides capacity bonuses: unit housing, storage expansion, terrain blocking.
 */
UCLASS()
class OURLASTCHANCE_API AOLCInfrastructure : public AOLCBuildingBase
{
	GENERATED_BODY()

public:
	AOLCInfrastructure();

	/** Get the unit housing bonus provided by this building. */
	UFUNCTION(BlueprintPure, Category = "OLC|Infrastructure")
	int32 GetUnitHousingBonus() const { return UnitHousingBonus; }

	/** Get the storage capacity bonus (in resource units). */
	UFUNCTION(BlueprintPure, Category = "OLC|Infrastructure")
	float GetStorageBonus() const { return StorageCapacityBonus; }

	/** Check if this building blocks movement (walls, etc.). */
	UFUNCTION(BlueprintPure, Category = "OLC|Infrastructure")
	bool BlocksMovement() const { return bBlocksMovement; }

	/** Toggle open/close state for gates. When open, movement is allowed through. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Infrastructure")
	void ToggleGate();

	/** Start training a unit (adds to queue). BlueprintCallable for editor wiring. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Infrastructure|Training")
	void StartTraining(UOLCUnitData* UnitDataAsset);

	/** Get the current active training entry (null if none). */
	const FOLCTrainingQueueEntry* GetActiveTraining() const { return ActiveTraining; }

	/** Get the number of units in the training queue. */
	UFUNCTION(BlueprintPure, Category = "OLC|Infrastructure|Training")
	int32 GetQueueSize() const { return TrainingQueue.Num(); }

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:
	/** Advance active training progress and complete when done. */
	void UpdateTraining(float DeltaTime);

	/** Spawn the trained unit at this building's location. */
	void CompleteTraining();

	// -----------------------------------------------------------------------
	// Training queue state
	// -----------------------------------------------------------------------
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Infrastructure|Training", meta = (AllowPrivateAccess = "true"))
	TArray<FOLCTrainingQueueEntry> TrainingQueue;

	/** Pointer into TrainingQueue for the currently active entry (or null). */
	FOLCTrainingQueueEntry* ActiveTraining = nullptr;

	FTimerHandle TrainingTimer;

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Infrastructure|Training", meta = (AllowPrivateAccess = "true"))
	bool bIsTraining = false;

private:
	/** Additional unit housing slots provided. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Infrastructure", meta = (AllowPrivateAccess = "true"))
	int32 UnitHousingBonus = 0;

	/** Flat storage capacity increase for all resources. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Infrastructure", meta = (AllowPrivateAccess = "true"))
	float StorageCapacityBonus = 0.0f;

	/** Whether this building blocks unit movement (walls, fences). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Infrastructure", meta = (AllowPrivateAccess = "true"))
	bool bBlocksMovement = false;

	/** Gate open/closed state — when open, collision is disabled. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Infrastructure", meta = (AllowPrivateAccess = "true"))
	bool bGateOpen = false;
};
