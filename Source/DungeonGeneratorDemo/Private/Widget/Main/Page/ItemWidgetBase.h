/**
@author		Shun Moriya


*/

#pragma once
#include "DataAsset/ItemDataId.h"
#include "Save/Inventory.h"
#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "ItemWidgetBase.generated.h"

/**
 * 所有しているアイテム
 */
USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	EItemDataId Id;

	UPROPERTY(BlueprintReadOnly)
	FInventoryPair Inventory;
};

/**
ベースクラス
*/
UCLASS(Abstract)
class UItemWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UItemWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UItemWidgetBase() override = default;

	/**
	 * 所有しているアイテムの一覧を取得します
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FInventoryItem> GetInventory() const;

	/**
	所有アイテムの数を取得します
	*/
	UFUNCTION(BlueprintCallable)
	uint8 GetCount(const EItemDataId itemDataId) const;

	/**
	所有アイテムの熟練度を取得します
	*/
	UFUNCTION(BlueprintCallable)
	int32 GetSkill(const EItemDataId itemDataId) const;

	/**
	所有アイテムを使用します
	*/
	UFUNCTION(BlueprintCallable)
	void Use(const EItemDataId itemDataId) const;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStringTable> StringTable;
};
