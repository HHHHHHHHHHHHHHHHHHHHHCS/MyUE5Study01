// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPowerUpActor.generated.h"

UCLASS()
class MYUE5STUDY01_API AMyPowerUpActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Powerups")
	float powerupInterval;

	UPROPERTY(EditDefaultsOnly, Category="Powerups")
	int32 totalOfTicks;

private:
	FTimerHandle TimerHandle_PowerupTicks;
	//已经处理过的更新
	int32 tickProcessed;

public:
	// Sets default values for this actor's properties
	AMyPowerUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//应用增强效果更新时候
	UFUNCTION()
	void OnTickPowerup();

	//激活增强道具时
	void ActivePowerup();

	//当激活时 在蓝图中实现
	UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
	void OnActived();

	//当失效时 在蓝图中实现
	UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
	void OnExpired();

	//应用增强更新时候 在蓝图中实现
	UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
	void OnPowerupTicked();
};
