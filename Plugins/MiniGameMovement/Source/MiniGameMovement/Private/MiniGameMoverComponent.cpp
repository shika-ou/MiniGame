// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameMoverComponent.h"

#include "MiniGameMoverTypes.h"
#include "MoveLibrary/FloorQueryUtils.h"


// Sets default values for this component's properties
UMiniGameMoverComponent::UMiniGameMoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMiniGameMoverComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

FGameplayTagContainer UMiniGameMoverComponent::GetTagsFromSyncState() const
{
	if (!bHasValidCachedState)
	{
		return FGameplayTagContainer();
	}

	// get the tags sync state
	const FMiniGameTagsSyncState* TagsSyncState = CachedLastSyncState.SyncStateCollection.FindDataByType<FMiniGameTagsSyncState>();

	// return the tags
	if (TagsSyncState)
	{
		return TagsSyncState->GetMovementTags();
	}

	return FGameplayTagContainer();
}

FVector UMiniGameMoverComponent::GetGroundNormal() const
{
	FFloorCheckResult CurrentFloor;
	const UMoverBlackboard* SimBB = GetSimBlackboard();

	if (SimBB && SimBB->TryGet(CommonBlackboard::LastFloorResult, CurrentFloor))
	{
		return CurrentFloor.HitResult.ImpactNormal;
	}

	return  FVector::ZeroVector;
}

/*FTransform UMiniGameMoverComponent::GetRaftLeftHandTransform_Implementation() const
{
	if (CurrentRaft)
	{
		
	}
}

FTransform UMiniGameMoverComponent::GetRaftRightHandTransform_Implementation() const
{
}

FTransform UMiniGameMoverComponent::GetRaftLeftFootTransform_Implementation() const
{
}

FTransform UMiniGameMoverComponent::GetRaftRightFootTransform_Implementation() const
{
}

FTransform UMiniGameMoverComponent::GetRaftPelvisTransform_Implementation() const
{
}*/