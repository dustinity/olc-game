#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/OLCWeaponData.h"
#include "OLCTargetingSystem.generated.h"

UCLASS(ClassGroup=(OLC), meta=(BlueprintSpawnableComponent))
class OURLASTCHANCE_API UOLCTargetingSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UOLCTargetingSystem();

	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	void SetTargetingMode(EOLCTargetingMode InMode);

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	AActor* SelectTarget(const TArray<AActor*>& Candidates, const FVector& ShooterLocation) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	TArray<AActor*> SelectSpreadTargets(const TArray<AActor*>& Candidates, const FVector& ShooterLocation, int32 MaxTargets) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	EOLCTargetingMode CurrentTargetingMode = EOLCTargetingMode::Auto;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	bool bAutoTargeting = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	float PulseShieldBonus = 1.25f;

private:
	AActor* GetNearestTarget(const TArray<AActor*>& Candidates, const FVector& ShooterLocation) const;
	AActor* GetHighestThreatTarget(const TArray<AActor*>& Candidates, const FVector& ShooterLocation) const;
	AActor* GetShieldPriorityTarget(const TArray<AActor*>& Candidates, const FVector& ShooterLocation) const;
	float EstimateThreat(AActor* Candidate, const FVector& ShooterLocation) const;
	bool LooksShielded(AActor* Candidate) const;
};

