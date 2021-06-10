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
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
