#include "OLCVehicleUnit.h"

#include "Core/OLCUIDataSubsystem.h"
#include "Core/OLCUnitData.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"

AOLCVehicleUnit::AOLCVehicleUnit()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOLCVehicleUnit::BeginPlay()
{
	Super::BeginPlay();

	FuelConsumptionPerMinute = UnitData ? UnitData->FuelConsumptionPerMinute : 0.0f;

	if (FuelConsumptionPerMinute > 0.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("[OLC] Vehicle '%s' fuel consumption: %.1f/min"),
			UnitData ? *UnitData->DisplayName.ToString() : TEXT("Unknown"), FuelConsumptionPerMinute);
	}
}

void AOLCVehicleUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FuelConsumptionPerMinute <= 0.0f || IsDead()) return;

	// Drain fuel over time.
	FuelAccumulator += DeltaTime;
	float FuelDrain = FuelConsumptionPerMinute * (FuelAccumulator / 60.0f);
	CurrentFuel = FMath::Max(0.0f, CurrentFuel - FuelDrain);
	FuelAccumulator = 0.0f;

	if (CurrentFuel <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Vehicle '%s' out of fuel — immobilized"),
			UnitData ? *UnitData->DisplayName.ToString() : TEXT("Unknown"));
	}
}
