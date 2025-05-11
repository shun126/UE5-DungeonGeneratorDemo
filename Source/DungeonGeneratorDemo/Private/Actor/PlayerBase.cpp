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

	// TODO: ÉfÉÇíÜÇÕêHéñÇé~ÇﬂÇÈ
	if (auto* playerState = GetPlayerState<AGamePlayerState>())
	{
		if (playerState->GetLife() <= 0)
		{
			// éÄñSÇµÇΩÇÃÇ≈í ím
			if (mIsDead == false)
			{
				mIsDead = true;
				UGameplayStatics::SetGamePaused(GetWorld(), true);
				OnDied();
			}
		}
		else
		{
			// êHéñÇÇµÇƒëÃóÕÇâÒïúÇµÇ‹Ç∑
			mEatIntervalTimer += DeltaTime;
			if (mEatIntervalTimer > EatIntervalTimer)
			{
				mEatIntervalTimer = 0;
				// TODO: ÉåÉxÉãÇ…ÇÊÇÈâÒïúó ÇîΩâfÇµÇƒÇ≠ÇæÇ≥Ç¢
				int32 life = 1;
				if (!playerState->GetPlayerStatus().Eat())
					life = -life;
				playerState->AddLife(life);
			}
		}
	}

	DamageHistory.Update(DeltaTime);
}
