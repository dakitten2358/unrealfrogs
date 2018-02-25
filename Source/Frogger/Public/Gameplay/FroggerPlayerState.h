// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FroggerPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FROGGER_API AFroggerPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AFroggerPlayerState(const FObjectInitializer& ObjectInitializer);
	
public:
	UPROPERTY(BlueprintReadOnly, Category = Scoring)
	int32 Lives;
};
