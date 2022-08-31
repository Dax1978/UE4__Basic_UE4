// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PatrollingPoint.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"					// для DrawDebugLine
#include "TankPawn.h"
#include "TargetController.h"
#include "EnemyTankAIController.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API AEnemyTankAIController : public AAIController, public ITargetController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Patrolling)
	FName PatrollingPointTag;	// Таг для поиска объектов в мире

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Patrolling)
	float MovementAccuracy = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	float TurretRotationSpeed = 0.1f;
	// Допустимая точность прицеливания
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	float Accuracy = 10;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual FVector GetTargetLocation() const override;

private:
	void RebuildWaypoints();
	void OnTargetsChanged();
	// Функция, которая ищет лучшую цель
	void FindBestTarget();
	// Прицеливвание
	// void Targetting();
	// Можем ли мы попасть из турели в выбранную цель
	bool CanFire();

	ATankPawn* TankPawn;
	int CurrentWaypointIndex = 0;
	FDelegateHandle TargetChangedDelegateHandle;

	TArray<TWeakObjectPtr<APatrollingPoint>> PatrollingPoints;

	TWeakObjectPtr<AActor> CurrentTarget;
};
