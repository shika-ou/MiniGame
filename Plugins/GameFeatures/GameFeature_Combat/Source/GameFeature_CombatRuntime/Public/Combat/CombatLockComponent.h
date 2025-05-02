// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CombatLockComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEFEATURE_COMBATRUNTIME_API UCombatLockComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatLockComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// max lock distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Lock")
	float LockOnDistance;

	// Debug
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Lock")
	bool bEnableDebug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Lock")
	float CameraInterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Lock")
	float CameraPitch;

	// is locking
	UPROPERTY(BlueprintReadOnly, Category = "Combat Lock")
	bool bIsLockOn;

	// locking target
	UPROPERTY(BlueprintReadOnly, Category = "Combat Lock")
	TObjectPtr<AActor> LockedTarget;

	UFUNCTION(BlueprintCallable, Category = "Combat Lock")
	void SearchAndLockTarget();

	UFUNCTION(BlueprintCallable, Category = "Combat Lock")
	void UnlockTarget();

	UFUNCTION(BlueprintCallable, Category = "Combat Lock")
	void UpdateCameraRotation(float DeltaTime);
	
	void UpdateLockTarget(AActor* NewLockedTarget);
	
	UFUNCTION(BlueprintCallable, Category = "Combat Lock")
	AActor* GetLockedTarget() const;
};
