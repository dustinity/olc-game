#pragma once

#include "CoreMinimal.h"
#include "OLCFormationTypes.generated.h"

UENUM(BlueprintType)
enum class EOLCFormationType : uint8
{
	Line UMETA(DisplayName = "Line"),
	Column UMETA(DisplayName = "Column"),
	Diamond UMETA(DisplayName = "Diamond"),
	VShape UMETA(DisplayName = "V-Shape")
};

USTRUCT(BlueprintType)
struct FOLCFormationConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Formation")
	EOLCFormationType FormationType = EOLCFormationType::Line;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Formation")
	float Spacing = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Formation")
	float TransitionSeconds = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Formation")
	bool bSnapToGrid = true;
};
