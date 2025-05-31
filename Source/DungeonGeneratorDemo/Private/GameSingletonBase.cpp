/**
@author		Shun Moriya

ゲームシングルトンクラス
GameInstanceはRuntime中のみ生存しますが、
GameSingletonクラスはRuntimeとEditor両方で存在します。

有効にする場合は、編集 → プロジェクト設定 → 基本設定のGame Singleton Classを変更してください

https://historia.co.jp/archives/6567/
*/

#include "GameSingletonBase.h"

UGameSingletonBase* UGameSingletonBase::Instance()
{
	if (GEngine)
	{
		UGameSingletonBase* instance = Cast<UGameSingletonBase>(GEngine->GameSingleton);
		return instance;
	}
	return nullptr;
}

UGameSingletonBase::UGameSingletonBase(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}
