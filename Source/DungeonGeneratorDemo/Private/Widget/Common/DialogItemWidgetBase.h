/**
@author		Shun Moriya
*/

#pragma once
#include "DialogItemData.h"
#include <Blueprint/UserWidget.h>
#include "DialogItemWidgetBase.generated.h"

/**
 * ダイアログ リストアイテム ウィジットクラス
 */
UCLASS(Abstract)
class UDialogItemWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * コンストラクタ
	 */
	explicit UDialogItemWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	 * デストラクタ
	 */
	virtual ~UDialogItemWidgetBase() override = default;

	/*
	 * UDialogItemDataの内容を設定します
	 */
	UFUNCTION(BlueprintCallable)
	void Set(const UDialogItemData* dialogEntryBase);

protected:
	// 名称
	UPROPERTY(BlueprintReadWrite)
	FText Name;

	// 識別番号
	UPROPERTY(BlueprintReadWrite)
	int32 Identifier = 0;
};
