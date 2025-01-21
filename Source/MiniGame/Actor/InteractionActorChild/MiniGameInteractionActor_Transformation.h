// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MiniGame/Actor/MiniGameInteractionActor.h"
#include "MiniGameInteractionActor_Transformation.generated.h"

class AMiniGamePlayerCharacter;

UCLASS()
class MINIGAME_API AMiniGameInteractionActor_Transformation : public AMiniGameInteractionActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMiniGameInteractionActor_Transformation(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	///////////////////////////////////
	/// Transformation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MiniGame|Transformation")
	TSubclassOf<AMiniGamePlayerCharacter> TransformCharacterClass;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// override overlap function
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void TransformToNewChracter(AActor* OldCharacter);

	UFUNCTION(BlueprintCallable)
	void SpawnAndPossessNewCharacter(AActor* OldCharacter);
};
