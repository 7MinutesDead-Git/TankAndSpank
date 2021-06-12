// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToonTanks/GameModes/TankGameModeBase.h"

#include "HealthComponent.generated.h"

// Forward declarations.
class ATankGameModeBase;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float DefaultHealth = 200;
	float Health = 0;

	ATankGameModeBase* GameModeRef;

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser);
};
