// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MiniGameCameraState.generated.h"

class UMiniGameCameraComponent;
/**
 * 
 */
UCLASS()
class MINIGAMECAMERA_API UMiniGameCameraState : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Initializes the camera with this state data. Normally called when this is the first state to get applied. */
	virtual void InitializeCamera(UMiniGameCameraComponent* Camera) const;

	/** Called when this camera state becomes the topmost in the state stack */
	virtual void EnterState(UMiniGameCameraComponent* Camera) const;
	
protected:
	/** Gameplay Tag that identifies this camera state on the stack*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="Camera")
	FGameplayTag Tag;

	/** Speed at which camera state should blend to this state's settings */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category ="Camera|Transition", meta = (ClampMin = 0.0, ClampMax = 5.0))
	float BlendTime = 1.0f;

	/** Desired spring arm length */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category ="Camera|Spring Arm", meta = (ClampMin = 0.0))
	float ArmLength = 600.0f;
};
