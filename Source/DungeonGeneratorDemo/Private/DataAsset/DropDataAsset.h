/**
@author		Shun Moriya
https://historia.co.jp/archives/13665/
*/

#pragma once
#include "DropDataId.h"
#include "Actor/Item/Stockable.h"
#include <Engine/DataAsset.h>
#include <Engine/DataTable.h>
#include "DropDataAsset.generated.h"

/**
実際にゲーム側で使うデータの構造体

FActorGeneratorRecordと内容を合わせて下さい
*/
USTRUCT(BlueprintType)
struct FDropRate
{
	GENERATED_BODY()

	//! ドロップする物
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AStockable> DropObject;

	//! ドロップ率
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0, ClampMax = 1))
	float Rate = 1.f;
};

/**
データの登録に使うデータテーブル用構造体
*/
USTRUCT()
struct FDropTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDropRate> DropObjects;
};

/**
実際にゲーム側で使うデータの構造体

FActorGeneratorRecordと内容を合わせて下さい
*/
USTRUCT(BlueprintType)
struct FDropData
{
	GENERATED_BODY()

public:
	//! シンボル名
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDropRate> DropObjects;
};

/**
*/
UCLASS()
class UDropDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	UDropDataAsset() = default;

	/**
	デストラクタ
	*/
	virtual ~UDropDataAsset() override = default;

	/**
	DataTableからDataAssetをビルドします
	*/
	UFUNCTION(meta = (CallInEditor = "true"))
	void Build();

	/**
	情報を取得します
	*/
	UFUNCTION()
	const FDropData& Get(const EDropDataId id) const;

	/**
	情報を検索します
	*/
	UFUNCTION()
	const FDropData& Find(const FString& name) const;

	/**
	アイテムを抽選します
	*/
	UFUNCTION()
	TSubclassOf<AStockable> Draw(const EDropDataId id) const;

	/**
	 * セーブ前に通知される関数
	 * Cook前のBuildに使用しています
	 */
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;

private:
	/**
	enumヘッダーを出力します
	*/
	bool GenerateEnumHeader(const TArray<FName>& names) const;

protected:
#if WITH_EDITORONLY_DATA
	//! データテーブル
	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* DataTable;

	/*
	データにアクセスするENUM定義を出力するファイル名
	ソースディクトリからの相対パスを指定して下さい
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString OutputHeaderPath;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDropData> Data;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, EDropDataId> Map;

private:
	//! 無効なアイテムドロップデータ
	static FDropData mInvalidDropData;
};
