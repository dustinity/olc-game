#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OLCCombatTriggerActor.generated.h"

class UBoxComponent;
class UOLCTacticalCombatWidget;
class UOLCCombatResultsWidget;

UCLASS()
class OURLASTCHANCE_API AOLCCombatTriggerActor : public AActor
{
	GENERATED_BODY()

public:
	AOLCCombatTriggerActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCombatEnded(bool bVictory);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OLC|Combat")
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OLC|Combat")
	TSubclassOf<UOLCTacticalCombatWidget> TacticalCombatWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OLC|Combat")
	TSubclassOf<UOLCCombatResultsWidget> CombatResultsWidgetClass;

	UPROPERTY()
	TObjectPtr<UOLCTacticalCombatWidget> ActiveCombatWidget;

	bool bTriggered = false;
};
