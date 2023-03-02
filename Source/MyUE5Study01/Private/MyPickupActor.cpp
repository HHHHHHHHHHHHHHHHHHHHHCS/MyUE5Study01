// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPickupActor.h"

#include "Components/DecalComponent.h"

// Sets default values
AMyPickupActor::AMyPickupActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	sphereComp->SetSphereRadius(80.0f);
	SetRootComponent(sphereComp);

	decalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	decalComp->SetupAttachment(sphereComp);
	decalComp->SetRelativeLocation(FVector(0, 0, 20));
	decalComp->SetRelativeRotation(FRotator(90, 0, 0));
	decalComp->DecalSize = FVector(40, 80, 80);

	spawnDuration = 10.0f;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AMyPickupActor::BeginPlay()
{
	Super::BeginPlay();
	if (GetLocalRole() == ROLE_Authority)
	{
		Respawn();
	}
}

// Called every frame
void AMyPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyPickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	//增强相关的逻辑
	if (powerUpInst)
	{
		powerUpInst->ActivePowerup();
		powerUpInst = nullptr;
		GetWorldTimerManager().SetTimer(timerHandle_respawn, this, &AMyPickupActor::Respawn, spawnDuration);
	}
}

void AMyPickupActor::Respawn()
{
	if (powerUpClass == nullptr || GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	powerUpInst = GetWorld()->SpawnActor<AMyPowerUpActor>(powerUpClass, GetTransform(), spawnParams);
}
