// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/LLL_PlayerStatusWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void ULLL_PlayerStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void ULLL_PlayerStatusWidget::UpdateWidgetView(int MaxHealth, int CurrentHealth, int MaxShield, int CurrentShield) const
{
	if(MaxHealth)
	{
		HealthGaugeBar->SetPercent(CurrentHealth / MaxHealth);
		HealthTextBlock->SetText(FText::FromString(FString::FromInt(CurrentHealth).Append(TEXT(" / ")).Append(FString::FromInt(MaxHealth))));
	}
	else
	{
		HealthGaugeBar->SetPercent(0.f);
		HealthTextBlock->SetText(FText::FromString(TEXT("!!! MAX HEALTH ERROR !!!")));
	}
	if(MaxShield)
	{
		ShieldGaugeBar->SetPercent(CurrentShield / MaxShield);
		ShieldTextBlock->SetText(FText::FromString(FString::FromInt(CurrentShield).Append(TEXT(" / ")).Append(FString::FromInt(MaxShield))));
	}
	else
	{
		ShieldGaugeBar->SetPercent(0.f);
		ShieldTextBlock->SetText(FText::FromString(TEXT("!!! MAX SHIELD ERROR !!!")));
	}
}

