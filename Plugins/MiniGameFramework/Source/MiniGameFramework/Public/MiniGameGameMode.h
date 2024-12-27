// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MiniGameGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAMEFRAMEWORK_API AMiniGameGameMode : public AGameMode
{
	GENERATED_BODY()


public:
	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;
};
