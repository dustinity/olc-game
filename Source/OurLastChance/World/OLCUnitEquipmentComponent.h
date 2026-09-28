#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/OLCEquipmentData.h"
#include "OLCUnitEquipmentComponent.generated.h"

UCLASS(ClassGroup=(OLC), meta=(BlueprintSpawnableComponent))
class OURLASTCHANCE_API UOLCUnitEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOLCUnitEquipmentComponent();

	UFUNCTION(BlueprintCallable, Category = "OLC|Equipment")
	void InitializeDefaultSlots(EOLCUnitType UnitType, bool bChampion);

	UFUNCTION(BlueprintCallable, Category = "OLC|Equipment")
	bool AssignEquipment(const FOLCEquipmentInstance& Instance, EOLCEquipmentSlotType SlotType, const FString& FactionId, int32 ColonyTIR);

	UFUNCTION(BlueprintCallable, Category = "OLC|Equipment")
	bool RemoveEquipment(EOLCEquipmentSlotType SlotType, bool bReturnToPool = true);

	UFUNCTION(BlueprintCallable, Category = "OLC|Equipment")
	void OnUnitDeath();

	UFUNCTION(BlueprintPure, Category = "OLC|Equipment")
	const TMap<EOLCEquipmentSlotType, FOLCEquipmentInstance>& GetEquipmentSlots() const { return EquipmentSlots; }

	UFUNCTION(BlueprintPure, Category = "OLC|Equipment")
	FOLCEquipmentStats GetTotalEquipmentStats() const;

private:
	UPROPERTY()
	TMap<EOLCEquipmentSlotType, FOLCEquipmentInstance> EquipmentSlots;
};

