#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Combat/OLCCombatTypes.h"
#include "OLCDamageCalculator.generated.h"

UCLASS()
class OURLASTCHANCE_API UOLCDamageCalculator : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	static FOLCDamageResult ResolveDamage(const FOLCDamageInput& Input, float CurrentShield, float CurrentHull, float Armor, int32 DefenderTIR);

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	static float GetArmorMultiplier(EOLCDamageType DamageType, float Armor);

	/**
	 * WP-125: Apply Alien Shield Generator damage modifier to an already-resolved result.
	 * Kinetic → all damage zeroed; Energy → ShieldDamage and HullDamage halved;
	 * Void/Explosive → unchanged. Call after ResolveDamage when bAlienShieldActive is true.
	 */
	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	static FOLCDamageResult ApplyAlienShieldModifier(const FOLCDamageResult& InResult, EOLCDamageType DamageType);
};
