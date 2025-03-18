/**
チーム設定が可能なPlayerController ソースファイル

https://www.thinkandbuild.it/ue4-ai-perception-system/
*/

#include "PlayerControllerBase.h"

APlayerControllerBase::APlayerControllerBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//PrimaryActorTick.bCanEverTick = true;
	mTeamId = FGenericTeamId(10);
}

void APlayerControllerBase::BeginPlay()
{
	// 親クラスを呼び出す
	Super::BeginPlay();

	// チームを設定
	SetGenericTeamId(mTeamId);
}

FGenericTeamId APlayerControllerBase::GetGenericTeamId() const
{
	return mTeamId;
}
