#include "Core/OLCEquipmentData.h"

namespace
{
	UOLCEquipmentData* MakeEquipment(UObject* Outer, const TCHAR* Id, const TCHAR* Name, EOLCEquipmentType Type, EOLCEquipmentSlotType Slot, EOLCEquipmentRarity Rarity, int32 TIR, float Damage, float HP, float Speed, const FString& Faction = TEXT("any"), bool bUnique = false)
	{
		UOLCEquipmentData* Item = NewObject<UOLCEquipmentData>(Outer ? Outer : GetTransientPackage(), NAME_None, RF_Transient);
		Item->EquipmentId = FName(Id);
		Item->DisplayName = FText::FromString(Name);
		Item->EquipmentType = Type;
		Item->SlotType = Slot;
		Item->Rarity = Rarity;
		Item->RequiresTIR = TIR;
		Item->Stats.DamageBonus = Damage;
		Item->Stats.HPBonus = HP;
		Item->Stats.SpeedModifier = Speed;
		Item->FactionRestriction = Faction;
		Item->bUniqueTemplate = bUnique;
		return Item;
	}

	void AddEffect(UOLCEquipmentData* Item, const TCHAR* Effect)
	{
		if (Item)
		{
			Item->SpecialEffects.Add(FText::FromString(Effect));
		}
	}
}

bool UOLCEquipmentData::IsAvailableForFaction(const FString& FactionId) const
{
	return FactionRestriction.IsEmpty() || FactionRestriction.Equals(TEXT("any"), ESearchCase::IgnoreCase) || FactionRestriction.Equals(FactionId, ESearchCase::IgnoreCase);
}

bool UOLCEquipmentData::CanEquip(EOLCEquipmentSlotType TargetSlot, EOLCUnitType UnitType, const FString& FactionId, int32 ColonyTIR, FText& OutReason) const
{
	if (TargetSlot != SlotType)
	{
		OutReason = FText::FromString(TEXT("Wrong equipment slot."));
		return false;
	}
	if (ColonyTIR < RequiresTIR)
	{
		OutReason = FText::FromString(TEXT("Colony TIR is too low."));
		return false;
	}
	if (!IsAvailableForFaction(FactionId))
	{
		OutReason = FText::FromString(TEXT("Faction-restricted equipment."));
		return false;
	}
	if (AllowedUnitTypes.Num() > 0 && !AllowedUnitTypes.Contains(UnitType))
	{
		OutReason = FText::FromString(TEXT("Unit type cannot equip this item."));
		return false;
	}
	OutReason = FText::GetEmpty();
	return true;
}

FLinearColor UOLCEquipmentData::GetRarityColor() const
{
	switch (Rarity)
	{
		case EOLCEquipmentRarity::Common: return FLinearColor(0.55f, 0.56f, 0.58f, 1.0f);
		case EOLCEquipmentRarity::Uncommon: return FLinearColor(0.18f, 0.78f, 0.32f, 1.0f);
		case EOLCEquipmentRarity::Rare: return FLinearColor(0.18f, 0.45f, 1.0f, 1.0f);
		case EOLCEquipmentRarity::Epic: return FLinearColor(0.62f, 0.25f, 0.95f, 1.0f);
		case EOLCEquipmentRarity::Legendary: return FLinearColor(1.0f, 0.48f, 0.08f, 1.0f);
		case EOLCEquipmentRarity::Alien: return FLinearColor(0.0f, 0.88f, 1.0f, 1.0f);
	}
	return FLinearColor::White;
}

