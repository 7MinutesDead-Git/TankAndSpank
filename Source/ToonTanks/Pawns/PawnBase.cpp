// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"

#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Actors/ProjectileBase.h"

// -------------------------------------------------------------------------------------------
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// This is creating a capsule component, naming it, and attaching it to the pawn.
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	// We can set the Capsule as the root component with inherited "RootComponent",
	// although whichever component is created first is set to root as default.
	// See resulting hierarchy in editor for this pawn.
	RootComponent = Capsule;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

// -------------------------------------------------------------------------------------------
/// Update TurretMesh rotation to face towards the LootAtTarget, locked by Tank's Z axis. \n
/// (So the turret doesn't tilt up and down since we don't have decoupled turret bits for that)
void APawnBase::RotateTurret(FVector LookAtTarget)
{
	// To avoid the turret actually tilting up and down, we only want to get the X and Y of our LookAtTarget.
	// The Z we can grab from the TurretMesh location itself to keep it locked on that axis.
	float TurretZ = TurretMesh->GetComponentLocation().Z;
	// SafeLookTarget could either be the mouse cursor (for the player), or the tank (for the turrets).
	FVector SafeLookTarget = FVector(LookAtTarget.X, LookAtTarget.Y, TurretZ);

	// Then we want to subtract this from the turret's own location to get the look direction.
	FVector SafeLookDirection = SafeLookTarget - TurretMesh->GetComponentLocation();

	// Then finally we convert that SafeLookDirection vector into a rotation, to rotate the turret.
	FRotator TurretRotation = SafeLookDirection.Rotation();
	TurretMesh->SetWorldRotation(TurretRotation);
}

// -------------------------------------------------------------------------------------------
/// Spawn Projectile at Location, firing towards Rotation.
void APawnBase::Fire()
{
	// Ensures we don't run and crash if we forget to set the type of projectile in the editor.
	if (ProjectileClass) {
		FVector Location = ProjectileSpawnPoint->GetComponentLocation();
		FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
		// Spawn a new TempProjectile.
		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, Location, Rotation);
		// Setting the owner helps, for example, down the line to ensure we don't shoot ourselves.
		TempProjectile->SetOwner(this);
	}
}


// -------------------------------------------------------------------------------------------
/// Handle destruction logic of the pawn (with overrides for specific pawn types).\n
/// Visualize destruction, inform GameMode of pawn death, etc.
void APawnBase::HandleDestruction()
{
	/*
	    Base Class:
			- Visualize destruction (particles, sound, camera shake).

		Child Class overrides:
			- In PawnTurret:
				Inform GameMode it died.
				Destroy itself.
			- In PawnTank:
				Inform GameMode player died.
				Hide() all of its components.
				Stop movement.
	*/
}
