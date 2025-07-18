/**
@author		Shun Moriya

https://historia.co.jp/archives/13665/
*/

#pragma once
#include "ItemDataId.h"
#include <Engine/DataAsset.h>
#include <Engine/DataTable.h>
#include "ItemDataAsset.generated.h"

class UItemDataAsset;
class UStringTable;

/**
データの登録に使うデータテーブル用構造体
*/
USTRUCT()
struct FItemTableRow : public FTableRowBase
{
	GENERATED_BODY()

	//! 売値
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 SellingPrice = 10000;

	//! メニューで使用可能？
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool CanUseInMenu = true;

	//! メニューから捨てられる？
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool CanDropFromMenu = true;

	//! 敵が落とす？
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool CanDropFromEnemy = true;

	//! 生成するアクターのクラス
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath ActorPath;
};

/**
実際にゲーム側で使うデータの構造体
*/
USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	//! シンボル名
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Name;

	//! 売値
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 SellingPrice = 10000;

	//! メニューで使用可能？
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanUseInMenu = false;

	//! メニューから捨てられる？
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanDropFromMenu = false;

	//! 敵が落とす？
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanDropFromEnemy = false;

	//! 生成するアクターのクラス
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ActorClass;

	//! 名称
	FText GetName() const;

	//! 説明
	FText GetDescription() const;

private:
	const UItemDataAsset* mOwnerAsset = nullptr;
	friend class UItemDataAsset;
};

/**
*/
UCLASS()
class UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/**
	コンストラクタ
	*/
	UItemDataAsset() = default;

	/**
	デストラクタ
	*/
	virtual ~UItemDataAsset() override = default;

	const UStringTable* GetStringTable() const;

	/**
	DataTableからDataAssetをビルドします
	*/
	UFUNCTION(meta = (CallInEditor = "true"))
	void Build();

	/**
	情報を取得します
	*/
	UFUNCTION()
	const FItemData& Get(const EItemDataId id) const;

	/**
	情報を検索します
	*/
	UFUNCTION()
	const FItemData& Find(const FString& name) const;

	/**
	無効データを取得します
	*/
	static const FItemData& Invalid();

private:
	/**
	enumヘッダーを出力します
	*/
	bool GenerateEnumHeader(const TArray<FName>& names) const;

	/**
	 * ストリングテーブルのひな型にするcsvファイルを出力します
	 */
	bool GenerateStringTableCsv(const TArray<FName>& names) const;

public:
	/**
	 * セーブ前に通知される関数
	 * Cook前のBuildに使用しています
	 */
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;

protected:
#if WITH_EDITORONLY_DATA
	//! データテーブル
	UPROPERTY(EditAnywhere)
	UDataTable* DataTable;
#endif

	// 文字列テーブル
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStringTable> StringTable;

#if WITH_EDITORONLY_DATA
	/*
	データにアクセスするENUM定義を出力するファイル名
	ソースディクトリからのパスを指定して下さい
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString OutputHeaderPath;

	/*
	ストリングテーブルのひな型を出力するファイル名
	コンテンツディクトリからのパスを指定して下さい
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString OutputStringTableCsvPath;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FItemData> Data;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, EItemDataId> Map;

private:
	//! 無効なアイテムデータ
	static FItemData mInvalidItemData;
};
