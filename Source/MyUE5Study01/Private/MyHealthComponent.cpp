// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHealthComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UMyHealthComponent::UMyHealthComponent()
{
	defaultHealth = 100;
	SetIsReplicated(true);
}


// Called when the game starts
void UMyHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	// 仅为服务器的时候, 生命值和伤害时间挂钩
	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* myOwner = GetOwner();
		if (myOwner)
		{
			myOwner->OnTakeAnyDamage.AddDynamic(this, &UMyHealthComponent::TakeDamage);
		}
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

void UMyHealthComponent::OnRep_Health(float oldHealth)
{
	float damage = health - oldHealth;
	onHealthChanged.Broadcast(this, health, damage, nullptr, nullptr, nullptr);
}

void UMyHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMyHealthComponent, health);
}
