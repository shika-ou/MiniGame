// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OKAMI_PaintWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEFEATURE_OKAMIRUNTIME_API UOKAMI_PaintWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
