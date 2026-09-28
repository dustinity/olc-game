#include "SpaceTravel/OLCSpaceWeatherData.h"

UOLCSpaceWeatherData::UOLCSpaceWeatherData()
{
	// Five default space weather effects (WP-122 step 7 canon). These constructor defaults
	// seed the class CDO, which UOLCTravelSubsystem uses as its code-side fallback when no
	// authored DataAsset instance is assigned.

	// Solar flare — reduced visibility and flickering systems: sensor penalty + slight slowdown.
	{
		FOLCSpaceWeatherEffect Effect;
		Effect.WeatherType = EOLCSpaceWeatherType::SolarFlare;
		Effect.Weight = 1.0f;
		Effect.SpeedMultiplier = 0.95f;
		Effect.SensorPenalty = 0.4f;
		Effect.DurationSeconds = 12.0f;
		WeatherTable.Add(Effect);
	}

	// Radiation belt — hull takes slow damage over time while crossing it.
	{
		FOLCSpaceWeatherEffect Effect;
		Effect.WeatherType = EOLCSpaceWeatherType::RadiationBelt;
		Effect.Weight = 1.0f;
		Effect.SpeedMultiplier = 1.0f;
		Effect.SensorPenalty = 0.1f;
		Effect.HullDamagePerSecond = 0.5f;
		Effect.DurationSeconds = 15.0f;
		WeatherTable.Add(Effect);
	}

	// Asteroid storm — continuous asteroid threats: slower, riskier, and it raises the
	// MeteorShower event weight for this trip's EventCheck roll.
	{
		FOLCSpaceWeatherEffect Effect;
		Effect.WeatherType = EOLCSpaceWeatherType::AsteroidStorm;
		Effect.Weight = 1.2f;
		Effect.SpeedMultiplier = 0.9f;
		Effect.SensorPenalty = 0.3f;
		Effect.HullDamagePerSecond = 0.2f;
		Effect.DurationSeconds = 20.0f;
		FOLCWeatherEventBias Bias;
		Bias.EventType = EOLCSpaceEventType::MeteorShower;
		Bias.WeightMultiplier = 3.0f;
		Effect.EventWeightBias.Add(Bias);
		WeatherTable.Add(Effect);
	}

	// Gravity well — ship pulled off course: noticeably slower, extra fuel burned to fight it.
	{
		FOLCSpaceWeatherEffect Effect;
		Effect.WeatherType = EOLCSpaceWeatherType::GravityWell;
		Effect.Weight = 0.8f;
		Effect.SpeedMultiplier = 0.75f;
		Effect.SensorPenalty = 0.2f;
		Effect.HullDamagePerSecond = 0.1f;
		Effect.ExtraFuelBurnPerSecond = 0.3f;
		Effect.DurationSeconds = 18.0f;
		WeatherTable.Add(Effect);
	}

	// Nebula — sensors jammed until cleared (full sensor penalty), long drift through it.
	{
		FOLCSpaceWeatherEffect Effect;
		Effect.WeatherType = EOLCSpaceWeatherType::Nebula;
		Effect.Weight = 1.0f;
		Effect.SpeedMultiplier = 1.0f;
		Effect.SensorPenalty = 1.0f;
		Effect.DurationSeconds = 25.0f;
		WeatherTable.Add(Effect);
	}
}

const FOLCSpaceWeatherEffect* UOLCSpaceWeatherData::FindEffect(EOLCSpaceWeatherType InType) const
{
	for (const FOLCSpaceWeatherEffect& Effect : WeatherTable)
	{
		if (Effect.WeatherType == InType)
		{
			return &Effect;
		}
	}
	return nullptr;
}
