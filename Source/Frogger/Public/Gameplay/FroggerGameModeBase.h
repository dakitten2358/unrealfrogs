// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FroggerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FROGGER_API AFroggerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AFroggerGameModeBase(const FObjectInitializer& ObjectInitializer);
	
public:
	UFUNCTION(BlueprintPure, Category = Gameplay)
	class AMapSegment* FindStartingSegment() const;

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void PlayerEnteredMapSegment(class APlayerController* Player, class AMapSegment* MapSegment);

	UPROPERTY(VisibleInstanceOnly, Category = Gameplay)
	class AMapSegment* CurrentMapSegment;

private:
	bool hasSetCamera;
};
