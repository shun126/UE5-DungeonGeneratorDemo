/**
@author		Shun Moriya
*/

#include "PlayerBase.h"
#include "Actor/GamePlayerState.h"

APlayerBase::APlayerBase(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ƒfƒ‚’†‚ÍHŽ–‚ðŽ~‚ß‚é
	{
		// HŽ–‚ð‚µ‚Ä‘Ì—Í‚ð‰ñ•œ‚µ‚Ü‚·
		mEatIntervalTimer += DeltaTime;
		if (mEatIntervalTimer > EatIntervalTimer)
		{
			mEatIntervalTimer = 0;
			if (auto* playerState = GetPlayerState<AGamePlayerState>())
			{
				// TODO: ƒŒƒxƒ‹‚É‚æ‚é‰ñ•œ—Ê‚ð”½‰f‚µ‚Ä‚­‚¾‚³‚¢
				int32 life = 1;
				if (!playerState->GetPlayerStatus().Eat())
					life = -life;
				playerState->AddLife(life);
			}
		}
	}
}
