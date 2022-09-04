// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBaseClass.h"

// Sets default values
AProjectileBaseClass::AProjectileBaseClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// сфера, с которой рассчитывается пересечение
	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	RootComponent = Collision;
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBaseClass::OnProjectileBeginOverlap);

	ProjectileBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileBodyMesh");
	ProjectileBodyMesh->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");// его нельзя подключить через SetupAttachment, т.к. у него по сути нет сущности

	// СОЗДАЕМ ЭФФЕКТЫ
	// при полете
	ParticlesEffectTrail = CreateDefaultSubobject<UParticleSystemComponent>("ParticlesEffectTrail");
	ParticlesEffectTrail->SetupAttachment(RootComponent);
	AudioEffectTrail = CreateDefaultSubobject<UAudioComponent>("AudioEffectTrail");
	AudioEffectTrail->SetupAttachment(RootComponent);
	// при попадании
	ParticlesEffectHit = CreateDefaultSubobject<UParticleSystemComponent>("ParticlesEffectHit");
	ParticlesEffectHit->SetupAttachment(RootComponent);
	AudioEffectHit = CreateDefaultSubobject<UAudioComponent>("AudioEffectHit");
	AudioEffectHit->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectileBaseClass::BeginPlay()
{
	Super::BeginPlay();
	
	// Записываем начальную координату, от куда вылетел снаряд
	StartLocation = GetActorLocation();

	// Активируем эффекты в полете снаряда
	ParticlesEffectTrail->ActivateSystem();
	AudioEffectTrail->Play();
}

// Called every frame
void AProjectileBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Меняем координату в каждом кадре в зависимости от скорости (по прямой)
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * MovementSpeed * DeltaTime, true);
	// Уничтожаем снаряд, при достижении максимального расстояния
	if (FVector::DistXY(StartLocation, GetActorLocation()) > FlyDistance)
	{
		Destroy();
	}
}

void AProjectileBaseClass::AddDamaging(AActor* Other)
{
	// Активируем эффекты при попадании снаряда вр что-то
	ParticlesEffectHit->ActivateSystem();
	AudioEffectHit->Play();
	// Если можем нанести урон -> наносим
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
	// получаем root компонент цели и смотрим, можем ли мы применить к нему физику
	auto Root = Cast<UPrimitiveComponent>(Other->GetRootComponent());
	if (Root)
	{
		// проверяем, включена ли физика у цели (включена ли галочка у этого объекта
		if (Root->IsSimulatingPhysics())
		{
			// прилагаем силу к этому объекту в центр объекта
			// Root->AddImpulse()
			// прилагаем импульс силы в точке попадания
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

	// При пересечении снаряда с другим объектом:
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

	// При пересечении снаряда с другим объектом:
	if (Other)
	{
		// 1. Results - список попаданий
		TArray<FHitResult> Results;
		// 2 и 3. HitLocation и HitLocation будут одинаковыми, это точка, куда попал снаряд
		// 4. FQuat::Identity кватернион нет, так как у нас снаряд это сфера
		// 5. ECollisionChannel::ECC_Visibility
		// 6. FCollisionShape::MakeSphere(ExplosionRadius) то какую форму будет иметь нанесение урона, у нас сфера радиусом заданным в созданном нами параметре ExplosionRadius
		// 7. Params - Параметры коллизии
		FCollisionQueryParams Params;
		Params.bTraceComplex = true;
		Params.AddIgnoredActor(this); // игнорим себя
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