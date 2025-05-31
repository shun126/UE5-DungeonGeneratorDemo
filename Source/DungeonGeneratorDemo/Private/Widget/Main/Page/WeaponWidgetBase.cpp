/**
@author		Shun Moriya



ベースクラス
*/

#include "WeaponWidgetBase.h"
#include "Actor/GamePlayerState.h"
#include "Save/PlayerStatus.h"

UWeaponWidgetBase::UWeaponWidgetBase(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}

TArray<FInventoryWeapon> UWeaponWidgetBase::GetInventory() const
{
	TArray<FInventoryWeapon> result;
	if (const auto* playerState = GetOwningPlayerState<AGamePlayerState>())
	{
		const FInventory& inventory = playerState->GetPlayerStatus().GetInventory();
		for (int32 i = 0; i < WeaponDataIdSize; ++i)
		{
			const auto& pair = inventory.Weapon[i];
			if (pair.Count > 0)
			{
				FInventoryWeapon value;
				value.Id = static_cast<EWeaponDataId>(i);
				value.Inventory = pair;
				result.Emplace(value);
			}
		}
	}
	return result;
}
