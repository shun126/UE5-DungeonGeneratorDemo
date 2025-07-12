/**
 * @author Shun Moriya
 */

#pragma once
#include <CoreMinimal.h>
#include <Components/SphereComponent.h>
#include <Math/RandomStream.h>
#include "Stockable.generated.h"

class UStaticMeshComponent;
class UWidgetComponent;

/**
 * Classes of items that can be stocked
 * A prop actor that can be placed statically in the level editor and dynamically by drop
 * This class is an abstraction class.Generation must specify derived class.
 * 在庫可能なアイテムのクラス
 * レベルエディタで静的に配置、ドロップによる動的に配置が可能な小道具アクター
 * このクラスは抽象化クラスです。生成は派生されたクラスを指定してください。
 */
UCLASS(Abstract)
class AStockable : public AActor
{
	GENERATED_BODY()

protected:
	/**
	 * constructor
	 * コンストラクタ
	 */
	explicit AStockable(const FObjectInitializer& ObjectInitializer);

	/**
	 * destructor
	 * デストラクタ
	 */
	virtual ~AStockable() override = default;

public:
	/**
	 * Called when equipped
	 * deactivates DoActionComponent,WidgetComponent and disables StaticMeshComponent collision
	 * 装備された時に呼び出されます
	 * DoActionComponent,WidgetComponentを非アクティブ化してStaticMeshComponentのコリジョンを無効化します
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Equip(const bool playerAttack);
	virtual void Equip_Implementation(const bool playerAttack);

	/**
	 * Called when unequipped
	 * Activates DoActionComponent,WidgetComponent and enables collision of StaticMeshComponent
	 * 装備解除された時に呼び出されます
	 * DoActionComponent,WidgetComponentをアクティブ化してStaticMeshComponentのコリジョンを有効化します
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnEquip();
	virtual void UnEquip_Implementation();

	/**
	 * Get the mesh
	 * メッシュを取得します
	 */
	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetMesh();

	/**
	 * Impulse when it appears
	 * @return Impulse(not considering mass)
	 * 出現時の激力
	 * @return 激力（質量を考慮していない）
	 */
	UFUNCTION(BlueprintCallable)
	FVector GetImpulseAtAppear() const;

	/**
	 * Intense force when discarding
	 * @return Impulse(not considering mass)
	 * 捨てる時の激力
	 * @return 激力（質量を考慮していない）
	 */
	UFUNCTION(BlueprintCallable)
	FVector GetImpulseAtDrop() const;

	/**
	 * 登録したストリングテーブルのIDとストリングテーブルのキー取得します
	 * @return 登録したストリングテーブルのID
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FText GetLabel() const;
	virtual FText GetLabel_Implementation() const;

	// AActor overrides
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	/**
	 * Called when equipped
	 * Deactivates DoActionComponent,WidgetComponent and disables SimulatingPhysics of StaticMeshComponent
	 * 装備された時に呼び出されます
	 * DoActionComponent,WidgetComponentを非アクティブ化してStaticMeshComponentのSimulatingPhysicsを無効化します
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnEquip(const bool playerAttack);

	/**
	 * Called when unequipped
	 * Activates DoActionComponent,WidgetComponent and enables SimulatingPhysics of StaticMeshComponent
	 * 装備解除された時に呼び出されます
	 * DoActionComponent,WidgetComponentをアクティブ化してStaticMeshComponentのSimulatingPhysicsを有効化します
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnUnEquip();

	/**
	 * Sensor for acquisition determination
	 * 取得判定用のセンサー
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> Sphere;

	/**
	 * Mesh for display
	 * 表示用のメッシュ
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Mesh;

	/**
	 * Widget to display name as HUD
	 * 名前をHUDとして表示する為のウィジット
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> Widget;

	/**
	 * Effective time. Deleted when expired.
	 * 有効時間。失効すると削除される
	 */
	UPROPERTY(EditAnywhere)
	float TimeUntilExpiration = 30.f;

	/**
	 * Pop at spawn?
	 * スポーン時にはじけ飛ぶか？
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	bool PopOnBegin = true;

private:
	FRandomStream mRandomStream;
};
