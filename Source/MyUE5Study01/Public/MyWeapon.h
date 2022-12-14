// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> surfaceType;

	UPROPERTY()
	FVector_NetQuantize traceTo;
};

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
	UParticleSystem* traceEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName traceEndName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<UCameraShakeBase> fireCameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* defaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* fleshImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	float baseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	float rateOfFire;

	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
	FHitScanTrace hitScanTrace;

public:
	// Sets default values for this actor's properties
	AMyWeapon();

protected:
	FTimerHandle timerHandle_TimeBetweenShots;

	float lastFiredTime;

	float timeBetweenShots;

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Fire();

	virtual void PlayFireEffect(FVector traceEndPoint);

	virtual void StartFire();

	virtual void StopFire();

	//实现服务器开火函数
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	void PlayImpactEffects(EPhysicalSurface surfaceType, FVector impactPoint);

	UFUNCTION()
	void OnRep_HitScanTrace();
};
