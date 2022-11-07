// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

UCLASS()
class MYUE5STUDY01_API AMyWeapon : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USkeletalMeshComponent* skMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<UDamageType> damageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* muzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName muzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* impactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* traceEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName traceEndName;


public:
	// Sets default values for this actor's properties
	AMyWeapon();

protected:
public:
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Fire();

	virtual void PlayFireEffect();
};
