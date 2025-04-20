/**
@author		Shun Moriya

敵の出現確率
*/

#pragma once
#include "EnemyAppearanceAsset.generated.h"

/**
敵の出現確率
*/
USTRUCT(BlueprintType)
struct DUNGEONGENERATORDEMO_API FEnemyAppearanceData
{
	GENERATED_BODY()

	/*
	 * 出現優先度
	 * 0ならスタート部屋から出現して、1に近づくほど奥の部屋にしか出現しない
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DepthRatio = 0.f;

	// 出現する敵アクター
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "Actor, Blueprint"))
	FSoftObjectPath EnemyAvatarPath = nullptr;

	// 出現する敵アクター
	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowedClasses = "Actor"))
	TSubclassOf<AActor> EnemyAvatarClass = nullptr;

	// 出現する敵アクタークラスを取得します
	void LoadEnemyAvatarClass();

	// 出現確率
	float GetPriority(const float depthFromStartRatio) const;
};

/**
敵の出現確率テーブル
*/
UCLASS(Blueprintable)
class DUNGEONGENERATORDEMO_API UEnemyAppearanceAsset : public UObject
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UEnemyAppearanceAsset(const FObjectInitializer& initializer);

	/**
	デストラクタ
	*/
	virtual ~UEnemyAppearanceAsset() override = default;

	/**
	敵の情報を取得します
	*/
	UFUNCTION(BlueprintCallable)
	const FEnemyAppearanceData& DrawEnemy(const float depthFromStartRatio) const;

private:
	/**
	情報を取得します
	*/
	const FEnemyAppearanceData& Get(const int32 index) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FEnemyAppearanceData> EnemyAppearanceData;
};
