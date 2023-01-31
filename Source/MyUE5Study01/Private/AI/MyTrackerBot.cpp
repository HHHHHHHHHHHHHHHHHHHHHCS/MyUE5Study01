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
	meshComp->SetSimulatePhysics(true);
	healthComponent = CreateDefaultSubobject<UMyHealthComponent>(TEXT("HealthComp"));
	healthComponent->onHealthChanged.AddDynamic(this, &AMyTrackerBot::HandleTakeDamage);
	SetRootComponent(meshComp);
	useVelocityChange = true;
	movementForce = 1000;
	requireDistanceToTarget = 100.0f;
}

// Called when the game starts or when spawned
void AMyTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	nextPathPoint = GetNextPathPoint();
}

FVector AMyTrackerBot::GetNextPathPoint()
{
	AMyCharacter* playerPawn = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	UNavigationPath* navPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), playerPawn);
	if (navPath && navPath->PathPoints.Num() > 1)
	{
		return navPath->PathPoints[1];
	}
	return GetActorLocation();
}

// Called every frame
void AMyTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FVector::Distance(GetActorLocation(), nextPathPoint) > requireDistanceToTarget)
	{
		//执行移动
		FVector forceDir = nextPathPoint - GetActorLocation();
		forceDir.Normalize();
		forceDir *= movementForce;
		meshComp->AddForce(forceDir, NAME_None, useVelocityChange);
		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100, 32, FColor::Red);
	}
	else
	{
		nextPathPoint = GetNextPathPoint();
		DrawDebugString(GetWorld(), GetActorLocation(), "Target Reached!");
	}
	DrawDebugSphere(GetWorld(), nextPathPoint, 20, 12, FColor::Yellow);
}

void AMyTrackerBot::HandleTakeDamage(UMyHealthComponent* HealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy,
                                     AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("Health %s of  %s"), *FString::SanitizeFloat(Health), *DamageCauser->GetName());
}
