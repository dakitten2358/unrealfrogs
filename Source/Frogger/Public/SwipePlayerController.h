// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SwipePlayerController.generated.h"

UENUM(BlueprintType)
enum class ESwipeDirection : uint8
{
	SD_Up,
	SD_Right,
	SD_Down,
	SD_Left,
};

/**
 * 
 */
UCLASS()
class FROGGER_API ASwipePlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Swipe Input (RAW)"))
	void SwipeInputRaw(FVector2D Origin, FVector2D Destination);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Swipe Input"))
	void SwipeInput(FVector2D InputVector, ESwipeDirection Direction);
};
