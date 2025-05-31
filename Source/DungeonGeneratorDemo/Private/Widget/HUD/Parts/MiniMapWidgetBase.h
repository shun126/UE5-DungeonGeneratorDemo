/**
@author		Shun Moriya


*/

#pragma once
#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "MiniMapWidgetBase.generated.h"

/**
UPlayWidgetBaseの配置するミニマップウィジットクラス
*/
UCLASS(Abstract)
class UMiniMapWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual ~UMiniMapWidgetBase() override = default;

protected:
};
