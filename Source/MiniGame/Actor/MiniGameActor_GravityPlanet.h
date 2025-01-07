// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MiniGameActor_GravityPlanet.generated.h"

class AMiniGamePlayerCharacter;
class UCapsuleComponent;

UCLASS()
class MINIGAME_API AMiniGameActor_GravityPlanet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMiniGameActor_GravityPlanet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//////////////////////////////
	/// capsule component
	///
protected:
	// Capsule Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	// the function at the overlap begin/end
	UFUNCTION()
	virtual void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	
	// the pointer for MiniGame Character
	TObjectPtr<AMiniGamePlayerCharacter> PlayerCharacter;

	// update player gravity direction by planet
	void UpdatePlayerGravityDirectionByPlanet() const;
	
	// the function for get the unit direction between the player and planet
	FVector GetUnitGravity() const;
};
