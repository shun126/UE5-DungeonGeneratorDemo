/**
ドロップアイテム基底クラス
*/

#pragma once
#include "Stockable.h"
#include "DataAsset/ItemDataId.h"
#include "DropItemBase.generated.h"

/**
アイテム基底クラス
*/
UCLASS(Abstract)
class ADropItemBase : public AStockable
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit ADropItemBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~ADropItemBase() override = default;

	/**
	アイテムIDを取得します
	*/
	EItemDataId GetId() const;

	/**
	アイテムIDを設定します
	*/
	void SetId(const EItemDataId itemDataId);

	/**
	このアイテムを拾って所持品に加える
	\param[in]	finderPawn		拾ったポーン
	*/
	UFUNCTION()
	void PickUp(const APawn* finderPawn);

	// UAvatarComponent overrides
	virtual FString GetStringTableKey_Implementation() const override;

	// AActor overrides
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemDataId ItemDataId = EItemDataId::Invalid;
};
