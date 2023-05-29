// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyHealthComponent.h"
#include "MyWeapon.h"
#include "Camera/CameraComponent.h"
#include "Components/Image.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyCharacter.generated.h"

UCLASS()
class MYUE5STUDY01_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCameraComponent* cameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	float zoomFOVScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(ClampMin = 0.001, ClampMax = 0.999))
	float fovSpeed;

	// 默认生成的武器
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player")
	TSubclassOf<AMyWeapon> defaultWeaponCls;

	// 默认生成的武器的插槽名字
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player")
	FName weaponAttachSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player")
	UMyHealthComponent* healthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player")
	float deadDuration;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category="Player")
	bool bDied = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player")
	UParticleSystem* deadEffect;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UUserWidget> uiCrosshairsCls;

	UPROPERTY(Replicated, EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> uiHealthIndicatorCls;

protected:
	UPROPERTY(Replicated)
	AMyWeapon* currWeapon;

private:
	bool bWantsToZoom;

	float defaultFOV;

	UMaterialInstanceDynamic* mat_img_health;

	FTimerHandle dead_timerHandle;

	UUserWidget* ui_crosshairs;
	UUserWidget* ui_healthIndicator;
	
public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void MoveForward(float val);

	virtual void MoveRight(float val);

	virtual void BeginCrouch();

	virtual void EndCrouch();

	virtual void CharacterDead();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	UFUNCTION()
	void OnHealthChanged(UMyHealthComponent* HealthComp, float Health, float HealthDelta, const UDamageType* DamageType,
						AController* InstigatedBy, AActor* DamageCauser);

	void ResetPlayer();
	
private:
	void BeginZoomFOV();

	void EndZoomFOV();

	void AddInitUI();

	void RemoveInitUI();
};
