// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameFunctionLibrary.h"

bool UMiniGameFunctionLibrary::MiniGame_CreateSphereTraceSingForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{	
	// Process sphere logic
	return UKismetSystemLibrary::SphereTraceMultiForObjects(
		WorldContextObject,
		Start,
		End,
		Radius,
		ObjectTypes,
		bTraceComplex,
		ActorsToIgnore,
		DrawDebugType,
		OutHits,
		bIgnoreSelf,
		TraceColor,
		TraceHitColor,
		DrawTime
		);
}
