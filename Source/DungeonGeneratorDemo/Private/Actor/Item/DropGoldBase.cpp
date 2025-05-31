/**
ドロップ金基底クラス
*/

#include "DropGoldBase.h"
#include "Actor/GamePlayerState.h"
#include "Actor/PlayerBase.h"
#include "Helper/Log.h"
#include "GameInstanceBase.h"

ADropGoldBase::ADropGoldBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FText ADropGoldBase::GetLabel_Implementation() const
{
	if (ItemDataId != EItemDataId::Invalid)
	{
		if (const auto gameInstance = UGameInstanceBase::Instance())
			return gameInstance->GetItemDataAsset().Get(ItemDataId).GetName();
	}

	return FText();
}

void ADropGoldBase::PickUp(const APawn* finderPawn)
{
	if (IsValid(finderPawn))
	{
		if (auto* playerState = finderPawn->GetPlayerState<AGamePlayerState>())
		{
			playerState->GetPlayerStatus().AddGold(Amount);
			Destroy();
		}
		else
		{
			DUNGEON_GENERATOR_DEMO_ERROR(TEXT("金を拾うプレイヤーステートが見つかりません"));
		}
	}
	else
	{
		DUNGEON_GENERATOR_DEMO_ERROR(TEXT("金を拾うプレイヤーを指定して下さい"));
	}
}

void ADropGoldBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(Sphere))
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ADropGoldBase::OnBeginOverlap);
	}
}

void ADropGoldBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(Sphere))
	{
		Sphere->OnComponentBeginOverlap.RemoveDynamic(this, &ADropGoldBase::OnBeginOverlap);
	}

	Super::EndPlay(EndPlayReason);
}

void ADropGoldBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PickUp(Cast<APlayerBase>(OtherActor));
}
