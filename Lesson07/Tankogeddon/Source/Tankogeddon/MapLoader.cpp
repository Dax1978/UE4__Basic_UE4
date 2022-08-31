// Fill out your copyright notice in the Description page of Project Settings.


#include "MapLoader.h"

// Sets default values
AMapLoader::AMapLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// ��� ��������, �.�. �� ����� �� �����
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;
	// ���������� �� ������
	// ����� ������ ��� �� � ������������ � � BeginPlay - ����� ���� ��������
	// 1. �������� ����� �� ����������� -> ������� �������� �� �����
	// 2. �������� �����������, �� ���� �������� �� �������, �� �� ������ ��� �������
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AMapLoader::OnTriggerBeginOverlap);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("BodyMesh");
	BodyMesh->SetupAttachment(RootComponent);

	ActiveLight = CreateDefaultSubobject<UPointLightComponent>("ActiveLight");
	ActiveLight->SetupAttachment(RootComponent);

	InactiveLight = CreateDefaultSubobject<UPointLightComponent>("InactiveLight");
	InactiveLight->SetupAttachment(RootComponent);	
}

void AMapLoader::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetActive(IsActive);
}

// Called when the game starts or when spawned
void AMapLoader::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMapLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapLoader::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (this == Other)
	{
		return;
	}

	// ����� ����� ������� �� ��������� �������
	if (IsActive && !LevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	}	
}

// ����� ���������� �� �������, � ������ ����� ������� ����� ����������
void AMapLoader::SetActive(bool Active)
{
	IsActive = Active;

	ActiveLight->SetVisibility(Active);
	InactiveLight->SetVisibility(!Active);
}