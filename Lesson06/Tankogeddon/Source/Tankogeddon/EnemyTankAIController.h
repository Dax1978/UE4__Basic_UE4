// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PatrollingPoint.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"					// ��� DrawDebugLine
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
	FName PatrollingPointTag;	// ��� ��� ������ �������� � ����

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Patrolling)
	float MovementAccuracy = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	float TurretRotationSpeed = 0.1f;
	// ���������� �������� ������������
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
	// �������, ������� ���� ������ ����
	void FindBestTarget();
	// �������������
	// void Targetting();
	// ����� �� �� ������� �� ������ � ��������� ����
	bool CanFire();

	ATankPawn* TankPawn;
	int CurrentWaypointIndex = 0;
	FDelegateHandle TargetChangedDelegateHandle;

	TArray<TWeakObjectPtr<APatrollingPoint>> PatrollingPoints;

	TWeakObjectPtr<AActor> CurrentTarget;
};
