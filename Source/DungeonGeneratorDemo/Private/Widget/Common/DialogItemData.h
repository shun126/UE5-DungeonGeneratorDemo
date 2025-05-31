/**
@author		Shun Moriya

https://historia.co.jp/archives/10389/
*/

#pragma once
#include <CoreMinimal.h>
#include "DialogItemData.generated.h"

/**
 * UDialogWidgetBaseとリストボックス登録用クラス
 */
UCLASS(Abstract)
class UDialogItemData : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * コンストラクタ
	 */
	explicit UDialogItemData(const FObjectInitializer& ObjectInitializer);

	/**
	 * デストラクタ
	 */
	virtual ~UDialogItemData() override = default;

	/**
	 * 文字列テーブルとテーブル名、識別子を設定します
	 */
	UFUNCTION(BlueprintCallable)
	UDialogItemData* Set(const FName& stringTablePackageName, const FString& tableName, const int32 identifier);

	/**
	 * 名前を取得します
	 */
	const FText& GetName() const noexcept;

	/**
	 * 識別子を取得します
	 */
	int32 GetIdentifier() const noexcept;

protected:
	// 名称
	UPROPERTY(BlueprintReadWrite)
	FText Name;

	// 識別番号
	UPROPERTY(BlueprintReadWrite)
	int32 Identifier = 0;
};
