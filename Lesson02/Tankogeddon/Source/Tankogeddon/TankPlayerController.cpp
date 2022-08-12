// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &ATankPlayerController::OnShoot);
	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::OnMoveForward);
}

void ATankPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Tank = Cast<ATankPawn>(InPawn);
}

void ATankPlayerController::OnShoot()
{
	if (Tank)
	{
		Tank->Shoot();
	}
}

void ATankPlayerController::OnMoveForward(float Scale)
{
	if (Tank) 
	{
		Tank->MoveForward(Scale);
	}
}