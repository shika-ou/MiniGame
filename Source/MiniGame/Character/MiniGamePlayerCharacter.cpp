// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGamePlayerCharacter.h"

#include "MiniGameAbilitySet.h"
#include "MiniGameAbilitySystemComponent.h"
#include "MoverComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"

// define gameMode tag
UE_DEFINE_GAMEPLAY_TAG(TAG_MINIGAME_GAMEMODE_2DGAME, "MiniGame.GameMode.2DGame");
UE_DEFINE_GAMEPLAY_TAG(TAG_MINIGAME_GAMEMODE_3DGAME, "MiniGame.GameMode.3DGame");


AMiniGamePlayerCharacter::AMiniGamePlayerCharacter(const FObjectInitializer& ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bInitializedAbilities = false;

	// create ASC
	AbilitySystemComponent = CreateDefaultSubobject<UMiniGameAbilitySystemComponent>(TEXT("ASC"));
	check(AbilitySystemComponent)
}

// Called when the game starts or when spawned
void AMiniGamePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// initialize the ability system
	InitializeAbilitySystem();
}

// Called every frame
void AMiniGamePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AMiniGamePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMiniGamePlayerCharacter::InitializeAbilitySystem()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		// grant the ability set but only once
		if (AbilitySet && !bInitializedAbilities)
		{
			bInitializedAbilities = true;

			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}

void AMiniGamePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CreateHUD();
}

void AMiniGamePlayerCharacter::UnPossessed()
{
	Super::UnPossessed();

	ResetHUD();
}

void AMiniGamePlayerCharacter::CreateHUD()
{
	HUD = CreateWidget(GetWorld(), HUDClass);
	if (ensure(HUD))
	{
		HUD->AddToViewport();
	}
}

void AMiniGamePlayerCharacter::ResetHUD()
{
	if (ensure(HUD))
	{
		HUD->RemoveFromParent();

		HUD = nullptr;
	}
}

void AMiniGamePlayerCharacter::SetStrafeStateByWantsToStrafe()
{
	UCharacterMovementComponent* MoverComponent = GetCharacterMovement();
	check(MoverComponent);

	// bUseControllerDesiredRotation means camera should stay behind the character
	// bOrientRotationToMovement is the inverse
	MoverComponent->bUseControllerDesiredRotation = bWantsToStrafe;
	MoverComponent->bOrientRotationToMovement = !bWantsToStrafe;
}

void AMiniGamePlayerCharacter::SetRotationRateByFallingState()
{
	UCharacterMovementComponent* MoverComponent = GetCharacterMovement();
	check(MoverComponent);
	
	MoverComponent->RotationRate = MoverComponent->IsFalling() ? FallingRotationRate : NoFallingRotationRate;
}

