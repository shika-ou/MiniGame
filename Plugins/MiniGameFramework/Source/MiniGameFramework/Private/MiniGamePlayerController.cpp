#include "MiniGamePlayerController.h"

#include "EnhancedInputSubsystems.h"

AMiniGamePlayerController::AMiniGamePlayerController(const FObjectInitializer& ObjectInitializer)
{
}

void AMiniGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only deal with input if we're the local player
	if (HasAuthority())
	{
		// get the input subsystem
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			// add the default input mapping contexts
			for (TObjectPtr<UInputMappingContext>& CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
}
