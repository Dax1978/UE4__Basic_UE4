// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"

// Sets default values
ATankFactory::ATankFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("BodyMesh");
	BodyMesh->SetupAttachment(RootComponent);
	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>("TankSpawnPoint");
	TankSpawnPoint->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->OnDamaged.AddUObject(this, &ATankFactory::OnDamaged);
	HealthComponent->OnDeath.AddUObject(this, &ATankFactory::OnDeath);

}

// Called when the game starts or when spawned
void ATankFactory::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ATankFactory::OnSpawnTick, SpawnRate, true);
}

// Called every frame
void ATankFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATankFactory::TakeDamage(FDamageData Damage)
{
	// HealthComponent->TakeDamage(Damage);
}

void ATankFactory::OnDamaged(FDamageData Damage)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, FString::Printf(TEXT("Tank health %f"), HealthComponent->HealthCur));
}

void ATankFactory::OnDeath()
{
	// Активируем эффекты перед уничтожением
	// ParticlesEffect->ActivateSystem();
	// AudioEffect->Play();
	// isAlive = false;
	Destroy();
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
}

void ATankFactory::OnSpawnTick()
{

}

void ATankFactory::EnemyHit()
{
	// Активируем эффекты при попадании
	// ParticlesEffectHit->ActivateSystem();
	// AudioEffectHit->Play();
}
