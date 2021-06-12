

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class TOONTANKS_API ATankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ActorDied(AActor* DeadActor);

private:
	void HandleGameStart();
	void HandleGameOver(bool PlayerWon);

protected:
	virtual void BeginPlay() override;
	// This means we'll be able to implement these functions in Blueprints.
	// Certain things like setting timers and creating widgets are much faster and easier in BP.
	UFUNCTION(BlueprintImplementableEvent)
	void GameStart();
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool PlayerWon);
};
