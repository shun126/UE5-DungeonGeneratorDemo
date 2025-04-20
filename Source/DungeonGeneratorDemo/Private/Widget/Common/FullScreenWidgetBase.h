/**
@author		Shun Moriya
*/

#pragma once
#include <Blueprint/UserWidget.h>
#include "FullScreenWidgetBase.generated.h"

/**
 * 全画面表示メニューのベースクラス
 * ゲームの更新を一時中断してUIの入力を許可します
 */
UCLASS(Abstract)
class UFullScreenWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * コンストラクタ
	 */
	explicit UFullScreenWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	 * デストラクタ
	 */
	virtual ~UFullScreenWidgetBase() override = default;

	/**
	 * フルスクリーンメニューの操作を開始します
	 * - ゲームの更新を一時停止します
	 * - マウスカーソルを表示します
	 * - ゲームの入力を禁止して、ＵＩへの入力のみ許可します
	 */
	UFUNCTION(BlueprintCallable)
	void StartFullScreenMenuOperation();

	/**
	 * フルスクリーンメニューの操作を終了します
	 * - ゲームの更新を再開します
	 * - マウスカーソルを消します
	 * - ＵＩへの入力を禁止してゲームへの入力を許可します
	 */
	UFUNCTION(BlueprintCallable)
	void EndFullScreenMenuOperation();

	/**
	 * 子ウィジット全ての有効性を設定します
	 */
	UFUNCTION(BlueprintCallable)
	void SetEnableChildren(const bool enable, const UWidget* withoutWidget = nullptr);

protected:
	// overrides
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	bool mFullScreenMenuOperationStarted = false;
};
