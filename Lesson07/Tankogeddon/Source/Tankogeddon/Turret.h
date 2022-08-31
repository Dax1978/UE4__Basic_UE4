// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"		// Для UBoxComponent
#include "Components/ArrowComponent.h"		// Для UArrowComponent
#include "Cannon1.h"						// Для <ACannon1> CannonClass
#include "HealthComponent.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.h"
//#include "CommonPawn.h"
#include "MapLoader.h"

// для создания эффектов при уничтожении
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

#include "Turret.generated.h"

UCLASS()
class TANKOGEDDON_API ATurret : public APawn, public IDamageTaker
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UBoxComponent* Collision;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UStaticMeshComponent* BodyMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UStaticMeshComponent* TurretMesh;

	// Для определения сферы в пределах которой турель будет стрелять по появившимся там акторам\павнам
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	USphereComponent* TargetRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UArrowComponent* CannonAttachment;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UHealthComponent* HealthComponent;

	// Эффекты при уничтожении
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UParticleSystemComponent* ParticlesEffect;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UAudioComponent* AudioEffect;
	// Эффекты при попадании
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UParticleSystemComponent* ParticlesEffectHit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UAudioComponent* AudioEffectHit;

	// Sets default values for this actor's properties
	ATurret();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	TSubclassOf<ACannon1> CannonClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	float TurretRotationSpeed = 0.1f;
	// Допустимая точность прицеливания
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	float Accuracy = 10;

	// для проверки жива ли турель
	// но это не корректно наверное? ведь тогда Pawn остается и все еще является целью для вражеских танков
	// тогда надо делать проверку на жизнь, при добовлении в список целей
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	bool isAlive = true;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = MovementParams)
	float AliveTime = 777;

	// для загрузки уровней
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tank Factory")
	class AMapLoader* MapLoader;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Вызывается когда будет уничтожена
	virtual void Destroyed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TakeDamage(FDamageData Damage) override;

	// Попадание врагом
	virtual void EnemyHit() override;

private:
	UFUNCTION()
	void OnTargetRangeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTargetRangeEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* Other, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Функция, которая ищет лучшую цель
	void FindBestTarget();
	// Прицеливвание
	void Targetting();
	// Можем ли мы попасть из турели в выбранную цель
	bool CanFire();

	void OnDamaged(FDamageData Damage);
	void OnDeath();


	UPROPERTY()
	ACannon1* Cannon;

	// Массив акторов, вошедших в сферу и соответственно это массив целей
	TArray<TWeakObjectPtr<AActor>> Targets;
	TWeakObjectPtr<AActor> CurrentTarget;	

	// живет еще сколько то (но не стреляет) после критического количества попаданий
	//void OnReload();
	FTimerHandle AliveTimer;
	float CurrentAlive = 0;
};
