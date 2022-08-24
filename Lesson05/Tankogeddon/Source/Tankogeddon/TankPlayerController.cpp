// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "DrawDebugHelpers.h"

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &ATankPlayerController::OnShoot);
	InputComponent->BindAction("ShootSpecial", EInputEvent::IE_Pressed, this, &ATankPlayerController::OnShootSpecial);
	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::OnMoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATankPlayerController::OnMoveRight);
	InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::OnRotateRight);

	SetShowMouseCursor(true);
}

void ATankPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Tank = Cast<ATankPawn>(InPawn);
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Tank)
	{
		FVector MousePosition;
		FVector MouseDirection;
		DeprojectMousePositionToWorld(MousePosition, MouseDirection);

		auto Z = FMath::Abs(Tank->GetActorLocation().Z - MousePosition.Z);
		WorldMousePosition = MousePosition - MouseDirection * Z / MouseDirection.Z;

		// DrawDebugLine(GetWorld(), MousePosition, WorldMousePosition, FColor::Blue, false, 2);
	}
}

void ATankPlayerController::OnShoot()
{
	if (Tank)
	{
		Tank->Shoot();
	}
}

void ATankPlayerController::OnShootSpecial()
{
	if (Tank)
	{
		Tank->Shoot(1);
	}
}

void ATankPlayerController::OnMoveForward(float Scale)
{
	if (Tank) 
	{
		Tank->MoveForward(Scale);
	}
}

void ATankPlayerController::OnMoveRight(float Scale)
{
	if (Tank)
	{
		Tank->MoveRight(Scale);
	}
}

void ATankPlayerController::OnRotateRight(float Scale)
{
	if (Tank)
	{
		Tank->RotateRight(Scale);
	}
}