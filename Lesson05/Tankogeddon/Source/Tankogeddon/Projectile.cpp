// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	RootComponent = Collision;
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileBeginOverlap);

	ProjectileBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileBodyMesh");
	ProjectileBodyMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * MovementSpeed * DeltaTime, true);

	if (FVector::DistXY(StartLocation, GetActorLocation()) > FlyDistance)
	{
		Destroy();
	}
}

void AProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Other == this || GetInstigator() == Other)
	{
		return;
	}

	// При пересечении снаряда с другим объектом:
	if (Other)
	{
		auto Damagable = Cast<IDamageTaker>(Other);
		// Если можем нанести урон -> наносим, иначе уничтожается объект
		if (Damagable)
		{
			FDamageData Data;
			Data.DamageMaker = this;
			Data.DamageValue = Damage;
			Data.Instigator = GetInstigator();
			Damagable->TakeDamage(Data);
		}
		else
		{
			Other->Destroy(); // Другой объект уничтожается
		}
	}
	Destroy(); // Снаряд тоже уничтожается
}