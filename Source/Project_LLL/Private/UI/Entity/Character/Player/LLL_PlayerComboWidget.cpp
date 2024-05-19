// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Player/LLL_PlayerComboWidget.h"

#include "Components/TextBlock.h"

void ULLL_PlayerComboWidget::SetComboText(float value)
{
	if (value != FCString::Atof(*ComboCount->GetText().ToString()))
	{
		PlayComboTextAnimation();
	}
	ComboCount->SetText(FText::FromString(FString::FromInt(value)));
}

void ULLL_PlayerComboWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
