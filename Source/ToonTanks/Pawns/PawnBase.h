// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Pawn.h"
#include "PawnBase.generated.h"

// -------------------------------------------------------------------------------------------
// Forward declarations.
class APawnTank;
class AProjectileBase;
class UHealthComponent;

// -------------------------------------------------------------------------------------------
/// This is the base class for our pawns (both the tank and the immobile turrets).
UCLASS()
class TOONTANKS_API APawnBase : public APawn
{
	GENERATED_BODY()

public:
	// ---------------------------------------------------------
	/// Sets default values for this pawn's properties.
	APawnBase();
	// To be overridden in any child classes.
	virtual void HandleDestruction();

private:
	// ---------------------------------------------------------
	UPROPERTY(EditAnywhere, Category="Effects")
	UParticleSystem* DeathParticle;
	UPROPERTY(EditAnywhere, Category="Effects")
	USoundBase* ExplosionSound;

	// "AllowPrivateAccess" is needed to access private variables from Blueprints.
	/// Acts as visual representation for where to spawn the projectile from.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	/// Type of Projectile to be spawned. \n
	/// Using TSubclassOf means in the editor we'll only be able to select from our
	/// AProjectileBase classes to fill this variable, including the derived BPs we might make from it.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile Type", meta= (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;


protected:
	void ShakeCamera(TSubclassOf<UMatineeCameraShake> ShakeType);
	// UMatineeCameraShake is a legacy Camera Shake. The new one is CameraShakeBase.
	UPROPERTY(EditAnywhere, Category="Effects")
	TSubclassOf<UMatineeCameraShake> DeathShake;
	UPROPERTY(EditAnywhere, Category="Effects")
	TSubclassOf<UMatineeCameraShake> ShotShake;
	UPROPERTY(EditAnywhere, Category="Effects")
	float CameraShakeScale = 1;

	// ---------------------------------------------------------
	/// Simple collision shape for our pawn.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UCapsuleComponent* Capsule;

	/// The base of the tank, or anything else that will have a rotating turret on top.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	/// The rotating turret on top.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;

	// ---------------------------------------------------------
	void RotateTurret(FVector LookAtTarget);
	virtual void Fire();
};
