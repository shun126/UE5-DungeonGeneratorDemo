/**
@author		Shun Moriya


*/

#pragma once
#include "Widget/Common/FullScreenWidgetBase.h"
#include "SaveWidgetBase.generated.h"

// 前方宣言

/**
寺院ベースクラス
*/
UCLASS(Abstract)
class USaveWidgetBase : public UFullScreenWidgetBase
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit USaveWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~USaveWidgetBase() override = default;
};
