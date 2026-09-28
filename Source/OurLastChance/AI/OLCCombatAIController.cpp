#include "AI/OLCCombatAIController.h"

#include "Core/OLCUnitData.h"
#include "Kismet/GameplayStatics.h"
#include "World/OLCGroundUnit.h"
#include "World/OLCUnitBase.h"

TArray<AOLCUnitBase*> UOLCCombatAIController::ScanForTargets(AOLCUnitBase* Unit, float SensorRange) const
{
	TArray<AOLCUnitBase*> Targets;
	if (!Unit || !Unit->GetWorld())
	{
		return Targets;
	}

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(Unit->GetWorld(), AOLCUnitBase::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		AOLCUnitBase* Candidate = Cast<AOLCUnitBase>(Actor);
		if (Candidate && Candidate != Unit && !Candidate->IsDead() && FVector::Dist(Unit->GetActorLocation(), Candidate->GetActorLocation()) <= SensorRange)
		{
			Targets.Add(Candidate);
		}
	}
	return Targets;
}

float UOLCCombatAIController::CalculateThreatLevel(AOLCUnitBase* ControlledUnit, AOLCUnitBase* Target, const FOLCThreatWeights& Weights) const
{
	if (!ControlledUnit || !Target || !Target->GetUnitData())
	{
		return 0.0f;
	}
	const UOLCUnitData* Data = Target->GetUnitData();
	const float Distance = FMath::Max(1.0f, FVector::Dist(ControlledUnit->GetActorLocation(), Target->GetActorLocation()));
	float TypeWeight = 1.0f;
	if (Data->UnitType == EOLCUnitType::Vehicle) TypeWeight *= Weights.VehicleWeight;
	if (Data->UnitType == EOLCUnitType::Aerial) TypeWeight *= Weights.AerialWeight;
	if (Data->UnitId.Contains(TEXT("SNIPER"))) TypeWeight *= Weights.SniperWeight;
	return (Data->AttackDamage * Weights.DamageWeight + Data->Range * 0.01f) * TypeWeight * (Weights.DistanceWeight / (Distance / 300.0f));
}

AOLCUnitBase* UOLCCombatAIController::SelectBestTarget(AOLCUnitBase* ControlledUnit, const TArray<AOLCUnitBase*>& Targets, const FOLCThreatWeights& Weights) const
{
	AOLCUnitBase* Best = nullptr;
	float BestScore = -1.0f;
	for (AOLCUnitBase* Target : Targets)
	{
		const float Score = CalculateThreatLevel(ControlledUnit, Target, Weights);
		if (Score > BestScore)
		{
			BestScore = Score;
			Best = Target;
		}
	}
	return Best;
}

EOLCCombatDecision UOLCCombatAIController::EvaluateEngageRetreat(AOLCUnitBase* ControlledUnit, AOLCUnitBase* Target) const
{
	if (!ControlledUnit || !Target)
	{
		return EOLCCombatDecision::Hold;
	}
	const float HealthRatio = ControlledUnit->GetMaxHP() > 0.0f ? ControlledUnit->GetCurrentHP() / ControlledUnit->GetMaxHP() : 1.0f;
	return HealthRatio < 0.2f ? EOLCCombatDecision::Retreat : EOLCCombatDecision::Engage;
}

void UOLCCombatAIController::IssueAttackCommand(AOLCUnitBase* ControlledUnit, AOLCUnitBase* Target) const
{
	if (ControlledUnit && Target)
	{
		ControlledUnit->Attack(Target);
	}
}

void UOLCCombatAIController::IssueRetreatCommand(AOLCUnitBase* ControlledUnit, AOLCUnitBase* Threat, float RetreatDistance) const
{
	if (!ControlledUnit || !Threat)
	{
		return;
	}
	const FVector Direction = (ControlledUnit->GetActorLocation() - Threat->GetActorLocation()).GetSafeNormal2D();
	const FVector Destination = ControlledUnit->GetActorLocation() + Direction * RetreatDistance;
	if (AOLCGroundUnit* Ground = Cast<AOLCGroundUnit>(ControlledUnit))
	{
		Ground->MoveTo(Destination);
	}
	else
	{
		ControlledUnit->SetActorLocation(Destination);
	}
}
