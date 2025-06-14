/**
 * @author		Shun Moriya
 */

#pragma once
#include "DataAsset/ItemDataId.h"
#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "ItemIconWidgetBase.generated.h"

/**
 * Base class for the item icon widget in the item menu
 * Use and destroy player's items.
 * 
 * アイテムメニュー内のアイテムアイコンウィジットのベースクラス
 * プレイヤーのアイテムの使用と破棄を行います。
*/
UCLASS(Abstract)
class UItemIconWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * constructor
	 * コンストラクタ
	 */
	explicit UItemIconWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	 * destructor
	 * デストラクタ
	 */
	virtual ~UItemIconWidgetBase() override = default;

	/**
	 * Class of actor to be generated
	 * 生成するアクターのクラス
	 */
	UFUNCTION(BlueprintCallable)
	TSubclassOf<AActor> GetActorClass() const;

	/**
	 * Get Name
	 * 名称を取得します
	 */
	UFUNCTION(BlueprintPure)
	FText GetLabel() const;

	/**
	 * Retrieves the number of items owned
	 * 所有アイテムの数を取得します
	 */
	UFUNCTION(BlueprintCallable)
	int32 GetCount() const;

	/**
	 * Acquire proficiency in owned items
	 * 所有アイテムの熟練度を取得します
	 */
	UFUNCTION(BlueprintCallable)
	int32 GetSkill() const;

	/**
	 * Use owned items
	 * 所有アイテムを使用します
	 */
	UFUNCTION(BlueprintCallable)
	void Use() const;

	/**
	 * You will be notified when an item you own is about to be used
	 * 所有アイテムを使用する時に通知されます
	 *
	 * @param skillRate 熟練度
	 * @return trueならば個数を減らし、熟練度を1増価させます
	 */
	UFUNCTION(BlueprintImplementableEvent)
	bool OnUse(float skillRate) const;

	/**
	 * Retrieves whether the owned item can be used
	 * 所有アイテムを使用できるか取得します
	 */
	UFUNCTION(BlueprintCallable)
	bool CanUse() const;

	/**
	 * Discard owned items
	 * 所有アイテムを捨てる
	 */
	UFUNCTION(BlueprintCallable)
	void Drop() const;

	/**
	 * Find out if you can throw away items you own.
	 * 所有アイテムを捨てられるか調べます
	 */
	UFUNCTION(BlueprintCallable)
	bool CanDrop() const;

protected:
	/**
	 * Item Identification Number
	 * アイテム識別番号
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemDataId ItemDataId = EItemDataId::Invalid;
};
