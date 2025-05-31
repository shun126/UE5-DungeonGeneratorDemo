/**
@author		Shun Moriya


*/

#pragma once
#include "Widget/Common/FullScreenWidgetBase.h"
#include "MainWidgetBase.generated.h"

// 前方宣言
class ADungeonGenerateActor;
class UWidgetSwitcher;

/**
ベースクラス
*/
UCLASS(Abstract)
class UMainWidgetBase : public UFullScreenWidgetBase
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UMainWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UMainWidgetBase() override = default;

	/*
	指定したUWidgetSwitcherのアクティブなインデックスを切り替えます
	*/
	UFUNCTION(BlueprintCallable)
	static void ChangePage(UWidgetSwitcher* widgetSwitcher, const int32 page, const bool loop);

	/*
	指定したUWidgetSwitcherを左（配列では上）に切り替えます
	*/
	UFUNCTION(BlueprintCallable)
	static int32 ChangeLeftPage(UWidgetSwitcher* widgetSwitcher, const bool loop);

	/*
	指定したUWidgetSwitcherを右（配列では下）に切り替えます
	*/
	UFUNCTION(BlueprintCallable)
	static int32 ChangeRightPage(UWidgetSwitcher* widgetSwitcher, const bool loop);

	/*
	指定したUWidgetSwitcherを左（配列では上）に切り替えられるか調べます
	*/
	UFUNCTION(BlueprintCallable)
	static bool CanChangeLeftPage(UWidgetSwitcher* widgetSwitcher);

	/*
	指定したUWidgetSwitcherを右（配列では下）に切り替えられるか調べます
	*/
	UFUNCTION(BlueprintCallable)
	static bool CanChangeRightPage(UWidgetSwitcher* widgetSwitcher);

private:
	/**
	ページ番号のクランプ
	*/
	static int32 ClampPage(UWidgetSwitcher* widgetSwitcher, const int32 page, const bool loop);

protected:
	//UPROPERTY(Transient, BlueprintReadWrite)
	//TObjectPtr<ADungeonGenerateActor> DungeonGenerateActor;
};
