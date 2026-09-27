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

protected:
	virtual void BeginPlay() override;

private:
	/** Current movement speed from UnitData. */
	float CurrentSpeed = 200.0f;
};
