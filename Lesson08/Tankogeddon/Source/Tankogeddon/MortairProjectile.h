// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"

#include "MortairProjectile.generated.h"

UCLASS()
class TANKOGEDDON_API AMortairProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Components)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Components)
	UParticleSystemComponent* TrailParticles;

	virtual void OnConstruction(const FTransform& Transform) override;

	// Sets default values for this actor's properties
	AMortairProjectile();

	// радиус сферы урона
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Projectile)
	float ExplosionRadius = 100;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, 
		AActor* Other, 
		class UPrimitiveComponent* OtherComp, 
		bool bSelfMoved, 
		FVector HitLocation, 
		FVector HitNormal, 
		FVector NormalImpulse, 
		const FHitResult& Hit) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
