// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/LLL_PlayerChaseActionWidget.h"
#include "Util/LLLConstructorHelper.h"
#include "Components/Image.h"
#include <Constant/LLL_FilePath.h>

void ULLL_PlayerChaseActionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CircleProgressBarInst = FLLLConstructorHelper::FindAndGetObject<UMaterialInstanceDynamic>(PATH_UI_PROGRESSBAR_INST, EAssertionLevel::Check);
	
	CircleProgressBar->SetBrushFromMaterial(CircleProgressBarInst);
}

void ULLL_PlayerChaseActionWidget::SetCircleProgressBarValue(const float value)
{
	CircleProgressBarInst->SetScalarParameterValue(FName(TEXT("Percent")), value);
}
