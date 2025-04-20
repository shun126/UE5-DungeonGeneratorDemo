/**
@author		Shun Moriya


*/

#pragma once
#include "Widget/Common/FullScreenWidgetBase.h"
#include "LoadWidgetBase.generated.h"

// 前方宣言

/**
ロードベースクラス
*/
UCLASS(Abstract)
class ULoadWidgetBase : public UFullScreenWidgetBase
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit ULoadWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~ULoadWidgetBase() override = default;
};
