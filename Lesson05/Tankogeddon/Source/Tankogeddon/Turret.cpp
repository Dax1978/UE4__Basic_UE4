// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("BodyMesh");
	BodyMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
	TurretMesh->SetupAttachment(BodyMesh);

	CannonAttachment = CreateDefaultSubobject<UArrowComponent>("CannonAttachment");
	CannonAttachment->SetupAttachment(TurretMesh);

	TargetRange = CreateDefaultSubobject<USphereComponent>("TargetRange");
	TargetRange->SetupAttachment(Collision);
	TargetRange->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnTargetRangeBeginOverlap);
	TargetRange->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnTargetRangeEndOverlap);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->OnDamaged.AddUObject(this, &ATurret::OnDamaged);
	HealthComponent->OnDeath.AddUObject(this, &ATurret::OnDeath);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	if (CannonClass)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = this;
		// auto Transformation = CannonAttachment->GetComponentTransform();
		Cannon = GetWorld()->SpawnActor<ACannon1>(CannonClass, SpawnParameters);
		Cannon->AttachToComponent(CannonAttachment, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Targetting();
	if (Cannon && CanFire())
	{
		Cannon->Shoot();
	}
}

void ATurret::TakeDamage(FDamageData Damage)
{
	HealthComponent->TakeDamage(Damage);
}

void ATurret::Targetting()
{
	if (!CurrentTarget.IsValid())
	{
		return;
	}
	auto Rotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), CurrentTarget->GetActorLocation());
	auto CurrentRotation = TurretMesh->GetComponentRotation();
	Rotation.Pitch = CurrentRotation.Pitch;
	Rotation.Roll = CurrentRotation.Roll;
	CurrentRotation = FMath::Lerp(CurrentRotation, Rotation, TurretRotationSpeed);
	TurretMesh->SetWorldRotation(CurrentRotation);
}

bool ATurret::CanFire()
{
	if (!CurrentTarget.IsValid())
	{
		return false;
	}

	auto Rotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), CurrentTarget->GetActorLocation());
	auto CurrentRotation = TurretMesh->GetComponentRotation();
	if (FMath::Abs(Rotation.Yaw - CurrentRotation.Yaw) <= Accuracy)
	{
		return true;
	}
	return false;
}

void ATurret::OnDamaged(FDamageData Damage)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Turret health %f"), HealthComponent->HealthCur));
}

void ATurret::OnDeath()
{
	Destroy();
}

void ATurret::OnTargetRangeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Что бы не стреляла в себя
	if (Other == this || Other == GetInstigator() || Other == nullptr)
	{
		return;
	}
	if (!Other->IsA<ATankPawn>())
	{
		return;
	}
	Targets.Add(Other);
	FindBestTarget();
}

void ATurret::OnTargetRangeEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* Other, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Other == this || Other == GetInstigator() || Other == nullptr)
	{
		return;
	}
	if (!Other->IsA<ATankPawn>())
	{
		return;
	}
	Targets.Remove(Other);
	if (Other == CurrentTarget)
	{
		FindBestTarget();
	}	
}

void ATurret::FindBestTarget()
{
	float MinDistance = 1000000000;
	auto Location = GetActorLocation();
	AActor* TargetF = nullptr;
	for (auto Target : Targets) {
		if (Target.IsValid())
		{
			auto Distance = FVector::DistXY(Location, Target->GetActorLocation());
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				TargetF = Target.Get();
			}
		}
	}

	CurrentTarget = TargetF;
}

void ATurret::Destroyed()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
}