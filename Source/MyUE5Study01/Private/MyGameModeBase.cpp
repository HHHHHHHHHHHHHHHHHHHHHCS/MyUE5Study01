// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"

AMyGameModeBase::AMyGameModeBase()
{
	betweenWaveTime = 2.0f;
}

void AMyGameModeBase::StartPlay()
{
	Super::StartPlay();
}

void AMyGameModeBase::StartWave()
{
	waveCount++;
	botNumber = 2 * waveCount;
	GetWorldTimerManager().SetTimer(timerHandle_BotSpawner, this, &AMyGameModeBase::SpawnNewBot, 1.0f, true, 0.0f);
}

void AMyGameModeBase::EndWave()
{
	GetWorldTimerManager().ClearTimer(timerHandle_BotSpawner);
	PrepareNextWave();
}

void AMyGameModeBase::PrepareNextWave()
{
	GetWorldTimerManager().SetTimer(timerHandle_NextWaveStart, this, &AMyGameModeBase::StartWave, betweenWaveTime, true);
}

void AMyGameModeBase::SpawnBotTimeElapsed()
{
	SpawnNewBot();
	botNumber--;
	if (botNumber <= 0)
	{
		EndWave();
	}
}
