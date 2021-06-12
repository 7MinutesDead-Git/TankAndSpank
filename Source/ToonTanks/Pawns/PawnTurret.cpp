/*
Objects are the highest level class, but they don't have a representation in the World.
The "World" is the top level Object, that can represent a map or a sandbox, in which the Actors and Components exist in a game.
A World could be a single persistent Level, or made up of many sub-Levels (eg via Level-streaming features).

Actors are the highest level class in the World, and are derived from Objects.

Since Objects do not have physical representations in the World, we can't spawn Actors or return references from them..
	UNLESS they're provided a "Context Actor" that *does* exist in the Game/World.
*/

#include "PawnTurret.h"

#include "DrawDebugHelpers.h"
#include "PawnTank.h"
#include "Kismet/GameplayStatics.h"
#define OUT

// -------------------------------------------------------------------------------------------
APawnTurret::APawnTurret()
{
	//...
}

// -------------------------------------------------------------------------------------------
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

	// This will start the FireRateTimerHandle, which fires off every "FireRate" seconds.
	CreateFireRateTimer();
	PlayerPawn = GetPlayerPawnTank();
}

// -------------------------------------------------------------------------------------------
/// Destroy the Turret when it is killed by the player. Inform GameMode of Turret death.
void APawnTurret::HandleDestruction()
{
	// Call base pawn first to play effects,
	// then we can do the rest of override logic specific to the turret.
	Super::HandleDestruction();

	Destroy();
}

// -------------------------------------------------------------------------------------------
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If there is no player, or the player is out of range, do nothing.
	if (!PlayerPawn || DistanceToPlayer() > ThreatRange) {
		return;
	}

	// Otherwise, move our turret head's aim/rotation towards the Player.
	RotateTurret(PlayerPawn->GetActorLocation());


}

// -------------------------------------------------------------------------------------------
void APawnTurret::CheckFireCondition()
{
	if (EnableDebug) {
		// A debug sphere to show detection range.
		DrawDebugSphere(GetWorld(), GetActorLocation(), ThreatRange, 8, Blue, false, 2, 0, 1);
	}

	// If the player doesn't exist, return.
	if (!PlayerPawn) {
		return;
	}

	// If they're in range, fire!
	if (DistanceToPlayer() <= ThreatRange) {
		Fire();
	}

}

// -------------------------------------------------------------------------------------------
/// Create a timer that will dictate the turret's fire rate.\n\n <b>OUT</b> to <i>FireRateTimerHandle</i>.
void APawnTurret::CreateFireRateTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	TimerManager.SetTimer(
		OUT FireRateTimerHandle,		  // OUT to our Timer Handle (the thing that will be handling this timer).
		this,							  // Reference to this class.
		&APawnTurret::CheckFireCondition, // The memory location of the function we'll be firing off.
		FireRate,						  // The amount of time (in seconds) between set and firing.
		true							  // Keep looping/firing at our set FireRate intervals.
		);
}

// -------------------------------------------------------------------------------------------
/// Get the distance from this Turret to the Player.
float APawnTurret::DistanceToPlayer()
{
	// Technically GetDistance should never be called if the player doesn't exist,
	// but this is extra backup anyway.
	if (!PlayerPawn) {
		return 0;
	}

	// All of this could be done on one line, but the verbosity helps me remember later.
	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	FVector TurretLocation = GetActorLocation();

	float Distance = FVector::Dist(PlayerLocation, TurretLocation);
	return Distance;
}

// -------------------------------------------------------------------------------------------
/// Retrieve player pawn at 0 index (Player One). Return as PawnTank reference.
APawnTank* APawnTurret::GetPlayerPawnTank()
{
	// This retrieves our player pawn at 0 index (player 1), and stores it as a APawnTank class (converted with a Cast).
	// See above notes why we have to pass in "this".
	APawnTank* PawnRef = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
	return PawnRef;
}
