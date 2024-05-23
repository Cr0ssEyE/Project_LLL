// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Monster/LLL_MonsterStatusWidget.h"

#include "Components/ProgressBar.h"
#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"
#include "GAS/Attribute/Character/Monster/Base/LLL_MonsterAttributeSet.h"

void ULLL_MonsterStatusWidget::UpdateWidgetView(const ULLL_CharacterAttributeSetBase* CharacterAttributeSet)
{
	Super::UpdateWidgetView(CharacterAttributeSet);

	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(CharacterAttributeSet);
	const float MaxShield = MonsterAttributeSet->GetMaxShield();
	const float CurrentShield = MonsterAttributeSet->GetCurrentShield();

	if(MaxShield)
	{
		ShieldGaugeBar->SetPercent(CurrentShield / MaxShield);
	}
	else
	{
		ShieldGaugeBar->SetPercent(0.f);
	}
}
