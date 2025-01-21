// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameActor_Smears.h"

#include "Components/PoseableMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "MiniGame/Character/MiniGamePlayerCharacter.h"
#include "MiniGame/Logging/MiniGameLogChannels.h"

AMiniGameActor_Smears::AMiniGameActor_Smears()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SetRootComponent(CustomRootComponent);
	
	PoseableMeshComponent = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	check(PoseableMeshComponent);

	PoseableMeshComponent->SetupAttachment(CustomRootComponent);

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	check(TimelineComponent);
}

// Called when the game starts or when spawned
void AMiniGameActor_Smears::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat UpdateFunction;
	UpdateFunction.BindUFunction(this, FName("OnTimelineUpdate"));

	FOnTimelineEvent FinishFunction;
	FinishFunction.BindUFunction(this, FName("OnTimelineFinish"));
	
	// set bind function
	TimelineComponent->AddInterpFloat(AlphaCurve, UpdateFunction);
	TimelineComponent->SetTimelineFinishedFunc(FinishFunction);

	// set timeline length
	TimelineComponent->SetTimelineLength(TimeLineLength);
	
	// set timeline play mod
	TimelineComponent->SetLooping(false);
	TimelineComponent->SetPlayRate(1.f);
}

// Called every frame
void AMiniGameActor_Smears::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMiniGameActor_Smears::OnTimelineUpdate(float NewFloat)
{
	SetPoseableMeshMaterial(TEXT("Alpha"), NewFloat);
}

void AMiniGameActor_Smears::OnTimelineFinish()
{
	Destroy();
}

void AMiniGameActor_Smears::ChangeEffectAndDestroy(AMiniGamePlayerCharacter* TargetPlayerCharacter)
{
	PlayerCharacter = TargetPlayerCharacter;
	check(PlayerCharacter);
	
	if (nullptr == PlayerCharacter)
	{
		UE_LOG(LogMiniGameActor, Error, TEXT("MiniGameActor Smears [%s] PlayerCharacter is nullptr"), *GetNameSafe(this));
		return;
	}

	// set new skin asset 
	PoseableMeshComponent->SetSkinnedAssetAndUpdate(PlayerCharacter->GetMesh()->GetSkeletalMeshAsset());
	

	// change mesh material to effect PoseMaterial
	SetPoseableMeshMaterial(TEXT("Random"));

	// copy skeletal pose
	PoseableMeshComponent->CopyPoseFromSkeletalComponent(PlayerCharacter->GetMesh());

	// set player from start
	TimelineComponent->PlayFromStart();
}

void AMiniGameActor_Smears::SetPoseableMeshMaterial(const FString& MaterialParameterName, float ParameterFloat /*= 0.0f*/)
{
	const TArray<UMaterialInterface*>& Materials = PoseableMeshComponent->GetMaterials();

	float RandomFloat = FMath::FRandRange(0.0f, 1.0f);
	
	for (int32 i = 0; i < Materials.Num(); ++i)
	{
		if (nullptr == Materials[i])
		{
			continue;
		}

		PoseableMeshComponent->SetMaterial(i, PoseMaterial);
		
		if (MaterialParameterName == TEXT("Random"))
		{
			PoseableMeshComponent->SetScalarParameterValueOnMaterials(FName(TEXT("Random")), RandomFloat);
		}
		else if (MaterialParameterName == TEXT("Alpha"))
		{
			PoseableMeshComponent->SetScalarParameterValueOnMaterials(FName(TEXT("Alpha")), ParameterFloat);
		}
	}
}


