/**
在庫可能なアイテムのクラス ヘッダーファイル
*/

#pragma once
#include <CoreMinimal.h>
#include <Components/SphereComponent.h>
#include <Math/RandomStream.h>
#include "Stockable.generated.h"

// 前方宣言
class UStaticMeshComponent;
class UStringTable;
class UWidgetComponent;

/**
在庫可能なアイテムのクラス
レベルエディタで静的に配置、ドロップによる動的に配置が可能な小道具アクター
このクラスは抽象化クラスです。生成は派生されたクラスを指定してください。
*/
UCLASS(Abstract)
class AStockable : public AActor
{
	GENERATED_BODY()

protected:
	/**
	コンストラクタ
	*/
	explicit AStockable(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~AStockable() override = default;

public:
	/**
	装備された時に呼び出されます
	DoActionComponent,WidgetComponentを非アクティブ化してStaticMeshComponentのコリジョンを無効化します
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Equip(const bool playerAttack);
	virtual void Equip_Implementation(const bool playerAttack);

	/**
	装備解除された時に呼び出されます
	DoActionComponent,WidgetComponentをアクティブ化してStaticMeshComponentのコリジョンを有効化します
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnEquip();
	virtual void UnEquip_Implementation();

	/**
	メッシュを取得します
	*/
	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetMesh();

	/**
	出現時の激力
	\return		激力（質量を考慮していない）
	*/
	UFUNCTION(BlueprintCallable)
	FVector GetImpulseAtAppear() const;

	/**
	捨てる時の激力
	\return		激力（質量を考慮していない）
	*/
	UFUNCTION(BlueprintCallable)
	FVector GetImpulseAtDrop() const;

	/**
	登録したストリングテーブルのIDとストリングテーブルのキー取得します
	\return		登録したストリングテーブルのID
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FString GetStringTableKey() const;
	virtual FString GetStringTableKey_Implementation() const;

	// AActor overrides
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	/**
	装備された時に呼び出されます
	DoActionComponent,WidgetComponentを非アクティブ化してStaticMeshComponentのSimulatingPhysicsを無効化します
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnEquip(const bool playerAttack);

	/**
	装備解除された時に呼び出されます
	DoActionComponent,WidgetComponentをアクティブ化してStaticMeshComponentのSimulatingPhysicsを有効化します
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnUnEquip();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> Widget;

	// 文字列テーブル
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStringTable> StringTable;

	UPROPERTY(EditAnywhere)
	float TimeUntilExpiration = 30.f;

	// スポーン時にはじけ飛ぶか？
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	bool PopOnBegin = true;

private:
	FRandomStream mRandomStream;
};
