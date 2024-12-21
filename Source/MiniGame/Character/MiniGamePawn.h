// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Pawn.h"
#include "MiniGamePawn.generated.h"

struct FInputActionValue;
struct FMoverInputCmdContext;
struct FInputActionInstance;
class UMiniGameInputEventSet;
class UInputAction;
class UMoverComponent;
class UCameraComponent;
class UMiniGameAbilitySystemComponent;
class UMiniGameAbilitySet;
class AMiniGamePlayerController;
class UAbilitySystemComponent;
class UCapsuleComponent;
class UMiniGameCameraComponent;

// delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMiniGamePawn_OnMoved, FVector2D, MoveInput);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMiniGamePawn_OnJumped, bool, bPressed);

UCLASS()
class MINIGAME_API AMiniGamePawn : public APawn
{
	GENERATED_BODY()

public:
	/** Class constructor */
	AMiniGamePawn(const FObjectInitializer& ObjectInitializer);

	/////////////////////////////////////
	// Components
private:

	/** Player collision capsule */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> PlayerCapsule;

	/** Torso skeletal mesh. This will be the leader pose. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr <USkeletalMeshComponent> TorsoMesh;

	/** Head poseable mesh. This will copy its pose from the torso. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr <USkeletalMeshComponent> HeadMesh;

	/** Headwear poseable mesh. This will copy its pose from the torso. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr <USkeletalMeshComponent> HeadwearMesh;

	/** Legs poseable mesh. This will copy its pose from the torso. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr <USkeletalMeshComponent> LegsMesh;

	/** Glider skeletal mesh. Independent of the rest of the character. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr <USkeletalMeshComponent> GliderMesh;

	/** Ability System Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UMiniGameAbilitySystemComponent> AbilitySystem;
	
	/** Player camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UMiniGameCameraComponent> Camera;

	/** Water Detection Component */
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UMiniGameWaterDetectionComponent> WaterDetection;*/


	/** Mover Component */
	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UMoverComponent/*UMiniGameMoverComponent*/> CharacterMotionComponent;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** 
	 * Called when this Pawn is possessed. Only called on the server (or in standalone).
	 * @param NewController The controller possessing this pawn
	 */
	virtual void PossessedBy(AController* NewController) override;


	UFUNCTION(BlueprintImplementableEvent, Category="MiniGame")
	void OnPawnInitialized(AMiniGamePlayerController* PlayerController);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TObjectPtr<AMiniGamePlayerController> PC;

	/////////////////////////////////////
	// Input
	
	// indirect input delegates other Actors can subscribe to
public:
	/** Move input delegate */
	UPROPERTY(BlueprintAssignable, Category="Input");
	FMiniGamePawn_OnMoved OnMoved;

	/** Jump input delegate */
	UPROPERTY(BlueprintAssignable, Category="Input");
	FMiniGamePawn_OnJumped OnJumped;
	
	// Ability Set to grant to the pawn on initialization
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UMiniGameAbilitySet> AbilitySet;

	/** Ensures Ability Sets are only granted upon first Possess only */
	bool bInitializedAbilities = false;

	
protected:

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	TObjectPtr <UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	TObjectPtr <UInputAction> LookAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	TObjectPtr<UInputAction> JumpAction;

	/** Auto walk Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	TObjectPtr<UInputAction> AutoWalkAction;

	/** Camera distance adjustment Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	TObjectPtr<UInputAction> CameraDistanceAction;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void MoveCompleted(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	void LookCompleted(const FInputActionValue& Value);

	/** Called when the player wants to jump */
	void Jump();

	/** Called when the player wants to stop jumping */
	void StopJumping();

	/** Called for autorun input */
	void AutoWalk();

	/** Called for camera distance adjust input */
	void AdjustCameraDistance(const FInputActionValue& Value);

	/** Called to add wind speed to the Mover inputs */
	UFUNCTION(BlueprintCallable, Category="Titan|Input")
	void AddWind(const FVector& Wind);
public:
	
