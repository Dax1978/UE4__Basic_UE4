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
// ��� �������� �������� ��� �����������
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
	// ����� ������ �������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UArrowComponent* TankSpawnPoint;
	// ��������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UHealthComponent* HealthComponent;

	// ����� ������ ��� �������� �����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UAudioComponent* AudioEffectCreateTank;
	// ������� ��� �����������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UParticleSystemComponent* ParticlesEffectDeath;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UAudioComponent* AudioEffectDeath;

	// Sets default values for this actor's properties
	ATankFactory();

	// ����� ���� ����� ������
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tank Factory")
	TSubclassOf<ATankPawn> TankClass;
	// ��� ����� ����� ���������� �����
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tank Factory")
	float SpawnRate = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tank Factory")
	FName PatrollingPointTag;	// ��� ��� ������ �������� � ����

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tank Factory")
	class AMapLoader* MapLoader;

	// ��� �������� ���� �� ������
	// �� ��� �� ��������� ��������? ���� ����� Pawn �������� � ��� ��� �������� ����� ��� ��������� ������
	// ����� ���� ������ �������� �� �����, ��� ���������� � ������ �����
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
	// ��� ��������� �����
	virtual void TakeDamage(FDamageData Damage) override;
	// ��������� ������
	virtual void EnemyHit() override;

private:
	void OnDamaged(FDamageData Damage);
	void OnDeath();

	void OnSpawnTick();

	FTimerHandle SpawnTimer;

	// ����� ��� ������� �� (�� �� ��������) ����� ������������ ���������� ���������
	FTimerHandle AliveTimer;
	float CurrentAlive = 0;
};
