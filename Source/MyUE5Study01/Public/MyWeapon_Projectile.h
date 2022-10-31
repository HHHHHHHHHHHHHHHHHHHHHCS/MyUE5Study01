// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyWeapon.h"
#include "MyWeapon_Projectile.generated.h"

/**
 * 
 */
UCLASS()
class MYUE5STUDY01_API AMyWeapon_Projectile : public AMyWeapon
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<AActor> projectileCls;

public :
	virtual void Fire() override;
};
