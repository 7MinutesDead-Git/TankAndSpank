// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// No tick needed since this is a completely reactive class (only called by others).
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...

}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* Instigator, AActor* DamageCauser)
{
	// ...
}
