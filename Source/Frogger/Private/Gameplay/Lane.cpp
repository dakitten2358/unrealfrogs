// Fill out your copyright notice in the Description page of Project Settings.

#include "Lane.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "DisplayDebugHelpers.h"

// Sets default values
ALane::ALane(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Event is fired when the player enters the Lane (scoring, etc)
	LaneArea = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("LaneArea"));
	check(LaneArea);
	LaneArea->OnComponentBeginOverlap.AddDynamic(this, &ALane::OnLaneOverlapBegin);
	LaneArea->ShapeColor = FColor::Green;
	RootComponent = LaneArea;

	// Objects entering the despawn area will be destroyed
	DespawnArea = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("DespawnArea"));
	check(DespawnArea);
	DespawnArea->SetupAttachment(LaneArea);
	DespawnArea->ShapeColor = FColor::Red;
	DespawnArea->OnComponentBeginOverlap.AddDynamic(this, &ALane::OnDespawnOverlapBegin);

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SpriteInfo.Category = TEXT("Gameplay");
		//ArrowComponent->SpriteInfo.DisplayName = FName("Spawner");
		ArrowComponent->SetupAttachment(LaneArea);
		ArrowComponent->bIsScreenSizeScaled = true;

		ArrowComponent->SetRelativeTransform(SpawnTransform);
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
		auto newActor = Cast<AActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ActorToSpawn, SpawnTransform));
		if (newActor)
		{
			newActor->SetOwner(this);
			UGameplayStatics::FinishSpawningActor(newActor, SpawnTransform);
		}
	}
}

void ALane::OnDespawnOverlapBegin(UPrimitiveComponent* thisComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		OtherActor->Destroy();
	}
}

void ALane::OnLaneOverlapBegin(UPrimitiveComponent* thisComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		OnFirstTimeEnteredByPlayer();
	}
}

void ALane::OnFirstTimeEnteredByPlayer_Implementation()
{

}

#if WITH_EDITORONLY_DATA
void ALane::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	Super::PostEditChangeProperty(propertyChangedEvent);

	FName propertyName = (propertyChangedEvent.Property != NULL) ? propertyChangedEvent.Property->GetFName() : NAME_None;
	if (propertyName == GET_MEMBER_NAME_CHECKED(ALane, SpawnTransform))
	{
		ArrowComponent->SetRelativeTransform(SpawnTransform);
	}
}
#endif
