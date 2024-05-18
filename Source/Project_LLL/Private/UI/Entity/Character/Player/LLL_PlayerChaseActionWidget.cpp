// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Player/LLL_PlayerChaseActionWidget.h"
#include "Components/Image.h"

void ULLL_PlayerChaseActionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CircleProgressBarInstDynamic = CircleProgressBar->GetDynamicMaterial();
}

void ULLL_PlayerChaseActionWidget::SetCircleProgressBarValue(const float value)
{
	CircleProgressBarInstDynamic->SetScalarParameterValue(FName(TEXT("Percent")), value);
}
