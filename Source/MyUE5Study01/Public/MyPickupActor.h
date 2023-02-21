// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPowerUpActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "MyPickupActor.generated.h"

UCLASS()
class MYUE5STUDY01_API AMyPickupActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyPickupActor();

	UPROPERTY(VisibleAnywhere, Category="Components")
	USphereComponent* sphereComp;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UDecalComponent* decalComp;

	UPROPERTY(EditAnywhere, Category="Pickup")
	TSubclassOf<AMyPowerUpActor> powerUpClass;

	UPROPERTY(EditAnywhere, Category="Pickup")
	float spawnDuration;

	
private:
	AMyPowerUpActor* powerUpInst;
	FTimerHandle timerHandle_respawn;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void Respawn();
};
