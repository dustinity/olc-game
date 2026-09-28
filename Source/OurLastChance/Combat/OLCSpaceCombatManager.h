#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Combat/OLCCombatTypes.h"
#include "OLCSpaceCombatManager.generated.h"

class UOLCCombatStateComponent;
class UOLCWeaponMountComponent;

UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCSpaceCombatManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	void StartSpaceCombat(AActor* PlayerShip, AActor* EnemyShip);

	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	void AdvanceSpaceCombat();

	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	void ExchangeFire();

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	EOLCCombatPhase GetPhase() const { return CurrentPhase; }

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Combat")
	float PlayerShield = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Combat")
	float PlayerHull = 150.0f;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Combat")
	float EnemyShield = 80.0f;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Combat")
	float EnemyHull = 120.0f;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Combat")
	bool bPlayerVictory = false;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Combat")
	bool bResolved = false;

private:
	void ApplyShipFire(AActor* Shooter, AActor* Target, float& TargetShield, float& TargetHull);

	UPROPERTY()
	TObjectPtr<AActor> PlayerShipActor;

	UPROPERTY()
	TObjectPtr<AActor> EnemyShipActor;

	EOLCCombatPhase CurrentPhase = EOLCCombatPhase::Engage;
};
