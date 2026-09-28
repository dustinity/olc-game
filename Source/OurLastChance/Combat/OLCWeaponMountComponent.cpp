#include "Combat/OLCWeaponMountComponent.h"
#include "Combat/OLCTargetingSystem.h"

UOLCWeaponMountComponent::UOLCWeaponMountComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	ConfigureDefaultMounts();
}

void UOLCWeaponMountComponent::ConfigureDefaultMounts()
{
	Mounts.Reset();
	TurretMountConfigs = GetDefaultTurretMountConfigs();
	auto AddMount = [this](EOLCWeaponMountPosition Position, float Arc, float Range, EOLCDamageType Type)
	{
		FOLCWeaponMountData Mount;
		Mount.Position = Position;
		Mount.ArcDegrees = Arc;
		Mount.Range = Range;
		Mount.Damage.DamageType = Type;
		Mounts.Add(Mount);
	};

	AddMount(EOLCWeaponMountPosition::Nose, 90.0f, 4200.0f, EOLCDamageType::Energy);
	AddMount(EOLCWeaponMountPosition::Port, 120.0f, 3200.0f, EOLCDamageType::Kinetic);
	AddMount(EOLCWeaponMountPosition::Starboard, 120.0f, 3200.0f, EOLCDamageType::Kinetic);
	AddMount(EOLCWeaponMountPosition::Tail, 80.0f, 2600.0f, EOLCDamageType::Explosive);
	AddMount(EOLCWeaponMountPosition::Dome, 360.0f, 2800.0f, EOLCDamageType::Void);
}

bool UOLCWeaponMountComponent::CanMountTarget(const FOLCWeaponMountData& Mount, const FVector& ShooterLocation, const FRotator& ShooterRotation, const FVector& TargetLocation) const
{
	if (!Mount.bEnabled)
	{
		return false;
	}

	const FVector ToTarget = TargetLocation - ShooterLocation;
	if (ToTarget.Size() > Mount.Range)
	{
		return false;
	}

	if (Mount.ArcDegrees >= 359.0f)
	{
		return true;
	}

	float CenterYaw = ShooterRotation.Yaw;
	switch (Mount.Position)
	{
		case EOLCWeaponMountPosition::Port: CenterYaw -= 90.0f; break;
		case EOLCWeaponMountPosition::Starboard: CenterYaw += 90.0f; break;
		case EOLCWeaponMountPosition::Tail: CenterYaw += 180.0f; break;
		default: break;
	}

	const float TargetYaw = ToTarget.Rotation().Yaw;
	const float Delta = FMath::Abs(FMath::FindDeltaAngleDegrees(CenterYaw, TargetYaw));
	return Delta <= Mount.ArcDegrees * 0.5f;
}

TArray<FOLCWeaponMountData> UOLCWeaponMountComponent::GetMountsThatCanTarget(const FVector& TargetLocation) const
{
	TArray<FOLCWeaponMountData> Result;
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return Result;
	}

	for (const FOLCWeaponMountData& Mount : Mounts)
	{
		if (CanMountTarget(Mount, Owner->GetActorLocation(), Owner->GetActorRotation(), TargetLocation))
		{
			Result.Add(Mount);
		}
	}
	return Result;
}

bool UOLCWeaponMountComponent::CanMountWeapon(const FOLCTurretMountConfig& MountConfig, const UOLCWeaponData* Weapon, FText& OutReason) const
{
	if (!Weapon)
	{
		OutReason = FText::FromString(TEXT("No weapon selected."));
		return false;
	}

	if (static_cast<uint8>(Weapon->RequiredMount) > static_cast<uint8>(MountConfig.MountType))
	{
		OutReason = FText::FromString(TEXT("Weapon requires a heavier turret mount."));
		return false;
	}

	if (MountConfig.AllowedDamageTypes.Num() > 0 && !MountConfig.AllowedDamageTypes.Contains(Weapon->DamageType))
	{
		OutReason = FText::FromString(TEXT("Weapon damage type is not supported by this mount."));
		return false;
	}

	if (MountConfig.bRequiresPower && Weapon->EnergyCostPerShot > 0.0f && MountConfig.WeaponSlots <= 0)
	{
		OutReason = FText::FromString(TEXT("Powered weapon requires an active powered slot."));
		return false;
	}

	OutReason = FText::GetEmpty();
	return true;
}

