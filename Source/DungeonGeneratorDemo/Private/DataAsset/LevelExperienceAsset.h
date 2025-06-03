/**
@author		Shun Moriya

レベルアップに必要な経験値
*/

#pragma once
#include <CoreMinimal.h>
#include <Engine/DataAsset.h>
#include <Engine/DataTable.h>
#include "LevelExperienceAsset.generated.h"

class ULevelExperienceAsset;
class UStringTable;

/**
データの登録に使うデータテーブル用構造体
*/
USTRUCT()
struct FLevelExperienceTableRow : public FTableRowBase
{
	GENERATED_BODY()

	// レベルアップに必要な経験値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 Experience = 10;

	// 宿泊料
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 RoomCharges = 10;
};

/**
実際にゲーム側で使うデータの構造体
*/
USTRUCT(BlueprintType)
struct FLevelExperienceData
{
	GENERATED_BODY()

	//! シンボル名
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Name;

	// レベルアップに必要な経験値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 Experience = 10;

	// 宿泊料
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 RoomCharges = 10;

	//! 称号
	FText GetTitle() const;

private:
	const ULevelExperienceAsset* mOwnerAsset = nullptr;
	friend class ULevelExperienceAsset;
};

/**
レベルアップに必要な経験値テーブル
*/
UCLASS()
class  ULevelExperienceAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	ULevelExperienceAsset() = default;

	/**
	デストラクタ
	*/
	virtual ~ULevelExperienceAsset() override = default;

	const UStringTable* GetStringTable() const;

	/**
	DataTableからDataAssetをビルドします
	*/
	UFUNCTION(meta = (CallInEditor = "true"))
	void Build();

	/**
	レベルアップに必要な経験値を取得します
	*/
	UFUNCTION(BlueprintCallable)
	const FLevelExperienceData& Get(const int32 index) const;

private:
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
	ストリングテーブルのひな型を出力するファイル名
	コンテンツディクトリからのパスを指定して下さい
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString OutputStringTableCsvPath;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FLevelExperienceData> Data;
};
