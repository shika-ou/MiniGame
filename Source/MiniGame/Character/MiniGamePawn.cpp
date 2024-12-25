// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGamePawn.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "MiniGameAbilitySystemComponent.h"
#include "MiniGameCameraComponent.h"
#include "MiniGameGameplayAbility.h"
#include "MiniGameInputEventSet.h"
#include "MiniGameMoverComponent.h"
#include "MiniGameMoverTypes.h"
#include "MiniGamePlayerController.h"
#include "MoverComponent.h"
#include "MiniGameAbilities/Public/MiniGameAbilitySet.h"
#include "Components/CapsuleComponent.h"


AMiniGamePawn::AMiniGamePawn(const FObjectInitializer& ObjectInitializer)
{
		PrimaryActorTick.bCanEverTick = true;

	//WindVelocity = FVector::ZeroVector;

	// create the collision capsule
	PlayerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Player Capsule"));

	check(PlayerCapsule);

	SetRootComponent(PlayerCapsule);
	PlayerCapsule->InitCapsuleSize(34.0f, 88.0f);
	PlayerCapsule->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	PlayerCapsule->CanCharacterStepUpOn = ECB_No;
	PlayerCapsule->SetShouldUpdatePhysicsVolume(true);
	PlayerCapsule->SetCanEverAffectNavigation(false);
	PlayerCapsule->bDynamicObstacle = true;

	// create the torso skeletal mesh
	TorsoMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Torso Mesh"));

	check(TorsoMesh);

	TorsoMesh->SetupAttachment(PlayerCapsule);
	TorsoMesh->AlwaysLoadOnClient = true;
	TorsoMesh->AlwaysLoadOnServer = true;
	TorsoMesh->bOwnerNoSee = false;
	TorsoMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	TorsoMesh->bCastDynamicShadow = true;
	TorsoMesh->bAffectDynamicIndirectLighting = true;
	TorsoMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;

	static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
	TorsoMesh->SetCollisionProfileName(MeshCollisionProfileName);
	TorsoMesh->SetGenerateOverlapEvents(false);
	TorsoMesh->SetCanEverAffectNavigation(false);

	// create the head skinned mesh
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head Mesh"));

	check(HeadMesh);

	HeadMesh->SetupAttachment(TorsoMesh);
	HeadMesh->AlwaysLoadOnClient = true;
	HeadMesh->AlwaysLoadOnServer = true;
	HeadMesh->bOwnerNoSee = false;
	HeadMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	HeadMesh->bCastDynamicShadow = true;
	HeadMesh->bAffectDynamicIndirectLighting = true;
	HeadMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;

	HeadMesh->SetCollisionProfileName(MeshCollisionProfileName);
	HeadMesh->SetGenerateOverlapEvents(false);
	HeadMesh->SetCanEverAffectNavigation(false);

	// create the headwear mesh
	HeadwearMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Headwear Mesh"));

	check(HeadwearMesh);

 	HeadwearMesh->SetupAttachment(TorsoMesh);
	HeadwearMesh->AlwaysLoadOnClient = true;
	HeadwearMesh->AlwaysLoadOnServer = true;
	HeadwearMesh->bOwnerNoSee = false;
	HeadwearMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	HeadwearMesh->bCastDynamicShadow = true;
	HeadwearMesh->bAffectDynamicIndirectLighting = true;
	HeadwearMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;

	HeadwearMesh->SetCollisionProfileName(MeshCollisionProfileName);
	HeadwearMesh->SetGenerateOverlapEvents(false);
	HeadwearMesh->SetCanEverAffectNavigation(false);
	

	// create the lower body mesh
	LegsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs Mesh"));

	check(LegsMesh);

	LegsMesh->SetupAttachment(TorsoMesh);
	LegsMesh->AlwaysLoadOnClient = true;
	LegsMesh->AlwaysLoadOnServer = true;
	LegsMesh->bOwnerNoSee = false;
	LegsMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	LegsMesh->bCastDynamicShadow = true;
	LegsMesh->bAffectDynamicIndirectLighting = true;
	LegsMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;

	LegsMesh->SetCollisionProfileName(MeshCollisionProfileName);
	LegsMesh->SetGenerateOverlapEvents(false);
	LegsMesh->SetCanEverAffectNavigation(false);
	
	// create the glider skeletal mesh
	GliderMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Glider Mesh"));

	check(GliderMesh);

	GliderMesh->SetupAttachment(PlayerCapsule);
	GliderMesh->AlwaysLoadOnClient = true;
	GliderMesh->AlwaysLoadOnClient = true;
	GliderMesh->AlwaysLoadOnServer = true;
	GliderMesh->bOwnerNoSee = false;
	GliderMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	GliderMesh->bCastDynamicShadow = true;
	GliderMesh->bAffectDynamicIndirectLighting = true;
	GliderMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;

	// create the camera
	Camera = CreateDefaultSubobject<UMiniGameCameraComponent>(TEXT("Camera"));

	check(Camera);

	Camera->SetupAttachment(PlayerCapsule);

	// create the Mover component
	CharacterMotionComponent = CreateDefaultSubobject<UMiniGameMoverComponent>(TEXT("MoverComponent"));
	ensure(CharacterMotionComponent);

	// create the ASC
	AbilitySystem = CreateDefaultSubobject<UMiniGameAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	check(AbilitySystem);

	/*// create the water detection comp
	WaterDetection = CreateDefaultSubobject<UMiniGameWaterDetectionComponent>(TEXT("Water Detection"));
	check(WaterDetection);

	// add water detection as a tick prerequisite
	AddTickPrerequisiteComponent(WaterDetection);*/

	// disable Actor-level movement replication, since our Mover component will handle it
	SetReplicatingMovement(false);
}

