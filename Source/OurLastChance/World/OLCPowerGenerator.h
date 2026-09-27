#pragma once

#include "CoreMinimal.h"
#include "World/OLCBuildingBase.h"
#include "OLCPowerGenerator.generated.h"

/**
 * Power generation buildings (solar arrays, wind turbines, reactors).
 * Feeds power to the grid and tracks total output.
 */
UCLASS()
class OURLASTCHANCE_API AOLCPowerGenerator : public AOLCBuildingBase
{
	GENERATED_BODY()

public:
	AOLCPowerGenerator();

	virtual void Tick(float DeltaTime) override;

	/** Get current power output (negative from data = production). */
	UFUNCTION(BlueprintPure, Category = "OLC|Power")
	float GetCurrentPowerOutput() const { return CurrentPowerOutput; }

protected:
	virtual void BeginPlay() override;
	void SetPowered(bool bNewPowered);

private:
	/** Cached power output from BuildingData (negative = produces). */
	float CurrentPowerOutput = 0.0f;

	/** Timer handle for power production tick. */
	FTimerHandle PowerProductionTimer;

	/** Accumulated delta time toward next power production tick. */
	float PowerAccumulator = 0.0f;

	/** Default production interval in seconds (1 game-day cycle). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Power", meta = (AllowPrivateAccess = "true"))
	float ProductionInterval = 60.0f;
};
