// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Delegate.h"
#include "Delegates/DelegateCombinations.h"
#include "Lane.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerEnteredLaneSignature, class APlayerController*, Player, class ALane*, Lane);

UCLASS()
class FROGGER_API ALane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALane(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
	class UBoxComponent* LaneArea;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
	class UBoxComponent* DespawnArea;

	// default object to spawn with
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float TimeBetweenSpawns;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
	class USceneComponent* SpawnTransform;

	UPROPERTY(BlueprintAssignable, Category = "Collision")
	FPlayerEnteredLaneSignature OnPlayerEnteredLane;

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	class UArrowComponent* ArrowComponent;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FTimerHandle SpawnTimerHandle;

private:
	void SpawnCollidingActor();

	UFUNCTION()
	void OnDespawnOverlap(class UPrimitiveComponent* thisComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex/*, bool bFromSweep, const FHitResult& SweepResult*/);

	UFUNCTION()
	void OnLaneOverlapBegin(class UPrimitiveComponent* thisComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
