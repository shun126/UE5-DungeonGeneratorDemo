/**
チーム設定が可能なPlayerController ヘッダーファイル

https://www.thinkandbuild.it/ue4-ai-perception-system/
*/

#pragma once
#include <GenericTeamAgentInterface.h>
#include <GameFramework/PlayerController.h>
#include "PlayerControllerBase.generated.h"

/**
チーム設定が可能なPlayerController クラス
*/
UCLASS(Blueprintable, BlueprintType)
class APlayerControllerBase : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	explicit APlayerControllerBase(const FObjectInitializer& ObjectInitializer);
	virtual ~APlayerControllerBase() override = default;

private:
	// IGenericTeamAgentInterface overrides
	virtual void BeginPlay() override;
	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	FGenericTeamId mTeamId;
};
