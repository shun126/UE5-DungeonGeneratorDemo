/**
@author		Shun Moriya


*/

#pragma once
#include "DataAsset/WeaponDataId.h"
#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "WeaponIconWidgetBase.generated.h"

/**
ベースクラス
*/
UCLASS(Abstract)
class UWeaponIconWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UWeaponIconWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UWeaponIconWidgetBase() override = default;

	/**
	登録したストリングテーブルのIDとを所有武器名のストリングテーブルのキー取得します
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
	所有武器の数を取得します
	*/
	UFUNCTION(BlueprintPure)
	int32 GetCount() const;

	/**
	武器を持っているか？
	*/
	UFUNCTION(BlueprintPure)
	bool Has() const;

	/**
	所有武器の攻撃力を取得します
	*/
	UFUNCTION(BlueprintPure)
	int32 GetAttackPower() const;

	/**
	所有武器の熟練度を取得します
	*/
	UFUNCTION(BlueprintPure)
	int32 GetSkill() const;

	/**
	所有武器を装備します
	*/
	UFUNCTION(BlueprintCallable)
	void Equip() const;

	/**
	所有武器を装備できるか取得します
	*/
	UFUNCTION(BlueprintPure)
	bool CanEquip() const;

	/**
	所有武器を装備します
	*/
	UFUNCTION(BlueprintCallable)
	void UnEquip() const;

	/**
	所有武器を装備できるか取得します
	*/
	UFUNCTION(BlueprintPure)
	bool CanUnEquip() const;

	/**
	所有武器を捨てる
	*/
	UFUNCTION(BlueprintCallable)
	void Drop() const;

	/**
	所有武器を捨てられるか取得します
	*/
	UFUNCTION(BlueprintPure)
	bool CanDrop() const;

protected:
	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "StringTable"))
	FSoftObjectPath StringTablePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponDataId WeaponDataId = EWeaponDataId::Invalid;
};
