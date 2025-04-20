/**
@author		Shun Moriya


*/

#pragma once
#include "Parts/TypewriterWidgetBase.h"
#include "MessageWidgetBase.generated.h"

/**
メッセージウィジットベースクラス
*/
UCLASS(Abstract)
class UMessageWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UMessageWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UMessageWidgetBase() = default;

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

	/*
	タイプライターウィジットを取得します
	*/
	UFUNCTION(BlueprintImplementableEvent)
		UTypewriterWidgetBase* GetTypewriterWidgetBase() const;
};

inline UMessageWidgetBase::UMessageWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

inline void UMessageWidgetBase::StartMessage(const FString& message, const float interval)
{
	if (TObjectPtr<UTypewriterWidgetBase> typewriter = GetTypewriterWidgetBase())
	{
		typewriter->StartMessage(message, interval);
	}
}

inline void UMessageWidgetBase::SkipMessage()
{
	if (TObjectPtr<UTypewriterWidgetBase> typewriter = GetTypewriterWidgetBase())
	{
		typewriter->SkipMessage();
	}
}

inline bool UMessageWidgetBase::IsEndMessage() const noexcept
{
	if (TObjectPtr<UTypewriterWidgetBase> typewriter = GetTypewriterWidgetBase())
	{
		return typewriter->IsEndMessage();
	}
	return false;
}

inline const FString& UMessageWidgetBase::GetMessage() const noexcept
{
	if (TObjectPtr<UTypewriterWidgetBase> typewriter = GetTypewriterWidgetBase())
	{
		return typewriter->GetMessage();
	}

	static FString dummy;
	return dummy;
}
