/**
 * ドロップ食料基底クラス
 */

#include "DropFoodBase.h"
#include "Actor/GamePlayerState.h"
#include "Actor/PlayerBase.h"
#include "Helper/Log.h"
#include "GameInstanceBase.h"

ADropFoodBase::ADropFoodBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FText ADropFoodBase::GetLabel_Implementation() const
{
	// TODO: ローカライズに対応してください
	return FText::FromString(TEXT("食料"));
}

void ADropFoodBase::PickUp(const APawn* finderPawn)
{
	if (IsValid(finderPawn))
	{
		if (auto playerState = finderPawn->GetPlayerState<AGamePlayerState>())
		{
			playerState->GetPlayerStatus().AddFood(Amount);
			Destroy();
		}
		else
		{
			DUNGEON_GENERATOR_DEMO_ERROR(TEXT("Cannot find player state to pick up items for actor '%s'"), *GetName());
		}
	}
	else
	{
		DUNGEON_GENERATOR_DEMO_ERROR(TEXT("Specify the player who will pick up the item for actor '%s'"), *GetName());
	}
}

void ADropFoodBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(Sphere))
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ADropFoodBase::OnBeginOverlap);
	}
}

void ADropFoodBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(Sphere))
	{
		Sphere->OnComponentBeginOverlap.RemoveDynamic(this, &ADropFoodBase::OnBeginOverlap);
	}

	Super::EndPlay(EndPlayReason);
}

void ADropFoodBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PickUp(Cast<APlayerBase>(OtherActor));
}
