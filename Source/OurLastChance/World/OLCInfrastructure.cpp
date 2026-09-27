#include "OLCInfrastructure.h"

#include "Core/OLCUIDataSubsystem.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"
#include "World/OLCUnitBase.h"

AOLCInfrastructure::AOLCInfrastructure()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOLCInfrastructure::BeginPlay()
{
	Super::BeginPlay();

	// Configure footprint collision based on building type.
	if (Footprint)
	{
		Footprint->SetCollisionEnabled(bBlocksMovement ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::QueryOnly);
		Footprint->SetGenerateOverlapEvents(bBlocksMovement);
	}

	UE_LOG(LogTemp, Log, TEXT("[OLC] Infrastructure '%s' online: housing=%d, storage+%.0f, blocks=%s"),
		*BuildingData.DisplayName.ToString(),
		UnitHousingBonus,
		StorageCapacityBonus,
		bBlocksMovement ? TEXT("YES") : TEXT("NO"));
}

void AOLCInfrastructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTraining(DeltaTime);
}

void AOLCInfrastructure::UpdateTraining(float DeltaTime)
{
	if (!bIsTraining || !ActiveTraining) return;

	// Advance progress.
	ActiveTraining->Progress += DeltaTime / ActiveTraining->Duration;

	if (ActiveTraining->Progress >= 1.0f)
	{
		CompleteTraining();
	}
}

void AOLCInfrastructure::CompleteTraining()
{
	if (!ActiveTraining || !ActiveTraining->UnitDataAsset)
	{
		ActiveTraining = nullptr;
		bIsTraining = false;
		return;
	}

	// Spawn the trained unit at this building's location.
	const FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);
	const FRotator SpawnRotation = GetActorRotation();

	UE_LOG(LogTemp, Log, TEXT("[OLC] Infrastructure '%s' finished training unit '%s' at %s"),
		*BuildingData.DisplayName.ToString(),
		*ActiveTraining->UnitName.ToString(),
		*SpawnLocation.ToString());

	// Spawn using the UnitDataAsset's DisplayName to look up a Blueprint class.
	// In production, this would use a pre-registered unit class map.
	// For now, spawn a generic AOLCUnitBase placeholder that can be replaced
	// by Blueprint subclasses in editor.
	AOLCUnitBase* SpawnedUnit = GetWorld()->SpawnActor<AOLCUnitBase>(AOLCUnitBase::StaticClass(), SpawnLocation, SpawnRotation);

	if (SpawnedUnit)
	{
		SpawnedUnit->SetUnitData(ActiveTraining->UnitDataAsset);

		// Register unit with subsystem.
		if (UGameInstance* GI = GetWorld()->GetGameInstance())
		{
			if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
			{
				Data->AddUnit();
			}
		}

		UE_LOG(LogTemp, Log, TEXT("[OLC] Unit '%s' spawned at %s"),
			*ActiveTraining->UnitName.ToString(), *SpawnLocation.ToString());
	}

	// Remove from queue and promote next.
	TrainingQueue.RemoveAt(0);

	if (TrainingQueue.Num() > 0)
	{
		ActiveTraining = &TrainingQueue[0];
		UE_LOG(LogTemp, Log, TEXT("[OLC] Infrastructure '%s' started training next unit: %s"),
			*BuildingData.DisplayName.ToString(), *ActiveTraining->UnitName.ToString());
	}
	else
	{
		ActiveTraining = nullptr;
		bIsTraining = false;
		UE_LOG(LogTemp, Log, TEXT("[OLC] Infrastructure '%s' training queue empty — idle"),
			*BuildingData.DisplayName.ToString());
	}
}

void AOLCInfrastructure::ToggleGate()
{
	if (!bBlocksMovement) return; // Only gates toggle.

	bGateOpen = !bGateOpen;

	// Disable collision when gate is open.
	if (Footprint)
	{
		Footprint->SetCollisionEnabled(bGateOpen ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);
	}

	UE_LOG(LogTemp, Log, TEXT("[OLC] Gate '%s' %s"),
		*BuildingData.DisplayName.ToString(),
		bGateOpen ? TEXT("OPEN") : TEXT("CLOSED"));
}

void AOLCInfrastructure::StartTraining(UOLCUnitData* UnitDataAsset)
{
	if (!UnitDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] StartTraining called with null UnitDataAsset on '%s'"),
			*BuildingData.DisplayName.ToString());
		return;
	}

	FOLCTrainingQueueEntry Entry;
	Entry.UnitDataAsset = UnitDataAsset;
	Entry.Duration = UnitDataAsset->TrainingTime;
	Entry.Progress = 0.0f;
	Entry.UnitName = UnitDataAsset->DisplayName;

	TrainingQueue.Add(Entry);

	if (!bIsTraining || !ActiveTraining)
	{
		// Start active training immediately.
		ActiveTraining = &TrainingQueue[0];
		bIsTraining = true;
	}

	UE_LOG(LogTemp, Log, TEXT("[OLC] Infrastructure '%s' queued unit '%s' (queue size: %d)"),
		*BuildingData.DisplayName.ToString(),
		*Entry.UnitName.ToString(),
		TrainingQueue.Num());
}
