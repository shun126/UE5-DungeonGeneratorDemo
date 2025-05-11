/**
@author		Shun Moriya


*/

#pragma once
#include "Save/Inventory.h"
#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "WeaponWidgetBase.generated.h"

/**
 * 所有している武器
 */
USTRUCT(BlueprintType)
struct FInventoryWeapon
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	EWeaponDataId Id;

	UPROPERTY(BlueprintReadOnly)
	FInventoryPair Inventory;
};

/**
ベースクラス
*/
UCLASS(Abstract)
class UWeaponWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UWeaponWidgetBase(const FObjectInitializer& objectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UWeaponWidgetBase() override = default;

	/**
	 * 所有している武器の一覧を取得します
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FInventoryWeapon> GetInventory() const;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStringTable> StringTable;
};
