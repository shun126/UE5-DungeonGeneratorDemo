/**
@author		Shun Moriya



ベースクラス
*/

#include "WeaponIconWidgetBase.h"
#include "GameInstanceBase.h"
#include "Actor/GamePlayerState.h"
/*
 *#include "Actor/Item/Stockable.h"
#include "../../../../../LegacyGameSingletonBase.h"
#include "../../../../../LegacyPlayerStateBase.h"
*/
#include <Internationalization/StringTable.h>


UWeaponIconWidgetBase::UWeaponIconWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWeaponIconWidgetBase::GetStringTableNameAndKey(FName& name, FString& key) const
{
	name = StringTablePath.GetAssetPath().GetPackageName();

#if 0
	if (WeaponDataId != EWeaponDataId::Invalid)
	{
		ULegacyGameSingletonBase* gameInstance = ULegacyGameSingletonBase::Instance();
		if (IsValid(gameInstance))
		{
			const FWeaponData& weaponData = gameInstance->GetWeaponDataAsset().Get(WeaponDataId);
			key = weaponData.Key;
		}
	}
#endif
}

FText UWeaponIconWidgetBase::GetLabel() const
{
	const auto* instance = Cast<UGameInstanceBase>(GetGameInstance());
	if (IsValid(instance))
	{
		const auto& asset = instance->GetWeaponDataAsset();
		const auto& data = asset.Get(WeaponDataId);
		return FText::FromString(data.Name);
	}

	static FText dummy;
	return dummy;
}

TSubclassOf<AActor> UWeaponIconWidgetBase::GetActorClass() const
{
	if (WeaponDataId != EWeaponDataId::Invalid)
	{
		const auto* instance = Cast<UGameInstanceBase>(GetGameInstance());
		if (IsValid(instance))
			return instance->GetWeaponDataAsset().Get(WeaponDataId).EquippedActorClass;
	}

	static TSubclassOf<AActor> dummy;
	return dummy;
}

int32 UWeaponIconWidgetBase::GetCount() const
{
	if (WeaponDataId == EWeaponDataId::Invalid)
		return 0;

	const auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	if (!IsValid(playerState))
		return 0;

	const auto inventory = playerState->GetPlayerStatus().GetInventory();
	return inventory.Weapon[static_cast<size_t>(WeaponDataId)].Count;
}

bool UWeaponIconWidgetBase::Has() const
{
	return GetCount() > 0;
}

int32 UWeaponIconWidgetBase::GetAttackPower() const
{
	if (WeaponDataId == EWeaponDataId::Invalid)
		return 0;

	const auto* instance = Cast<UGameInstanceBase>(GetGameInstance());
	if (!IsValid(instance))
		return 0;

	const FWeaponData& weaponData = instance->GetWeaponDataAsset().Get(WeaponDataId);
	return weaponData.AttackPower;
}

int32 UWeaponIconWidgetBase::GetSkill() const
{
	if (WeaponDataId == EWeaponDataId::Invalid)
		return 0;

	const auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	if (!IsValid(playerState))
		return 0;

	const FInventory& inventory = playerState->GetPlayerStatus().GetInventory();
	return inventory.Weapon[static_cast<size_t>(WeaponDataId)].Skill;
}

void UWeaponIconWidgetBase::Equip() const
{
	if (WeaponDataId != EWeaponDataId::Invalid)
	{
		auto* playerState = GetOwningPlayerState<AGamePlayerState>();
		if (IsValid(playerState))
			playerState->GetPlayerStatus().EquipWeapon(WeaponDataId);
	}
}

bool UWeaponIconWidgetBase::CanEquip() const
{
	return GetCount() > 0;
}

void UWeaponIconWidgetBase::UnEquip() const
{
	if (WeaponDataId != EWeaponDataId::Invalid)
	{
		auto* playerState = GetOwningPlayerState<AGamePlayerState>();
		if (IsValid(playerState))
			playerState->GetPlayerStatus().UnEquipWeapon();
	}
}

bool UWeaponIconWidgetBase::CanUnEquip() const
{
	return GetCount() > 0;
}

void UWeaponIconWidgetBase::Drop() const
{
#if 0
	if (WeaponDataId == EWeaponDataId::Invalid)
		return;

	ALegacyPlayerStateBase* playerState = GetOwningPlayerState<ALegacyPlayerStateBase>();
	ULegacyGameSingletonBase* gameInstance = ULegacyGameSingletonBase::Instance();
	if (IsValid(playerState) && IsValid(gameInstance))
	{
		if (playerState->CanDrop(WeaponDataId))
		{
			APawn* pawn = playerState->GetPawn();
			if (IsValid(pawn))
			{
				// スポーンを開始
				AStockable* stockableProp = GetWorld()->SpawnActorDeferred<AStockable>(
					gameInstance->GetWeaponDataAsset().Get(WeaponDataId).PlacedActorClass,
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
					const bool dropResult = playerState->Drop(WeaponDataId);
					check(dropResult);
				}
			}
		}
	}
#endif
}

bool UWeaponIconWidgetBase::CanDrop() const
{
	return GetCount() > 0;
}
