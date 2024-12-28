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

void UMiniGameGameplayAbility_GravityControl::ChangeGravity() const
{
	AActor* OwnerActor = GetOwningActorFromActorInfo();
	if (nullptr == OwnerActor)
	{
		return;
	}
	
	UCharacterMovementComponent* TargetCharacterMovementComponent = Cast<UCharacterMovementComponent>(OwnerActor->FindComponentByClass(UCharacterMovementComponent::StaticClass()));
	if (nullptr == TargetCharacterMovementComponent)
	{
		return;
	}
	
	// add force to the actor use computed gravity
	TargetCharacterMovementComponent->SetGravityDirection(TargetCharacterMovementComponent->GetGravityDirection() == GravityVector ? FVector(0, 0, -1) : GravityVector);
}
