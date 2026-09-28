#pragma once

#include "CoreMinimal.h"
#include "World/OLCUnitBase.h"
#include "Core/OLCUnitData.h"
#include "OLCAerialUnit.generated.h"

UENUM(BlueprintType)
enum class EOLCAltitudeBand : uint8
{
	Low UMETA(DisplayName = "Low"),
	Medium UMETA(DisplayName = "Medium"),
	High UMETA(DisplayName = "High")
};

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

	UFUNCTION(BlueprintCallable, Category = "OLC|Aerial")
	void SetAltitudeBand(EOLCAltitudeBand Band);

	UFUNCTION(BlueprintPure, Category = "OLC|Aerial")
	float GetSpeedMultiplier() const;

	UFUNCTION(BlueprintPure, Category = "OLC|Aerial")
	float GetDetectionProfile() const;

	UFUNCTION(BlueprintCallable, Category = "OLC|Aerial")
	AOLCUnitBase* DeployUnit(TSubclassOf<AOLCUnitBase> UnitClass);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OLC|Aerial", meta = (AllowPrivateAccess = "true"))
	EOLCAltitudeBand AltitudeBand = EOLCAltitudeBand::Medium;
};
