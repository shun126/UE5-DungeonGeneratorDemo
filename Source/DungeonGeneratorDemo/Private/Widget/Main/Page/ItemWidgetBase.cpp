/**
@author		Shun Moriya



ベースクラス
*/

#include "ItemWidgetBase.h"
//#include "../../../../LegacyPlayerStateBase.h"
#include <Internationalization/StringTable.h>
#include "Actor/GamePlayerState.h"
#include "Save/PlayerStatus.h"

UItemWidgetBase::UItemWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FName UItemWidgetBase::GetStringTableName() const
{
	return StringTablePath.GetAssetPath().GetPackageName();
}

TArray<FInventoryItem> UItemWidgetBase::GetInventory() const
{
	TArray<FInventoryItem> result;
	if (const auto* playerState = GetOwningPlayerState<AGamePlayerState>())
	{
		const FInventory& inventory = playerState->GetPlayerStatus().GetInventory();
		for (int32 i = 0; i < ItemDataIdSize; ++i)
		{
			const auto& pair = inventory.Item[i];
			if (pair.Count > 0)
			{
				FInventoryItem value;
				value.Id = static_cast<EItemDataId>(i);
				value.Inventory = pair;
				result.Emplace(value);
			}
		}
	}
	return result;
}

uint8 UItemWidgetBase::GetCount(const EItemDataId itemDataId) const
{
#if 0
	const ALegacyPlayerStateBase* playerState = GetOwningPlayerState<ALegacyPlayerStateBase>();
	if (!IsValid(playerState))
		return 0;

	const FInventory& inventory = playerState->GetInventory();
	return inventory.Item[static_cast<size_t>(itemDataId)].Count;
#else
	return 0;
#endif
}

int32 UItemWidgetBase::GetSkill(const EItemDataId itemDataId) const
{
#if 0
	const ALegacyPlayerStateBase* playerState = GetOwningPlayerState<ALegacyPlayerStateBase>();
	if (!IsValid(playerState))
		return 0;

	const FInventory& inventory = playerState->GetInventory();
	return inventory.Item[static_cast<size_t>(itemDataId)].Skill;
#else
	return 0;
#endif
}

void UItemWidgetBase::Use(const EItemDataId itemDataId) const
{
#if 0
	ALegacyPlayerStateBase* playerState = GetOwningPlayerState<ALegacyPlayerStateBase>();
	if (IsValid(playerState))
	{
		FInventory& inventory = playerState->GetInventory();
		if (inventory.Item[static_cast<size_t>(itemDataId)].Count > 0)
		{
			--inventory.Item[static_cast<size_t>(itemDataId)].Count;
			++inventory.Item[static_cast<size_t>(itemDataId)].Skill;
		}
	}
#endif
}
