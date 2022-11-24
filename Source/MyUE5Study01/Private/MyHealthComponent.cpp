// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHealthComponent.h"

// Sets default values for this component's properties
UMyHealthComponent::UMyHealthComponent()
{
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
}
