/**
@author		Shun Moriya
*/

#pragma once
#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "GuildWidgetBase.generated.h"

// 前方宣言

/**
宿屋ベースクラス
*/
UCLASS(Abstract)
class UGuildWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UGuildWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UGuildWidgetBase() override = default;

	/*
	宿泊料金を取得します
	レベルによって料金が上がります
	*/
	/*
	宿泊します
	*/

	/*
	食料を購入します
	*/
};
