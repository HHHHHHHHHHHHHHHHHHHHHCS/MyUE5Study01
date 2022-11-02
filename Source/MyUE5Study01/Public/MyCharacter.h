// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

private:
	bool bWantsToZoom;

	float defaultFOV;
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
	virtual void SetupPlayerInputComponent( UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

private:
	void BeginZoomFOV();

	void EndZoomFOV();
};
