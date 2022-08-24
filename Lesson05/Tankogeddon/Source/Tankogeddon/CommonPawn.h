// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DamageTaker.h"
#include "Components/BoxComponent.h"		// Äëÿ UBoxComponent
#include "Components/ArrowComponent.h"		// Äëÿ UArrowComponent
#include "Cannon1.h"						// Äëÿ <ACannon1> CannonClass
#include "HealthComponent.h"
#include "DamageTaker.h"

/**
 * 
 */

class TANKOGEDDON_API CommonPawn : public IDamageTaker
{
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UBoxComponent* Collision;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UStaticMeshComponent* BodyMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	UArrowComponent* CannonAttachment;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MovementParams)
	//TSubclassOf<ACannon1> CannonClass;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
	//UHealthComponent* HealthComponent;

	CommonPawn();

	// virtual void TakeDamage(FDamageData Damage) override;
	// void OnDamaged(FDamageData Damage);
	// void OnDeath(FDamageData Damage);

	~CommonPawn();
};
