// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MiniGameAnimNotifyState_SpawnActor.generated.h"

class AMiniGameActor_Smears;
/**
 * 
 */
UCLASS()
class MINIGAME_API UMiniGameAnimNotify_SpawnActor : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMiniGameActor_Smears> SpawnActorClass;
};
