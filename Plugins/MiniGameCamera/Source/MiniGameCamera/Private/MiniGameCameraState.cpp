// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameCameraState.h"

#include "MiniGameCameraComponent.h"

void UMiniGameCameraState::InitializeCamera(UMiniGameCameraComponent* Camera) const
{
	check(Camera);

	// set the current interp values
	Camera->CurrentArmLength = ArmLength;


	/** update the pitch limits */
}

void UMiniGameCameraState::EnterState(UMiniGameCameraComponent* Camera) const
{
	check(Camera);
}
