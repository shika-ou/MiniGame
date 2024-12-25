// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "GameFramework/Character.h"
#include "MiniGamePlayerCharacter.generated.h"

class UMiniGameAbilitySet;
class UMiniGameAbilitySystemComponent;
class UUserWidget;
// declare gameMode tag
MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MINIGAME_GAMEMODE_2DGAME);
MINIGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MINIGAME_GAMEMODE_3DGAME);

struct FGameplayTag;

UCLASS()
class MINIGAME_API AMiniGamePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMiniGamePlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// initialize the ability system
	void InitializeAbilitySystem();

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

protected:
	
	/** the tag for distinction that 2DGame and 3DGame */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GameMode")
	FGameplayTag GameModeTag;

	/** the bool for camera whether the camera should stay behind the character */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="MiniGame|Movement")
	uint8 bWantsToStrafe;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="MiniGame|Camera")
	FRotator FallingRotationRate;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="MiniGame|Camera")
	FRotator NoFallingRotationRate;

	// HUD blueprint class
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="MiniGame|HUD")
	TSubclassOf<UUserWidget> HUDClass;
	
	// Ability Set to grant to the pawn on initialization
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MiniGame|Abilities")
	TObjectPtr<UMiniGameAbilitySet> AbilitySet;

private:
	/** Ensures Ability Sets are only granted upon first Possess only */
	bool bInitializedAbilities;
	
	TObjectPtr<UUserWidget> HUD;

	/** Ability System Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame|AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UMiniGameAbilitySystemComponent> AbilitySystemComponent;

public:
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetGameModeTag() { return GameModeTag; }

	UFUNCTION(BlueprintCallable)
	UMiniGameAbilitySystemComponent* GetMiniGameAbilitySystemComponent() { return AbilitySystemComponent; }

	
private:
	void CreateHUD();

	void ResetHUD();
	
	UFUNCTION(BlueprintCallable)
	void SetStrafeStateByWantsToStrafe();

	UFUNCTION(BlueprintCallable)
	void SetRotationRateByFallingState();
};
