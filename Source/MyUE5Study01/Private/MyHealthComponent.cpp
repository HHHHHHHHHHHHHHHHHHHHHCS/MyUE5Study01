// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHealthComponent.h"

// Sets default values for this component's properties
UMyHealthComponent::UMyHealthComponent()
{
	defaultHealth = 100;
}


// Called when the game starts
void UMyHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* myOwner = GetOwner();
	if (myOwner)
	{
		myOwner->OnTakeAnyDamage.AddDynamic(this, &UMyHealthComponent::TakeDamage);
	}
	health = defaultHealth;
}

void UMyHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
	{
		return;
	}
	health = FMath::Max(health - Damage, 0);
	UE_LOG(LogTemp, Log, TEXT("HealthChanged: %s"), *FString::SanitizeFloat(health));
	onHealthChanged.Broadcast(this, health, Damage, DamageType, InstigatedBy, DamageCauser);
}
