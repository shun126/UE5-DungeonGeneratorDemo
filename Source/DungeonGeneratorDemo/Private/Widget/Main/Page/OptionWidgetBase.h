/**
@author		Shun Moriya


*/

#pragma once
#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "OptionWidgetBase.generated.h"

// 前方宣言

/**
オプションベースクラス
*/
UCLASS(Abstract)
class UOptionWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UOptionWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UOptionWidgetBase() override = default;
};
