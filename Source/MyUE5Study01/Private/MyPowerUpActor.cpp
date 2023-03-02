// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPowerUpActor.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AMyPowerUpActor::AMyPowerUpActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	powerupInterval = 0;
	totalOfTicks = 0;
	tickProcessed = 0;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AMyPowerUpActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPowerUpActor::OnTickPowerup()
{
	tickProcessed++;
	OnPowerupTicked();
	if (tickProcessed >= totalOfTicks)
	{
		OnExpired();
		isPowerupActive = false;
		OnRep_PowerupActive();
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTicks);
	}
}

void AMyPowerUpActor::ActivePowerup()
{
	OnActived();
	isPowerupActive = true;
	OnRep_PowerupActive();
	if (powerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTicks, this, &AMyPowerUpActor::OnTickPowerup
		                                , powerupInterval, true);
	}
	else
	{
		OnTickPowerup();
	}
}

void AMyPowerUpActor::OnRep_PowerupActive()
{
	OnPowerStateChanged(isPowerupActive);
}

void AMyPowerUpActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyPowerUpActor, isPowerupActive);
}
