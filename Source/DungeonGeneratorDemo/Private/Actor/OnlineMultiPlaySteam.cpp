/**
@author		Shun Moriya
@copyright	2024- Shun Moriya


Steamオンラインシステムアクター
https://tech.dentsusoken.com/entry/onlinemultiplay-cpp
*/

#include "OnlineMultiPlaySteam.h"
#include <CoreMinimal.h>
#include <OnlineSubsystem.h>
#include <OnlineSessionSettings.h>
#include <Online/OnlineSessionNames.h>
#include <Templates/SharedPointer.h>

AOnlineMultiPlaySteam::AOnlineMultiPlaySteam(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
    , CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete))
	, FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete))
	, JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete))
{
}

void AOnlineMultiPlaySteam::PreInitializeComponents()
{
    Super::PreInitializeComponents();

    if (const IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get())
    {
        OnlineSessionInterface = onlineSubsystem->GetSessionInterface();
    }
}

void AOnlineMultiPlaySteam::BeginDestroy()
{
    OnlineSessionInterface.Reset();

	Super::BeginDestroy();
}

void AOnlineMultiPlaySteam::CreateGameSession()
{
    if (!OnlineSessionInterface.IsValid())
    {
        return;
    }

    // check existing session
    auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
    if (ExistingSession != nullptr)
    {
        OnlineSessionInterface->DestroySession(NAME_GameSession);
    }

    // Add Delegete variable to OnlineSessionInterface
    OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

    // Create Session Settings
    TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
    SessionSettings->bIsLANMatch = false;
    SessionSettings->NumPublicConnections = 4;
    SessionSettings->bAllowJoinInProgress = true;
    SessionSettings->bAllowJoinViaPresence = true;
    SessionSettings->bShouldAdvertise = true;
    SessionSettings->bUsesPresence = true;
    SessionSettings->bUseLobbiesIfAvailable = true;
    SessionSettings->Set(FName("MatchType"), FString("FreeForAll"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    //Create Session
    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

    OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
}

void AOnlineMultiPlaySteam::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    OnCreateSessionCompleteEvent.Broadcast(SessionName, bWasSuccessful);
}

void AOnlineMultiPlaySteam::JoinGameSession()
{
    if (!OnlineSessionInterface.IsValid())
    {
        return;
    }
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            15.f,
            FColor::Blue,
            FString::Printf(TEXT("Executed function: JoinGameSession()"))
        );
    }

    OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->MaxSearchResults = 10000;
    SessionSearch->bIsLanQuery = false;
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

void AOnlineMultiPlaySteam::OnFindSessionsComplete(bool bWasSuccessful)
{
    if (!OnlineSessionInterface.IsValid())
    {
        return;
    }

    if (bWasSuccessful)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("FindSession Complete! SearchResults.Num() = %d"), SessionSearch->SearchResults.Num()));

        for (auto Result : SessionSearch->SearchResults)
        {
            FString Id = Result.GetSessionIdStr();
            FString User = Result.Session.OwningUserName;
            FString MatchType;
            Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);

            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(
                    -1,
                    15.f,
                    FColor::Cyan,
                    FString::Printf(TEXT("Successsfully Find Session! Id: %s , OwningUser: %s"), *Id, *User)
                );
            }

            if (MatchType == FString("FreeForAll"))
            {
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(
                        -1,
                        15.f,
                        FColor::Cyan,
                        FString::Printf(TEXT("Joining Match Type: %s"), *MatchType)
                    );
                }
                OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

                const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
                OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);


                OnFindSessionCompleteEvent.Broadcast(bWasSuccessful);
            }
        }
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                15.f,
                FColor::Red,
                FString(TEXT("Failed to join session!"))
            );
        }
    }
}

void AOnlineMultiPlaySteam::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (!OnlineSessionInterface.IsValid())
    {
        return;
    }
    FString Address;
    if (OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1, 15.f, FColor::Yellow,
                FString::Printf(TEXT("Connect string: %s"), *Address)
            );
        }
        APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
        if (PlayerController)
        {
            PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
        }


        OnCreateSessionCompleteEvent.Broadcast(SessionName, Result == EOnJoinSessionCompleteResult::Success);
    }
}
