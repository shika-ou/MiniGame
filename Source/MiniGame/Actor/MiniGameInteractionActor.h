// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiniGameInteractionActor.generated.h"

class UNiagaraSystem;
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

	// indicates whether the actor should be destroyed after interaction.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame|Destory", meta = (AllowPrivateAccess = "true"))
	bool bShouldDestroyActorAfterInterAction;

	// the effect in the destroy actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame|Destory", meta = (AllowPrivateAccess = "true", EditCondition="bShouldDestroyActorAfterInterAction", true))
	TObjectPtr<UNiagaraSystem> DestroyEffect;

	// indicates whether the target actor should change the skin.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame|ChangeSkin", meta = (AllowPrivateAccess = "true"))
	bool bShouldChangeSkinAfterOverlay;

	// target skin index
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame|ChangeSkin", meta = (AllowPrivateAccess = "true", EditCondition="bShouldChangeSkinAfterOverlay", true))
	uint8 SkinIndex;
	
	///////////////////////////////
	/// component
	// actor static mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	// actor box comp for interaction with player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComponent;

	///////////////////
	/// Details of Actor's Cyclic Motion

	UPROPERTY(EditAnywhere, Category="MiniGame|ActorMovement"/*, meta=(TitleProperty=Movement)*/)
	bool bEnableActorMovement;
	
	// movement of location 
	UPROPERTY(EditAnywhere, Category="MiniGame|ActorMovement", meta=(EditCondition="bEnableActorMovement", true))
	FActorMovementStruct LocationMovementStruct;

	// movement of rotation
	UPROPERTY(EditAnywhere, Category="MiniGame|ActorMovement", meta=(EditCondition="bEnableActorMovement", true))
	FActorMovementStruct RotationMovementStruct;

	// movement of scale
	UPROPERTY(EditAnywhere, Category="MiniGame|ActorMovement", meta=(EditCondition="bEnableActorMovement", true))
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

	void UpdateActorMovement(float DeltaTime);
	
	void UpdateMovementCurve(EMovementType MovementType, const float& DeltaTime,
							 const FActorMovementStruct& MovementStruct,
							 float& CurrentPlayTime, bool& bMovementIsOvered) const;


protected:
	//////////////////////////////
	/// actor interaction function
	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
