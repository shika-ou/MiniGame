#include "MiniGamePlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "MiniGamePlayerStart.h"
#include "GameFramework/Character.h"

// define gameMode tag
UE_DEFINE_GAMEPLAY_TAG(TAG_MINIGAME_GAMEMODE_2DGAME, "MiniGame.GameMode.2DGame");
UE_DEFINE_GAMEPLAY_TAG(TAG_MINIGAME_GAMEMODE_3DGameKeepCamera, "MiniGame.GameMode.3DGameKeepCamera");
UE_DEFINE_GAMEPLAY_TAG(TAG_MINIGAME_GAMEMODE_3DGAME, "MiniGame.GameMode.3DGame");

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

void AMiniGamePlayerController::ChangeCameraGameModeByTag()
{
	AMiniGamePlayerStart* MiniGamePlayerStart = Cast<AMiniGamePlayerStart>(StartSpot);
	if (nullptr == MiniGamePlayerStart)
	{
		return;
	}

	CameraModeTag = MiniGamePlayerStart->GetCameraModeTag();

	ChangeCameraMode();
}

void AMiniGamePlayerController::ChangeCameraMode_Implementation()
{
}

void AMiniGamePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	check(StartSpot.IsValid());

	ChangeCameraGameModeByTag();

	/*ACharacter* ControllerCharacter = Cast<ACharacter>(GetPawn());
	if (nullptr == ControllerCharacter)
	{
		return;
	}
	
	ControllerCharacter->SetActorScale3D(StartSpot->GetActorScale());*/
	/*UPrimitiveComponent* rootcom = Cast<UPrimitiveComponent>(ControllerCharacter->GetRootComponent());
	rootcom->SetSimulatePhysics(false);*/
}
