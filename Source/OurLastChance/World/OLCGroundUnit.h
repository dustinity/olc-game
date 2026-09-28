#pragma once

#include "CoreMinimal.h"
#include "World/OLCUnitBase.h"
#include "Core/OLCUnitData.h"
#include "OLCGroundUnit.generated.h"

/**
 * Ground unit — standard movement, A* pathfinding on navmesh.
 * Base class for Infantry and LightVehicle units.
 */
UCLASS()
class OURLASTCHANCE_API AOLCGroundUnit : public AOLCUnitBase
{
	GENERATED_BODY()

public:
	AOLCGroundUnit();

	/** Move to a world location (A* pathfinding). */
	UFUNCTION(BlueprintCallable, Category = "OLC|Ground")
	void MoveTo(const FVector& TargetLocation);

	UFUNCTION(BlueprintPure, Category = "OLC|Ground")
	bool IsMoving() const { return CurrentPath.Num() > 0; }

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

protected:
	/** Current movement speed from UnitData. */
	float CurrentSpeed = 200.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OLC|Ground")
	TArray<FVector> CurrentPath;

	int32 PathIndex = 0;
};
