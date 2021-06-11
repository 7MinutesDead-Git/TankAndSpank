// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Pawn.h"
#include "PawnBase.generated.h"

/// This is the base class for our pawns (both the tank and the immobile turrets).
UCLASS()
class TOONTANKS_API APawnBase : public APawn
{
	GENERATED_BODY()

public:
	/// Sets default values for this pawn's properties.
	APawnBase();

private:
	// "AllowPrivateAccess" is needed to access private variables from Blueprints.
	// ...

	/// Simple collision shape for our pawn.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UCapsuleComponent* Capsule;

	/// The base of the tank, or anything else that will have a rotating turret on top.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	/// The rotating turret on top.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;

	/// Acts as visual representation for where to spawn the projectile from.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;
	// ...


protected:
	void RotateTurret(FVector LookAtTarget);
	void Fire();
	// To be overridden in any child classes.
	virtual void HandleDestruction();

};
