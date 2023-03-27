// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"

#include "EngineUtils.h"
#include "MyHealthComponent.h"

AMyGameModeBase::AMyGameModeBase()
{
	betweenWaveTime = 2.0f;
	isAnyBotAlive = false;
}

void AMyGameModeBase::StartPlay()
{
	Super::StartPlay();
	PrepareNextWave();
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void AMyGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckWaveState();
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

void AMyGameModeBase::CheckWaveState()
{
	bool isPrepareWave = GetWorldTimerManager().IsTimerActive(timerHandle_NextWaveStart);
	if (isPrepareWave && botNumber > 0)
	{
		return;
	}
	isAnyBotAlive = false;
	for (TActorIterator<APawn> it(GetWorld()); it; ++it)
	{
		APawn* pawn = *it;
		if (pawn == nullptr || pawn->IsPawnControlled())
		{
			continue;
		}

		auto healthComp = Cast<UMyHealthComponent>(pawn->GetComponentByClass(UMyHealthComponent::StaticClass()));
		if (healthComp && healthComp->GetHealth() > 0.0f)
		{
			isAnyBotAlive = true;
			break;
		}
	}
}

void AMyGameModeBase::GameOver()
{
	EndWave();
}

void AMyGameModeBase::CheckAnyPlayerAlive()
{
	for (auto it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		APlayerController* pc = it->Get();
		if (pc && pc->GetPawn())
		{
			APawn* myPawn = pc->GetPawn();
			UMyHealthComponent* healthComp = Cast<UMyHealthComponent>(myPawn->GetComponentByClass(UMyHealthComponent::StaticClass()));
			if (ensure(healthComp) && healthComp->GetHealth() > 0)
			{
				// 玩家存在
				return;
			}
		}
	}

	GameOver();
}
