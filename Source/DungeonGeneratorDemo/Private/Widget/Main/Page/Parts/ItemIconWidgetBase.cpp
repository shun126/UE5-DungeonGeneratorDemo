/**
@author		Shun Moriya
*/

#include "ItemIconWidgetBase.h"
#include "GameInstanceBase.h"
#include "Actor/GamePlayerState.h"

UItemIconWidgetBase::UItemIconWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FText UItemIconWidgetBase::GetLabel() const
{
	const auto* instance = Cast<UGameInstanceBase>(GetGameInstance());
	if (IsValid(instance))
	{
		const auto& asset = instance->GetItemDataAsset();
		const auto& data = asset.Get(ItemDataId);
		return data.GetName();
	}
	return FText();
}

FText UItemIconWidgetBase::GetDescription() const
{
	const auto* instance = Cast<UGameInstanceBase>(GetGameInstance());
	if (IsValid(instance))
	{
		const auto& asset = instance->GetItemDataAsset();
		const auto& data = asset.Get(ItemDataId);
		return data.GetDescription();
	}
	return FText();
}

TSubclassOf<AActor> UItemIconWidgetBase::GetActorClass() const
{
	const auto* instance = Cast<UGameInstanceBase>(GetGameInstance());
	if (IsValid(instance))
	{
		const auto& asset = instance->GetItemDataAsset();
		const auto& data = asset.Get(ItemDataId);
		return data.ActorClass;
	}
	return TSubclassOf<AActor>();
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
	if (ItemDataId == EItemDataId::Invalid)
		return;

	auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	if (!IsValid(playerState))
		return;

	auto& inventory = playerState->GetPlayerStatus().GetInventory();
	if (inventory.Item[static_cast<size_t>(ItemDataId)].Count <= 0)
		return;

	const float skillRate = inventory.Item[static_cast<size_t>(ItemDataId)].Skill / FInventoryPair::MaxSkill;
	if (!OnUse(skillRate))
		return;

	--inventory.Item[static_cast<size_t>(ItemDataId)].Count;
	++inventory.Item[static_cast<size_t>(ItemDataId)].Skill;
	if (inventory.Item[static_cast<size_t>(ItemDataId)].Skill > FInventoryPair::MaxSkill)
		inventory.Item[static_cast<size_t>(ItemDataId)].Skill = FInventoryPair::MaxSkill;
}

bool UItemIconWidgetBase::CanUse() const
{
	return GetCount() > 0;
}

void UItemIconWidgetBase::Drop() const
{
	if (ItemDataId == EItemDataId::Invalid)
		return;
#if 1
	auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	if (!IsValid(playerState))
		return;

	auto inventory = playerState->GetPlayerStatus().GetInventory();
	if (inventory.Item[static_cast<size_t>(ItemDataId)].Count > 0)
		--inventory.Item[static_cast<size_t>(ItemDataId)].Count;

#else

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
