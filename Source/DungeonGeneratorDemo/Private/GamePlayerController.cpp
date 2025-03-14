/**
チーム設定が可能なPlayerController ソースファイル

https://www.thinkandbuild.it/ue4-ai-perception-system/
*/

#include "GamePlayerController.h"

AGamePlayerController::AGamePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//PrimaryActorTick.bCanEverTick = true;
	mTeamId = FGenericTeamId(10);
}

void AGamePlayerController::BeginPlay()
{
	// 親クラスを呼び出す
	Super::BeginPlay();

	// チームを設定
	SetGenericTeamId(mTeamId);
}

FGenericTeamId AGamePlayerController::GetGenericTeamId() const
{
	return mTeamId;
}
