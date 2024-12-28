// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameInteractionActor_GravityAndCameraMode.h"

#include "MiniGamePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MiniGame/Logging/MiniGameLogChannels.h"


// Sets default values
AMiniGameInteractionActor_GravityAndCameraMode::AMiniGameInteractionActor_GravityAndCameraMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMiniGameInteractionActor_GravityAndCameraMode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMiniGameInteractionActor_GravityAndCameraMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMiniGameInteractionActor_GravityAndCameraMode::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	check(OtherActor);
	
	if (false == OtherActor->IsA(OverlayTargetClass))
	{
		return;
	}

	// change camera mode
	ChangeCameraModeByPawn(OtherActor);

	// change gravity direction
	ChangeGravityDirectionByActor(OtherActor);
	

	Super::OnBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AMiniGameInteractionActor_GravityAndCameraMode::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                                                AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnBoxEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AMiniGameInteractionActor_GravityAndCameraMode::ChangeCameraModeByPawn(AActor* PawnActor) const
{
	if (NewCameraMode.IsValid())
	{
		APawn* OverlayPawn = Cast<APawn>(PawnActor);
		if (nullptr == OverlayPawn)
		{
			return;
		}

		AMiniGamePlayerController* MiniGamePlayerController = Cast<AMiniGamePlayerController>(OverlayPawn->GetController());
		if (false == ensure(MiniGamePlayerController))
		{
			UE_LOG(LogMiniInteraction, Warning, TEXT("[%s] PlayerController is not valid!"), *GetNameSafe(OverlayPawn));
			return;
		}

		MiniGamePlayerController->SetCameraModeTag(NewCameraMode);
		MiniGamePlayerController->ChangeCameraMode();
	}
}

void AMiniGameInteractionActor_GravityAndCameraMode::ChangeGravityDirectionByActor(const AActor* Actor) const
{
	UCharacterMovementComponent* TargetCharacterMovementComponent = Cast<UCharacterMovementComponent>(Actor->FindComponentByClass(UCharacterMovementComponent::StaticClass()));
	if (nullptr == TargetCharacterMovementComponent)
	{
		return;
	}
	
	// add force to the actor use computed gravity
	TargetCharacterMovementComponent->SetGravityDirection(TargetCharacterMovementComponent->GetGravityDirection() == NewGravityVector ? FVector(0, 0, -1) : NewGravityVector);
}


