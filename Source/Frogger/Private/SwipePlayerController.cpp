// Fill out your copyright notice in the Description page of Project Settings.

#include "SwipePlayerController.h"
#include "GameFramework/Pawn.h"
#include "NumericLimits.h"

std::map<ESwipeDirection, FVector> ASwipePlayerController::DirectionMap = 
{
	{ ESwipeDirection::SD_Up, FVector(0, -1, 0) },
	{ ESwipeDirection::SD_Right, FVector(1, 0, 0) },
	{ ESwipeDirection::SD_Down, FVector(0, 1, 0) },
	{ ESwipeDirection::SD_Left, FVector(-1, 0, 0) }
};

void ASwipePlayerController::SetupInputComponent()
{
	APlayerController::SetupInputComponent();

	check(InputComponent);
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ASwipePlayerController::SwipeBegan);
	InputComponent->BindTouch(EInputEvent::IE_Released, this, &ASwipePlayerController::SwipeEnded);
}

void ASwipePlayerController::SwipeBegan(ETouchIndex::Type fingerIndex, FVector location)
{
	RecordedTouchBegins.FindOrAdd(fingerIndex) = location;
}

void ASwipePlayerController::SwipeEnded(ETouchIndex::Type fingerIndex, FVector location)
{
	// were we recording a touch for this finger?
	auto pInitialLocation = RecordedTouchBegins.Find(fingerIndex);
	if (pInitialLocation)
	{
		auto initialLocation = *pInitialLocation;
		
		// get the direction vector
		auto direction = (location - initialLocation);
		direction.Normalize();

		// find the nearest cardinal direction
		auto cardinalDirection = FindNearestCardinalDirection(direction);

		// send out the raw input, just in case someone wants it
		SwipeInputRaw(initialLocation, location, cardinalDirection, fingerIndex);

		FVector worldInitialLocation, worldFinalLocation;
		if (GetWorldspaceLocation(initialLocation, worldInitialLocation) && GetWorldspaceLocation(location, worldFinalLocation))
		{
			// send the world space stuff
			auto worldDirection = worldFinalLocation - worldInitialLocation;
			worldDirection.Normalize();
			SwipeInput(worldInitialLocation, worldFinalLocation, worldDirection, cardinalDirection, fingerIndex);
		}
				
		// make sure we clear our the key we're using to track this touch
		RecordedTouchBegins.Remove(fingerIndex);
	}
}

ESwipeDirection ASwipePlayerController::FindNearestCardinalDirection(const FVector& directionVector) const
{
	// loop over the directions to find the nearest direction
	auto nearestDirection = ESwipeDirection::SD_None;
	auto closestDistanceSquared = MAX_flt;
	for (const auto& p : DirectionMap)
	{
		auto cardinalDirection = p.first;
		const auto& cardinalVector = p.second;

		auto distanceSquared = FVector::DistSquared(cardinalVector, directionVector);
		if (distanceSquared < closestDistanceSquared)
		{
			nearestDirection = cardinalDirection;
			closestDistanceSquared = distanceSquared;
		}
	}

	return nearestDirection;
}

bool ASwipePlayerController::GetWorldspaceLocation(const FVector& screenLocation, FVector& worldLocation) const
{
	worldLocation = FVector::ZeroVector;
	FVector worldDirection = FVector::ZeroVector;

	bool success = DeprojectScreenPositionToWorld(screenLocation.X, screenLocation.Y, worldLocation, worldDirection);
	if (!success)
	{
		// figure out where this intersects with the plane that the player is on
		const FVector& lineStart = worldLocation;
		FVector lineEnd = worldLocation + (worldDirection * 1000000);
		FVector planeOrigin = FVector::ZeroVector;
		if (auto pawn = GetPawn())
		{
			planeOrigin = pawn->GetActorLocation();
			FVector planeNormal = pawn->GetActorUpVector();
			auto locationOnPlane = FMath::LinePlaneIntersection(lineStart, lineEnd, planeOrigin, planeNormal);
			worldLocation = locationOnPlane;
			return true;
		}
	}

	return false;
}
