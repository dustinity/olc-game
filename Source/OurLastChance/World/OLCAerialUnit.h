#pragma once

#include "CoreMinimal.h"
#include "World/OLCUnitBase.h"
#include "Core/OLCUnitData.h"
#include "OLCAerialUnit.generated.h"

/**
 * Aerial unit — ignores terrain collision, has altitude and flight physics.
 * Base class for all flying units (drones, fighters, dropships).
 */
UCLASS()
class OURLASTCHANCE_API AOLCAerialUnit : public AOLCUnitBase
{
	GENERATED_BODY()

public:
	AOLCAerialUnit();

	/** Get current altitude in world units. */
	UFUNCTION(BlueprintPure, Category = "OLC|Aerial")
	float GetAltitude() const { return CurrentAltitude; }

	/** Set target altitude. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Aerial")
	void SetTargetAltitude(float NewAltitude);

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:
	/** Current flight altitude above ground. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Aerial", meta = (AllowPrivateAccess = "true"))
	float CurrentAltitude = 500.0f;

	/** Target altitude for smooth transitions. */
	float TargetAltitude = 500.0f;

	/** Altitude change speed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Aerial", meta = (AllowPrivateAccess = "true"))
	float AltitudeSpeed = 100.0f;
};
