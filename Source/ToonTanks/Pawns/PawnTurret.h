// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTurret.generated.h"

// -------------------------------------------------------------------------------------------
// Forward declarations.
class APawnTank;

// -------------------------------------------------------------------------------------------
/**
 * Our Enemy AI turrets.
 */
UCLASS()
class TOONTANKS_API APawnTurret : public APawnBase
{
	GENERATED_BODY()

public:
	// ---------------------------------------------------------
	/// Sets default values for this pawn's properties.
	APawnTurret();
	/// Called every frame.
	virtual void Tick(float DeltaTime) override;

private:
	// ---------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta=(AllowPrivateAccess = "true"))
	float FireRate = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta=(AllowPrivateAccess = "true"))
	float ThreatRange = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta=(AllowPrivateAccess = "true"))
	bool EnableDebug = false;

	UPROPERTY() APawnTank* PlayerPawn;

	FColor Blue = FColor(0, 0, 255, 255);

	FVector PlayerPosition;
	FVector TurretPosition;
	FTimerHandle FireRateTimerHandle;

	void CheckFireCondition();
	void CreateFireRateTimer();
	float DistanceToPlayer();
	APawnTank* GetPlayerPawn();

protected:
	// ---------------------------------------------------------
	/// Called when the game starts or when spawned.
	virtual void BeginPlay() override;
	virtual void HandleDestruction() override;

};
