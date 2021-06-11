// Subclass of PawnBase.
// This is the player tank!


#include "PawnTank.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// -----------------------------------------------------
APawnTank::APawnTank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// -----------------------------------------------------
void APawnTank::BeginPlay()
{
	Super::BeginPlay();
}

// -----------------------------------------------------
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// -----------------------------------------------------
// Technically this method has it's own tick component,
// it just sends a 0 value when not pressing anything.
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForwardAndBack", this, &APawnTank::MoveTank);
	PlayerInputComponent->BindAxis("TurnRightAndLeft", this, &APawnTank::RotateTank);
}

// -----------------------------------------------------
/// Determine move speed and direction on x-axis (forward or back). Update MoveDirection.
void APawnTank::MoveTank(float Input)
{
	// Since we're driving a tank, we won't be strafing, so x-axis only.
	float X = Input * MoveSpeed * GetWorld()->DeltaTimeSeconds;
	float Y = 0;
	float Z = 0;

	MoveDirection = FVector(X, Y, Z);
	AddActorLocalOffset(MoveDirection, true);
}

// -----------------------------------------------------
/// Determine rotation speed and direction on y-axis (spinning/yaw). Update RotationDirection.
void APawnTank::RotateTank(float Input)
{
	float Pitch = 0;
	float Yaw = Input * TurnSpeed * GetWorld()->DeltaTimeSeconds;
	float Roll = 0;

	FRotator Rotation = FRotator(Pitch, Yaw, Roll);
	RotationDirection = FQuat(Rotation);
	AddActorLocalRotation(RotationDirection, true);
}
