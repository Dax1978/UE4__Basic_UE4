// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "TankPawn.h"
#include "Kismet/GameplayStatics.h"
#include "MapLoader.h"
// дл€ создани€ эффектов при уничтожении
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

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

	// јудио эффект при создании танка
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UAudioComponent* AudioEffectCreateTank;
	// Ёффекты при уничтожении
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UParticleSystemComponent* ParticlesEffectDeath;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UAudioComponent* AudioEffectDeath;

	// Sets default values for this actor's properties
	ATankFactory();

	// какой танк будет делать
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tank Factory")
	TSubclassOf<ATankPawn> TankClass;
	// как часто будут спавнитьс€ танки
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tank Factory")
	float SpawnRate = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tank Factory")
	FName PatrollingPointTag;	// “аг дл€ поиска объектов в мире

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tank Factory")
	class AMapLoader* MapLoader;

	// дл€ проверки жива ли турель
	// но это не корректно наверное? ведь тогда Pawn остаетс€ и все еще €вл€етс€ целью дл€ вражеских танков
	// тогда надо делать проверку на жизнь, при добовлении в список целей
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	bool isAlive = true;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = MovementParams)
	float AliveTime = 777;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// дл€ получени€ урона
	virtual void TakeDamage(FDamageData Damage) override;
	// ѕопадание врагом
	virtual void EnemyHit() override;

private:
	void OnDamaged(FDamageData Damage);
	void OnDeath();

	void OnSpawnTick();

	FTimerHandle SpawnTimer;

	// живет еще сколько то (но не стрел€ет) после критического количества попаданий
	FTimerHandle AliveTimer;
	float CurrentAlive = 0;
};
