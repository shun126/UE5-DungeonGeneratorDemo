/**
@author		Shun Moriya


*/

#pragma once
#include "DataAsset/ItemDataId.h"
#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "ItemIconWidgetBase.generated.h"

/**
ベースクラス
*/
UCLASS(Abstract)
class UItemIconWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UItemIconWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UItemIconWidgetBase() override = default;

	/**
	登録したストリングテーブルのIDとを所有アイテム名のストリングテーブルのキー取得します
	\return		登録したストリングテーブルのID
	*/
	UFUNCTION(BlueprintCallable)
	void GetStringTableNameAndKey(FName& name, FString& key) const;

	/**
	生成するアクターのクラス
	*/
	UFUNCTION(BlueprintCallable)
	TSubclassOf<AActor> GetActorClass() const;

	/**
	名称を取得します
	*/
	UFUNCTION(BlueprintPure)
	FText GetLabel() const;

	/**
	所有アイテムの数を取得します
	*/
	UFUNCTION(BlueprintCallable)
	int32 GetCount() const;

	/**
	所有アイテムの熟練度を取得します
	*/
	UFUNCTION(BlueprintCallable)
	int32 GetSkill() const;

	/**
	所有アイテムを使用します
	*/
	UFUNCTION(BlueprintCallable)
	void Use() const;

	/**
	所有アイテムを使用できるか取得します
	*/
	UFUNCTION(BlueprintCallable)
	bool CanUse() const;

	/**
	所有アイテムを捨てる
	*/
	UFUNCTION(BlueprintCallable)
	void Drop() const;

	/**
	所有アイテムを捨てられるか取得します
	*/
	UFUNCTION(BlueprintCallable)
	bool CanDrop() const;

protected:
	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "StringTable"))
	FSoftObjectPath StringTablePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemDataId ItemDataId = EItemDataId::Invalid;
};
