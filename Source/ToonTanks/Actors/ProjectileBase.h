// TSubClassOf: A way to store a type in a variable, instead of a value.
// We just specify the base type it can hold, in this case UDamageType.
// This is a type-safe way to provide access to specific class pointers in the editor.
// So in this case, we'll get access to everything related to UDamageType, labeled as "DamageType".

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ProjectileBase.generated.h"


// -------------------------------------------------------------------------------------------
class UProjectileMovementComponent;

// -------------------------------------------------------------------------------------------
UCLASS()
class TOONTANKS_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileBase();

private:
	/* Dynamic Delegate is an event you can set to be called and responded to,
	 *   so that every time this event is fired, anything that's listening to
	 *   this event will receive it, and then be able to take action or call
	 *   their own function based on that event.
	 *
	 * Dynamic Delegates need to be created as UFUNCTIONS.
	 *
	 * UFUNCTIONS are the same as C++ functions, with a few differences.
	 *	- Can be called or overridden from within Blueprints Visual Scripting.
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
	/// Dynamic Delegate. Used to handle our On Hit info for damage, destruction, etc.
	UFUNCTION() void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	// See notes at top.
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = "true"))
	float MoveSpeedStart = 500;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = "true"))
	float MoveSpeedMax = 3000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Life", meta=(AllowPrivateAccess = "true"))
	float LifeSpan = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage", meta=(AllowPrivateAccess = "true"))
	float Damage = 50;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
