// Fill out your copyright notice in the Description page of Project Settings.


#include "MortairProjectile.h"

// Sets default values
AMortairProjectile::AMortairProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// создаем заявленные компоненты
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");// его нельзя подключить через SetupAttachment, т.к. у него по сути нет сущности
	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>("TrailParticles");
	TrailParticles->SetupAttachment(RootComponent);
}

void AMortairProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Other == this || GetInstigator() == Other)
	{
		return;
	}

	// При пересечении снаряда с другим объектом:
	if (Other)
	{
		// наносим урон ...? чему то
		// 1. список попаданий
		TArray<FHitResult> Results;
		// 2 и 3. будут одинаковыми, это точка, куда попал снаряд, т.е. полученный HitLocation
		// 4. кватернион нет, так как у нас снаряд это сфера
		// 5. колижн чанл будет визибилити
		// 6. то какую форму будет иметь нанесение урона, у нас сфера радиусом заданным в созданном нами параметре ExplosionRadius
		// 7. Параметры коллизии
		FCollisionQueryParams Params;
		Params.bTraceComplex = true;
		Params.AddIgnoredActor(this); // игнорим себя

		if (GetWorld()->SweepMultiByChannel(Results, HitLocation, HitLocation, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(ExplosionRadius), Params))
		{
			DrawDebugSphere(GetWorld(), HitLocation, ExplosionRadius, 16, FColor::Red, false, 2);

			// перебираем все наши потенциальные цели, кому мы можем нанести урон
			// здесь небольшой баг, т.к. танк состоит из нескольких компонетов, то все они попадают в этот перечень
			// и соответственно за 1 попадание урон засчитывает несколько раз по каждому компоненту
			// для исправления сделаем сет где проверять, что этот актор уже участвовал в подсчете урона TSet<AActor*> 
			
			for (auto HitResult : Results)
			{
				auto Target = HitResult.Actor;

				auto Damagable = Cast<IDamageTaker>(Target);
				// Если можем нанести урон -> наносим, иначе уничтожается объект
				if (Damagable)
				{
					FDamageData Data;
					Data.DamageMaker = this;
					Data.DamageValue = Damage;
					Data.Instigator = GetInstigator();
					Damagable->TakeDamage(Data);

					// вызываем попадание
					Damagable->EnemyHit();
				}

				if (Target.IsValid())
				{

					/*
					* теперь не будем уничтожать, а будем применять физику
					else
					{
						Other->Destroy(); // Другой объект уничтожается
					}
					*/

					// получаем root компонент цели и смотрим, можем ли мы применить к нему физику
					auto Root = Cast<UPrimitiveComponent>(Target->GetRootComponent());
					if (Root)
					{
						// проверяем, включена ли физика у цели (включена ли галочка у этого объекта
						if (Root->IsSimulatingPhysics())
						{
							// прилагаем силу к этому объекту в центр объекта
							// Root->AddImpulse()
							// прилагаем импульс силы в точке пересечения нашей сферы взрыва с объектом
							// GetActorForwardVector() - по сути не в том направлении, в связи с чем сфера взрыва направлена внутрь 
							// Root->AddImpulseAtLocation(GetActorForwardVector() * Damage * 100, HitResult.Location);
							// в связи с этим
							// Vector - теперь из центра взрыва к точке пересечения
							auto Vector = HitLocation - HitResult.Location;
							Vector.Normalize();
							Root->AddImpulseAtLocation(Vector * Damage * 100, HitResult.Location);
						}
					}
				}
			}
		}
	}
	Destroy(); // Снаряд тоже уничтожается
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

