// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMyWeapon::AMyWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	skMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkMeshComp"));
	SetRootComponent(skMeshComp);
}

// Called when the game starts or when spawned
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyWeapon::Fire()
{
	AActor* myOwner = GetOwner();
	if (myOwner)
	{
		FVector eyeLocation;
		FRotator eyeRotation;
		myOwner->GetActorEyesViewPoint(eyeLocation, eyeRotation);
		FVector traceEnd = eyeLocation + eyeRotation.Vector() * 10000;

		FCollisionQueryParams params;
		params.AddIgnoredActor(myOwner);
		params.AddIgnoredActor(this);
		params.bTraceComplex = true;

		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(hit, eyeLocation, traceEnd, ECC_Visibility, params))
		{
		}
		DrawDebugLine(GetWorld(), eyeLocation, traceEnd, FColor::White, false, 1.0f, 0.0, 1.0f);
	}
}
