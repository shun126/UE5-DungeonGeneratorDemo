/**
 * ドロップ金基底クラス
 */

#pragma once
#include "Stockable.h"
#include "DataAsset/ItemDataId.h"
#include "DropGoldBase.generated.h"

/**
 * 金基底クラス
 */
UCLASS(Abstract)
class ADropGoldBase : public AStockable
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit ADropGoldBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~ADropGoldBase() override = default;

	/**
	この金を拾って財布に入れる
	\param[in]	finderPawn		拾ったポーン
	*/
	UFUNCTION()
	void PickUp(const APawn* finderPawn);

	// UAvatarComponent overrides
	virtual FText GetLabel_Implementation() const override;

	// AActor overrides
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// ラベル名のキー
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString StringTableKey;

	/**
	 * 量
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 Amount = 1;
};
