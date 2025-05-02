// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Camera/Combat_PlayerCameraMangaer.h"

#include "GameFramework/CharacterMovementComponent.h"

bool ACombat_PlayerCameraMangaer::IsOwnerStrafing() const
{
	bool bIsStrafing = false;
	if (MovementComponent && OwnerPawn)
	{
		bIsStrafing = OwnerPawn->bUseControllerRotationYaw || (MovementComponent->bUseControllerDesiredRotation && !MovementComponent->bOrientRotationToMovement);
	}
	else if (OwnerPawn)
	{
		/*if (OwnerPawn->GetClass()->ImplementsInterface(UUGC_PawnMovementInterface::StaticClass()))
		{
			bIsStrafing = IUGC_PawnMovementInterface::Execute_IsOwnerStrafing(OwnerPawn);
		}
		else
		{
			bIsStrafing = OwnerPawn->bUseControllerRotationYaw;
		}*/
		bIsStrafing = OwnerPawn->bUseControllerRotationYaw;
	}
	return bIsStrafing;
}
