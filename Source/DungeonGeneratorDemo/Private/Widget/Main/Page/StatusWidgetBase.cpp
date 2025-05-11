/**
@author		Shun Moriya



ステータスベースクラス
*/

#include "StatusWidgetBase.h"
#include "GameInstanceBase.h"
#include "Actor/GamePlayerState.h"


FText UStatusWidgetBase::GetHeroTitle() const noexcept
{
	if (auto playerState = GetOwningPlayerState<AGamePlayerState>())
	{
		return playerState->GetPlayerStatus().GetHeroTitle();
	}

	return FText();
}

FText UStatusWidgetBase::GetEquippedWeaponName() const noexcept
{
	const auto* instance = Cast<UGameInstanceBase>(GetGameInstance());
	if (IsValid(instance))
	{
		auto* playerState = GetOwningPlayerState<AGamePlayerState>();
		if (IsValid(playerState))
		{
			const auto weaponDataId = playerState->GetPlayerStatus().GetEquippedWeapon();
			const auto& asset = instance->GetWeaponDataAsset();
			const auto& data = asset.Get(weaponDataId);
			return FText::FromString(data.Name);
		}
	}

	return FText();
}

int32 UStatusWidgetBase::GetStrength() const noexcept
{
	const auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	return IsValid(playerState) ? playerState->GetPlayerStatus().GetAbility().Strength : 0;
}

int32 UStatusWidgetBase::GetWisdom() const noexcept
{
	const auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	return IsValid(playerState) ? playerState->GetPlayerStatus().GetAbility().Wisdom : 0;
}

int32 UStatusWidgetBase::GetCharisma() const noexcept
{
	const auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	return IsValid(playerState) ? playerState->GetPlayerStatus().GetAbility().Charisma : 0;
}

int32 UStatusWidgetBase::GetKeyCount() const noexcept
{
	const auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	return IsValid(playerState) ? playerState->GetPlayerStatus().Inventory.Dungeons.Key : 0;
}

bool UStatusWidgetBase::HasUniqueKey() const noexcept
{
	const auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	return IsValid(playerState) ? playerState->GetPlayerStatus().Inventory.Dungeons.UniqueKey : false;
}

int32 UStatusWidgetBase::GetGold() const noexcept
{
	const auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	return IsValid(playerState) ? playerState->GetPlayerStatus().GetGold() : 0;
}

int32 UStatusWidgetBase::GetFood() const noexcept
{
	const auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	return IsValid(playerState) ? playerState->GetPlayerStatus().GetFood() : 0;
}

int32 UStatusWidgetBase::GetExperience() const noexcept
{
	const auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	return IsValid(playerState) ? playerState->GetPlayerStatus().GetWeaponExperience() : 0;
}

int32 UStatusWidgetBase::GetLevel() const noexcept
{
	const auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	return IsValid(playerState) ? playerState->GetPlayerStatus().GetWeaponLevel() : 0;
}

int32 UStatusWidgetBase::GetHitPoint() const noexcept
{
	const auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	return IsValid(playerState) ? playerState->GetLife() : 0;
}

int32 UStatusWidgetBase::GetMaxHitPoint() const noexcept
{
	const auto* playerState = GetOwningPlayerState<AGamePlayerState>();
	return IsValid(playerState) ? playerState->GetPlayerStatus().GetMaxHitPoint() : 0;
}
