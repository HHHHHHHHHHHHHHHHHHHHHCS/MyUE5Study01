// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyTrackerBot.generated.h"

UCLASS()
class MYUE5STUDY01_API AMyTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyTrackerBot();

public:
	UPROPERTY(VisibleDefaultsOnly,Category="Components")
	UStaticMeshComponent* meshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
