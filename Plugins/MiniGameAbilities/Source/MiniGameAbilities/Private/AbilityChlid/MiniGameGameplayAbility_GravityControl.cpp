// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityChlid/MiniGameGameplayAbility_GravityControl.h"

#include "MiniGamePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/MovementComponent.h"

void UMiniGameGameplayAbility_GravityControl::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ChangeGravity();
}

void UMiniGameGameplayAbility_GravityControl::ChangeGravity()
{
	APawn* OwnerActorPawn = Cast<APawn>(GetOwningActorFromActorInfo());
	if (nullptr == OwnerActorPawn)
	{
		return;
	}

	// get movement component
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(OwnerActorPawn->FindComponentByClass(UCharacterMovementComponent::StaticClass()));
	if (nullptr == MovementComponent)
	{
		return;
	}
	
	// get mini game controller
	AMiniGamePlayerController* MiniGamePlayerController = Cast<AMiniGamePlayerController>(OwnerActorPawn->GetController());
	if (nullptr == MiniGamePlayerController)
	{
		return;
	}

	// change the input by camera tag
	MiniGamePlayerController->SetCameraModeTag(MovementComponent->GetGravityDirection() == GravityVector ? TAG_MINIGAME_GAMEMODE_2DGAME : TAG_MINIGAME_GAMEMODE_3DGameKeepCamera);

	// add force to the actor use computed gravity
	MovementComponent->SetGravityDirection(MovementComponent->GetGravityDirection() == GravityVector ? FVector(0, 0, -1) : GravityVector);
}
