// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LLL_CharacterStatusWidget.h"

#include "Components/ProgressBar.h"
#include "GAS/Attribute/Base/LLL_CharacterAttributeSetBase.h"

void ULLL_CharacterStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void ULLL_CharacterStatusWidget::UpdateWidgetView(const ULLL_CharacterAttributeSetBase* CharacterAttributeSet) const
{
	const float MaxHealth = CharacterAttributeSet->GetMaxHealth();
	const float CurrentHealth = CharacterAttributeSet->GetCurrentHealth();
	const float MaxShield = CharacterAttributeSet->GetMaxShield();
	const float CurrentShield = CharacterAttributeSet->GetCurrentShield();
	
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
