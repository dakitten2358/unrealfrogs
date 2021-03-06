// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapSegment.generated.h"


UCLASS()
class FROGGER_API AMapSegment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapSegment(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleDefaultsOnly, Category = "Display")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay")
	class UBoxComponent* StartArea;

	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
	class UBoxComponent* BlockingArea;

	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay")
	class UCameraComponent* Camera;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Gameplay")
	bool bInitialSegment;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Gameplay")
	bool bFinalSegment;

	UPROPERTY(EditInstanceOnly, Category = "Gameplay")
	TArray<class ALane*> Lanes;

	UFUNCTION(BlueprintPure)
	class AFroggerGameModeBase* GetFroggerGameMode() const;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void ResetOnPlayerDeath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnStartAreaBeginOverlap(class UPrimitiveComponent* thisComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerEnteredLane(class APlayerController* Player, class ALane* Lane);

private:
	TMap<class ALane*, bool> HasLaneBeenSeen;

	void PlayerEnteredSegment(class APlayerController* Player);
	
	
};
