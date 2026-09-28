#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/OLCEquipmentData.h"
#include "OLCEquipmentSubsystem.generated.h"

UCLASS()
class OURLASTCHANCE_API UOLCEquipmentSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "OLC|Equipment")
	void InitializeFactionDefaults(const FString& FactionId, int32 ColonyTIR);

	UFUNCTION(BlueprintCallable, Category = "OLC|Equipment")
	bool AssignFromPool(EOLCEquipmentSlotType SlotType, EOLCUnitType UnitType, const FString& FactionId, int32 ColonyTIR, FOLCEquipmentInstance& OutInstance);

	UFUNCTION(BlueprintCallable, Category = "OLC|Equipment")
	void AddToPool(UOLCEquipmentData* EquipmentData, int32 Count = 1);

	UFUNCTION(BlueprintCallable, Category = "OLC|Equipment")
	void ReturnToPool(const FOLCEquipmentInstance& Instance);

	UFUNCTION(BlueprintCallable, Category = "OLC|Equipment")
	FOLCEquipmentInstance AddUniqueLoot(UOLCEquipmentData* EquipmentData, const FText& UniqueName);

	UFUNCTION(BlueprintPure, Category = "OLC|Equipment")
	const TArray<FOLCEquipmentInstance>& GetEquipmentPool() const { return EquipmentPool; }

	UFUNCTION(BlueprintPure, Category = "OLC|Equipment")
	const TArray<FOLCEquipmentInstance>& GetUniqueLoot() const { return UniqueLoot; }

	UFUNCTION(BlueprintPure, Category = "OLC|Equipment")
	TArray<FOLCEquipmentInstance> GetAvailableForSlot(EOLCEquipmentSlotType SlotType, EOLCUnitType UnitType, const FString& FactionId, int32 ColonyTIR) const;

	UFUNCTION(BlueprintCallable, Category = "OLC|Equipment")
	TArray<FOLCEquipmentInstance> GetFactionDefaultsForUnit(EOLCUnitType UnitType, const FString& FactionId, int32 ColonyTIR) const;

	UFUNCTION(BlueprintCallable, Category = "OLC|Equipment")
	int32 AssignBestToUnits(const TArray<AActor*>& Units, EOLCEquipmentSlotType SlotType, const FString& FactionId, int32 ColonyTIR);

private:
	UPROPERTY()
	TArray<FOLCEquipmentInstance> EquipmentPool;

	UPROPERTY()
	TArray<FOLCEquipmentInstance> FactionDefaults;

	UPROPERTY()
	TArray<FOLCEquipmentInstance> UniqueLoot;
};

