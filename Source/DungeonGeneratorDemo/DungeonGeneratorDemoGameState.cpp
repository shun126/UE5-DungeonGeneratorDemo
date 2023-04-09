// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGeneratorDemoGameState.h"

bool ADungeonGeneratorDemoGameState::UseKey() noexcept
{
	if (KeyCount <= 0)
	{
		return false;
	}
	else
	{
		--KeyCount;
		return true;
	}
}

void ADungeonGeneratorDemoGameState::GetKey() noexcept
{
	++KeyCount;
}

int32 ADungeonGeneratorDemoGameState::GetKeyCount() const noexcept
{
	return KeyCount;
}

bool ADungeonGeneratorDemoGameState::UseUniqueKey() noexcept
{
	if (UniqueKeyCount <= 0)
	{
		return false;
	}
	else
	{
		--UniqueKeyCount;
		return true;
	}
}

void ADungeonGeneratorDemoGameState::GetUniqueKey() noexcept
{
	++UniqueKeyCount;
}

int32 ADungeonGeneratorDemoGameState::GetUniqueKeyCount() const noexcept
{
	return KeyCount;
}
