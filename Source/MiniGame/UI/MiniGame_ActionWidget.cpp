// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGame_ActionWidget.h"
#include "CommonActionWidget.h"
#include "CommonTextBlock.h"


void UMiniGame_ActionWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	check(ActionWidget);
	ActionWidget->SetEnhancedInputAction(InputAction);

	check(ActionLabel);
	ActionLabel->SetText(ActionText);
}
