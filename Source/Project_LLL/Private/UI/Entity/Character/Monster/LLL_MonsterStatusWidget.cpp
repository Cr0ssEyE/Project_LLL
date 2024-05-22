// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Monster/LLL_MonsterStatusWidget.h"

#include "Components/ProgressBar.h"
#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"

void ULLL_MonsterStatusWidget::UpdateWidgetView(const ULLL_CharacterAttributeSetBase* CharacterAttributeSet)
{
	Super::UpdateWidgetView(CharacterAttributeSet);

	const float MaxShield = CharacterAttributeSet->GetMaxShield();
	const float CurrentShield = CharacterAttributeSet->GetCurrentShield();

	if(MaxShield)
	{
		ShieldGaugeBar->SetPercent(CurrentShield / MaxShield);
	}
	else
	{
		ShieldGaugeBar->SetPercent(0.f);
	}
}
