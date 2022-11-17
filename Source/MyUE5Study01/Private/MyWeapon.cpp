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
		if (GetWorld()->LineTraceSingleByChannel(hit, eyeLocation, traceEnd, ECC_Visibility, params))
		{
			//射线检测成功
			AActor* hitActor = hit.GetActor();
			UGameplayStatics::ApplyPointDamage(hitActor, 20.0f, shotDirection, hit, myOwner->GetInstigatorController(), this, damageType);
			EPhysicalSurface surfaceType = UPhysicalMaterial::DetermineSurfaceType(hit.PhysMaterial.Get());
			UParticleSystem* selectEffect = nullptr;
			switch (surfaceType)
			{
			case SURFACE_FLESH_DEFAULT:
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
}
