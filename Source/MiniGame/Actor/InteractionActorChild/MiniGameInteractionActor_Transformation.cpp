// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameInteractionActor_Transformation.h"

#include "MiniGamePlayerController.h"
#include "MiniGame/Character/MiniGamePlayerCharacter.h"
#include "MiniGame/Logging/MiniGameLogChannels.h"


// Sets default values
AMiniGameInteractionActor_Transformation::AMiniGameInteractionActor_Transformation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMiniGameInteractionActor_Transformation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMiniGameInteractionActor_Transformation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMiniGameInteractionActor_Transformation::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                                 AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                                 const FHitResult& SweepResult)
{
	TransformToNewChracter(OtherActor);
	
	//Super::OnBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AMiniGameInteractionActor_Transformation::TransformToNewChracter_Implementation(AActor* OldCharacter)
{
}

void AMiniGameInteractionActor_Transformation::SpawnAndPossessNewCharacter(AActor* OldCharacter)
{
	if (OldCharacter->IsA(TransformCharacterClass))
	{
		return;
	}
	
	APawn* TargetPawn = Cast<APawn>(OldCharacter);
	if (nullptr == TargetPawn)
	{
		return;
	}
	
	AMiniGamePlayerController* MiniGamePlayerController = Cast<AMiniGamePlayerController>(TargetPawn->GetController());
	if (nullptr == MiniGamePlayerController)
	{
		return;
	}

	AMiniGamePlayerCharacter* NewMiniGamePlayerCharacter = GetWorld()->SpawnActor<AMiniGamePlayerCharacter>(TransformCharacterClass, GetActorLocation(), MiniGamePlayerController->GetControlRotation());
	if (nullptr == NewMiniGamePlayerCharacter)
	{
		UE_LOG(LogMiniInteraction, Warning, TEXT("Transformation spawn new actor [%s] was failed!"), *GetNameSafe(this));
		return;
	}

	MiniGamePlayerController->Possess(NewMiniGamePlayerCharacter);

	TargetPawn->Destroy();
}

