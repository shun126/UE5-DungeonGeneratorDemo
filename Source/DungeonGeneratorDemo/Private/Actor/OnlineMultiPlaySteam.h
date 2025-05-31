/**
@author		Shun Moriya

Steamオンラインシステムアクター
*/

#pragma once
#include <GameFramework/Actor.h>
#include <Interfaces/OnlineSessionInterface.h>
#include "OnlineMultiPlaySteam.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnlineMultiPlaySteamOnCreateSessionComplete, FName, SessionName, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnlineMultiPlaySteamOnFindSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnlineMultiPlaySteamOnJoinSessionComplete, FName, SessionName, bool, Result);

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class AOnlineMultiPlaySteam : public AActor
{
	GENERATED_BODY()

public:
	explicit AOnlineMultiPlaySteam(const FObjectInitializer& objectInitializer);
	virtual ~AOnlineMultiPlaySteam() override = default;

	virtual void PreInitializeComponents() override;
	virtual void BeginDestroy() override;
	//virtual void BeginPlay() override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable)
	void CreateGameSession();

	UFUNCTION(BlueprintCallable)
	void JoinGameSession();

protected:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

protected:
	UPROPERTY(BlueprintAssignable)
	FOnlineMultiPlaySteamOnCreateSessionComplete OnCreateSessionCompleteEvent;

	UPROPERTY(BlueprintAssignable)
	FOnlineMultiPlaySteamOnFindSessionComplete OnFindSessionCompleteEvent;

	UPROPERTY(BlueprintAssignable)
	FOnlineMultiPlaySteamOnJoinSessionComplete OnJoinSessionCompleteEvent;

private:
	IOnlineSessionPtr OnlineSessionInterface;
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
