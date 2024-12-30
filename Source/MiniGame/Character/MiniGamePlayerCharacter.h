// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "MiniGamePlayerCharacter.generated.h"

class UMiniGameAbilitySet;
class UMiniGameAbilitySystemComponent;
class UUserWidget;

UENUM(BlueprintType)
enum ESkinMode
{
	Mesh,
	Material,
};

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
	///////////////////////////////////
	/// Strafe
	/** the bool for camera whether the camera should stay behind the character */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="MiniGame|Movement")
	uint8 bWantsToStrafe;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="MiniGame|Camera")
	FRotator FallingRotationRate;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="MiniGame|Camera")
	FRotator NoFallingRotationRate;

	///////////////////////////////
	/// HUD
	// HUD blueprint class
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="MiniGame|HUD")
	TSubclassOf<UUserWidget> HUDClass;

	///////////////////////////////
	/// Ability
	// Ability Set to grant to the pawn on initialization
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MiniGame|Abilities")
	TObjectPtr<UMiniGameAbilitySet> AbilitySet;

	/////////////////////////////////
	/// CameraMode
	// the tag for change camera mode
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MiniGame|CameraMode")
	FGameplayTag CameraModeTag;

	
	//////////////////////////////////
	/// Skin
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MiniGame|Skin")
	TEnumAsByte<ESkinMode> SkinMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MiniGame|Skin", meta=(EditCondition="SkinMode==Material"))
	TArray<TObjectPtr<UMaterialInstance>> MaterialArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MiniGame|Skin", meta=(EditCondition="SkinMode==Mesh"))
	TArray<TObjectPtr<USkeletalMesh>> SkeletalMeshArray;

public:
	/////////////////////////////////
	/// Hit Anim
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MiniGame|HitReation")
	TObjectPtr<UAnimMontage> HitMontage;

private:
	/** Ensures Ability Sets are only granted upon first Possess only */
	bool bInitializedAbilities;
	
	TObjectPtr<UUserWidget> HUD;

	/** Ability System Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame|AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UMiniGameAbilitySystemComponent> AbilitySystemComponent;

public:
	// get character asc
	UFUNCTION(BlueprintCallable)
	UMiniGameAbilitySystemComponent* GetMiniGameAbilitySystemComponent() { return AbilitySystemComponent; }
	
	// change player skin from actor
	UFUNCTION(BlueprintNativeEvent)
	void ChangeSkinBySkinIndex(int32 SkinIndex);
	
	// change 
	UFUNCTION(BlueprintCallable)
	void ChangeToSkin(int32 SkinIndex);

	
	
private:
	void CreateHUD();

	void ResetHUD();
	
	UFUNCTION(BlueprintCallable)
	void SetStrafeStateByWantsToStrafe();

	UFUNCTION(BlueprintCallable)
	void SetRotationRateByFallingState();

	void ChangeCameraModeByCameraTag() const;
};
