
#include "TankGameModeBase.h"


void ATankGameModeBase::BeginPlay()
{
	// Get references, and win/lose conditions.
	// Call HandleGameStart() to initialize start countdown, turret activation, pawn check, etc.
}

/// What to do with dead actors (player or NPC).
void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
	// Check what type of Actor died.
	// - If turret, up difficulty, spawn items, etc.
	// - If player, go to lose condition.
}

/// Call the GameStart() Blueprint function.
void ATankGameModeBase::HandleGameStart()
{
	// Initialize start countdown, turret activation, pawn check, etc.
	GameStart();
}

/// Call the GameOver() Blueprint function.
void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
	// If player destroys all turrets, do win result.
	// If turret destroys player, do lose result.
	GameOver(PlayerWon);
}
