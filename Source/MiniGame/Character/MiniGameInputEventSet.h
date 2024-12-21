// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MiniGameInputEventSet.generated.h"

class UInputAction;
class AMiniGamePawn;

/**
 *  FMiniGameInputEventSet_InputEvent
 *  An individual input event struct to translate an input action to a Gameplay Event
 */
USTRUCT(BlueprintType)
struct FMiniGameInputEventSet_InputEvent
{
	GENERATED_BODY()

public:

	// Input Action that triggers the event
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InputAction = nullptr;

	// Tag of the event triggered in the ASC when the action is triggered
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputEventTag;
};

/**
 *  UMiniGameInputEventSet
 *  Non-mutable data asset used to bind Gameplay Event triggers to Input Actions
 */
UCLASS()
class MINIGAME_API UMiniGameInputEventSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UMiniGameInputEventSet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GiveToPawn(AMiniGamePawn* MiniGamePawn, UEnhancedInputComponent* InputComponent) const;

protected:

	// Input Events to bind when this input event set is granted.
	UPROPERTY(EditDefaultsOnly, Category="Input Events", meta=(TitleProperty=InputAction))
	TArray<FMiniGameInputEventSet_InputEvent> GrantedInputEvents;
};
