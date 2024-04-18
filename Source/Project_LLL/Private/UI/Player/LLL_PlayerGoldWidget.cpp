// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/LLL_PlayerGoldWidget.h"
#include "Components/TextBlock.h"

void ULLL_PlayerGoldWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULLL_PlayerGoldWidget::UpdateInitWidget(float InGold) const
{
	InitGoldTextBlock->SetText(FText::FromString(FString::FromInt(InGold)));
}

void ULLL_PlayerGoldWidget::UpdateGoldWidget(float TotalGold) const
{
	GoldTextBlock->SetText(FText::FromString(FString::FromInt(TotalGold)));
}
