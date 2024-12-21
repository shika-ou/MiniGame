// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameInputEventSet.h"

#include "MiniGamePawn.h"
#include "MiniGame/Logging/MiniGameLogChannels.h"
#include "InputAction.h"

UMiniGameInputEventSet::UMiniGameInputEventSet(const FObjectInitializer& ObjectInitializer)
{
}

void UMiniGameInputEventSet::GiveToPawn(AMiniGamePawn* MiniGamePawn, UEnhancedInputComponent* InputComponent) const
{
	check(MiniGamePawn);
	check(InputComponent);

	// iterate through all granted input events
	for (int32 index = 0; index < GrantedInputEvents.Num(); ++index)
	{
		// get the input event
		const FMiniGameInputEventSet_InputEvent& InputEvent = GrantedInputEvents[index];

		// ensure the input action is valid
		if (false == ensure(IsValid(InputEvent.InputAction))) 
		{
			UE_LOG(LogMiniGameCharacter, Warning, TEXT("GrantedInputEvents[%d] on Input Event Set [%s] is not valid."), index, *GetNameSafe(this));
		}

		// bind the input event
		MiniGamePawn->BindInputEvent(InputEvent.InputAction, InputEvent.InputEventTag, InputComponent);
	}
}
