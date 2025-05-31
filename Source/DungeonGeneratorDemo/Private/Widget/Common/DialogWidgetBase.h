/**
@author		Shun Moriya
*/

#pragma once
#include "PartsWidgetBase.h"
#include <Delegates/IDelegateInstance.h>
#include "DialogWidgetBase.generated.h"

// イベント型定義
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogActionSignature, const int32, identifier);

/**
 * ダイアログウィジットベースクラス
 */
UCLASS(Abstract)
class UDialogWidgetBase : public UPartsWidgetBase
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UDialogWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UDialogWidgetBase() override = default;

	/*
	返事に文字列を追加します
	識別子として配列番号を設定します。個別の識別子を指定したい場合はAddAnswerを使用してください。
	*/
	UFUNCTION(BlueprintCallable)
	void AddAnswers(const TArray<FString>& answers);

	/*
	返事に文字列を追加します
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void AddAnswer(const FText& answer, const int32 identifier = 0);

	/*
	決定
	*/
	UFUNCTION(BlueprintCallable)
	void Decide(const int32 identifier);

	/*
	決定したか取得します
	*/
	UFUNCTION(BlueprintCallable)
	bool Decided() const noexcept;

	/*
	決定した識別子を取得します
	*/
	UFUNCTION(BlueprintCallable)
	int32 GetDecideIdentifier() const noexcept;

protected:
	/*
	ダイアログ決定イベント
	ボタン決定時に呼び出されます
	*/
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FDialogActionSignature OnDecided;

	// 決定時、自動的に自身を閉じるか？
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AutoClose = true;

private:
	int32 mDecideIdentifier = 0;
	bool mDecided = false;
};
