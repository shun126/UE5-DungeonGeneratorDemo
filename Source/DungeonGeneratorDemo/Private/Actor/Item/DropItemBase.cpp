/**
ドロップアイテム基底クラス
*/

#include "DropItemBase.h"
#include "Actor/GamePlayerState.h"
#include "Actor/PlayerBase.h"
#include "Helper/Log.h"
#include "../../GameInstanceBase.h"

ADropItemBase::ADropItemBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

EItemDataId ADropItemBase::GetId() const
{
	return ItemDataId;
}

void ADropItemBase::SetId(const EItemDataId itemDataId)
{
	ItemDataId = itemDataId;
}

FString ADropItemBase::GetStringTableKey_Implementation() const
{
	if (ItemDataId != EItemDataId::Invalid)
	{
		if (const auto gameInstance = UGameInstanceBase::Instance())
			return gameInstance->GetItemDataAsset().Get(ItemDataId).Key();
	}

	return TEXT("");
}

void ADropItemBase::PickUp(const APawn* finderPawn)
{
	if (ItemDataId != EItemDataId::Invalid)
	{
		if (IsValid(finderPawn))
		{
			if (auto* playerState = finderPawn->GetPlayerState<AGamePlayerState>())
			{
				// TODO: FPlayerStatus内で操作してください
				FInventory& inventory = playerState->GetPlayerStatus().GetInventory();
				inventory.Item[static_cast<size_t>(ItemDataId)].Count += 1;
				Destroy();
			}
			else
			{
				DUNGEON_GENERATOR_DEMO_ERROR(TEXT("アイテムを拾うプレイヤーステートが見つかりません"));
			}
		}
		else
		{
			DUNGEON_GENERATOR_DEMO_ERROR(TEXT("アイテムを拾うプレイヤーを指定して下さい"));
		}
	}
	else
	{
		DUNGEON_GENERATOR_DEMO_ERROR(TEXT("ItemDataIdを設定して下さい"));
	}
}

void ADropItemBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(Sphere))
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ADropItemBase::OnBeginOverlap);
	}
}

void ADropItemBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(Sphere))
	{
		Sphere->OnComponentBeginOverlap.RemoveDynamic(this, &ADropItemBase::OnBeginOverlap);
	}

	Super::EndPlay(EndPlayReason);
}

void ADropItemBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PickUp(Cast<APlayerBase>(OtherActor));
}
