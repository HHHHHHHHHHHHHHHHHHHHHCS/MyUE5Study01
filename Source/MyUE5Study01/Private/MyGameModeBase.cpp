// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"

#include "EngineUtils.h"
#include "MyHealthComponent.h"
#include "GameFramework/PlayerState.h"

AMyGameModeBase::AMyGameModeBase()
{
	betweenWaveTime = 2.0f;
	isAnyBotAlive = true;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
	GameStateClass = AMyGameState::StaticClass();
	PlayerStateClass = APlayerState::StaticClass();
}

void AMyGameModeBase::StartPlay()
{
	Super::StartPlay();
	PrepareNextWave();
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
	GetWorldTimerManager().SetTimer(timerHandle_BotSpawner, this, &AMyGameModeBase::SpawnBotTimeElapsed, 1.0f, true, 0.0f);
	SetWaveState(EWaveState::WaveInProgress);
}

void AMyGameModeBase::EndWave()
{
	GetWorldTimerManager().ClearTimer(timerHandle_BotSpawner);
	PrepareNextWave();
	SetWaveState(EWaveState::WaitingToComplete);
}

void AMyGameModeBase::PrepareNextWave()
{
	GetWorldTimerManager().SetTimer(timerHandle_NextWaveStart, this, &AMyGameModeBase::StartWave, betweenWaveTime, false);
	SetWaveState(EWaveState::WaitingToStart);
	RestartDeadPlayer();
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
	if (isPrepareWave || botNumber > 0)
	{
		return;
	}
	isAnyBotAlive = false;
	for (TActorIterator<APawn> it(GetWorld()); it; ++it)
	{
		APawn* pawn = *it;
		if (pawn == nullptr || pawn->IsPlayerControlled())
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
	if (!isAnyBotAlive)
	{
		SetWaveState(EWaveState::WaveComplete);
		PrepareNextWave();
	}
}

void AMyGameModeBase::GameOver()
{
	EndWave();
	SetWaveState(EWaveState::GameOver);
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

void AMyGameModeBase::SetWaveState(EWaveState newState)
{
	AMyGameState* gs = GetGameState<AMyGameState>();
	if (ensureAlways(gs))
	{
		gs->SetWaveState(newState);
	}
}

void AMyGameModeBase::RestartDeadPlayer()
{
	for (auto it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		APlayerController* pc = it->Get();
		if (pc && pc->GetPawn() == nullptr)
		{
			RestartPlayer(pc);
		}
	}
}
