// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiniGameInteractionActor.generated.h"

class UMiniGameAbilitySet;
class UCurveVector;
class UBoxComponent;

UENUM()
enum EMovementType : uint8
{
	Location,
	Rotation,
	Scale
};

USTRUCT(BlueprintType)
struct FActorMovementStruct
{
	GENERATED_BODY()

	// Movement Curve
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveVector> Curve;

	// loop play the curve ?
	UPROPERTY(EditAnywhere)
	uint8 bLoopPlay : 1;

	// movement duration time
	UPROPERTY(EditAnywhere)
	float MovementDuration;

	FActorMovementStruct()
	{
		bLoopPlay = true;
		MovementDuration = 0.0f;
	};
};

UCLASS()
class MINIGAME_API AMiniGameInteractionActor : public AActor
{
	GENERATED_BODY()

public:
	/** Class constructor */
	AMiniGameInteractionActor(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// actor static mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractionActor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	// actor box comp for interaction with player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractionActor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComponent;

	///////////////////
	/// Details of Actor's Cyclic Motion

	// movement of location 
	UPROPERTY(EditAnywhere, Category="MiniGame|ActorMovement"/*, meta=(TitleProperty=Movement)*/)
	FActorMovementStruct LocationMovementStruct;

	// movement of rotation
	UPROPERTY(EditAnywhere, Category="MiniGame|ActorMovement")
	FActorMovementStruct RotationMovementStruct;

	// movement of scale
	UPROPERTY(EditAnywhere, Category="MiniGame|ActorMovement")
	FActorMovementStruct ScaleMovementStruct;

	////////////////////
	/// interaction target
	UPROPERTY(EditAnywhere, Category="MiniGame|Overlay")
	TSubclassOf<APawn> OverlayTargetClass;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FTransform MeshOriginalTransform;
	
	bool bLocationMovementIsOvered;

	bool bRotationMovementIsOvered;

	bool bScaleMovementIsOvered;

	float CurrentActorMovementPlayTime_Location;

	float CurrentActorMovementPlayTime_Rotation;

	float CurrentActorMovementPlayTime_Scale;

	void UpdateMovementCurve(EMovementType MovementType, const float& DeltaTime,
							 const FActorMovementStruct& MovementStruct,
							 float& CurrentPlayTime, bool& bMovementIsOvered) const;

	
	//////////////////////////////
	/// actor interaction function
	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
