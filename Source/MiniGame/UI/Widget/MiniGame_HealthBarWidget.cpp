// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGame_HealthBarWidget.h"

#include "AbilitySystemGlobals.h"
#include "MiniGameAbilitiesLogging.h"
#include "MiniGameAttributeSet.h"
#include "Components/ProgressBar.h"
#include "MiniGame/Character/MiniGamePlayerCharacter.h"


UMiniGame_HealthBarWidget::UMiniGame_HealthBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	/*CurrentAttribute = UMiniGameAttributeSet::GetHealthAttribute();
	MaximumValueAttribute = UMiniGameAttributeSet::GetMaxHealthAttribute();*/
}

void UMiniGame_HealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindToAbilityComponent();
}

void UMiniGame_HealthBarWidget::BindToAbilityComponent()
{
	CachedAbilityComponent = GetAbilitySystemComponent();
	if (false == ensure(CachedAbilityComponent))
	{
		UE_LOG(LogMiniGameAbilitySystem, Warning, TEXT("UMiniGame_HealthBarWidget [%s] CacheAbilityComponent is not valid!"), *GetNameSafe(this));
		return;
	}

	// initialization of the value
	CurrentValue = CachedAbilityComponent->GetNumericAttribute(CurrentAttribute);
	MaximumValue = CachedAbilityComponent->GetNumericAttribute(MaximumValueAttribute);

	UpdateStatus();

	CurrentValueDelegate = CachedAbilityComponent->GetGameplayAttributeValueChangeDelegate(CurrentAttribute).AddUObject(this, &ThisClass::HandleCurrentAttributeChanged);
	MaximumValueDelegate = CachedAbilityComponent->GetGameplayAttributeValueChangeDelegate(MaximumValueAttribute).AddUObject(this, &ThisClass::HandleMaximumAttributeChanged);
	
	
}

UAbilitySystemComponent* UMiniGame_HealthBarWidget::GetAbilitySystemComponent()
{
	return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPlayerCharacter());
}

void UMiniGame_HealthBarWidget::HandleCurrentAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (IsValid(CachedAbilityComponent))
	{
		CurrentValue = OnAttributeChangeData.NewValue;
		UpdateStatus();
	}
}

void UMiniGame_HealthBarWidget::HandleMaximumAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (IsValid(CachedAbilityComponent))
	{
		MaximumValue = OnAttributeChangeData.NewValue;
		UpdateStatus();
	}
}

void UMiniGame_HealthBarWidget::SetMiniGameCharacter_Implementation(AActor* NewMiniGameCharacter)
{
	if (NewMiniGameCharacter && NewMiniGameCharacter != MiniGameCharacter && NewMiniGameCharacter->IsA(AMiniGamePlayerCharacter::StaticClass()))
	{
		MiniGameCharacter = NewMiniGameCharacter;

		BindToAbilityComponent();
	}
}

AActor* UMiniGame_HealthBarWidget::GetPlayerCharacter_Implementation()
{
	return IsValid(MiniGameCharacter) ? MiniGameCharacter : GetOwningPlayerPawn();
}

void UMiniGame_HealthBarWidget::UpdateStatus_Implementation()
{
	if (IsValid(OverheadProgressBar))
	{
		OverheadProgressBar->SetPercent(CurrentValue / MaximumValue);
	}
}
