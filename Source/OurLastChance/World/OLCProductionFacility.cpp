#include "OLCProductionFacility.h"

#include "Core/OLCUIDataSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"

AOLCProductionFacility::AOLCProductionFacility()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOLCProductionFacility::BeginPlay()
{
	Super::BeginPlay();

	if (BuildingData.OutputPerTick.Num() > 0)
	{
		// Auto-queue the primary output from building data.
		FText DefaultProduct = BuildingData.DisplayName;
		QueueProduct(DefaultProduct, 60.0f);
	}
}

void AOLCProductionFacility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsPowered() || !bHasActiveProduction) return;

	// Advance active production progress.
	ActiveProduction.Progress += DeltaTime / ActiveProduction.Duration;

	if (ActiveProduction.Progress >= 1.0f)
	{
		// Production complete — move to next in queue or clear.
		UE_LOG(LogTemp, Log, TEXT("[OLC] Production complete: %s"), *ActiveProduction.ProductName.ToString());

		ProductionQueue.RemoveAt(0);

		if (ProductionQueue.Num() > 0)
		{
			ActiveProduction = ProductionQueue[0];
			ProductionQueue.RemoveAt(0);
		}
		else
		{
			bHasActiveProduction = false;
			ActiveProduction = FOLCProductionQueueEntry();
		}
	}
}

void AOLCProductionFacility::QueueProduct(const FText& ProductName, float Duration)
{
	FOLCProductionQueueEntry Entry;
	Entry.ProductName = ProductName;
	Entry.Duration = Duration;
	Entry.Progress = 0.0f;

	ProductionQueue.Add(Entry);

	// Start active production if none running.
	if (!bHasActiveProduction)
	{
		bHasActiveProduction = true;
		ActiveProduction = ProductionQueue[0];
		ProductionQueue.RemoveAt(0);
	}

	UE_LOG(LogTemp, Log, TEXT("[OLC] Queued product '%s' (duration=%.1fs, queue=%d)"),
		*ProductName.ToString(), Duration, GetQueueSize());
}
