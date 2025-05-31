/**
@author		Shun Moriya


*/

#pragma once
#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "MapWidgetBase.generated.h"

/**
ベースクラス
*/
UCLASS(Abstract)
class UMapWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual ~UMapWidgetBase() override = default;

protected:
};
