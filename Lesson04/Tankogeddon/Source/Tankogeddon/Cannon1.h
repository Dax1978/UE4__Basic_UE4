// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Projectile.h"

#include "Cannon1.generated.h"

UENUM()
enum class ECannonType
{
	Projectile,
	Trace
};

UCLASS()
class TANKOGEDDON_API ACannon1 : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	USceneComponent* DefaultRoot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UStaticMeshComponent* Cannon1BodyMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UArrowComponent* ProjectileSpawnPoint;

	// Sets default values for this actor's properties
	ACannon1();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Cannon)
	ECannonType CannonType = ECannonType::Projectile;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Cannon)
	float ReloadTime = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Cannon)
	int AmmoCount = 7;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Cannon)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Cannon)
	float TraceDistance = 1000;

	void Shoot(int ShootType = 0);
	void AddAmmo(int count);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void OnReload();
	FTimerHandle ReloadTimer;
	bool IsReadyShoot = true;
};
