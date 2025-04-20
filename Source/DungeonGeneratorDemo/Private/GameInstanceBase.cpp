/**
@author		Shun Moriya

UGameInstance派生クラスはServerとClient個別に情報を保持します。
情報の共有は行われず切断後も情報は保持されるので、各クライアント固有の情報を扱うことができます。

Serverのみの情報ならAGameModeBaseの利用を検討して下さい。
ServerとClientをリプリケーションするならばAPlayerState派生クラスやAGameStateBase派生クラスの利用を検討して下さい。

有効にする場合は、プロジェクト設定 → Map&Mode → GameInstanceを変更してください

https://www.wenyanet.com/opensource/ja/60b954a2f9529b73a72b1a32.html
*/

#include "GameInstanceBase.h"
#include <Blueprint/UserWidget.h>

UGameInstanceBase* UGameInstanceBase::Instance()
{
	UGameInstanceBase* instance = nullptr;

	if (GEngine)
	{
		FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		instance = Cast<UGameInstanceBase>(context->OwningGameInstance);
	}

	return instance;
}

void UGameInstanceBase::Init()
{
	// 親クラス呼び出し
	Super::Init();
}

void UGameInstanceBase::Shutdown()
{
	// 親クラス呼び出し
	Super::Shutdown();
}

const UItemDataAsset& UGameInstanceBase::GetItemDataAsset() const
{
	check(ItemDataAsset);
	return *ItemDataAsset;
}

const UDropDataAsset& UGameInstanceBase::GetDropDataAsset() const
{
	check(DropDataAsset);
	return *DropDataAsset;
}

const UWeaponDataAsset& UGameInstanceBase::GetWeaponDataAsset() const
{
	check(WeaponDataAsset);
	return *WeaponDataAsset;
}

const TArray<TObjectPtr<UEnemyAppearanceAsset>>& UGameInstanceBase::GetEnemyAppearanceAsset() const
{
	return EnemyAppearanceAsset;
}

const ULevelExperienceAsset& UGameInstanceBase::GetLevelExperienceAsset() const
{
	check(LevelExperienceAsset);
	return *LevelExperienceAsset;
}

UUserWidget* UGameInstanceBase::ChangeCurrentWidget(TSubclassOf<UUserWidget> newWidgetClass)
{
	CloseCurrentWidget();

	if (newWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), newWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}

	return CurrentWidget;
}

void UGameInstanceBase::CloseCurrentWidget()
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
}

UUserWidget* UGameInstanceBase::GetCurrentWidget() const
{
	return CurrentWidget;
}
