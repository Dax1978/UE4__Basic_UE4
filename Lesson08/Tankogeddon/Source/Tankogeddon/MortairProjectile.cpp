// Fill out your copyright notice in the Description page of Project Settings.


#include "MortairProjectile.h"

// Sets default values
AMortairProjectile::AMortairProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ������� ���������� ����������
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");// ��� ������ ���������� ����� SetupAttachment, �.�. � ���� �� ���� ��� ��������
	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>("TrailParticles");
	TrailParticles->SetupAttachment(RootComponent);
}

void AMortairProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Other == this || GetInstigator() == Other)
	{
		return;
	}

	// ��� ����������� ������� � ������ ��������:
	if (Other)
	{
		// ������� ���� ...? ���� ��
		// 1. ������ ���������
		TArray<FHitResult> Results;
		// 2 � 3. ����� �����������, ��� �����, ���� ����� ������, �.�. ���������� HitLocation
		// 4. ���������� ���, ��� ��� � ��� ������ ��� �����
		// 5. ������ ���� ����� ����������
		// 6. �� ����� ����� ����� ����� ��������� �����, � ��� ����� �������� �������� � ��������� ���� ��������� ExplosionRadius
		// 7. ��������� ��������
		FCollisionQueryParams Params;
		Params.bTraceComplex = true;
		Params.AddIgnoredActor(this); // ������� ����

		if (GetWorld()->SweepMultiByChannel(Results, HitLocation, HitLocation, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(ExplosionRadius), Params))
		{
			DrawDebugSphere(GetWorld(), HitLocation, ExplosionRadius, 16, FColor::Red, false, 2);

			// ���������� ��� ���� ������������� ����, ���� �� ����� ������� ����
			// ����� ��������� ���, �.�. ���� ������� �� ���������� ����������, �� ��� ��� �������� � ���� ��������
			// � �������������� �� 1 ��������� ���� ����������� ��������� ��� �� ������� ����������
			// ��� ����������� ������� ��� ��� ���������, ��� ���� ����� ��� ���������� � �������� ����� TSet<AActor*> 
			
			for (auto HitResult : Results)
			{
				auto Target = HitResult.Actor;

				auto Damagable = Cast<IDamageTaker>(Target);
				// ���� ����� ������� ���� -> �������, ����� ������������ ������
				if (Damagable)
				{
					FDamageData Data;
					Data.DamageMaker = this;
					Data.DamageValue = Damage;
					Data.Instigator = GetInstigator();
					Damagable->TakeDamage(Data);

					// �������� ���������
					Damagable->EnemyHit();
				}

				if (Target.IsValid())
				{

					/*
					* ������ �� ����� ����������, � ����� ��������� ������
					else
					{
						Other->Destroy(); // ������ ������ ������������
					}
					*/

					// �������� root ��������� ���� � �������, ����� �� �� ��������� � ���� ������
					auto Root = Cast<UPrimitiveComponent>(Target->GetRootComponent());
					if (Root)
					{
						// ���������, �������� �� ������ � ���� (�������� �� ������� � ����� �������
						if (Root->IsSimulatingPhysics())
						{
							// ��������� ���� � ����� ������� � ����� �������
							// Root->AddImpulse()
							// ��������� ������� ���� � ����� ����������� ����� ����� ������ � ��������
							// GetActorForwardVector() - �� ���� �� � ��� �����������, � ����� � ��� ����� ������ ���������� ������ 
							// Root->AddImpulseAtLocation(GetActorForwardVector() * Damage * 100, HitResult.Location);
							// � ����� � ����
							// Vector - ������ �� ������ ������ � ����� �����������
							auto Vector = HitLocation - HitResult.Location;
							Vector.Normalize();
							Root->AddImpulseAtLocation(Vector * Damage * 100, HitResult.Location);
						}
					}
				}
			}
		}
	}
	Destroy(); // ������ ���� ������������
}

// Called when the game starts or when spawned
void AMortairProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMortairProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMortairProjectile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	// MovementComponent->InitialSpeed = MovementSpeed;
}

