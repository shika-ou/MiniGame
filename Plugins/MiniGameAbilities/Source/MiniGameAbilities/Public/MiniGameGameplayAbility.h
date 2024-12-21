// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Abilities/GameplayAbility.h"

#include "MiniGameGameplayAbility.generated.h"

// input event tags
MINIGAMEABILITIES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MiniGame_Input_Pressed);
MINIGAMEABILITIES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MiniGame_Input_Ongoing);
MINIGAMEABILITIES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MiniGame_Input_Released);

/**
 *  UMiniGameGameplayAbility
 *  Extends GameplayAbility with extra functionality:
 *  - Input event handling
 */
UCLASS(Abstract)
class MINIGAMEABILITIES_API UMiniGameGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
};
