// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MiniGamePawn.generated.h"

class UMoverComponent;
class UCameraComponent;
class UMiniGameAbilitySystemComponent;
class UMiniGameAbilitySet;
class AMiniGamePlayerController;
class UAbilitySystemComponent;
class UCapsuleComponent;

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
	TObjectPtr <UCameraComponent/*UMiniGameCameraComponent*/> Camera;

	/** Water Detection Component */
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UMiniGameWaterDetectionComponent> WaterDetection;*/


	/** Mover Component */
	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UMoverComponent/*UMiniGameMoverComponent*/> CharacterMotionComponent;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	
	// Ability Set to grant to the pawn on initialization
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UMiniGameAbilitySet> AbilitySet;

	/** Ensures Ability Sets are only granted upon first Possess only */
	bool bInitializedAbilities = false;
};
