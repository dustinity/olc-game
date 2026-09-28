#include "Combat/OLCSpaceCombatManager.h"

#include "Combat/OLCDamageCalculator.h"
#include "Combat/OLCWeaponMountComponent.h"

void UOLCSpaceCombatManager::StartSpaceCombat(AActor* PlayerShip, AActor* EnemyShip)
{
	PlayerShipActor = PlayerShip;
	EnemyShipActor = EnemyShip;
	PlayerShield = 100.0f;
	PlayerHull = 150.0f;
	EnemyShield = 80.0f;
	EnemyHull = 120.0f;
	bPlayerVictory = false;
	bResolved = false;
	CurrentPhase = EOLCCombatPhase::Engage;
}

void UOLCSpaceCombatManager::AdvanceSpaceCombat()
{
	if (bResolved)
	{
		return;
	}

	switch (CurrentPhase)
	{
		case EOLCCombatPhase::Engage:
			CurrentPhase = EOLCCombatPhase::ExchangeFire;
			break;
		case EOLCCombatPhase::ExchangeFire:
			ExchangeFire();
			CurrentPhase = (EnemyHull <= 0.0f || PlayerHull <= 0.0f) ? EOLCCombatPhase::Resolution : EOLCCombatPhase::Tactical;
			break;
		case EOLCCombatPhase::Tactical:
			CurrentPhase = EOLCCombatPhase::ExchangeFire;
			break;
		case EOLCCombatPhase::Resolution:
			bResolved = true;
			bPlayerVictory = EnemyHull <= 0.0f && PlayerHull > 0.0f;
			break;
	}
}

void UOLCSpaceCombatManager::ExchangeFire()
{
	ApplyShipFire(PlayerShipActor.Get(), EnemyShipActor.Get(), EnemyShield, EnemyHull);
	ApplyShipFire(EnemyShipActor.Get(), PlayerShipActor.Get(), PlayerShield, PlayerHull);
	if (EnemyHull <= 0.0f || PlayerHull <= 0.0f)
	{
		CurrentPhase = EOLCCombatPhase::Resolution;
		bResolved = true;
		bPlayerVictory = EnemyHull <= 0.0f && PlayerHull > 0.0f;
	}
}

void UOLCSpaceCombatManager::ApplyShipFire(AActor* Shooter, AActor* Target, float& TargetShield, float& TargetHull)
{
	if (!Shooter || !Target)
	{
		return;
	}

	UOLCWeaponMountComponent* Mounts = Shooter->FindComponentByClass<UOLCWeaponMountComponent>();
	if (!Mounts)
	{
		return;
	}

	for (const FOLCWeaponMountData& Mount : Mounts->GetMountsThatCanTarget(Target->GetActorLocation()))
	{
		const FOLCDamageResult Damage = UOLCDamageCalculator::ResolveDamage(Mount.Damage, TargetShield, TargetHull, 20.0f, 1);
		TargetShield = FMath::Max(0.0f, TargetShield - Damage.ShieldDamage);
		TargetHull = FMath::Max(0.0f, TargetHull - Damage.HullDamage);
	}
}
