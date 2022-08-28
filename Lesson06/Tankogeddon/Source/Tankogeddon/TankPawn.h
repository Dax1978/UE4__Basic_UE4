// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Cannon1.h"
#include "HealthComponent.h"
#include "DamageTaker.h"

#include "TankPawn.generated.h"

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn, public IDamageTaker
{
	GENERATED_BODY()

	DECLARE_EVENT(ATankPawn, FOnTargetsChanged);

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

	// Для определения сферы в пределах которой турель будет стрелять по появившимся там акторам\павнам 
	// (это нужно по сути только для танков врагов)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	USphereComponent* TargetRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UHealthComponent* HealthComponent;

	// Эффекты при уничтожении
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UParticleSystemComponent* ParticlesEffect;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UAudioComponent* AudioEffect;
	// Эффекты при попадании
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UParticleSystemComponent* ParticlesEffectHit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UAudioComponent* AudioEffectHit;

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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Cannon)
	int AmmoCount = 77;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	TSubclassOf<ACannon1> CannonClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	bool isAlive = true;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = MovementParams)
	float AliveTime = 250;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Patrolling)
	FName PatrollingPointTag;	// Таг для поиска объектов в мире

	virtual void PossessedBy(AController* NewController) override;

	void SetupCannon(TSubclassOf<ACannon1> InCannonClass);
	void AddAmmo(int count);

	FOnTargetsChanged OnTargetsChanged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TakeDamage(FDamageData Damage) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	const TArray<TWeakObjectPtr<AActor>>& GetPossibleTargets() const
	{
		return Targets;
	}

	// Попадание врагом
	virtual void EnemyHit() override;

private:
	void OnDamaged(FDamageData Damage);
	void OnDeath();

	UFUNCTION()
	void OnTargetRangeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTargetRangeEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* Other, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	float MoveForwardScale = 0;
	float MoveRightScale = 0;
	float RotateRightScale = 0;

	float CurrentMoveForwardScale = 0;
	float CurrentRotateRightScale = 0;

	class ITargetController* TankController;

	UPROPERTY()
	ACannon1* Cannon;

	TArray<TWeakObjectPtr<AActor>> Targets;

	float CurrentAlive = 0;
};
