// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Tankogeddon.h"
#include "TankPlayerController.h"

#include "Kismet/KismetMathLibrary.h"
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
}

void ATankPawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ATankPawn::Shoot(int ShootType)
{
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

	TankController = Cast<ATankPlayerController>(NewController);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (CannonClass)
	{
		// auto Transformation = CannonAttachment->GetComponentTransform();
		Cannon = GetWorld()->SpawnActor<ACannon1>(CannonClass);
		Cannon->AttachToComponent(CannonAttachment, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		auto MousePosition = TankController->GetWorldMousePosition();
		auto MouseRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MousePosition);
		auto CurrentRotationTurret = TurretMesh->GetComponentRotation();
		MouseRotation.Pitch = CurrentRotationTurret.Pitch;
		MouseRotation.Roll = CurrentRotationTurret.Roll;
		CurrentRotationTurret = FMath::Lerp(CurrentRotationTurret, MouseRotation, TurretRotationSpeed);
		TurretMesh->SetWorldRotation(CurrentRotationTurret);

		DrawDebugLine(GetWorld(), CannonAttachment->GetComponentLocation(), CannonAttachment->GetComponentLocation() + CannonAttachment->GetForwardVector() * 1000, FColor::Green, false, -1, 0, 5);
	}
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

