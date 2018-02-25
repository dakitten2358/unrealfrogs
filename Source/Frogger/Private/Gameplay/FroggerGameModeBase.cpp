// Fill out your copyright notice in the Description page of Project Settings.

#include "FroggerGameModeBase.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Gameplay/MapSegment.h"
#include "EngineUtils.h"

AFroggerGameModeBase::AFroggerGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CurrentMapSegment(nullptr)
	, hasSetCamera(false)
{

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

void AFroggerGameModeBase::PlayerEnteredMapSegment(APlayerController* Player, AMapSegment* MapSegment)
{
	// keep track of which segment we're in
	CurrentMapSegment = MapSegment;

	// adjust camera
	if (Player)
	{
		if (!hasSetCamera)
		{
			hasSetCamera = true;
			Player->SetViewTargetWithBlend(this, 0.0f, VTBlend_EaseInOut, 1.0F);
		}
		else
			Player->SetViewTargetWithBlend(this, 2.0f, VTBlend_EaseInOut, 1.0F);
	}
}
