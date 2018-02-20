// Fill out your copyright notice in the Description page of Project Settings.

#include "MapSegment.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"


// Sets default values
AMapSegment::AMapSegment(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	StartArea = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("StartArea"));
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

