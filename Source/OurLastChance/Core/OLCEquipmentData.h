#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Combat/OLCWeaponData.h"
#include "Core/OLCResourceTypes.h"
#include "Core/OLCUnitData.h"
#include "OLCEquipmentData.generated.h"

UENUM(BlueprintType)
enum class EOLCEquipmentType : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Armor UMETA(DisplayName = "Armor"),
	Utility UMETA(DisplayName = "Utility")
};

UENUM(BlueprintType)
enum class EOLCEquipmentSlotType : uint8
{
	Primary UMETA(DisplayName = "Primary"),
	Secondary UMETA(DisplayName = "Secondary"),
	Tertiary UMETA(DisplayName = "Tertiary"),
	Armor UMETA(DisplayName = "Armor"),
	Utility UMETA(DisplayName = "Utility")
};

UENUM(BlueprintType)
enum class EOLCEquipmentRarity : uint8
{
	Common UMETA(DisplayName = "Common"),
	Uncommon UMETA(DisplayName = "Uncommon"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary"),
	Alien UMETA(DisplayName = "Alien")
};

UENUM(BlueprintType)
enum class EOLCEquipmentLayer : uint8
{
	FactionDefault UMETA(DisplayName = "Faction Default"),
	Pool UMETA(DisplayName = "Pool"),
	Unique UMETA(DisplayName = "Unique")
};

USTRUCT(BlueprintType)
struct FOLCEquipmentStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	float DamageBonus = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	float HPBonus = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	float SpeedModifier = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	float ShieldBonus = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	float UtilityBonus = 0.0f;
};

USTRUCT(BlueprintType)
struct FOLCEquipmentInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	FGuid InstanceId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	TObjectPtr<class UOLCEquipmentData> EquipmentData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	EOLCEquipmentLayer Layer = EOLCEquipmentLayer::Pool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	bool bUniqueNamedItem = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	FText CustomName;

	FOLCEquipmentInstance()
		: InstanceId(FGuid::NewGuid())
	{
	}
};

USTRUCT(BlueprintType)
struct FOLCChampionLoadout
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	FName LoadoutId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	TArray<FOLCEquipmentInstance> Items;
};

UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCEquipmentData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	FName EquipmentId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	EOLCEquipmentType EquipmentType = EOLCEquipmentType::Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	EOLCEquipmentSlotType SlotType = EOLCEquipmentSlotType::Primary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	EOLCEquipmentRarity Rarity = EOLCEquipmentRarity::Common;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	FOLCEquipmentStats Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	TArray<FText> SpecialEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	FString FactionRestriction = TEXT("any");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	int32 RequiresTIR = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	TObjectPtr<UOLCWeaponData> WeaponReference = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	TArray<EOLCUnitType> AllowedUnitTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	bool bUniqueTemplate = false;

	UFUNCTION(BlueprintPure, Category = "OLC|Equipment")
	bool IsAvailableForFaction(const FString& FactionId) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Equipment")
	bool CanEquip(EOLCEquipmentSlotType TargetSlot, EOLCUnitType UnitType, const FString& FactionId, int32 ColonyTIR, FText& OutReason) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Equipment")
	FLinearColor GetRarityColor() const;

	UFUNCTION(BlueprintCallable, Category = "OLC|Equipment")
	static TArray<UOLCEquipmentData*> GetDefaultEquipmentCatalog(UObject* Outer);

	UFUNCTION(BlueprintCallable, Category = "OLC|Equipment")
	static TArray<UOLCEquipmentData*> GetEquipmentBySlot(UObject* Outer, EOLCEquipmentSlotType InSlotType);

	UFUNCTION(BlueprintCallable, Category = "OLC|Equipment")
	static TArray<UOLCEquipmentData*> GetEquipmentByRarity(UObject* Outer, EOLCEquipmentRarity InRarity);
};

UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCEquipmentPoolData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	FText PoolName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	FString FactionId = TEXT("any");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Equipment")
	TArray<TObjectPtr<UOLCEquipmentData>> AvailableItems;

	UFUNCTION(BlueprintPure, Category = "OLC|Equipment")
	UOLCEquipmentData* GetRandomItem() const;
};
