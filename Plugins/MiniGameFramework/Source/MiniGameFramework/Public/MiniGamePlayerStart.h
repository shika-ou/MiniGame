// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerStart.h"
#include "MiniGamePlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAMEFRAMEWORK_API AMiniGamePlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="MiniGame")
	FGameplayTag CameraModeTag;

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetCameraModeTag() { return CameraModeTag; }
};
