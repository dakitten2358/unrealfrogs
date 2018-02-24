// Fill out your copyright notice in the Description page of Project Settings.

#include "MapSegment.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
AMapSegment::AMapSegment(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));

	bInitialSegment = false;

	StartArea = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("StartArea"));
	StartArea->SetupAttachment(RootComponent);
	// 10 meters wide by default, 2 meters tall, 2 meters deep
	StartArea->SetBoxExtent(FVector(100, 500, 100));
	// Ahead, and rooted on the plane
	StartArea->SetRelativeLocation(FVector(100, 0, 100));

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Camera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetFieldOfView(47.0f);
	Camera->SetAspectRatio(9.0f / 16.0f);
	Camera->bConstrainAspectRatio = true;
	Camera->SetRelativeLocation(FVector(227, 0, 1225));
	Camera->SetRelativeRotation(FQuat(FVector::RightVector, FMath::DegreesToRadians(65)));
}

// Called when the game starts or when spawned
void AMapSegment::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMapSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

