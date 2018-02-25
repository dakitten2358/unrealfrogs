// Fill out your copyright notice in the Description page of Project Settings.

#include "FroggerGameModeBase.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Gameplay/MapSegment.h"
#include "EngineUtils.h"
#include "Private/FroggerCameraControllerComponent.h"
#include "Gameplay/FroggerGameState.h"
#include "Gameplay/FroggerPlayerState.h"

AFroggerGameModeBase::AFroggerGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CameraController = ObjectInitializer.CreateDefaultSubobject<UFroggerCameraControllerComponent>(this, TEXT("CameraController"));

	GameStateClass = AFroggerGameState::StaticClass();
	PlayerStateClass = AFroggerPlayerState::StaticClass();
}

AMapSegment* AFroggerGameModeBase::FindStartingSegment() const
{
	for (TActorIterator<AActor> It(GetWorld(), AMapSegment::StaticClass()); It; ++It)
	{
		AMapSegment* MapSegment = Cast<AMapSegment>(*It);
		if (MapSegment && !MapSegment->IsPendingKill())
		{
			if (MapSegment->bInitialSegment)
				return MapSegment;
		}
	}
	return nullptr;
}

void AFroggerGameModeBase::SetCameraToSegment(APlayerController* Player, AMapSegment* MapSegment)
{
	if (Player && MapSegment)
	{
		CameraController->SetCamera(Player, MapSegment);
	}
}

void AFroggerGameModeBase::PlayerDied_Implementation(APlayerController* Player)
{
	// do stuff, adjust life, etc

	// reset all map segments
	for (TActorIterator<AActor> It(GetWorld(), AMapSegment::StaticClass()); It; ++It)
	{
		AMapSegment* MapSegment = Cast<AMapSegment>(*It);
		if (MapSegment && !MapSegment->IsPendingKill())
		{
			MapSegment->ResetOnPlayerDeath();
		}
	}
}

void AFroggerGameModeBase::PlayerEnteredMapSegment_Implementation(APlayerController* Player, AMapSegment* MapSegment)
{
	// keep track of which segment we're in
	auto gameState = GetGameState<AFroggerGameState>();
	if (gameState)
		gameState->CurrentMapSegment = MapSegment;

	// adjust camera
	SetCameraToSegment(Player, MapSegment);
}

void AFroggerGameModeBase::PlayerEnteredLane_Implementation(APlayerController* Player, AMapSegment* MapSegment, ALane* Lane)
{
	// score?
	auto playerState = GetPlayerState<AFroggerPlayerState>(Player);
	if (playerState)
	{
		playerState->Score += 10;
	}
}

AMapSegment* AFroggerGameModeBase::GetCurrentMapSegment() const
{
	auto gameState = GetGameState<AFroggerGameState>();
	if (gameState == nullptr || gameState->CurrentMapSegment == nullptr)
		return FindStartingSegment();

	return gameState->CurrentMapSegment;
}
