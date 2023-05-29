// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHealthComponent.h"

#include "MyGameModeBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UMyHealthComponent::UMyHealthComponent()
{
	defaultHealth = 100;
	SetIsReplicated(true);
	isDead = false;
	teamNum = 255;
}

bool UMyHealthComponent::IsFriendly(AActor* actorA, AActor* actorB)
{
	if (actorA == nullptr || actorB == nullptr)
	{
		return true;
	}

	if (actorA == actorB)
	{
		return true;
	}

	UMyHealthComponent* healthA = Cast<UMyHealthComponent>(
		actorA->GetComponentByClass(UMyHealthComponent::StaticClass()));
	UMyHealthComponent* healthB = Cast<UMyHealthComponent>(
		actorB->GetComponentByClass(UMyHealthComponent::StaticClass()));

	if (healthA == nullptr || healthB == nullptr)
	{
		return true;
	}

	return healthA->teamNum == healthB->teamNum;
}

void UMyHealthComponent::ResetHealth()
{
	health = defaultHealth;
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

void UMyHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                    AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0 && isDead)
	{
		return;
	}
	if (IsFriendly(DamagedActor, DamageCauser))
	{
		return;
	}
	health = FMath::Max(health - Damage, 0);
	UE_LOG(LogTemp, Log, TEXT("HealthChanged: %s"), *FString::SanitizeFloat(health));
	onHealthChanged.Broadcast(this, health, Damage, DamageType, InstigatedBy, DamageCauser);
	isDead = health <= 0;
	if (isDead)
	{
		AMyGameModeBase* gm = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
		if (gm)
		{
			gm->onActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}
}

void UMyHealthComponent::OnRep_Health(float oldHealth)
{
	float damage = health - oldHealth;
	onHealthChanged.Broadcast(this, health, damage, nullptr, nullptr, nullptr);
}

void UMyHealthComponent::AddHealth(float value)
{
	if (value <= 0 || health <= 0)
	{
		return;
	}
	health = FMath::Clamp(health + value, 0, defaultHealth);
	onHealthChanged.Broadcast(this, health, value, nullptr, nullptr, nullptr);
}

void UMyHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMyHealthComponent, health);
}
