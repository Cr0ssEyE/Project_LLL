// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Player/LLL_PlayerGoldWidget.h"
#include "Components/TextBlock.h"

void ULLL_PlayerGoldWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//GoldTextBlock = Cast<UTextBlock>(GetWidgetFromName(FName(FString::Printf("GoldText"))));
}

void ULLL_PlayerGoldWidget::UpdateWidget(float InGold) const
{
	GoldTextBlock->SetText(FText::FromString(FString::FromInt(InGold)));
}
