// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animations/CameraAnimationCameraModifier.h"

#include "Combat_CameraAnimationModifier.generated.h"

DECLARE_DELEGATE_TwoParams(FOnCameraAnimationEnded, class UCameraAnimationSequence*, bool /*bInterrupted*/)
DECLARE_DELEGATE_OneParam(FOnCameraAnimationEaseOutStarted, class UCameraAnimationSequence*)

UENUM()
enum class ECameraAnimationResetType : uint8
{
	BackToStart UMETA(ToolTip = "The camera will go back to the position it started from."),
	ResetToZero UMETA(ToolTip = "The camera's orientation will be reset to zero. This is usually the back of the character. If UseControllerRotationYaw is true, this is forcefully used."),
	ContinueFromEnd UMETA(ToolTip = "The camera will blend out from the last position of the animation.")
};

class ACombat_PlayerCameraMangaer;
/**
 * 
 */
UCLASS()
class GAMEFEATURE_COMBATRUNTIME_API UCombat_CameraAnimationModifier : public UCameraAnimationCameraModifier
{
	GENERATED_BODY()

	void CombatTickActiveAnimation(float DeltaTime, FMinimalViewInfo& InOutPOV);
	
	void CombatTickAnimation(FActiveCameraAnimationInfo& CameraAnimation, float DeltaTime, FMinimalViewInfo& InOutPOV);
	void CombatDeactivateCameraAnimation(FActiveCameraAnimationInfo& ActiveAnimation);

	virtual bool ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV) override;

	TObjectPtr<ACombat_PlayerCameraMangaer> CombatCameraManager;

	UPROPERTY(Transient)
	ECameraAnimationResetType CurrentResetType = ECameraAnimationResetType::ResetToZero;
	
	bool bWasEasingOut = false;
	
	// Delegates
	FOnCameraAnimationEnded OnAnimationEnded;
	FOnCameraAnimationEaseOutStarted OnAnimationEaseOutStarted;
};
