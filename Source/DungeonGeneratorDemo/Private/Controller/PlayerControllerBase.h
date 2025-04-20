/**
@author		Shun Moriya



チーム設定が可能なPlayerController ヘッダーファイル

https://www.thinkandbuild.it/ue4-ai-perception-system/
*/

#pragma once
#include "GamePlayerController.h"
#include "PlayerControllerBase.generated.h"

/**
チーム設定が可能なPlayerController クラス
AGamePlayerControllerに統合して削除予定
*/
UCLASS(Blueprintable, BlueprintType)
class APlayerControllerBase : public AGamePlayerController
{
	GENERATED_BODY()

public:
	explicit APlayerControllerBase(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
	}
	virtual ~APlayerControllerBase() override = default;
};
