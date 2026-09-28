#include "Combat/OLCTargetingSystem.h"

UOLCTargetingSystem::UOLCTargetingSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UOLCTargetingSystem::SetTargetingMode(EOLCTargetingMode InMode)
{
	CurrentTargetingMode = InMode;
}

AActor* UOLCTargetingSystem::SelectTarget(const TArray<AActor*>& Candidates, const FVector& ShooterLocation) const
{
	switch (CurrentTargetingMode)
	{
		case EOLCTargetingMode::Focus:
			return GetHighestThreatTarget(Candidates, ShooterLocation);
		case EOLCTargetingMode::Spread:
		{
			const TArray<AActor*> SpreadTargets = SelectSpreadTargets(Candidates, ShooterLocation, 1);
			return SpreadTargets.Num() > 0 ? SpreadTargets[0] : nullptr;
		}
		case EOLCTargetingMode::Pulse:
			return GetShieldPriorityTarget(Candidates, ShooterLocation);
		case EOLCTargetingMode::Auto:
		default:
			return GetNearestTarget(Candidates, ShooterLocation);
	}
}

TArray<AActor*> UOLCTargetingSystem::SelectSpreadTargets(const TArray<AActor*>& Candidates, const FVector& ShooterLocation, int32 MaxTargets) const
{
	TArray<AActor*> ValidTargets;
	for (AActor* Candidate : Candidates)
	{
		if (IsValid(Candidate))
		{
			ValidTargets.Add(Candidate);
		}
	}

	ValidTargets.Sort([ShooterLocation](const AActor& A, const AActor& B)
	{
		return FVector::DistSquared(ShooterLocation, A.GetActorLocation()) < FVector::DistSquared(ShooterLocation, B.GetActorLocation());
	});

	if (ValidTargets.Num() > MaxTargets)
	{
		ValidTargets.SetNum(FMath::Max(1, MaxTargets));
	}
	return ValidTargets;
}

AActor* UOLCTargetingSystem::GetNearestTarget(const TArray<AActor*>& Candidates, const FVector& ShooterLocation) const
{
	AActor* BestTarget = nullptr;
	float BestDistanceSq = TNumericLimits<float>::Max();
	for (AActor* Candidate : Candidates)
	{
		if (!IsValid(Candidate))
		{
			continue;
		}

		const float DistanceSq = FVector::DistSquared(ShooterLocation, Candidate->GetActorLocation());
		if (DistanceSq < BestDistanceSq)
		{
			BestDistanceSq = DistanceSq;
			BestTarget = Candidate;
		}
	}
	return BestTarget;
}

AActor* UOLCTargetingSystem::GetHighestThreatTarget(const TArray<AActor*>& Candidates, const FVector& ShooterLocation) const
{
	AActor* BestTarget = nullptr;
	float BestThreat = -1.0f;
	for (AActor* Candidate : Candidates)
	{
		if (!IsValid(Candidate))
		{
			continue;
		}

		const float Threat = EstimateThreat(Candidate, ShooterLocation);
		if (Threat > BestThreat)
		{
			BestThreat = Threat;
			BestTarget = Candidate;
		}
	}
	return BestTarget;
}

AActor* UOLCTargetingSystem::GetShieldPriorityTarget(const TArray<AActor*>& Candidates, const FVector& ShooterLocation) const
{
	TArray<AActor*> Shielded;
	for (AActor* Candidate : Candidates)
	{
		if (IsValid(Candidate) && LooksShielded(Candidate))
		{
			Shielded.Add(Candidate);
		}
	}
	return Shielded.Num() > 0 ? GetNearestTarget(Shielded, ShooterLocation) : GetNearestTarget(Candidates, ShooterLocation);
}

float UOLCTargetingSystem::EstimateThreat(AActor* Candidate, const FVector& ShooterLocation) const
{
	const float Distance = FVector::Dist(ShooterLocation, Candidate->GetActorLocation());
	float Threat = FMath::Max(1.0f, 10000.0f / FMath::Max(1.0f, Distance));
	const FString Name = Candidate->GetName();
	if (Name.Contains(TEXT("Boss")) || Name.Contains(TEXT("Champion")) || Name.Contains(TEXT("Heavy")))
	{
		Threat += 100.0f;
	}
	if (Name.Contains(TEXT("Shield")))
	{
		Threat += 25.0f;
	}
	return Threat;
}

bool UOLCTargetingSystem::LooksShielded(AActor* Candidate) const
{
	if (!Candidate)
	{
		return false;
	}
	const FString Name = Candidate->GetName();
	return Name.Contains(TEXT("Shield")) || Name.Contains(TEXT("Barrier")) || Name.Contains(TEXT("Dome"));
}

