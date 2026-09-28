#pragma once

#include "CoreMinimal.h"
#include "OLCCombatTypes.generated.h"

UENUM(BlueprintType)
enum class EOLCCombatPhase : uint8
{
	Engage UMETA(DisplayName = "Engage"),
	ExchangeFire UMETA(DisplayName = "Exchange Fire"),
	Tactical UMETA(DisplayName = "Tactical"),
	Resolution UMETA(DisplayName = "Resolution")
};

UENUM(BlueprintType)
enum class EOLCDamageType : uint8
{
	Kinetic UMETA(DisplayName = "Kinetic"),
	Energy UMETA(DisplayName = "Energy"),
	Explosive UMETA(DisplayName = "Explosive"),
	Void UMETA(DisplayName = "Void")
};

UENUM(BlueprintType)
enum class EOLCWeaponMountPosition : uint8
{
	Nose UMETA(DisplayName = "Nose"),
	Port UMETA(DisplayName = "Port"),
	Starboard UMETA(DisplayName = "Starboard"),
	Tail UMETA(DisplayName = "Tail"),
	Dome UMETA(DisplayName = "Dome")
};

UENUM(BlueprintType)
enum class EOLCCombatLogType : uint8
{
	Damage UMETA(DisplayName = "Damage"),
	Ability UMETA(DisplayName = "Ability"),
	Death UMETA(DisplayName = "Death"),
	Loot UMETA(DisplayName = "Loot"),
	System UMETA(DisplayName = "System")
};

UENUM(BlueprintType)
enum class EOLCMinigameType : uint8
{
	WireRepair UMETA(DisplayName = "Wire Repair"),
	AsteroidEvasion UMETA(DisplayName = "Asteroid Evasion"),
	SignalDecoding UMETA(DisplayName = "Signal Decoding"),
	WarpNavigation UMETA(DisplayName = "Warp Navigation"),
	CargoBayManagement UMETA(DisplayName = "Cargo Bay Management"),
	ReactorStabilization UMETA(DisplayName = "Reactor Stabilization")
};

USTRUCT(BlueprintType)
struct FOLCDamageInput
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	float RawDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	EOLCDamageType DamageType = EOLCDamageType::Kinetic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	int32 WeaponTIR = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	float CriticalChance = 0.05f;
};

USTRUCT(BlueprintType)
struct FOLCDamageResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Combat")
	float ShieldDamage = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Combat")
	float HullDamage = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Combat")
	float FinalDamage = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Combat")
	bool bCriticalHit = false;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Combat")
	bool bDestroyed = false;
};

USTRUCT(BlueprintType)
struct FOLCWeaponMountData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	EOLCWeaponMountPosition Position = EOLCWeaponMountPosition::Nose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	float ArcDegrees = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	float Range = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	FOLCDamageInput Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	bool bEnabled = true;
};

USTRUCT(BlueprintType)
struct FOLCCombatLogEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Combat")
	FText Message;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Combat")
	EOLCCombatLogType Type = EOLCCombatLogType::System;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Combat")
	float TimeSeconds = 0.0f;
};
