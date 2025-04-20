/**
@author		Shun Moriya
*/

#pragma once
#include "Widget/Common/FullScreenWidgetBase.h"
#include "TitleWidgetBase.generated.h"

// 前方宣言

/**
 * タイトルベースクラス
 */
UCLASS(Abstract)
class UTitleWidgetBase : public UFullScreenWidgetBase
{
	GENERATED_BODY()

public:
	/**
	 * コンストラクタ
	 */
	explicit UTitleWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	 * デストラクタ
	 */
	virtual ~UTitleWidgetBase() override = default;
};