// Called every frame
void AMiniGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// spin the camera base on input
	if (PC)
	{
		// apply control input
		PC->AddYawInput(CachedLookInput.Yaw * CameraRotationRateYaw * DeltaTime / GetActorTimeDilation());
		PC->AddPitchInput(-CachedLookInput.Pitch * CameraRotationRatePitch * DeltaTime / GetActorTimeDilation());

		if (CachedMoveInputIntent.Size() > 0.f)
		{
			if (ShouldAlignCameraOnMovement())
			{
				AlignCameraToFacing(DeltaTime, CameraMovementAlignSpeed * CachedMoveInputIntent.GetClampedToMaxSize(1.f).Size());
			}

			// calculate the time since our last relevant input
			float TimeSinceLastInput = GetWorld()->GetTimeSeconds() - CameraAutoAlignLastInputTime;

			float AutoAlignTime = bOverrideCameraAutoAlign ? bOverrideCameraAutoAlign : CameraAutoAlignSpeed;

			// check if it is time to auto align the camera
			if (ShouldAutoAlignCamera() && TimeSinceLastInput >= AutoAlignTime)
			{
				float AutoAlignSpeed = bOverrideCameraAutoAlign ? bOverrideCameraAutoAlign : CameraAutoAlignSpeed;

				AlignCameraToFacing(DeltaTime, AutoAlignSpeed);
			}
		}
	}

}

void AMiniGamePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PC = Cast<AMiniGamePlayerController>(NewController);

	if (ensure(PC))
	{
		// initialize the ability system
		if (ensure(AbilitySystem))
		{
			AbilitySystem->InitAbilityActorInfo(this, this);

			// grant the ability set but only once
			if (ensure(AbilitySet) && !bInitializedAbilities)
			{
				bInitializedAbilities = true;

				AbilitySet->GiveToAbilitySystem(AbilitySystem, nullptr);

				// call BP initialization handle
				OnPawnInitialized(PC);
			}
			
		}

		// initialize the camera pitch limits
		//Camera->InitializeCameraForPlayer();
	}
	
}

// Called when the game starts or when spawned
void AMiniGamePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMiniGamePawn::Move(const FInputActionValue& Value)
{
	// input is a vector 2D
	FVector2d MovementVector = Value.Get<FVector2d>();

	// set up the input vector. we flip the axis, so they correspond with the expected Mover input
	CachedMoveInputIntent.X = FMath::Clamp(MovementVector.Y, -1.f, 1.f);
	CachedMoveInputIntent.Y = FMath::Clamp(MovementVector.X, -1.f, 1.f);

	// cancel autorun if the input intent is nonzero
	if (!CachedMoveInputIntent.IsNearlyZero())
	{
		bWantsToAutoWalk = false;
	}

	// broadcast the delegate
	OnMoved.Broadcast(MovementVector);

	// update the camera auto align timeout
	//UpdateCameraAutoAlignTime();
}