	/** Setup player input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Input Event Set to grant for triggering Gameplay Abilities */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	TObjectPtr<UMiniGameInputEventSet> InputEventSet;

	/** Input Event Map holding all translated input events and their triggering actions */
	TMap<const UInputAction*, FGameplayTag> InputEventMap;

public:
	/** Binds an input event set to an enhanced input component */
	void BindInputEventSet(const UMiniGameInputEventSet* EventSet, UEnhancedInputComponent* EnhancedInputComponent);

	/** Binds an input action to a gameplay event */
	void BindInputEvent(const UInputAction* InputAction, FGameplayTag EventTag, UEnhancedInputComponent* EnhancedInputComponent);

protected:
	/** Ability input event handlers */
	void HandleInputPressed(const FInputActionInstance& ActionInstance);
	void HandleInputOngoing(const FInputActionInstance& ActionInstance);
	void HandleInputReleased(const FInputActionInstance& ActionInstance);\

	/////////////////////////////////////
	// Mover Interface
	
private:
	/* cached move variables */
	FVector CachedMoveInputIntent = FVector::ZeroVector;
	FRotator CachedTurnInput = FRotator::ZeroRotator;
	FRotator CachedLookInput = FRotator::ZeroRotator;

	/* cached jump variables */
	bool bWantsToJump = false;
	bool bIsJumpPressed = false;

	/** cached sprint variables */
	bool bWantsToSprint = false;
	bool bIsSprintPressed = false;

	/** cached glide variables */
	bool bWantsToGlide = false;
	bool bIsGlidePressed = false;

	/** cached aim variables */
	bool bIsAimPressed = false;

	/** cached wind velocity */
	FVector WindVelocity;

	bool bWantsToAutoWalk = false;

	/** Set to true if a teleport is queued */
	bool bTeleportQueued = false;

	/** Set to true when the teleport target location is preloaded by WP */
	bool bCompletedPreloadingTeleport = false;

	/** Set to true when the pre-teleport animation has completed */
	bool bCompletedTeleportAnimation = false;

	/** Cached teleport target location */
	FVector QueuedTeleportLocation;


protected:
	/* Entry point for input production. */
	//virtual void ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult) override;


	
	/////////////////////////////////////
	// Camera Controls
	
protected:
	/** Scaling factor for camera yaw rotation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Controls")
	float CameraRotationRateYaw = 100.0f;

	/** Scaling factor for camera pitch rotation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Controls")
	float CameraRotationRatePitch = 100.0f;

	/** Time elapsed without input before the camera auto align kicks in */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Auto Alignment")
	float CameraAutoAlignTime = 3.0f;

	/** Speed at which the camera should auto align due to idling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Auto Alignment")
	float CameraAutoAlignSpeed = 25.0f;
	
	/** If true, camera auto align override values will be used instead */
	bool bOverrideCameraAutoAlign = false;
	
	/** Last cached input time. Used to calculate time elapsed for camera auto align */
	float CameraAutoAlignLastInputTime = 0.0f;

	/** Override auto align speed */
	float OverrideCameraAutoAlignSpeed = 10.0f;

	/** Attempts to align the camera yaw towards the pawn's facing direction through move inputs */
	void AlignCameraToFacing(float DeltaTime, float AlignSpeed);

	/** Returns true if the camera should be automatically aligned to the pawn's facing direction */
	bool ShouldAutoAlignCamera();
	
	/** Updates the cached input time to determine camera auto alignment timeout */
	void UpdateCameraAutoAlignTime();

protected:

	/** If true, the camera will be automatically turn to align towards the character when moving sideways */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Auto Alignment")
	bool bAlignCameraOnMovement = true;

	/** Speed at which the camera should auto align due to movement */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Auto Alignment")
	float CameraMovementAlignSpeed = 25.0f;

	/** Returns true if the camera should be automatically aligned to the pawn's facing direction as a result of movement */
	bool ShouldAlignCameraOnMovement();
};
