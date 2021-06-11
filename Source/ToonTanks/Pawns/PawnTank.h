
#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"

#include "PawnTank.generated.h"

// -----------------------------------------------------
// Forward declarations to speed up compile time (do the includes in the cpp file instead).
class USpringArmComponent;
class UCameraComponent;

// -----------------------------------------------------
/// This is the player tank class!
UCLASS()
class TOONTANKS_API APawnTank : public APawnBase
{
	GENERATED_BODY()

public:
	// -----------------------------------------------------
	/// Sets default values for this pawn's properties.
	APawnTank();
	/// Called every frame.
	virtual void Tick(float DeltaTime) override;
	/// Called to bind functionality to input.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// -----------------------------------------------------
	/// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

private:
	// -----------------------------------------------------
	void MoveTank(float Input);
	void RotateTank(float Input);
	FVector MoveDirection;
	FQuat RotationDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess = "true"))
	float MoveSpeed = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess = "true"))
	float TurnSpeed = 300;

	/// The Spring Arm Component for our Camera.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	/// The player Camera attached to the tank.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

};
