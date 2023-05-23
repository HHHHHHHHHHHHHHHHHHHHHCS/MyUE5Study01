// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyTrackerBot.h"

#include "EngineUtils.h"
#include "MyCharacter.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

static int32 k_DebugTrackBot = 0;
FAutoConsoleVariableRef ACVR_DebugTrackBot(TEXT("My.DebugTrackBot"), k_DebugTrackBot,
                                           TEXT("Draw debug track bot line."), ECVF_Cheat);

// Sets default values
AMyTrackerBot::AMyTrackerBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComp->SetCanEverAffectNavigation(false);
	meshComp->SetSimulatePhysics(true);
	SetRootComponent(meshComp);

	healthComponent = CreateDefaultSubobject<UMyHealthComponent>(TEXT("HealthComp"));
	healthComponent->onHealthChanged.AddDynamic(this, &AMyTrackerBot::HandleTakeDamage);

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	sphereComp->SetSphereRadius(200);
	sphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	sphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	sphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	sphereComp->SetupAttachment(RootComponent);

	useVelocityChange = true;
	movementForce = 1000;
	requireDistanceToTarget = 100.0f;
	isExploded = false;
	explosionDamage = 40;
	explosionRadius = 350;

	isStartDamageSelf = false;
	selfDamageInterval = 0.5f;
}

// Called when the game starts or when spawned
void AMyTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	if (GetLocalRole() == ROLE_Authority)
	{
		nextPathPoint = GetNextPathPoint();
	}
}

FVector AMyTrackerBot::GetNextPathPoint()
{
	float nearestTargetDistance = MAX_FLT;
	AActor* bestTarget = nullptr;
	for (TActorIterator<APawn> it(GetWorld()); it; ++it)
	{
		APawn* pawn = *it;
		if (pawn == nullptr || UMyHealthComponent::IsFriendly(pawn, this))
		{
			continue;
		}

		auto healthComp = Cast<UMyHealthComponent>(pawn->GetComponentByClass(UMyHealthComponent::StaticClass()));
		if (healthComp && healthComp->GetHealth() > 0.0f)
		{
			float dist = FVector::Distance(pawn->GetActorLocation(), GetActorLocation());
			if (dist < nearestTargetDistance)
			{
				nearestTargetDistance = dist;
				bestTarget = pawn;
			}
		}
	}

	if (bestTarget)
	{
		UNavigationPath* navPath = UNavigationSystemV1::FindPathToActorSynchronously(
			this, GetActorLocation(), bestTarget);

		GetWorldTimerManager().ClearTimer(timerHandle_refreshPath);
		GetWorldTimerManager().SetTimer(timerHandle_refreshPath, this, &AMyTrackerBot::RefreshNextPath,
		                                5.0f, false);

		if (navPath && navPath->PathPoints.Num() > 1)
		{
			return navPath->PathPoints[1];
		}
	}

	return GetActorLocation();
}

// Called every frame
void AMyTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetLocalRole() != ROLE_Authority || isExploded)
	{
		return;
	}
	if (FVector::Distance(GetActorLocation(), nextPathPoint) > requireDistanceToTarget)
	{
		//执行移动
		FVector forceDir = nextPathPoint - GetActorLocation();
		forceDir.Normalize();
		forceDir *= movementForce;
		meshComp->AddForce(forceDir, NAME_None, useVelocityChange);
		if (k_DebugTrackBot != 0)
		{
			DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(),
			                          GetActorLocation() + GetActorForwardVector() * 100, 32, FColor::Red);
		}
	}
	else
	{
		RefreshNextPath();
		if (k_DebugTrackBot != 0)
		{
			DrawDebugString(GetWorld(), GetActorLocation(), "Target Reached!");
		}
	}
	if (k_DebugTrackBot != 0)
	{
		DrawDebugSphere(GetWorld(), nextPathPoint, 20, 12, FColor::Yellow);
	}
}

void AMyTrackerBot::HandleTakeDamage(UMyHealthComponent* HealthComp, float Health, float HealthDelta,
                                     const UDamageType* DamageType, AController* InstigatedBy,
                                     AActor* DamageCauser)
{
	// UE_LOG(LogTemp, Log, TEXT("Health %s of  %s"), *FString::SanitizeFloat(Health), *DamageCauser->GetName());
	if (matInst == nullptr)
	{
		matInst = meshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, meshComp->GetMaterial(0));
	}
	if (matInst)
	{
		matInst->SetScalarParameterValue(TEXT("LastTimeDamage"), GetWorld()->TimeSeconds);
	}
	if (Health <= 0)
	{
		SelfDestroy();
	}
}

void AMyTrackerBot::RefreshNextPath()
{
	nextPathPoint = GetNextPathPoint();
}

void AMyTrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
	SelfDestroy();
}

void AMyTrackerBot::SelfDestroy()
{
	if (isExploded)
	{
		return;
	}

	isExploded = true;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, explodeSound, GetActorLocation());

	meshComp->SetVisibility(false, true);
	meshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	if (GetLocalRole() == ROLE_Authority)
	{
		TArray<AActor*> ignoredActors;
		ignoredActors.Add(this);

		UGameplayStatics::ApplyRadialDamage(this, explosionDamage, GetActorLocation(), explosionRadius,
		                                    nullptr, ignoredActors, this, GetInstigatorController(), true);
		SetLifeSpan(2.0f);
	}
}

void AMyTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (isStartDamageSelf || isExploded)
	{
		return;
	}

	AMyCharacter* character = Cast<AMyCharacter>(OtherActor);
	if (character && !UMyHealthComponent::IsFriendly(this, OtherActor))
	{
		if (GetLocalRole() == ROLE_Authority)
		{
			GetWorldTimerManager().SetTimer(timerHandle_damageSelf, this, &AMyTrackerBot::DamageSelf,
			                                selfDamageInterval, true, 0.0f);
		}
		isStartDamageSelf = true;
		UGameplayStatics::SpawnSoundAttached(selfDestroyCue, RootComponent);
	}
}
