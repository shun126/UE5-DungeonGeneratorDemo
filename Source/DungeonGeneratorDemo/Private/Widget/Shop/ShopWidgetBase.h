/**
@author		Shun Moriya
*/

#pragma once
#include "Helper/ShopDialogWidgetBase.h"
#include "Widget/Common/FullScreenWidgetBase.h"
#include "ShopWidgetBase.generated.h"

UENUM(BlueprintType)
enum class EShopWidgetMode : uint8
{
	Buy		UMETA(DisplayName = "買う"),
	Sell	UMETA(DisplayName = "売る")
};

/**
店ベースクラス
*/
UCLASS(Abstract)
class UShopWidgetBase : public UFullScreenWidgetBase
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UShopWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UShopWidgetBase() override = default;

	/**
	登録したストリングテーブルのIDとを所有防具名のストリングテーブルのキー取得します
	\return		登録したストリングテーブルのID
	*/
	UFUNCTION(BlueprintCallable)
	FText Message(const FString& key) const;

	/*
	売買のモードを取得します
	*/
	EShopWidgetMode GetMode() const noexcept;

	/*
	販売します
	*/

	/*
	買取します
	*/

	/*
	モードに合わせた価格を計算します
	*/
	UFUNCTION(BlueprintCallable)
	int32 CalculatePrice(const int32 basePrice) const;

	/*
	販売価格を計算します
	*/
	UFUNCTION(BlueprintCallable)
	int32 CalculateSellingPrice(const int32 basePrice) const;

	/*
	買取価格を計算します
	*/
	UFUNCTION(BlueprintCallable)
	int32 CalculateBuyingPrice(const int32 basePrice) const;

	/*
	PlayGameWidgeへ戻ります
	*/
	UFUNCTION(BlueprintCallable)
	void ReturnToPlayGameWidget();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EShopWidgetMode Mode = EShopWidgetMode::Buy;
};

inline UShopWidgetBase::UShopWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

inline EShopWidgetMode UShopWidgetBase::GetMode() const noexcept
{
	return Mode;
}

inline int32 UShopWidgetBase::CalculatePrice(const int32 basePrice) const
{
	return Mode == EShopWidgetMode::Buy ? CalculateBuyingPrice(basePrice) : CalculateSellingPrice(basePrice);
}
