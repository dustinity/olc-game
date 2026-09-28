#include "AI/OLCNavMeshPathfinder.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"

FOLCPathResult UOLCNavMeshPathfinder::FindPath(UWorld* World, const FVector& Start, const FVector& Goal, float AgentRadius) const
{
	FOLCPathResult Result;
	if (!World)
	{
		return Result;
	}

	UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(World, Start, Goal);
	if (!Path || !Path->IsValid() || Path->PathPoints.Num() == 0)
	{
		Result.Points = { Start, Goal };
		Result.Cost = FVector::Dist(Start, Goal);
		Result.bValid = true;
		return Result;
	}

	Result.Points = SmoothPath(Path->PathPoints);
	Result.bValid = true;
	for (int32 Index = 1; Index < Result.Points.Num(); ++Index)
	{
		Result.Cost += FVector::Dist(Result.Points[Index - 1], Result.Points[Index]);
	}
	Result.Cost += AgentRadius * 0.01f;
	return Result;
}

TArray<FVector> UOLCNavMeshPathfinder::SmoothPath(const TArray<FVector>& RawPath) const
{
	if (RawPath.Num() <= 2)
	{
		return RawPath;
	}

	TArray<FVector> Smoothed;
	Smoothed.Add(RawPath[0]);
	for (int32 Index = 1; Index < RawPath.Num() - 1; ++Index)
	{
		const FVector A = RawPath[Index] - RawPath[Index - 1];
		const FVector B = RawPath[Index + 1] - RawPath[Index];
		if (!A.GetSafeNormal2D().Equals(B.GetSafeNormal2D(), 0.05f))
		{
			Smoothed.Add(RawPath[Index]);
		}
	}
	Smoothed.Add(RawPath.Last());
	return Smoothed;
}
