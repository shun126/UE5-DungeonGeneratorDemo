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

	// �f�����͐H�����~�߂�
	{
		// �H�������đ̗͂��񕜂��܂�
		mEatIntervalTimer += DeltaTime;
		if (mEatIntervalTimer > EatIntervalTimer)
		{
			mEatIntervalTimer = 0;
			if (auto* playerState = GetPlayerState<AGamePlayerState>())
			{
				// TODO: ���x���ɂ��񕜗ʂ𔽉f���Ă�������
				int32 life = 1;
				if (!playerState->GetPlayerStatus().Eat())
					life = -life;
				playerState->AddLife(life);
			}
		}
	}
}
