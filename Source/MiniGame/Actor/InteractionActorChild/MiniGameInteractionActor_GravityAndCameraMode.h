// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MiniGame/Actor/MiniGameInteractionActor.h"
#include "MiniGameInteractionActor_GravityAndCameraMode.generated.h"

class UCharacterMovementComponent;

UCLASS()
class MINIGAME_API AMiniGameInteractionActor_GravityAndCameraMode : public AMiniGameInteractionActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMiniGameInteractionActor_GravityAndCameraMode(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// the new gravity vector that will be applied
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MiniGame|Overlay|GravityControl")
	FVector NewGravityVector;

private:
	//////////////////////////////
	/// actor interaction function
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	//////////////////////////////
	/// CameraMode
	// tag for camera mode
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MiniGame|Overlay|CameraMode")
	FGameplayTag NewCameraMode;

private:
	void ChangeCameraModeByPawn(AActor* PawnActor) const;

	//////////////////////////////
	/// gravity change
	void ChangeGravityDirectionByActor(const AActor* Actor) const;
};
