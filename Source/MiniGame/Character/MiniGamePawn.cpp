// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGamePawn.h"

#include "AbilitySystemComponent.h"
#include "MiniGameAbilitySystemComponent.h"
#include "MiniGamePlayerController.h"
#include "MoverComponent.h"
#include "MiniGameAbilities/Public/MiniGameAbilitySet.h"
#include "Components/CapsuleComponent.h"

// Called when the game starts or when spawned
void AMiniGamePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

AMiniGamePawn::AMiniGamePawn(const FObjectInitializer& ObjectInitializer)
{
		PrimaryActorTick.bCanEverTick = true;

	//WindVelocity = FVector::ZeroVector;

	// create the collision capsule
	PlayerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Player Capsule"));

	check(PlayerCapsule);

	SetRootComponent(PlayerCapsule);
	PlayerCapsule->InitCapsuleSize(34.0f, 88.0f);
	PlayerCapsule->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	PlayerCapsule->CanCharacterStepUpOn = ECB_No;
	PlayerCapsule->SetShouldUpdatePhysicsVolume(true);
	PlayerCapsule->SetCanEverAffectNavigation(false);
	PlayerCapsule->bDynamicObstacle = true;

	// create the torso skeletal mesh
	TorsoMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Torso Mesh"));

	check(TorsoMesh);

	TorsoMesh->SetupAttachment(PlayerCapsule);
	TorsoMesh->AlwaysLoadOnClient = true;
	TorsoMesh->AlwaysLoadOnServer = true;
	TorsoMesh->bOwnerNoSee = false;
	TorsoMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	TorsoMesh->bCastDynamicShadow = true;
	TorsoMesh->bAffectDynamicIndirectLighting = true;
	TorsoMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;

	static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
	TorsoMesh->SetCollisionProfileName(MeshCollisionProfileName);
	TorsoMesh->SetGenerateOverlapEvents(false);
	TorsoMesh->SetCanEverAffectNavigation(false);

	// create the head skinned mesh
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head Mesh"));

	check(HeadMesh);

	HeadMesh->SetupAttachment(TorsoMesh);
	HeadMesh->AlwaysLoadOnClient = true;
	HeadMesh->AlwaysLoadOnServer = true;
	HeadMesh->bOwnerNoSee = false;
	HeadMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	HeadMesh->bCastDynamicShadow = true;
	HeadMesh->bAffectDynamicIndirectLighting = true;
	HeadMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;

	HeadMesh->SetCollisionProfileName(MeshCollisionProfileName);
	HeadMesh->SetGenerateOverlapEvents(false);
	HeadMesh->SetCanEverAffectNavigation(false);

	// create the headwear mesh
	HeadwearMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Headwear Mesh"));

	check(HeadwearMesh);

 	HeadwearMesh->SetupAttachment(TorsoMesh);
	HeadwearMesh->AlwaysLoadOnClient = true;
	HeadwearMesh->AlwaysLoadOnServer = true;
	HeadwearMesh->bOwnerNoSee = false;
	HeadwearMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	HeadwearMesh->bCastDynamicShadow = true;
	HeadwearMesh->bAffectDynamicIndirectLighting = true;
	HeadwearMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;

	HeadwearMesh->SetCollisionProfileName(MeshCollisionProfileName);
	HeadwearMesh->SetGenerateOverlapEvents(false);
	HeadwearMesh->SetCanEverAffectNavigation(false);
	

	// create the lower body mesh
	LegsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs Mesh"));

	check(LegsMesh);

	LegsMesh->SetupAttachment(TorsoMesh);
	LegsMesh->AlwaysLoadOnClient = true;
	LegsMesh->AlwaysLoadOnServer = true;
	LegsMesh->bOwnerNoSee = false;
	LegsMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	LegsMesh->bCastDynamicShadow = true;
	LegsMesh->bAffectDynamicIndirectLighting = true;
	LegsMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;

	LegsMesh->SetCollisionProfileName(MeshCollisionProfileName);
	LegsMesh->SetGenerateOverlapEvents(false);
	LegsMesh->SetCanEverAffectNavigation(false);
	
	// create the glider skeletal mesh
	GliderMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Glider Mesh"));

	check(GliderMesh);

	GliderMesh->SetupAttachment(PlayerCapsule);
	GliderMesh->AlwaysLoadOnClient = true;
	GliderMesh->AlwaysLoadOnClient = true;
	GliderMesh->AlwaysLoadOnServer = true;
	GliderMesh->bOwnerNoSee = false;
	GliderMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	GliderMesh->bCastDynamicShadow = true;
	GliderMesh->bAffectDynamicIndirectLighting = true;
	GliderMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;

	/*// create the camera
	Camera = CreateDefaultSubobject<UMiniGameCameraComponent>(TEXT("Camera"));

	check(Camera);

	Camera->SetupAttachment(PlayerCapsule);*/

	// create the Mover component
	CharacterMotionComponent = CreateDefaultSubobject<UMoverComponent>(TEXT("MoverComponent"));
	ensure(CharacterMotionComponent);

	// create the ASC
	AbilitySystem = CreateDefaultSubobject<UMiniGameAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	check(AbilitySystem);

	/*// create the water detection comp
	WaterDetection = CreateDefaultSubobject<UMiniGameWaterDetectionComponent>(TEXT("Water Detection"));
	check(WaterDetection);

	// add water detection as a tick prerequisite
	AddTickPrerequisiteComponent(WaterDetection);*/

	// disable Actor-level movement replication, since our Mover component will handle it
	SetReplicatingMovement(false);
}

// Called every frame
void AMiniGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMiniGamePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMiniGamePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PC = Cast<AMiniGamePlayerController>(NewController);

	if (ensure(PC))
	{
		// initialize the ability system
		if (ensure(AbilitySystem))
		{
			AbilitySystem->InitAbilityActorInfo(this, this);

			// grant the ability set but only once
			if (ensure(AbilitySet) && !bInitializedAbilities)
			{
				bInitializedAbilities = true;

				AbilitySet->GiveToAbilitySystem(AbilitySystem, nullptr);

				// call BP initialization handle
				OnPawnInitialized(PC);
			}
			
		}

		// initialize the camera pitch limits
		//Camera->InitializeCameraForPlayer();
	}
	
}

