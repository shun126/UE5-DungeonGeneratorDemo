/**
@author		Shun Moriya
*/

#pragma once
#include <Blueprint/UserWidget.h>
#include <Delegates/IDelegateInstance.h>
#include "ShopDialogWidgetBase.generated.h"

// 前方宣言
class UShopWidgetBase;

UENUM(BlueprintType)
enum class EShopDialogResult : uint8
{
	NO		UMETA(DisplayName = "はい"),
	YES		UMETA(DisplayName = "いいえ")
};

// イベント型定義
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShopDialogActionSignature, const int32, index, const EShopDialogResult, shopActionType);

/**
店の行動ウィジットベースクラス
*/
UCLASS(Abstract)
class UShopDialogWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UShopDialogWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UShopDialogWidgetBase() override = default;

	// 決定
	UFUNCTION(BlueprintCallable)
	void Decide(const EShopDialogResult shopActionType);

protected:
	/*
	店ダイアログ決定イベント
	購入・販売決定時に呼び出されます
	*/
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FShopDialogActionSignature OnDecided;

	// 識別番号
	UPROPERTY(BlueprintReadWrite)
	int32 Index = 0;

	// 名称
	UPROPERTY(BlueprintReadWrite)
	FText Name;
};
