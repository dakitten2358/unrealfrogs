// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FroggerGameState.generated.h"

/**
 * 
 */
UCLASS()
class FROGGER_API AFroggerGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	class AMapSegment* CurrentMapSegment;

};
