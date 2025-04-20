/**
@author		Shun Moriya



https://historia.co.jp/archives/13665/
*/

#pragma once
#include "WeaponDataId.h"
#include <Engine/DataAsset.h>
#include <Engine/DataTable.h>
#include "WeaponDataAsset.generated.h"

/**
武器毎のアニメーションタイプ
主に汎用的な戦闘を行うキャラクターのアニメーションを判定するための識別子です。
*/
UENUM(BlueprintType)
enum class EWeaponAnimationType : uint8
{
	Invalid				UMETA(DisplayName = "無効（エラー）"),
	Unarmed				UMETA(DisplayName = "素手"),
	GunAndSword			UMETA(DisplayName = "武器"),
	SingleHandedSword	UMETA(DisplayName = "大武器（片手持ち）"),
	TwoHandedSword		UMETA(DisplayName = "大武器（両手持ち"),
	Spear				UMETA(DisplayName = "槍"),
	SingleHandedGun		UMETA(DisplayName = "魔法（片手持ち）"),
	TwoHandedGun		UMETA(DisplayName = "魔法（両手持ち）"),
};

/**
データの登録に使うデータテーブル用構造体
*/
USTRUCT()
struct FWeaponTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	//! 武器タイプ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponAnimationType WeaponAnimationType = EWeaponAnimationType::GunAndSword;

	//! 魔法弾
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Bullet = false;

	//! 有効距離（間合い）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 Range = 100;

	//! 攻撃力
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 AttackPower = 1;

	//! 売値
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 SellingPrice = 10000;

	//! 生成する配置アクターのクラス
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath PlacedActorPath;

	//! 生成する装備アクターのクラス
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath EquippedActorPath;
};

/**
実際にゲーム側で使うデータの構造体
*/
USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

public:
	//! シンボル名
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Name;

	//! 武器タイプ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EWeaponAnimationType WeaponAnimationType = EWeaponAnimationType::GunAndSword;

	//! 魔法弾
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Bullet = false;

	//! 有効距離（間合い）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 Range = 100;

	//! 攻撃力
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 AttackPower = 1;

	//! 売値
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 SellingPrice = 10000;

	//! 生成する配置アクターのクラス
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> PlacedActorClass;

	//! 生成する装備アクターのクラス
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> EquippedActorClass;

	//! キー
	FString Key() const;

	//! 説明
	FString Description() const;
};

/**
*/
UCLASS()
class UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	UWeaponDataAsset() = default;

	/**
	デストラクタ
	*/
	virtual ~UWeaponDataAsset() override = default;

	/**
	DataTableからDataAssetをビルドします
	*/
	UFUNCTION(meta = (CallInEditor = "true"))
	void Build();

	/**
	情報を取得します
	*/
	UFUNCTION()
	const FWeaponData& Get(const EWeaponDataId id) const;

	/**
	情報を検索します
	*/
	UFUNCTION()
	const FWeaponData& Find(const FString& name) const;

private:
	/**
	enumヘッダーを出力します
	*/
	bool GenerateEnumHeader(const TArray<FName>& names) const;

	/**
	 * ストリングテーブルのひな型にするcsvファイルを出力します
	 */
	bool GenerateStringTableCsv(const TArray<FName>& names) const;

protected:
#if WITH_EDITORONLY_DATA
	//! データテーブル
	UPROPERTY(EditAnywhere)
	UDataTable* DataTable;

	/*
	データにアクセスするENUM定義を出力するファイル名
	ソースディクトリからの相対パスを指定して下さい
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
	TArray<FWeaponData> Data;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, EWeaponDataId> Map;

private:
	//! 無効な武器データ
	static FWeaponData mInvalidWeaponData;
};
