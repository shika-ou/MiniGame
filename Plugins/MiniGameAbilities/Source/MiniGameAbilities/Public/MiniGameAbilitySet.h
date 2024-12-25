// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MiniGameAbilitySet.generated.h"

class UGameplayAbility;
class UMiniGameGameplayEffect;
class UAttributeSet;
struct FActiveGameplayEffectHandle;
struct FGameplayAbilitySpecHandle;
class UMiniGameAbilitySystemComponent;

/**
 * FMiniGameAbilitySet_GameplayAbility
 * Data used by the ability set to grant a gameplay ability.
 */
USTRUCT(BlueprintType)
struct MINIGAMEABILITIES_API FMiniGameAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:

	// Class of the gameplay ability to grant
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability = nullptr;

	// Level of the ability to grant
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// If true, the ability will be activated as soon as it's granted
	UPROPERTY(EditDefaultsOnly)
	bool bActivateImmediately = false;
};

/**
 * FMiniGameAbilitySet_GameplayEffect
 * Data used by the ability set to grant a gameplay effect.
 */
USTRUCT(BlueprintType)
struct MINIGAMEABILITIES_API FMiniGameAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:

	// Class of the gameplay effect to grant
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMiniGameGameplayEffect> GameplayEffect = nullptr;

	// Level of the gameplay effect to grant
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

/**
 * FMiniGameAbilitySet_AttributeSet
 * Data used by the ability set to grant an attribute set.
 */
USTRUCT(BlueprintType)
struct MINIGAMEABILITIES_API FMiniGameAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:

	// Class of the gameplay effect to grant
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet = nullptr;

	// Level of the gameplay effect to grant
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

/**
 * FMiniGameAbilitySet_GrantedHandles
 * Data used to store ASC handles granted by the ability set.
 */
USTRUCT(BlueprintType)
struct MINIGAMEABILITIES_API FMiniGameAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	void TakeFromAbilitySystem(UMiniGameAbilitySystemComponent* ASC);

protected:

	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	// Pointers to the granted attribute sets
	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
	
};

/**
 *  UMiniGameAbilitySet
 *  
 *  Non-mutable data asset used to grant gameplay abilities, effects and attributes to an ASC
 */
UCLASS(BlueprintType, Const)
class MINIGAMEABILITIES_API UMiniGameAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UMiniGameAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	 *  Grants the ability set to the specified ability system component.
	 *  The returned handles can be used later to take away anything that was granted.
	 */
	void GiveToAbilitySystem(UMiniGameAbilitySystemComponent* ASC, FMiniGameAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:

	// Gameplay Abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<FMiniGameAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// Gameplay Effects to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Abilities", meta=(TitleProperty=GameplayEffect))
	TArray<FMiniGameAbilitySet_GameplayEffect> GrantedGameplayEffects;

	// Attribute Sets to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Abilities", meta=(TitleProperty=AttributeSet))
	TArray<FMiniGameAbilitySet_AttributeSet> GrantedAttributeSets;
};
