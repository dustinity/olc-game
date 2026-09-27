#include "OLCGroundUnit.h"

#include "Core/OLCUnitData.h"
#include "Logging/LogMacros.h"

AOLCGroundUnit::AOLCGroundUnit()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOLCGroundUnit::BeginPlay()
{
	Super::BeginPlay();

	if (UnitData)
	{
		CurrentSpeed = UnitData->MovementSpeed;
	}

	UE_LOG(LogTemp, Log, TEXT("[OLC] Ground unit '%s' online (speed=%.0f)"),
		UnitData ? *UnitData->DisplayName.ToString() : TEXT("Unknown"), CurrentSpeed);
}

void AOLCGroundUnit::MoveTo(const FVector& TargetLocation)
{
	if (IsDead()) return;

	SetActorLocation(TargetLocation);
	UE_LOG(LogTemp, Verbose, TEXT("[OLC] Ground unit '%s' moved to %s"),
		UnitData ? *UnitData->DisplayName.ToString() : TEXT("Unknown"), *TargetLocation.ToString());
}
