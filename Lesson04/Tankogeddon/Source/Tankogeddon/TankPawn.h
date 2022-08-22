// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Cannon1.h"

#include "TankPawn.generated.h"

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UBoxComponent* Collision;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UStaticMeshComponent* BodyMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	USpringArmComponent* Arm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UArrowComponent* CannonAttachment;

	// Sets default values for this pawn's properties
	ATankPawn();

	virtual void OnConstruction(const FTransform& Transform) override;

	void Shoot(int ShootType = 0);
	void MoveForward(float Scale);
	void MoveRight(float Scale);
	void RotateRight(float Scale);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	float Speed = 500;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	float SpeedRotation = 200;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	float Acceleration = 0.1f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	float TurretRotationSpeed = 0.1f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	TSubclassOf<ACannon1> CannonClass;

	virtual void PossessedBy(AController* NewController) override;

	void SetupCannon(TSubclassOf<ACannon1> InCannonClass);
	void AddAmmo(int count);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float MoveForwardScale = 0;
	float MoveRightScale = 0;
	float RotateRightScale = 0;

	float CurrentMoveForwardScale = 0;
	float CurrentRotateRightScale = 0;

	class ATankPlayerController* TankController;

	UPROPERTY()
	ACannon1* Cannon;
};
