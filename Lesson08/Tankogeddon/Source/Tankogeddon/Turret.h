// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"		// ��� UBoxComponent
#include "Components/ArrowComponent.h"		// ��� UArrowComponent
#include "Cannon1.h"						// ��� <ACannon1> CannonClass
#include "HealthComponent.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.h"
//#include "CommonPawn.h"
#include "MapLoader.h"

// ��� �������� �������� ��� �����������
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

	// ��� ����������� ����� � �������� ������� ������ ����� �������� �� ����������� ��� �������\������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	USphereComponent* TargetRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UArrowComponent* CannonAttachment;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UHealthComponent* HealthComponent;

	// ������� ��� �����������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UParticleSystemComponent* ParticlesEffect;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UAudioComponent* AudioEffect;
	// ������� ��� ���������
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
	// ���������� �������� ������������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	float Accuracy = 10;

	// ��� �������� ���� �� ������
	// �� ��� �� ��������� ��������? ���� ����� Pawn �������� � ��� ��� �������� ����� ��� ��������� ������
	// ����� ���� ������ �������� �� �����, ��� ���������� � ������ �����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	bool isAlive = true;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = MovementParams)
	float AliveTime = 777;

	// ��� �������� �������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tank Factory")
	class AMapLoader* MapLoader;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// ���������� ����� ����� ����������
	virtual void Destroyed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TakeDamage(FDamageData Damage) override;

	// ��������� ������
	virtual void EnemyHit() override;

private:
	UFUNCTION()
	void OnTargetRangeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTargetRangeEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* Other, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// �������, ������� ���� ������ ����
	void FindBestTarget();
	// �������������
	void Targetting();
	// ����� �� �� ������� �� ������ � ��������� ����
	bool CanFire();

	void OnDamaged(FDamageData Damage);
	void OnDeath();


	UPROPERTY()
	ACannon1* Cannon;

	// ������ �������, �������� � ����� � �������������� ��� ������ �����
	TArray<TWeakObjectPtr<AActor>> Targets;
	TWeakObjectPtr<AActor> CurrentTarget;	

	// ����� ��� ������� �� (�� �� ��������) ����� ������������ ���������� ���������
	//void OnReload();
	FTimerHandle AliveTimer;
	float CurrentAlive = 0;
};
