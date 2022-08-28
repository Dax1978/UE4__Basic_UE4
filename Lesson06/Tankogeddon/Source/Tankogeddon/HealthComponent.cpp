// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false; // Tick нам здесь не нужен

	// В начале игры у нас текущее значение здоровья равно максимальному (в конструктор на всякий случай)
	HealthCur = HealthMax;
}


void UHealthComponent::TakeDamage(FDamageData Damage)
{
	auto HealthPre = HealthCur;
	HealthCur -= Damage.DamageValue;
	if (HealthCur < 0) 
	{
		HealthCur = 0;
	}
	if (HealthCur != HealthPre)
	{
		// Событие рассылается всем, кто на него подписался
		OnDamaged.Broadcast(Damage);
		if (HealthCur <= 0)
		{
			OnDeath.Broadcast();
		}
	}
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// В начале игры у нас текущее значение здоровья равно максимальному
	HealthCur = HealthMax;
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

