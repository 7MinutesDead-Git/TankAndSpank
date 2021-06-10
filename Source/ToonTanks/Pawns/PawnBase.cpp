// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"

APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// This is creating a capsule component, naming it, and attaching it to the pawn.
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	// We can set the Capsule as the root component with inherited "RootComponent",
	// although whichever component is created first is set to root as default.
	RootComponent = Capsule;

	// Create the base, name it, and attach it to the root.
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	// Create the turret, and attach it to the base (since we want our turret to follow the base).
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	// Create the visual representation of the projectile's spawn point, and attach to turret.
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}
