// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MiniGame/Actor/MiniGameInteractionActor.h"
#include "MiniGameInteractionActor_GiveAbilitySystem.generated.h"

UCLASS()
class MINIGAME_API AMiniGameInteractionActor_GiveAbilitySystem : public AMiniGameInteractionActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMiniGameInteractionActor_GiveAbilitySystem(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	/////////////////////
	/// give Ability to character
	// Ability Set to grant to the pawn on initialization
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MiniGame|Abilities")
	TObjectPtr<UMiniGameAbilitySet> AbilitySet;
	
private:
	//////////////////////////////
	/// actor interaction function
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
