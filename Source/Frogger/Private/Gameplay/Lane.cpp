// Fill out your copyright notice in the Description page of Project Settings.

#include "Lane.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALane::ALane(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DespawnArea = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("StartArea"));
}

// Called when the game starts or when spawned
void ALane::BeginPlay()
{
	Super::BeginPlay();

	const bool isLooping = true;
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ALane::SpawnCollidingActor, TimeBetweenSpawns, isLooping);
}

void ALane::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ALane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALane::SpawnCollidingActor()
{
	if (ActorToSpawn != nullptr)
	{
		auto newActor = Cast<AActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ActorToSpawn, SpawnTransform));
		if (newActor)
		{
			newActor->SetOwner(this);
			UGameplayStatics::FinishSpawningActor(newActor, SpawnTransform);
		}
	}
}
