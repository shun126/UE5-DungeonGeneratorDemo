// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonGeneratorDemoGameMode.h"
#include "DungeonGeneratorDemoGameState.h"
#include "DungeonGeneratorDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADungeonGeneratorDemoGameMode::ADungeonGeneratorDemoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	GameStateClass = ADungeonGeneratorDemoGameState::StaticClass();
}
