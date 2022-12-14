// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "GameFramework//PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyUE5Study01/MyUE5Study01.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->bUsePawnControlRotation = true;
	springArmComp->SetupAttachment(GetRootComponent());

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(springArmComp);

	ACharacter::GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	zoomFOVScale = 2.0f;
	fovSpeed = 0.25f;

	weaponAttachSocketName = "MyWeaponSocket";

	healthComponent = CreateDefaultSubobject<UMyHealthComponent>(TEXT("HealthComp"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	defaultFOV = cameraComp->FieldOfView;


	if (defaultWeaponCls)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		currWeapon = GetWorld()->SpawnActor<AMyWeapon>(defaultWeaponCls, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
		currWeapon->SetOwner(this);
		currWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, weaponAttachSocketName);
	}

	if (GetController() == UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (uiCrosshairsCls)
		{
			UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), uiCrosshairsCls);
			widget->AddToViewport();
		}

		if (uiHealthIndicatorCls)
		{
			UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), uiHealthIndicatorCls);
			widget->AddToViewport();
			UImage* ui_img_health = Cast<UImage>(widget->GetWidgetFromName(FName("Image_Health")));
			mat_img_health = ui_img_health->GetDynamicMaterial();
			mat_img_health->SetScalarParameterValue(FName("Alpha"), 1.0f);
		}
	}

	healthComponent->onHealthChanged.AddDynamic(this, &AMyCharacter::OnHealthChanged);
}

void AMyCharacter::MoveForward(float val)
{
	AddMovementInput(GetActorForwardVector() * val);
}

void AMyCharacter::MoveRight(float val)
{
	AddMovementInput(GetActorRightVector() * val);
}

void AMyCharacter::BeginCrouch()
{
	Crouch();
}

void AMyCharacter::EndCrouch()
{
	UnCrouch();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float targetFOV = bWantsToZoom ? defaultFOV / zoomFOVScale : defaultFOV;
	cameraComp->SetFieldOfView(FMath::Lerp(cameraComp->FieldOfView, targetFOV, fovSpeed));
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AMyCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMyCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMyCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AMyCharacter::BeginZoomFOV);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AMyCharacter::EndZoomFOV);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMyCharacter::StopFire);
}

FVector AMyCharacter::GetPawnViewLocation() const
{
	if (cameraComp)
	{
		return cameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

void AMyCharacter::StartFire()
{
	if (currWeapon)
	{
		currWeapon->StartFire();
	}
}

void AMyCharacter::StopFire()
{
	if (currWeapon)
	{
		currWeapon->StopFire();
	}
}

void AMyCharacter::OnHealthChanged(UMyHealthComponent* HealthComp, float Health, float HealthDelta, const UDamageType*
                                   DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (mat_img_health)
	{
		float val = Health / healthComponent->defaultHealth;
		mat_img_health->SetScalarParameterValue(FName("Alpha"), val);
	}

	if (Health <= 0 && !bDied)
	{
		//处理死亡状态
		bDied = true;
		StopFire();
		GetMovementComponent()->StopMovementImmediately();
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);
	}
}

void AMyCharacter::BeginZoomFOV()
{
	bWantsToZoom = true;
}

void AMyCharacter::EndZoomFOV()
{
	bWantsToZoom = false;
}
