// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Monster/LLL_BossStatusWidget.h"

#include "Components/ProgressBar.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"

void ULLL_BossStatusWidget::UpdateWidgetView(const ULLL_CharacterAttributeSetBase* CharacterAttributeSet)
{
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(CharacterAttributeSet);
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