TArray<FOLCTurretMountConfig> UOLCWeaponMountComponent::GetDefaultTurretMountConfigs() const
{
	TArray<FOLCTurretMountConfig> Configs;

	FOLCTurretMountConfig Light;
	Light.MountType = EOLCTurretMountType::Light;
	Light.WeaponSlots = 1;
	Light.ArcDegrees = 180.0f;
	Light.AllowedDamageTypes = { EOLCWeaponDamageType::Ballistic, EOLCWeaponDamageType::Energy };
	Configs.Add(Light);

	FOLCTurretMountConfig Heavy;
	Heavy.MountType = EOLCTurretMountType::Heavy;
	Heavy.WeaponSlots = 1;
	Heavy.ArcDegrees = 360.0f;
	Heavy.AllowedDamageTypes = { EOLCWeaponDamageType::Ballistic, EOLCWeaponDamageType::Energy, EOLCWeaponDamageType::Rocket, EOLCWeaponDamageType::Ion, EOLCWeaponDamageType::Void };
	Configs.Add(Heavy);

	FOLCTurretMountConfig Dual;
	Dual.MountType = EOLCTurretMountType::Dual;
	Dual.WeaponSlots = 2;
	Dual.ArcDegrees = 180.0f;
	Dual.AllowedDamageTypes = Heavy.AllowedDamageTypes;
	Configs.Add(Dual);

	FOLCTurretMountConfig Quad;
	Quad.MountType = EOLCTurretMountType::Quad;
	Quad.WeaponSlots = 4;
	Quad.ArcDegrees = 360.0f;
	Quad.AllowedDamageTypes = Heavy.AllowedDamageTypes;
	Configs.Add(Quad);

	return Configs;
}

AActor* UOLCWeaponMountComponent::SelectTargetForMode(EOLCTargetingMode Mode, const TArray<AActor*>& Candidates) const
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return nullptr;
	}

	UOLCTargetingSystem* TargetingSystem = GetOwner()->FindComponentByClass<UOLCTargetingSystem>();
	if (!TargetingSystem)
	{
		TargetingSystem = NewObject<UOLCTargetingSystem>(GetOwner());
	}
	TargetingSystem->SetTargetingMode(Mode);
	return TargetingSystem->SelectTarget(Candidates, Owner->GetActorLocation());
}

FOLCWeaponMountData UOLCWeaponMountComponent::BuildShipMountFromWeapon(const UOLCWeaponData* Weapon, EOLCWeaponMountPosition Position) const
{
	FOLCWeaponMountData Mount;
	Mount.Position = Position;
	Mount.bEnabled = Weapon != nullptr;
	if (!Weapon)
	{
		return Mount;
	}

	Mount.Range = Weapon->Range;
	Mount.Damage.RawDamage = Weapon->BaseDamage;
	Mount.Damage.DamageType = Weapon->ToCombatDamageType();
	Mount.Damage.WeaponTIR = Weapon->TIRTier;
	switch (Position)
	{
		case EOLCWeaponMountPosition::Nose: Mount.ArcDegrees = 90.0f; break;
		case EOLCWeaponMountPosition::Port: Mount.ArcDegrees = 180.0f; break;
		case EOLCWeaponMountPosition::Starboard: Mount.ArcDegrees = 180.0f; break;
		case EOLCWeaponMountPosition::Tail: Mount.ArcDegrees = 90.0f; break;
		case EOLCWeaponMountPosition::Dome: Mount.ArcDegrees = 360.0f; break;
	}
	return Mount;
}
