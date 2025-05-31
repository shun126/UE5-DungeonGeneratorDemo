/**
@author		Shun Moriya


*/

#include "MessageFormatter.h"
#include <stack>

void MessageFormatter::Start(const FString& message, const float interval)
{
	mSourceMessage = message;
	mSourceLength = mSourceMessage.Len();
	mMessage.Reset();
	mIndex = 0;
	mInterval = interval;
	mWait = mInterval;
}

void MessageFormatter::Skip()
{
	/*
	ボタン待ちが入るようであれば、ボタン待ちまでスキップする事を検討してください
	*/
	mMessage = mSourceMessage;
	mIndex = mSourceLength;
}

bool MessageFormatter::Update(const float deltaSeconds)
{
	static_assert(sizeof(TCHAR) == sizeof(WIDECHAR));
	bool result = false;
	mWait += deltaSeconds;
	while (mWait > mInterval)
	{
		mWait -= mInterval;
		if (AddIndex())
			result = true;
	}
	return result;
}

bool MessageFormatter::AddIndex()
{
	static_assert(sizeof(TCHAR) == sizeof(WIDECHAR));

	if (mIndex >= mSourceLength)
		return false;

	// 一文字進める
	++mIndex;

	// 出力メッセージをリセット
	mMessage.Reset();

	// メッセージを解析
	std::stack<FString> styles;
	int32 i = 0;
	while (i < mIndex)
	{
		mMessage += mSourceMessage[i];

		if (mSourceMessage[i] == '<')
		{
			// スタイル指定開始
			FString styleName;
			++i;
			while (i < mSourceLength)
			{
				styleName += mSourceMessage[i];
				if (mSourceMessage[i] == '>')
				{
					// スタイルを確定して終了
					if (styleName == TEXT("/>"))
						styles.pop();
					else
						styles.emplace(styleName);

					mMessage += styleName;
					++i;

					if (mIndex < i)
						mIndex = i;
					break;
				}
				else
				{
					styleName += mSourceMessage[i];
					++i;
				}
			}
		}
		else
		{
			// 次の文字へ進む
			++i;
		}
	}

	// 開いているスタイルを強制的に閉じる
	while (!styles.empty())
	{
		styles.pop();
		mMessage += TEXT("</>");
	}

	return true;
}
