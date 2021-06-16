// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

void APlayerControllerBase::SetPlayerEnabledState(bool SetPlayerEnabled)
{
	if (SetPlayerEnabled) {
		GetPawn()->EnableInput(this);
	}
	else {
		GetPawn()->DisableInput(this);
	}

	// // Uncomment if we want to shoot at mouse cursor.
	// // Could possibly do this if we used holding right-click to aim freely and hide cursor.
	// bShowMouseCursor = SetPlayerEnabled;

}
