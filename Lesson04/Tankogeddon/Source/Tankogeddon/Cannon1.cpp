// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon1.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACannon1::ACannon1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultRoot");
	RootComponent = DefaultRoot;

	Cannon1BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Cannon1BodyMesh");
	Cannon1BodyMesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>("ProjectileSpawnPoint");
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
}

void ACannon1::Shoot(int ShootType)
{
	if (ACannon1::AmmoCount <= 0)
	{
		GEngine->AddOnScreenDebugMessage(14, 2, FColor::Red, FString::Printf(TEXT("You don't have any ammo!")));
		return;
	}

	if (!IsReadyShoot)
	{
		return;
	}

	switch (CannonType)
	{
	case ECannonType::Projectile:
		if (ShootType == 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Projectile")));

			// ��������� ������� ������ ������
			if (ProjectileClass)
			{
				FActorSpawnParameters SpawnParameters;
				SpawnParameters.Owner = this;
				SpawnParameters.Instigator = GetInstigator();

				FTransform Transform = ProjectileSpawnPoint->GetComponentTransform();
				GetWorld()->SpawnActor(ProjectileClass, &Transform, SpawnParameters);
				// GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentTransform());
			}			
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("ProjectileSpecial")));			
		}		
		break;
	case ECannonType::Trace:
		if (ShootType == 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Trace")));

			// GetWorld()->SweepSingleByChannel();
			// GetWorld()->LineTraceTestByChannel();

			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);
			Params.AddIgnoredActor(GetInstigator());
			Params.bTraceComplex = true;
			auto LocStart = ProjectileSpawnPoint->GetComponentLocation();
			auto LocEnd = ProjectileSpawnPoint->GetComponentLocation() + ProjectileSpawnPoint->GetForwardVector() * TraceDistance;
			// ������ ������� ��� ��������
			DrawDebugLine(GetWorld(), LocStart, LocEnd, FColor::Cyan, false, 0.5f);
			// �������� ��� ��������
			if (GetWorld()->LineTraceSingleByChannel(HitResult, LocStart, LocEnd, ECollisionChannel::ECC_Visibility, Params))
			{
				if (HitResult.Actor.IsValid())
				{
					HitResult.Actor->Destroy();
				}
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("TraceSpecial")));
		}		
		break;
	}

	IsReadyShoot = false;
	GetWorldTimerManager().SetTimer(ReloadTimer, this, &ACannon1::OnReload, ReloadTime);

	ACannon1::AmmoCount--;
}

void ACannon1::AddAmmo(int count)
{
	ACannon1::AmmoCount += count;
}

// Called when the game starts or when spawned
void ACannon1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACannon1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(13, -1, FColor::Yellow, FString::Printf(TEXT("Reload in %f seconds."), GetWorldTimerManager().GetTimerRemaining(ReloadTimer)));
	GEngine->AddOnScreenDebugMessage(12, -1, FColor::Blue, FString::Printf(TEXT("Ammo %i count"), ACannon1::AmmoCount));
}

void ACannon1::OnReload()
{
	IsReadyShoot = true;
}

