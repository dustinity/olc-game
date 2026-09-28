#include "OLCProductionFacility.h"
#include "OurLastChance.h"

#include "Core/OLCUIDataSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"
#include "World/OLCUnitBase.h"

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
		CompleteActiveProduction();

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

	UE_LOG(LogOLC, Log, TEXT("[OLC] Queued product '%s' (duration=%.1fs, queue=%d)"),
		*ProductName.ToString(), Duration, GetQueueSize());
}

bool AOLCProductionFacility::QueueUnitProduction(UOLCUnitData* UnitDataAsset, bool bHasAirfield)
{
	if (!UnitDataAsset)
	{
		return false;
	}
	if (UnitDataAsset->UnitType != EOLCUnitType::Vehicle && UnitDataAsset->UnitType != EOLCUnitType::Aerial)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Factory cannot produce infantry unit '%s'"), *UnitDataAsset->DisplayName.ToString());
		return false;
	}
	if (UnitDataAsset->bRequiresAirfield && !bHasAirfield)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Factory cannot produce aerial unit '%s' without Airfield"), *UnitDataAsset->DisplayName.ToString());
		return false;
	}

	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
		{
			if (Data->GetColonyTIR() < UnitDataAsset->TIRRequirement || !Data->CanAffordBuild(UnitDataAsset->BuildCost))
			{
				return false;
			}
			if (!Data->ConsumeResourcesForBuild(UnitDataAsset->BuildCost))
			{
				return false;
			}
		}
	}

	FOLCProductionQueueEntry Entry;
	Entry.ProductName = UnitDataAsset->DisplayName;
	Entry.Duration = FMath::Max(UnitDataAsset->ProductionTimeSeconds, UnitDataAsset->TrainingTime * 2.0f);
	Entry.Progress = 0.0f;
	Entry.UnitDataAsset = UnitDataAsset;
	ProductionQueue.Add(Entry);

	if (!bHasActiveProduction)
	{
		bHasActiveProduction = true;
		ActiveProduction = ProductionQueue[0];
		ProductionQueue.RemoveAt(0);
	}
	return true;
}

bool AOLCProductionFacility::GetActiveUnitProduction(UOLCUnitData*& OutUnitData, float& OutProgress) const
{
	if (!bHasActiveProduction || !ActiveProduction.UnitDataAsset)
	{
		OutUnitData = nullptr;
		OutProgress = 0.0f;
		return false;
	}
	OutUnitData = ActiveProduction.UnitDataAsset;
	OutProgress = ActiveProduction.Progress;
	return true;
}

void AOLCProductionFacility::CompleteActiveProduction()
{
	UE_LOG(LogOLC, Log, TEXT("[OLC] Production complete: %s"), *ActiveProduction.ProductName.ToString());
	if (ActiveProduction.UnitDataAsset && GetWorld())
	{
		const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 260.0f + FVector(0.0f, 0.0f, 60.0f);
		if (AOLCUnitBase* SpawnedUnit = GetWorld()->SpawnActor<AOLCUnitBase>(AOLCUnitBase::StaticClass(), SpawnLocation, GetActorRotation()))
		{
			SpawnedUnit->SetUnitData(ActiveProduction.UnitDataAsset);
			if (UGameInstance* GI = GetWorld()->GetGameInstance())
			{
				if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
				{
					Data->AddUnit();
				}
			}
		}
	}
}
