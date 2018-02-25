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

	// shouldn't be blueprint callbable, but we'll leave it so until we 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Gameplay)
	void PlayerDied(class APlayerController* Player);

	UFUNCTION(BlueprintNativeEvent, Category = Gameplay)
	void PlayerEnteredMapSegment(class APlayerController* Player, class AMapSegment* MapSegment);

	UFUNCTION(BlueprintNativeEvent, Category = Gameplay)
	void PlayerEnteredLane(class APlayerController* Player, class AMapSegment* MapSegment, class ALane* Lane);

	UFUNCTION(BlueprintCallable, Category = Camera)
	void SetCameraToSegment(class APlayerController* Player, class AMapSegment* MapSegment);

	UFUNCTION(BlueprintPure, Category = Gameplay)
	class AMapSegment* GetCurrentMapSegment() const;

private:
	UPROPERTY()
	class UFroggerCameraControllerComponent* CameraController;

	template<typename PlayerStateType> 
	PlayerStateType* GetPlayerState(class APlayerController* Player) const
	{
		return Cast<PlayerStateType>(Player->PlayerState);
	}
};
