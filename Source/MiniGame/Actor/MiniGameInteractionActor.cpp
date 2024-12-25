// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameInteractionActor.h"

#include "Components/BoxComponent.h"
#include "Curves/CurveVector.h"


AMiniGameInteractionActor::AMiniGameInteractionActor(const FObjectInitializer& ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	check(StaticMeshComponent);
	
	// set static mesh component as root component
	StaticMeshComponent->SetupAttachment(RootComponent);

	//
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	check(BoxComponent);

	// attach to root component
	BoxComponent->SetupAttachment(StaticMeshComponent);

	// initialize the value
	bLocationMovementIsOvered = false;
	bRotationMovementIsOvered = false;
	bScaleMovementIsOvered = false;

	CurrentActorMovementPlayTime_Location = 0.f;
	CurrentActorMovementPlayTime_Rotation = 0.f;
	CurrentActorMovementPlayTime_Scale = 0.f;
}

// Called when the game starts or when spawned
void AMiniGameInteractionActor::BeginPlay()
{
	Super::BeginPlay();

	// set timer for end movement
	//SetTimerForMovementOver();
	

	MeshOriginalTransform = StaticMeshComponent->GetComponentTransform();
	
	// bind box event
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMiniGameInteractionActor::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AMiniGameInteractionActor::OnBoxEndOverlap);
	
}

// Called every frame
void AMiniGameInteractionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	UpdateMovementCurve(EMovementType::Location, DeltaTime, LocationMovementStruct, CurrentActorMovementPlayTime_Location, bLocationMovementIsOvered);
	UpdateMovementCurve(EMovementType::Rotation, DeltaTime, RotationMovementStruct, CurrentActorMovementPlayTime_Rotation, bRotationMovementIsOvered);
	UpdateMovementCurve(EMovementType::Scale, DeltaTime, ScaleMovementStruct, CurrentActorMovementPlayTime_Scale, bScaleMovementIsOvered);
}

void AMiniGameInteractionActor::UpdateMovementCurve(const EMovementType MovementType, const float& DeltaTime, const FActorMovementStruct& MovementStruct, float& CurrentPlayTime, bool& bMovementIsOvered) const
{
	// if movement already over, return
	if (bMovementIsOvered)
	{
		return;
	}

	// check curve is valid
	if (false == ensure(MovementStruct.Curve))
	{
		//static const UEnum* EnumPtr = StaticEnum<EMovementType>();
		//UE_LOG(LogMiniGameCharacter, Warning, TEXT("MovementStruct %s Curve is not valid!"), EnumPtr ? *(EnumPtr->GetNameByValue(MovementType).ToString()) : *(FString::FromInt((int8)MovementType)));
		return;
	}

	// update current play time
	CurrentPlayTime += DeltaTime;
	
	const FVector& ValueVector = MovementStruct.Curve->GetVectorValue(CurrentPlayTime);

	switch (MovementType)
	{
		case EMovementType::Location:
			StaticMeshComponent->SetRelativeLocation(ValueVector + MeshOriginalTransform.GetLocation());
			break;

		case EMovementType::Rotation:
			StaticMeshComponent->SetRelativeRotation(FRotator(ValueVector.X, ValueVector.Y, ValueVector.Z));
			break;

		case EMovementType::Scale:
			StaticMeshComponent->SetRelativeScale3D(ValueVector * MeshOriginalTransform.GetScale3D());
			break;
	}

	// if current play time over duration
	if (CurrentPlayTime >= MovementStruct.MovementDuration)
	{
		// if loop, reset it
		if (MovementStruct.bLoopPlay)
		{
			CurrentPlayTime = 0.f;
		}
		// else set it over
		else
		{
			bMovementIsOvered = true;
		}
	}
}

void AMiniGameInteractionActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// only apply for target class	
	if (false == OtherActor->IsA(OverlayTargetClass))
	{
		return;
	}
}

void AMiniGameInteractionActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

