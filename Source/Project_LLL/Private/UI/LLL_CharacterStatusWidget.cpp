// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LLL_CharacterStatusWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void ULLL_CharacterStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void ULLL_CharacterStatusWidget::UpdateWidgetView(float MaxHealth, float CurrentHealth, float MaxShield, float CurrentShield) const
{
	if(MaxHealth)
	{
		HealthGaugeBar->SetPercent(CurrentHealth / MaxHealth);
	}
	else
	{
		HealthGaugeBar->SetPercent(0.f);
	}
	if(MaxShield)
	{
		ShieldGaugeBar->SetPercent(CurrentShield / MaxShield);
	}
	else
	{
		ShieldGaugeBar->SetPercent(0.f);
	}
}
