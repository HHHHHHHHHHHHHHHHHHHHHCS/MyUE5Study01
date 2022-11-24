// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyHealthComponent.generated.h"


UCLASS(ClassGroup=(COOP), meta=(BlueprintSpawnableComponent))
class MYUE5STUDY01_API UMyHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player")
	float health;


public:
	// Sets default values for this component's properties
	UMyHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
