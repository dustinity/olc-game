// Copyright OLC Project. WP-128 Step 3 — state-slot animation player component.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Core/OLCUnitAnimationData.h"
#include "Core/OLCChampionAnimationData.h"
#include "OLCAnimationPlayerComponent.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UMaterialInstanceDynamic;
struct FOLCAnimationEntry;

/**
 * State-slot animation player for the WP-128 animation system (step 3).
 *
 * Holds a UOLCUnitAnimationSet and plays the binding for the active slot:
 *  - Asset-backed: UAnimSequence on the owning actor's skeletal mesh, or a
 *    material on the primary static mesh (sprite-style visuals), honoring
 *    bLoop/PlayRate.
 *  - Procedural fallback (defined for all 17 EOLCAnimSlot values): transform
 *    motion applied to the owner's visual component so movement/pathfinding
 *    is never disturbed.
 *
 * The component ticks at the owning actor's tick, blends between slots over
 * BlendTime, and cleans up on EndPlay (relative transforms, visibility,
 * material instances). Root motion is intentionally not consumed: this is a
 * top-down RTS where units stay in place while animating.
 */
UCLASS(ClassGroup = (OLC), meta = (BlueprintSpawnableComponent))
class OURLASTCHANCE_API UOLCAnimationPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOLCAnimationPlayerComponent();

	/** Animation set for this unit. Null = every slot uses its procedural fallback. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OLC|Animation")
	TObjectPtr<UOLCUnitAnimationSet> AnimationSet;

	/** Slot applied in BeginPlay (also serves as the PIE/debug entry point). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Animation")
	EOLCAnimSlot InitialSlot = EOLCAnimSlot::Idle;

	/**
	 * Switch the active animation slot.
	 * Idempotent: re-setting the same settled slot is a no-op (no restart flicker).
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Animation")
	void SetSlot(EOLCAnimSlot NewSlot, float BlendTime = 0.2f);

	/** Currently active slot. */
	UFUNCTION(BlueprintPure, Category = "OLC|Animation")
	EOLCAnimSlot GetCurrentSlot() const { return CurrentSlot; }

	/**
	 * Play a champion ability animation on Target (static helper for step 6 wiring).
	 * Plays Anim.AnimationAsset when set (UAnimSequence on a skeletal-mesh owner,
	 * material on a static-mesh owner); otherwise spawns a VFX flash through
	 * UOLCVFXSubsystem: an ability-keyed environmental system when one is
	 * registered for the AbilityId in DA_VFX, else a deterministic combat flash
	 * keyed off the ability id.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Champion")
	static void PlayAbilityAnimation(AActor* Target, const FOLCChampionAbilityAnim& Anim);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// --- Slot / blend state ---------------------------------------------------
	EOLCAnimSlot CurrentSlot = EOLCAnimSlot::Idle;
	float BlendDuration = 0.0f;
	float BlendTimeRemaining = 0.0f;
	float SlotClock = 0.0f;          // time spent in the current slot
	float AttackPulseClock = 0.0f;   // repeating recoil pulse clock (Attack/Fire)
	bool bAssetBackedActive = false; // true when an asset (not procedural) drives the slot
	bool bSlotHidden = false;        // Crash/Collapse hide-on-completion

	// Transform offsets applied to the visual component (relative to base).
	FVector BlendStartLocation = FVector::ZeroVector;
	FRotator BlendStartRotation = FRotator::ZeroRotator;
	FVector BlendStartScale = FVector::OneVector;
	FVector AppliedLocationOffset = FVector::ZeroVector;
	FRotator AppliedRotationOffset = FRotator::ZeroRotator;
	FVector AppliedScale = FVector::OneVector;

	// --- Visual target ----------------------------------------------------------
	UPROPERTY(Transient)
	TObjectPtr<USceneComponent> VisualComponent;
	FTransform BaseVisualTransform;

	void ResolveVisualComponent();
	UStaticMeshComponent* FindPrimaryStaticMesh() const;

	// --- Asset-backed playback ----------------------------------------------------
	bool TryPlayAssetBacked(const FOLCAnimationEntry& Entry) const;

	// --- Procedural fallbacks ------------------------------------------------------
	/** Target transform offset for the current slot at SlotClock. */
	void ComputeSlotOffset(FVector& OutLocation, FRotator& OutRotation, FVector& OutScale) const;
	static float EaseOutCubic(float X);
	static float EaseInCubic(float X);
	static float EaseInQuad(float X);

	// --- Emissive pulse (Heal green / Damage red) — resource-marker MID pattern -----
	void SetupPulseMaterial(EOLCAnimSlot Slot);
	void ClearPulseMaterial();
	void UpdatePulseMaterial(float DeltaTime);

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> PulseMID;
	UMaterialInterface* PulseBaseMaterial = nullptr; // material to restore on cleanup
	int32 PulseMaterialSlot = 0;
	float PulsePhase = 0.0f;
	float PulseFrequencyHz = 2.0f;
};
