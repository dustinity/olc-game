#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/OLCEquipmentData.h"
#include "OLCEquipmentAssignmentWidget.generated.h"

class AOLCUnitBase;

UCLASS()
class OURLASTCHANCE_API UOLCEquipmentAssignmentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "OLC|Equipment")
	void InitializeForUnit(AOLCUnitBase* InUnit, const FString& InFactionId, int32 InColonyTIR);

	UFUNCTION(BlueprintCallable, Category = "OLC|Equipment")
	bool AssignEquipmentToSlot(const FOLCEquipmentInstance& Instance, EOLCEquipmentSlotType SlotType);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	TSharedRef<SWidget> BuildSlot(EOLCEquipmentSlotType SlotType);
	TSharedRef<SWidget> BuildInventory();
	FText GetSlotLabel(EOLCEquipmentSlotType SlotType) const;

	UPROPERTY(Transient)
	TObjectPtr<AOLCUnitBase> TargetUnit = nullptr;

	FString FactionId = TEXT("any");
	int32 ColonyTIR = 1;
};

