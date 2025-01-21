// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "MiniGameWidgetInterface.h"
#include "Blueprint/UserWidget.h"
#include "MiniGame_HealthBarWidget.generated.h"

class AMiniGamePlayerCharacter;
struct FOnAttributeChangeData;
class UProgressBar;
/**
 * 
 */
UCLASS()
class MINIGAME_API UMiniGame_HealthBarWidget : public UUserWidget, public IMiniGameWidgetInterface
{
	GENERATED_BODY()

	UMiniGame_HealthBarWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:

	// -- Begin Widget implementation
	virtual void NativeConstruct() override;
	
protected:
	/** Progress Bar used to represent the status. */
	UPROPERTY(BlueprintReadWrite, Category = "MiniGame|Progress", meta = (BindWidget))
	TObjectPtr<UProgressBar> OverheadProgressBar;
	
	/** Attribute that backs the Progress Bar's current value. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MiniGame|Progress")
	FGameplayAttribute CurrentAttribute;

	/** Attribute that backs the Progress Bar's maximum value. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame|Progress")
	FGameplayAttribute MaximumValueAttribute;

	/** Connect to the owner's ASC*/
	UFUNCTION()
	virtual void BindToAbilityComponent();

	UFUNCTION(BlueprintPure, Category = "MiniGame|Widget")
	UAbilitySystemComponent* GetAbilitySystemComponent();

	/** Tracks changes in the current attribute. */
	void HandleCurrentAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData);

	/** Tracks changes in the maximum attribute. */
	void HandleMaximumAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData);

	/** Update State Value */
	UFUNCTION(BlueprintNativeEvent, Category = "MiniGame|Widget")
	void UpdateStatus();

public:
	UFUNCTION(BlueprintNativeEvent, Category = "MiniGame|Widget")
	AActor* GetPlayerCharacter();

	virtual void SetMiniGameCharacter_Implementation(AActor* NewMiniGameCharacter) override;
	
private:
	float CurrentValue;
	float MaximumValue;
	/*float MaximumAddValue;
	float MaximumPercentValue;
	float MaximumTotalValue;*/

	FDelegateHandle CurrentValueDelegate;
	FDelegateHandle MaximumValueDelegate;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> CachedAbilityComponent;

	UPROPERTY()
	TObjectPtr<AActor> MiniGameCharacter;	
};
