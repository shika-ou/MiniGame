// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameInteractionActor_GravityControl.h"

#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AMiniGameInteractionActor_GravityControl::AMiniGameInteractionActor_GravityControl(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMiniGameInteractionActor_GravityControl::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMiniGameInteractionActor_GravityControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetCharacterMovementComponent)
	{
		UpdateGravityDirection();
	}
}

void AMiniGameInteractionActor_GravityControl::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	check(OtherActor);
	
	if (false == OtherActor->IsA(OverlayTargetClass))
	{
		return;
	}

	TargetCharacterMovementComponent = Cast<UCharacterMovementComponent>(OtherActor->FindComponentByClass(UCharacterMovementComponent::StaticClass()));
	if (nullptr == TargetCharacterMovementComponent)
	{
		return;
	}

	SourceGravityVector = TargetCharacterMovementComponent->GetGravityDirection();

	//UPrimitiveComponent* ActorPrimitive = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
	// check(ActorPrimitve);	
	
	//OtherComp->AddForce(GravityForce);
}

void AMiniGameInteractionActor_GravityControl::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                                                AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (nullptr == TargetCharacterMovementComponent)
	{
		return;
	}
	
	TargetCharacterMovementComponent->SetGravityDirection(SourceGravityVector);

	TargetCharacterMovementComponent = nullptr;
}

void AMiniGameInteractionActor_GravityControl::UpdateGravityDirection() const
{
	// compute the vector that actor to center
	FVector GravityDirection = GetActorLocation() - TargetCharacterMovementComponent->GetLocation();
	//  FVector UpwardGravityDirection = FVector(0.0f, 0.0f, 1.0f); 
    
	// normal the gravity
	FVector GravityForce = GravityDirection.GetSafeNormal() * NewGravityStrength;
    
	// add force to the actor use computed gravity
	TargetCharacterMovementComponent->SetGravityDirection(GravityForce);
}


