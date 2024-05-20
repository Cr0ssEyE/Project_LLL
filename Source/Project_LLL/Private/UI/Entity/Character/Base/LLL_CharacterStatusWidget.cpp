// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Base/LLL_CharacterStatusWidget.h"

#include "Components/ProgressBar.h"
#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"

void ULLL_CharacterStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void ULLL_CharacterStatusWidget::UpdateWidgetView(const ULLL_CharacterAttributeSetBase* CharacterAttributeSet) const
{
	const float MaxHealth = CharacterAttributeSet->GetMaxHealth();
	const float CurrentHealth = CharacterAttributeSet->GetCurrentHealth();
	
	if(MaxHealth)
	{
		HealthGaugeBar->SetPercent(CurrentHealth / MaxHealth);
	}
	else
	{
		HealthGaugeBar->SetPercent(0.f);
	}
	
}
