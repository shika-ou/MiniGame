// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameGameMode.h"

#include "MiniGamePlayerController.h"
#include "MiniGamePlayerStart.h"

void AMiniGameGameMode::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	Super::RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
}