void AMiniGamePawn::MoveCompleted(const FInputActionValue& Value)
{
	// zero out the cached input
	CachedMoveInputIntent = FVector::ZeroVector;

	// broadcast the delegate
	OnMoved.Broadcast(FVector2D::ZeroVector);
	
}

void AMiniGamePawn::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// set up the look input rotator
	CachedLookInput.Yaw = CachedTurnInput.Yaw = FMath::Clamp(LookAxisVector.X, -1.f, 1.f);
	CachedLookInput.Pitch = CachedTurnInput.Pitch = FMath::Clamp(LookAxisVector.Y, -1.f, 1.f);

	// update the camera auto align timeout
	UpdateCameraAutoAlignTime();
}

void AMiniGamePawn::LookCompleted(const FInputActionValue& Value)
{
	// zero out the cached input
	CachedLookInput = FRotator::ZeroRotator;
}

void AMiniGamePawn::Jump()
{
	// is this the first frame we want to jump?
	bWantsToJump = !bIsJumpPressed;

	// update the flag
	bIsJumpPressed = true;

	// broadcast the delegate
	OnJumped.Broadcast(true);

	// update the camera auto align timeout
	UpdateCameraAutoAlignTime();
}

void AMiniGamePawn::StopJumping()
{
	// reset the flag
	bIsJumpPressed = bWantsToJump = false;

	// broadcast the delegate
	OnJumped.Broadcast(false);
}

void AMiniGamePawn::AutoWalk()
{
	// toggle autorun
	bWantsToAutoWalk = !bWantsToAutoWalk;
}

void AMiniGamePawn::AdjustCameraDistance(const FInputActionValue& Value)
{
	Camera->AdjustArmLengthMultiplier(Value.Get<float>());
}

void AMiniGamePawn::AddWind(const FVector& Wind)
{
}

// Called to bind functionality to input
void AMiniGamePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMiniGamePawn::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMiniGamePawn::MoveCompleted);

		// Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMiniGamePawn::Look);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Completed, this, &AMiniGamePawn::LookCompleted);

		// Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMiniGamePawn::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMiniGamePawn::StopJumping);

		// Autorun
		EnhancedInputComponent->BindAction(AutoWalkAction, ETriggerEvent::Completed, this, &AMiniGamePawn::AutoWalk);

		// Camera distance adjust
		EnhancedInputComponent->BindAction(CameraDistanceAction, ETriggerEvent::Triggered, this, &AMiniGamePawn::AdjustCameraDistance);

		// bind the input event set
		BindInputEventSet(InputEventSet, EnhancedInputComponent);
	}
}

void AMiniGamePawn::BindInputEventSet(const UMiniGameInputEventSet* EventSet,
	UEnhancedInputComponent* EnhancedInputComponent)
{
	if (!EventSet)
	{
		return;
	}

	// grant the input event set to this pawn
	EventSet->GiveToPawn(this,EnhancedInputComponent);
}

void AMiniGamePawn::BindInputEvent(const UInputAction* InputAction, FGameplayTag EventTag,
	UEnhancedInputComponent* EnhancedInputComponent)
{
	// ensure input action is valid
	if (!IsValid(InputAction))
	{
		return;
	}

	// ensure we only one binding per input action
	if (InputEventMap.Find(InputAction))
	{
		return;
	}

	// ensure the event tag is valid
	if (EventTag == FGameplayTag::EmptyTag)
	{
		return;
	}

	// save the event tag to the action map
	InputEventMap.Add(InputAction, EventTag);

	// create the bindings
	EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AMiniGamePawn::HandleInputPressed);
	EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Ongoing, this, &AMiniGamePawn::HandleInputOngoing);
	EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AMiniGamePawn::HandleInputReleased);
}

