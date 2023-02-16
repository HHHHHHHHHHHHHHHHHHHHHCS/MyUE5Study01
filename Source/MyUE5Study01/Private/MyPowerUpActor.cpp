// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPowerUpActor.h"

// Sets default values
AMyPowerUpActor::AMyPowerUpActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	powerupInterval = 0;
	totalOfTicks = 0;
	tickProcessed = 0;
}

// Called when the game starts or when spawned
void AMyPowerUpActor::BeginPlay()
{
	Super::BeginPlay();
	ActivePowerup();
}

void AMyPowerUpActor::OnTickPowerup()
{
	tickProcessed++;
	if (tickProcessed >= totalOfTicks)
	{
		OnExpired();
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTicks);
	}
}

void AMyPowerUpActor::ActivePowerup()
{
	if (powerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTicks, this, &AMyPowerUpActor::OnTickPowerup
		                                , powerupInterval, true, 0.0f);
	}
	else
	{
		OnTickPowerup();
	}
}
