// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MiniGameAnimNotifyState.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class MINIGAME_API UMiniGameAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGameAnimation")
	FName StartSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGameAnimation")
	FName EndSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGameAnimation")
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGameAnimation")
	float PointNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGameAnimation")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGameAnimation")
	bool bEnableDebug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGameAnimation")
	TSubclassOf<UGameplayEffect> GameplayEffect;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	void SweepActorAndApplyGameEffect(USkeletalMeshComponent* MeshComp) const;
	
	bool GetHitResults(USkeletalMeshComponent* MeshComp, TArray<FHitResult>& OutHits) const;

	bool GetLineTraceResults(USkeletalMeshComponent* MeshComp, TArray<FHitResult>& OutHitResult) const;

	void UpdateCharacterLinePoints(USkeletalMeshComponent* MeshComp, TArray<FVector>& LinePoints) const;

	bool GetSphereTraceResults(USkeletalMeshComponent* MeshComp, TArray<FHitResult>& OutHitResult) const;
};
