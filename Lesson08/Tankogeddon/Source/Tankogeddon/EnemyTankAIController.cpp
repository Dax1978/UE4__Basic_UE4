// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyTankAIController.h"
#include "Kismet/KismetMathLibrary.h"

void AEnemyTankAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (TankPawn)
	{
		TankPawn->OnTargetsChanged.Remove(TargetChangedDelegateHandle);
	}

	TankPawn = Cast<ATankPawn>(InPawn);
	if (TankPawn)
	{
		PatrollingPointTag = TankPawn->PatrollingPointTag;
		RebuildWaypoints();

		TargetChangedDelegateHandle = TankPawn->OnTargetsChanged.AddUObject(this, &AEnemyTankAIController::OnTargetsChanged);
	}
}

void AEnemyTankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!TankPawn || PatrollingPoints.Num() < 2)
	{
		return;
	}

	if (CurrentWaypointIndex >= PatrollingPoints.Num() || CurrentWaypointIndex < 0)
	{
		CurrentWaypointIndex = 0;
	}

	auto Point = PatrollingPoints[CurrentWaypointIndex];
	auto PointLocation = Point->GetActorLocation();
	auto Location = TankPawn->GetActorLocation();

	// ������ ����, ������ �� �� ������������� ���� ��� ���
	auto MovementDirection = PointLocation - Location;
	MovementDirection.Normalize();
	// ���� ����� �������� ����������� � ����� � ���� ��� ���� ��������
	auto ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TankPawn->GetActorForwardVector(), MovementDirection)));
	auto RightAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TankPawn->GetActorRightVector(), MovementDirection)));
	float RotationValue = 0;
	if (ForwardAngle > 5) RotationValue = 1;	
	if (RightAngle > 90) RotationValue = -RotationValue;
	// ����� ��������, ��� ���� ���� ������ 45 �� �� ������������� ��������, ��� �� ��������� ���� �� ������� �����
	if (ForwardAngle > 90)
	{
		TankPawn->MoveForward(0.1);
	}	
	else
	{
		TankPawn->MoveForward(1);
	}

	TankPawn->RotateRight(RotationValue);

	if (FVector::Dist2D(PointLocation, Location) < MovementAccuracy)
	{
		CurrentWaypointIndex++;
	}

	if (CanFire())
	{
		TankPawn->Shoot();
	}
}

void AEnemyTankAIController::BeginPlay()
{
	Super::BeginPlay();

	RebuildWaypoints();
}

void AEnemyTankAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

FVector AEnemyTankAIController::GetTargetLocation() const
{
	if (CurrentTarget.IsValid())
	{
		return CurrentTarget->GetActorLocation();
	}

	if (TankPawn)
	{
		return TankPawn->GetActorLocation() + TankPawn->GetActorForwardVector() * 1000;
	}

	return FVector::ZeroVector;
}

void AEnemyTankAIController::RebuildWaypoints()
{
	// ����� ��������� ��� ������ PatrollingPoints
	// �������� ������ ����� ��� ����������
	PatrollingPoints.Empty();

	CurrentWaypointIndex = 0;

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APatrollingPoint::StaticClass(), PatrollingPointTag, Actors);

	// ��� ����������
	TArray<APatrollingPoint*> Points;
	for (auto Actor : Actors)
	{
		// �������� ����� ����������� � ������ �� ������ ����, ���� ����� ������� ������ ������ ����, �� ���������������� APatrollingPoint
		Points.Add(CastChecked<APatrollingPoint>(Actor));
	}

	// ������ ����������� ������ �� Index
	Points.Sort([](const APatrollingPoint& A, const APatrollingPoint& B) {
		return A.Index > B.Index;
	});

	for (auto Actor : Points)
	{		
		PatrollingPoints.Add(Actor);
	}

	for (int i = 1; i < PatrollingPoints.Num(); i++)
	{
		// PatrollingPoints[i - 1]->GetActorLocation();
		// +FVector(0, 0, 20) ������������ ��� ����� �� 20 ��
		DrawDebugLine(GetWorld(),
			PatrollingPoints[i - 1]->GetActorLocation() + FVector(0, 0, 20),
			PatrollingPoints[i]->GetActorLocation() + FVector(0, 0, 20),
			FColor::Green,
			false,
			20);
		/*
		#include "DrawDebugHelpers.h"
		void DrawDebugLine
		(
			const UWorld * InWorld,
			FVector const& LineStart,	// ������
			FVector const& LineEnd,		// �����
			FColor const& Color,		// ����
			bool bPersistentLines,		//
			float LifeTime,				// ����� �����
			uint8,						//
			float Thickness				// �������
		)
		*/
		// ������� ��������� � ������ �����
		if (PatrollingPoints.Num() > 1)
		{
			DrawDebugLine(GetWorld(),
				PatrollingPoints[0]->GetActorLocation() + FVector(0, 0, 20),
				PatrollingPoints[PatrollingPoints.Num() - 1]->GetActorLocation() + FVector(0, 0, 20),
				FColor::Green,
				false,
				20);
		}
	}
}

void AEnemyTankAIController::OnTargetsChanged()
{
	FindBestTarget();
}

//void AEnemyTankAIController::Targetting()
//{
//	if (!CurrentTarget.IsValid())
//	{
//		return;
//	}
//	auto Rotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), CurrentTarget->GetActorLocation());
//	auto CurrentRotation = TurretMesh->GetComponentRotation();
//	Rotation.Pitch = CurrentRotation.Pitch;
//	Rotation.Roll = CurrentRotation.Roll;
//	CurrentRotation = FMath::Lerp(CurrentRotation, Rotation, TurretRotationSpeed);
//	TurretMesh->SetWorldRotation(CurrentRotation);
//}

bool AEnemyTankAIController::CanFire()
{
	if (!CurrentTarget.IsValid() || !TankPawn)
	{
		return false;
	}

	auto Rotation = UKismetMathLibrary::FindLookAtRotation(TankPawn->TurretMesh->GetComponentLocation(), CurrentTarget->GetActorLocation());
	auto CurrentRotation = TankPawn->TurretMesh->GetComponentRotation();
	if (FMath::Abs(Rotation.Yaw - CurrentRotation.Yaw) <= Accuracy)
	{
		// ��������� �������� ��������� ����
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetInstigator());
		Params.bTraceComplex = true;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, 
			TankPawn->CannonAttachment->GetComponentLocation(),
			CurrentTarget->GetActorLocation(),
			ECollisionChannel::ECC_Visibility, 
			Params))
		{
			if (HitResult.Actor == CurrentTarget)
			{
				return true; // ���� ��� ��������� ����, �� �������� ���������
			}
		}
	}
	return false;
}

void AEnemyTankAIController::FindBestTarget()
{
	if (!TankPawn)
	{
		return;
	}

	float MinDistance = 1000000000;
	auto Location = TankPawn->GetActorLocation();
	AActor* TargetF = nullptr;
	for (auto Target : TankPawn->GetPossibleTargets()) {
		if (Target.IsValid())
		{
			// ������ ������� ��� �� ���� ����� ���� ������ ������ ������!
			auto PlayerController = Cast<APlayerController>(Target->GetController());
			if (PlayerController)
			{
				auto Distance = FVector::DistXY(Location, Target->GetActorLocation());
				if (Distance < MinDistance)
				{
					MinDistance = Distance;
					TargetF = Target.Get();
				}
			}
			
		}
	}

	CurrentTarget = TargetF;
}
