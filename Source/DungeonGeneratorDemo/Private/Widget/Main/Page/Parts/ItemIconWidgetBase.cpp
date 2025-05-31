/**
@author		Shun Moriya



ベースクラス
*/

#include "ItemIconWidgetBase.h"
#include "GameInstanceBase.h"
#include "Actor/GamePlayerState.h"
//#include "Actor/Item/Stockable.h"
//#include "../../../../../LegacyGameSingletonBase.h"
//#include "../../../../../LegacyPlayerStateBase.h"
#include <Internationalization/StringTable.h>

UItemIconWidgetBase::UItemIconWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UItemIconWidgetBase::GetStringTableNameAndKey(FName& name, FString& key) const
{
#if 0
	name = StringTablePath.GetAssetPath().GetPackageName();

	if (ItemDataId != EItemDataId::Invalid)
	{
		ULegacyGameSingletonBase* gameInstance = ULegacyGameSingletonBase::Instance();
		if (IsValid(gameInstance))
		{
			const FItemData& itemData = gameInstance->GetItemDataAsset().Get(ItemDataId);
			key = itemData.Key;
		}
	}
#endif
}

FText UItemIconWidgetBase::GetLabel() const
{
	const auto* instance = Cast<UGameInstanceBase>(GetGameInstance());
	if (IsValid(instance))
	{
		const auto& asset = instance->GetItemDataAsset();
		const auto& data = asset.Get(ItemDataId);
		return FText::FromString(data.Name);
	}

	static FText dummy;
	return dummy;
}

TSubclassOf<AActor> UItemIconWidgetBase::GetActorClass() const
{
#if 0
	if (ItemDataId != EItemDataId::Invalid)
	{
		ULegacyGameSingletonBase* gameInstance = ULegacyGameSingletonBase::Instance();
		if (IsValid(gameInstance))
			return gameInstance->GetItemDataAsset().Get(ItemDataId).ActorClass;
	}
#endif
	static TSubclassOf<AActor> dummy;
	return dummy;
}

int32 UItemIconWidgetBase::GetCount() const
{
	if (ItemDataId == EItemDataId::Invalid)
		return 0;

	const auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	if (!IsValid(playerState))
		return 0;

	const auto inventory = playerState->GetPlayerStatus().GetInventory();
	return inventory.Item[static_cast<size_t>(ItemDataId)].Count;
}

int32 UItemIconWidgetBase::GetSkill() const
{
	if (ItemDataId == EItemDataId::Invalid)
		return 0;

	const auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	if (!IsValid(playerState))
		return 0;

	const auto inventory = playerState->GetPlayerStatus().GetInventory();
	return inventory.Item[static_cast<size_t>(ItemDataId)].Skill;
}

void UItemIconWidgetBase::Use() const
{
#if 0
	if (ItemDataId != EItemDataId::Invalid)
	{
		ALegacyPlayerStateBase* playerState = GetOwningPlayerState<ALegacyPlayerStateBase>();
		if (IsValid(playerState))
		{
			playerState->Use(ItemDataId);
		}
	}
#endif
}

bool UItemIconWidgetBase::CanUse() const
{
	return GetCount() > 0;
}

void UItemIconWidgetBase::Drop() const
{
#if 0
	if (ItemDataId == EItemDataId::Invalid)
		return;

	ALegacyPlayerStateBase* playerState = GetOwningPlayerState<ALegacyPlayerStateBase>();
	ULegacyGameSingletonBase* gameInstance = ULegacyGameSingletonBase::Instance();
	if (IsValid(playerState) && IsValid(gameInstance))
	{
		if (playerState->CanDrop(ItemDataId))
		{
			APawn* pawn = playerState->GetPawn();
			if (IsValid(pawn))
			{
				// アクターをスポーンを開始
				AStockable* stockableProp = GetWorld()->SpawnActorDeferred<AStockable>(
					gameInstance->GetItemDataAsset().Get(ItemDataId).ActorClass,
					pawn->GetActorTransform(),
					pawn,
					pawn,
					ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
				if (IsValid(stockableProp))
				{
					// スポーンを完了
					stockableProp->FinishSpawning(pawn->GetActorTransform());

					// 少し勢いをつける
					if (IsValid(stockableProp->GetMesh()))
					{
						stockableProp->GetMesh()->AddImpulse(stockableProp->GetImpulseAtDrop(), NAME_None, true);
					}

					// 所持品から捨てる
					const bool dropResult = playerState->Drop(ItemDataId);
					check(dropResult);
				}
			}
		}
	}
#endif
}

bool UItemIconWidgetBase::CanDrop() const
{
	return GetCount() > 0;
}
