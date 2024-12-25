// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameAbilitySet.h"

#include "MiniGameAbilitiesLogging.h"
#include "MiniGameAbilitySystemComponent.h"
#include "MiniGameGameplayAbility.h"
#include "MiniGameGameplayEffect.h"

void FMiniGameAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FMiniGameAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FMiniGameAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	if (IsValid(Set))
	{
		GrantedAttributeSets.Add(Set);
	}
}

void FMiniGameAbilitySet_GrantedHandles::TakeFromAbilitySystem(UMiniGameAbilitySystemComponent* ASC)
{
	check(ASC);

	// don't grant or remove ability sets unless the actor has authority
	if (false == ASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	// remove all abilities from the ASC
	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			ASC->ClearAbility(Handle);
		}
	}

	// remove all gameplay effects from the ASC
	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	// remove all attribute sets from the ASC
	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		ASC->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

UMiniGameAbilitySet::UMiniGameAbilitySet(const FObjectInitializer& ObjectInitializer)
{
}

void UMiniGameAbilitySet::GiveToAbilitySystem(UMiniGameAbilitySystemComponent* ASC,
                                              FMiniGameAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(ASC);

	// don't grant or remove ability sets unless the actor has authority
	if (false == ASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	// grant the attribute sets
	for (int32 SetIndex = 0; SetIndex < GrantedAttributeSets.Num(); ++SetIndex)
	{
		const FMiniGameAbilitySet_AttributeSet& SetToGrant = GrantedAttributeSets[SetIndex];

		// check the ability set is vaild
		if (false == ensure(IsValid(SetToGrant.AttributeSet)))
		{
			UE_LOG(LogMiniGameAbilitySystem, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not vaild"), SetIndex, *GetNameSafe(this));
			continue;
		}

		// create attribute set and give it to the ASC
		UAttributeSet* NewSet = NewObject<UAttributeSet>(ASC->GetOwner(), SetToGrant.AttributeSet);
		ensure(NewSet);

		ASC->AddAttributeSetSubobject(NewSet);

		// save the handle
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}

	// grant the gameplay effects
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FMiniGameAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		// check the gameplay effect is vaild
		if (false == ensure(IsValid(EffectToGrant.GameplayEffect)))
		{
			UE_LOG(LogMiniGameAbilitySystem, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not vaild"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		// get the CDO and grant the GE
		const UMiniGameGameplayEffect* GameplayEffectCDO = EffectToGrant.GameplayEffect->GetDefaultObject<UMiniGameGameplayEffect>();
		ensure(GameplayEffectCDO);

		const FActiveGameplayEffectHandle GameplayEffectHandle = ASC->ApplyGameplayEffectToSelf(GameplayEffectCDO, EffectToGrant.EffectLevel, ASC->MakeEffectContext());

		// save the handle
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	// grant the gameplay abilities
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FMiniGameAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		// check the gameplay ability is vaild
		if (false == ensure(IsValid(AbilityToGrant.Ability)))
		{
			UE_LOG(LogMiniGameAbilitySystem, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not vaild"), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		// get the CDO and build the ability spec
		UGameplayAbility* GameplayAbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UGameplayAbility>();
		ensure(GameplayAbilityCDO);

		FGameplayAbilitySpec AbilitySpec(GameplayAbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;

		const FGameplayAbilitySpecHandle AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);

		// save the handle
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}

		// check if we should activate the ability right away
		if (AbilityToGrant.bActivateImmediately)
		{
			ASC->TryActivateAbility(AbilitySpecHandle);
		}
	}
	
}
