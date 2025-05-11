/**
@author		Shun Moriya



プレイヤー能力
*/

#include "PlayerStatus.h"
#include "../GameInstanceBase.h"
#include <array>

namespace
{
	static FWeaponData dummyWeaponData;
}

void FPlayerStatus::Reset(const bool reset)
{
	Ability.Reset();
	Inventory.Reset(reset);
	Equipment.Reset(reset);
	Gold = 0;
	Food = InitialFood;
	WeaponExperience = 0;
	WeaponLevel = 0;
}

bool FPlayerStatus::CanDrop(const EItemDataId itemId) const
{
	const uint_fast8_t index = static_cast<uint_fast8_t>(itemId);
	if (index >= ItemDataIdSize)
		return false;
	if (Inventory.Item[index].Count < 1)
		return false;
	return true;
}

bool FPlayerStatus::CanDrop(const EWeaponDataId weaponDataId) const
{
	const uint_fast8_t index = static_cast<uint_fast8_t>(weaponDataId);
	if (index >= WeaponDataIdSize)
		return false;
	if (Inventory.Weapon[index].Count <= 0)
		return false;
	return true;
}

bool FPlayerStatus::Drop(const EItemDataId itemId)
{
	if (!CanDrop(itemId))
		return false;

	--Inventory.Item[static_cast<uint_fast8_t>(itemId)].Count;

	return true;
}

bool FPlayerStatus::Drop(const EWeaponDataId weaponDataId)
{
	if (!CanDrop(weaponDataId))
		return false;

	Inventory.Weapon[static_cast<uint_fast8_t>(weaponDataId)].Count = 0;

	return true;
}

bool FPlayerStatus::Buy(const EItemDataId itemId, const int32 price)
{
	const uint_fast8_t index = static_cast<uint_fast8_t>(itemId);
	if (index >= ItemDataIdSize)
		return false;

	if (Gold < price)
		return false;
	Gold -= price;

	++Inventory.Item[index].Count;
	return true;
}

bool FPlayerStatus::Buy(const EWeaponDataId weaponDataId, const int32 price)
{
	const uint_fast8_t index = static_cast<uint_fast8_t>(weaponDataId);
	if (index >= WeaponDataIdSize)
		return false;

	if (Gold < price)
		return false;
	Gold -= price;

	++Inventory.Weapon[index].Count;
	return true;
}

bool FPlayerStatus::Sell(const EItemDataId itemId, const int32 price)
{
	if (!Drop(itemId))
		return false;

	AddGold(price);
	return true;
}

bool FPlayerStatus::Sell(const EWeaponDataId weaponDataId, const int32 price)
{
	if (!Drop(weaponDataId))
		return false;

	AddGold(price);
	return true;
}

void FPlayerStatus::EquipWeapon(const EWeaponDataId weaponDataId)
{
	Equipment.Weapon = weaponDataId;
}

void FPlayerStatus::UnEquipWeapon()
{
	Equipment.Weapon = EWeaponDataId::Invalid;
}

EWeaponDataId FPlayerStatus::GetEquippedWeapon() const
{
	return Equipment.Weapon;
}

const FWeaponData& FPlayerStatus::GetEquippedWeaponData() const
{
#if 0
	if (ULegacyGameSingletonBase* instance = ULegacyGameSingletonBase::Instance())
	{
		const UWeaponDataAsset& weaponDataAsset = instance->GetWeaponDataAsset();
		const FWeaponData& weaponData = weaponDataAsset.Get(GetEquippedWeapon());
		return weaponData;
	}
#endif
	return dummyWeaponData;
}

TSubclassOf<AActor> FPlayerStatus::GetEquippedWeaponActorClass() const
{
	return GetEquippedWeaponData().EquippedActorClass;
}

FAbility& FPlayerStatus::GetAbility()
{
	return Ability;
}

const FAbility& FPlayerStatus::GetAbility() const
{
	return Ability;
}

FInventory& FPlayerStatus::GetInventory()
{
	return Inventory;
}

const FInventory& FPlayerStatus::GetInventory() const
{
	return Inventory;
}

TArray<EItemDataId> FPlayerStatus::GetSalableItems() const
{
	TArray<EItemDataId> result;
#if 0
	if (const ULegacyGameSingletonBase* instance = ULegacyGameSingletonBase::Instance())
	{
		for (uint8 i = 0; i < ItemDataIdSize; ++i)
		{
			const FItemData& data = instance->GetItemDataAsset().Get(static_cast<EItemDataId>(i));
			if (data.CanUseInMenu)
			{
				const auto& pair = Inventory.Item[i];
				if (pair.Count > 0)
					result.Add(static_cast<EItemDataId>(i));
			}
		}
	}
#endif
	return result;
}

TArray<EWeaponDataId> FPlayerStatus::GetSalableWeapons() const
{
	TArray<EWeaponDataId> result;
	for (uint8 i = 0; i < ItemDataIdSize; ++i)
	{
		const auto& pair = Inventory.Weapon[i];
		if (pair.Count > 0)
			result.Add(static_cast<EWeaponDataId>(i));
	}
	return result;
}

