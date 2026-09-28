#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/OLCCombatTypes.h"
#include "OLCCombatStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOLCCombatPhaseChanged, EOLCCombatPhase, PreviousPhase, EOLCCombatPhase, NewPhase);

UCLASS(ClassGroup=(OLC), meta=(BlueprintSpawnableComponent))
class OURLASTCHANCE_API UOLCCombatStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOLCCombatStateComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "OLC|Combat")
	FOnOLCCombatPhaseChanged OnPhaseChanged;

	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	void StartCombat();

	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	void AdvancePhase();

	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	void SetPaused(bool bNewPaused);

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	EOLCCombatPhase GetCurrentPhase() const { return CurrentPhase; }

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	float GetPhaseTime() const { return PhaseTime; }

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	bool IsPaused() const { return bPaused; }

private:
	void SetPhase(EOLCCombatPhase NewPhase);

	UPROPERTY(VisibleAnywhere, Category = "OLC|Combat")
	EOLCCombatPhase CurrentPhase = EOLCCombatPhase::Engage;

	UPROPERTY(VisibleAnywhere, Category = "OLC|Combat")
	float PhaseTime = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "OLC|Combat")
	bool bPaused = false;
};
