// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameGameplayAbility.h"

#include "MiniGameAbilitySystemComponent.h"
#include "GameFramework/Character.h"

// event tag definitions
UE_DEFINE_GAMEPLAY_TAG(TAG_MiniGame_Input_Pressed, "MiniGame.Input.Pressed");
UE_DEFINE_GAMEPLAY_TAG(TAG_MiniGame_Input_Ongoing, "MiniGame.Input.Ongoing");
UE_DEFINE_GAMEPLAY_TAG(TAG_MiniGame_Input_Released, "MiniGame.Input.Released");


ACharacter* UMiniGameGameplayAbility::GetMiniGameCharacterFromActorInfo() const
{
	return CurrentActorInfo ? Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}

UMiniGameAbilitySystemComponent* UMiniGameGameplayAbility::GetMiniGameAbilitySystemComponent() const
{
	return (CurrentActorInfo ? Cast<UMiniGameAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}
