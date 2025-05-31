/**
@author		Shun Moriya
*/

#pragma once
#include "Widget/Common/FullScreenWidgetBase.h"
#include "GameOverWidgetBase.generated.h"

// 前方宣言

/**
 * ゲームオーバーベースクラス
 */
UCLASS(Abstract)
class UGameOverWidgetBase : public UFullScreenWidgetBase
{
	GENERATED_BODY()

public:
	/**
	 * コンストラクタ
	 */
	explicit UGameOverWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	 * デストラクタ
	 */
	virtual ~UGameOverWidgetBase() override = default;
};
