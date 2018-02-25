// Fill out your copyright notice in the Description page of Project Settings.

#include "Lane.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "DisplayDebugHelpers.h"
#include "GameFramework/Pawn.h"

// Sets default values
ALane::ALane(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));

	// Event is fired when the player enters the Lane (scoring, etc)
	LaneArea = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("LaneArea"));
	LaneArea->SetupAttachment(RootComponent);
	LaneArea->ShapeColor = FColor::Green;
	LaneArea->SetBoxExtent(FVector(100, 500, 100));
	LaneArea->SetRelativeLocation(FVector(100, 0, 100));
	LaneArea->OnComponentBeginOverlap.AddDynamic(this, &ALane::OnLaneOverlapBegin);
	
	// Objects entering the despawn area will be destroyed
	DespawnArea = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("DespawnArea"));
	check(DespawnArea);
	DespawnArea->SetupAttachment(RootComponent);
	DespawnArea->ShapeColor = FColor::Red;
	DespawnArea->SetBoxExtent(FVector(100, 100, 100));
	DespawnArea->SetRelativeLocation(FVector(100, -1000, 100));
	DespawnArea->OnComponentEndOverlap.AddDynamic(this, &ALane::OnDespawnOverlap);

	SpawnTransform = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("SpawnTransform"));
	SpawnTransform->SetupAttachment(RootComponent);
	SpawnTransform->SetRelativeLocation(FVector(100, 1000, 0));
	SpawnTransform->SetRelativeRotation(FQuat(FVector::UpVector, FMath::DegreesToRadians(-90)));

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SpriteInfo.Category = TEXT("Gameplay");
		ArrowComponent->bIsScreenSizeScaled = true;
		ArrowComponent->SetupAttachment(SpawnTransform);
	}
#endif
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
		auto spawnTransform = SpawnTransform->GetComponentTransform();
		auto newActor = Cast<AActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ActorToSpawn, spawnTransform));
		if (newActor)
		{
			newActor->SetOwner(this);
			UGameplayStatics::FinishSpawningActor(newActor, spawnTransform);
		}
	}
}

void ALane::OnDespawnOverlap(UPrimitiveComponent* thisComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex/*, bool bFromSweep, const FHitResult& SweepResult*/)
{
	// destroy the actor if we're the owner
	if (OtherActor != nullptr && OtherActor != this && OtherActor->GetOwner() == this)
	{
		OtherActor->Destroy();
	}
}

void ALane::OnLaneOverlapBegin(UPrimitiveComponent* thisComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// need to check to see if it's the player, and the player hasn't entered before
	if (OtherActor != nullptr && OtherActor != this)
	{
		auto pawn = Cast<APawn>(OtherActor);
		auto controller = pawn ? Cast<APlayerController>(pawn->GetController()) : nullptr;

		if (pawn && controller)
		{ 
			OnPlayerEnteredLane.Broadcast(controller, this);
		}
	}
}