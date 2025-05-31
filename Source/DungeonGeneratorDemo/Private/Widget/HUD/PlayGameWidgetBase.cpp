/**
@author		Shun Moriya

ゲームプレイ中のメインHUDベースクラス

参考
https://docs.unrealengine.com/4.27/ja/InteractiveExperiences/UMG/QuickStart/
*/

#include "PlayGameWidgetBase.h"
#include "Actor/GamePlayerState.h"
#include <Internationalization/StringTable.h>
#include <Kismet/KismetStringTableLibrary.h>

UPlayGameWidgetBase::UPlayGameWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPlayGameWidgetBase::BeginDestroy()
{
	Super::BeginDestroy();
}

void UPlayGameWidgetBase::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
}

void UPlayGameWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UPlayGameWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
#if 0
	// 魔法を構えた状態と魔法の照準を一致させる
	if (TObjectPtr<APlayerAvatar> playerAvatar = Cast<APlayerAvatar>(GetOwningPlayerPawn()))
	{
		const bool isHoldWeapon = playerAvatar->IsHoldWeapon();
		if (mShowGunSight != isHoldWeapon)
		{
			if (isHoldWeapon)
				OnHoldWeapon();
			else
				OnLowerWeapon();
			mShowGunSight = isHoldWeapon;
		}
	}
#endif
}

FText UPlayGameWidgetBase::GetFoodValue() const noexcept
{
	if (auto playerState = GetOwningPlayerState<AGamePlayerState>())
	{
		return FText::AsNumber(playerState->GetPlayerStatus().GetFood());
	}

	return FText::AsNumber(0);
}

FText UPlayGameWidgetBase::GetGoldValue() const noexcept
{
	if (auto playerState = GetOwningPlayerState<AGamePlayerState>())
	{
		return FText::AsNumber(playerState->GetPlayerStatus().GetGold());
	}

	return FText::AsNumber(0);
}

FText UPlayGameWidgetBase::GetHeroTitle() const noexcept
{
	if (auto playerState = GetOwningPlayerState<AGamePlayerState>())
	{
		return playerState->GetPlayerStatus().GetHeroTitle();
	}

	return FText();
}

FText UPlayGameWidgetBase::GetExperienceValue() const noexcept
{
	if (auto playerState = GetOwningPlayerState<AGamePlayerState>())
	{
		return FText::AsNumber(playerState->GetPlayerStatus().GetWeaponExperience());
	}

	return FText::AsNumber(0);
}

FText UPlayGameWidgetBase::GetSkillPercentOfEquippedWeapon() const noexcept
{
#if 0
	ALegacyPlayerStateBase* playerState = GetOwningPlayerState<ALegacyPlayerStateBase>();
	if (IsValid(playerState))
	{
		const EWeaponDataId weaponDataId = playerState->GetEquippedWeapon();
		if (EWeaponDataId::Invalid != weaponDataId)
		{
			FNumberFormattingOptions numberFormattingOptions;
			numberFormattingOptions.SetRoundingMode(ERoundingMode::ToZero);
			numberFormattingOptions.SetMinimumIntegralDigits(1);
			numberFormattingOptions.SetMaximumIntegralDigits(3);
			numberFormattingOptions.SetMinimumFractionalDigits(0);
			numberFormattingOptions.SetMaximumFractionalDigits(1);
			const float percent = playerState->GetWeaponSkillPercent(weaponDataId);
			return FText::AsPercent(percent, &numberFormattingOptions);
		}
		else
		{
			return FText::FromString(TEXT("---"));
		}
	}
	else
	{
		//PROP_ERROR(TEXT("アイテムを拾うプレイヤーステートが見つかりません"));
		return FText::AsNumber(0);
	}
#else
	return FText::AsNumber(0);
#endif
}

FText UPlayGameWidgetBase::GetSkillPercentOfEquippedMagic() const noexcept
{
#if 0
	ALegacyPlayerStateBase* playerState = GetOwningPlayerState<ALegacyPlayerStateBase>();
	if (IsValid(playerState))
	{
		const EWeaponDataId weaponDataId = playerState->GetEquippedMagic();
		if (EWeaponDataId::Invalid != weaponDataId)
		{
			FNumberFormattingOptions numberFormattingOptions;
			numberFormattingOptions.SetRoundingMode(ERoundingMode::ToZero);
			numberFormattingOptions.SetMinimumIntegralDigits(1);
			numberFormattingOptions.SetMaximumIntegralDigits(3);
			numberFormattingOptions.SetMinimumFractionalDigits(0);
			numberFormattingOptions.SetMaximumFractionalDigits(1);
			const float percent = playerState->GetWeaponSkillPercent(weaponDataId);
			return FText::AsPercent(percent, &numberFormattingOptions);
		}
		else
		{
			return FText::FromString(TEXT("---"));
		}
	}
	else
	{
		//PROP_ERROR(TEXT("アイテムを拾うプレイヤーステートが見つかりません"));
		return FText::AsNumber(0);
	}
#else
	return FText::AsNumber(0);
#endif
}

FText UPlayGameWidgetBase::GetSkillPercentOfEquippedArmor() const noexcept
{
#if 0
	ALegacyPlayerStateBase* playerState = GetOwningPlayerState<ALegacyPlayerStateBase>();
	if (IsValid(playerState))
	{
		const EArmorDataId armorDataId = playerState->GetEquippedArmor();
		if (EArmorDataId::Invalid != armorDataId)
		{
			FNumberFormattingOptions numberFormattingOptions;
			numberFormattingOptions.SetRoundingMode(ERoundingMode::ToZero);
			numberFormattingOptions.SetMinimumIntegralDigits(1);
			numberFormattingOptions.SetMaximumIntegralDigits(3);
			numberFormattingOptions.SetMinimumFractionalDigits(0);
			numberFormattingOptions.SetMaximumFractionalDigits(1);
			const float percent = playerState->GetArmorSkillPercent(armorDataId);
			return FText::AsPercent(percent, &numberFormattingOptions);
		}
		else
		{
			return FText::FromString(TEXT("---"));
		}
	}
	else
	{
		//PROP_ERROR(TEXT("アイテムを拾うプレイヤーステートが見つかりません"));
		return FText::AsNumber(0);
	}
#else
	return FText::AsNumber(0);
#endif
}
