#pragma once

#include "CoreMinimal.h"
#include "World/OLCUnitBase.h"
#include "Core/OLCUnitData.h"
#include "OLCVehicleUnit.generated.h"

/**
 * Vehicle unit — tracks fuel consumption, larger collision footprint.
 * Base class for LightVehicle and HeavyVehicle units.
 */
UCLASS()
class OURLASTCHANCE_API AOLCVehicleUnit : public AOLCUnitBase
{
	GENERATED_BODY()

public:
	AOLCVehicleUnit();

	/** Get current fuel level (0 = out of fuel, cannot move). */
	UFUNCTION(BlueprintPure, Category = "OLC|Vehicle")
	float GetCurrentFuel() const { return CurrentFuel; }

	/** Check if vehicle has enough fuel to move. */
	UFUNCTION(BlueprintPure, Category = "OLC|Vehicle")
	bool HasFuel() const { return CurrentFuel > 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "OLC|Vehicle")
	void SetMoving(bool bNewMoving) { bIsMoving = bNewMoving; }

	UFUNCTION(BlueprintCallable, Category = "OLC|Vehicle")
	void SetInCombat(bool bNewInCombat) { bIsInCombat = bNewInCombat; }

	UFUNCTION(BlueprintCallable, Category = "OLC|Vehicle")
	void Refuel(float Amount);

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:
	/** Current fuel level (starts at 100). */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Vehicle", meta = (AllowPrivateAccess = "true"))
	float CurrentFuel = 100.0f;

	/** Fuel consumption per minute from UnitData. */
	float FuelConsumptionPerMinute = 0.0f;

	/** Accumulated time for fuel drain. */
	float FuelAccumulator = 0.0f;

	bool bFuelWarningIssued = false;
	bool bIsMoving = false;
	bool bIsInCombat = false;
};
