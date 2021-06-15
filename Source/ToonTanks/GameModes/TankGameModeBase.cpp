/*
 * StaticClass() is a static function (which means it does not require object instance).
 * It will return a UClass of the class you are calling from. Part of UE4's reflection system.
 *
 * This was created because C++ does not let you use class identifiers as variable content.
 * The reason for that is because for the CPU, classes and structures don't exist; The compiler
 * simply creates native code to read memory with specific offsets to read specific variables in
 * the structure of a class object.
 *
 * This is why the UE4 reflection system is needed.
 * UE4 does this by generating objects that identify classes and properties within it.
*/

#include "TankGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "Kismet/GameplayStatics.h"


// -------------------------------------------------------------------------------------------
void ATankGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UpdateTurretsAlive();
	PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
	HandleGameStart();
}

// -------------------------------------------------------------------------------------------
/// What to do with dead actors (player or NPC).
void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Actor %s died! Bye-bye."), *DeadActor->GetName());

	// If the player died then we kill it and game over man.
	if (DeadActor == PlayerTank) {
		PlayerTank->HandleDestruction();
		HandleGameOver(false);
	}
	// If a turret died, then we go here.
	else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor)) {
		DestroyedTurret->HandleDestruction();

		UpdateTurretsAlive();
		if (Turrets.Num() == 0) {
			HandleGameOver(true);
		}
	}
}

// -------------------------------------------------------------------------------------------
/// Call the GameStart() Blueprint function.
void ATankGameModeBase::HandleGameStart()
{
	GameStart();
}

// -------------------------------------------------------------------------------------------
/// Call the GameOver() Blueprint function.
void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
	GameOver(PlayerWon);
}

// -------------------------------------------------------------------------------------------
/// Get array of current turrets alive. \n Update Turrets array. \n Update TurretsAliveAmt.
void ATankGameModeBase::UpdateTurretsAlive()
{
	// We'll get a list of all our turrets.
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),						// World context.
		APawnTurret::StaticClass(),		// Class of actor we're looking for. See notes at top.
		OUT Turrets						// Where to store the result.
		);
}
