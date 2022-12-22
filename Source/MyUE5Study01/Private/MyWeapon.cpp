// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "MyUE5Study01/MyUE5Study01.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

static int k_debugWeaponDrawing = 0;
FAutoConsoleVariableRef ACVR_debugWeaponDrawing(TEXT("My.DebugWeapons"), k_debugWeaponDrawing,TEXT("Draw debug weapon line."), ECVF_Cheat);

// Sets default values
AMyWeapon::AMyWeapon()
{
	skMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkMeshComp"));
	SetRootComponent(skMeshComp);
	traceEndName = "BeamEnd";
	baseDamage = 20;
	rateOfFire = 600.0f;
	SetReplicates(true);
}

void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
	timeBetweenShots = 60.0f / rateOfFire;
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
		params.bReturnPhysicalMaterial = true;

		FVector traceEndPoint = traceEnd;
		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(hit, eyeLocation, traceEnd, COLLISION_WEAPON, params))
		{
			//射线检测成功
			AActor* hitActor = hit.GetActor();
			EPhysicalSurface surfaceType = UPhysicalMaterial::DetermineSurfaceType(hit.PhysMaterial.Get());
			float damage = 0;
			UParticleSystem* selectEffect = nullptr;
			switch (surfaceType)
			{
			case SURFACE_FLESH_DEFAULT:
				{
					selectEffect = fleshImpactEffect;
					damage = baseDamage;
					break;
				}
			case SURFACE_FLESH_VULNERABLE:
				{
					selectEffect = fleshImpactEffect;
					damage = baseDamage * 5;
					break;
				}
			default:
				{
					selectEffect = defaultImpactEffect;
					damage = baseDamage;
					break;
				}
			}
			UGameplayStatics::ApplyPointDamage(hitActor, damage, shotDirection, hit, myOwner->GetInstigatorController(), this, damageType);

			if (selectEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), selectEffect, hit.ImpactPoint, hit.ImpactNormal.Rotation());
			}
			traceEndPoint = hit.ImpactPoint;
		}

		if (k_debugWeaponDrawing != 0)
		{
			DrawDebugLine(GetWorld(), eyeLocation, traceEnd, FColor::White, false, 1.0f, 0.0, 1.0f);
		}

		PlayFireEffect(traceEndPoint);
	}
}

// 播放开火相关特效
void AMyWeapon::PlayFireEffect(FVector traceEndPoint)
{
	//开火特效
	if (muzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(muzzleEffect, skMeshComp, muzzleSocketName);
	}
	//创建发射特效
	if (traceEffect)
	{
		FVector muzzleLocation = skMeshComp->GetSocketLocation(muzzleSocketName);
		UParticleSystemComponent* traceComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), traceEffect, muzzleLocation);
		if (traceComp)
		{
			traceComp->SetVectorParameter(traceEndName, traceEndPoint);
		}
	}

	APawn* myOwner = Cast<APawn>(GetOwner());
	if (myOwner)
	{
		APlayerController* pc = Cast<APlayerController>(myOwner->GetInstigatorController());
		if (pc)
		{
			pc->ClientStartCameraShake(fireCameraShake);
		}
	}

	lastFiredTime = GetWorld()->TimeSeconds;
}

void AMyWeapon::StartFire()
{
	float firstDelay = FMath::Max(lastFiredTime + timeBetweenShots - GetWorld()->TimeSeconds, 0);
	GetWorldTimerManager().SetTimer(timerHandle_TimeBetweenShots, this, &AMyWeapon::Fire, timeBetweenShots, true, firstDelay);
}

void AMyWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(timerHandle_TimeBetweenShots);
}
