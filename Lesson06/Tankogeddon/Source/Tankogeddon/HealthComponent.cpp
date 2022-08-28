// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false; // Tick ��� ����� �� �����

	// � ������ ���� � ��� ������� �������� �������� ����� ������������� (� ����������� �� ������ ������)
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
		// ������� ����������� ����, ��� �� ���� ����������
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

	// � ������ ���� � ��� ������� �������� �������� ����� �������������
	HealthCur = HealthMax;
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

