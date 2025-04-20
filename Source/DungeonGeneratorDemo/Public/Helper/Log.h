/**
@author		Shun Moriya

ログ出力に関するヘッダーファイル

セーブ共通ログ出力マクロです。
ただし大量にログを出力したり、他のカテゴリに分割するとログが見やすいなど、
必要に応じてログカテゴリを分けるように努めてください。
*/

#pragma once
#include <CoreMinimal.h>

// ログカテゴリ宣言
DECLARE_LOG_CATEGORY_EXTERN(LogDungeonGenerator, Log, All);

// ログマクロ
#define DUNGEON_GENERATOR_DEMO_ERROR(Format, ...)	UE_LOG(LogDungeonGenerator, Error, Format, __VA_ARGS__)
#define DUNGEON_GENERATOR_DEMO_WARNING(Format, ...)	UE_LOG(LogDungeonGenerator, Warning, Format, __VA_ARGS__)
#define DUNGEON_GENERATOR_DEMO_LOG(Format, ...)		UE_LOG(LogDungeonGenerator, Log, Format, __VA_ARGS__)
