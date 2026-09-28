#pragma once

#include "CoreMinimal.h"
#include "World/OLCBuildingBase.h"
#include "Core/OLCUnitData.h"
#include "OLCProductionFacility.generated.h"

/** Pending production entry in the queue. */
USTRUCT(BlueprintType)
struct FOLCProductionQueueEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Production")
	FText ProductName;

	/** Progress toward completion (0.0 to 1.0). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Production")
	float Progress = 0.0f;

	/** Total time required in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Production")
	float Duration = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Production")
	TObjectPtr<UOLCUnitData> UnitDataAsset;

	FOLCProductionQueueEntry() {}
};

/**
 * Production facility buildings (fabricators, forges, refineries).
 * Manages a production queue with input/output resource conversion.
 */
UCLASS()
class OURLASTCHANCE_API AOLCProductionFacility : public AOLCBuildingBase
{
	GENERATED_BODY()

public:
	AOLCProductionFacility();

	virtual void Tick(float DeltaTime) override;

	/** Add a product to the production queue. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Production")
	void QueueProduct(const FText& ProductName, float Duration);

	UFUNCTION(BlueprintCallable, Category = "OLC|Production")
	bool QueueUnitProduction(UOLCUnitData* UnitDataAsset, bool bHasAirfield);

	/** Get current number of queued products. */
	UFUNCTION(BlueprintPure, Category = "OLC|Production")
	int32 GetQueueSize() const { return ProductionQueue.Num(); }

	/** Check if the production queue is empty. */
	UFUNCTION(BlueprintPure, Category = "OLC|Production")
	bool IsQueueEmpty() const { return ProductionQueue.IsEmpty(); }

	/** Get the unit currently in production and its progress (0-1), if the active entry is a unit (not a resource product). */
	UFUNCTION(BlueprintPure, Category = "OLC|Production")
	bool GetActiveUnitProduction(UOLCUnitData*& OutUnitData, float& OutProgress) const;

protected:
	virtual void BeginPlay() override;

private:
	void CompleteActiveProduction();

	/** Queue of pending productions. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Production", meta = (AllowPrivateAccess = "true"))
	TArray<FOLCProductionQueueEntry> ProductionQueue;

	/** Currently active production (first in queue). */
	FOLCProductionQueueEntry ActiveProduction;

	/** Whether there is an active production in progress. */
	bool bHasActiveProduction = false;
};
