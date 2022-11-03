// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

static int k_debugWeaponDrawing = 0;
FAutoConsoleVariableRef ACVR_debugWeaponDrawing(TEXT("My.DebugWeapons"), k_debugWeaponDrawing,TEXT("Draw debug weapon line."), ECVF_Cheat);

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
		FVector shotDirection = eyeRotation.Vector();
		FVector traceEnd = eyeLocation + shotDirection * 10000;

		FCollisionQueryParams params;
		params.AddIgnoredActor(myOwner);
		params.AddIgnoredActor(this);
		params.bTraceComplex = true;

		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(hit, eyeLocation, traceEnd, ECC_Visibility, params))
		{
			//射线检测成功
			AActor* hitActor = hit.GetActor();
			UGameplayStatics::ApplyPointDamage(hitActor, 20.0f, shotDirection, hit, myOwner->GetInstigatorController(), this, damageType);
			if (impactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactEffect, hit.ImpactPoint, hit.ImpactNormal.Rotation());
			}
		}

		if (k_debugWeaponDrawing != 0)
		{
			DrawDebugLine(GetWorld(), eyeLocation, traceEnd, FColor::White, false, 1.0f, 0.0, 1.0f);
		}

		if (muzzleEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(muzzleEffect, skMeshComp, muzzleSocketName);
		}
	}
}
