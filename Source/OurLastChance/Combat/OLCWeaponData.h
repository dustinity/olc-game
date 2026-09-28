#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Combat/OLCCombatTypes.h"
#include "Core/OLCResourceTypes.h"
#include "OLCWeaponData.generated.h"

UENUM(BlueprintType)
enum class EOLCWeaponDamageType : uint8
{
	Ballistic UMETA(DisplayName = "Ballistic"),
	Energy UMETA(DisplayName = "Energy"),
	Rocket UMETA(DisplayName = "Rocket"),
	Ion UMETA(DisplayName = "Ion"),
	Void UMETA(DisplayName = "Void")
};

UENUM(BlueprintType)
enum class EOLCWeaponAmmoType : uint8
{
	Infinite UMETA(DisplayName = "Infinite"),
	Burst UMETA(DisplayName = "Burst"),
	Limited UMETA(DisplayName = "Limited")
};

UENUM(BlueprintType)
enum class EOLCTurretMountType : uint8
{
	Light UMETA(DisplayName = "Light"),
	Heavy UMETA(DisplayName = "Heavy"),
	Dual UMETA(DisplayName = "Dual"),
	Quad UMETA(DisplayName = "Quad")
};

UENUM(BlueprintType)
enum class EOLCTargetingMode : uint8
{
	Auto UMETA(DisplayName = "Auto"),
	Focus UMETA(DisplayName = "Focus"),
	Spread UMETA(DisplayName = "Spread"),
	Pulse UMETA(DisplayName = "Pulse")
};

USTRUCT(BlueprintType)
struct FOLCWeaponTIRUpgradeLevel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	float DamageMultiplier = 1.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	TArray<FOLCResourceAmount> MaterialCost;
};

USTRUCT(BlueprintType)
struct FOLCWeaponVisualVariant
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	int32 TIRTier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	FLinearColor GlowColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	float GlowIntensity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	float ProjectileScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	FText EffectDescription;
};

USTRUCT(BlueprintType)
struct FOLCTurretMountConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	EOLCTurretMountType MountType = EOLCTurretMountType::Light;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	EOLCWeaponMountPosition Position = EOLCWeaponMountPosition::Dome;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	int32 WeaponSlots = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	float ArcDegrees = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	bool bRequiresPower = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	TArray<EOLCWeaponDamageType> AllowedDamageTypes;
};

UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	FName WeaponId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	EOLCWeaponDamageType DamageType = EOLCWeaponDamageType::Ballistic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	float BaseDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	float Range = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	float EnergyCostPerShot = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	int32 TIRTier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	float FireRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	EOLCWeaponAmmoType AmmoType = EOLCWeaponAmmoType::Infinite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	int32 BurstCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	float ShieldPenetration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	bool bIgnoresArmor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	EOLCTurretMountType RequiredMount = EOLCTurretMountType::Light;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	EOLCTargetingMode PreferredTargetingMode = EOLCTargetingMode::Auto;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	TArray<FOLCWeaponTIRUpgradeLevel> TIRUpgradeLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Weapon")
	TArray<FOLCWeaponVisualVariant> VisualVariants;

	UFUNCTION(BlueprintPure, Category = "OLC|Weapon")
	float GetDamageWithTIRUpgrade(int32 UpgradeLevel) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Weapon")
	FOLCWeaponVisualVariant GetVisualVariantForTier(int32 InTIRTier) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Weapon")
	EOLCDamageType ToCombatDamageType() const;

	UFUNCTION(BlueprintCallable, Category = "OLC|Weapon")
	static TArray<UOLCWeaponData*> GetDefaultWeaponCatalog(UObject* Outer);

	UFUNCTION(BlueprintCallable, Category = "OLC|Weapon")
	static TArray<UOLCWeaponData*> GetWeaponsByDamageType(UObject* Outer, EOLCWeaponDamageType InDamageType, int32 MaxTIR);
};

