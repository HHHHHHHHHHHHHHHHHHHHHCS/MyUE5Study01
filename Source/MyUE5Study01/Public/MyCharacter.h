// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyHealthComponent.h"
#include "MyWeapon.h"
#include "Camera/CameraComponent.h"
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
	TSubclassOf<UUserWidget> uiCrosshairsCls;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Player")
	UMyHealthComponent* healthComp;
	
private:
	bool bWantsToZoom;

	float defaultFOV;

	AMyWeapon* currWeapon;

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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	void StartFire();

	void StopFire();
	
private:
	void BeginZoomFOV();

	void EndZoomFOV();
};
