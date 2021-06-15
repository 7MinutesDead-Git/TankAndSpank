

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankGameModeBase.generated.h"

/**
 *
 */

// -------------------------------------------------------------------------------------------
// Forward declarations.
class APawnTurret;
class APawnTank;


// -------------------------------------------------------------------------------------------
UCLASS()
class TOONTANKS_API ATankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ActorDied(AActor* DeadActor);

private:
	UPROPERTY()
	APawnTank* PlayerTank;
	UPROPERTY()
	TArray<AActor*> Turrets;
	void HandleGameStart();
	void HandleGameOver(bool PlayerWon);
	void UpdateTurretsAlive();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Game Loop")
	int32 StartDelay = 3;

	// The following UFUNCTIONs mean we'll be able to implement these functions in Blueprints.
	// Certain things like setting timers and creating widgets are much faster and easier in BP.

	/// Imeplementable event/method in Blueprints!
	UFUNCTION(BlueprintImplementableEvent)
	void GameStart();

	/// Imeplementable event/method in Blueprints!
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool PlayerWon);
};
