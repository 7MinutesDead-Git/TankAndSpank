// -------------------------------------------------------------------------------------------
/* TSubClassOf is a way to store a type in a variable, instead of a value.
 * We just specify the base type it can hold, in this case UDamageType.
 * This is a type-safe way to provide access to specific class pointers in the editor.
 * So in this case, we'll get access to everything related to UDamageType, labeled as "DamageType".
*/
// -------------------------------------------------------------------------------------------
/* Dynamic Delegates are events you can set to be called and responded to,
 *   so that every time this event is fired, anything that's listening to
 *   this event will receive it, and then be able to take action or call
 *   their own function based on that event.
 *
 *   Example:    ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
 *
 *   Makes "OnHit" a Delegate, that is called when an OnComponentHit Event is fired off.
 *   To do this, our OnHit() has to be declared a UFUNCTION first, and
 *   given enough parameters to make use of the info from the OnComponentHit Event.
 *
 *
 *  All Dynamic Delegates need to be created as UFUNCTION.
 *
 * UFUNCTION is the same as C++ functions, with a few differences.
 *	- Can be called or overridden from within Blueprints Visual Scripting,
 *		/w parameters: BlueprintCallable, BlueprintImplementableEvent, BlueprintPure.
 *
 *	- Can be assigned as delegates within the default properties of a class.
 *		This is common for input action or axes binds to functions in Input classes.
 *
 *	- Replication callbacks: UFunction will be called when the variable changes that
 *		it's associated with, and is replicated across the network.
 *
 *	- The only type of function that can be declared as an exec function, allowing them
 *		to be called by the player from the in-game console.
 *
*/
// -------------------------------------------------------------------------------------------

// /**
// * Delegate for notification of blocking collision against a specific component.
// * NormalImpulse will be filled in for physics-simulating bodies, but will be zero for swept-component blocking collisions.
// * Uncomment to see syntax highlighting.
// */
// DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_FiveParams(
// 	FComponentHitSignature,					// (Don't need) The type this event fires off from behind the scenes (I think).
// 	UPrimitiveComponent,					// (Don't need) The class this event is attached to behind the scenes (I think), which is why we can access it from a StaticMesh component.
// 	OnComponentHit,							// (Don't need) The event name! This is how we know what it is.
// 	UPrimitiveComponent*, HitComponent,		// First parameter we'll actually get from the event. Our component that was hit (our ProjectileMesh in this case).
// 	AActor*, OtherActor,					// Second parameter. The other actor hit by our ProjectileMesh.
// 	UPrimitiveComponent*, OtherComp,		// Third. The other specific component hit on that actor, if needed (like if we could specifically damage an arm, rather than a player).
// 	FVector, NormalImpulse,					// Fourth. The impulse of the hit.
// 	const FHitResult&, Hit					// Fifth. The info in the hit result of a trace, like point of impact, etc.
// 	);
// -------------------------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ProjectileBase.generated.h"


// -------------------------------------------------------------------------------------------
// Forward declarations.
class UProjectileMovementComponent;
class ATriggerSphere;

// -------------------------------------------------------------------------------------------
UCLASS()
class TOONTANKS_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileBase();

private:
	// See notes above about UFUNCTIONS and Delegates for working with Events.
	/// Will be a Dynamic Delegate. Used to handle our OnComponentHit info for damage, destruction, etc. \n
	/// Parameters determined by parameters accessible from OnComponentHit event.
	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
		);

	void PlaySoundNoSpam(USoundBase* SoundToPlay);

	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, Category="Effects")
	UParticleSystem* ExplosionParticle;
	UPROPERTY(EditAnywhere, Category="Effects")
	UParticleSystem* HitParticle;
	UPROPERTY(EditAnywhere, Category="Effects")
	USoundBase* ImpactSound;
	UPROPERTY(EditAnywhere, Category="Effects")
	USoundBase* DirectImpactSound;
	UPROPERTY(EditAnywhere, Category="Effects")
	USoundBase* LaunchSound;
	UPROPERTY(EditAnywhere, Category="Effects")
	USoundBase* ExplosionSound;
	float WorldTime;
	float TimeHitSoundPlayed = 0;

	// UMatineeCameraShake is a legacy Camera Shake. The new one is CameraShakeBase.
	UPROPERTY(EditAnywhere, Category="Effects")
	TSubclassOf<UMatineeCameraShake> HitShake;
	UPROPERTY(EditAnywhere, Category="Effects")
	float HitShakeScale = 1;


	// -----------------------------------------------------------------------
	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category="Components",
		meta=(AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category="Components",
		meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	// -----------------------------------------------------------------------
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category="Movement",
		meta=(AllowPrivateAccess = "true"))
	float MoveSpeedStart = 500;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category="Movement",
		meta=(AllowPrivateAccess = "true"))
	float MoveSpeedMax = 3000;

	// -----------------------------------------------------------------------
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category="Life",
		meta=(AllowPrivateAccess = "true"))
	float LifeSpan = 5;
	float ExplosionTimer = LifeSpan / 2;

	// -----------------------------------------------------------------------
	// See notes up top about TSubclassOf.
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category="Damage",
		meta=(AllowPrivateAccess = "true"))
	float Damage = 50;

	// -----------------------------------------------------------------------
	UPROPERTY()
	FTimerHandle ExplosionTimerHandle;
	bool IsTurret = false;
	bool IsTank = false;
	UPROPERTY(EditAnywhere)
	bool EnableDebugView;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void DestroyProjectile();
	void CreateExplosionImpulse(FVector Location);

	// Radial Impulse.
	UPROPERTY(EditAnywhere)
	float ImpulseRadius = 500;
	UPROPERTY(EditAnywhere)
	float ImpulseForce = 2000;
	UPROPERTY()
	TArray<FHitResult> HitResults;

};
