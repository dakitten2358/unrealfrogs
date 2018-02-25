// Fill out your copyright notice in the Description page of Project Settings.

#include "MapSegment.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/FroggerGameModeBase.h"
#include "Gameplay/Lane.h"


// Sets default values
AMapSegment::AMapSegment(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//  These are instance only properties, but we'll disable both for now
	bInitialSegment = false;
	bFinalSegment = false;

	// RootComponent is a scene component so that we can easily set some default values,
	// and move sub-components around easily.
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));

	// StartArea is the area that when the player enters it means we've started this segment
	{
		StartArea = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("StartArea"));
		StartArea->SetupAttachment(RootComponent);
		// 10 meters wide by default, 2 meters tall, 2 meters deep
		StartArea->SetBoxExtent(FVector(100, 500, 100));
		StartArea->SetRelativeLocation(FVector(100, 0, 100));
		StartArea->OnComponentBeginOverlap.AddDynamic(this, &AMapSegment::OnStartAreaBeginOverlap);
	}

	// BlockArea is a collision volume that prevents the player moving backwards after having entered this segment
	{
		BlockingArea = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BlockingArea"));
		BlockingArea->ShapeColor = FColor::Red;
		BlockingArea->SetupAttachment(StartArea);
		BlockingArea->SetBoxExtent(FVector(100, 500, 100));
		BlockingArea->SetRelativeLocation(FVector(-200, 0, 0));

		// block  er'ything except visibility
		BlockingArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		BlockingArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

		// start off, we'll only turn this on after the player has entered
		BlockingArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	// Display mesh for this segment
	{
		Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
		Mesh->SetupAttachment(RootComponent);
	}

	// Camera for this segment
	{
		Camera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
		Camera->SetupAttachment(RootComponent);
		Camera->SetFieldOfView(47.0f);
		Camera->SetAspectRatio(9.0f / 16.0f);
		Camera->bConstrainAspectRatio = true;
		Camera->SetRelativeLocation(FVector(227, 0, 1225));
		Camera->SetRelativeRotation(FQuat(FVector::RightVector, FMath::DegreesToRadians(65)));
	}
}

// Called when the game starts or when spawned
void AMapSegment::BeginPlay()
{
	Super::BeginPlay();

	// wire ourselves up to our lanes
	for (const auto& lane : Lanes)
	{
		lane->OnPlayerEnteredLane.AddDynamic(this, &AMapSegment::OnPlayerEnteredLane);
	}

	// check for overlapping actors at the start
	TArray<AActor*> OverlappingActors;
	StartArea->GetOverlappingActors(OverlappingActors, APawn::StaticClass());
	for (auto actor : OverlappingActors)
	{
		auto pawn = Cast<APawn>(actor);
		auto controller = pawn ? Cast<APlayerController>(pawn->GetController()) : nullptr;
		if (controller)
		{
			PlayerEnteredSegment(controller);
		}
	}
}

void AMapSegment::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// unwire ourselves from our lanes
	for (const auto& lane : Lanes)
	{
		lane->OnPlayerEnteredLane.RemoveDynamic(this, &AMapSegment::OnPlayerEnteredLane);
	}

	Super::EndPlay(EndPlayReason);
}

void AMapSegment::OnStartAreaBeginOverlap(UPrimitiveComponent* thisComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// we'll check to see if it's a player that entered, and if so, let the gmae mode know
	if (OtherActor != nullptr && OtherActor != this && Cast<APawn>(OtherActor) != nullptr)
	{
		auto pawn = Cast<APawn>(OtherActor);
		auto controller = pawn != nullptr ? Cast<APlayerController>(pawn->GetController()) : nullptr;
		
		if (controller)
		{
			PlayerEnteredSegment(controller);
		}
	}
}

void AMapSegment::OnPlayerEnteredLane(APlayerController* Player, ALane* Lane)
{
	// first time?  forward it to the game mode
	if (!HasLaneBeenSeen.Contains(Lane) || !HasLaneBeenSeen[Lane])
	{
		HasLaneBeenSeen.Add(Lane, true);

		auto gameMode = GetFroggerGameMode();
		gameMode->PlayerEnteredLane(Player, this, Lane);
	}
}

void AMapSegment::PlayerEnteredSegment(APlayerController* Player)
{
	auto gameMode = GetFroggerGameMode();
	if (gameMode)
	{
		// let the game mode know
		gameMode->PlayerEnteredMapSegment(Player, this);

		// turn on our blocking area to prevent the player from going back
		BlockingArea->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}	
}

void AMapSegment::ResetOnPlayerDeath()
{
	// disable collision
	BlockingArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// reset lanes
	HasLaneBeenSeen.Empty(Lanes.Num());
}

AFroggerGameModeBase* AMapSegment::GetFroggerGameMode() const
{
	auto gameMode = Cast<AFroggerGameModeBase>(GetWorld()->GetAuthGameMode());
	check(gameMode);
	return gameMode;
}

