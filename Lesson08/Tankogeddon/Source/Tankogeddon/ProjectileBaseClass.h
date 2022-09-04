// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"
#include "DamageTaker.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"

#include "ProjectileBaseClass.generated.h"

UCLASS()
class TANKOGEDDON_API AProjectileBaseClass : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	USphereComponent* Collision;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UStaticMeshComponent* ProjectileBodyMesh;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Components)
	UProjectileMovementComponent* MovementComponent;
	// Эффекты при полете и попадании
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UParticleSystemComponent* ParticlesEffectTrail;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UAudioComponent* AudioEffectTrail;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UParticleSystemComponent* ParticlesEffectHit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UAudioComponent* AudioEffectHit;

	// Начальная скорость
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")
	float MovementSpeed = 700;
	// Максимальная дальность (после уничтожается)
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")
	float FlyDistance = 1000;
	// Наносимый урон
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")
	float Damage = 1;
	// будет взрыв или точечное попадание
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")	
	bool IsExplosion = true;
	// радиус сферы урона
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")
	float ExplosionRadius = 100;

	// Sets default values for this actor's properties
	AProjectileBaseClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector StartLocation;

	UFUNCTION()
	void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComp, 
		AActor* Other, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void NotifyHit(class UPrimitiveComponent* MyComp,
		AActor* Other,
		class UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit) override;

	void AddDamaging(AActor* Other);
	void AddPhysicalEffort(AActor* Other, FVector VectorPhysicalEffort, FVector PointPhysicalEffort);
};