TArray<UOLCEquipmentData*> UOLCEquipmentData::GetDefaultEquipmentCatalog(UObject* Outer)
{
	TArray<UOLCEquipmentData*> Items;
	Items.Reserve(34);

	Items.Add(MakeEquipment(Outer, TEXT("EQ_WPN_IMPROVED_RIFLE"), TEXT("Improved Rifles"), EOLCEquipmentType::Weapon, EOLCEquipmentSlotType::Primary, EOLCEquipmentRarity::Common, 2, 5.0f, 0.0f, 0.0f));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_WPN_MACHINE_GUN_V2"), TEXT("Machine Gun Mk II"), EOLCEquipmentType::Weapon, EOLCEquipmentSlotType::Primary, EOLCEquipmentRarity::Uncommon, 2, 8.0f, 0.0f, -0.02f));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_WPN_AP_ROUNDS"), TEXT("Armor-Piercing Rounds"), EOLCEquipmentType::Weapon, EOLCEquipmentSlotType::Secondary, EOLCEquipmentRarity::Rare, 3, 12.0f, 0.0f, 0.0f));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_WPN_SHIELD_DISRUPTOR"), TEXT("Shield Disruptor Attachment"), EOLCEquipmentType::Weapon, EOLCEquipmentSlotType::Secondary, EOLCEquipmentRarity::Epic, 4, 10.0f, 0.0f, 0.0f));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_WPN_VOID_FOCUS"), TEXT("Void Focus Lens"), EOLCEquipmentType::Weapon, EOLCEquipmentSlotType::Primary, EOLCEquipmentRarity::Legendary, 5, 22.0f, 0.0f, 0.0f));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_WPN_ALIEN_SPLINTER"), TEXT("Alien Splinter Cannon"), EOLCEquipmentType::Weapon, EOLCEquipmentSlotType::Primary, EOLCEquipmentRarity::Alien, 5, 35.0f, 0.0f, 0.0f));

	Items.Add(MakeEquipment(Outer, TEXT("EQ_ARM_LIGHT_PLATING"), TEXT("Light Plating"), EOLCEquipmentType::Armor, EOLCEquipmentSlotType::Armor, EOLCEquipmentRarity::Common, 1, 0.0f, 15.0f, -0.01f));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_ARM_COMPOSITE"), TEXT("Composite Armor"), EOLCEquipmentType::Armor, EOLCEquipmentSlotType::Armor, EOLCEquipmentRarity::Uncommon, 2, 0.0f, 28.0f, -0.02f));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_ARM_REACTIVE"), TEXT("Reactive Armor"), EOLCEquipmentType::Armor, EOLCEquipmentSlotType::Armor, EOLCEquipmentRarity::Rare, 3, 0.0f, 45.0f, -0.04f));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_ARM_NANO_WEAVE"), TEXT("Nano-Weave"), EOLCEquipmentType::Armor, EOLCEquipmentSlotType::Armor, EOLCEquipmentRarity::Epic, 4, 0.0f, 55.0f, 0.02f));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_ARM_PHASE_SHIELD"), TEXT("Phase Shielding"), EOLCEquipmentType::Armor, EOLCEquipmentSlotType::Armor, EOLCEquipmentRarity::Legendary, 5, 0.0f, 75.0f, 0.0f));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_ARM_ALIEN_CARAPACE"), TEXT("Alien Carapace"), EOLCEquipmentType::Armor, EOLCEquipmentSlotType::Armor, EOLCEquipmentRarity::Alien, 5, 0.0f, 100.0f, 0.04f));

	Items.Add(MakeEquipment(Outer, TEXT("EQ_UTIL_FIRST_AID"), TEXT("First Aid Kit"), EOLCEquipmentType::Utility, EOLCEquipmentSlotType::Utility, EOLCEquipmentRarity::Common, 1, 0.0f, 10.0f, 0.0f));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_UTIL_SIGNAL_BOOSTER"), TEXT("Signal Booster"), EOLCEquipmentType::Utility, EOLCEquipmentSlotType::Utility, EOLCEquipmentRarity::Uncommon, 2, 0.0f, 0.0f, 0.03f));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_UTIL_STEALTH_COATING"), TEXT("Stealth Coating"), EOLCEquipmentType::Utility, EOLCEquipmentSlotType::Utility, EOLCEquipmentRarity::Rare, 3, 3.0f, 0.0f, 0.08f));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_UTIL_EMP_GENERATOR"), TEXT("EMP Generator"), EOLCEquipmentType::Utility, EOLCEquipmentSlotType::Utility, EOLCEquipmentRarity::Epic, 4, 8.0f, 0.0f, 0.0f));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_UTIL_QUANTUM_SCANNER"), TEXT("Quantum Scanner"), EOLCEquipmentType::Utility, EOLCEquipmentSlotType::Utility, EOLCEquipmentRarity::Legendary, 5, 5.0f, 0.0f, 0.05f));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_UTIL_ALIEN_DECODER"), TEXT("Alien Artifact Decoder"), EOLCEquipmentType::Utility, EOLCEquipmentSlotType::Utility, EOLCEquipmentRarity::Alien, 5, 10.0f, 20.0f, 0.06f));

	Items.Add(MakeEquipment(Outer, TEXT("EQ_NP_IMPLANTS"), TEXT("Cybernetic Implants"), EOLCEquipmentType::Utility, EOLCEquipmentSlotType::Utility, EOLCEquipmentRarity::Epic, 3, 10.0f, 0.0f, 0.20f, TEXT("NeonPunk")));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_NP_EMP_GRENADES"), TEXT("EMP Grenades"), EOLCEquipmentType::Weapon, EOLCEquipmentSlotType::Secondary, EOLCEquipmentRarity::Rare, 3, 7.0f, 0.0f, 0.0f, TEXT("NeonPunk")));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_DR_BALLISTIC_SHIELD"), TEXT("Ballistic Shield"), EOLCEquipmentType::Armor, EOLCEquipmentSlotType::Armor, EOLCEquipmentRarity::Rare, 2, 0.0f, 30.0f, -0.03f, TEXT("DarkRealistic")));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_DR_TACTICAL_NUKE"), TEXT("Tactical Nuke"), EOLCEquipmentType::Weapon, EOLCEquipmentSlotType::Tertiary, EOLCEquipmentRarity::Legendary, 5, 70.0f, 0.0f, -0.05f, TEXT("DarkRealistic")));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_CS_BANANA_TRAP"), TEXT("Banana Peel Trap"), EOLCEquipmentType::Utility, EOLCEquipmentSlotType::Utility, EOLCEquipmentRarity::Uncommon, 2, 2.0f, 0.0f, 0.05f, TEXT("CartoonSciFi")));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_CS_ROCKET_BOOTS"), TEXT("Rocket Boots"), EOLCEquipmentType::Utility, EOLCEquipmentSlotType::Utility, EOLCEquipmentRarity::Epic, 3, 0.0f, 0.0f, 0.50f, TEXT("CartoonSciFi")));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_BR_FIELD_HOSPITAL"), TEXT("Field Hospital Kit"), EOLCEquipmentType::Utility, EOLCEquipmentSlotType::Utility, EOLCEquipmentRarity::Epic, 3, 0.0f, 35.0f, 0.0f, TEXT("BrightRealistic")));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_BR_REINFORCED_HULL"), TEXT("Reinforced Hull"), EOLCEquipmentType::Armor, EOLCEquipmentSlotType::Armor, EOLCEquipmentRarity::Legendary, 4, 0.0f, 80.0f, -0.05f, TEXT("BrightRealistic")));

	Items.Add(MakeEquipment(Outer, TEXT("EQ_UNIQUE_SHADOW_BLADE"), TEXT("Shadow Blade"), EOLCEquipmentType::Weapon, EOLCEquipmentSlotType::Primary, EOLCEquipmentRarity::Legendary, 4, 40.0f, 0.0f, 0.12f, TEXT("any"), true));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_UNIQUE_TITANS_SHIELD"), TEXT("Titan's Shield"), EOLCEquipmentType::Armor, EOLCEquipmentSlotType::Armor, EOLCEquipmentRarity::Legendary, 4, 0.0f, 120.0f, -0.08f, TEXT("any"), true));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_UNIQUE_STAR_HEART"), TEXT("Star Heart Relay"), EOLCEquipmentType::Utility, EOLCEquipmentSlotType::Utility, EOLCEquipmentRarity::Alien, 5, 15.0f, 30.0f, 0.10f, TEXT("any"), true));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_POOL_WORKBENCH_KIT"), TEXT("Workbench Calibration Kit"), EOLCEquipmentType::Utility, EOLCEquipmentSlotType::Tertiary, EOLCEquipmentRarity::Common, 1, 0.0f, 0.0f, 0.02f));
	Items.Add(MakeEquipment(Outer, TEXT("EQ_POOL_LOCKER_HARNESS"), TEXT("Locker Deployment Harness"), EOLCEquipmentType::Utility, EOLCEquipmentSlotType::Tertiary, EOLCEquipmentRarity::Uncommon, 2, 0.0f, 12.0f, 0.02f));

	for (UOLCEquipmentData* Item : Items)
	{
		AddEffect(Item, TEXT("Generated equipment catalog entry"));
	}
	return Items;
}

TArray<UOLCEquipmentData*> UOLCEquipmentData::GetEquipmentBySlot(UObject* Outer, EOLCEquipmentSlotType InSlotType)
{
	TArray<UOLCEquipmentData*> Filtered;
	for (UOLCEquipmentData* Item : GetDefaultEquipmentCatalog(Outer))
	{
		if (Item && Item->SlotType == InSlotType)
		{
			Filtered.Add(Item);
		}
	}
	return Filtered;
}

TArray<UOLCEquipmentData*> UOLCEquipmentData::GetEquipmentByRarity(UObject* Outer, EOLCEquipmentRarity InRarity)
{
	TArray<UOLCEquipmentData*> Filtered;
	for (UOLCEquipmentData* Item : GetDefaultEquipmentCatalog(Outer))
	{
		if (Item && Item->Rarity == InRarity)
		{
			Filtered.Add(Item);
		}
	}
	return Filtered;
}

UOLCEquipmentData* UOLCEquipmentPoolData::GetRandomItem() const
{
	if (AvailableItems.Num() == 0)
	{
		return nullptr;
	}
	return AvailableItems[FMath::RandRange(0, AvailableItems.Num() - 1)];
}

