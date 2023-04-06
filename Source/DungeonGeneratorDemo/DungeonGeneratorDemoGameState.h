// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DungeonGeneratorDemoGameState.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONGENERATORDEMO_API ADungeonGeneratorDemoGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Key)
		bool UseKey() noexcept;

	UFUNCTION(BlueprintCallable, Category = Key)
		void GetKey() noexcept;

	UFUNCTION(BlueprintCallable, Category = Key)
		int32 GetKeyCount() const noexcept;

	UFUNCTION(BlueprintCallable, Category = UniqueKey)
		bool UseUniqueKey() noexcept;

	UFUNCTION(BlueprintCallable, Category = UniqueKey)
		void GetUniqueKey() noexcept;

	UFUNCTION(BlueprintCallable, Category = UniqueKey)
		int32 GetUniqueKeyCount() const noexcept;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int32 KeyCount = 0;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int32 UniqueKeyCount = 0;
};
