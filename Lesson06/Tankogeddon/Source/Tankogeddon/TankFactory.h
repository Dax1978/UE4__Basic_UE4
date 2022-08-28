// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "TankPawn.h"

#include "TankFactory.generated.h"

UCLASS()
class TANKOGEDDON_API ATankFactory : public AActor, public IDamageTaker
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UBoxComponent* Collision;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UStaticMeshComponent* BodyMesh;
	// точка спавна акторов
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UArrowComponent* TankSpawnPoint;
	// здоровье
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UHealthComponent* HealthComponent;

	// Sets default values for this actor's properties
	ATankFactory();

	// какой танк будет делать
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tank Factory")
	TSubclassOf<ATankPawn> TankClass;
	// как часто будут спавниться танки
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tank Factory")
	float SpawnRate = 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// для получения урона
	virtual void TakeDamage(FDamageData Damage) override;
	// Попадание врагом
	virtual void EnemyHit() override;

private:
	void OnDamaged(FDamageData Damage);
	void OnDeath();

	void OnSpawnTick();

	FTimerHandle SpawnTimer;
};
