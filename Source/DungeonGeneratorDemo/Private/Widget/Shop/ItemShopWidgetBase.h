/**
@author		Shun Moriya
*/

#pragma once
#include "ShopWidgetBase.h"
#include "ItemShopWidgetBase.generated.h"

// 前方宣言

/**
アイテム店ベースクラス
*/
UCLASS(Abstract)
class UItemShopWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UItemShopWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UItemShopWidgetBase() override = default;

	/*
	販売します
	*/
	/*
	買取します
	*/
};
