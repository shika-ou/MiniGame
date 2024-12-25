// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"

#include "MiniGameCameraComponent.generated.h"

class UMiniGameCameraState;

UINTERFACE(MinimalAPI)
class UMiniGameCameraOwnerInterface : public UInterface
{
	GENERATED_BODY()
};

class MINIGAMECAMERA_API IMiniGameCameraOwnerInterface
{
	GENERATED_BODY()

public:

	/** Enables or disables camera auto-align */
	virtual void SetCameraAutoAlignState(bool bEnable, float AutoAlignTime, float AutoAlignSpeed) = 0;
};

/**
 *  UMiniGameCameraComponent
 * 
 *  Custom camera component for Titan
 *  Incorporates some built-in Spring Arm elements
 *  Supports a Camera State stack
 *  Supports spring damper blending of multiple camera and spring arm properties
 *  Does not support Additive Offsets or HMD
 */
UCLASS()
class MINIGAMECAMERA_API UMiniGameCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

protected:

	/** Optional owner interface */
	TScriptInterface<IMiniGameCameraOwnerInterface> CameraOwnerInterface;
	
	// Current values used for calculating the view
	/////////////////////////////////////////////////

	/** Current spring arm length */
	float CurrentArmLength;

	/** Current target offset */
	FVector CurrentOffset;
	
	/** Scales the desired spring arm length */
	float CurrentArmLengthMultiplier = 1.0f;

	/** Maximum allowed value for the arm length multiplier */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera|Spring Arm", meta=(ClampMin=0.1, ClampMax=10))
	float MinArmLengthMultiplier = 1.0f;

	/** Maximum allowed value for the arm length multiplier */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera|Spring Arm", meta=(ClampMin=0.1, ClampMax=10))
	float MaxArmLengthMultiplier = 2.0f;
	
	/** Last calculated view location */
	FVector LastViewLocation;

	/** Last calculated view rotation */
	FRotator LastViewRotation;

	/** Last calculated target location, including offsets */
	FVector LastDesiredTarget;
	
	/** Last calculated camera rotation */
	FRotator LastDesiredRotation;

	
	// Camera state stack
	/////////////////////////////

	/** Pointer to the default camera state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera|States")
	UMiniGameCameraState* DefaultCameraState;

	/** Stack of camera states. The LAST element in the array is considered active */
	TArray<UMiniGameCameraState*> CameraStateStack;

protected:

	/** BeginPlay camera initialization */
	virtual  void BeginPlay() override;
	
public:
	/** Returns the currently active camera state */
	UFUNCTION(BlueprintCallable, Category="MiniGame Camera")
	UMiniGameCameraState* GetActiveCameraState() const;
	
	/** Returns the last calculated view location */
	UFUNCTION(BlueprintPure, Category="MiniGame Camera")
	FVector GetViewLocation()
	{
		return LastViewLocation;
	}

	/** Returns the last calculated view rotation */
	UFUNCTION(BlueprintPure, Category="MiniGame Camera")
	FRotator GetViewRotation()
	{
		return LastViewRotation;
	}

public:
	/** Adjusts the spring arm length multiplier by the given delta value. */
	UFUNCTION(BlueprintCallable, Category="Titan Camera")
	void AdjustArmLengthMultiplier(float Delta);

	// friend access to camera states
	friend class UMiniGameCameraState;
};
