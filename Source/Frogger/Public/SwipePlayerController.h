// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <map>
#include "SwipePlayerController.generated.h"

UENUM(BlueprintType)
enum class ESwipeDirection : uint8
{
	SD_None,
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

protected:
	virtual void SetupInputComponent() override;
	
public:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Swipe Input (Screen)"))
	void SwipeInputRaw(FVector Origin, FVector Destination, ESwipeDirection Direction, ETouchIndex::Type TouchIndex);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Swipe Input (World)"))
	void SwipeInput(FVector WorldOrigin, FVector WorldDestination, FVector WorldDirection, ESwipeDirection Direction, ETouchIndex::Type TouchIndex);

private:
	void SwipeBegan(ETouchIndex::Type fingerIndex, FVector location);
	void SwipeEnded(ETouchIndex::Type fingerIndex, FVector location);

	TMap<ETouchIndex::Type, FVector> RecordedTouchBegins;

	bool GetWorldspaceLocation(const FVector& screenLocation, FVector& outWorldLocation) const;
	ESwipeDirection FindNearestCardinalDirection(const FVector& directionVector) const;

private:
	static std::map<ESwipeDirection, FVector> DirectionMap;
};
