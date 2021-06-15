// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "ToonTanks/GameModes/TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// -------------------------------------------------------------------------------------------
/// Sets default values for this component's properties.
UHealthComponent::UHealthComponent()
{
	// No tick needed since this is a completely reactive class (only called by others).
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// -------------------------------------------------------------------------------------------
/// Called when the game starts.
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = DefaultHealth;
	// A reference to our world's current active game mode, cast as GameModeBase class.
	GameModeRef = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	// Bind "OnTakeAnyDamage" event to our TakeDamage function.
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	// ...

}

// -------------------------------------------------------------------------------------------
/// Handle taking damage. Receive healing if negative damage is taken.
void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* Instigator, AActor* DamageCauser)
{
	// If 0 damage was taken, or if target is already dead, we don't need to do anything below.
	if(Damage == 0 || Health <= 0) {
		return;
	}

	// Clamp here ensures we can't have negative health, or more health than DefaultHealth.
	Health = FMath::Clamp(Health - Damage, 0.f, DefaultHealth);
	UE_LOG(LogTemp, Warning, TEXT("%s Health: %f"), *GetOwner()->GetName(), Health);

	// Death condition.
	if (Health <= 0) {
		if (!GameModeRef) {
			UE_LOG(LogTemp, Error, TEXT("HealthComponent has no reference to GameMode!"))
			UE_LOG(LogTemp, Error, TEXT("Unable to report death of actor %s to GameMode."), *GetOwner()->GetName());
			return;
		}
		// Tell our GameMode this actor died.
		GameModeRef->ActorDied(GetOwner());
	}
	// ...
}
