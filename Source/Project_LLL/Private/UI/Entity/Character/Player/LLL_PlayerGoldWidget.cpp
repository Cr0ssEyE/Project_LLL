// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Player/LLL_PlayerGoldWidget.h"
#include "Components/TextBlock.h"

void ULLL_PlayerGoldWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULLL_PlayerGoldWidget::UpdateInitWidget(float InGold) const
{
	if (!InitGoldTextBlock)
	{
		return;
	}
	
	if (InGold > 0)
	{
		InitGoldTextBlock->SetText(FText::FromString(FString(TEXT("+")).Append(FString::FromInt(InGold))));
	}
	else
	{
		InitGoldTextBlock->SetText(FText::FromString(FString::FromInt(InGold)));
	}
}

void ULLL_PlayerGoldWidget::UpdateGoldWidget(float TotalGold) const
{
	GoldTextBlock->SetText(FText::FromString(FString::FromInt(TotalGold)));
}
