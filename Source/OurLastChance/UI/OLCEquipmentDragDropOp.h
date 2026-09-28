#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Core/OLCEquipmentData.h"
#include "OLCEquipmentDragDropOp.generated.h"

UCLASS()
class OURLASTCHANCE_API UOLCEquipmentDragDropOp : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	FOLCEquipmentInstance EquipmentInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	EOLCEquipmentSlotType SourceSlot = EOLCEquipmentSlotType::Primary;
};

