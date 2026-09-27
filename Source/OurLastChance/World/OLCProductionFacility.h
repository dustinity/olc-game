#pragma once

#include "CoreMinimal.h"
#include "World/OLCBuildingBase.h"
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

	/** Get current number of queued products. */
	UFUNCTION(BlueprintPure, Category = "OLC|Production")
	int32 GetQueueSize() const { return ProductionQueue.Num(); }

	/** Check if the production queue is empty. */
	UFUNCTION(BlueprintPure, Category = "OLC|Production")
	bool IsQueueEmpty() const { return ProductionQueue.IsEmpty(); }

protected:
	virtual void BeginPlay() override;

private:
	/** Queue of pending productions. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Production", meta = (AllowPrivateAccess = "true"))
	TArray<FOLCProductionQueueEntry> ProductionQueue;

	/** Currently active production (first in queue). */
	FOLCProductionQueueEntry ActiveProduction;

	/** Whether there is an active production in progress. */
	bool bHasActiveProduction = false;
};
