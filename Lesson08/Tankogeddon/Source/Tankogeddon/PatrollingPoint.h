// Fill out your copyright notice in the Description page of Project Settings.

// В основном здесь будет индекс текущей точки патрулирования

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrollingPoint.generated.h"

UCLASS()
class TANKOGEDDON_API APatrollingPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrollingPoint();

	// В принципе больше ничего не нужно
	// Все остальное будем добавлять в блюпринтах
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Patrolling)
	int Index = -1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
