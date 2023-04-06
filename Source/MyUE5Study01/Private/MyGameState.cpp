// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"

#include "Net/UnrealNetwork.h"

void AMyGameState::OnRep_WaveState(EWaveState oldState)
{
	WaveStateChanged(waveState, oldState);
}

void AMyGameState::SetWaveState(EWaveState newState)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		EWaveState oldState = waveState;
		waveState = newState;
		OnRep_WaveState(oldState);
	}
}


void AMyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyGameState, waveState);
}
