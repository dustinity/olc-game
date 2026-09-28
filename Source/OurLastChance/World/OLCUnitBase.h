#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/OLCUnitData.h"
#include "Combat/OLCCombatTypes.h"
#include "OLCUnitBase.generated.h"

class USkeletalMeshComponent;
class UWidgetComponent;
class UDecalComponent;
class UOLCUnitEquipmentComponent;
struct FDamageEvent;

/**
 * Base actor for all player-controlled units.
 * Provides skeletal mesh, health bar widget, selection highlight,
 * and unit data reference. Category subclasses extend movement/behavior.
 */
UCLASS(Blueprintable)
class OURLASTCHANCE_API AOLCUnitBase : public AActor
{
	GENERATED_BODY()

public:
	AOLCUnitBase();

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** Get the unit data asset. */
	UFUNCTION(BlueprintPure, Category = "OLC|Unit")
	UOLCUnitData* GetUnitData() const { return UnitData; }

	UFUNCTION(BlueprintPure, Category = "OLC|Equipment")
	UOLCUnitEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent; }

	/** Set the unit data asset (called from Blueprint when assigning a DataAsset). */
	UFUNCTION(BlueprintCallable, Category = "OLC|Unit")
	void SetUnitData(UOLCUnitData* InData);

	/** Get current HP. */
	UFUNCTION(BlueprintPure, Category = "OLC|Unit")
	float GetCurrentHP() const { return CurrentHP; }

	/** Get maximum HP. */
	UFUNCTION(BlueprintPure, Category = "OLC|Unit")
	float GetMaxHP() const { return MaxHP; }

	/** Set current HP (clamped to max). */
	UFUNCTION(BlueprintCallable, Category = "OLC|Unit")
	void SetCurrentHP(float NewHP);

	/** Apply damage and trigger death if HP reaches 0. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Unit")
	void ApplyDamage(float DamageAmount);

	/** Route engine/GameplayFramework damage into ApplyDamage. */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** Apply typed combat damage and return the resolved shield/hull result. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	FOLCDamageResult ApplyCombatDamage(const FOLCDamageInput& DamageInput);

	/** Check if this unit is dead. */
	UFUNCTION(BlueprintPure, Category = "OLC|Unit")
	bool IsDead() const { return CurrentHP <= 0.0f; }

	/** Set selection highlight state. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Unit")
	void SetSelected(bool bNewSelected);

	/** Start combat AI scanning (auto-attack nearest enemy). */
	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	void StartCombatAI();

	/** Stop combat AI scanning. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	void StopCombatAI();

	/** Apply this unit's attack damage to a target actor. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	void Attack(AActor* Target);

	/** Activate a numbered tactical ability if its cooldown is ready. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	bool ActivateAbility(int32 AbilityIndex, AActor* Target);

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	float GetAbilityCooldownRemaining(int32 AbilityIndex) const;

	/** Configurable unit data asset - set in constructor or editor. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Unit|Data")
	TObjectPtr<UOLCUnitData> UnitData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit|Components")
	TObjectPtr<UOLCUnitEquipmentComponent> EquipmentComponent;

protected:
	/** Scan for nearest enemy within range and attack if cooldown expired. */
	void CheckForTarget();

	/** Scene root component. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Components")
	TObjectPtr<USceneComponent> SceneRoot;

	/** Skeletal mesh for unit visual. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Components")
	TObjectPtr<USkeletalMeshComponent> Mesh;

	/** Health bar widget component (follows unit in world space). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Components")
	TObjectPtr<UWidgetComponent> HealthBarWidget;

	/** Selection highlight decal ring. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Components")
	TObjectPtr<UDecalComponent> SelectionHighlight;

	/** Current HP (derived from UnitData.MaxHP). */
	UPROPERTY(BlueprintReadWrite, Category = "Unit|Health")
	float MaxHP = 100.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Unit|Health")
	float CurrentHP = 100.0f;

	/** Whether this unit is currently selected by the player. */
	UPROPERTY(BlueprintReadWrite, Category = "Unit|Selection")
	bool bIsSelected = false;

	// -----------------------------------------------------------------------
	// Combat AI state
	// -----------------------------------------------------------------------
	FTimerHandle CombatTimer;

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Combat")
	float CombatCheckInterval = 2.0f; // seconds between enemy scans

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Combat")
	float AttackCooldown = 1.0f; // minimum seconds between attacks

	UPROPERTY(BlueprintReadWrite, Category = "OLC|Combat")
	float LastAttackTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	TArray<float> AbilityCooldowns = { 1.0f, 3.0f, 6.0f, 8.0f, 12.0f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OLC|Combat")
	TArray<float> AbilityReadyTimes;
};
