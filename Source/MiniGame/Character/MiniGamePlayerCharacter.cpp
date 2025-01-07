// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGamePlayerCharacter.h"

#include "AIController.h"
#include "MiniGameAbilitySet.h"
#include "MiniGameAbilitySystemComponent.h"
#include "MiniGamePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MiniGame/Logging/MiniGameLogChannels.h"


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

	if (NewController->IsA(AAIController::StaticClass()))
	{
		return;
	}
	
	CreateHUD();

	ChangeCameraModeByCameraTag();
}

void AMiniGamePlayerCharacter::UnPossessed()
{
	Super::UnPossessed();
	
	ResetHUD();
}

void AMiniGamePlayerCharacter::ChangeSkinBySkinIndex_Implementation(int32 SkinIndex)
{
}

void AMiniGamePlayerCharacter::ChangeToSkin(int32 SkinIndex)
{
	// get attached mesh compoent
	USkeletalMeshComponent* MeshComponent = Cast<USkeletalMeshComponent>(GetMesh()->GetChildComponent(0));
	if (nullptr == MeshComponent)
	{
		MeshComponent = Cast<USkeletalMeshComponent>(GetMesh());
		if (nullptr == MeshComponent)
		{
			UE_LOG(LogMiniGameCharacter, Warning, TEXT("[%s] Skeletal and chlid USkeletalMeshComponent is not setting! "), *GetNameSafe(this));
			return;
		}
	}
	
	switch (SkinMode)
	{
	case ESkinMode::Material:
		if (false == ensure(MaterialArray.IsValidIndex(SkinIndex)))
		{
			UE_LOG(LogMiniGameCharacter, Warning, TEXT("SkinIndex [%d] is overflod in MaterialArray at [%s]"), SkinIndex, *GetNameSafe(this));
			return;
		}

		MeshComponent->SetMaterial(0, MaterialArray[SkinIndex]);
		break;
		
	case ESkinMode::Mesh:
		if (false == ensure(SkeletalMeshArray.IsValidIndex(SkinIndex)))
		{
			UE_LOG(LogMiniGameCharacter, Warning, TEXT("SkinIndex [%d] is overflod in SkeletalMeshArray at [%s]"), SkinIndex, *GetNameSafe(this));
			return;
		}
		
		MeshComponent->SetSkeletalMesh(SkeletalMeshArray[SkinIndex]);
		break;
	}
}

void AMiniGamePlayerCharacter::ChangeGravityDirectionByNewDirection(FVector NewDirection)
{
	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (false == ensure(CharacterMovementComponent))
	{
		UE_LOG(LogMiniGameCharacter, Warning, TEXT("Character [%s] Movement Component is not valid!"), *GetNameSafe(this));
	}

	CharacterMovementComponent->SetGravityDirection(NewDirection);
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
	if (HUD)
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

void AMiniGamePlayerCharacter::ChangeCameraModeByCameraTag() const
{
	AMiniGamePlayerController* MiniGamePlayerController = Cast<AMiniGamePlayerController>(GetController());
	if (false == ensure(MiniGamePlayerController))
	{
		UE_LOG(LogMiniGameCharacter, Warning, TEXT("[%s] PlayerController is not valid!"), *GetNameSafe(this));
		return;
	}

	MiniGamePlayerController->SetCameraModeTag(CameraModeTag);
}

