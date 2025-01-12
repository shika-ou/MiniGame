// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameAnimNotifyState.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MiniGame/Character/MiniGamePlayerCharacter.h"
#include "MiniGame/Logging/MiniGameLogChannels.h"

void UMiniGameAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UMiniGameAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	SweepActorAndApplyGameEffect(MeshComp);
}

void UMiniGameAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AMiniGamePlayerCharacter* MeshMiniPlayerCharacter = Cast<AMiniGamePlayerCharacter>(MeshComp->GetOwner());
	if (nullptr == MeshMiniPlayerCharacter)
	{
		return ;
	}

	MeshMiniPlayerCharacter->GetHitActors().Empty();

	MeshMiniPlayerCharacter->GetLineMultiPoints().Empty();
}

void UMiniGameAnimNotifyState::SweepActorAndApplyGameEffect(USkeletalMeshComponent* MeshComp) const
{
	if (false == ensure(IsValid(GameplayEffect)))
	{
		UE_LOG(LogMiniGameCharacter, Warning, TEXT("Anim Notify [%s] GameplayEffect is not valid!"), *GetNameSafe(this));
		return;
	}

	UGameplayEffect* GameplayEffectCDO = GameplayEffect->GetDefaultObject<UGameplayEffect>();
	ensure(GameplayEffectCDO);
	
	if (false == ensure(MeshComp))
	{
		UE_LOG(LogMiniGameCharacter, Warning, TEXT("Anim Notify [%s] MeshComp is not valid!"), *GetNameSafe(this));
		return;
	}

	// Get HitResult
	TArray<FHitResult> HitResults;
	bool Result = GetHitResults(MeshComp, HitResults);
	if (false == Result)
	{
		return;
	}
	
	if (HitResults.Num() <= 0)
	{
		return;
	}

	// get character hit actor
	AMiniGamePlayerCharacter* MeshMiniPlayerCharacter = Cast<AMiniGamePlayerCharacter>(MeshComp->GetOwner());
	if (nullptr == MeshMiniPlayerCharacter)
	{
		return ;
	}
	TArray<AActor*>& HitActors = MeshMiniPlayerCharacter->GetHitActors();
	
	for (int32 i = 0; i < HitResults.Num(); ++i)
	{
		const FHitResult& HitResult = HitResults[i];

		AActor* HitActor = HitResult.GetHitObjectHandle().FetchActor();
		if (nullptr == HitActor)
		{
			continue;
		}

		if (INDEX_NONE != HitActors.Find(HitActor))
		{
			continue;
		}

		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (nullptr == TargetASC)
		{
			continue;
		}
		
		TargetASC->ApplyGameplayEffectToSelf(GameplayEffectCDO, 1, TargetASC->MakeEffectContext());
		
		HitActors.Emplace(HitActor);
	}
}

bool UMiniGameAnimNotifyState::GetHitResults(USkeletalMeshComponent* MeshComp, TArray<FHitResult>& OutHits) const
{
	bool Result = false;

	Result |= GetLineTraceResults(MeshComp, OutHits);
	Result |= GetSphereTraceResults(MeshComp, OutHits);

	return Result;
}

bool UMiniGameAnimNotifyState::GetLineTraceResults(USkeletalMeshComponent* MeshComp, TArray<FHitResult>& OutHitResult) const
{
	if (nullptr == MeshComp)
	{
		return false;
	}

	TArray<AActor*> ActorsToIgnore { MeshComp->GetOwner() };

	// get character LinePoints
	AMiniGamePlayerCharacter* MeshMiniPlayerCharacter = Cast<AMiniGamePlayerCharacter>(MeshComp->GetOwner());
	if (nullptr == MeshMiniPlayerCharacter)
	{
		return false;
	}

	TArray<FVector>& LinePoints = MeshMiniPlayerCharacter->GetLineMultiPoints();
	if (LinePoints.Num() < PointNumber)
	{
		UpdateCharacterLinePoints(MeshComp, LinePoints);
	}

	TArray<FVector> NewPoints;
	UpdateCharacterLinePoints(MeshComp, NewPoints);

	TArray<FHitResult> LineResults;

	bool Result = false;
	
	for (int32 i = 0; i < PointNumber; ++i)
	{
		if (false == LinePoints.IsValidIndex(i))
		{
			return false;
		}
		
		Result |= UKismetSystemLibrary::LineTraceMultiForObjects(
		MeshComp,
		LinePoints[i],
		NewPoints[i],
		ObjectTypes,
		false,
		ActorsToIgnore,
		bEnableDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		LineResults,
		true
		);
	}

	LinePoints = NewPoints;
	
	OutHitResult.Append(LineResults);

	return Result;
}

void UMiniGameAnimNotifyState::UpdateCharacterLinePoints(USkeletalMeshComponent* MeshComp, TArray<FVector>& LinePoints) const
{
	FVector StartSocketLocation = MeshComp->GetSocketLocation(StartSocketName);
	FVector EndSocketLocation = MeshComp->GetSocketLocation(EndSocketName);

	FVector NormalVector = (EndSocketLocation - StartSocketLocation).GetSafeNormal();
	float UnitLength = (EndSocketLocation - StartSocketLocation).Size() / PointNumber;

	for (int32 i = 0; i < PointNumber; ++i)
	{
		FVector CurrentLocation = StartSocketLocation + NormalVector * UnitLength * i;
		
		if (false == LinePoints.IsValidIndex(i))
		{
			LinePoints.Emplace(CurrentLocation);
			continue;
		}

		LinePoints[i] = CurrentLocation;
	}
}

bool UMiniGameAnimNotifyState::GetSphereTraceResults(USkeletalMeshComponent* MeshComp,
                                                     TArray<FHitResult>& OutHitResult) const
{
	if (nullptr == MeshComp)
	{
		return false;
	}

	TArray<AActor*> ActorsToIgnore { MeshComp->GetOwner() };

	FVector StartSocketLocation = MeshComp->GetSocketLocation(StartSocketName);
	FVector EndSocketLocation = MeshComp->GetSocketLocation(EndSocketName);

	TArray<FHitResult> SphereResults;
	
	bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(
	MeshComp,
	StartSocketLocation,
	EndSocketLocation,
	Radius,
	ObjectTypes,
	false,
	ActorsToIgnore,
	bEnableDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
	SphereResults,
	true);

	OutHitResult.Append(SphereResults);

	return Result;
}
