// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

#include "Projects.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	// Since this isn't a *thing* to be part of the scene, it doesn't need to attach to anything.
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = MoveSpeedStart;
	ProjectileMovement->MaxSpeed = MoveSpeedMax;
	InitialLifeSpan = LifeSpan;

	// Bind "OnComponentHit" to "OnHit()" function, so OnHit is called any time this component is hit.
	// OnComponentHit is an event, so when it fires off, it'll call OnHit.
	// TODO: If this doesn't work, either recreate the Blueprint or move this to BeginPlay().
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

}

/// When this actor hits another actor. Handles damage, type, etc.
void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();

	if (!MyOwner) {
		return;
	}

	// If we hit a valid actor that isn't itself nor the owning pawn:
	if (OtherActor && OtherActor != this && OtherActor != MyOwner) {
		AController* InstigatorController = MyOwner->GetInstigatorController();

		// Generate and apply the damage.
		UGameplayStatics::ApplyDamage(
			OtherActor,				// Actor that will be damaged.
			Damage,					// Damage amount.
			InstigatorController,	// Which player instigated it.
			this,					// What actor caused the damage.
			DamageType				// Type of damage done.
			);

		// // Potential Alternative?
		// FDamageEvent DamageEvent;
		// OtherActor->TakeDamage(
		// 	Damage,					// Damage amount.
		// 	OUT DamageEvent,		// Data package to describe the damage.
		// 	InstigatorController,	// Which player instigated it.
		// 	this					// What actor caused the damage.
		// 	);

		// // Debug notifications.
		// UE_LOG(LogTemp, Warning,
		// 	TEXT("%s hit %s for %f damage!"),
		// 	*InstigatorController->GetName(),
		// 	*OtherActor->GetName(),
		// 	Damage
		// 	);
	}

	// Then destroy the projectile after LifeSpan seconds, via a Timer (aka Delay :D).
	FTimerManager& DestroyTimer = GetWorld()->GetTimerManager();
	DestroyTimer.SetTimer(
		OUT ExplosionTimerHandle,
		this,
		&AProjectileBase::DestroyProjectile,
		LifeSpan,
		false
		);

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

}

void AProjectileBase::DestroyProjectile()
{
	Destroy();
}
