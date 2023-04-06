// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameState.generated.h"


UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingToStart,
	WaveInProgress,
	WaitingToComplete,
	WaveComplete,
	GameOver,
};

/**
 * 
 */
UCLASS()
class MYUE5STUDY01_API AMyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_WaveState, Category="GameState")
	EWaveState waveState;

public:
	UFUNCTION()
	void OnRep_WaveState(EWaveState oldState);

	UFUNCTION(BlueprintImplementableEvent, Category="GameState")
	void WaveStateChanged(EWaveState newState, EWaveState oldState);

	void SetWaveState(EWaveState newState);
};
