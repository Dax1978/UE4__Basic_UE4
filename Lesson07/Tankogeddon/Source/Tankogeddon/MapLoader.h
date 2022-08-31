// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"

#include "MapLoader.generated.h"

UCLASS()
class TANKOGEDDON_API AMapLoader : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UBoxComponent* Collision;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UPointLightComponent* ActiveLight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UPointLightComponent* InactiveLight;

	// ��� ������� ����� ��� ������, �� ������� �� ����� ����������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	FName LevelName;

	// ���� ����� ����������, ����� ���������� �������� ������ �������
	virtual void OnConstruction(const FTransform& Transform) override;

	// Sets default values for this actor's properties
	AMapLoader();

	void SetActive(bool Active);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// � ���������, ��� �� ��� ������ ���� ��������, ����� ��� �� ������ �������, �.�. � ��� ���� SetActive(bool Active)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Portal)
	bool IsActive = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	
};
