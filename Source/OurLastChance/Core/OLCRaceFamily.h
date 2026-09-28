#pragma once

#include "CoreMinimal.h"

#include "OLCRaceFamily.generated.h"

UENUM(BlueprintType)
enum class EOLCRaceFamily : uint8
{
	Insectoid   UMETA(DisplayName = "Insectoid"),
	Reptilian   UMETA(DisplayName = "Reptilian"),
	Molluskoid  UMETA(DisplayName = "Molluskoid"),
	Humanoid    UMETA(DisplayName = "Humanoid"),
	Crystalloid UMETA(DisplayName = "Crystalloid"),
	Neutral     UMETA(DisplayName = "Neutral"),
	Unknown     UMETA(DisplayName = "Unknown")
};
