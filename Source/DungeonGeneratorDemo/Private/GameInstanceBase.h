/**
@author		Shun Moriya

UGameInstance派生クラスはServerとClient個別に情報を保持します。
情報の共有は行われず切断後も情報は保持されるので、各クライアント固有の情報を扱うことができます。

Serverのみの情報ならAGameModeBaseの利用を検討して下さい。
ServerとClientをリプリケーションするならばAPlayerState派生クラスやAGameStateBase派生クラスの利用を検討して下さい。

有効にする場合は、プロジェクト設定 → Map&Mode → GameInstanceを変更してください
*/

#pragma once
#include "DataAsset/DropDataAsset.h"
#include "DataAsset/ItemDataAsset.h"
#include "DataAsset/WeaponDataAsset.h"
#include "DataAsset/LevelExperienceAsset.h"
#include <Engine/GameInstance.h>
#include "GameInstanceBase.generated.h"

class UEnemyAppearanceAsset;
class UUserWidget;

/**
プロジェクト専用GameInstanceクラス

マルチプレイヤーの場合、UGameInstanceは各クライアント毎に固有で持つので、
同期されないことに注意して下さい。
*/
UCLASS(Abstract)
class UGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:
	/**
	UGameInstanceBaseインスタンスを取得します
	*/
	static UGameInstanceBase* Instance();

	/**
	アイテムデータを取得します
	*/
	const UItemDataAsset& GetItemDataAsset() const;

	/**
	ドロップデータを取得します
	*/
	const UDropDataAsset& GetDropDataAsset() const;

	/**
	武器データを取得します
	*/
	const UWeaponDataAsset& GetWeaponDataAsset() const;

	/**
	敵出現確率を取得します
	*/
	const TArray<TObjectPtr<UEnemyAppearanceAsset>>& GetEnemyAppearanceAsset() const;

	/**
	レベルアップに必要な経験値データを取得します
	*/
	const ULevelExperienceAsset& GetLevelExperienceAsset() const;

	/**
	現在のメニューウィジェットを取り除き、
	指定されたクラスがあればそこから新しいものを作成します
	*/
	UFUNCTION(BlueprintCallable)
	UUserWidget* ChangeCurrentWidget(TSubclassOf<UUserWidget> newWidgetClass);

	/**
	現在のメニューウィジェットを取り除きます
	*/
	UFUNCTION(BlueprintCallable)
	void CloseCurrentWidget();

	/**
	現在のメニューウィジェットを取得します
	*/
	UFUNCTION(BlueprintCallable)
	UUserWidget* GetCurrentWidget() const;

	// UGameInstance overrides
	virtual void Init() override;
	virtual void Shutdown() override;

protected:
	// アイテムデータ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Database")
	TObjectPtr<UItemDataAsset> ItemDataAsset;

	// ドロップデータ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Database")
	TObjectPtr<UDropDataAsset> DropDataAsset;

	// 武器データ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Database")
	TObjectPtr<UWeaponDataAsset> WeaponDataAsset;

	// 敵出現確率
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Database")
	TArray<TObjectPtr<UEnemyAppearanceAsset>> EnemyAppearanceAsset;

	// レベルアップに必要な経験値データ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Database")
	TObjectPtr<ULevelExperienceAsset> LevelExperienceAsset;

	// メニューとして使用するウィジェット インスタンスです
	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> CurrentWidget;
};