float FPlayerStatus::GetItemSkillPercent(const EItemDataId itemDataId) const noexcept
{
	const auto& pair = Inventory.Item[static_cast<uint8>(itemDataId)];
	const float ratio = static_cast<float>(pair.Skill) / static_cast<float>(FInventoryPair::MaxSkill);
	return ratio;
}

float FPlayerStatus::GetWeaponSkillPercent(const EWeaponDataId weaponDataId) const noexcept
{
	const auto& pair = Inventory.Weapon[static_cast<uint8>(weaponDataId)];
	const float ratio = static_cast<float>(pair.Skill) / static_cast<float>(FInventoryPair::MaxSkill);
	return ratio;
}

int32 FPlayerStatus::GetWeaponComboCount() const
{
#if 0
	if (Equipment.Weapon != EWeaponDataId::Invalid)
	{
		ULegacyGameSingletonBase* instance = ULegacyGameSingletonBase::Instance();
		if (instance)
		{
			const size_t index = static_cast<size_t>(Equipment.Weapon);

			//! NOTE:最大コンボ数が変わった時は変更して下さい
			static constexpr std::array<int32, 4> levels = {
				{
					 (FInventoryPair::MaxSkill / 4) * (1),
					 (FInventoryPair::MaxSkill / 4) * (1 + 2),
					 (FInventoryPair::MaxSkill / 4) * (1 + 2 + 3),
					 (FInventoryPair::MaxSkill / 4) * (1 + 2 + 3 + 4),
				}
			};
			for (int_fast8_t i = static_cast<std::int_fast8_t>(levels.size()) - 1; i >= 0; --i)
			{
				if (Inventory.Weapon[index].Skill >= levels[i])
					return i;
			}
		}
	}
#endif
	return 0;
}

/*
武器の基本ダメージ値 * 力
*/
int32 FPlayerStatus::GetWeaponAttackPower() const
{
#if 0
	if (Equipment.Weapon != EWeaponDataId::Invalid)
	{
		ULegacyGameSingletonBase* instance = ULegacyGameSingletonBase::Instance();
		if (instance)
		{
			const UWeaponDataAsset& weaponDataAsset = instance->GetWeaponDataAsset();
			const FWeaponData& weaponData = weaponDataAsset.Get(Equipment.Weapon);
			const float strengthRatio = static_cast<float>(std::min(Ability.Strength, FAbility::Max)) / 100.f;
			return weaponData.AttackPower * strengthRatio;
		}
	}
#endif
	return 0;
}

void FPlayerStatus::AddWeaponExperience(const int32 value)
{
	if (Equipment.Weapon != EWeaponDataId::Invalid)
	{
		const size_t index = static_cast<size_t>(Equipment.Weapon);
		if (Inventory.Weapon[index].Skill < FInventoryPair::MaxSkill)
			++Inventory.Weapon[index].Skill;

		if (WeaponExperience < std::numeric_limits<decltype(WeaponExperience)>::max())
			WeaponExperience += value;
	}
}

int32 FPlayerStatus::GetWeaponExperience() const
{
	return WeaponExperience;
}

uint8 FPlayerStatus::GetWeaponLevel() const
{
	return WeaponLevel;
}

FText FPlayerStatus::GetHeroTitle() const noexcept
{
	if (const auto* instance = UGameInstanceBase::Instance())
	{
		const auto& levelExperience = instance->GetLevelExperienceAsset();
		return levelExperience.Get(GetWeaponLevel()).GetTitle();
	}

	return FText();
}

void FPlayerStatus::AddGold(const int32 amount)
{
	Gold += amount;
	if (Gold < 0)
		Gold = 0;
}

int32 FPlayerStatus::GetGold() const noexcept
{
	return Gold;
}

bool FPlayerStatus::Eat()
{
	const uint32 eatLevel = WeaponLevel + 1;
	const bool result = Food > 0;
	Food -= eatLevel;
	if (Food < 0)
		Food = 0;
	return result;
}

bool FPlayerStatus::Hungry() const
{
	return Food <= 0;
}

void FPlayerStatus::AddFood(const int32 amount)
{
	Food += amount;
	if (Food < 0)
		Food = 0;
}

int32 FPlayerStatus::GetFood() const noexcept
{
	return Food;
}

int32 FPlayerStatus::AddKey(const FString& dungeonName, const int32 count)
{
	Inventory.Dungeons.Key += count;
	return count;
}

bool FPlayerStatus::UseKey(const FString& dungeonName)
{
	if (Inventory.Dungeons.Key <= 0)
		return false;

	--Inventory.Dungeons.Key;
	return true;
}

bool FPlayerStatus::AddUniqueKey(const FString& dungeonName)
{
	if (Inventory.Dungeons.UniqueKey)
		return false;

	Inventory.Dungeons.UniqueKey = true;
	return true;
}

bool FPlayerStatus::UseUniqueKey(const FString& dungeonName)
{
	if (!Inventory.Dungeons.UniqueKey)
		return false;

	Inventory.Dungeons.UniqueKey = false;
	return true;
}

int32 FPlayerStatus::GetMaxHitPoint() const noexcept
{
	return 1000;
}
