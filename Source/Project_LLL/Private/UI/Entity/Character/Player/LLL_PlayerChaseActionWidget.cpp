// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Player/LLL_PlayerChaseActionWidget.h"
#include "Util/LLL_ConstructorHelper.h"
#include "Components/Image.h"
#include <Constant/LLL_FilePath.h>

void ULLL_PlayerChaseActionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CircleProgressBarInst = FLLL_ConstructorHelper::FindAndGetObject<UMaterialInstanceDynamic>(PATH_UI_PROGRESSBAR_INST, EAssertionLevel::Check);
	
	CircleProgressBar->SetBrushFromMaterial(CircleProgressBarInst);
}

void ULLL_PlayerChaseActionWidget::SetCircleProgressBarValue(const float value)
{
	CircleProgressBarInst->SetScalarParameterValue(FName(TEXT("Percent")), value);
}
