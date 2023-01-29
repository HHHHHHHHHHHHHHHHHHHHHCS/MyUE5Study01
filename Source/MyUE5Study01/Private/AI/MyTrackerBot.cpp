// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyTrackerBot.h"

#include "MyCharacter.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyTrackerBot::AMyTrackerBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComp->SetCanEverAffectNavigation(false);
	SetRootComponent(meshComp);
}

// Called when the game starts or when spawned
void AMyTrackerBot::BeginPlay()
{
	Super::BeginPlay();
}

FVector AMyTrackerBot::GetNextPathPoint()
{
	AMyCharacter* playerPawn = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	UNavigationPath* navPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), playerPawn);
	if(navPath->PathPoints.Num()>1)
	{
		return navPath->PathPoints[1];
	}
	return GetActorLocation();
}

// Called every frame
void AMyTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
