// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrollingPoint.h"

// Sets default values
APatrollingPoint::APatrollingPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APatrollingPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APatrollingPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

