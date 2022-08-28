// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Tankogeddon.h"
// #include "TankPlayerController.h"
#include "TargetController.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("BodyMesh");
	BodyMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
	TurretMesh->SetupAttachment(BodyMesh);

	CannonAttachment = CreateDefaultSubobject<UArrowComponent>("CannonAttachment");
	CannonAttachment->SetupAttachment(TurretMesh);

	Arm = CreateDefaultSubobject<USpringArmComponent>("Arm");
	Arm->SetupAttachment(RootComponent);
	Arm->bInheritPitch = false;
	Arm->bInheritRoll = false;
	Arm->bInheritYaw = false;
	Arm->bDoCollisionTest = false;
	Arm->TargetArmLength = 800;


	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(Arm);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->OnDamaged.AddUObject(this, &ATankPawn::OnDamaged);
	HealthComponent->OnDeath.AddUObject(this, &ATankPawn::OnDeath);

	TargetRange = CreateDefaultSubobject<USphereComponent>("TargetRange");
	TargetRange->SetupAttachment(Collision);
	TargetRange->OnComponentBeginOverlap.AddDynamic(this, &ATankPawn::OnTargetRangeBeginOverlap);
	TargetRange->OnComponentEndOverlap.AddDynamic(this, &ATankPawn::OnTargetRangeEndOverlap);

	ParticlesEffect = CreateDefaultSubobject<UParticleSystemComponent>("ParticlesEffect");
	ParticlesEffect->SetupAttachment(RootComponent);
	AudioEffect = CreateDefaultSubobject<UAudioComponent>("AudioEffect");
	AudioEffect->SetupAttachment(RootComponent);
	ParticlesEffectHit = CreateDefaultSubobject<UParticleSystemComponent>("ParticlesEffectHit");
	ParticlesEffectHit->SetupAttachment(RootComponent);
	AudioEffectHit = CreateDefaultSubobject<UAudioComponent>("AudioEffectHit");
	AudioEffectHit->SetupAttachment(RootComponent);
}

void ATankPawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ATankPawn::Shoot(int ShootType)
{
	if (ATankPawn::AmmoCount <= 0)
	{
		GEngine->AddOnScreenDebugMessage(14, 2, FColor::Red, FString::Printf(TEXT("You don't have any ammo!")));
		return;
	}

	if (Cannon)
	{
		if (ShootType == 0)
		{
			Cannon->Shoot();
		}
		else
		{
			Cannon->Shoot(1);
		}
		// По идее запас снарядов должен убывать
		// Но надо разделить танки, т.к. на вражеском запас снарядов убывает слишком уж быстро :)
		//ATankPawn::AmmoCount--;
		GEngine->AddOnScreenDebugMessage(12, -1, FColor::Blue, FString::Printf(TEXT("Ammo %i count"), ATankPawn::AmmoCount));
	}
}

void ATankPawn::MoveForward(float Scale)
{
	MoveForwardScale = Scale;
}

void ATankPawn::MoveRight(float Scale)
{
	MoveRightScale = Scale;
}

void ATankPawn::RotateRight(float Scale)
{
	RotateRightScale = Scale;
}

void ATankPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//TankController = Cast<ATankPlayerController>(NewController);
	TankController = Cast<ITargetController>(NewController);	
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	// В начале ставим пушку по умолчанию
	SetupCannon(CannonClass);
}

void ATankPawn::SetupCannon(TSubclassOf<ACannon1> InCannonClass)
{
	// Если пушка была то старую уничтожаем
	if (Cannon)
	{
		Cannon->Destroy();
		Cannon = nullptr;
	}

	// Применяем указанную пушку
	if (InCannonClass)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = this;
		// auto Transformation = CannonAttachment->GetComponentTransform();
		Cannon = GetWorld()->SpawnActor<ACannon1>(InCannonClass, SpawnParameters);
		Cannon->AttachToComponent(CannonAttachment, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

void ATankPawn::AddAmmo(int count)
{
	ATankPawn::AmmoCount += count;
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isAlive) {
		CurrentMoveForwardScale = FMath::Lerp(CurrentMoveForwardScale, MoveForwardScale, Acceleration);
		// UE_LOG(LogTanks, Warning, TEXT("CurrentMoveForwardScale %f"), CurrentMoveForwardScale);
		auto Location = GetActorLocation();
		auto NewLocation = Location + GetActorForwardVector() * CurrentMoveForwardScale * Speed * DeltaTime + GetActorRightVector() * MoveRightScale * Speed * DeltaTime;
		SetActorLocation(NewLocation, true);

		CurrentRotateRightScale = FMath::Lerp(CurrentRotateRightScale, RotateRightScale, Acceleration);
		auto Rotation = GetActorRotation();
		Rotation.Yaw = Rotation.Yaw + SpeedRotation * CurrentRotateRightScale * DeltaTime;
		SetActorRotation(Rotation);

		if (TankController)
		{
			auto MousePosition = TankController->GetTargetLocation();
			auto MouseRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MousePosition);
			auto CurrentRotationTurret = TurretMesh->GetComponentRotation();
			MouseRotation.Pitch = CurrentRotationTurret.Pitch;
			MouseRotation.Roll = CurrentRotationTurret.Roll;
			CurrentRotationTurret = FMath::Lerp(CurrentRotationTurret, MouseRotation, TurretRotationSpeed);
			TurretMesh->SetWorldRotation(CurrentRotationTurret);

			DrawDebugLine(GetWorld(), CannonAttachment->GetComponentLocation(), CannonAttachment->GetComponentLocation() + CannonAttachment->GetForwardVector() * 1000, FColor::Green, false, -1, 0, 5);
		}
	}

	if (!isAlive)
	{
		CurrentAlive++;
	}

	if (CurrentAlive >= AliveTime)
	{
		Destroy();
	}
}

void ATankPawn::TakeDamage(FDamageData Damage)
{
	HealthComponent->TakeDamage(Damage);
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATankPawn::EnemyHit()
{
	// Активируем эффекты при попадании
	ParticlesEffectHit->ActivateSystem();
	AudioEffectHit->Play();
}

void ATankPawn::OnDamaged(FDamageData Damage)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("Tank health %f"), HealthComponent->HealthCur));
}

void ATankPawn::OnDeath()
{
	// Активируем эффекты перед уничтожением
	ParticlesEffect->ActivateSystem();
	AudioEffect->Play();
	isAlive = false;
	// Destroy();
}

void ATankPawn::Destroyed() {
	if (Cannon)
	{
		Cannon->Destroy();

		// настроим вид после смерти
		auto PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
			// FGenericPlatformMisc::RequestExit(false);
		}
	}
}

void ATankPawn::OnTargetRangeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

	OnTargetsChanged.Broadcast();
	// FindBestTarget();
}

void ATankPawn::OnTargetRangeEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* Other, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

	OnTargetsChanged.Broadcast();

	// if (Other == CurrentTarget)
	// {
	// 	// FindBestTarget();
	// }
}