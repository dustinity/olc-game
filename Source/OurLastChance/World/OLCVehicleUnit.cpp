#include "OLCVehicleUnit.h"
#include "OurLastChance.h"

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
		UE_LOG(LogOLC, Log, TEXT("[OLC] Vehicle '%s' fuel consumption: %.1f/min"),
			UnitData ? *UnitData->DisplayName.ToString() : TEXT("Unknown"), FuelConsumptionPerMinute);
	}
}

void AOLCVehicleUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FuelConsumptionPerMinute <= 0.0f || IsDead()) return;
	if (!bIsMoving && !bIsInCombat) return;

	// Drain fuel over time.
	FuelAccumulator += DeltaTime;
	float Multiplier = 1.0f;
	if (bIsMoving) Multiplier += 0.5f;
	if (bIsInCombat) Multiplier += 0.25f;
	float FuelDrain = FuelConsumptionPerMinute * Multiplier * (FuelAccumulator / 60.0f);
	CurrentFuel = FMath::Max(0.0f, CurrentFuel - FuelDrain);
	FuelAccumulator = 0.0f;

	if (!bFuelWarningIssued && CurrentFuel <= 20.0f && CurrentFuel > 0.0f)
	{
		bFuelWarningIssued = true;
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Vehicle '%s' fuel below 20%%"),
			UnitData ? *UnitData->DisplayName.ToString() : TEXT("Unknown"));
	}

	if (CurrentFuel <= 0.0f)
	{
		bIsMoving = false;
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Vehicle '%s' out of fuel — immobilized"),
			UnitData ? *UnitData->DisplayName.ToString() : TEXT("Unknown"));
	}
}

void AOLCVehicleUnit::Refuel(float Amount)
{
	CurrentFuel = FMath::Clamp(CurrentFuel + Amount, 0.0f, 100.0f);
	if (CurrentFuel > 20.0f)
	{
		bFuelWarningIssued = false;
	}
}
