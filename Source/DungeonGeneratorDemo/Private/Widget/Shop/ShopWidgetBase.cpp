/**
@author		Shun Moriya

店ベースクラス
*/

#include "ShopWidgetBase.h"
#include "Helper/Log.h"
//#include "../../GameLevelScriptActor.h"
#include "../../Save/Ability.h"
//#include "../../../LegacyGameModeBase.h"
//#include "../../../LegacyPlayerStateBase.h"
#include <Kismet/GameplayStatics.h>

FText UShopWidgetBase::Message(const FString& key) const
{
	return FText::FromStringTable(GetStringTablePackageName(), key);
}

/*
カリスマ値が最大で販売価格が半額。カリスマ値が最小になると倍額
*/
int32 UShopWidgetBase::CalculateSellingPrice(const int32 basePrice) const
{
#if 0
	const ALegacyPlayerStateBase* playerState = GetOwningPlayerState<ALegacyPlayerStateBase>();
	if (!IsValid(playerState))
		return basePrice;

	const uint8 inverseCharisma = FAbility::Max - playerState->GetAbility().Charisma;
	const float inverseCharismaRatio = static_cast<float>(inverseCharisma) / static_cast<float>(FAbility::Max);
	const float charismaRatio = 0.5f + (2.f - 0.5f) * inverseCharismaRatio;
	return basePrice * charismaRatio;
#else
	return 0;
#endif
}

/*
買取価格は販売価格の３分の１の値段が基本
カリスマ値が最大で販売価格の３分の２の値段
*/
int32 UShopWidgetBase::CalculateBuyingPrice(const int32 basePrice) const
{
#if 0
	const ALegacyPlayerStateBase* playerState = GetOwningPlayerState<ALegacyPlayerStateBase>();
	if (!IsValid(playerState))
		return basePrice;

	float charismaRatio = static_cast<float>(playerState->GetAbility().Charisma) / static_cast<float>(FAbility::Max);
	if (charismaRatio <= 0.f)
		charismaRatio = std::numeric_limits<float>::epsilon();

	const float buyingPrice = static_cast<float>(basePrice / 3) * charismaRatio;
	return std::max(1.f, buyingPrice);
#else
	return 0;
#endif
}

void UShopWidgetBase::ReturnToPlayGameWidget()
{
#if 0
#if 0
	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameLevelScriptActor::StaticClass());
	AGameLevelScriptActor* gameLevelScriptActor = Cast<AGameLevelScriptActor>(actor);
	if (IsValid(gameLevelScriptActor))
	{
		gameLevelScriptActor->OpenMainWidget();
	}
#else
	if (UWorld* world = GetValid(GetWorld()))
	{
		/*
		TODO:AGameLevelScriptActorの静的関数化を検討してください
		*/
		if (ALegacyGameModeBase* gameMode = GetValid(world->GetAuthGameMode<ALegacyGameModeBase>()))
		{
			// PlayWidget(BPの方)を生成
			TSubclassOf<UUserWidget> playWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*AGameLevelScriptActor::PlayWidgetPath)).LoadSynchronous();
			if (playWidgetClass == nullptr)
			{
				WIDGET_ERROR(TEXT("%s の生成に失敗しました"), *AGameLevelScriptActor::PlayWidgetPath);
			}
			gameMode->ChangeMenuWidget(playWidgetClass);
		}
	}
#endif
#endif
}
