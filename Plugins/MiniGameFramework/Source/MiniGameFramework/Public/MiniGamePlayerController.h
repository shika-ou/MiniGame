// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MiniGamePlayerController.generated.h"

class UInputMappingContext;
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
};
