// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoverTypes.h"

#include "MiniGameMoverTypes.generated.h"

/**
 * FMiniGameTagsSyncState
 * Extends the Mover sync state to provide gameplay tag tracking.
 */
USTRUCT(BlueprintType)
struct MINIGAMEMOVEMENT_API FMiniGameTagsSyncState : public FMoverDataStructBase
{
	GENERATED_BODY()

public:

	FMiniGameTagsSyncState()
	{
	};

	virtual ~FMiniGameTagsSyncState() {};

public:
	/** Returns the movement tags container */
	const FGameplayTagContainer& GetMovementTags() const { return MovementTags; }

protected:

	/** Tags container */
	FGameplayTagContainer MovementTags;
	
};

/**
 * FMiniGameMovementInputs
 * Input data block for MiniGame movement modes
 */
USTRUCT(BlueprintType)
struct MINIGAMEMOVEMENT_API FMiniGameMovementInputs : public FMoverDataStructBase
{
	GENERATED_USTRUCT_BODY()

	FMiniGameMovementInputs()
	: bIsSprintJustPressed(false)
	, bIsSprintPressed(false)
	, bIsGlideJustPressed(false)
	, bIsGlidePressed(false)
	{
		Wind = FVector::ZeroVector;
	};

	virtual ~FMiniGameMovementInputs(){}
	
public:

	/** Was the Sprint input just pressed? */
	UPROPERTY(BlueprintReadWrite, Category = MiniGame)
	bool bIsSprintJustPressed;

	/** Is the Sprint input held down? */
	UPROPERTY(BlueprintReadWrite, Category = MiniGame)
	bool bIsSprintPressed;

	/** Was the Glide input just pressed? */
	UPROPERTY(BlueprintReadWrite, Category = MiniGame)
	bool bIsGlideJustPressed;

	/** Is the Glide input held down? */
	UPROPERTY(BlueprintReadWrite, Category = MiniGame)
	bool bIsGlidePressed;

	/** Wind speed vector applied while gliding */
	UPROPERTY(BlueprintReadWrite, Category = MiniGame)
	FVector Wind;

	/** FStruct Utility */
	virtual FMoverDataStructBase* Clone() const override;
	
	virtual UScriptStruct* GetScriptStruct() const override { return StaticStruct(); };
};

/**
 * UMiniGameMovementSettings
 * Common Movement settings used by MiniGame movement modes
 */
UCLASS()
class MINIGAMEMOVEMENT_API UMiniGameMovementSettings : public UObject
{
	GENERATED_BODY()
};
