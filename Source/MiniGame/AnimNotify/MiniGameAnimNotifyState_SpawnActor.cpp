// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameAnimNotifyState_SpawnActor.h"

#include "MiniGame/Actor/MiniGameActor_Smears.h"
#include "MiniGame/Character/MiniGamePlayerCharacter.h"

void UMiniGameAnimNotify_SpawnActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AMiniGamePlayerCharacter* MiniGameCharacter = Cast<AMiniGamePlayerCharacter>(MeshComp->GetOwner());
	if (MiniGameCharacter == nullptr)
	{
		return;
	}

	AMiniGameActor_Smears* SmearsActor = MiniGameCharacter->GetWorld()->SpawnActor<AMiniGameActor_Smears>(SpawnActorClass, MiniGameCharacter->GetActorLocation(), MiniGameCharacter->GetActorRotation());
	if (SmearsActor)
	{
		SmearsActor->ChangeEffectAndDestroy(MiniGameCharacter);
	}
	
}
