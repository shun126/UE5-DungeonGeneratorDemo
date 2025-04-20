/**
@author		Shun Moriya

ゲームシングルトンクラス
GameInstanceはRuntime中のみ生存しますが、
GameSingletonクラスはRuntimeとEditor両方で存在します。

有効にする場合は、編集 → プロジェクト設定 → 基本設定のGame Singleton Classを変更してください
*/

#pragma once
#include <CoreMinimal.h>
#include "GameSingletonBase.generated.h"

UCLASS()
class UGameSingletonBase : public UObject
{
	GENERATED_BODY()

public:
	/**
	UGameSingletonBaseインスタンスを取得します
	\return		UGameSingletonBase
	*/
	static UGameSingletonBase* Instance();

	/*!
	 * コンストラクタ
	 */
	UGameSingletonBase(const FObjectInitializer& objectInitializer);

	/*!
	 * デストラクタ
	 */
	virtual ~UGameSingletonBase() override = default;
};
