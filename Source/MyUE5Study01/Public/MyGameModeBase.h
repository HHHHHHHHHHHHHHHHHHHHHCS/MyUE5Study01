// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameState.h"
#include "MyGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorKilled, AActor*, VictimActor, AActor*, killerActor);

UCLASS()
class MYUE5STUDY01_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	float betweenWaveTime;

	UPROPERTY(BlueprintAssignable, Category="GameMode")
	FOnActorKilled onActorKilled;
	
private:
	FTimerHandle timerHandle_BotSpawner;
	FTimerHandle timerHandle_NextWaveStart;

	int waveCount;

	int botNumber;

	bool isAnyBotAlive;

public:
	AMyGameModeBase();

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnNewBot();

	//开始生成一波机器人
	void StartWave();

	//停止这波机器人的生成
	void EndWave();

	//准备下波机器人的生成
	void PrepareNextWave();

	void SpawnBotTimeElapsed();

	void CheckWaveState();

	void GameOver();

	//检测玩家存活状态
	void CheckAnyPlayerAlive();

	void SetWaveState(EWaveState newWaveState);
};
