// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankPawn.h"
#include "TargetController.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankPlayerController : public APlayerController, public ITargetController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;

	//FVector GetWorldMousePosition() const
	//{
	//	return WorldMousePosition;
	//}

	virtual FVector GetTargetLocation() const override
	{
		return WorldMousePosition;
	}
 
private:
	void OnShoot();
	void OnShootSpecial();

	void OnMoveForward(float Scale);
	void OnMoveRight(float Scale);
	void OnRotateRight(float Scale);
	
	ATankPawn* Tank = nullptr;

	FVector WorldMousePosition;
};
