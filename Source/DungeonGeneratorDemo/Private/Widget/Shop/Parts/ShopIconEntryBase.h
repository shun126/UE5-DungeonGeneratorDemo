/**
@author		Shun Moriya

https://historia.co.jp/archives/10389/
*/

#pragma once
#include <Blueprint/IUserObjectListEntry.h>
#include "ShopIconEntryBase.generated.h"

UCLASS(Abstract)
class UShopIconEntryBase : public UObject
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UShopIconEntryBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UShopIconEntryBase() override = default;

	UFUNCTION(BlueprintCallable)
	UShopIconEntryBase* Set(
		const FName& stringTablePackageName,
		const int32 index,
		const FString& name,
		const int32 inventory,
		const int32 performance,
		const int32 price,
		const FString& description,
		const float skill
	);

	const FText& GetName() const noexcept;
	const FText& GetDescription() const noexcept;
	int32 GetInventory() const noexcept;
	int32 GetPrice() const noexcept;
	int32 GetPerformance() const noexcept;
	float GetSkill() const noexcept;
	int32 GetIndex() const noexcept;

protected:
	// 名称
	UPROPERTY(BlueprintReadWrite)
	FText Name;

	// 説明
	UPROPERTY(BlueprintReadWrite)
	FText Description;

	// 在庫数/所持数
	UPROPERTY(BlueprintReadWrite)
	int32 Inventory = 1;

	// 価格
	UPROPERTY(BlueprintReadWrite)
	int32 Price = 0;

	// 性能
	UPROPERTY(BlueprintReadWrite)
	int32 Performance = 0;

	// 熟練度
	UPROPERTY(BlueprintReadWrite)
	float Skill = 0;

	// 識別番号
	UPROPERTY(BlueprintReadWrite)
	int32 Index = 0;
};

inline UShopIconEntryBase::UShopIconEntryBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

inline UShopIconEntryBase* UShopIconEntryBase::Set(
	const FName& stringTablePackageName,
	const int32 index,
	const FString& name,
	const int32 inventory,
	const int32 performance,
	const int32 price,
	const FString& description,
	const float skill)
{
	Name = FText::FromStringTable(stringTablePackageName, name);
	Description = FText::FromStringTable(stringTablePackageName, description);
	Inventory = inventory;
	Price = price;
	Performance = performance;
	Skill = skill;
	Index = index;
	return const_cast<UShopIconEntryBase*>(this);
}

inline const FText& UShopIconEntryBase::GetName() const noexcept
{
	return Name;
}

inline const FText& UShopIconEntryBase::GetDescription() const noexcept
{
	return Description;
}

inline int32 UShopIconEntryBase::GetInventory() const noexcept
{
	return Inventory;
}

inline int32 UShopIconEntryBase::GetPrice() const noexcept
{
	return Price;
}

inline int32 UShopIconEntryBase::GetPerformance() const noexcept
{
	return Performance;
}

inline float UShopIconEntryBase::GetSkill() const noexcept
{
	return Skill;
}

inline int32 UShopIconEntryBase::GetIndex() const noexcept
{
	return Index;
}
