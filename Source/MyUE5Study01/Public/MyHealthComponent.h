// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyHealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSingnature, class UMyHealthComponent*, HealthComp, float, Health, float, HealthDelta, const class UDamageType*,
                                             DamageType, class AController*, InstigatedBy, class AActor*, DamageCauser);

UCLASS(ClassGroup=(COOP), meta=(BlueprintSpawnableComponent))
class MYUE5STUDY01_API UMyHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player")
	float defaultHealth;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnHealthChangedSingnature onHealthChanged;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category="Player")
	float health;

public:
	// Sets default values for this component's properties
	UMyHealthComponent();

	FORCEINLINE float GetHealth() const { return health; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);

	UFUNCTION()
	void OnRep_Health(float oldHealth);

	UFUNCTION(BlueprintCallable)
	void AddHealth(float value);
};
