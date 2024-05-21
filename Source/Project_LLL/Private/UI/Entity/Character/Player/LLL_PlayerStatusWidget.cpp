// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Player/LLL_PlayerStatusWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerSkillAttributeSet.h"

void ULLL_PlayerStatusWidget::UpdateWidgetView(const ULLL_CharacterAttributeSetBase* CharacterAttributeSet)
{
	Super::UpdateWidgetView(CharacterAttributeSet);
	const float MaxHealth = CharacterAttributeSet->GetMaxHealth();
	const float CurrentHealth = CharacterAttributeSet->GetCurrentHealth();

	if(MaxHealth)
	{
		HealthTextBlock->SetText(FText::FromString(FString::FromInt(CurrentHealth).Append(TEXT(" / ")).Append(FString::FromInt(MaxHealth))));
	}
	else
	{
		HealthTextBlock->SetText(FText::FromString(TEXT("!!! MAX HEALTH ERROR !!!")));
	}
}

void ULLL_PlayerStatusWidget::UpdateWidgetView(const UAbilitySystemComponent* CharacterASC) const
{
	const ULLL_PlayerSkillAttributeSet* SkillAttributeSet = Cast<ULLL_PlayerSkillAttributeSet>(CharacterASC->GetAttributeSet(ULLL_PlayerSkillAttributeSet::StaticClass()));
	const float MaxSkillGauge = SkillAttributeSet->GetMaxSkillGauge();
	const float CurrentSkillGauge = SkillAttributeSet->GetCurrentSkillGauge();

	if(MaxSkillGauge > 0.f)
	{
		SkillGaugeBar->SetPercent(CurrentSkillGauge / MaxSkillGauge);
		SkillTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), SkillGaugeBar->GetPercent() * 100.f).Append(TEXT("%"))));
	}
}
