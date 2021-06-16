
#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"

#include "PawnTank.generated.h"

// -------------------------------------------------------------------------------------------
// Forward declarations to speed up compile time (do the includes in the cpp file instead).
class USpringArmComponent;
class UCameraComponent;

// -------------------------------------------------------------------------------------------
/// This is the player tank class!
UCLASS()
class TOONTANKS_API APawnTank : public APawnBase
{
	GENERATED_BODY()

public:
	// ---------------------------------------------------------
	/// Sets default values for this pawn's properties.
	APawnTank();
	/// Called every frame.
	virtual void Tick(float DeltaTime) override;
	/// Called to bind functionality to input.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void HandleDestruction() override;

	/* Making this function public allows any class to check if the player is alive,
	 * without needing to expose the actual "IsPlayerAlive" variable, to avoid
	 * accidentally setting it externally. So, "get" is public, but "set" is private. */
	bool IsPlayerAlive();

protected:
	// ---------------------------------------------------------
	/// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

private:
	// ---------------------------------------------------------
	void MoveTank(float Input);
	void RotateTank(float Input);
	void LookAtMouse();
	/// Alternative to aiming at cursor.
	void RotateView(float Input);
	void CreateFireRateTimer();
	void CheckFireCondition();
	void FireToggle();

	bool IsFiring = false;
	bool PlayerAlive = true;

	FVector MoveDirection;
	FQuat RotationDirection;
	FTimerHandle FireRateTimerHandle;

	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess = "true"))
	float MoveSpeed = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess = "true"))
	float TurnSpeed = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess = "true"))
	float MouseSensitivity = 150;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta=(AllowPrivateAccess = "true"))
	/// Lower is faster.
	float FireRate = 0.5;


	/// The Spring Arm Component for our Camera.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	/// The player Camera attached to the tank.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

};
