#pragma once

#include "CoreMinimal.h"
#include "World/OLCBuildingBase.h"
#include "OLCDefenseStructure.generated.h"

/**
 * Defense structure buildings (turrets, shield generators, beacon systems).
 * Has HP, attack range, damage, and targeting logic.
 */
UCLASS()
class OURLASTCHANCE_API AOLCDefenseStructure : public AOLCBuildingBase
{
	GENERATED_BODY()

public:
	AOLCDefenseStructure();

	virtual void Tick(float DeltaTime) override;

	/** Get current HP of the defense structure. */
	UFUNCTION(BlueprintPure, Category = "OLC|Defense")
	float GetCurrentHP() const { return CurrentHP; }

	/** Get maximum HP. */
	UFUNCTION(BlueprintPure, Category = "OLC|Defense")
	float GetMaxHP() const { return MaxHP; }

	/** Get attack range in world units. */
	UFUNCTION(BlueprintPure, Category = "OLC|Defense")
	float GetAttackRange() const { return AttackRange; }

	/** Get damage per attack tick. */
	UFUNCTION(BlueprintPure, Category = "OLC|Defense")
	float GetDamagePerTick() const { return DamagePerTick; }

protected:
	virtual void BeginPlay() override;

private:
	/** Current hit points. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Defense", meta = (AllowPrivateAccess = "true"))
	float MaxHP = 500.0f;

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Defense", meta = (AllowPrivateAccess = "true"))
	float CurrentHP = 500.0f;

	/** Attack range in world units. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Defense", meta = (AllowPrivateAccess = "true"))
	float AttackRange = 500.0f;

	/** Damage dealt per attack tick. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Defense", meta = (AllowPrivateAccess = "true"))
	float DamagePerTick = 15.0f;

	/** Timer for attack tick interval. */
	FTimerHandle AttackTimer;

	/** Accumulated time toward next attack. */
	float AttackAccumulator = 0.0f;

	/** Default attack interval in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Defense", meta = (AllowPrivateAccess = "true"))
	float AttackInterval = 1.0f;
};
