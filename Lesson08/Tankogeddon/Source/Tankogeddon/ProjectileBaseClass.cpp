// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBaseClass.h"

// Sets default values
AProjectileBaseClass::AProjectileBaseClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �����, � ������� �������������� �����������
	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	RootComponent = Collision;
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBaseClass::OnProjectileBeginOverlap);

	ProjectileBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileBodyMesh");
	ProjectileBodyMesh->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");// ��� ������ ���������� ����� SetupAttachment, �.�. � ���� �� ���� ��� ��������

	// ������� �������
	// ��� ������
	ParticlesEffectTrail = CreateDefaultSubobject<UParticleSystemComponent>("ParticlesEffectTrail");
	ParticlesEffectTrail->SetupAttachment(RootComponent);
	AudioEffectTrail = CreateDefaultSubobject<UAudioComponent>("AudioEffectTrail");
	AudioEffectTrail->SetupAttachment(RootComponent);
	// ��� ���������
	ParticlesEffectHit = CreateDefaultSubobject<UParticleSystemComponent>("ParticlesEffectHit");
	ParticlesEffectHit->SetupAttachment(RootComponent);
	AudioEffectHit = CreateDefaultSubobject<UAudioComponent>("AudioEffectHit");
	AudioEffectHit->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectileBaseClass::BeginPlay()
{
	Super::BeginPlay();
	
	// ���������� ��������� ����������, �� ���� ������� ������
	StartLocation = GetActorLocation();

	// ���������� ������� � ������ �������
	ParticlesEffectTrail->ActivateSystem();
	AudioEffectTrail->Play();
}

// Called every frame
void AProjectileBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������ ���������� � ������ ����� � ����������� �� �������� (�� ������)
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * MovementSpeed * DeltaTime, true);
	// ���������� ������, ��� ���������� ������������� ����������
	if (FVector::DistXY(StartLocation, GetActorLocation()) > FlyDistance)
	{
		Destroy();
	}
}

void AProjectileBaseClass::AddDamaging(AActor* Other)
{
	// ���������� ������� ��� ��������� ������� �� ���-��
	ParticlesEffectHit->ActivateSystem();
	AudioEffectHit->Play();
	// ���� ����� ������� ���� -> �������
	auto Damagable = Cast<IDamageTaker>(Other);		
	if (Damagable)
	{
		FDamageData Data;
		Data.DamageMaker = this;
		Data.DamageValue = Damage;
		Data.Instigator = GetInstigator();
		Damagable->TakeDamage(Data);			
	}
}

void AProjectileBaseClass::AddPhysicalEffort(AActor* Other, FVector VectorPhysicalEffort, FVector PointPhysicalEffort)
{
	// �������� root ��������� ���� � �������, ����� �� �� ��������� � ���� ������
	auto Root = Cast<UPrimitiveComponent>(Other->GetRootComponent());
	if (Root)
	{
		// ���������, �������� �� ������ � ���� (�������� �� ������� � ����� �������
		if (Root->IsSimulatingPhysics())
		{
			// ��������� ���� � ����� ������� � ����� �������
			// Root->AddImpulse()
			// ��������� ������� ���� � ����� ���������
			Root->AddImpulseAtLocation(VectorPhysicalEffort * Damage * 100, PointPhysicalEffort);
		}
	}
}

void AProjectileBaseClass::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Other == this || GetInstigator() == Other || IsExplosion)
	{
		return;
	}

	// ��� ����������� ������� � ������ ��������:
	if (Other)
	{
		AddDamaging(Other);
		AddPhysicalEffort(Other, GetActorForwardVector(), SweepResult.Location);
	}
}

void AProjectileBaseClass::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Other == this || GetInstigator() == Other || !IsExplosion)
	{
		return;
	}

	// ��� ����������� ������� � ������ ��������:
	if (Other)
	{
		// 1. Results - ������ ���������
		TArray<FHitResult> Results;
		// 2 � 3. HitLocation � HitLocation ����� �����������, ��� �����, ���� ����� ������
		// 4. FQuat::Identity ���������� ���, ��� ��� � ��� ������ ��� �����
		// 5. ECollisionChannel::ECC_Visibility
		// 6. FCollisionShape::MakeSphere(ExplosionRadius) �� ����� ����� ����� ����� ��������� �����, � ��� ����� �������� �������� � ��������� ���� ��������� ExplosionRadius
		// 7. Params - ��������� ��������
		FCollisionQueryParams Params;
		Params.bTraceComplex = true;
		Params.AddIgnoredActor(this); // ������� ����
		if (GetWorld()->SweepMultiByChannel(Results, HitLocation, HitLocation, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(ExplosionRadius), Params))
		{
			DrawDebugSphere(GetWorld(), HitLocation, ExplosionRadius, 16, FColor::Red, false, 2);

			for (auto HitResult : Results)
			{
				auto Target = HitResult.Actor;
				auto TargetActor = Cast<AActor>(HitResult.Actor);
				AddDamaging(TargetActor);
				if (Target.IsValid())
				{
					auto Vector = HitLocation - HitResult.Location;
					Vector.Normalize();
					AddPhysicalEffort(Other, Vector, HitResult.Location);
				}
			}
		}
	}
}