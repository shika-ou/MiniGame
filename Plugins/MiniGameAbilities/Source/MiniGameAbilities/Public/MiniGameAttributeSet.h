// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "MiniGameAttributeSet.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MINIGAMEABILITIES_API UMiniGameAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UMiniGameAttributeSet(const FObjectInitializer& ObjectInitializer);

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "AttributeTest", meta = (HideFromModifiers))			// You can't make a GameplayEffect modify Health Directly (go through)
	mutable float	MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "AttributeTest", meta = (HideFromModifiers))			// You can't make a GameplayEffect modify Health Directly (go through)
	mutable float	Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "AttributeTest")
	mutable FGameplayAttributeData	Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "AttributeTest")
	mutable float	MaxMana;

	/** This Damage is just used for applying negative health mods. Its not a 'persistent' attribute. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeTest")		// You can't make a GameplayEffect 'powered' by Damage (Its transient)
	mutable float	Damage;

	/** This Attribute is the actual spell damage for this actor. It will power spell based GameplayEffects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "AttributeTest")
	mutable float	SpellDamage;

	/** This Attribute is the actual physical damage for this actor. It will power physical based GameplayEffects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "AttributeTest")
	mutable float	PhysicalDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "AttributeTest")
	mutable float	CritChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "AttributeTest")
	mutable float	CritMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "AttributeTest")
	mutable float	ArmorDamageReduction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "AttributeTest")
	mutable float	DodgeChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "AttributeTest")
	mutable float	LifeSteal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "AttributeTest")
	mutable float	Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeTest")
	mutable float	StackingAttribute1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeTest")
	mutable float	StackingAttribute2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeTest")
	mutable float	NoStackAttribute;

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData &Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
};

