#include "MiniGameCameraComponent.h"

#include "MiniGameCameraState.h"

void UMiniGameCameraComponent::BeginPlay()
{
	Super::BeginPlay();

#if ENABLE_VISUAL_LOG
	// redirect Visual Logger to the owning Actor
	REDIRECT_TO_VLOG(GetOwner());
#endif

	// cast the owner to the camera interface
	CameraOwnerInterface = GetOwner();

	// initialize and enter the default camera state
	GetActiveCameraState()->InitializeCamera(this);
	GetActiveCameraState()->EnterState(this);

	// initialize the cached target location and rotation from last frame
	LastDesiredRotation = GetComponentRotation();
	LastDesiredTarget = GetComponentLocation() + LastDesiredRotation.RotateVector(CurrentOffset); 
}

UMiniGameCameraState* UMiniGameCameraComponent::GetActiveCameraState() const
{
	// if we have states in the stack, return the last one
	if (CameraStateStack.Num() > 0)
	{
		return CameraStateStack[CameraStateStack.Num() - 1];
	}

	// otherwise return the default camera state
	return DefaultCameraState;
}

void UMiniGameCameraComponent::AdjustArmLengthMultiplier(float Delta)
{
	CurrentArmLengthMultiplier = FMath::Clamp(CurrentArmLengthMultiplier + Delta, MinArmLengthMultiplier, MaxArmLengthMultiplier);
}
