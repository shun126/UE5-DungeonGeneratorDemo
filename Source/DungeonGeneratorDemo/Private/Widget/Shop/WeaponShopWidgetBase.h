/**
@author		Shun Moriya
*/

#pragma once
#include "ShopWidgetBase.h"
#include "WeaponShopWidgetBase.generated.h"

// 前方宣言

/**
武器店ベースクラス
*/
UCLASS(Abstract)
class UWeaponShopWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UWeaponShopWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UWeaponShopWidgetBase() override = default;

	/*
	販売します
	*/
	/*
	買取します
	*/
};

UCLASS(Blueprintable, BlueprintType)
class USwordShopWidgetBase : public UWeaponShopWidgetBase
{
	GENERATED_BODY()
};
