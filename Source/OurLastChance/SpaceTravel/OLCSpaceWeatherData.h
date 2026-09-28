#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpaceTravel/OLCSpaceEventData.h"
#include "OLCSpaceWeatherData.generated.h"

/**
 * Space weather types rolled at the start of a trip's Transit phase (WP-122 step 5).
 * None is the sentinel for "clear skies — no weather this trip".
 */
UENUM(BlueprintType)
enum class EOLCSpaceWeatherType : uint8
{
	None          UMETA(DisplayName = "None"),
	SolarFlare    UMETA(DisplayName = "Solar Flare"),
	RadiationBelt UMETA(DisplayName = "Radiation Belt"),
	AsteroidStorm UMETA(DisplayName = "Asteroid Storm"),
	GravityWell   UMETA(DisplayName = "Gravity Well"),
	Nebula        UMETA(DisplayName = "Nebula")
};

/**
 * Multiplies the weight of one space event type in the EventCheck roll for a trip
 * that experienced the owning weather (e.g. AsteroidStorm raises MeteorShower odds).
 */
USTRUCT(BlueprintType)
struct FOLCWeatherEventBias
{
	GENERATED_BODY()

	/** Space event type whose weight is multiplied. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceWeather")
	EOLCSpaceEventType EventType = EOLCSpaceEventType::None;

	/** Multiplier applied to that event's roll weight (1.0 = no bias). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceWeather")
	float WeightMultiplier = 1.0f;
};

/**
 * One row of the space weather table (WP-122 step 5).
 * Weight drives the roll; SpeedMultiplier / SensorPenalty / HullDamagePerSecond /
 * ExtraFuelBurnPerSecond are applied for DurationSeconds during Transit;
 * EventWeightBias conditions the trip's EventCheck roll.
 */
USTRUCT(BlueprintType)
struct FOLCSpaceWeatherEffect
{
	GENERATED_BODY()

	/** Weather type this row defines. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceWeather")
	EOLCSpaceWeatherType WeatherType = EOLCSpaceWeatherType::None;

	/** Relative weight in the weather roll (0 disables the row). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceWeather")
	float Weight = 1.0f;

	/** Transit clock speed factor while active (1.0 = no change; <1 slows the trip down). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceWeather")
	float SpeedMultiplier = 1.0f;

	/** Fraction of sensor/radar effectiveness lost while active (0-1; 1.0 = fully jammed). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceWeather")
	float SensorPenalty = 0.0f;

	/** Hull damage risk in percent per second added to the trip hull total while active. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceWeather")
	float HullDamagePerSecond = 0.0f;

	/** Extra fuel burned per second while active (e.g. fighting a gravity well). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceWeather")
	float ExtraFuelBurnPerSecond = 0.0f;

	/** How long the weather lasts, in seconds of transit. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceWeather")
	float DurationSeconds = 10.0f;

	/** Event roll biases applied for this trip when this weather was rolled. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceWeather")
	TArray<FOLCWeatherEventBias> EventWeightBias;
};

/**
 * Data-driven space weather table (WP-122 step 5).
 *
 * The constructor seeds the five default weather effects, so the class CDO doubles as the
 * code-side fallback table: UOLCTravelSubsystem reads an authored instance when one is
 * assigned and otherwise falls back to GetDefault<UOLCSpaceWeatherData>().
 */
UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCSpaceWeatherData : public UDataAsset
{
	GENERATED_BODY()

public:
	UOLCSpaceWeatherData();

	/** Base chance that a weather condition rolls for a trip (before per-row weights). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceWeather")
	float BaseWeatherChance = 0.30f;

	/** The weather table (looked up by WeatherType, order-independent). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|SpaceWeather")
	TArray<FOLCSpaceWeatherEffect> WeatherTable;

	/** @return pointer into WeatherTable, or nullptr if the type has no row. */
	const FOLCSpaceWeatherEffect* FindEffect(EOLCSpaceWeatherType InType) const;
};
