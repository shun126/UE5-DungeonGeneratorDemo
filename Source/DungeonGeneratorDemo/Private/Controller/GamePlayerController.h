/**
@author		Shun Moriya

チーム設定が可能なPlayerController ヘッダーファイル

https://www.thinkandbuild.it/ue4-ai-perception-system/
*/

#pragma once
#include <GenericTeamAgentInterface.h>
#include <GameFramework/PlayerController.h>
#include "GamePlayerController.generated.h"

/**
チーム設定が可能なPlayerController クラス
*/
UCLASS(Blueprintable, BlueprintType)
class AGamePlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	explicit AGamePlayerController(const FObjectInitializer& ObjectInitializer);
	virtual ~AGamePlayerController() override = default;

private:
	// IGenericTeamAgentInterface overrides
	virtual void BeginPlay() override;
	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	FGenericTeamId mTeamId;
};
