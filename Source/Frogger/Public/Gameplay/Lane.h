// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lane.generated.h"

UCLASS()
class FROGGER_API ALane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALane(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, Meta = (MakeEditWidget = true))
	class UBoxComponent* DespawnArea;

	// default object to spawn with
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float TimeBetweenSpawns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, Meta = (MakeEditWidget = true))
	FTransform SpawnTransform;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FTimerHandle SpawnTimerHandle;

private:
	void SpawnCollidingActor();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
