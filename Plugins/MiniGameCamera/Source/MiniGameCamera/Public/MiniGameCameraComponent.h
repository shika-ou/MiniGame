// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"

#include "MiniGameCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAMECAMERA_API UMiniGameCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

protected:
	/** Last calculated view location */
	FVector LastViewLocation;

	/** Last calculated view rotation */
	FRotator LastViewRotation;

public:
	/** Returns the last calculated view location */
	UFUNCTION(BlueprintPure, Category="Titan Camera")
	FVector GetViewLocation()
	{
		return LastViewLocation;
	}

	/** Returns the last calculated view rotation */
	UFUNCTION(BlueprintPure, Category="Titan Camera")
	FRotator GetViewRotation()
	{
		return LastViewRotation;
	}
};
