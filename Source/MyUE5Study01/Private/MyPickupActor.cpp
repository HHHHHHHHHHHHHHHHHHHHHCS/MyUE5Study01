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
}

// Called when the game starts or when spawned
void AMyPickupActor::BeginPlay()
{
	Super::BeginPlay();
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
}
