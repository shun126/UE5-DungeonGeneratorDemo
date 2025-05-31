/**
@author		Shun Moriya
*/

#pragma once
#include <Blueprint/UserWidget.h>
#include "PartsWidgetBase.generated.h"

/**
 * 画面の部分表示メニューのベースクラス
 * FullScreenWidgetBaseクラスと違いゲームの更新を中断しません
 */
UCLASS(Abstract)
class UPartsWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * コンストラクタ
	 */
	explicit UPartsWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	 * デストラクタ
	 */
	virtual ~UPartsWidgetBase() override = default;
};
