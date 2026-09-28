#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/OLCCombatTypes.h"
#include "Combat/OLCWeaponData.h"
#include "OLCWeaponMountComponent.generated.h"

class UOLCTargetingSystem;

UCLASS(ClassGroup=(OLC), meta=(BlueprintSpawnableComponent))
class OURLASTCHANCE_API UOLCWeaponMountComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOLCWeaponMountComponent();

	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	void ConfigureDefaultMounts();

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	bool CanMountTarget(const FOLCWeaponMountData& Mount, const FVector& ShooterLocation, const FRotator& ShooterRotation, const FVector& TargetLocation) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	TArray<FOLCWeaponMountData> GetMountsThatCanTarget(const FVector& TargetLocation) const;

	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	bool CanMountWeapon(const FOLCTurretMountConfig& MountConfig, const UOLCWeaponData* Weapon, FText& OutReason) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	TArray<FOLCTurretMountConfig> GetDefaultTurretMountConfigs() const;

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	AActor* SelectTargetForMode(EOLCTargetingMode Mode, const TArray<AActor*>& Candidates) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	FOLCWeaponMountData BuildShipMountFromWeapon(const UOLCWeaponData* Weapon, EOLCWeaponMountPosition Position) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	const TArray<FOLCWeaponMountData>& GetMounts() const { return Mounts; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	TArray<FOLCWeaponMountData> Mounts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	TArray<FOLCTurretMountConfig> TurretMountConfigs;
};