void AMiniGamePawn::HandleInputPressed(const FInputActionInstance& ActionInstance)
{
	const UInputAction* InputAction = ActionInstance.GetSourceAction();

	if (FGameplayTag* EventMapping = InputEventMap.Find(InputAction))
	{
		FGameplayEventData Payload;
		Payload.InstigatorTags.AddTag(TAG_MiniGame_Input_Pressed);

		AbilitySystem->HandleGameplayEvent(*EventMapping, &Payload);
	}
}

void AMiniGamePawn::HandleInputOngoing(const FInputActionInstance& ActionInstance)
{
	const UInputAction* InputAction = ActionInstance.GetSourceAction();

	if (FGameplayTag* EventMapping = InputEventMap.Find(InputAction))
	{
		FGameplayEventData Payload;
		Payload.InstigatorTags.AddTag(TAG_MiniGame_Input_Ongoing);

		AbilitySystem->HandleGameplayEvent(*EventMapping, &Payload);
	}
}

void AMiniGamePawn::HandleInputReleased(const FInputActionInstance& ActionInstance)
{
	const UInputAction* InputAction = ActionInstance.GetSourceAction();

	if (FGameplayTag* EventMapping = InputEventMap.Find(InputAction))
	{
		FGameplayEventData Payload;
		Payload.InstigatorTags.AddTag(TAG_MiniGame_Input_Released);

		AbilitySystem->HandleGameplayEvent(*EventMapping, &Payload);
	}
}

