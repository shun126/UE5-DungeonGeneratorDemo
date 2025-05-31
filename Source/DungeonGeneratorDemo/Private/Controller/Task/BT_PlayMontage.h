/**
@author		Shun Moriya
*/

#pragma once
#include <BehaviorTree/BTTaskNode.h>
#include "BT_PlayMontage.generated.h"

/*
メンバー変数で指定されているモンタージュを再生するタスク
*/
UCLASS()
class UBT_PlayMontage : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBT_PlayMontage() = default;
	virtual ~UBT_PlayMontage() override = default;

	// override
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/**
	Montageを再生します
	\param[in]	character				再生するACharacterオブジェクト
	\param[in]	montage					再生するUAnimMontageオブジェクト
	\param[in]	playRate				再生速度
	\param[in]	inTimeToStartMontageAt	開始時間
	\param[in]	stopAllMontages			他のモンタージュの再生を停止するか？
	\return		モンタージュ時間
	*/
	float Play(const ACharacter* character, UAnimMontage* montage, float playRate = 1.f, float inTimeToStartMontageAt = 0.f, bool stopAllMontages = true);

	/**
	Montageを再生します
	\param[in]	character				再生するACharacterオブジェクト
	\param[in]	montage					再生するUAnimMontageオブジェクト
	\param[in]	sectionName				再生するセクション名
	\param[in]	playRate				再生速度
	\param[in]	inTimeToStartMontageAt	開始時間
	\param[in]	stopAllMontages			他のモンタージュの再生を停止するか？
	\return		モンタージュ時間
	*/
	float Play(const ACharacter* character, UAnimMontage* montage, FName sectionName, float playRate = 1.f, float inTimeToStartMontageAt = 0.f, bool stopAllMontages = true);

	/**
	内部でMontageを生成してAnimSequenceを再生します
	\param[in]	character				再生するACharacterオブジェクト
	\param[in]	animSequence			再生するUAnimSequenceBaseオブジェクト
	\param[in]	slotNodeName			スロットノード名
	\param[in]	blendInTime				開始ブレンド時間
	\param[in]	blendOutTime			終了ブレンド時間
	\param[in]	inPlayRate				再生速度
	\param[in]	loopCount				再生回数
	\param[in]	blendOutTriggerTime
	\param[in]	inTimeToStartMontageAt	開始時間
	\return		モンタージュ時間
	*/
	float Play(const ACharacter* character, UAnimSequence* animSequence, FName slotNodeName, const float blendInTime = 0.25f, const float blendOutTime = 0.25f, const float inPlayRate = 1.f, const int32 loopCount = 1, const float blendOutTriggerTime = -1.f, const float inTimeToStartMontageAt = 0.f);

	float Play(const ACharacter* character, UAnimMontage* montage, const FRandomStream& random, float playRate = 1.f, float inTimeToStartMontageAt = 0.f, bool stopAllMontages = true);

	/**
	Get Current Montage Position
	*/
	float GetPosition() const;

	/**
	Montageの長さを取得します
	\return		Montageの長さ
	*/
	float GetLength() const;

	/**
	Montageのブレンドアウトが開始する時間を取得します
	\return		Montageのブレンドアウトが開始する時間
	*/
	float GetBlendOutTriggerTime() const;

	/** Get PlayRate for Montage.
	If Montage reference is NULL, PlayRate for any Active Montage will be returned.
	If Montage is not playing, 0 is returned. */
	float GetPlayRate() const;

	/** Get PlayRate for Montage.
	If Montage reference is NULL, PlayRate for any Active Montage will be returned.
	If Montage is not playing, 0 is returned. */
	void SetPlayRate(const float rate);

	/**
	Montageのセクションの時間を取得します
	\param[out]	outStartTime	開始時間
	\param[out]	outEndTime		終了時間
	\param[in]	sectionName		セクション名
	\return		trueならば取得成功
	*/
	bool GetSectionStartAndEndTime(float& outStartTime, float& outEndTime, const FName& sectionName) const;

	/**
	Montageの再生が既に終了している。または、次のフレームで終了するか？
	\param[in]	deltaSecond		経過時間
	\return		trueならばアニメーションの再生は終了とみなす
	*/
	bool IsFinish(const float deltaSecond) const;

	/**
	Montageの再生が既に終了しているか？
	\return		trueならばアニメーションの再生は終了している
	*/
	bool IsFinish() const;

	/**
	全てのMontageの再生が既に終了しているか？
	\return		trueならばアニメーションの再生は終了している
	*/
	bool IsFinishAll() const;

	/**
	Montageが再生中か？
	\return		trueならばアニメーションの再生中
	*/
	bool IsPlaying() const;

	/**
	Montageの再生時間か？
	\param[in]	time			再生時間
	\param[in]	deltaSecond		経過時間
	\return		trueならばアニメーションの時間
	*/
	bool IsOnTime(const float time, const float deltaSecond) const;

	/**
	再生中のセクション名を取得します
	\return		セクション名
	*/
	FName GetCurrentSection() const;

	/**
	内部状態をリセットします
	*/
	void Stop(const float blendOutTime);

	/**
	内部状態をリセットします
	*/
	void Reset(const float blendOutTime = 0.1f);

	/**
	再生中のUAnimMontageを取得します
	*/
	UAnimMontage* GetCurrentMontage();

	/**
	再生中のUAnimMontageを取得します
	*/
	const UAnimMontage* GetCurrentMontage() const;

	/**
	再生中のUAnimInstanceを取得します
	*/
	UAnimInstance* GetCurrentAnimInstance();

	/**
	再生中のUAnimInstanceを取得します
	*/
	const UAnimInstance* GetCurrentAnimInstance() const;

	/**
	 * ルートモーションが有効か取得します
	 */
	bool IsRootMotionEnabled() const;

protected:
	/*
	 * Specify the Montage to play
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> Animation;

	//! 再生中のUAnimMontageオブジェクト（AddReferencedObjects経由でGCに登録）
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAnimMontage> CurrentMontage;

	//! 再生中のUAnimInstanceオブジェクト（AddReferencedObjects経由でGCに登録）
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAnimInstance> CurrentAnimInstance;

private:
	float mLength = 0.f;
	float mInPlayRate = 1.f;
};
