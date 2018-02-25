// Fill out your copyright notice in the Description page of Project Settings.

#include "FroggerCameraControllerComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"


// Sets default values for this component's properties
UFroggerCameraControllerComponent::UFroggerCameraControllerComponent()
	: IsInitialCameraPositionSet(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFroggerCameraControllerComponent::SetCamera(APlayerController* Player, AActor* CameraObject)
{
	if (!IsInitialCameraPositionSet)
	{
		IsInitialCameraPositionSet = true;
		Player->SetViewTargetWithBlend(CameraObject, 0.0f, VTBlend_EaseInOut, 1.0F);
	}
	else
		Player->SetViewTargetWithBlend(CameraObject, 1.0f, VTBlend_EaseInOut, 12.0F);
}