void AMiniGamePawn::ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult)
{
	float DeltaMs = (float)SimTimeMs;

	FCharacterDefaultInputs& DefaultkinematicInputs = InputCmdResult.InputCollection.FindOrAddMutableDataByType<FCharacterDefaultInputs>();
	FMiniGameMovementInputs& MiniGameInputs = InputCmdResult.InputCollection.FindOrAddMutableDataByType<FMiniGameMovementInputs>();

	static const FCharacterDefaultInputs DoNothingInput;

	// do we have a controller ?
	if (nullptr == Controller)
	{
		// are we the authority ?
		if (GetLocalRole() == ENetRole::ROLE_Authority && GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
		{
			// no controller, so pass a do nothing input
			DefaultkinematicInputs = DoNothingInput;
		}

		// no need to run input code without a controller
		return;
	}

	// is movement disabled ?
	if (GetMoverComponent()->IsMovementDisabled())
	{
		// pass a do nothing input
		DefaultkinematicInputs = DoNothingInput;
	}

	DefaultkinematicInputs.ControlRotation = FRotator::ZeroRotator;

	// copy the control rotation
	if (PC)
	{
		DefaultkinematicInputs.ControlRotation = PC->GetControlRotation();
	}

	// force the forward input intent if autoWalk is on
	FVector MoveInputIntent = CachedMoveInputIntent;

	if (bWantsToAutoWalk)
	{
		MoveInputIntent.X = 1.f;
	}

	// use only the control rotation yaw to avoid tapering out inputs if looking at the character from a too low or too high angle
	FRotator ControlFacing = FRotator::ZeroRotator;
	ControlFacing.Yaw = DefaultkinematicInputs.ControlRotation.Yaw;

	// set the move input
	DefaultkinematicInputs.SetMoveInput(EMoveInputType::DirectionalIntent, ControlFacing.RotateVector(MoveInputIntent));

	// check if we have a nonzero input
	static float RotationMagMin(1e-3);
	const bool bHasAffirmativeMoveInput = (DefaultkinematicInputs.GetMoveInput().Size() >= RotationMagMin);

	// Figure out the orientation intent for the character
	DefaultkinematicInputs.OrientationIntent = FVector::ZeroVector;

	if (bIsAimPressed)
	{
		// set the orientation intent to the camera forward vector
		DefaultkinematicInputs.OrientationIntent = Camera->GetViewRotation().RotateVector(FVector::ForwardVector);

		// save the last nonzero input
		LastAffirmativeMoveInput = DefaultkinematicInputs.OrientationIntent;
	}
	// do we have an affirmative input intent?
	else if (bHasAffirmativeMoveInput)
	{
		// set the orientation intent to the movement direction
		DefaultkinematicInputs.OrientationIntent = DefaultkinematicInputs.GetMoveInput();

		// save the last nonzero input
		LastAffirmativeMoveInput = DefaultkinematicInputs.OrientationIntent;
	}
	else
	{
		// no input intent, so keep the last orientation from input
		DefaultkinematicInputs.OrientationIntent = LastAffirmativeMoveInput;
	}

	// cancel out any z intent to keep the actor verical
	DefaultkinematicInputs.OrientationIntent = DefaultkinematicInputs.OrientationIntent.GetSafeNormal2D();

	// set the jump inputs
	DefaultkinematicInputs.bIsJumpPressed = bIsJumpPressed;
	DefaultkinematicInputs.bIsJumpJustPressed = bWantsToJump;

	// set the sprint inputs
	MiniGameInputs.bIsSprintPressed = bIsSprintPressed;
	MiniGameInputs.bIsSprintJustPressed = bWantsToSprint;

	// set the glide inputs
	MiniGameInputs.bIsGlidePressed = bIsGlidePressed;
	MiniGameInputs.bIsGlideJustPressed = bWantsToGlide;

	// set the wind velocity
	MiniGameInputs.Wind = WindVelocity;

	// Convert inputs to be relative to the current movement base (depending on options and state)
	DefaultkinematicInputs.bUsingMovementBase = false;

	// get the Mover component
	if (const UMoverComponent* MoverComponent = GetMoverComponent())
	{
		// get the Mover component
		if (UPrimitiveComponent* MovementBase = MoverComponent->GetMovementBase())
		{
			FName MovementBaseBoneName = MoverComponent->GetMovementBaseBoneName();

			FVector RelativeMoveInput, RelativeOrientDir;

			UBasedMovementUtils::TransformWorldDirectionToBased(MovementBase, MovementBaseBoneName, DefaultkinematicInputs.GetMoveInput(), RelativeMoveInput);
			UBasedMovementUtils::TransformWorldDirectionToBased(MovementBase, MovementBaseBoneName, DefaultkinematicInputs.OrientationIntent, RelativeOrientDir);

			DefaultkinematicInputs.SetMoveInput(DefaultkinematicInputs.GetMoveInputType(), RelativeMoveInput);
			DefaultkinematicInputs.OrientationIntent = RelativeOrientDir;

			DefaultkinematicInputs.bUsingMovementBase = true;
			DefaultkinematicInputs.MovementBase = MovementBase;
			DefaultkinematicInputs.MovementBaseBoneName = MovementBaseBoneName;
		}
	}

	// Clear/consume the inputs
	bWantsToJump = false;
	bWantsToGlide = false;
	bWantsToSprint = false;
}

void AMiniGamePawn::AlignCameraToFacing(float DeltaTime, float AlignSpeed)
{
	/*// get the camera facing vector
	FVector CameraFacing = Camera->GetViewRotation().RotateVector(FVector::ForwardVector);
	CameraFacing = CameraFacing.GetSafeNormal2D();

	// dot product with our right vector to get the yaw input strength
	float FacingDot = -FVector::DotProduct(CameraFacing, GetActorRightVector());

	// rotate the camera facing through a controller yaw input
	PC->AddYawInput(FacingDot * AlignSpeed * DeltaTime);*/
}

bool AMiniGamePawn::ShouldAutoAlignCamera()
{
	// skip auto alignment if we are aiming the grapple
	return !bIsAimPressed || bOverrideCameraAutoAlign;
}

void AMiniGamePawn::UpdateCameraAutoAlignTime()
{
	CameraAutoAlignLastInputTime = GetWorld()->GetTimeSeconds();
}

void AMiniGamePawn::SetCameraAutoAlignState(bool bEnable, float AutoAlignTime, float AutoAlignSpeed)
{
	bOverrideCameraAutoAlign = bEnable;

	OverrideCameraAutoAlignTime = AutoAlignTime;
	OverrideCameraAutoAlignSpeed = AutoAlignSpeed;
}

bool AMiniGamePawn::ShouldAlignCameraOnMovement()
{
	return (bAlignCameraOnMovement && !bIsAimPressed);
}


