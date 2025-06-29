/**
@author		Shun Moriya
*/

#include "PlayerBase.h"
#include "Actor/GamePlayerState.h"
#include <Kismet/GameplayStatics.h>

APlayerBase::APlayerBase(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerBase::RegisterDamageHistory(EDamageHistoryRegisterResult& Result, const AActor* actor)
{
	Result = DamageHistory.Register(actor)
		? EDamageHistoryRegisterResult::Success
		: EDamageHistoryRegisterResult::Failure;
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO: デモ中は食事を止める
	if (auto* playerState = GetPlayerState<AGamePlayerState>())
	{
		if (playerState->GetLife() <= 0)
		{
			// 死亡したので通知
			if (mIsDead == false)
			{
				mIsDead = true;
				UGameplayStatics::SetGamePaused(GetWorld(), true);
				OnDied();
			}
		}
		else
		{
			// 食事をして体力を回復します
			mEatIntervalTimer += DeltaTime;
			if (mEatIntervalTimer > EatIntervalTimer)
			{
				mEatIntervalTimer = 0;
				// TODO: レベルによる回復量を反映してください
				int32 life = 1;
				if (!playerState->GetPlayerStatus().Eat())
					life = -life;
				playerState->AddLife(life);
			}
		}
	}

	DamageHistory.Update(DeltaTime);
}
