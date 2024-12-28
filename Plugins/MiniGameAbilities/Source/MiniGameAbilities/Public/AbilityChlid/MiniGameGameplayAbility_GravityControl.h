// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MiniGameGameplayAbility.h"
#include "MiniGameGameplayAbility_GravityControl.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAMEABILITIES_API UMiniGameGameplayAbility_GravityControl : public UMiniGameGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MiniGame")
	FVector GravityVector;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	void ChangeGravity() const;
};
