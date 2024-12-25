// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniGame_ActionWidget.generated.h"

class UCommonTextBlock;
class UCommonActionWidget;
class UInputAction;
/**
 * 
 */
UCLASS()
class MINIGAME_API UMiniGame_ActionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	////////////////////////////////
	/// config message
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MiniGame|ActionWidget")
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MiniGame|ActionWidget")
	FText ActionText;

	//////////////////////////////
	/// edit target
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCommonActionWidget> ActionWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> ActionLabel;

public:
	virtual void NativePreConstruct() override;
};
