// Fill out your copyright notice in the Description page of Project Settings.
#include "Combat/CombatLockComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UCombatLockComponent::UCombatLockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	LockOnDistance = 1000.f;
	bEnableDebug = false;
	CameraInterpSpeed = 8.0f;
	CameraPitch = -20.0f;
	bIsLockOn = false;
}


// Called when the game starts
void UCombatLockComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCombatLockComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UpdateCameraRotation(DeltaTime);
}

void UCombatLockComponent::SearchAndLockTarget()
{
	UWorld* World = GetWorld();
	if (ensure(World) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("The world was null!"));
		return;
	}

	FVector StartLocation = GetOwner()->GetActorLocation();
	TArray<FHitResult> HitResults;

	// scan the enemy in forward
	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		World,
		StartLocation,
		StartLocation + FVector::UpVector * 10.f, // a little over
		LockOnDistance,
		TArray<TEnumAsByte<EObjectTypeQuery>>{UEngineTypes::ConvertToObjectType(ECC_Pawn)},
		false,
		TArray<AActor*>{GetOwner()},
		bEnableDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		HitResults,
		true
	);

	if (bHit)
	{
		AActor* ClosestTarget = nullptr;
		float MinDistance = LockOnDistance;

		for (FHitResult HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor == nullptr || HitActor == GetOwner())
			{
				continue;
			}

			float Dist = FVector::Distance(StartLocation, HitActor->GetActorLocation());
			if (Dist < MinDistance)
			{
				ClosestTarget = HitActor;
				MinDistance = Dist;
			}
		}

		if (ClosestTarget)
		{
			LockedTarget = ClosestTarget;
			bIsLockOn = true;
		}
	}
}

void UCombatLockComponent::UnlockTarget()
{
	bIsLockOn = false;
	LockedTarget = nullptr;
}

void UCombatLockComponent::UpdateCameraRotation(float DeltaTime)
{
	if (bIsLockOn == false || IsValid(LockedTarget) == false)
	{
		return;
	}

	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (PawnOwner == nullptr)
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(PawnOwner->GetController());
	if (PlayerController == nullptr)
	{
		return;
	}
	
	// turn character face to the target
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
		GetOwner()->GetActorLocation(),
		LockedTarget->GetActorLocation()
	);

	FRotator NewRotator = FRotator(0, LookAtRotation.Yaw, 0);
	GetOwner()->SetActorRotation(NewRotator);

	// update the rotation of camera
	FVector DirToTarget = LockedTarget->GetActorLocation() - PawnOwner->GetActorLocation();
	FRotator DesiredRotator = DirToTarget.Rotation();

	// only use the yaw to keep pitch
	FRotator CurrentRotator = PlayerController->GetControlRotation();
	FRotator NewControllerRotation = FMath::RInterpTo(CurrentRotator, FRotator(CameraPitch, DesiredRotator.Yaw, 0.f), DeltaTime, 8.f);
	PlayerController->SetControlRotation(NewControllerRotation);
}
