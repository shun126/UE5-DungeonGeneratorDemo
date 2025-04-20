/**
@author		Shun Moriya

移動履歴
*/

#pragma once
#include <Math/IntVector.h>
#include "MovementHistoryComponent.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class UMovementHistoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/*
	コンストラクタ
	*/
	explicit UMovementHistoryComponent(const FObjectInitializer& ObjectInitializer);

	/**
	履歴をクリアします
	*/
	UFUNCTION(BlueprintCallable)
		void Clear();

	/*
	履歴サイズを変更します
	履歴はクリアされるので注意して下さい
	*/
	UFUNCTION(BlueprintCallable)
		void Resize(const int32 size);

	/*
	記録したデータのサイズを取得します
	*/
	UFUNCTION(BlueprintCallable)
		int32 Size() const;

	/*
	記録した位置を取得します
	整数型ベクターで返します。
	*/
	UFUNCTION(BlueprintCallable)
		FVector3f Get(const int32 index) const;

	/*
	アクターの位置を記録するスケールを設定します
	0以下を指定する事はできません。
	履歴はクリアされるので注意して下さい
	*/
	UFUNCTION(BlueprintCallable)
		void SetIntervalDistance(const double intervalDistance);

	/*
	記録したアクターの位置を取得します
	スケールで量子化された座標なのでSetLocationと非可逆な事に注意して下さい
	*/
	UFUNCTION(BlueprintCallable)
		FVector GetLocation(const int32 index) const;

	/*
	記録したアクターの位置を取得します
	スケールで量子化された座標なのでSetLocationと非可逆な事に注意して下さい
	*/
	UFUNCTION(BlueprintCallable)
		FVector2D GetLocation2D(const int32 index) const;

	/*
	アクターの位置を記録します
	最後に記録した位置から平行方向へ一定以上離れないと記録しません
	*/
	UFUNCTION(BlueprintCallable)
		void SetLocation(const FVector& location);

	// UActorComponent overrides
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		double IntervalDistance = 100.0;

private:
	TArray<FVector3f> mLocationHistory;
	int32 mHistoryPointer = 0;
	bool mInitialized = false;
	bool mLoop = false;
};

