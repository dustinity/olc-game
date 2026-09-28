#include "OLCGroundUnit.h"

#include "AI/OLCNavMeshPathfinder.h"
#include "Core/OLCUnitData.h"
#include "Logging/LogMacros.h"

#include "OurLastChance.h"
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

	UE_LOG(LogOLC, Log, TEXT("[OLC] Ground unit '%s' online (speed=%.0f)"),
		UnitData ? *UnitData->DisplayName.ToString() : TEXT("Unknown"), CurrentSpeed);
}

void AOLCGroundUnit::MoveTo(const FVector& TargetLocation)
{
	if (IsDead()) return;

	UOLCNavMeshPathfinder* Pathfinder = NewObject<UOLCNavMeshPathfinder>(this);
	const float AgentRadius = UnitData ? FMath::Max(UnitData->GridSize.X, UnitData->GridSize.Y) * 45.0f : 45.0f;
	const FOLCPathResult Path = Pathfinder->FindPath(GetWorld(), GetActorLocation(), TargetLocation, AgentRadius);
	CurrentPath = Path.Points;
	PathIndex = CurrentPath.Num() > 1 ? 1 : 0;
	UE_LOG(LogOLC, Verbose, TEXT("[OLC] Ground unit '%s' moved to %s"),
		UnitData ? *UnitData->DisplayName.ToString() : TEXT("Unknown"), *TargetLocation.ToString());
}

void AOLCGroundUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead() || !CurrentPath.IsValidIndex(PathIndex))
	{
		return;
	}

	const FVector Current = GetActorLocation();
	const FVector Target = CurrentPath[PathIndex];
	const FVector Next = FMath::VInterpConstantTo(Current, Target, DeltaTime, CurrentSpeed);
	SetActorLocation(Next);
	if (FVector::DistSquared2D(Next, Target) <= FMath::Square(12.0f))
	{
		PathIndex++;
		if (!CurrentPath.IsValidIndex(PathIndex))
		{
			CurrentPath.Reset();
			PathIndex = 0;
		}
	}
}
