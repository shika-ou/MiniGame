// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "GameFramework/PlayerController.h"
#include "MiniGamePlayerController.generated.h"

class UInputMappingContext;

// declare gameMode tag
MINIGAMEFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MINIGAME_GAMEMODE_2DGAME);
MINIGAMEFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MINIGAME_GAMEMODE_3DGameKeepCamera);
MINIGAMEFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MINIGAME_GAMEMODE_3DGAME);

/**
 * 
 */
UCLASS()
class MINIGAMEFRAMEWORK_API AMiniGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMiniGamePlayerController(const FObjectInitializer& ObjectInitializer);

protected:

	/** BeginPlay initialization */
	virtual void BeginPlay() override;
	
protected:

	/** Default Input Mapping Contexts to add to the player on initialization */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TArray< TObjectPtr<UInputMappingContext> > DefaultMappingContexts;

	/** Input Mapping Contexts to add to the player when menus are open */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TArray< TObjectPtr<UInputMappingContext> > MenuMappingContexts;

	/** If true, menu input mappings are active */
	bool bMenuMappingsActive = false;


	///////////////////////////////
	/// Camera Mode
protected:
	/** the tag for distinction that 2DGame and 3DGame */
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag CameraModeTag;

public:
	UFUNCTION(BlueprintCallable)
	void SetCameraModeTag(FGameplayTag NewCamMode) { CameraModeTag = NewCamMode; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ChangeCameraMode();

	virtual void OnPossess(APawn* InPawn) override;

	
};
