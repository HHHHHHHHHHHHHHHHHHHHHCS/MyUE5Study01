// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon_Projectile.h"

void AMyWeapon_Projectile::Fire()
{
	AActor* myOwner = GetOwner();
	if (myOwner && projectileCls)
	{
		FVector eyeLocation;
		FRotator eyeRotation;
		myOwner->GetActorEyesViewPoint(eyeLocation, eyeRotation);
		FVector muzzleLocation = skMeshComp->GetSocketLocation(muzzleSocketName);
		// FRotator muzzleRotator = skMeshComp->GetSocketRotation(muzzleSocketName);

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AMyWeapon_Projectile>(projectileCls, muzzleLocation, eyeRotation, spawnParams);
	}
}
