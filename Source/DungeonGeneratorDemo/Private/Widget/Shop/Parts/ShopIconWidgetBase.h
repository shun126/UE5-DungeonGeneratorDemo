/**
@author		Shun Moriya
*/

#pragma once
#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "ShopIconWidgetBase.generated.h"

class UShopIconEntryBase;

/**
店アイコンベースクラス
*/
UCLASS(Abstract)
class UShopIconWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UShopIconWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UShopIconWidgetBase() override = default;

	/*
	商品アイコンを設定します
	*/
	UFUNCTION(BlueprintCallable)
	void Set(const UShopIconEntryBase* shopIconEntryBase);

protected:
	// 名称
	UPROPERTY(BlueprintReadWrite)
	FText Name;

	// 説明
	UPROPERTY(BlueprintReadWrite)
	FText Description;

	// 在庫数
	UPROPERTY(BlueprintReadWrite)
	int32 Inventory = 1;

	// 価格
	UPROPERTY(BlueprintReadWrite)
	int32 Price = 0;

	// 性能
	UPROPERTY(BlueprintReadWrite)
	int32 Performance = 0;

	// 熟練度
	UPROPERTY(BlueprintReadWrite)
	float Skill = 0;

	// 識別番号
	UPROPERTY(BlueprintReadWrite)
	int32 Index = 0;
};
