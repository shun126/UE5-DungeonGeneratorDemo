/**
@author		Shun Moriya


*/

#pragma once
#include <CoreMinimal.h>

/*
一文字ずつ表示するメッセージの書式を整えるクラス
Rich text block widgetと組み合わせて使用する事を想定しています
*/
class MessageFormatter final
{
public:
	/*
	コンストラクタ
	*/
	MessageFormatter();

	/*
	デストラクタ
	*/
	~MessageFormatter();

	/*
	メッセージの表示を開始
	*/
	void Start(const FString& message, const float interval);

	/*
	メッセージの表示を表示完了までスキップします
	*/
	void Skip();

	/*
	メッセージの表示が完了したか取得します
	*/
	bool IsEnd() const noexcept;

	/*
	解析が完了したメッセージを取得します
	*/
	const FString& GetMessage() const noexcept;

	/*
	メッセージを解析して現在表示すべきメッセージを取得します
	*/
	bool Update(const float deltaSeconds);

private:
	bool AddIndex();

private:
	FText mSourceText;
	FString mSourceMessage;
	FString mMessage;
	int32 mSourceLength = 0;
	int32 mIndex = 0;
	float mWait = 0.f;
	float mInterval = 0.5f;
};

inline MessageFormatter::MessageFormatter()
{
}

inline MessageFormatter::~MessageFormatter()
{
}

inline bool MessageFormatter::IsEnd() const noexcept
{
	return mIndex >= mSourceLength;
}

inline const FString& MessageFormatter::GetMessage() const noexcept
{
	return mMessage;
}
