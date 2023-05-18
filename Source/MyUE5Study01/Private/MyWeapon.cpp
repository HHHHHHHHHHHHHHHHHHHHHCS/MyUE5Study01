// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "MyUE5Study01/MyUE5Study01.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Net/UnrealNetwork.h"

static int32 k_DebugWeaponDrawing = 0;
FAutoConsoleVariableRef ACVR_DebugWeaponDrawing(TEXT("My.DebugWeapons"), k_DebugWeaponDrawing,
                                                TEXT("Draw debug weapon line."), ECVF_Cheat);

// Sets default values
AMyWeapon::AMyWeapon()
{
	skMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkMeshComp"));
	SetRootComponent(skMeshComp);
	traceEndName = "BeamEnd";
	baseDamage = 20;
	rateOfFire = 600.0f;
	bulletSpread = 1.0f;
	bReplicates = true;
	SetReplicates(true);
	//网络更新频率的设置
	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}

void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
	timeBetweenShots = 60.0f / rateOfFire;
}

void AMyWeapon::Fire()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerFire();
	}
	AActor* myOwner = GetOwner();
	if (myOwner)
	{
		FVector eyeLocation;
		FRotator eyeRotation;
		myOwner->GetActorEyesViewPoint(eyeLocation, eyeRotation);
		FVector shotDirection = eyeRotation.Vector();
		FVector traceEnd = eyeLocation + shotDirection * 10000;

		float halfRad = FMath::DegreesToRadians(bulletSpread);
		shotDirection = FMath::VRandCone(shotDirection, halfRad, halfRad);

		FCollisionQueryParams params;
		params.AddIgnoredActor(myOwner);
		params.AddIgnoredActor(this);
		params.bTraceComplex = true;
		params.bReturnPhysicalMaterial = true;

		FVector traceEndPoint = traceEnd;
		EPhysicalSurface surfaceType = SurfaceType_Default;
		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(hit, eyeLocation, traceEnd, COLLISION_WEAPON, params))
		{
			//射线检测成功
			AActor* hitActor = hit.GetActor();
			surfaceType = UPhysicalMaterial::DetermineSurfaceType(hit.PhysMaterial.Get());
			float damage = -1;
			switch (surfaceType)
			{
			case SURFACE_FLESH_DEFAULT:
				{
					damage = baseDamage;
					break;
				}
			case SURFACE_FLESH_VULNERABLE:
				{
					damage = baseDamage * 5;
					break;
				}
			default:
				{
					damage = -1;
					break;
				}
			}
			if (damage >= 0)
			{
				UGameplayStatics::ApplyPointDamage(hitActor, damage, shotDirection, hit,
				                                   myOwner->GetInstigatorController(), GetOwner(), damageType);
				PlayImpactEffects(surfaceType, hit.Location);
			}

			traceEndPoint = hit.ImpactPoint;
		}

		if (k_DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), eyeLocation, traceEnd, FColor::White, false, 1.0f, 0.0, 1.0f);
		}

		PlayFireEffect(traceEndPoint);

		if (GetLocalRole() == ROLE_Authority)
		{
			hitScanTrace.surfaceType = surfaceType;
			hitScanTrace.traceTo = traceEndPoint;
		}
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
		UParticleSystemComponent* traceComp = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), traceEffect, muzzleLocation);
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
	GetWorldTimerManager().SetTimer(timerHandle_TimeBetweenShots, this, &AMyWeapon::Fire, timeBetweenShots, true,
	                                firstDelay);
}

void AMyWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(timerHandle_TimeBetweenShots);
}

void AMyWeapon::PlayImpactEffects(EPhysicalSurface surfaceType, FVector impactPoint)
{
	UParticleSystem* selectEffect;
	switch (surfaceType)
	{
	case SURFACE_FLESH_DEFAULT:
		{
			selectEffect = fleshImpactEffect;
			break;
		}
	case SURFACE_FLESH_VULNERABLE:
		{
			selectEffect = fleshImpactEffect;
			break;
		}
	default:
		{
			selectEffect = defaultImpactEffect;
			break;
		}
	}

	if (selectEffect)
	{
		FVector shortDir = impactPoint - skMeshComp->GetSocketLocation(muzzleSocketName);
		shortDir.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), selectEffect, impactPoint, shortDir.Rotation());
	}
}

void AMyWeapon::OnRep_HitScanTrace()
{
	PlayFireEffect(hitScanTrace.traceTo);
	PlayImpactEffects(hitScanTrace.surfaceType, hitScanTrace.traceTo);
}

void AMyWeapon::ServerFire_Implementation()
{
	Fire();
}

bool AMyWeapon::ServerFire_Validate()
{
	return true;
}

void AMyWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AMyWeapon, hitScanTrace, COND_SkipOwner);
}
