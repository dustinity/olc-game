#pragma once

// WP-129 Step 3: world-target highlight for the active tutorial objective.
// Billboard sprite + pulsing ground ring, sized for top-down RTS visibility
// at ~2000+ units. Visual parameters are read from UOLCTutorialData
// (FOLCTutorialHighlightParams) when a tutorial subsystem is available;
// the EditAnywhere properties below act as fallbacks/overrides.

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "OLCObjectiveHighlightComponent.generated.h"

class UBillboardComponent;
class UMaterialInstanceDynamic;
class UStaticMeshComponent;

UCLASS(ClassGroup = (OLC), meta = (BlueprintSpawnableComponent))
class OURLASTCHANCE_API UOLCObjectiveHighlightComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UOLCObjectiveHighlightComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** World-space radius of the pulsing ground ring (UOLCTutorialData::RingRadius wins when > 0). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial")
	float RingRadius = 500.0f;

	/** Billboard sprite size in world units — sized to stay visible from a top-down camera at ~2000+ units. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial")
	float BillboardSpriteScale = 600.0f;

	/** Billboard hover height above the target origin. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Tutorial")
	float BillboardHeight = 500.0f;

private:
	/** Apply visual parameters from UOLCTutorialData (texture, color, pulse, ring radius). */
	void ApplyVisualParameters();

	/** Pulse phase accumulator (seconds). */
	float PulseTime = 0.0f;

	/** Base ring scale in world units (X/Y); Z stays flat. */
	FVector2D BaseRingScale = FVector2D(500.0f, 500.0f);

	/** Unpulsed billboard sprite scale (world units the sprite should span). */
	float BaseSpriteScale = 600.0f;

	/** Billboard world scale that yields BaseSpriteScale span for the loaded texture. */
	FVector BaseBillboardScale = FVector::OneVector;

	/** Pulse parameters from UOLCTutorialData (Hz / 0..1). */
	float PulseSpeedHz = 1.5f;
	float PulseAmplitude = 0.3f;

	UPROPERTY()
	TObjectPtr<UBillboardComponent> Billboard;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> Ring;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> RingMaterial;
};
