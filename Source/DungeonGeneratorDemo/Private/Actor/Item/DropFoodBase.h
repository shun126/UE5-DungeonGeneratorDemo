/**
ドロップ食料基底クラス
*/

#pragma once
#include "Stockable.h"
#include "DataAsset/ItemDataId.h"
#include "DropFoodBase.generated.h"

/**
食料基底クラス
*/
UCLASS(Abstract)
class ADropFoodBase : public AStockable
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit ADropFoodBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~ADropFoodBase() override = default;

	/**
	この食料を拾う
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
	// ラベル名のキー
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString StringTableKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemDataId ItemDataId = EItemDataId::Invalid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 Amount = 100;
};
