// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyHealthComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "Sound/SoundCue.h"
#include "MyTrackerBot.generated.h"

UCLASS()
class MYUE5STUDY01_API AMyTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyTrackerBot();

public:
	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UStaticMeshComponent* meshComp;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UMyHealthComponent* healthComponent;

	UPROPERTY(EditAnywhere, Category="TrackerBot")
	float movementForce;

	UPROPERTY(EditAnywhere, Category="TrackerBot")
	bool useVelocityChange;

	UPROPERTY(EditAnywhere, Category="TrackerBot")
	float requireDistanceToTarget;

	UPROPERTY(EditAnywhere, Category="TrackerBot")
	UParticleSystem* explosionEffect;

	UPROPERTY(EditAnywhere, Category="TrackerBot")
	float explosionRadius;

	UPROPERTY(EditAnywhere, Category="TrackerBot")
	float explosionDamage;

	UPROPERTY(VisibleDefaultsOnly, Category="TrackerBot")
	USphereComponent* sphereComp;

	UPROPERTY(EditAnywhere, Category="TrackerBot")
	float selfDamageInterval;
	
	// 对人物造成伤害的音效
	UPROPERTY(EditAnywhere, Category="TrackerBot")
	USoundCue* selfDestroyCue;

	// 爆炸音效
	UPROPERTY(EditAnywhere, Category="TrackerBot")
	USoundCue* explodeSound;
	
private:
	FVector nextPathPoint;

	UMaterialInstanceDynamic* matInst;

	bool isExploded;

	FTimerHandle timerHandle_damageSelf;

	FTimerHandle timerHandle_refreshPath;
	
	bool isStartDamageSelf;

	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector GetNextPathPoint();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleTakeDamage(UMyHealthComponent* HealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	void RefreshNextPath();

	void DamageSelf();
	
	void SelfDestroy();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
