// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FroggerCameraControllerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UFroggerCameraControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFroggerCameraControllerComponent();

	void SetCamera(class APlayerController* Player, class AActor* CameraObject);

private:
	bool IsInitialCameraPositionSet;
};
