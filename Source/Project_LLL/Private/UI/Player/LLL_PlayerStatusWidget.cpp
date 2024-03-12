// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/LLL_PlayerStatusWidget.h"

#include "Components/TextBlock.h"

void ULLL_PlayerStatusWidget::UpdateWidgetView(float MaxHealth, float CurrentHealth, float MaxShield, float CurrentShield) const
{
	Super::UpdateWidgetView(MaxHealth, CurrentHealth, MaxShield, CurrentShield);

	if(MaxHealth)
	{
		HealthTextBlock->SetText(FText::FromString(FString::FromInt(CurrentHealth).Append(TEXT(" / ")).Append(FString::FromInt(MaxHealth))));
	}
	else
	{
		HealthTextBlock->SetText(FText::FromString(TEXT("!!! MAX HEALTH ERROR !!!")));
	}
	if(MaxShield)
	{
		ShieldTextBlock->SetText(FText::FromString(FString::FromInt(CurrentShield).Append(TEXT(" / ")).Append(FString::FromInt(MaxShield))));
	}
	else
	{
		ShieldTextBlock->SetText(FText::FromString(TEXT("!!! MAX SHIELD ERROR !!!")));
	}
}
