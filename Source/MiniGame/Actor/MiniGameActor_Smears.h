// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiniGameActor_Smears.generated.h"

class UTimelineComponent;
class AMiniGamePlayerCharacter;
class UPoseableMeshComponent;

UCLASS()
class MINIGAME_API AMiniGameActor_Smears : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMiniGameActor_Smears();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TObjectPtr<AMiniGamePlayerCharacter> PlayerCharacter;

	

	TObjectPtr<USceneComponent> CustomRootComponent;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MnigGmae|Smears", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPoseableMeshComponent> PoseableMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MnigGmae|Smears", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTimelineComponent> TimelineComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MiniGame|Smears")
	TObjectPtr<UMaterial> PoseMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MiniGame|Smears")
	TObjectPtr<UCurveFloat> AlphaCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MiniGame|Smears")
	float TimeLineLength;

	UFUNCTION()
	void OnTimelineUpdate(float NewFloat);

	UFUNCTION()
	void OnTimelineFinish();
	
public:
	UFUNCTION(BlueprintCallable, Category = "MiniGame|Smears")
	void ChangeEffectAndDestroy(AMiniGamePlayerCharacter* TargetPlayerCharacter);

private:
	void SetPoseableMeshMaterial(const FString& MaterialParameterName, float ParameterFloat = 0.0f);
};
