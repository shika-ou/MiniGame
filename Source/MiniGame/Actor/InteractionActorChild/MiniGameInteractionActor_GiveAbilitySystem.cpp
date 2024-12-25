// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameInteractionActor_GiveAbilitySystem.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "MiniGameAbilitySet.h"
#include "MiniGameAbilitySystemComponent.h"
#include "MiniGame/Character/MiniGamePlayerCharacter.h"


// Sets default values
AMiniGameInteractionActor_GiveAbilitySystem::AMiniGameInteractionActor_GiveAbilitySystem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMiniGameInteractionActor_GiveAbilitySystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMiniGameInteractionActor_GiveAbilitySystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMiniGameInteractionActor_GiveAbilitySystem::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (AMiniGamePlayerCharacter* MiniGamePlayerCharacter = Cast<AMiniGamePlayerCharacter>(OtherActor))
	{
		if (UMiniGameAbilitySystemComponent* TargetAsc = Cast<UMiniGameAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MiniGamePlayerCharacter)))
		{
			AbilitySet->GiveToAbilitySystem(TargetAsc, nullptr);
		}
	}
}

