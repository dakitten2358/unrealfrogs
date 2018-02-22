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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
	class UBoxComponent* LaneArea;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
	class UBoxComponent* DespawnArea;

	// default object to spawn with
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float TimeBetweenSpawns;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Gameplay, Meta = (MakeEditWidget = true))
	FTransform SpawnTransform;

	UFUNCTION(BlueprintNativeEvent, Category = Gameplay)
	void OnFirstTimeEnteredByPlayer();
	void OnFirstTimeEnteredByPlayer_Implementation();

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	class UArrowComponent* ArrowComponent;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#if WITH_EDITORONLY_DATA
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	FTimerHandle SpawnTimerHandle;

private:
	void SpawnCollidingActor();

	UFUNCTION()
	void OnDespawnOverlapBegin(class UPrimitiveComponent* thisComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLaneOverlapBegin(class UPrimitiveComponent* thisComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
