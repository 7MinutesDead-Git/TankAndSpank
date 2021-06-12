// Subclass of PawnBase.
// This is the player tank!


#include "PawnTank.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// -------------------------------------------------------------------------------------------
APawnTank::APawnTank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	SpringArm->SetupAttachment(TurretMesh);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// -------------------------------------------------------------------------------------------
void APawnTank::BeginPlay()
{
	Super::BeginPlay();
	// Get the controller of the tank pawn, and cast it to PlayerController type,
	// so we can reference it later.
	PlayerController = Cast<APlayerController>(GetController());
}

// -------------------------------------------------------------------------------------------
void APawnTank::HandleDestruction()
{
	Super::HandleDestruction();
	// Hide Player. TODO: Create function for this.
}

// -------------------------------------------------------------------------------------------
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerController) {
		// LookAtMouse();
	}

}


// -------------------------------------------------------------------------------------------
// Technically this method has it's own tick component, it just sends a 0 value when not pressing anything.
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForwardAndBack", this, &APawnTank::MoveTank);
	PlayerInputComponent->BindAxis("TurnRightAndLeft", this, &APawnTank::RotateTank);
	PlayerInputComponent->BindAxis("RotateTurret", this, &APawnTank::RotateView);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);
}

// -------------------------------------------------------------------------------------------
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

// -------------------------------------------------------------------------------------------
/// Determine rotation speed and direction on y-axis (spinning/yaw). Update RotationDirection.
void APawnTank::RotateTank(float Input)
{
	float Pitch = 0;
	float Yaw = Input * TurnSpeed * GetWorld()->DeltaTimeSeconds;
	float Roll = 0;

	FRotator Rotation = FRotator(Pitch, Yaw, Roll);
	FRotator CounterRotation = FRotator(Pitch, -Yaw, Roll);

	RotationDirection = FQuat(Rotation);
	AddActorLocalRotation(RotationDirection, true);

	// Counter rotate the turret so the view stays the same.
	TurretMesh->AddLocalRotation(CounterRotation, false);
}

// -------------------------------------------------------------------------------------------
/// Aim the tank turret towards the mouse cursor location via RotateTurret().
void APawnTank::LookAtMouse()
{
	if (PlayerController) {
		FHitResult CursorHit;
		// This traces from the cursor to the world space. (cool!)
		PlayerController->GetHitResultUnderCursor(
			ECC_Visibility,		// The trace channel (in this case, anything visible).
			false,				// Use complex tracing? No need for subpart hit detection.. yet.
			OUT CursorHit		// OUT to FHitResult.
			);
		// This is the end location we want our tank turret to look at.
		FVector HitLocation = CursorHit.Location;
		RotateTurret(HitLocation);

	}
}

// -------------------------------------------------------------------------------------------
/// Rotate turret with mouse (alternative to aiming at cursor).
void APawnTank::RotateView(float Input)
{
	FRotator Rotation = TurretMesh->GetRelativeRotation();
	Rotation.Yaw += Input * MouseSensitivity * GetWorld()->DeltaTimeSeconds;
	TurretMesh->SetRelativeRotation(Rotation);
}
