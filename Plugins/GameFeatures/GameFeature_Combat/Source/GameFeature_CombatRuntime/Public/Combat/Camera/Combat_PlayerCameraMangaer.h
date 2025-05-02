// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Combat_PlayerCameraMangaer.generated.h"

/**
 * 
 */
UCLASS()
class GAMEFEATURE_COMBATRUNTIME_API ACombat_PlayerCameraMangaer : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "UGC|Camera Manager|Movement")
	bool IsOwnerStrafing() const;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "UGC|Camera Manager|Internal")
	TObjectPtr<class APawn> OwnerPawn;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "UGC|Camera Manager|Internal")
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;
};
