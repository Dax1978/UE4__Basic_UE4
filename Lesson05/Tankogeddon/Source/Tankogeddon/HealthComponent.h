// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageTaker.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_EVENT(UHealthComponent, FOnDeath);
	DECLARE_EVENT_OneParam(UHealthComponent, FOnDamaged, FDamageData);

	// DECLARE_EVENT - Базовый. Используется для простого информирования о различных событиях
	// DECLARE_DELEGATE - простой эвент, у которого м.б. только один подписчик
	// DECLARE_EVENT_OneParam - создает событие с одним параметром
	// DECLARE_EVENT_TwoParams( OwningType, EventName, Param1Type, Param2Type ) - с двумя параметрами
	// DECLARE_EVENT_<Num>Params( OwningType, EventName, Param1Type, Param2Type, ... ) - с N параметрами
	// EVENT - м.б. вызван только из класса, а DELEGATE абсолютно ото всюду
	// DECLARE_MULTICAST_DELEGATE - можно много подписчиков из разных мест
	// DECLARE_DYNAMIC_DELEGATE - на него можно подписываться из Bluprint + подписка с помощью UFUNCTION - здесь один подписчик
	// DECLARE_DYNAMIC_MULTICAST_DELEGATE - на него можно подписываться из Bluprint + подписка с помощью UFUNCTION - здесь много подписчиков

public:	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Health)
	float HealthMax = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Health)
	float HealthCur = 0;

	// Sets default values for this component's properties
	UHealthComponent();

	void TakeDamage(FDamageData Damage);

	FOnDeath OnDeath;
	FOnDamaged OnDamaged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
