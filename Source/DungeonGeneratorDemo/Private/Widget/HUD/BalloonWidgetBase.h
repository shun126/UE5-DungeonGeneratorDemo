/**
@author		Shun Moriya
*/

#pragma once
#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "BalloonWidgetBase.generated.h"

/**
バルーン（吹き出し）ウィジットクラス
*/
UCLASS(Blueprintable, BlueprintType)
class UBalloonWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * コンストラクタ
	 */
	explicit UBalloonWidgetBase(const FObjectInitializer& ObjectInitializer);
	
	/**
	 * デストラクタ
	 */
	virtual ~UBalloonWidgetBase() override = default;

	/**
	ラベルを取得します
	*/
	const FText& GetLabel() const;

	/**
	ラベルを設定します
	*/
	void SetLabel(const FText& label);

	/**
	 * 更新
	 */
	void Update(const AActor* ownerActor);

protected:
	/**
	 * 噴き出しを表示する距離
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double VisibleDistance = 500;

	/**
	 * 吹き出しに表示するラベル（ローカライズに注意してください）
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Label;
};
