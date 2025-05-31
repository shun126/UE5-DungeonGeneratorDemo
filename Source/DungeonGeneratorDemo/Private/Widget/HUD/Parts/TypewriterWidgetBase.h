/**
@author		Shun Moriya


*/

#pragma once
#include "Helper/MessageFormatter.h"
#include <Blueprint/UserWidget.h>
#include "TypewriterWidgetBase.generated.h"

/**
メッセージウィジットベースクラス
*/
UCLASS(Abstract)
class UTypewriterWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UTypewriterWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UTypewriterWidgetBase() override = default;

	/*
	メッセージの表示を開始
	*/
	UFUNCTION(BlueprintCallable)
		void StartMessage(const FString& message, const float interval = 0.1f);

	/*
	メッセージの表示を表示完了までスキップします
	*/
	UFUNCTION(BlueprintCallable)
		void SkipMessage();

	/*
	メッセージの表示が完了したか取得します
	*/
	UFUNCTION(BlueprintCallable)
		bool IsEndMessage() const noexcept;

	/*
	解析が完了したメッセージを取得します
	*/
	UFUNCTION(BlueprintCallable)
		const FString& GetMessage() const noexcept;

protected:
	UFUNCTION(BlueprintImplementableEvent)
		void OnMessageChanged(const FString& message);

protected:
	// overrides
	virtual void NativeTick(const FGeometry& myGeometry, float deltaTime) override;

protected:
	
private:
	MessageFormatter mMessageFormatter;
};
