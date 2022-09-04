// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBoxTrace.h"
#include "TankPawn.h"

// Sets default values
AAmmoBoxTrace::AAmmoBoxTrace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBoxTrace::OnProjectileBeginOverlap);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("BodyMesh");
	BodyMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AAmmoBoxTrace::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmmoBoxTrace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoBoxTrace::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Tank = Cast<ATankPawn>(Other);
	if (Tank) {
		Tank->SetupCannon(CannonClass);

		// При наезде на коробку с оружием добавляем патроны
		Tank->AddAmmo(7);

		// Это для уничтожения коробки после взятия пушки
		// Destroy();
	}
}