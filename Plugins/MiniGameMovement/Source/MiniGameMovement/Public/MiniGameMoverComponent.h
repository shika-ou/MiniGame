// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "MiniGameMoverComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINIGAMEMOVEMENT_API UMiniGameMoverComponent : public UCharacterMoverComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMiniGameMoverComponent();

protected:

	/** Set to true while movement has been disabled externally */
	bool bDisableMovement = false;

public:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	/** Returns true if movement for the owner has been disabled */
	UFUNCTION(BlueprintPure, Category="Mover")
	bool IsMovementDisabled() const { return bDisableMovement; };

	/** Returns the Gameplay Tag Container from the MiniGame Tags Sync State */
	UFUNCTION(BlueprintPure, Category="Mover")
	FGameplayTagContainer GetTagsFromSyncState() const;
	
	/** Returns the last recorded ground contact normal, or a zero vector if not on the ground */
	UFUNCTION(BlueprintPure, Category="Mover")
	FVector GetGroundNormal() const;

protected:

	//////////////////////////
	/// Raft

	/*/** Pointer to the current raft the pawn is riding #1#
	TObjectPtr<AMiniGaRaft> CurrentRaft;

public:

	/** Returns pointer to the current raft if any #1#
	UFUNCTION(BlueprintPure, Category="MiniGame|Raft")
	const AMiniGaRaft* GetRaft() const;*/

	
	///////////////////////////
	/// Glider
	
public:

	/*/** Blueprint overrideable event to return the IK target Transform for the raft's left hand #1#
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="MiniGame|Raft")
	FTransform GetRaftLeftHandTransform() const;

	/** Blueprint overrideable event to return the IK target Transform for the raft's right hand #1#
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="MiniGame|Raft")
	FTransform GetRaftRightHandTransform() const;

	/** Blueprint overrideable event to return the IK target Transform for the raft's left foot #1#
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="MiniGame|Raft")
	FTransform GetRaftLeftFootTransform() const;

	/** Blueprint overrideable event to return the IK target Transform for the raft's right foot #1#
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="MiniGame|Raft")
	FTransform GetRaftRightFootTransform() const;

	/** Blueprint overrideable event to return the IK target Transform for the raft's pelvis socket #1#
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="MiniGame|Raft")
	FTransform GetRaftPelvisTransform() const;*/
};
